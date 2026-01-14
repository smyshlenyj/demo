#pragma once

#include <memory>
#include <stdexcept>

#include "mathlib.h"
#include "parsedArgs.h"
#include "loggerWrapper.h"

class Calculator
{
   private:
    std::shared_ptr<spdlog::logger> log;

   public:
    Calculator(const std::shared_ptr<spdlog::logger>& logger) : log(logger)
    {
    }
    mathlib::MathResult executeOperation(const ParsedArgs& args);

   private:
    static std::string mathErrorToString(mathlib::MathError error);
};

struct CalculationError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};