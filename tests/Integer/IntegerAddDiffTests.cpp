#include <bignum/Integer.hpp>

#include <gtest/gtest.h>

TEST(IntegerAddDiffTests, testThatAddingNegativeWithBiggerAbsIsNegative)
{
    // given
    const auto biggerAbs = bignum::Integer(bignum::Sign::Minus, bignum::Unsigned{1u, 1u, 1u});
    const auto lessAbs   = bignum::Integer(bignum::Sign::Plus, bignum::Unsigned{1u, 1u});
    const auto expected  = bignum::Sign::Minus;

    // when
    const auto actual = (biggerAbs + lessAbs).sign();

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatAddingNegativeWithLessAbsIsPositive)
{
    // given
    const auto lessAbs   = bignum::Integer(bignum::Sign::Minus, bignum::Unsigned{1u, 1u});
    const auto biggerAbs = bignum::Integer(bignum::Sign::Plus, bignum::Unsigned{1u, 1u, 1u});
    const auto expected  = bignum::Sign::Plus;

    // when
    const auto actual = (lessAbs + biggerAbs).sign();

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatAddingTwoNegativesIsNegative)
{
    // given
    const auto minus = bignum::Sign::Minus;
    const auto lhs = bignum::Integer(minus, bignum::Unsigned{1u, 1u, 1u});
}
