#include <bignum/Unsigned.hpp>
#include <bignum/Utility.hpp>

#include <gtest/gtest.h>

TEST(UtilityTests, testThatReturnsHighestBitNumber)
{
    // given
    const auto expected = 5u;
    const auto value = 1 << expected;

    // when
    const auto actual = bignum::highestBitNumber(value).value();

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UtilityTests, testThatReturnsNoneBitNumber)
{
    // given
    const auto expected = std::nullopt;
    const auto value = 0;

    // when
    const auto actual = bignum::highestBitNumber(value);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UtilityTests, testThatReturnsHighestBitNumberForBignum)
{
    // given
    const auto expected = 64u;
    const auto value = bignum::Unsigned
    {
        std::uint32_t(),
        std::uint32_t(),
        std::uint32_t(1),
    };

    // when
    const auto actual = bignum::highestBitNumber(value).value();

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UtilityTests, testThatReturnsNoneBitNumberForBignum)
{
    // given
    const auto expected = std::nullopt;
    const auto value = bignum::Unsigned<unsigned>();

    // when
    const auto actual = bignum::highestBitNumber(value);

    // then
    ASSERT_EQ(expected, actual);
}
