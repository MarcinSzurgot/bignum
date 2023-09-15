#include <gtest/gtest.h>
#include "bignum/BigUnsigned.hpp"

std::ostream& operator<<(std::ostream& os, const BigUnsigned& num) {
    os << std::hex << std::uppercase;
    for (auto d = 0u; d < num.mag(); ++d) {
        os << "0x" << num[d] << ", ";
    }
    return os;
}

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

/////////////////////////////

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

////////////

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

//////////////

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
    ASSERT_EQ(num, BigUnsigned({0xE468ACF1, 0x3579BDE0, 0x1}));
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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
