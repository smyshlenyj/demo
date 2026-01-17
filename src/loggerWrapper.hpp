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
        static std::shared_ptr<spdlog::logger> loggerInstance;

        if (!spdlog::get("app"))
        {
            try
            {
                loggerInstance = spdlog::basic_logger_mt("app", "app.log");
                loggerInstance->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
                loggerInstance->set_level(spdlog::level::trace);
                loggerInstance->flush_on(spdlog::level::trace);

                // loggerInstance->info("Logger initialized at: {}", logPath.string());
            }

            catch (const spdlog::spdlog_ex& e)
            {
                loggerInstance->error("Logger init failed");
                std::cerr << "Logger init failed: " << e.what() << "\n";
            }
        }
    }

    static std::shared_ptr<ILogger> get()
    {
        static std::shared_ptr<ILogger> instance = []()
        {
            auto spd = spdlog::get("app");
            if (!spd) throw std::runtime_error("Logger not initialized");
            return std::make_shared<SpdLogger>(spd);
        }();
        return instance;
    }
};
