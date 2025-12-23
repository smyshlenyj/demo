#pragma once
#include "mathlib.h"

int runner(int argc, char* argv[]);
void printHelp(const char* prog);
void printResult(const mathlib::MathResult& result);
const char* errorToString(mathlib::MathError error);