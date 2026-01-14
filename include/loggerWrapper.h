#pragma once

#include <filesystem>
#include <memory>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

class LoggerWrapper
{
   public:
    static void init();
    static std::shared_ptr<spdlog::logger> get();

   private:
    static std::filesystem::path getExecutableDir();
};
