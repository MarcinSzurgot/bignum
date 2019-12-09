#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

// Addition

TEST(UnsignedAdditionAndSubtractionOperatorsTests, testThatUnaryPlusDoesNotChange)
{
    // given
    const auto expected = bignum::Unsigned
    ({
        bignum::digit_type(23),
        bignum::digit_type(45),
        bignum::digit_type(35),
        bignum::digit_type(3),
        bignum::digit_type(1),
        bignum::digit_type(0),
        bignum::digit_type(1)
    });

    // when
    const auto given = +expected;

    // then
    ASSERT_EQ(given, expected);
}

TEST(UnsignedAdditionAndSubtractionOperatorsTests, testThatUnaryMinusDoesNotChange)
{
    // given
    const auto expected = bignum::Unsigned
    ({
        bignum::digit_type(23),
        bignum::digit_type(45),
        bignum::digit_type(35),
        bignum::digit_type(3),
        bignum::digit_type(1),
        bignum::digit_type(0),
        bignum::digit_type(1)
    });

    // when
    const auto given = -expected;

    // then
    ASSERT_EQ(given, expected);
}

TEST(UnsignedAdditionAndSubtractionOperatorsTests, testThatSimplyAddsDigits)
{
    // given
    const auto lhs = bignum::Unsigned
    ({
        bignum::digit_type(1),
        bignum::digit_type(2),
        bignum::digit_type(3),
        bignum::digit_type(4),
    });
    const auto rhs = bignum::Unsigned
    ({
        bignum::digit_type(2),
        bignum::digit_type(3),
        bignum::digit_type(4),
        bignum::digit_type(5),
    });
    const auto expected = bignum::Unsigned
    ({
        bignum::digit_type(3),
        bignum::digit_type(5),
        bignum::digit_type(7),
        bignum::digit_type(9),
    });

    // when
    const auto given = lhs + rhs;

    // then
    ASSERT_EQ(given, expected);
}

TEST(UnsignedAdditionAndSubtractionOperatorsTests, testThatAddsDifferentMagnitudeValues)
{
    // given
    const auto smaller = bignum::Unsigned
    ({
        bignum::digit_type(1),
        bignum::digit_type(2),
        bignum::digit_type(3),
        bignum::digit_type(4),
    });
    const auto greater = bignum::Unsigned
    ({
        bignum::digit_type(2),
        bignum::digit_type(3),
    });
    const auto expected = bignum::Unsigned
    ({
        bignum::digit_type(3),
        bignum::digit_type(5),
        bignum::digit_type(3),
        bignum::digit_type(4),
    });

    // when
    const auto givens = {smaller + greater, greater + smaller};

    // then
    for(const auto& given : givens)
    {
        ASSERT_EQ(given, expected);
    }
}
