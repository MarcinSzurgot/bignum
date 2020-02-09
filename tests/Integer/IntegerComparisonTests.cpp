#include <bignum/Integer.hpp>

#include <gtest/gtest.h>

TEST(IntegerComparisonTests, testThatMinusIsLessThanPlus)
{
    // given
    const auto minus = bignum::Integer(bignum::Sign::Minus, bignum::Unsigned{1u});
    const auto plus  = bignum::Integer(bignum::Sign::Plus,  bignum::Unsigned{1u});
    const auto expected = bignum::Comparison::LT;

    // when
    const auto actual = compare(minus, plus);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerComparisonTests, testThatPlusIsLessThanMinus)
{
    // given
    const auto minus = bignum::Integer(bignum::Sign::Minus, bignum::Unsigned{1u});
    const auto plus  = bignum::Integer(bignum::Sign::Plus,  bignum::Unsigned{1u});
    const auto expected = bignum::Comparison::GT;

    // when
    const auto actual = compare(plus, minus);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerComparisonTests, testThatLessAbsIsGreaterForMinus)
{
    // given
    const auto lessAbs    = bignum::Unsigned{1u, 1u, 1u};
    const auto greaterAbs = bignum::Unsigned{1u, 1u, 2u};
    const auto minus      = bignum::Integer(bignum::Sign::Minus, lessAbs);
    const auto plus       = bignum::Integer(bignum::Sign::Plus,  greaterAbs);
    const auto expected   = bignum::Comparison::GT;

    // when
    const auto actual = bignum::compare(minus, plus);

    // then
    ASSERT_EQ(expected, actual);
}
