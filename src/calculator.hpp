#pragma once

#include <memory>
#include <stdexcept>

#include "mathlib.h"
#include "parsedArgs.hpp"
#include "loggerWrapper.hpp"

struct CalculationError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class Calculator
{
   private:
    std::shared_ptr<spdlog::logger> log;

   public:
    Calculator(const std::shared_ptr<spdlog::logger>& logger) : log(logger)
    {
    }

    mathlib::MathResult executeOperation(const ParsedArgs& args)
    {
        log->trace("Entered Calculator::executeOperation");

        mathlib::MathResult result{0, mathlib::MathError::None};

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
                throw CalculationError("Unknown operation: " + std::string(1, args.operation));
        }

        if (result.error != mathlib::MathError::None)
        {
            std::string error = mathErrorToString(result.error);
            log->error("Calculation failed: {}", error);
            throw CalculationError("Calculation failed: " + error);
        }

        return result;
    }

   private:
    static std::string mathErrorToString(mathlib::MathError error)
    {
        switch (error)
        {
            case mathlib::MathError::None:
                return "No error";  // useful?
            case mathlib::MathError::DivisionByZero:
                return "Division by zero";
            case mathlib::MathError::NegativeFactorial:
                return "Factorial of negative number";
            case mathlib::MathError::InvalidArgument:
                return "Invalid argument";
            default:
                return "Unknown math error";
        }
    }
};
