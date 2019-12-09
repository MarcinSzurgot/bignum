#include <bignum/Unsigned.hpp>

#include <bignum/Defines.hpp>
#include <bignum/Utility.hpp>

#include <gtest/gtest.h>

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
        const auto given = initial << digitOffset * bignum::bitsPerDigitType;

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
        bignum::digit_type(1u << (bignum::bitsPerDigitType / 2u))
    });
    const auto expected = initial.magnitude();
    const auto highestBit = *bignum::highestBitNumber(initial.msd());
    const auto bitOffsets = bignum::bitsPerDigitType - highestBit;

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
        bignum::digit_type(1u << (bignum::bitsPerDigitType / 2u))
    });
    const auto expected = initial.magnitude() + 1u;
    const auto highestBit = *bignum::highestBitNumber(initial.msd());
    const auto startBitOffset = bignum::bitsPerDigitType - highestBit;
    const auto endBitOffset = bignum::bitsPerDigitType;

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
        const auto given = expected << digitOffset * bignum::bitsPerDigitType;

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
    const auto bitOffset = 3 * bignum::bitsPerDigitType / 4;
    const auto reversedBitOffset = bignum::bitsPerDigitType - bitOffset;

    // when
    const auto given = bignum::Unsigned({expected}) << bitOffset;

    // then
    EXPECT_EQ(given.msd(), bignum::digit_type(expected >> reversedBitOffset));
    EXPECT_EQ(given.lsd(), bignum::digit_type(expected << bitOffset));
}

// Right shift operator tests

TEST(UnsignedBinaryShiftTests, testThatRightShiftReducesSize)
{
    // given
    const auto initial = bignum::Unsigned
    ({
        bignum::digit_type(),
        bignum::digit_type(),
        bignum::digit_type(),
        bignum::digit_type(),
        bignum::digit_type(),
        bignum::digit_type(),
        bignum::digit_type(1),
    });
    const auto offsets = initial.magnitude() + 1;

    for(auto offset = decltype(offsets)(); offset < offsets; ++offset)
    {
        // given
        const auto expected = offset >= initial.magnitude() ? 1u : initial.magnitude() - offset;

        // when
        const auto given = initial >> offset * bignum::bitsPerDigitType;

        // then
        EXPECT_EQ(given.magnitude(), expected);
    }
}

TEST(UnsignedBinaryShiftTests, testThatRightShiftsWholeDigits)
{
    // given
    const auto initial = bignum::Unsigned
    ({
        bignum::digit_type(1),
        bignum::digit_type(2),
        bignum::digit_type(3),
        bignum::digit_type(4),
        bignum::digit_type(5),
        bignum::digit_type(6),
        bignum::digit_type(7),
        bignum::digit_type(8),
        bignum::digit_type(9),
    });
    const auto offsets = initial.magnitude() - 1;

    for(auto offset = decltype(offsets)(); offset < offsets; ++offset)
    {
        // when
        const auto given = initial >> offset * bignum::bitsPerDigitType;

        // then
        for(auto digit = 0u; digit < given.magnitude(); ++digit)
        {
            EXPECT_EQ(given.digit(digit), initial.digit(digit + offset));
        }
    }
}

TEST(UnsignedBinaryShiftTests, testThatRightShiftsPartOfDigit)
{
    // given
    const auto lsb = bignum::digit_type(1);
    const auto msb = bignum::digit_type(lsb << bignum::bitsPerDigitType - 1);
    const auto initial = bignum::Unsigned
    ({
        bignum::digit_type(),
        bignum::digit_type(lsb | msb),
        bignum::digit_type(lsb | msb)
    });
    const auto offset = 1u;

    // when
    const auto given = initial >> offset;

    // then
    EXPECT_EQ(given.digit(0), msb);
    EXPECT_EQ(given.digit(1), bignum::digit_type(msb | (msb >> 1)));
    EXPECT_EQ(given.digit(2), bignum::digit_type(msb >> 1));
}

TEST(UnsignedBinaryShiftTests, testThatRightShiftPartialReducesSize)
{
    // given
    const auto offset = bignum::bitsPerDigitType / 2;
    const auto lsb = bignum::digit_type(1);
    const auto hsb = bignum::digit_type(lsb << offset);
    const auto initial = bignum::Unsigned
    ({
        bignum::digit_type(),
        bignum::digit_type(),
        bignum::digit_type(lsb | hsb),
    });
    const auto expected = initial.magnitude() - 1;

    // when
    const auto given = initial >> offset + 1;

    // then
    ASSERT_EQ(given.magnitude(), expected);
}

TEST(UnsignedBinaryShiftTests, testThatRightShiftZeroesWhenIsBiggerThanHighestBit)
{
    // given
    const auto offset = bignum::bitsPerDigitType / 2;
    const auto hsb = bignum::digit_type(1 << offset);
    const auto initial = bignum::Unsigned
    ({
        bignum::digit_type(),
        bignum::digit_type(),
        bignum::digit_type(hsb),
    });
    const auto expected = bignum::Unsigned();
    const auto offsetBiggerThanHighestBit = offset + initial.magnitude() * bignum::bitsPerDigitType + 1;

    // when
    const auto given = initial >> offsetBiggerThanHighestBit;

    // then
    ASSERT_EQ(given, bignum::Unsigned());
}

TEST(UnsignedBinaryShiftTests, testThatRightShiftsZeroToZero)
{
    // given
    const auto expected = bignum::Unsigned();
    const auto offset0   = 0u;
    const auto offset1   = 1u;
    const auto offset100 = 100u;

    // when
    const auto givens =
    {
        expected >> offset0,
        expected >> offset1,
        expected >> offset100
    };

    // then
    for(const auto& given : givens)
    {
        EXPECT_EQ(given, expected);
    }
}
