#pragma once

#include <string>
#include <memory>
#include <stdexcept>
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

    ParsedArgs parse(const std::string& jsonString)
    {
        log_->trace("Entered Parser::parse with JSON string: " + jsonString);

        if (jsonString.empty())
        {
            std::string error = "Error: Missing JSON argument. Usage: calc '<json_string>'";
            log_->error(error);
            throw ParseError(error);
        }

        nlohmann::json json;

        try
        {
            json = nlohmann::json::parse(jsonString);
        }
        catch (const nlohmann::json::parse_error&)
        {
            std::string error = "Invalid JSON format";
            log_->error(error);
            throw ParseError(error);
        }

        log_->trace("Input JSON: " + json.dump());
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
