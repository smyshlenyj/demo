#include <gtest/gtest.h>
#include "calculator.hpp"
#include <memory>
#include <spdlog/sinks/null_sink.h>

// ---------------------------
// Fixture for Calculator
// ---------------------------
class CalculatorTest : public ::testing::Test
{
   protected:
    std::shared_ptr<spdlog::logger> logger;

    void SetUp() override
    {
        auto null_sink = std::make_shared<spdlog::sinks::null_sink_mt>();
        logger = std::make_shared<spdlog::logger>("test_logger", null_sink);
    }

    Calculator makeCalculator()
    {
        return Calculator(logger);
    }
};

TEST_F(CalculatorTest, Addition)
{
    Calculator calc = makeCalculator();

    ParsedArgs args;
    args.first = 2;
    args.second = 3;
    args.operation = '+';

    mathlib::MathResult result = calc.executeOperation(args);

    EXPECT_EQ(result.value, 5);
    EXPECT_EQ(result.error, mathlib::MathError::None);
}

TEST_F(CalculatorTest, DivisionByZero)
{
    Calculator calc = makeCalculator();

    ParsedArgs args;
    args.first = 10;
    args.second = 0;
    args.operation = '/';

    EXPECT_THROW(calc.executeOperation(args), CalculationError);
}

TEST_F(CalculatorTest, Subtract)
{
    Calculator calc = makeCalculator();

    ParsedArgs args;
    args.first = 222;
    args.second = 99;
    args.operation = '-';

    mathlib::MathResult result = calc.executeOperation(args);

    EXPECT_EQ(result.value, 123);
    EXPECT_EQ(result.error, mathlib::MathError::None);
}

TEST_F(CalculatorTest, Multiply)
{
    Calculator calc = makeCalculator();

    ParsedArgs args;
    args.first = 7;
    args.second = 5;
    args.operation = 'x';

    mathlib::MathResult result = calc.executeOperation(args);

    EXPECT_EQ(result.value, 35);
    EXPECT_EQ(result.error, mathlib::MathError::None);
}

TEST_F(CalculatorTest, Factorial)
{
    Calculator calc = makeCalculator();

    ParsedArgs args;
    args.first = 11;
    args.operation = '!';

    mathlib::MathResult result = calc.executeOperation(args);

    EXPECT_EQ(result.value, 39916800);
    EXPECT_EQ(result.error, mathlib::MathError::None);
}

TEST_F(CalculatorTest, Power)
{
    Calculator calc = makeCalculator();

    ParsedArgs args;
    args.first = 7;
    args.second = 5;
    args.operation = '^';

    mathlib::MathResult result = calc.executeOperation(args);

    EXPECT_EQ(result.value, 16807);
    EXPECT_EQ(result.error, mathlib::MathError::None);
}