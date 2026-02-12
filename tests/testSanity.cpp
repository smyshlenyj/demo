#include <memory>
#include <gtest/gtest.h>

#include "calculator.hpp"
#include "operation.hpp"
#include "checker.hpp"
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
};

TEST_F(CalculatorTest, Addition)
{
    EXPECT_EQ(Calculator::executeOperation(int64_t(2), int64_t(3), Operation::ADD, logger), 5);
}

TEST_F(CalculatorTest, DivisionByZero)
{
    EXPECT_THROW(Calculator::executeOperation(10, 0, Operation::DIV, logger), std::invalid_argument);
}

TEST_F(CalculatorTest, Subtract)
{
    EXPECT_EQ(Calculator::executeOperation(222, 99, Operation::SUB, logger), 123);
}

TEST_F(CalculatorTest, Multiply)
{
    EXPECT_EQ(Calculator::executeOperation(7, 5, Operation::MUL, logger), 35);
}

TEST_F(CalculatorTest, Factorial)
{
    EXPECT_EQ(Calculator::executeOperation(11, 0, Operation::FACT, logger), 39916800);
}

TEST_F(CalculatorTest, Power)
{
    EXPECT_EQ(Calculator::executeOperation(7, 5, Operation::POW, logger), 16807);
}

// ---------------------------
// Checker Tests
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

TEST_F(CheckerTest, ValidAddition)
{
    Checker checker;
    EXPECT_NO_THROW(checker.checkOperation(Operation::ADD, logger));
}

TEST_F(CheckerTest, InvalidOperation)
{
    Checker checker;
    EXPECT_THROW(checker.checkOperation(Operation::UNKNOWN, logger), ValidationError);
}
