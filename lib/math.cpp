#pragma once
#include <iostream>

double add(double x, double y) { return x + y; }

double subtract(double x, double y) { return x - y; }

double multiply(double x, double y) { return x * y; }

double divide(double x, double y) { return x / y; }

double power(double base, double exp) {
    double result = base;
    for (int i = 0; i < exp - 1; ++i) {
        result *= base;
    }
    return result;
}

double factorial(double x) {
    if (x == 1) return 1;
    return x * factorial(x - 1);
}
