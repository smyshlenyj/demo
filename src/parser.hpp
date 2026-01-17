#pragma once

#include <string>
#include <memory>
#include <stdexcept>
#include <fstream>
#include <nlohmann/json.hpp>

#include "parsedArgs.hpp"
#include "ILogger.hpp"

struct ParseError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class Parser
{
   public:
    Parser(std::shared_ptr<ILogger> log) : log_(std::move(log))
    {
    }

    ParsedArgs parse(int argc, const std::string& path)
    {
        log_->trace("Entered Parser::parse with file: {}" + path);

        if (argc < 2)
        {
            log_->error("Input file path is not provided");
        }

        std::ifstream file(path);
        if (!file.is_open())
        {
            std::string error = "Cannot open input file";
            log_->error(error);
            throw ParseError(error);
        }

        nlohmann::json json;

        try
        {
            file >> json;
        }
        catch (const nlohmann::json::parse_error&)
        {
            std::string error = "Invalid JSON format";
            log_->error(error);
            throw ParseError(error);
        }

        log_->trace("Input JSON: {}" + json.dump());
        ParsedArgs result{};

        try
        {
            result.first = json.at("firstOperand").get<std::int64_t>();
            result.second = json.at("secondOperand").get<std::int64_t>();

            const auto operation = json.at("operation").get<std::string>();
            if (operation.size() != 1)
            {
                std::string error = "Operation must be a single character";
                log_->error(error);
                throw ParseError(error);
            }

            result.operation = operation[0];
        }

        catch (const nlohmann::json::exception&)
        {
            std::string error = "Invalid or missing JSON fields";
            log_->error(error);
            throw ParseError(error);
        }

        return result;
    }

   private:
    std::shared_ptr<ILogger> log_;
};
