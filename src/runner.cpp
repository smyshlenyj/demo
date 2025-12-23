#include "runner.h"

#include <inttypes.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>

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
    std::int64_t first;
    std::int64_t second;  // Only used for binary operations
    char operation;       // '+', '-', 'x', '/', '^', '!'
    bool valid;
};

ParsedArgs parseArgs(int argc, char* argv[])
{
    ParsedArgs result = {0, 0, '\0', false};

    if (argc < 2 || argc > 4)
    {
        return result;
    }

    // Factorial
    if (argc == 3)
    {
        int first = atoi(argv[1]);
        char operation = argv[2][0];
        result.first = first;
        result.operation = operation;
        // result.valid = true;
        return result;
    }

    // Binary operations
    if (argc == 4)
    {
        int first = atoi(argv[1]);
        int second = atoi(argv[3]);
        char operation = argv[2][0];

        result.first = first;
        result.second = second;
        result.operation = operation;
        // result.valid = true;
        return result;
    }

    return result;
}

void checkParsedArgs(ParsedArgs& args)
{
    if (args.operation == '\0')
    {
        args.valid = false;
        return;
    }

    if (args.operation != '+' && args.operation != '-' && args.operation != 'x' && args.operation != '/' &&
        args.operation != '^' && args.operation != '!')
    {
        args.valid = false;
        return;
    }

    args.valid = true;
}

mathlib::MathResult executeOperation(const ParsedArgs& args)
{
    switch (args.operation)
    {
        case '+':
            return mathlib::add(args.first, args.second);
        case '-':
            return mathlib::subtract(args.first, args.second);
        case 'x':
            return mathlib::multiply(args.first, args.second);
        case '/':
            return mathlib::divide(args.first, args.second);
        case '^':
            return mathlib::power(args.first, args.second);
        case '!':
            return mathlib::factorial(args.first);
        default:
            return {0, mathlib::MathError::InvalidArgument};
    }
}

const char* errorToString(mathlib::MathError error)
{
    switch (error)
    {
        case mathlib::MathError::None:
            return "no error";
        case mathlib::MathError::DivisionByZero:
            return "division by zero";
        case mathlib::MathError::InvalidArgument:
            return "invalid argument, please check --help";
        case mathlib::MathError::NegativeFactorial:
            return "negative factorial";
        default:
            return "unknown error";
    }
}

void printResult(const mathlib::MathResult& result)
{
    if (result.error != mathlib::MathError::None)
    {
        printf("Error: %s\n", errorToString(result.error));
        return;
    }

    printf("Result: %" PRId64 "\n", result.value);
}

int runner(int argc, char* argv[])
{
    char* prog = argv[0];

    if ((argc == 2 && argv[1][0] == '-' &&
         ((argv[1][1] == 'h' && argv[1][2] == '\0') || (argv[1][1] == '-' && argv[1][2] == 'h' && argv[1][3] == 'e' &&
                                                        argv[1][4] == 'l' && argv[1][5] == 'p' && argv[1][6] == '\0'))))
    {
        printHelp(prog);
        return 0;
    }

    ParsedArgs parsedArgs = parseArgs(argc, argv);
    checkParsedArgs(parsedArgs);
    if (!parsedArgs.valid)
    {
        printf("Error: invalid arguments\n");
        return 1;
    }
    mathlib::MathResult result = executeOperation(parsedArgs);
    printResult(result);
    return 0;
}
