#include "runner.h"
#include "parser.h"
#include "checker.h"
#include "calculator.h"
#include "printer.h"

void Runner::run(const std::string& inputPath)
{
    log->trace("Entered Runner::run");

    Parser parser(log);
    auto parsedArgs = parser.parse(inputPath);

    Checker checker(log);
    checker.checkParsedArgs(parsedArgs);

    Calculator calculator(log);
    auto result = calculator.executeOperation(parsedArgs);

    Printer printer(log);
    printer.printResult(result);
}
