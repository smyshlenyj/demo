#pragma once

#include <memory>
#include <stdexcept>

#include "parsedArgs.hpp"
#include "loggerWrapper.hpp"

struct ValidationError : std::logic_error
{
    using std::logic_error::logic_error;
};

class Checker
{
   public:
    Checker(const std::shared_ptr<spdlog::logger>& logger) : log(logger)
    {
    }

    void checkParsedArgs(const ParsedArgs& args)
    {
        log->trace("Entered Checker::checkParsedArgs");

        if (args.operation == '\0') throw ValidationError("Operation is missing");

        switch (args.operation)
        {
            case '+':
            case '-':
            case 'x':
            case '/':
            case '^':
            case '!':
                return;
            default:
                throw ValidationError("Invalid operation: " + std::string(1, args.operation));
        }
    }

   private:
    std::shared_ptr<spdlog::logger> log;
};
