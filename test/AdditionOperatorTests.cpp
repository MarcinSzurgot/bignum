#include <gtest/gtest.h>
#include "bignum/BigUnsigned.hpp"

TEST(BigUnsignedTests, MaxValuePlusZero) {
    BigUnsigned num1 = {0xFFFFFFFF};
    BigUnsigned zero;
    num1 += zero;
    ASSERT_EQ(num1, BigUnsigned({0xFFFFFFFF}));
}

TEST(BigUnsignedTests, MultipleCarryOver) {
    BigUnsigned num1 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    BigUnsigned num2 = {0x1};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0x0, 0x0, 0x0, 0x1}));
}

TEST(BigUnsignedTests, MinValuesAddition) {
    BigUnsigned num1 = {0x0};
    BigUnsigned num2 = {0x0};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0x0}));
}

TEST(BigUnsignedTests, AddWithEmpty) {
    BigUnsigned num1 = {0x1, 0x2, 0x3};
    BigUnsigned empty;
    num1 += empty;
    ASSERT_EQ(num1, BigUnsigned({0x1, 0x2, 0x3}));
}

TEST(BigUnsignedTests, EarlyCarry) {
    BigUnsigned num1 = {0xFFFFFFFF, 0x0, 0x0};
    BigUnsigned num2 = {0x2};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0x1, 0x1, 0x0}));
}

TEST(BigUnsignedTests, ZerosInTheMiddle) {
    BigUnsigned num1 = {0xFFFFFFFF, 0x0, 0xFFFFFFFF};
    BigUnsigned num2 = {0x1};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0x0, 0x1, 0xFFFFFFFF}));
}

TEST(BigUnsignedTests, EqualLengthNoCarry) {
    BigUnsigned num1 = {0xF, 0xF};
    BigUnsigned num2 = {0xF, 0xF};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0x1E, 0x1E}));
}

TEST(BigUnsignedTests, NonZeroCarryToEnd) {
    BigUnsigned num1 = {0xFFFFFFFF, 0xFFFFFFFF};
    BigUnsigned num2 = {0xFFFFFFFF};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0xFFFFFFFE, 0x0, 0x1}));
}

TEST(BigUnsignedTests, CarryWithMultipleZeros) {
    BigUnsigned num1 = {0xFFFFFFFF, 0x0, 0x0, 0x0};
    BigUnsigned num2 = {0x1};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0x0, 0x1, 0x0, 0x0}));
}

TEST(BigUnsignedTests, MultipleCarryOverMultiplePlaces) {
    BigUnsigned num1 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    BigUnsigned num2 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x1}));
}

TEST(BigUnsignedTests, LongChainOfZeros) {
    BigUnsigned num1 = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    BigUnsigned num2 = {0x1};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0x1, 0x0, 0x0, 0x0, 0x0, 0x0}));
}

TEST(BigUnsignedTests, LongCarry) {
    BigUnsigned num1 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    BigUnsigned num2 = {0x1};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0x0, 0x0, 0x0, 0x0, 0x1}));
}

TEST(BigUnsignedTests, LastPlaceCarry) {
    BigUnsigned num1 = {0x1};
    BigUnsigned num2 = {0xFFFFFFFF};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0x0, 0x1}));
}

TEST(BigUnsignedTests, MiddleCarry) {
    BigUnsigned num1 = {0x1, 0xFFFFFFFF, 0x1};
    BigUnsigned num2 = {0x0, 0x1, 0x0};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0x1, 0x0, 0x2}));
}

TEST(BigUnsignedTests, DifferentLengthAddition) {
    BigUnsigned num1 = {0xFFFFFFFF, 0xFFFFFFFF};
    BigUnsigned num2 = {0xFFFFFFFF, 0xFFFFFFFF, 0x1};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0xFFFFFFFE, 0xFFFFFFFF, 0x2}));
}

TEST(BigUnsignedTests, LeadingZerosAddition) {
    BigUnsigned num1 = {0x0, 0x1};
    BigUnsigned num2 = {0x0, 0xFFFFFFFF};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0x0, 0x0, 0x1}));
}

TEST(BigUnsignedTests, NoCarryLargeValues) {
    BigUnsigned num1 = {0xFFFFFFFE};
    BigUnsigned num2 = {0xFFFFFFFE};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0xFFFFFFFC, 0x1}));
}

TEST(BigUnsignedTests, UnitValuesAddition) {
    BigUnsigned num1 = {0x1};
    BigUnsigned num2 = {0x1};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0x2}));
}

TEST(BigUnsignedTests, UltimateCarry) {
    BigUnsigned num1 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    BigUnsigned num2 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1}));
}

TEST(BigUnsignedTests, UnitAdditionToMax) {
    BigUnsigned num1 = {0xFFFFFFFF};
    BigUnsigned num2 = {0x1};
    num1 += num2;
    ASSERT_EQ(num1, BigUnsigned({0x0, 0x1}));
}