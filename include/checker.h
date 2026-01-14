#pragma once

#include <memory>
#include <stdexcept>

#include "parsedArgs.h"
#include "loggerWrapper.h"

class Checker
{
   private:
    std::shared_ptr<spdlog::logger> log;

   public:
    Checker(const std::shared_ptr<spdlog::logger>& logger) : log(logger)
    {
    }
    void checkParsedArgs(const ParsedArgs& args);
};

struct ValidationError : std::logic_error
{
    using std::logic_error::logic_error;
};