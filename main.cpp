#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

#include "math.h"

void printHelp(const std::string& prog) {
    std::cout << prog << " is a simple CLI-calculator.\n\n"
              << "Usage:\n"
              << "  " << prog << " <number1> <operation> <number2>\n"
              << "  " << prog << " <number> !\n\n"
              << "Operations:\n"
              << "  +   addition\n"
              << "  -   subtraction\n"
              << "  x   multiplication\n"
              << "  /   division\n"
              << "  ^   power (a ^ b)\n"
              << "  !   factorial (n!)\n\n"
              << "Notes:\n"
              << "  - For factorial, only one operand is used\n"
              << "  - Factorial is defined for non-negative integers\n"
              << "  - Division by zero is an error\n\n"
              << "Examples:\n"
              << "  " << prog << " 2 + 3\n"
              << "  " << prog << " 5 !\n"
              << "  " << prog << " 2 ^ 8\n";
}

int main(int argc, char* argv[]) {
    std::string prog = argv[0];

    if (argc < 2 || argc > 4) {
        std::cerr << "Error: bad arguments. Use " << prog << " --help\n";
        return 1;
    }

    // help
    if (argc == 2) {
        std::string arg = argv[1];
        if (arg == "-h" || arg == "--help") {
            printHelp(prog);
            return 0;
        }
        std::cerr << "Error: bad arguments. Use " << prog << " --help\n";
        return 1;
    }

    // factorial
    if (argc == 3) {
        try {
            double x = std::stod(argv[1]);
            char op = argv[2][0];

            if (op != '!') {
                std::cerr << "Error: unknown operation '" << op << "'\n";
                return 1;
            }

            if (x < 0 || std::floor(x) != x) {
                std::cerr << "Error: factorial requires non-negative integer\n";
                return 1;
            }

            int n = static_cast<int>(x);
            std::cout << n << "! = " << factorial(n) << std::endl;
        } catch (const std::exception&) {
            std::cerr << "Error: invalid number\n";
            return 1;
        }
        return 0;
    }

    // binary operations
    if (argc == 4) {
        try {
            double x = std::stod(argv[1]);
            char operation = argv[2][0];
            double y = std::stod(argv[3]);
            double result = 0.0;

            switch (operation) {
                case '+':
                    result = add(x, y);
                    break;
                case '-':
                    result = subtract(x, y);
                    break;
                case 'x':
                    result = multiply(x, y);
                    break;
                case '/':
                    if (std::abs(y) < 1e-12) {
                        std::cerr << "Error: division by zero\n";
                        return 1;
                    }
                    result = divide(x, y);
                    break;
                case '^':
                    if (std::floor(y) != y) {
                        std::cerr << "Error: exponent must be an integer\n";
                        return 1;
                    }
                    result = power(x, static_cast<int>(y));
                    break;
                default:
                    std::cerr << "Error: unknown operation '" << operation << "'\n";
                    return 1;
            }

            std::cout << x << " " << operation << " " << y << " = " << result << std::endl;
        } catch (const std::exception&) {
            std::cerr << "Error: invalid number\n";
            return 1;
        }
    }

    return 0;
}
