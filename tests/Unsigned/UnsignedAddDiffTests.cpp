#include "../TestHelpers.hpp"

#include <bignum/Operators/AddDiff.hpp>
#include <gtest/gtest.h>

#include <array>

TEST(UnsignedAddDiffTests, testThatAddsSingleDigits)
{
    // given
    const auto lhs      = std::uint8_t(20);
    const auto rhs      = std::uint8_t(40);
    const auto expected = std::make_pair(std::uint8_t(60), std::uint8_t(0));

    // when
    const auto actual = bignum::add(lhs, rhs);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedAddDiffTests, testThatAddsSingleDigitsWithOverflow)
{
    // given
    const auto lhs      = std::uint8_t(50);
    const auto rhs      = std::uint8_t(255);
    const auto expected = std::make_pair(std::uint8_t(49), std::uint8_t(1));

    // when
    const auto actual = bignum::add(lhs, rhs);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedAddDiffTests, testThatAddsSingleDigitsWithCarry)
{
    // given
    const auto lhs      = std::uint8_t(100);
    const auto rhs      = std::uint8_t(100);
    const auto carry    = std::uint8_t(1);
    const auto expected = std::make_pair(std::uint8_t(201), std::uint8_t(0));

    // when
    const auto actual = bignum::add(lhs, rhs, carry);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedAddDiffTests, testThatAddsSingleDigitsWithCarryOverflow)
{
    // given
    const auto lhs      = std::uint8_t(100);
    const auto rhs      = std::uint8_t(155);
    const auto carry    = std::uint8_t(1);
    const auto expected = std::make_pair(std::uint8_t(0), std::uint8_t(1));

    // when
    const auto actual = bignum::add(lhs, rhs, carry);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedAddDiffTests, testThatAddsAllSingleDigits)
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
        for(const auto rhs : values)
        {
            for (const auto carry : {std::uint8_t(0), std::uint8_t(1)})
            {
                const auto result = std::uint16_t
                (
                    std::uint16_t(lhs)
                    + std::uint16_t(rhs)
                    + std::uint16_t(carry)
                );
                const auto expected = std::make_pair
                (
                    std::uint8_t(result & 255),
                    std::uint8_t(result >> 8u)
                );

                // when
                const auto actual = bignum::add(lhs, rhs, carry);

                // then
                ASSERT_EQ(expected, actual)
                    << "Lhs:   " << toBinString(lhs) << "\n"
                    << "Rhs:   " << toBinString(rhs) << "\n"
                    << "Carry: " << toBinString(carry) << "\n"
                    << "Expected: {" << toBinString(expected.first) << ", " << toBinString(expected.second) << "}\n"
                    << "Actual:   {" << toBinString(actual.first) << ", " << toBinString(actual.second) << "}\n";
            }
        }
    }
}

TEST(UnsignedAddDiffTests, testThatSimplyAddsDigits)
{
    // given
    const auto lhs = bignum::Unsigned{1u, 2u, 3u};
    const auto rhs = bignum::Unsigned{4u, 5u, 6u};
    const auto expected = bignum::Unsigned{5u, 7u, 9u};

    // when
    const auto actual = lhs + rhs;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedAddDiffTests, testThatSimplyAddsDigitsWithOverflow)
{
    // given
    const auto lhs = bignum::Unsigned
    {
        std::uint8_t(255),
        std::uint8_t(255),
        std::uint8_t(1),
    };
    const auto rhs = bignum::Unsigned
    {
        std::uint8_t(1),
        std::uint8_t(1),
    };
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(0),
        std::uint8_t(1),
        std::uint8_t(2),
    };

    // when
    const auto actual = lhs + rhs;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedAddDiffTests, testThatAddsWithDigitOverflow)
{
    // given
    const auto lhs = bignum::Unsigned
    {
        std::uint8_t(255),
        std::uint8_t(255),
        std::uint8_t(255),
    };
    const auto rhs = bignum::Unsigned
    {
        std::uint8_t(1)
    };
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(1),
    };

    // when
    const auto actual = lhs + rhs;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedAddDiffTests, testThatAddsGreaterToLess)
{
    // given
    const auto greater = bignum::Unsigned
    {
        std::uint8_t(255),
        std::uint8_t(255),
        std::uint8_t(255),
    };
    const auto less = bignum::Unsigned
    {
        std::uint8_t(1)
    };
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(1),
    };

    // when
    const auto actual = less + greater;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedAddDiffTests, testThatSimplyDiffs)
{
    // given
    const auto lhs = bignum::Unsigned
    {
        std::uint8_t(100),
        std::uint8_t(100),
        std::uint8_t(100),
    };
    const auto rhs = bignum::Unsigned
    {
        std::uint8_t(20),
        std::uint8_t(20),
        std::uint8_t(20),
    };
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(80),
        std::uint8_t(80),
        std::uint8_t(80),
    };

    // when
    const auto actual = lhs - rhs;

    // then
    ASSERT_EQ(expected, actual) << "Expected: " << toString(expected) << "\nActual: " << toString(actual);
}

TEST(UnsignedAddDiffTests, testThatDiffsWithUnderflow)
{
    // given
    const auto lhs = bignum::Unsigned
    {
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(1),
    };
    const auto rhs = bignum::Unsigned
    {
        std::uint8_t(1)
    };
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(255),
        std::uint8_t(255),
    };

    // when
    const auto actual = lhs - rhs;

    // then
    ASSERT_EQ(expected, actual) << "Expected: " << toString(expected) << "\nActual: " << toString(actual);
}

TEST(UnsignedAddDiffTests, testThatSubtractsGreaterFromLess)
{
    // given
    const auto greater = bignum::Unsigned
    {
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(1),
    };
    const auto less = bignum::Unsigned
    {
        std::uint8_t(1)
    };
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(255),
        std::uint8_t(255),
    };

    // when
    const auto actual = less - greater;

    // then
    ASSERT_EQ(expected, actual) << "Expected: " << toString(expected) << "\nActual: " << toString(actual);
}

TEST(UnsignedAddDiffTests, testThatEvenDiffsByZero)
{
    // given
    const auto lhs = bignum::Unsigned
    {
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(1),
    };
    const auto rhs = bignum::Unsigned
    {
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(1),
    };
    const auto expected = bignum::Unsigned<std::uint8_t>();

    // when
    const auto actual = lhs - rhs;

    // then
    ASSERT_EQ(expected, actual) << "Expected: " << toString(expected) << "\nActual: " << toString(actual);
}
