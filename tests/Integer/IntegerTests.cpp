#include <bignum/Integer.hpp>

#include <gtest/gtest.h>

TEST(IntegerTests, testThatInitsFromDigit)
{
    // given
    const auto digit = -100;
    const auto expected = bignum::Integer(bignum::Sign::Minus, bignum::Unsigned{100u});

    // when
    const auto actual = bignum::Integer<unsigned>(digit);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerTests, testThatInitsFromMaxNegativeDigit)
{
    // given
    const auto min = std::numeric_limits<std::int32_t>::lowest();
    const auto expected = bignum::Integer(bignum::Sign::Minus, bignum::Unsigned<unsigned>(std::uint32_t(1) << 31));

    // when
    const auto actual = bignum::Integer<unsigned>(min);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerTests, testThatInitsNegativeFromString)
{
    // given
    const auto string = "-65536";
    const auto expected = bignum::Integer<unsigned>(-65536);

    // when
    const auto actual = bignum::Integer<unsigned>(string);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerTests, testThatInitsFromStringWithLeadingSpaces)
{
    // given
    const auto string = "     39847";
    const auto expected = bignum::Integer<unsigned>(39847);

    // when
    const auto actual = bignum::Integer<unsigned>(string);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerTests, testThatInitFromStringIgnoresRestIfPlusAfterMinus)
{
    // given
    const auto string = "-+39847";
    const auto expected = bignum::Integer<unsigned>();

    // when
    const auto actual = bignum::Integer<unsigned>(string);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerTests, testThatInitFromStringDoesNotCreateMinusZero)
{
    // given
    const auto string = "-0";
    const auto expected = bignum::Integer<unsigned>();

    // when
    const auto actual = bignum::Integer<unsigned>(string);

    // then
    ASSERT_EQ(expected, actual);
}
