#include "../TestHelpers.hpp"

#include <bignum/Unsigned.hpp>
#include <gtest/gtest.h>

#include <array>

TEST(UnsignedBitshiftTests, testThatLeftShiftsByWholeDigits)
{
    // given
    const auto offset = 5u;
    const auto bitOffset = offset * sizeof(unsigned) * CHAR_BIT;
    const auto initial = bignum::Unsigned{1u, 1u, 1u};
    const auto expected = initial.magnitude() + offset;

    // when
    const auto actual = (initial << bitOffset).magnitude();

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedBitshiftTests, testThatLeftShiftsDigits)
{
    // given
    const auto offset = 4u;
    const auto bitOffset = offset * sizeof(unsigned) * CHAR_BIT;
    const auto initial = bignum::Unsigned{1u, 2u, 3u};
    const auto expected = bignum::Unsigned{0u, 0u, 0u, 0u, 1u, 2u, 3u};

    // when
    const auto actual = initial << bitOffset;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedBitshiftTests, testThatLeftShiftsDigitPart)
{
    // given
    const auto bitOffset = 3;
    const auto initial = bignum::Unsigned
    {
        std::uint8_t(0b10000001),
        std::uint8_t(0b10000001)
    };
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(0b00001000),
        std::uint8_t(0b00001100),
        std::uint8_t(0b00000100)
    };

    // when
    const auto actual = initial << bitOffset;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedBitshiftTests, testThatLeftShiftsDigitPartButNoExtraDigit)
{
    // given
    const auto bitOffset = 3;
    const auto initial = bignum::Unsigned
    {
        std::uint8_t(0b10000001),
        std::uint8_t(0b00000001)
    };
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(0b00001000),
        std::uint8_t(0b00001100)
    };

    // when
    const auto actual = initial << bitOffset;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedBitshiftTests, testThatLeftShiftsZeroToZero)
{
    // given
    const auto bitOffset = 1000u;
    const auto expected = bignum::Unsigned<unsigned>();

    // when
    const auto actual = expected << bitOffset;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedBitshiftTests, testThatRightShiftsByWholeDigits)
{
    // given
    const auto digitOffset = 3u;
    const auto bitOffset = sizeof(unsigned) * CHAR_BIT * digitOffset;
    const auto initial = bignum::Unsigned{1u, 2u, 3u, 4u, 5u, 6u};
    const auto expected = bignum::Unsigned{4u, 5u, 6u};

    // when
    const auto actual = initial >> bitOffset;

    // then
    ASSERT_EQ(expected, actual) << "Expected: " << toString(expected) << "\nActual: " << toString(actual);
}

TEST(UnsignedBitshiftTests, testThatRightShiftsDigitsPart)
{
    // given
    const auto bitOffset = 5u;
    const auto initial = bignum::Unsigned
    {
        std::uint8_t(0b10000001),
        std::uint8_t(0b10000001),
        std::uint8_t(0b10000001),
    };
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(0b00001100),
        std::uint8_t(0b00001100),
        std::uint8_t(0b00000100),
    };

    // when
    const auto actual = initial >> bitOffset;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedBitshiftTests, testThatRightShiftsDigitsPartButNoExtraDigit)
{
    // given
    const auto bitOffset = 5u;
    const auto initial = bignum::Unsigned
    {
        std::uint8_t(0b10000001),
        std::uint8_t(0b10000001),
        std::uint8_t(0b00000001),
    };
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(0b00001100),
        std::uint8_t(0b00001100),
    };

    // when
    const auto actual = initial >> bitOffset;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedBitshiftTests, testThatRightShiftsToZero)
{
    // given
    const auto bitOffset = 1000u;
    const auto initial = bignum::Unsigned{1u, 1u, 1u, 1u};
    const auto expected = bignum::Unsigned<unsigned>();

    // when
    const auto actual = initial >> bitOffset;

    // then
    ASSERT_EQ(expected, actual) << "Expected: " << toString(expected) << "\nActual: " << toString(actual);
}

TEST(UnsignedBitshiftTests, testThatLeftShiftsAllValues)
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
    constexpr auto bitShiftLimit = 16u;

    for (const auto a : allUint8Values)
    {
        for (const auto b : allUint8Values)
        {
            for (auto bitShift = 0u; bitShift < bitShiftLimit; ++bitShift)
            {
                // given
                const auto initial = bignum::Unsigned{a, b};
                const auto initialSingleInt = std::uint32_t((b << 8u) | a);
                const auto result = initialSingleInt << bitShift;
                const auto expected = bignum::Unsigned
                {
                    std::uint8_t((result >>  0u) & 255),
                    std::uint8_t((result >>  8u) & 255),
                    std::uint8_t((result >> 16u) & 255),
                    std::uint8_t((result >> 24u) & 255),
                };

                // when
                const auto actual = initial << bitShift;

                // then
                ASSERT_EQ(expected, actual)
                    << "initial single int: " << initialSingleInt << "\n"
                    << "result:             " << result << "\n"
                    << "a:                  " << +a << "\n"
                    << "b:                  " << +b << "\n"
                    << "bit shift:          " << bitShift << "\n"
                    << "Expected:           " << toString(expected) << "\n"
                    << "Actual:             " << toString(actual) << "\n";
            }
        }
    }
}

TEST(UnsignedBitshiftTests, testThatRightShiftsAllValues)
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
    constexpr auto bitShiftLimit = 16u;

    for (const auto a : allUint8Values)
    {
        for (const auto b : allUint8Values)
        {
            for (auto bitShift = 0u; bitShift < bitShiftLimit; ++bitShift)
            {
                // given
                const auto initial = bignum::Unsigned{a, b};
                const auto initialSingleInt = std::uint32_t((b << 8u) | a);
                const auto result = initialSingleInt >> bitShift;
                const auto expected = bignum::Unsigned
                {
                    std::uint8_t((result >>  0u) & 255),
                    std::uint8_t((result >>  8u) & 255),
                    std::uint8_t((result >> 16u) & 255),
                    std::uint8_t((result >> 24u) & 255),
                };

                // when
                const auto actual = initial >> bitShift;

                // then
                ASSERT_EQ(expected, actual)
                    << "initial:            " << toString(initial) << "\n"
                    << "initial single int: " << initialSingleInt << "\n"
                    << "result:             " << result << "\n"
                    << "a:                  " << +a << "\n"
                    << "b:                  " << +b << "\n"
                    << "bit shift:          " << bitShift << "\n"
                    << "Expected:           " << toString(expected) << "\n"
                    << "Actual:             " << toString(actual) << "\n";
            }
        }
    }
}
