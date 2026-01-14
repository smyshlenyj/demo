#include <cinttypes>  // для PRId64
#include <cstdio>
#include "printer.h"

void Printer::printResult(const mathlib::MathResult& result)
{
    log->trace("Entered Printer::printResult");

    printf("Result: %" PRId64 "\n", result.value);
}