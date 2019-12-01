#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

TEST(UnsignedBinaryShiftTests, testThatLeftShiftsWholeDigits)
{
    // given
    const auto wholeDigitOffset = sizeof(bignum::digit_type) * 8u;
    const auto digit = bignum::digit_type(123);
    const auto offsets = 100;

    for(auto offset = 0u; offset < offsets; ++offset)
    {
        // when
        const auto given = bignum::Unsigned(digit) << offset * wholeDigitOffset;

        // then
        ASSERT_EQ(given.magnitude(), offset + 1);
        ASSERT_EQ(given.digit(given.magnitude() - 1), digit);
    }
}
