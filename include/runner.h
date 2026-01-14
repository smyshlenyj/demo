#pragma once

#include <memory>
#include <string>
#include <spdlog/logger.h>

#include "parsedArgs.h"

class Runner
{
   private:
    std::shared_ptr<spdlog::logger> log;

   public:
    Runner(const std::shared_ptr<spdlog::logger>& logger) : log(logger)
    {
    }

    void run(const std::string& inputPath);
};
