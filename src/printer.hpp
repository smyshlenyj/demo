#pragma once
#include <cinttypes>  // for PRId64
#include <cstdio>

#include "loggerWrapper.hpp"

class Printer
{
   public:
    Printer(const std::shared_ptr<spdlog::logger>& logger) : log(logger)
    {
    }

    void printResult(ParsedArgs& args, std::int64_t result)
    {
        log->trace("Entered Printer::printResult");
        std::cout << args.first << " " << args.operation << " " << args.second << " = " << result << '\n';
    }

   private:
    std::shared_ptr<spdlog::logger> log;
};
