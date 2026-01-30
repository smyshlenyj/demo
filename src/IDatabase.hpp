#pragma once

#include <string>
#include <vector>

struct QueryResultRow
{
    std::vector<std::string> columns;  // column values as strings
};

class IDatabase
{
   public:
    virtual ~IDatabase() = default;

    // Execute a query without result (INSERT, UPDATE, DELETE)
    virtual void execute(const std::string& sql) = 0;

    // Execute a SELECT and return all rows
    virtual std::vector<QueryResultRow> select(const std::string& sql) = 0;
};