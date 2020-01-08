#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

TEST(UnsignedComparisonOperatorTests, testThatValueWithLessMagnitudeIsLess)
{
    using unsigned_type = bignum::Unsigned<int>;
    using digit_type = typename unsigned_type::digit_type;

    // given
    const auto lhs = unsigned_type{1, 1, 1, 1};
    const auto rhs = unsigned_type{1, 1, 1, 1, 1};
    const auto expected = bignum::Comparison::LT;

    // when
    const auto actual = compare(lhs, rhs);

    // then
    ASSERT_EQ(expected, actual);
}
