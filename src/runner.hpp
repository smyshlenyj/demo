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
#include "ILogger.hpp"
#include "loggerWrapper.hpp"

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
            // Инициализация spdlog
            LoggerWrapper::init();

            // Создаём адаптер ILogger поверх spdlog
            log_ = LoggerWrapper::get();
            log_->info("=========================");
            log_->info("Calc started");
            log_->trace("Entered Runner::run");

            Parser parser(log_);
            auto parsedArgs = parser.parse(argc, inputPath);

            Checker checker(log_);
            checker.checkParsedArgs(parsedArgs);

            Calculator calculator(log_);
            auto result = calculator.executeOperation(parsedArgs);

            Printer printer(log_);
            printer.printResult(parsedArgs, result);

            log_->info("Calc finished successfully");
        }

        catch (const ParseError& e)
        {
            log_->error(std::string("Parse error: ") + e.what());
            std::cerr << "Error: " << e.what() << "\n";
        }

        catch (const ValidationError& e)
        {
            log_->error(std::string("Validation error: ") + e.what());
            std::cerr << "Error: " << e.what() << "\n";
        }

        catch (const std::invalid_argument& e)  // mathlib exceptions
        {
            log_->error(std::string("Calculation error: ") + e.what());
            std::cerr << "Error: " << e.what() << "\n";
        }

        catch (...)
        {
            log_->error("Unknown exception");
        }
    }

   private:
    std::shared_ptr<ILogger> log_;
};
