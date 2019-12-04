#include <bignum/Unsigned.hpp>

#include <bignum/Utility.hpp>

#include <gtest/gtest.h>

namespace
{

const auto bitsPerDigit = sizeof(bignum::digit_type) * 8u;

}

// Left Shift operator tests

TEST(UnsignedBinaryShiftTests, testThatLeftShiftExtendsWholeDigits)
{
    // given
    const auto initial = bignum::Unsigned
    ({
        bignum::digit_type(666),
        bignum::digit_type(237),
        bignum::digit_type(123)
    });
    const auto digitOffsets = 10u;

    for(auto digitOffset = 0u; digitOffset < digitOffsets; ++digitOffset)
    {
        // when
        const auto given = initial << digitOffset * bitsPerDigit;

        // then
        ASSERT_EQ(given.magnitude(), initial.magnitude() + digitOffset);
    }
}

TEST(UnsignedBinaryShiftTests, testThatLeftShiftDoesNotExtend)
{
    // given
    const auto initial = bignum::Unsigned
    ({
        bignum::digit_type(),
        bignum::digit_type(),
        bignum::digit_type(1u << (bitsPerDigit / 2u))
    });
    const auto expected = initial.magnitude();
    const auto highestBit = *bignum::highestBitNumber(initial.msd());
    const auto bitOffsets = bitsPerDigit - highestBit;

    for(auto bitOffset = 0u; bitOffset < bitOffsets; ++bitOffset)
    {
        // when
        const auto given = initial << bitOffset;

        // then
        ASSERT_EQ(given.magnitude(), expected);
    }
}

TEST(UnsignedBinaryShiftTests, testThatLeftShiftDoesExtend)
{
    // given
    const auto initial = bignum::Unsigned
    ({
        bignum::digit_type(),
        bignum::digit_type(),
        bignum::digit_type(1u << (bitsPerDigit / 2u))
    });
    const auto expected = initial.magnitude() + 1u;
    const auto highestBit = *bignum::highestBitNumber(initial.msd());
    const auto startBitOffset = bitsPerDigit - highestBit;
    const auto endBitOffset = bitsPerDigit;

    for(auto bitOffset = startBitOffset; bitOffset < endBitOffset; ++bitOffset)
    {
        // when
        const auto given = initial << bitOffset;

        // then
        ASSERT_EQ(given.magnitude(), expected);
    }
}

TEST(UnsignedBinaryShiftTests, testThatLeftShiftsWholeDigits)
{
    // given
    const auto expected = bignum::Unsigned
    ({
        bignum::digit_type(123),
        bignum::digit_type(53),
        bignum::digit_type(97)
    });
    const auto digitOffsets = 10u;

    for(auto digitOffset = 0u; digitOffset < digitOffsets; ++digitOffset)
    {
        // when
        const auto given = expected << digitOffset * bitsPerDigit;

        // then

        // offsetted part should be the same
        for(auto digit = 0u; digit < expected.magnitude(); ++digit)
        {
            ASSERT_EQ(given.digit(digit + digitOffset), expected.digit(digit));
        }

        // extra digits should be zero
        for(auto digit = 0u; digit < digitOffset; ++digit)
        {
            ASSERT_EQ(given.digit(digit), bignum::digit_type());
        }
    }
}

TEST(UnsignedBinaryShiftTests, testThatLeftShiftsDigitPart)
{
    // given
    const auto expected = bignum::digit_type(~bignum::digit_type());
    const auto bitOffset = 3 * bitsPerDigit / 4;
    const auto reversedBitOffset = bitsPerDigit - bitOffset;

    // when
    const auto given = bignum::Unsigned({expected}) << bitOffset;

    // then
    EXPECT_EQ(given.msd(), bignum::digit_type(expected >> reversedBitOffset));
    EXPECT_EQ(given.lsd(), bignum::digit_type(expected << bitOffset));
}

// Right shift operator tests

TEST(UnsignedBinaryShiftTests, testThatRightShiftExtendsWholeDigits)
{
    // given
    const auto initial = bignum::Unsigned
    ({
        bignum::digit_type(666),
        bignum::digit_type(237),
        bignum::digit_type(123),
        bignum::digit_type(223),
        bignum::digit_type(133),
        bignum::digit_type(13),
        bignum::digit_type(12),
    });
    const auto digitOffsets = 10u;

    for(auto digitOffset = 0u; digitOffset < digitOffsets; ++digitOffset)
    {
        // given
        const auto expectedMagnitude = digitOffset >= initial.magnitude() ? 1u : initial.magnitude() - digitOffset;

        // when
        const auto given = initial >> digitOffset * bitsPerDigit;

        // then
        ASSERT_EQ(given.magnitude(), expectedMagnitude);
    }
}

TEST(UnsignedBinaryShiftTests, testThatRighShiftDoesNotExtend)
{
    // given
    const auto initial = bignum::Unsigned
    ({
        bignum::digit_type(),
        bignum::digit_type(),
        bignum::digit_type(1u << (bitsPerDigit / 2u))
    });
    const auto expected = initial.magnitude();
    const auto highestBit = *bignum::highestBitNumber(initial.msd());
    const auto bitOffsets = bitsPerDigit - highestBit;

    for(auto bitOffset = 0u; bitOffset < bitOffsets; ++bitOffset)
    {
        // when
        const auto given = initial >> bitOffset;

        // then
        ASSERT_EQ(given.magnitude(), expected);
    }
}

TEST(UnsignedBinaryShiftTests, testThatRighShiftDoesExtend)
{
    // given
    const auto initial = bignum::Unsigned
    ({
        bignum::digit_type(),
        bignum::digit_type(),
        bignum::digit_type(1u << (bitsPerDigit / 2u))
    });
    const auto expected = initial.magnitude() + 1u;
    const auto highestBit = *bignum::highestBitNumber(initial.msd());
    const auto startBitOffset = bitsPerDigit - highestBit;
    const auto endBitOffset = bitsPerDigit;

    for(auto bitOffset = startBitOffset; bitOffset < endBitOffset; ++bitOffset)
    {
        // when
        const auto given = initial >> bitOffset;

        // then
        ASSERT_EQ(given.magnitude(), expected);
    }
}

TEST(UnsignedBinaryShiftTests, testThatRighShiftsWholeDigits)
{
    // given
    const auto expected = bignum::Unsigned
    ({
        bignum::digit_type(123),
        bignum::digit_type(53),
        bignum::digit_type(97)
    });
    const auto digitOffsets = 10u;

    for(auto digitOffset = 0u; digitOffset < digitOffsets; ++digitOffset)
    {
        // when
        const auto given = expected >> digitOffset * bitsPerDigit;

        // then

        // offsetted part should be the same
        for(auto digit = 0u; digit < expected.magnitude(); ++digit)
        {
            ASSERT_EQ(given.digit(digit + digitOffset), expected.digit(digit));
        }

        // extra digits should be zero
        for(auto digit = 0u; digit < digitOffset; ++digit)
        {
            ASSERT_EQ(given.digit(digit), bignum::digit_type());
        }
    }
}

TEST(UnsignedBinaryShiftTests, testThatRighShiftsDigitPart)
{
    // given
    const auto expected = bignum::digit_type(~bignum::digit_type());
    const auto bitOffset = 3 * bitsPerDigit / 4;
    const auto reversedBitOffset = bitsPerDigit - bitOffset;

    // when
    const auto given = bignum::Unsigned({expected}) >> bitOffset;

    // then
    EXPECT_EQ(given.msd(), bignum::digit_type(expected >> reversedBitOffset));
    EXPECT_EQ(given.lsd(), bignum::digit_type(expected << bitOffset));
}
