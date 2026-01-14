#include <fstream>

#include <unistd.h>
#include <limits.h>
#include <iostream>

#include "loggerWrapper.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

void LoggerWrapper::init()
{
    static std::shared_ptr<spdlog::logger> loggerInstance;

    if (!spdlog::get("app"))
    {
        try
        {
            // путь к бинарю теперь определяется методом класса
            std::filesystem::path logPath = getExecutableDir() / "app.log";

            // проверка возможности записи
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
            std::cerr << "Logger init failed: " << e.what() << "\n";
        }
        catch (const std::exception& e)
        {
            std::cerr << "Logger init failed: " << e.what() << "\n";
        }
    }
}

std::shared_ptr<spdlog::logger> LoggerWrapper::get()
{
    return spdlog::get("app");
}

// --- private static method класса ---
std::filesystem::path LoggerWrapper::getExecutableDir()
{
    char buf[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (len == -1) throw std::runtime_error("Cannot determine executable path");
    buf[len] = '\0';
    std::filesystem::path exePath(buf);
    return exePath.parent_path();
}
