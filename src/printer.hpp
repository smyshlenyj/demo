#pragma once
#include <cinttypes>  // for PRId64
#include <cstdio>

#include "mathlib.h"
#include "loggerWrapper.hpp"

class Printer
{
   private:
    std::shared_ptr<spdlog::logger> log;

   public:
    Printer(const std::shared_ptr<spdlog::logger>& logger) : log(logger)
    {
    }

    void printResult(const mathlib::MathResult& result)
    {
        log->trace("Entered Printer::printResult");

        printf("Result: %" PRId64 "\n", result.value);
    }
};
