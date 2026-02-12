#pragma once

#include <vector>
#include <memory>
#include <string>
#include <optional>
#include <algorithm>
#include <shared_mutex>

#include "IDatabase.hpp"
#include "cacheRecord.hpp"
#include "ILogger.hpp"
#include "operation.hpp"
#include "converters.hpp"

class CalculationStorage
{
   public:
    explicit CalculationStorage(std::shared_ptr<ILogger> log, IDatabase& database)
        : log_(std::move(log)), database_(database)
    {
    }

    void warmUp()
    {
        log_->trace("Entered CalculationStorage::warmUp");

        auto rows = database_.select(
            "SELECT lhs, rhs, op, result FROM calc_cache "
            "ORDER BY created_at DESC");

        {
            std::unique_lock<std::shared_mutex> lock(mutex_);

            for (const auto& row : rows)
            {
                Operation operation = operationFromString(row.columns[2]);  // convert string to enum
                warm_.push_back(
                    {std::stoll(row.columns[0]), std::stoll(row.columns[1]), operation, std::stoll(row.columns[3])});
            }
        }
    }

    void save(const CacheRecord& record)
    {
        log_->trace("Entered CalculationStorage::save");

        database_.execute("INSERT INTO calc_cache(lhs, rhs, op, result, created_at) VALUES (" +
                          std::to_string(record.lhs) + ", " + std::to_string(record.rhs) + ", '" +
                          operationToString(record.op) + "', " + std::to_string(record.result) +
                          ", now()) ON CONFLICT (lhs, rhs, op) DO NOTHING");
    }

    std::optional<std::int64_t> getCachedResult(std::int64_t first, std::int64_t second, Operation operation)
    {
        std::shared_lock<std::shared_mutex> lock(mutex_);

        log_->trace("Entered CalculationStorage::getCachedResult");

        auto cachedRecordIt =
            std::find_if(warm_.begin(), warm_.end(), [&](const CacheRecord& record)
                         { return record.lhs == first && record.rhs == second && record.op == operation; });

        if (cachedRecordIt != warm_.end()) return cachedRecordIt->result;

        return std::nullopt;
    }

   private:
    std::shared_ptr<ILogger> log_;
    IDatabase& database_;
    std::vector<CacheRecord> warm_;    // shared cache in memory
    mutable std::shared_mutex mutex_;  // cashe mutex for thread safety
};
