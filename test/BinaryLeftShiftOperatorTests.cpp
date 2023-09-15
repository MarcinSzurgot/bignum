#include <gtest/gtest.h>
#include "bignum/BigUnsigned.hpp"

TEST(BigUnsignedTests, LeftShiftByZero) {
    BigUnsigned num = {0x12345678};
    num <<= 0;
    ASSERT_EQ(num, BigUnsigned({0x12345678}));
}

TEST(BigUnsignedTests, SingleDigitLeftShift) {
    BigUnsigned num = {0x12345678};
    num <<= 4;
    ASSERT_EQ(num, BigUnsigned({0x23456780, 0x1}));
}

TEST(BigUnsignedTests, SingleDigitBoundaryShift) {
    BigUnsigned num = {0xF2345678};
    num <<= 4;
    ASSERT_EQ(num, BigUnsigned({0x23456780, 0xF}));
}

TEST(BigUnsignedTests, MultipleDigitLeftShift) {
    BigUnsigned num = {0xF2345678, 0x9ABCDEF0};
    num <<= 4;
    ASSERT_EQ(num, BigUnsigned({0x23456780, 0xABCDEF0F, 0x9}));
}

TEST(BigUnsignedTests, WholeDigitsShift) {
    BigUnsigned num = {0x12345678};
    num <<= 32;
    ASSERT_EQ(num, BigUnsigned({0x0, 0x12345678}));
}

TEST(BigUnsignedTests, MultipleWholeDigitsShift) {
    BigUnsigned num = {0x12345678};
    num <<= 64;
    ASSERT_EQ(num, BigUnsigned({0x0, 0x0, 0x12345678}));
}

TEST(BigUnsignedTests, CombinedWholeAndPartialDigitsShift) {
    BigUnsigned num = {0xF2345678, 0x9ABCDEF0};
    num <<= 36;
    ASSERT_EQ(num, BigUnsigned({0x0, 0x23456780, 0xABCDEF0F, 0x9}));
}

TEST(BigUnsignedTests, MaxBitShift) {
    BigUnsigned num = {0xF2345678, 0x9ABCDEF0};
    num <<= 31;
    ASSERT_EQ(num, BigUnsigned({0x0, 0x791A2B3C, 0x4D5E6F78}));
}

TEST(BigUnsignedTests, ShiftZero) {
    BigUnsigned num = {0x0};
    num <<= 4;
    ASSERT_EQ(num, BigUnsigned({0x0}));
}

TEST(BigUnsignedTests, ShiftZeroByWholeDigits) {
    BigUnsigned num = {0x0};
    num <<= 32;
    ASSERT_EQ(num, BigUnsigned({0x0}));
}

TEST(BigUnsignedTests, LargeShift) {
    BigUnsigned num = {0x1};
    num <<= 128;
    ASSERT_EQ(num, BigUnsigned({0x0, 0x0, 0x0, 0x0, 0x1}));
}