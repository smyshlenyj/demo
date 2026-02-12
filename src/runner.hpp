#pragma once

#include <memory>
#include <thread>
#include <string>
#include <csignal>
#include <iostream>

#include <grpcpp/grpcpp.h>
#include "calc.grpc.pb.h"

#include "ILogger.hpp"
#include "loggerWrapper.hpp"
#include "postgresDatabase.hpp"
#include "calculationStorage.hpp"
#include "checker.hpp"
#include "calculator.hpp"
#include "converters.hpp"

// ===== Runner =====
class Runner
{
   public:
    Runner()
    {
        LoggerWrapper::init();
        log_ = LoggerWrapper::get();
        log_->info("Runner ctor called");
    }

    void init()
    {
        const std::string connStr = "host=localhost port=5432 dbname=calc user=calc_user password=calc_pass";
        pgDb_ = std::make_unique<PostgresDatabase>(connStr, log_);
        storage_ = std::make_unique<CalculationStorage>(log_, *pgDb_);
        storage_->warmUp();
    }

    std::int64_t calculate(std::int64_t first, std::int64_t second, calc::Operation protoOperation)
    {
        std::stringstream sstream;
        sstream << std::this_thread::get_id();
        std::string tid = "Thread " + sstream.str() + " Runner:calculate";
        log_->info(tid);

        if (!storage_) throw std::runtime_error("Runner not initialized");

        Operation operation = toLocalOperation(protoOperation);

        Checker::checkOperation(operation, log_);

        auto cached = storage_->getCachedResult(first, second, operation);
        if (cached)
        {
            log_->info("Result found in cache");
            return cached.value();
        }

        std::int64_t result = Calculator::executeOperation(first, second, operation, log_);
        CacheRecord newRecord{first, second, operation, result};
        newRecord.normalize();
        storage_->save(newRecord);

        return result;
    }

    std::shared_ptr<ILogger> getLogger() const
    {
        return log_;
    }

   private:
    std::shared_ptr<ILogger> log_;
    std::unique_ptr<PostgresDatabase> pgDb_;
    std::unique_ptr<CalculationStorage> storage_;
};

// ===== gRPC service =====
class CalculatorServiceImpl final : public calc::Calculator::Service
{
   public:
    explicit CalculatorServiceImpl(const std::shared_ptr<Runner>& runner, const std::shared_ptr<ILogger>& log)
        : runner_(runner), log_(log)
    {
    }

    grpc::Status Compute(grpc::ServerContext* /*context*/, const calc::CalcRequest* request,
                         calc::CalcResponse* reply) override
    {
        if (!request)
        {
            return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "request is nullptr");
        }

        log_->info("request received: a=" + std::to_string(request->a()) + ", b=" + std::to_string(request->b()) +
                   ", op=" + operationToString(toLocalOperation(request->op())));

        try
        {
            std::int64_t result = runner_->calculate(request->a(), request->b(), request->op());
            reply->set_result(result);
            return grpc::Status::OK;
        }
        catch (const std::exception& e)
        {
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }

   private:
    std::shared_ptr<Runner> runner_;
    std::shared_ptr<ILogger> log_;
};

// ===== Server orchestrator =====
class ServerRunner
{
   public:
    void run()
    {
        blockSignals();

        runner_ = std::make_shared<Runner>();
        runner_->init();

        service_ = std::make_shared<CalculatorServiceImpl>(runner_, runner_->getLogger());

        serverThread_ = std::thread(&ServerRunner::serverLoop, this);
        {
            std::unique_lock<std::mutex> lock(startMutex_);
            startCv_.wait(lock, [this] { return server_ != nullptr; });
        }

        signalThread_ = std::thread(&ServerRunner::signalLoop, this);

        signalThread_.join();
        serverThread_.join();
    }

   private:
    void serverLoop()
    {
        std::string serverAddress("0.0.0.0:50051");
        grpc::ServerBuilder builder;
        if (!service_)
        {
            std::cerr << "ERROR: service_ is null!\n";
            std::terminate();
        }
        builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
        builder.RegisterService(service_.get());

        {
            std::lock_guard<std::mutex> lock(startMutex_);
            server_ = builder.BuildAndStart();
            serverReady_ = true;
        }
        startCv_.notify_all();

        std::cout << "gRPC server started on " << serverAddress << "\n";
        server_->Wait();
    }

    void signalLoop()
    {
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGTERM);
        sigaddset(&set, SIGINT);

        int sig;
        sigwait(&set, &sig);
        std::cout << "Signal received: " << sig << ", shutting down server\n";

        std::shared_ptr<grpc::Server> srv;
        {
            std::lock_guard<std::mutex> lock(startMutex_);
            srv = server_;
        }
        if (srv) srv->Shutdown();
    }

    static void blockSignals()
    {
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGTERM);
        sigaddset(&set, SIGINT);
        pthread_sigmask(SIG_BLOCK, &set, nullptr);
    }

   private:
    bool serverReady_ = false;
    std::shared_ptr<grpc::Server> server_;
    std::thread serverThread_;
    std::thread signalThread_;
    std::shared_ptr<CalculatorServiceImpl> service_;
    std::shared_ptr<Runner> runner_;

    std::mutex startMutex_;
    std::condition_variable startCv_;
};