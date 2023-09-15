#include <gtest/gtest.h>
#include "bignum/BigUnsigned.hpp"

TEST(BigUnsignedTests, BasicMultiplication) {
    BigUnsigned num1 = {0x5};
    BigUnsigned num2 = {0x3};
    num1 *= num2;
    ASSERT_EQ(num1, BigUnsigned({0xF}));
}

TEST(BigUnsignedTests, MultiplyByZero) {
    BigUnsigned num1 = {0x5};
    BigUnsigned num2 = {0x0};
    num1 *= num2;
    ASSERT_EQ(num1, BigUnsigned({0x0}));
}

TEST(BigUnsignedTests, MultiplyItself) {
    BigUnsigned num1 = {0x5};
    num1 *= num1;
    ASSERT_EQ(num1, BigUnsigned({0x19})); // 5*5 = 25
}

TEST(BigUnsignedTests, MultiplyByOne) {
    BigUnsigned num1 = {0x5};
    BigUnsigned num2 = {0x1};
    num1 *= num2;
    ASSERT_EQ(num1, BigUnsigned({0x5}));
}

TEST(BigUnsignedTests, MultipleDigitMultiplication1) {
    BigUnsigned num1 = {0xFFFFFFFF};
    BigUnsigned num2 = {0x2};
    num1 *= num2;
    ASSERT_EQ(num1, BigUnsigned({0xFFFFFFFE, 0x1}));
}

TEST(BigUnsignedTests, MultipleDigitMultiplication2) {
    BigUnsigned num1 = {0xFFFFFFFF, 0x1};
    BigUnsigned num2 = {0xFFFFFFFF, 0x1};
    num1 *= num2;
    ASSERT_EQ(num1, BigUnsigned({0x1, 0xFFFFFFFC, 0x3}));
}

TEST(BigUnsignedTests, LargeNumbersMultiplication) {
    BigUnsigned num1 = {0xFFFFFFFE};
    BigUnsigned num2 = {0xFFFFFFFE};
    num1 *= num2;
    ASSERT_EQ(num1, BigUnsigned({0x4, 0xFFFFFFFC}));
}

TEST(BigUnsignedTests, MultiplyWithZeroes) {
    BigUnsigned num1 = {0x0, 0x1};
    BigUnsigned num2 = {0x0, 0x2};
    num1 *= num2;
    ASSERT_EQ(num1, BigUnsigned({0x0, 0x0, 0x2}));
}

TEST(BigUnsignedTests, MultiplyWithLeadingZeroes) {
    BigUnsigned num1 = {0x1, 0x0};
    BigUnsigned num2 = {0x2, 0x0};
    num1 *= num2;
    ASSERT_EQ(num1, BigUnsigned({0x2, 0x0, 0x0}));
}

TEST(BigUnsignedTests, OverflowMultiplication) {
    BigUnsigned num1 = {0xFFFFFFFF};
    BigUnsigned num2 = {0xFFFFFFFF};
    num1 *= num2;
    ASSERT_EQ(num1, BigUnsigned({0x1, 0xFFFFFFFE}));
}

TEST(BigUnsignedTests, ComplexMultiplication1) {
    BigUnsigned num1 = {0xFFFFFFF0, 0x0, 0x0, 0x1};
    BigUnsigned num2 = {0xF};
    num1 *= num2;
    ASSERT_EQ(num1, BigUnsigned({0xFFFFFF10, 0xE, 0x0, 0xF}));
}

TEST(BigUnsignedTests, ComplexMultiplication2) {
    BigUnsigned num1 = {0xFFFFFFFE, 0xFFFFFFFE};
    BigUnsigned num2 = {0xFFFFFFFD, 0xFFFFFFFD};
    num1 *= num2;
    ASSERT_EQ(num1, BigUnsigned({0x6, 0x7, 0xFFFFFFFD, 0xFFFFFFFC}));
}

TEST(BigUnsignedTests, SingleDigitLargeMultiply) {
    BigUnsigned num1 = {0x7};
    BigUnsigned num2 = {0xFFFFFFFD};
    num1 *= num2;
    ASSERT_EQ(num1, BigUnsigned({0xFFFFFFEB, 0x6}));
}

TEST(BigUnsignedTests, MultiDigitCarry) {
    BigUnsigned num1 = {0xFFFFFFFC, 0x3};
    BigUnsigned num2 = {0x4, 0x4};
    num1 *= num2;
    ASSERT_EQ(num1, BigUnsigned({0xFFFFFFF0, 0xFFFFFFFF, 0xF}));
}

TEST(BigUnsignedTests, SingleDigitSelfMultiply) {
    BigUnsigned num1 = {0xFFFFFFFE};
    num1 *= num1;
    ASSERT_EQ(num1, BigUnsigned({0x4, 0xFFFFFFFC}));
}