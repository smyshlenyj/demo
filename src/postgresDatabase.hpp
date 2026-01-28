#pragma once
#include "IDatabase.hpp"
#include <libpq-fe.h>
#include <iostream>
#include <stdexcept>

struct PostgresDataBaseError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class PostgresDatabase : public IDatabase
{
   public:
    PostgresDatabase(const std::string& connInfo, std::shared_ptr<ILogger> log) : log_(std::move(log))
    {
        conn_ = PQconnectdb(connInfo.c_str());
        if (PQstatus(conn_) != CONNECTION_OK)
        {
            std::string err = PQerrorMessage(conn_);
            PQfinish(conn_);
            std::string error = "PostgreSQL connection failed: " + err;
            log_->error(error);
            throw PostgresDataBaseError(error);
        }

        log_->info("Connected to PostgreSQL successfully");
    }

    ~PostgresDatabase() override
    {
        if (conn_ != nullptr) PQfinish(conn_);
    }

    void execute(const std::string& sql) override
    {
        PGresult* res = PQexec(conn_, sql.c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            std::string err = PQerrorMessage(conn_);
            PQclear(res);

            std::string error = "SQL execute failed: " + err;
            log_->error(error);
            throw PostgresDataBaseError(error);
        }
        PQclear(res);
    }

    std::vector<QueryResultRow> select(const std::string& sql) override
    {
        PGresult* res = PQexec(conn_, sql.c_str());
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            std::string err = PQerrorMessage(conn_);
            PQclear(res);

            std::string error = "SQL select failed: " + err;
            log_->error(error);
            throw PostgresDataBaseError(error);
        }

        int rows = PQntuples(res);
        int cols = PQnfields(res);
        std::vector<QueryResultRow> result;
        result.reserve(rows);

        for (int i = 0; i < rows; ++i)
        {
            QueryResultRow row;
            row.columns.reserve(cols);
            for (int j = 0; j < cols; ++j)
            {
                char* val = PQgetvalue(res, i, j);
                row.columns.push_back(val != nullptr ? val : "");
            }
            result.push_back(std::move(row));
        }

        PQclear(res);
        return result;
    }

   private:
    std::shared_ptr<ILogger> log_;
    PGconn* conn_;
};
