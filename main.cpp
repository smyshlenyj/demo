#include <iostream>

#include "math.h"

void printHelp(const std::string& prog) {
    std::cout << prog << " is a simple CLI-calculator." << std::endl;
    std::cout << "Usage:\n"
                 "  "
              << prog
              << " <number1> <operation> <number2>\n"
                 "  "
              << prog
              << " <number> !\n\n"

                 "Operations:\n"
                 "  +   addition\n"
                 "  -   subtraction\n"
                 "  x   multiplication\n"
                 "  /   division\n"
                 "  ^   power (a ^ b)\n"
                 "  !   factorial (n!)\n\n"

                 "Notes:\n"
                 "  - For factorial, only one operand is used\n"
                 "  - Factorial is defined for non-negative integers\n"
                 "  - Division by zero is an error\n\n"

                 "Examples:\n"
                 "  "
              << prog
              << " 2 + 3\n"
                 "  "
              << prog
              << " 5 !\n"
                 "  "
              << prog << " 2 ^ 8\n";
}

int main(int argc, char* argv[]) {
    std::string prog = "Calc";
    if (argc < 2 || argc > 4) {
        std::cout << "\nError: bad arguments, please check help (" << prog << " --help)\n";
        return 1;
    }

    if (argc == 2) {
        std::string arg = argv[1];
        if (arg == "-help" || arg == "--help") {
            printHelp(prog);
            return 0;
        } else
            std::cout << "\nError: bad arguments, please check help (" << prog << " --help)\n";
    }

    else if (argc == 3) {
        std::string x = argv[1];
        char operation = argv[2][0];
        if (operation == '!')
            std::cout << factorial(std::stoi(x)) << std::endl;
        else
            std::cerr << "Error: unknown operation '" << operation << "'\n";
    }

    else if (argc == 4) {
        double x = std::stod(argv[1]);
        char operation = argv[2][0];
        double y = std::stod(argv[3]);
        double result = 0;
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
                result = divide(x, y);
                if (result == 1.0 / 0.0 || result == -1.0 / 0.0) {
                    std::cout << "Error: division by zero\n";
                    return 1;
                }
            case '^':
                result = power(x, y);
                break;

            default:
                std::cout << "Error: unknown operation '" << operation << "'\n";
                return 1;
        }
        std::cout << x << " " << operation << " " << y << " = " << result << std::endl;
    }
    return 0;
}