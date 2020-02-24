#include <bignum/Integer.hpp>

#include <gtest/gtest.h>

TEST(IntegerBitwiseLogicTests, testThatNegatingPositiveGivesNegaitve)
{
    // given
    const auto positive = bignum::Integer(bignum::Sign::Plus, bignum::Unsigned{1u});
    const auto expected = bignum::Sign::Minus;

    // when
    const auto actual = (~positive).sign();

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerBitwiseLogicTests, testThatNegatingNegativeGivesPositive)
{
    // given
    const auto negative = bignum::Integer(bignum::Sign::Minus, bignum::Unsigned{1u});
    const auto expected = bignum::Sign::Plus;

    // when
    const auto actual = (~negative).sign();

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerBitwiseLogicTests, testThatNegatingZeroGivesZero)
{
    // given
    const auto positive = bignum::Integer(bignum::Sign::Plus, bignum::Unsigned{1u});
    const auto expected = bignum::Sign::Minus;

    // when
    const auto actual = (~positive).sign();

    // then
    ASSERT_EQ(expected, actual);
}
