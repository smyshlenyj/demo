#pragma once

#include "mathlib.h"
#include "loggerWrapper.h"

class Printer
{
   private:
    std::shared_ptr<spdlog::logger> log;

   public:
    Printer(const std::shared_ptr<spdlog::logger>& logger) : log(logger)
    {
    }
    void printResult(const mathlib::MathResult& result);
    // static const char* errorToString(mathlib::MathError error);
};
