#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <cstdint>

#include "mathlib.hpp"
#include "parsedArgs.hpp"
#include "ILogger.hpp"
#include "cacheRecord.hpp"

class Calculator
{
   public:
    Calculator(std::shared_ptr<ILogger> log) : log_(std::move(log))
    {
    }

    std::int64_t executeOperation(const ParsedArgs& args)
    {
        log_->trace("Entered Calculator::executeOperation");

        std::int64_t result;

        switch (args.operation)
        {
            case '+':
                result = mathlib::add(args.first, args.second);
                break;
            case '-':
                result = mathlib::subtract(args.first, args.second);
                break;
            case 'x':
                result = mathlib::multiply(args.first, args.second);
                break;
            case '/':
                result = mathlib::divide(args.first, args.second);
                break;
            case '^':
                result = mathlib::power(mathlib::Base{args.first}, mathlib::Exponent{args.second});
                break;
            case '!':
                result = mathlib::factorial(args.first);
                break;
            default:
                log_->critical(std::string("Unexpected operation reached Calculator: ") + args.operation);
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

    std::shared_ptr<ILogger> log_;
};
