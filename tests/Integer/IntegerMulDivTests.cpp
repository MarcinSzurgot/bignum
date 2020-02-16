#include "../TestHelpers.hpp"

#include <bignum/Integer.hpp>

#include <gtest/gtest.h>

TEST(IntegerMulDivTests, testThatMultiplicationOfPlusesGivesPlus)
{
    // given
    const auto sign = bignum::Sign::Plus;
    const auto lhs = bignum::Integer(sign, bignum::Unsigned{1u, 1u});
    const auto rhs = bignum::Integer(sign, bignum::Unsigned{1u});
    const auto expected = sign;

    // when
    const auto actual = (lhs * rhs).sign();

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerMulDivTests, testThatMultiplicationOfMinusAndPlusGivesMinus)
{
    // given
    const auto minus = bignum::Integer(bignum::Sign::Minus, bignum::Unsigned{1u, 1u, 1u});
    const auto plus  = bignum::Integer(bignum::Sign::Plus,  bignum::Unsigned{1u, 1u, 1u});
    const auto expected = bignum::Sign::Minus;

    for (const auto& [lhs, rhs] : {std::make_pair(minus, plus),
                                   std::make_pair(plus, minus)})
    {
        // when
        const auto actual = (lhs * rhs).sign();

        // then
        ASSERT_EQ(expected, actual);
    }
}

TEST(IntegerMulDivTests, testThatMultiplicationOfMinusAndZeroGivesPlus)
{
    // given
    const auto zero  = bignum::Integer<unsigned>();
    const auto minus = bignum::Integer(bignum::Sign::Minus, bignum::Unsigned{1u, 1u});
    const auto expected = bignum::Sign::Plus;

    // when
    const auto actual = (zero * minus).sign();

    // then
    ASSERT_EQ(expected, actual);
}
