#pragma once

#include <cstdint>
#include <utility>  // std::swap

// Результат вычисления, пригодный для кэша и БД
struct CacheRecord
{
    std::int64_t lhs;
    std::int64_t rhs;
    char op;
    std::int64_t result;

    // Нормализация для коммутативных операций
    // Гарантирует, что 2+3 и 3+2 будут одним и тем же ключом
    void normalize()
    {
        if ((op == '+' || op == '*') && lhs > rhs)
        {
            std::swap(lhs, rhs);
        }
    }
};
