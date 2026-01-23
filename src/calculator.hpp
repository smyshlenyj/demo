#pragma once

#include <memory>
#include <stdexcept>
#include <cstdlib>
#include <string>

#include "mathlib.hpp"
#include "parsedArgs.hpp"
#include "ILogger.hpp"

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
                std::terminate();  // last guard
        }

        return result;
    }

   private:
    std::shared_ptr<ILogger> log_;
};
