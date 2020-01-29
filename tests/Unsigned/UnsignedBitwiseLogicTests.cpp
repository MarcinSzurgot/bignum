#include "../TestHelpers.hpp"

#include <bignum/Operators/BitwiseLogic.hpp>

#include <gtest/gtest.h>

TEST(UnsignedBitwiseLogicTests, testThatPerformsBitwiseAnd)
{
    // given
    const auto lhs = bignum::Unsigned
    {
        std::uint8_t(0b10101010),
        std::uint8_t(0b00000000),
        std::uint8_t(0b11110000),
    };
    const auto rhs = bignum::Unsigned
    {
        std::uint8_t(0b10100000),
        std::uint8_t(0b00000000),
        std::uint8_t(0b00111100),
    };
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(0b10100000),
        std::uint8_t(0b00000000),
        std::uint8_t(0b00110000),
    };

    // when
    const auto actual = lhs & rhs;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedBitwiseLogicTests, testThatPerformsBitwiseOr)
{
    // given
    const auto lhs = bignum::Unsigned
    {
        std::uint8_t(0b10101010),
        std::uint8_t(0b00000000),
        std::uint8_t(0b11110000),
    };
    const auto rhs = bignum::Unsigned
    {
        std::uint8_t(0b10100000),
        std::uint8_t(0b00000000),
        std::uint8_t(0b00111100),
    };
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(0b10101010),
        std::uint8_t(0b00000000),
        std::uint8_t(0b11111100),
    };

    // when
    const auto actual = lhs | rhs;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedBitwiseLogicTests, testThatPerformsBitwiseXor)
{
    // given
    const auto lhs = bignum::Unsigned
    {
        std::uint8_t(0b10101010),
        std::uint8_t(0b00000000),
        std::uint8_t(0b11110000),
    };
    const auto rhs = bignum::Unsigned
    {
        std::uint8_t(0b10100000),
        std::uint8_t(0b00000000),
        std::uint8_t(0b00111100),
    };
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(0b00001010),
        std::uint8_t(0b00000000),
        std::uint8_t(0b11001100),
    };

    // when
    const auto actual = lhs ^ rhs;

    // then
    ASSERT_EQ(expected, actual);
}
