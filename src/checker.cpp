#include "checker.h"
#include "parsedArgs.h"

void Checker::checkParsedArgs(const ParsedArgs& args)
{
    log->trace("Entered Checker::checkParsedArgs");

    if (args.operation == '\0') throw ValidationError("Operation is missing");

    switch (args.operation)
    {
        case '+':
        case '-':
        case 'x':
        case '/':
        case '^':
        case '!':
            return;  // операция валидна
        default:
            throw ValidationError("Invalid operation: " + std::string(1, args.operation));
    }
}
