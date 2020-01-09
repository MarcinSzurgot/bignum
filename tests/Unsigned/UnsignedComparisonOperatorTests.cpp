#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

TEST(UnsignedComparisonOperatorTests, testThatValueWithLessMagnitudeIsLessThan)
{
    using unsigned_type = bignum::Unsigned<int>;

    // given
    const auto lhs = unsigned_type{1, 1, 1, 1};
    const auto rhs = unsigned_type{1, 1, 1, 1, 1};
    const auto expected = bignum::Comparison::LT;

    // when
    const auto actual = bignum::compare(lhs, rhs);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedComparisonOperatorTests, testThatValueWithBiggerMangitudeisGreaterThan)
{
    using unsigned_type = bignum::Unsigned<int>;

    // given
    const auto lhs = unsigned_type{1, 1, 1, 1, 1};
    const auto rhs = unsigned_type{1, 1, 1, 1};
    const auto expected = bignum::Comparison::GT;

    // when
    const auto actual = bignum::compare(lhs, rhs);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedComparisonOperatorTests, testThatLessDigitIsLessThan)
{
    using unsigned_type = bignum::Unsigned<int>;

    // given
    const auto lhs = unsigned_type{1, 1, 1, 1, 1};
    const auto rhs = unsigned_type{1, 1, 1, 1, 2};
    const auto expected = bignum::Comparison::LT;

    // when
    const auto actual = bignum::compare(lhs, rhs);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedComparisonOperatorTests, testThatBiggerDigitIsGreaterThan)
{
    using unsigned_type = bignum::Unsigned<int>;

    // given
    const auto lhs = unsigned_type{1, 1, 1, 1, 2};
    const auto rhs = unsigned_type{1, 1, 1, 1, 1};
    const auto expected = bignum::Comparison::GT;

    // when
    const auto actual = bignum::compare(lhs, rhs);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedComparisonOperatorTests, testThatEqualValuesAreEqual)
{
    using unsigned_type = bignum::Unsigned<int>;

    // given
    const auto lhs = unsigned_type{1, 1, 1, 1, 1};
    const auto rhs = unsigned_type{1, 1, 1, 1, 1};
    const auto expected = bignum::Comparison::EQ;

    // when
    const auto actual = bignum::compare(lhs, rhs);

    // then
    ASSERT_EQ(expected, actual);
}
