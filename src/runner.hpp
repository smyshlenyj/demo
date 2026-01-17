#pragma once

#include <memory>
#include <string>
#include <spdlog/logger.h>
#include <stdexcept>

#include "parsedArgs.hpp"
#include "parser.hpp"
#include "checker.hpp"
#include "calculator.hpp"
#include "printer.hpp"

class Runner
{
   public:
    Runner()
    {
    }

    void run(int argc, const std::string& inputPath)
    {
        try
        {
            LoggerWrapper::init();
            auto log = LoggerWrapper::get();
            log->info("=========================");
            log->info("Calc started");
            log->trace("Entered Runner::run");

            Parser parser(log);
            auto parsedArgs = parser.parse(argc, inputPath);

            Checker checker(log);
            checker.checkParsedArgs(parsedArgs);

            Calculator calculator(log);
            auto result = calculator.executeOperation(parsedArgs);

            Printer printer(log);
            printer.printResult(parsedArgs, result);

            log->info("Calc finished successfully");
        }

        catch (const ParseError& e)
        {
            log->error("Parse error: {}", e.what());
            std::cerr << "Error: " << e.what() << "\n";
        }

        catch (const ValidationError& e)
        {
            log->error("Validation error: {}", e.what());
            std::cerr << "Error: " << e.what() << "\n";
        }

        catch (const std::invalid_argument& e)  // mathlib exceptions
        {
            log->error("Calculation error: {}", e.what());
            std::cerr << "Error: " << e.what() << "\n";
        }

        catch (...)
        {
            log->error("Unknown exception");
        }
    }

   private:
    std::shared_ptr<spdlog::logger> log;
};
