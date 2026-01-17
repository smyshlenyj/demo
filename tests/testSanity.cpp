#include <gtest/gtest.h>
#include "calculator.hpp"
#include "parser.hpp"
#include "checker.hpp"
#include <memory>
#include "loggerWrapper.hpp"

// ---------------------------
// Fixture for Calculator
// ---------------------------
class CalculatorTest : public ::testing::Test
{
   protected:
    std::shared_ptr<ILogger> logger;

    void SetUp() override
    {
        LoggerWrapper::init();
        logger = LoggerWrapper::get();
    }

    Calculator makeCalculator()
    {
        return Calculator(logger);
    }
};

// ---------------------------
// Fixture for Parser
// ---------------------------
class ParserTest : public ::testing::Test
{
   protected:
    std::shared_ptr<ILogger> logger;

    void SetUp() override
    {
        LoggerWrapper::init();
        logger = LoggerWrapper::get();
    }
};

// ---------------------------
// Fixture for Checker
// ---------------------------
class CheckerTest : public ::testing::Test
{
   protected:
    std::shared_ptr<ILogger> logger;

    void SetUp() override
    {
        LoggerWrapper::init();
        logger = LoggerWrapper::get();
    }
};

TEST_F(CalculatorTest, Addition)
{
    Calculator calc = makeCalculator();

    ParsedArgs args;
    args.first = 2;
    args.second = 3;
    args.operation = '+';

    std::int64_t result = calc.executeOperation(args);

    EXPECT_EQ(result, 5);
}

TEST_F(CalculatorTest, DivisionByZero)
{
    Calculator calc = makeCalculator();

    ParsedArgs args;
    args.first = 10;
    args.second = 0;
    args.operation = '/';

    EXPECT_THROW(calc.executeOperation(args), std::invalid_argument);
}

TEST_F(CalculatorTest, Subtract)
{
    Calculator calc = makeCalculator();

    ParsedArgs args;
    args.first = 222;
    args.second = 99;
    args.operation = '-';

    std::int64_t result = calc.executeOperation(args);

    EXPECT_EQ(result, 123);
}

TEST_F(CalculatorTest, Multiply)
{
    Calculator calc = makeCalculator();

    ParsedArgs args;
    args.first = 7;
    args.second = 5;
    args.operation = 'x';

    std::int64_t result = calc.executeOperation(args);

    EXPECT_EQ(result, 35);
}

TEST_F(CalculatorTest, Factorial)
{
    Calculator calc = makeCalculator();

    ParsedArgs args;
    args.first = 11;
    args.operation = '!';

    std::int64_t result = calc.executeOperation(args);

    EXPECT_EQ(result, 39916800);
}

TEST_F(CalculatorTest, Power)
{
    Calculator calc = makeCalculator();

    ParsedArgs args;
    args.first = 7;
    args.second = 5;
    args.operation = '^';

    std::int64_t result = calc.executeOperation(args);

    EXPECT_EQ(result, 16807);
}

TEST_F(ParserTest, InvalidJSON)
{
    Parser parser(logger);
    EXPECT_THROW(parser.parse("{invalid json}"), ParseError);
}

// ---------------------------
// Checker Tests
// ---------------------------
TEST_F(CheckerTest, ValidAddition)
{
    Checker checker(logger);
    ParsedArgs args;
    args.operation = '+';
    EXPECT_NO_THROW(checker.checkParsedArgs(args));
}

TEST_F(CheckerTest, InvalidOperation)
{
    Checker checker(logger);
    ParsedArgs args;
    args.operation = '?';
    EXPECT_THROW(checker.checkParsedArgs(args), ValidationError);
}

TEST_F(CheckerTest, MissingOperation)
{
    Checker checker(logger);
    ParsedArgs args;
    args.operation = '\0';
    EXPECT_THROW(checker.checkParsedArgs(args), ValidationError);
}