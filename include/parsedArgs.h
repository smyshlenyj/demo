#pragma once

#include <cstdint>
#include <iostream>

struct ParsedArgs
{
    std::int64_t first = 0;
    std::int64_t second = 0;
    char operation = '\0';
    // bool valid = false;
    std::string errorMsg;
};