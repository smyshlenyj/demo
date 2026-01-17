#pragma once

#include <string>

class ILogger
{
   public:
    virtual ~ILogger() = default;

    virtual void trace(const std::string& msg) = 0;
    virtual void info(const std::string& msg) = 0;
    virtual void error(const std::string& msg) = 0;
    virtual void critical(const std::string& msg) = 0;
};
