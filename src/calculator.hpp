#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <cstdint>

#include "mathlib.hpp"
#include "ILogger.hpp"
#include "cacheRecord.hpp"
#include "calc.grpc.pb.h"  // сгенерированный gRPC код

class Calculator
{
   public:
    Calculator()
    {
    }

    static std::int64_t executeOperation(std::int64_t first, std::int64_t second, Operation operation,
                                         const std::shared_ptr<ILogger>& log)
    {
        log->trace("Entered Calculator::executeOperation");

        std::int64_t result;

        switch (operation)
        {
            case Operation::ADD:
                result = mathlib::add(first, second);
                break;
            case Operation::SUB:
                result = mathlib::subtract(first, second);
                break;
            case Operation::MUL:
                result = mathlib::multiply(first, second);
                break;
            case Operation::DIV:
                result = mathlib::divide(first, second);
                break;
            case Operation::POW:
                result = mathlib::power(mathlib::Base{first}, mathlib::Exponent{second});
                break;
            case Operation::FACT:
                result = mathlib::factorial(first);
                break;
            default:
                log->critical(std::string("Unexpected operation reached Calculator: "));
                std::terminate();
        }

        // fake workload to demonstrate perf
        burner();

        return result;
    }

   private:
    static void burner()
    {
        volatile std::uint64_t sum = 0;

        for (std::uint64_t i = 0; i < 200'000'000ULL; ++i)
        {
            sum += i;
        }

        // options to prevent optimization
        // asm volatile("" : : "r"(sum) : "memory");

        // std::cout << "Burner sum: " << sum << "\n";
    }

    // std::shared_ptr<ILogger> log_;
};
