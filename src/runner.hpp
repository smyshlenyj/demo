#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>

#include "parsedArgs.hpp"
#include "ILogger.hpp"
#include "IDatabase.hpp"
#include "loggerWrapper.hpp"
#include "postgresDatabase.hpp"
#include "calculationStorage.hpp"
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

    void run(const std::string& jsonString)
    {
        try
        {
            LoggerWrapper::init();

            log_ = LoggerWrapper::get();
            log_->info("=========================");
            log_->info("Calc started");
            log_->trace("Entered Runner::run");

            PostgresDatabase pgDb("host=localhost port=5432 dbname=calc user=calc_user password=calc_pass", log_);
            log_->trace("Established connection to PostgreSQL database");

            IDatabase& database = pgDb;
            CalculationStorage storage(log_, database);
            log_->trace("CalculationStorage initialized");
            auto warm = storage.warmUp();  // cache warm up
            log_->trace("Cache warmed up");

            Parser parser(log_);
            auto parsedArgs = parser.parse(jsonString);

            Checker checker(log_);
            checker.checkParsedArgs(parsedArgs);

            // --------------------------
            // Check result: cached or not
            // --------------------------
            std::int64_t result;
            auto cached = storage.getCachedResult(warm, parsedArgs);

            if (cached)
            {
                log_->info("Result found in cache");
                result = cached.value();
            }
            else
            {
                Calculator calculator(log_);
                result = calculator.executeOperation(parsedArgs);

                // save to database
                CacheRecord newRecord{parsedArgs.first, parsedArgs.second, parsedArgs.operation, result};
                newRecord.normalize();
                storage.save(newRecord);

                // save to cache ? now is not necessary, because next time it will be read from DB
                warm.push_back(newRecord);
            }

            // Print result
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
