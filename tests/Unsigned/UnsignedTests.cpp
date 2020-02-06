#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

#include <vector>

TEST(UnsignedTests, testThatListInitializerTrimsLeadingZeroes)
{
    // given
    const auto digits = std::vector{1u, 1u, 1u, 0u, 0u, 0u};
    const auto expected = bignum::Unsigned{1u, 1u, 1u};

    // when
    const auto actual = bignum::Unsigned(digits);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedTests, testThatListInitializerLeavesOneDigit)
{
    // given
    const auto digits = std::vector{0u, 0u, 0u};
    const auto expected = bignum::Unsigned<unsigned>();

    // when
    const auto actual = bignum::Unsigned(digits);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedTests, testThatInitsFromString)
{
    // given
    const auto string = "65536";
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(1)
    };

    // when
    const auto actual = bignum::Unsigned<std::uint8_t>(string);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedTests, testThatInitFromStringIgnoresTrailingWhitespaces)
{
    // given
    const auto string = "  \n65536  \t \v";
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(1)
    };

    // when
    const auto actual = bignum::Unsigned<std::uint8_t>(string);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedTests, testThatInitFromStringIgnoresLeadingZeroes)
{
    // given
    const auto string = "000065536";
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(1)
    };

    // when
    const auto actual = bignum::Unsigned<std::uint8_t>(string);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedTests, testThatInitFromBiggerUint)
{
    // given
    const auto value = std::uint32_t((9 << 24u) | (11 << 16u) | (243 << 8u) | (23 << 0u));
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(23),
        std::uint8_t(243),
        std::uint8_t(11),
        std::uint8_t(9),
    };

    // when
    const auto actual = bignum::Unsigned<std::uint8_t>(value);

    // then
    ASSERT_EQ(expected, actual);
}
