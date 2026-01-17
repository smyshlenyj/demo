#pragma once

#include "ILogger.hpp"
#include <spdlog/logger.h>

class SpdLogger final : public ILogger
{
   public:
    explicit SpdLogger(std::shared_ptr<spdlog::logger> logger) : logger_(std::move(logger))
    {
    }

    void trace(const std::string& msg) override
    {
        logger_->trace(msg);
    }
    void info(const std::string& msg) override
    {
        logger_->info(msg);
    }
    void error(const std::string& msg) override
    {
        logger_->error(msg);
    }

    void critical(const std::string& msg) override
    {
        logger_->critical(msg);
    }

   private:
    std::shared_ptr<spdlog::logger> logger_;
};
