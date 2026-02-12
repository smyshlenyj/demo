#pragma once

#include <cstdint>
#include <utility>  // std::swap

#include "operation.hpp"

// Результат вычисления, пригодный для кэша и БД
struct CacheRecord
{
    std::int64_t lhs;
    std::int64_t rhs;
    Operation op;
    std::int64_t result;

    // Нормализация для коммутативных операций
    // Гарантирует, что 2+3 и 3+2 будут одним и тем же ключом
    void normalize() noexcept
    {
        if ((op == Operation::ADD || op == Operation::MUL) && lhs > rhs)
        {
            std::swap(lhs, rhs);
        }
    }
};
