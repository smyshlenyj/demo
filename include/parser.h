#pragma once

#include <string>
#include <memory>
#include <stdexcept>
#include <spdlog/logger.h>

#include "parsedArgs.h"

class Parser
{
   private:
    std::shared_ptr<spdlog::logger> log;

   public:
    Parser(const std::shared_ptr<spdlog::logger>& logger) : log(logger)
    {
    }

    ParsedArgs parse(const std::string& path);
};

struct ParseError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
