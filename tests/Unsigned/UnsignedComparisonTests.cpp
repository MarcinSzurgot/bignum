#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

TEST(UnsignedComparisonTests, testThatLessMagnitudeIsLess)
{
    // given
    const auto less    = bignum::Unsigned{1u, 1u};
    const auto greater = bignum::Unsigned{1u, 1u, 1u};

    // then
    ASSERT_LT(less, greater);
}

TEST(UnsignedComparisonTests, testThatGreaterMagnitudeIsGreater)
{
    // given
    const auto less    = bignum::Unsigned{1u, 1u, 1u};
    const auto greater = bignum::Unsigned{1u, 1u, 1u, 1u, 1u, 1u};

    // then
    ASSERT_GT(greater, less);
}

TEST(UnsignedComparisonTests, testThatLessValueIsLess)
{
    // given
    const auto less    = bignum::Unsigned{1u, 1u, 1u};
    const auto greater = bignum::Unsigned{2u, 1u, 1u};

    // then
    ASSERT_LT(less, greater);
}

TEST(UnsignedComparisonTests, testThatGreaterValueIsGreater)
{
    // given
    const auto less    = bignum::Unsigned{2u, 2u, 2u, 2u};
    const auto greater = bignum::Unsigned{1u, 1u, 1u, 3u};

    // then
    ASSERT_GT(greater, less);
}

TEST(UnsignedComparisonTests, testThatEqualValueIsEqual)
{
    // given
    const auto lhs = bignum::Unsigned{2u, 2u, 2u, 2u};
    const auto rhs = bignum::Unsigned{2u, 2u, 2u, 2u};

    // then
    ASSERT_EQ(lhs.magnitude(), rhs.magnitude());
    ASSERT_EQ(lhs, rhs);
}
