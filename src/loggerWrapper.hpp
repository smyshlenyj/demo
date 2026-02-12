#pragma once

#include <memory>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <iostream>

#include "spdLogger.hpp"
#include "ILogger.hpp"

class LoggerWrapper
{
   public:
    static void init()
    {
        if (!spdlog::get("app"))
        {
            try
            {
                auto logger = spdlog::basic_logger_mt("app", "app.log");
                logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
                logger->set_level(spdlog::level::trace);
                logger->flush_on(spdlog::level::trace);
            }

            catch (const spdlog::spdlog_ex& e)
            {
                std::cerr << "Logger init failed: " << e.what() << "\n";
            }
        }
    }

    static std::shared_ptr<ILogger> get()
    {
        auto spd = spdlog::get("app");
        if (!spd)
        {
            throw std::runtime_error("Logger not initialized");
        }

        return std::make_shared<SpdLogger>(spd);
    }
};
