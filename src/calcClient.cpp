#include <grpcpp/grpcpp.h>
#include "calc.pb.h"
#include "calc.grpc.pb.h"

#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <iostream>
#include <numeric>

using namespace std::chrono;

const std::string HOST = "localhost:50051";

const int NUM_CLIENTS = 2000;
const int REQUESTS_PER_CLIENT = 20;

struct Stats
{
    int total = 0;
    int success = 0;
    int failed = 0;
    std::vector<double> times;
    std::mutex mtx;
} stats;

void client_worker(int client_id)
{
    auto channel = grpc::CreateChannel(HOST, grpc::InsecureChannelCredentials());
    auto stub = calc::Calculator::NewStub(channel);

    for (int i = 0; i < REQUESTS_PER_CLIENT; ++i)
    {
        calc::CalcRequest request;
        request.set_a(7);
        request.set_b(6);
        request.set_op(calc::MUL);  // enum

        calc::CalcResponse response;
        grpc::ClientContext context;

        auto start = high_resolution_clock::now();
        grpc::Status status = stub->Compute(&context, request, &response);
        auto end = high_resolution_clock::now();
        double elapsed = duration<double>(end - start).count();

        std::lock_guard<std::mutex> lock(stats.mtx);
        stats.total++;

        if (!status.ok() || !response.error().empty())
        {
            stats.failed++;
        }
        else
        {
            stats.success++;
        }
        stats.times.push_back(elapsed);

        std::cout << "[Client " << client_id << "] Request " << i
                  << (status.ok() ? (" -> " + std::to_string(response.result())) : " failed") << " (" << elapsed << "s)"
                  << std::endl;
    }
}

int main()
{
    std::vector<std::thread> threads;

    for (int c = 0; c < NUM_CLIENTS; ++c)
    {
        threads.emplace_back(client_worker, c);
    }

    for (auto& th : threads)
    {
        th.join();
    }

    double avg_time =
        stats.times.empty() ? 0.0 : std::accumulate(stats.times.begin(), stats.times.end(), 0.0) / stats.times.size();

    std::cout << "\n=== Stress test summary ===" << std::endl;
    std::cout << "Total requests: " << stats.total << std::endl;
    std::cout << "Successful: " << stats.success << std::endl;
    std::cout << "Failed: " << stats.failed << std::endl;
    std::cout << "Average request time: " << avg_time << "s" << std::endl;

    return 0;
}
