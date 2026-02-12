#include <gtest/gtest.h>
#include <grpcpp/grpcpp.h>

#include "calc.grpc.pb.h"
#include "calculator.hpp"
#include "converters.hpp"
#include "loggerWrapper.hpp"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using calc::CalcRequest;
using calc::CalcResponse;

class DummyLogger final : public ILogger
{
   public:
    ~DummyLogger() override = default;

    void trace(const std::string& /*msg*/) noexcept override
    {
    }
    void info(const std::string& /*msg*/) noexcept override
    {
    }
    void error(const std::string& /*msg*/) noexcept override
    {
    }
    void critical(const std::string& /*msg*/) noexcept override
    {
    }
};

// -----------------------------
// gRPC Server implementation
// -----------------------------
class CalculatorServiceImpl final : public calc::Calculator::Service
{
   public:
    explicit CalculatorServiceImpl(std::shared_ptr<ILogger> logger) : logger_(std::move(logger))
    {
    }

    Status Compute(ServerContext* /*context*/, const calc::CalcRequest* request, calc::CalcResponse* response) override
    {
        try
        {
            Operation operation = toLocalOperation(request->op());
            auto result = Calculator::executeOperation(request->a(), request->b(), operation, logger_);
            response->set_result(result);
            response->set_error("");
            return Status::OK;
        }
        catch (const std::exception& e)
        {
            response->set_error(e.what());
            return Status::OK;
        }
    }

   private:
    std::shared_ptr<ILogger> logger_;
};

// -----------------------------
// Fixture for gRPC tests
// -----------------------------
class GRPCCalculatorTest : public ::testing::Test
{
   protected:
    void SetUp() override
    {
        service = std::make_unique<CalculatorServiceImpl>(std::make_shared<DummyLogger>());

        ServerBuilder builder;
        builder.RegisterService(service.get());
        server = builder.BuildAndStart();
        ASSERT_NE(server, nullptr);

        channel = server->InProcessChannel(grpc::ChannelArguments());
        stub = calc::Calculator::NewStub(channel);
    }

    void TearDown() override
    {
        server->Shutdown();
    }

    std::unique_ptr<CalculatorServiceImpl> service;
    std::unique_ptr<calc::Calculator::Stub> stub;
    std::shared_ptr<grpc::Channel> channel;
    std::unique_ptr<Server> server;
};

// -----------------------------
// Operation tests via gRPC
// -----------------------------
TEST_F(GRPCCalculatorTest, Addition)
{
    CalcRequest request;
    request.set_a(2);
    request.set_b(3);
    request.set_op(toGrpcOperation(Operation::ADD));

    CalcResponse response;
    grpc::ClientContext context;
    grpc::Status status = stub->Compute(&context, request, &response);

    ASSERT_TRUE(status.ok());
    EXPECT_EQ(response.result(), 5);
    EXPECT_EQ(response.error(), "");
}

TEST_F(GRPCCalculatorTest, DivisionByZero)
{
    CalcRequest request;
    request.set_a(10);
    request.set_b(0);
    request.set_op(toGrpcOperation(Operation::DIV));

    CalcResponse response;
    grpc::ClientContext context;
    grpc::Status status = stub->Compute(&context, request, &response);

    ASSERT_TRUE(status.ok());
    EXPECT_NE(response.error(), "");
}

TEST_F(GRPCCalculatorTest, Factorial)
{
    CalcRequest request;
    request.set_a(5);
    request.set_b(0);  // ignored for FACT
    request.set_op(toGrpcOperation(Operation::FACT));

    CalcResponse response;
    grpc::ClientContext context;
    grpc::Status status = stub->Compute(&context, request, &response);

    ASSERT_TRUE(status.ok());
    EXPECT_EQ(response.result(), 120);
    EXPECT_EQ(response.error(), "");
}

TEST_F(GRPCCalculatorTest, Power)
{
    CalcRequest request;
    request.set_a(3);
    request.set_b(4);
    request.set_op(toGrpcOperation(Operation::POW));

    CalcResponse response;
    grpc::ClientContext context;
    grpc::Status status = stub->Compute(&context, request, &response);

    ASSERT_TRUE(status.ok());
    EXPECT_EQ(response.result(), 81);
    EXPECT_EQ(response.error(), "");
}
