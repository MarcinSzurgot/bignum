#include <gtest/gtest.h>
#include "bignum/BigUnsigned.hpp"

TEST(BigUnsignedTests, BasicSubtraction) {
    BigUnsigned num1 = {0x5};
    BigUnsigned num2 = {0x3};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0x2}));
}

TEST(BigUnsignedTests, SubtractZero) {
    BigUnsigned num1 = {0x5};
    BigUnsigned num2 = {0x0};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0x5}));
}

TEST(BigUnsignedTests, SubtractItself) {
    BigUnsigned num1 = {0x5};
    num1 -= num1;
    ASSERT_EQ(num1, BigUnsigned({0x0}));
}

TEST(BigUnsignedTests, SubtractFromZero) {
    BigUnsigned num1 = {0x0};
    BigUnsigned num2 = {0x3};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0x3}));
}

TEST(BigUnsignedTests, BorrowAcrossDigits) {
    BigUnsigned num1 = {0x0, 0x2};
    BigUnsigned num2 = {0x1};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0xFFFFFFFF, 0x1}));
}

TEST(BigUnsignedTests, MultipleBorrows) {
    BigUnsigned num1 = {0x0, 0x0, 0x3};
    BigUnsigned num2 = {0x2};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0xFFFFFFFE, 0xFFFFFFFF, 0x2}));
}

TEST(BigUnsignedTests, SubtractLargeFromSmall) {
    BigUnsigned num1 = {0x3};
    BigUnsigned num2 = {0xFFFFFFFF};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0xFFFFFFFC}));
}

TEST(BigUnsignedTests, SubtractResultingInZero) {
    BigUnsigned num1 = {0xFFFFFFFF, 0x0};
    BigUnsigned num2 = {0xFFFFFFFF};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0x0, 0x0}));
}

TEST(BigUnsignedTests, SameLengthSubtraction) {
    BigUnsigned num1 = {0xFFFFFFFF, 0xFFFFFFFF};
    BigUnsigned num2 = {0xFFFFFFFE, 0xFFFFFFFE};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0x1, 0x1}));
}

TEST(BigUnsignedTests, LargeDiffInMag) {
    BigUnsigned num1 = {0x1, 0x0, 0x0, 0x0};
    BigUnsigned num2 = {0x1};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0x0, 0x0, 0x0, 0x0}));
}

TEST(BigUnsignedTests, FullBorrowChain) {
    BigUnsigned num1 = {0x0, 0x0, 0x0, 0x1};
    BigUnsigned num2 = {0x1};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}));
}

TEST(BigUnsignedTests, SingleDigitDifference) {
    BigUnsigned num1 = {0x7};
    BigUnsigned num2 = {0x3};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0x4}));
}

TEST(BigUnsignedTests, DifferenceLeadingToZeroes) {
    BigUnsigned num1 = {0x2, 0x1};
    BigUnsigned num2 = {0x2};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0x0, 0x1}));
}

TEST(BigUnsignedTests, MultipleDigitDifference1) {
    BigUnsigned num1 = {0x3, 0x4, 0x5};
    BigUnsigned num2 = {0x2, 0x3};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0x1, 0x1, 0x5}));
}

TEST(BigUnsignedTests, MultipleDigitDifference2) {
    BigUnsigned num1 = {0x3, 0x4, 0x5};
    BigUnsigned num2 = {0x4, 0x5, 0x6};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0x1, 0x1, 0x1}));
}

TEST(BigUnsignedTests, SubtractFromSmaller) {
    BigUnsigned num1 = {0x4};
    BigUnsigned num2 = {0x8};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0x4}));
}

TEST(BigUnsignedTests, SubtractWithIntermediateZeros) {
    BigUnsigned num1 = {0x1, 0x0, 0x1};
    BigUnsigned num2 = {0x1};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0x0, 0x0, 0x1}));
}

TEST(BigUnsignedTests, SubtractEndingWithZeros) {
    BigUnsigned num1 = {0x1, 0x0, 0x0};
    BigUnsigned num2 = {0x1};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0x0, 0x0, 0x0}));
}

TEST(BigUnsignedTests, LargeNumbersSubtraction1) {
    BigUnsigned num1 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    BigUnsigned num2 = {0xFFFFFFFE, 0xFFFFFFFE, 0xFFFFFFFE};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0x1, 0x1, 0x1}));
}

TEST(BigUnsignedTests, LargeNumbersSubtraction2) {
    BigUnsigned num1 = {0xFFFFFFFE, 0xFFFFFFFE, 0xFFFFFFFE};
    BigUnsigned num2 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    num1 -= num2;
    ASSERT_EQ(num1, BigUnsigned({0x1, 0x1, 0x1}));
}