#pragma once

#include <vector>
#include <memory>
#include <string>
#include <optional>
#include <algorithm>

#include "IDatabase.hpp"
#include "cacheRecord.hpp"
#include "ILogger.hpp"

class CalculationStorage
{
   public:
    explicit CalculationStorage(std::shared_ptr<ILogger> log, IDatabase& database)
        : log_(std::move(log)), database_(database)
    {
    }

    std::vector<CacheRecord> warmUp()
    {
        log_->trace("Entered CalculationStorage::warmUp");

        auto rows = database_.select(
            "SELECT lhs, rhs, op, result FROM calc_cache "
            "ORDER BY created_at DESC");

        std::vector<CacheRecord> records;
        records.reserve(rows.size());

        for (const auto& row : rows)
        {
            records.push_back({std::stoll(row.columns[0]), std::stoll(row.columns[1]), row.columns[2][0],
                               std::stoll(row.columns[3])});
        }

        return records;
    }

    void save(const CacheRecord& record)
    {
        log_->trace("Entered CalculationStorage::save");
        database_.execute("INSERT INTO calc_cache(lhs, rhs, op, result, created_at) VALUES (" +
                          std::to_string(record.lhs) + ", " + std::to_string(record.rhs) + ", '" +
                          std::string(1, record.op) + "', " + std::to_string(record.result) + ", now())");
    }

    std::optional<std::int64_t> getCachedResult(const std::vector<CacheRecord>& cache, const ParsedArgs& args)
    {
        log_->trace("Entered CalculationStorage::getCachedResult");

        auto cachedRecordIt = std::find_if(
            cache.begin(), cache.end(), [&](const CacheRecord& record)
            { return record.lhs == args.first && record.rhs == args.second && record.op == args.operation; });

        if (cachedRecordIt != cache.end()) return cachedRecordIt->result;

        return std::nullopt;
    }

   private:
    std::shared_ptr<ILogger> log_;
    IDatabase& database_;
};
