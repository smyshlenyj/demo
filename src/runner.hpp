#pragma once

#include <memory>
#include <string>
#include <spdlog/logger.h>

#include "parsedArgs.hpp"
#include "parser.hpp"
#include "checker.hpp"
#include "calculator.hpp"
#include "printer.hpp"

class Runner
{
   public:
    Runner(const std::shared_ptr<spdlog::logger>& logger) : log(logger)
    {
    }

    void run(const std::string& inputPath)
    {
        log->trace("Entered Runner::run");

        Parser parser(log);
        auto parsedArgs = parser.parse(inputPath);

        Checker checker(log);
        checker.checkParsedArgs(parsedArgs);

        Calculator calculator(log);
        auto result = calculator.executeOperation(parsedArgs);

        Printer printer(log);
        printer.printResult(result);
    }

   private:
    std::shared_ptr<spdlog::logger> log;
};
