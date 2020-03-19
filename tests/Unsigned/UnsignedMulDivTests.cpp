#include "../TestHelpers.hpp"

#include <bignum/Unsigned.hpp>
#include <gtest/gtest.h>

#include <array>
#include <chrono>
#include <random>

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
    constexpr auto allUint8Values = []()
    {
        auto values = std::array<std::uint8_t, 256>();
        for (auto i = 0u; i < size(values); ++i)
        {
            values[i] = i;
        }
        return values;
    }();

    for (const auto lhs : allUint8Values)
    {
        for (const auto rhs : allUint8Values)
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
        std::uint8_t(255),
        std::uint8_t(255),
        std::uint8_t(255),
    };
    const auto rhs = bignum::Unsigned
    {
        std::uint8_t(255),
        std::uint8_t(255),
        std::uint8_t(255),
        std::uint8_t(255),
    };
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(1),
        std::uint8_t(0),
        std::uint8_t(0),
        std::uint8_t(0),
        std::uint8_t(254),
        std::uint8_t(255),
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

TEST(UnsignedMulDivTests, testThatMultipliesRandomValues)
{
    // given
    constexpr auto numberOfIterations = 100000u;
    constexpr auto maxSize = 5u;
    const auto seed = std::uint64_t
    (
        std::chrono::high_resolution_clock::now()
        .time_since_epoch()
        .count()
    );
    auto generator = std::mt19937_64(seed);

    for (auto i = 0u; i < numberOfIterations; ++i)
    {
        // given
        const auto lhs = randomUnsigned<std::uint32_t>(generator, maxSize);
        const auto rhs = randomUnsigned<std::uint32_t>(generator, maxSize);
        const auto expected = additionBasedMultiplication(lhs, rhs);

        // when
        const auto actual = lhs * rhs;

        // then
        ASSERT_EQ(expected, actual)
            << "lhs:      " << toString(lhs) << "\n"
            << "rhs:      " << toString(rhs) << "\n"
            << "Expected: " << toString(expected) << "\n"
            << "Actual:   " << toString(actual);
    }
}

TEST(UnsignedMulDivTests, testThatDividesAllSingleDigitsValues)
{
    // given
    constexpr auto allUint8Values = []()
    {
        auto values = std::array<std::uint8_t, 256>();
        for (auto digit = 0u; digit < size(values); ++digit)
        {
            values[digit] = digit;
        }
        return values;
    }();

    for (const auto a : allUint8Values)
    {
        for (const auto b : allUint8Values)
        {
            for (const auto c : allUint8Values)
            {
                // given
                const auto dividend = std::make_pair
                (
                    std::uint8_t(a),
                    std::uint8_t(b)
                );
                const auto divisor = c;

                if (!divisor)
                {
                    continue;
                }

                const auto quotient = std::uint32_t(b * 256 + a) / std::uint32_t(c);
                const auto expected = std::make_pair
                (
                    std::uint8_t(quotient & 255),
                    std::uint8_t((quotient >> 8u) & 255)
                );

                // when
                const auto actual = bignum::div(dividend, divisor);

                // then
                ASSERT_EQ(expected, actual)
                    << "Dividend: {" << +dividend.first << ", " << +dividend.second << "}\n"
                    << "Divisor:   " << +divisor << "\n"
                    << "Quotient:  " << +quotient << "\n"
                    << "Expected: {" << +expected.first << ", " << +expected.second << "}\n"
                    << "Actual:   {" << +actual.first << ", " << +actual.second << "}\n";
            }
        }
    }
}

TEST(UnsignedMulDivTests, testThatDividesLessByGreater)
{
    // given
    const auto less = bignum::Unsigned{0u, 0u, 1u};
    const auto greater = bignum::Unsigned{0u, 0u, 0u, 1u};
    const auto expected = bignum::Unsigned{0u};

    // when
    const auto actual = less / greater;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedMulDivTests, testThatDividesByLessMsdDivisor)
{
    // given
    const auto lhs = bignum::Unsigned{10u};
    const auto rhs = bignum::Unsigned{3u};
    const auto expected = bignum::Unsigned{3u};

    // when
    const auto actual = lhs / rhs;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedMulDivTests, testThatDividesByEqualMsdDivisor)
{
    // given
    const auto lhs = bignum::Unsigned{std::uint8_t(), std::uint8_t(8)};
    const auto rhs = bignum::Unsigned{std::uint8_t(8)};
    const auto expected = bignum::Unsigned{std::uint8_t(), std::uint8_t(1)};

    // when
    const auto actual = lhs / rhs;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedMulDivTests, testThatDividesByGreaterMsdDivisor)
{
    // given
    const auto lhs = bignum::Unsigned{std::uint8_t(), std::uint8_t(8)};
    const auto rhs = bignum::Unsigned{std::uint8_t(9)};
    const auto expected = bignum::Unsigned{std::uint8_t(227)};

    // when
    const auto actual = lhs / rhs;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedMulDivTests, testThatDividesByLessMsdDivisorExtremeCase)
{
    // given
    const auto lhs = bignum::Unsigned{std::uint8_t(), std::uint8_t(), std::uint8_t(8)};
    const auto rhs = bignum::Unsigned{std::uint8_t(255), std::uint8_t(9)};
    const auto expected = bignum::Unsigned{std::uint8_t(204)};

    // when
    const auto actual = lhs / rhs;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedMulDivTests, testThatDividesByEqualMsdDivisorExtremeCase)
{
    // given
    const auto lhs = bignum::Unsigned{std::uint8_t(), std::uint8_t(), std::uint8_t(17)};
    const auto rhs = bignum::Unsigned{std::uint8_t(255), std::uint8_t(17)};
    const auto expected = bignum::Unsigned{std::uint8_t(241)};

    // when
    const auto actual = lhs / rhs;

    // then
    ASSERT_EQ(expected, actual)
        << "Expected: " << toString(expected) << "\n"
        << "Actual:   " << toString(actual) << "\n";
}

TEST(UnsignedMulDivTests, testThatDividesRandomValues)
{
    // given
    constexpr auto numberOfIterations = 100000u;
    constexpr auto maxSize = 5u;
    const auto seed = std::uint64_t
    (
        std::chrono::high_resolution_clock::now()
        .time_since_epoch()
        .count()
    );
    auto generator = std::mt19937_64(seed);

    for (auto i = 0u; i < numberOfIterations; ++i)
    {
        // given
        const auto divider = randomUnsigned<std::uint32_t>(generator, maxSize) + bignum::Unsigned{std::uint32_t(1)};
        const auto expected = randomUnsigned<std::uint32_t>(generator, maxSize);
        const auto multiplied = divider * expected;

        // when
        const auto actual = multiplied / divider;

        // then
        ASSERT_EQ(expected, actual)
            << "divider:    " << toString(divider) << "\n"
            << "multiplied: " << toString(multiplied) << "\n"
            << "expected:   " << toString(expected) << "\n"
            << "actual:     " << toString(actual) << "\n";
    }
}

TEST(UnsignedMulDivTests, testThatDivisionThrowsWhenDividingByZero)
{
    // given
    const auto zero = bignum::Unsigned{0u};
    const auto one  = bignum::Unsigned{1u};

    // when-then
    ASSERT_THROW(one / zero, std::invalid_argument);
}
