#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

TEST(Unsigned, LessMagnitudeIsLessThan)
{
    // given
    const auto lessMagnitude = bignum::Unsigned();
    const auto greaterMagnitude = bignum::Unsigned({1, 2, 3});

    // when
    const auto comparison = compare(lessMagnitude, greaterMagnitude);

    // then
    ASSERT_EQ(comparison, bignum::Comparison::LT);
}
