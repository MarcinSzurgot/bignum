#include <bignum/Integer.hpp>

#include <gtest/gtest.h>

TEST(IntegerTests, testThatInitsFromDigit)
{
    // given
    const auto digit = 100;
    const auto expected = bignum::Integer(bignum::Sign::Plus, bignum::Unsigned{100u});

    // when
    const auto actual = bignum::Integer<unsigned>(digit);

    // then
    // ASSERT_EQ(expected, actual);
}
