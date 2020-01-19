#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

TEST(UnsignedTests, testThatListInitializerTrimsLeadingZeroes)
{
    // given
    const auto digits = {1u, 1u, 1u, 0u, 0u, 0u};
    const auto expected = bignum::Unsigned{1u, 1u, 1u};

    // when
    const auto actual = bignum::Unsigned(digits);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedTests, testThatListInitializerLeavesOneDigit)
{
    // given
    const auto digits = {0u, 0u, 0u};
    const auto expected = bignum::Unsigned<unsigned>();

    // when
    const auto actual = bignum::Unsigned(digits);

    // then
    ASSERT_EQ(expected, actual);
}
