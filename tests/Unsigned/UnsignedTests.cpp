#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

TEST(UnsignedTests, testThatZeroIsFalse)
{
    // given
    const auto zero = bignum::Unsigned();

    // when
    const auto boolean = static_cast<bool>(zero);

    // then
    ASSERT_FALSE(boolean);
}

TEST(UnsignedTests, testThatNonZeroSingleDigitIsTrue)
{
    // given
    const auto nonZero = bignum::Unsigned({bignum::digit_type(1)});

    // when
    const auto boolean = static_cast<bool>(nonZero);

    // then
    ASSERT_TRUE(boolean);
}

TEST(UnsignedTests, testThatNonZeroMultipleDigitsIsTrue)
{
    // given
    const auto nonZero = bignum::Unsigned
    ({
        bignum::digit_type(1),
        bignum::digit_type(244),
        bignum::digit_type(5),
        bignum::digit_type(2),
    });

    // when
    const auto boolean = static_cast<bool>(nonZero);

    // then
    ASSERT_TRUE(boolean);
}
