#pragma once

#include <memory>
#include <stdexcept>
#include <string>

#include "ILogger.hpp"
#include "operation.hpp"

struct ValidationError : std::logic_error
{
    using std::logic_error::logic_error;
};

class Checker
{
   public:
    explicit Checker()
    {
    }

    static void checkOperation(Operation operation, const std::shared_ptr<ILogger>& log)
    {
        log->trace("Entered Checker::checkOperation");

        switch (operation)
        {
            case Operation::ADD:
            case Operation::SUB:
            case Operation::MUL:
            case Operation::DIV:
            case Operation::POW:
            case Operation::FACT:
                return;
            default:
                throw ValidationError("Invalid operation");
        }
    }

   private:
    // std::shared_ptr<ILogger> log_;
};
