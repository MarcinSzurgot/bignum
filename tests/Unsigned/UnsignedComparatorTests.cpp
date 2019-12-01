#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

TEST(UnsignedComparatorTests, testThatSmallerMagnitudeIsLessThan)
{
    // given
    const auto smallerMagnitude = bignum::Unsigned
    ({
        bignum::digit_type(1),
        bignum::digit_type(35),
        bignum::digit_type(0),
        bignum::digit_type(1),
        bignum::digit_type(44),
        bignum::digit_type(5),
        bignum::digit_type(12),
    });
    const auto biggerMagnitude = bignum::Unsigned
    ({
        bignum::digit_type(1),
        bignum::digit_type(9),
        bignum::digit_type(1),
        bignum::digit_type(2),
        bignum::digit_type(34),
        bignum::digit_type(59),
        bignum::digit_type(12),
        bignum::digit_type(22),
        bignum::digit_type(112),
    });

    // when
    const auto comparison = compare(smallerMagnitude, biggerMagnitude);

    // then
    ASSERT_EQ(comparison, bignum::Comparison::LT);
}

TEST(UnsignedComparatorTests, testThatBiggerMagnitudeIsBiggerThan)
{
    // given
    const auto smallerMagnitude = bignum::Unsigned
    ({
        bignum::digit_type(23),
        bignum::digit_type(34),
        bignum::digit_type(5),
    });
    const auto biggerMagnitude = bignum::Unsigned
    ({
        bignum::digit_type(55),
        bignum::digit_type(9),
        bignum::digit_type(1),
        bignum::digit_type(23),
        bignum::digit_type(4),
    });

    // when
    const auto comparison = compare(biggerMagnitude, smallerMagnitude);

    // then
    ASSERT_EQ(comparison, bignum::Comparison::GT);
}

TEST(UnsignedComparatorTests, testThatIsLessThan)
{
    // given
    const auto less = bignum::Unsigned
    ({
        bignum::digit_type(1),
        bignum::digit_type(234),
        bignum::digit_type(5),
        bignum::digit_type(6),
    });
    const auto greater = bignum::Unsigned
    ({
        bignum::digit_type(120),
        bignum::digit_type(24),
        bignum::digit_type(5),
        bignum::digit_type(30),
    });

    // when
    const auto comparison = compare(less, greater);

    // then
    ASSERT_EQ(comparison, bignum::Comparison::LT);
}

TEST(UnsignedComparatorTests, testThatIsGreaterThan)
{
    // given
    const auto less = bignum::Unsigned
    ({
        bignum::digit_type(1),
        bignum::digit_type(234),
        bignum::digit_type(5),
        bignum::digit_type(60),
        bignum::digit_type(11),
        bignum::digit_type(24),
    });
    const auto greater = bignum::Unsigned
    ({
        bignum::digit_type(120),
        bignum::digit_type(24),
        bignum::digit_type(5),
        bignum::digit_type(30),
        bignum::digit_type(25),
        bignum::digit_type(24),
    });

    // when
    const auto comparison = compare(greater, less);

    // then
    ASSERT_EQ(comparison, bignum::Comparison::GT);
}

TEST(UnsignedComparatorTests, testThatIsEqual)
{
    // given
    const auto lhs = bignum::Unsigned
    ({
        bignum::digit_type(1),
        bignum::digit_type(234),
        bignum::digit_type(5),
        bignum::digit_type(60),
        bignum::digit_type(11),
        bignum::digit_type(24),
    });
    const auto rhs = lhs;

    // when
    const auto comparison = compare(lhs, lhs);

    // then
    ASSERT_EQ(comparison, bignum::Comparison::EQ);
}
