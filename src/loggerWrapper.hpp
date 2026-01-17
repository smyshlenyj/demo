#pragma once

#include <filesystem>
#include <memory>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <fstream>
#include <unistd.h>
#include <limits.h>
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
                std::filesystem::path logPath = getExecutableDir() / "app.log";

                std::ofstream test(logPath.string(), std::ios::app);
                if (!test.is_open())
                {
                    std::cerr << "Logger init failed: cannot open " << logPath << " for writing\n";
                    return;
                }

                loggerInstance = spdlog::basic_logger_mt("app", logPath.string());
                loggerInstance->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
                loggerInstance->set_level(spdlog::level::trace);
                loggerInstance->flush_on(spdlog::level::trace);

                loggerInstance->info("Logger initialized at: {}", logPath.string());
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

   private:
    static std::filesystem::path getExecutableDir()
    {
        char buf[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
        if (len == -1) throw std::runtime_error("Cannot determine executable path");
        buf[len] = '\0';
        std::filesystem::path exePath(buf);
        return exePath.parent_path();
    }
};
