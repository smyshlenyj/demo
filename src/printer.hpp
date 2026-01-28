#pragma once

#include <iostream>
#include <memory>

#include "ILogger.hpp"
#include "parsedArgs.hpp"

class Printer
{
   public:
    Printer(std::shared_ptr<ILogger> log) : log_(std::move(log))
    {
    }

    void printResult(ParsedArgs& args, std::int64_t result)
    {
        log_->trace("Entered Printer::printResult");
        std::cout << args.first << " " << args.operation << " " << args.second << " = " << result << '\n';
    }

   private:
    std::shared_ptr<ILogger> log_;
};
