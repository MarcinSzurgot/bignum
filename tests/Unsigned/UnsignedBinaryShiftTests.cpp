#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

TEST(UnsignedBinaryShiftTests, testThatLeftShiftsWholeDigits)
{
    // given
    const auto wholeDigitOffset = sizeof(bignum::digit_type) * 8u;
    const auto expected = bignum::Unsigned({123, 53, 97});
    const auto digitOffsets = 10;

    for(auto digitOffset = 0u; digitOffset < digitOffsets; ++digitOffset)
    {
        // when
        const auto given = expected << digitOffset * wholeDigitOffset;

        // then
        ASSERT_EQ(given.magnitude(), expected.magnitude() + digitOffset);

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
