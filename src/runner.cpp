#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "math.h"

void printHelp(const char* prog)
{
    printf("%s is a simple CLI-calculator.\n\n", prog);
    puts("Usage:");
    printf("  %s <number1> <operation> <number2>\n", prog);
    printf("  %s <number> !\n\n", prog);

    puts("Operations:");
    puts("  +   addition");
    puts("  -   subtraction");
    puts("  x   multiplication");
    puts("  /   division");
    puts("  ^   power (a ^ b)");
    puts("  !   factorial (n!)\n");

    puts("Notes:");
    puts("  - For factorial, only one operand is used");
    puts("  - Factorial is defined for non-negative integers");
    puts("  - Division by zero is an error\n");

    puts("Examples:");
    printf("  %s 2 + 3\n", prog);
    printf("  %s 5 !\n", prog);
    printf("  %s 2 ^ 8\n", prog);
}

struct ParsedArgs
{
    int x;
    int y;           // Only used for binary operations
    char operation;  // '+', '-', 'x', '/', '^', '!'
    bool valid;
};

ParsedArgs parseArgs(int argc, char* argv[])
{
    ParsedArgs result = {0, 0, '\0', false};

    if (argc < 2 || argc > 4) return result;

    // Factorial
    if (argc == 3)
    {
        double x = atof(argv[1]);
        char op = argv[2][0];

        if (op != '!' || x < 0 || floor(x) != x) return result;

        result.x = static_cast<int>(x);
        result.operation = '!';
        result.valid = true;
        return result;
    }

    // Binary operations
    if (argc == 4)
    {
        int x = atoi(argv[1]);
        int y = atoi(argv[3]);
        char op = argv[2][0];

        if (op != '+' && op != '-' && op != 'x' && op != '/' && op != '^') return result;

        result.x = x;
        result.y = y;
        result.operation = op;
        result.valid = true;
        return result;
    }

    return result;
}

bool checkParsedArgs(const ParsedArgs& args)
{
    if (!args.valid) return false;

    if (args.operation == '!')
    {
        if (args.x < 0)
        {
            puts("Error: factorial requires non-negative integer");
            return false;
        }
        return true;
    }

    if (args.operation == '/' && args.y == 0)
    {
        puts("Error: division by zero");
        return false;
    }

    return true;
}

std::uint64_t executeOperation(const ParsedArgs& args)
{
    switch (args.operation)
    {
        case '+':
            return mathlib::add(args.x, args.y);
        case '-':
            return mathlib::subtract(args.x, args.y);
        case 'x':
            return mathlib::multiply(args.x, args.y);
        case '/':
            return mathlib::divide(args.x, args.y);
        case '^':
            return mathlib::power(args.x, args.y);
        case '!':
            return mathlib::factorial(args.x);
        default:
            return 0;
    }
}

int runner(int argc, char* argv[])
{
    char* prog = argv[0];

    if (argc == 2 && (argv[1][0] == '-' && (argv[1][1] == 'h' || argv[1][1] == '-')))
    {
        printHelp(prog);
        return 0;
    }

    ParsedArgs parsedArgs = parseArgs(argc, argv);
    if (!checkParsedArgs(parsedArgs))
    {
        printf("Usage: %s <number1> <operation> <number2> or %s <number> !\n", prog, prog);
        return 1;
    }

    std::uint64_t result = executeOperation(parsedArgs);
    printf("Result: %llu\n", result);

    return 0;
}
