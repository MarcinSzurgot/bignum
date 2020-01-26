#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

#include <array>
#include <sstream>

namespace
{

template<typename T>
std::string toString(const bignum::Unsigned<T>& value)
{
    auto stream = std::stringstream();
    stream << "{";
    for (auto d = 0u; d < value.magnitude(); ++d)
    {
        stream << +value[d] << ", ";
    }
    stream << "}";
    return stream.str();
}

template<typename Integer>
std::string toBinString(Integer value)
{
    constexpr auto bitSize = sizeof(Integer) * CHAR_BIT;

    auto binary = std::string(bitSize, '0');
    for (auto& bit : binary)
    {
        bit += static_cast<bool>(value & (Integer(1) << (bitSize - 1)));
        value <<= 1;
    }
    return binary;
}

}

TEST(UnsignedMulDivTests, testThatSingleDigitMultipliesForSimpleCase)
{
    // given
    const auto lhs = 3u;
    const auto rhs = 9u;
    const auto expected = std::make_pair(27u, 0u);

    // when
    const auto actual = bignum::mul(lhs, rhs);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedMulDivTests, testThatSingleDigitMultipliesLowerHalves)
{
    // given
    const auto lhs = std::uint8_t(0b00001111);
    const auto rhs = std::uint8_t(0b00001111);
    const auto expected = std::make_pair(std::uint8_t(0b11100001), std::uint8_t(0b00000000));

    // when
    const auto actual = bignum::mul(lhs, rhs);

    // then
    ASSERT_EQ(expected, actual) << "Actual: {" << toBinString(actual.first) << ", " << toBinString(actual.second) << "}";
}

TEST(UnsignedMulDivTests, testThatSingleDigitMultipliesMixedHalves)
{
    // given
    const auto lhs = std::uint8_t(0b11110000);
    const auto rhs = std::uint8_t(0b00001111);
    const auto expected = std::make_pair
    (
        std::uint8_t(0b00010000),
        std::uint8_t(0b00001110)
    );

    // when
    const auto actual = bignum::mul(lhs, rhs);

    // then
    ASSERT_EQ(expected, actual)
        << "Actual: {" << toBinString(actual.first) << ", " << toBinString(actual.second) << "}\n"
        << "Expected: {" << toBinString(expected.first) << ", " << toBinString(expected.second);
}

TEST(UnsignedMulDivTests, testThatSingleDigitMultipliesHigherHalves)
{
    // given
    const auto lhs      = std::uint8_t(0b11111111);
    const auto rhs      = std::uint8_t(0b11111111);
    const auto expected = std::uint8_t(0b11111110);

    // when
    const auto [_, actual] = bignum::mul(lhs, rhs);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedMulDivTests, testThatSingleDigitMultipliesForAllDigits)
{
    // given
    constexpr auto values = []()
    {
        auto values = std::array<std::uint8_t, 256>();
        for (auto i = 0u; i < size(values); ++i)
        {
            values[i] = i;
        }
        return values;
    }();

    for (const auto lhs : values)
    {
        for (const auto rhs : values)
        {
            const auto result = std::uint16_t(lhs * rhs);
            const auto expected = std::make_pair
            (
                std::uint8_t(result & 255),
                std::uint8_t(result >> 8u)
            );

            // when
            const auto actual = bignum::mul(lhs, rhs);

            // then
            ASSERT_EQ(expected, actual)
                << "Lhs: " << toBinString(lhs) << "\n"
                << "Rhs: " << toBinString(rhs) << "\n"
                << "result: " << toBinString(result) << "\n"
                << "Actual: {" << toBinString(actual.first) << ", " << toBinString(actual.second) << "}\n"
                << "Expected: {" << toBinString(expected.first) << ", " << toBinString(expected.second) << "}\n";
        }
    }
}

TEST(UnsignedMulDivTests, testThatSimplyMultipliesByDigit)
{
    // given
    const auto digit = bignum::Unsigned{3u};
    const auto value = bignum::Unsigned{3u, 2u, 10u};
    const auto expected = bignum::Unsigned{9u, 6u, 30u};

    // when
    const auto actual = digit * value;

    // then
    ASSERT_EQ(expected, actual) << "Expected: " << toString(expected) << "\nActual: " << toString(actual);
}

TEST(UnsignedMulDivTests, testThatMultipliesDigitByDigit)
{
    // given
    const auto lhs = bignum::Unsigned{2u, 3u, 4u};
    const auto rhs = bignum::Unsigned{3u, 2u, 10u};
    const auto expected = bignum::Unsigned{6u, 13u, 38u, 38u, 40u};

    // when
    const auto actual = lhs * rhs;

    // then
    ASSERT_EQ(expected, actual) << "Expected: " << toString(expected) << "\nActual: " << toString(actual);
}

TEST(UnsignedMulDivTests, testThatMultipliesWithOverflow)
{
    // given
    const auto lhs = bignum::Unsigned
    {
        std::uint8_t(255),
        // std::uint8_t(255),
        // std::uint8_t(255),
    };
    const auto rhs = bignum::Unsigned
    {
        std::uint8_t(255),
        // std::uint8_t(255),
        // std::uint8_t(255),
    };
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(1),
        std::uint8_t(0),
        std::uint8_t(0),

        std::uint8_t(254),
        std::uint8_t(255),
        std::uint8_t(255),
    };

    // when
    const auto actual = lhs * rhs;

    // then
    ASSERT_EQ(expected, actual)
        << "Expected: " << toString(expected) << "\n"
        << "Actual:   " << toString(actual);
}
