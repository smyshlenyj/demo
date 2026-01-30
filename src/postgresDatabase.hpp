#pragma once

#include "IDatabase.hpp"
#include <libpq-fe.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

struct PostgresDataBaseError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

struct PGconnDeleter
{
    void operator()(PGconn* connection) const noexcept
    {
        if (connection != nullptr) PQfinish(connection);
    }
};

struct PGresultDeleter
{
    void operator()(PGresult* result) const noexcept
    {
        if (result != nullptr) PQclear(result);
    }
};

using pgConnPtr = std::unique_ptr<PGconn, PGconnDeleter>;
using pgResultPtr = std::unique_ptr<PGresult, PGresultDeleter>;

class PostgresDatabase : public IDatabase
{
   public:
    PostgresDatabase(const std::string& connInfo, std::shared_ptr<ILogger> log)
        : log_(std::move(log)), conn_(PQconnectdb(connInfo.c_str()))
    {
        if (!conn_ || PQstatus(conn_.get()) != CONNECTION_OK)
        {
            std::string err = conn_ ? PQerrorMessage(conn_.get()) : "PQconnectdb returned nullptr";

            std::string error = "PostgreSQL connection failed: " + err;
            log_->error(error);
            throw PostgresDataBaseError(error);
        }

        log_->info("Connected to PostgreSQL successfully");
    }

    ~PostgresDatabase() override = default;

    void execute(const std::string& sql) override
    {
        pgResultPtr res(PQexec(conn_.get(), sql.c_str()));

        if (!res || PQresultStatus(res.get()) != PGRES_COMMAND_OK)
        {
            std::string err = PQerrorMessage(conn_.get());
            std::string error = "SQL execute failed: " + err;
            log_->error(error);
            throw PostgresDataBaseError(error);
        }
    }

    std::vector<QueryResultRow> select(const std::string& sql) override
    {
        pgResultPtr res(PQexec(conn_.get(), sql.c_str()));

        if (!res || PQresultStatus(res.get()) != PGRES_TUPLES_OK)
        {
            std::string err = PQerrorMessage(conn_.get());
            std::string error = "SQL select failed: " + err;
            log_->error(error);
            throw PostgresDataBaseError(error);
        }

        const int rows = PQntuples(res.get());
        const int cols = PQnfields(res.get());

        std::vector<QueryResultRow> result;
        result.reserve(rows);

        for (int i = 0; i < rows; ++i)
        {
            QueryResultRow row;
            row.columns.reserve(cols);

            for (int j = 0; j < cols; ++j)
            {
                char* value = PQgetvalue(res.get(), i, j);
                row.columns.emplace_back(value != nullptr ? value : "");
            }

            result.emplace_back(std::move(row));
        }

        return result;
    }

   private:
    std::shared_ptr<ILogger> log_;
    pgConnPtr conn_;
};
