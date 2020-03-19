#include <bignum/Integer.hpp>

#include <gtest/gtest.h>

// Addition

TEST(IntegerAddDiffTests, testThatAddingLessPositiveToPositiveGivesPositive)
{
    // given
    const auto less     = bignum::Integer<unsigned>(100);
    const auto greater  = bignum::Integer<unsigned>(1000);
    const auto expected = bignum::Integer<unsigned>(1100);

    // when
    const auto actual = greater + less;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatAddingGreaterPositiveToPositiveGivesPositive)
{
    // given
    const auto less     = bignum::Integer<unsigned>(39857);
    const auto greater  = bignum::Integer<unsigned>(40000);
    const auto expected = bignum::Integer<unsigned>(79857);

    // when
    const auto actual = less + greater;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatAddingEqualPositivesGivesPositive)
{
    // given
    const auto lhs      = bignum::Integer<unsigned>(100);
    const auto rhs      = bignum::Integer<unsigned>(100);
    const auto expected = bignum::Integer<unsigned>(200);

    // when
    const auto actual = lhs + rhs;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatAddingZeroesGivesPositive)
{
    // given
    const auto zero     = bignum::Integer<unsigned>();
    const auto expected = bignum::Integer<unsigned>();

    // when
    const auto actual = zero + zero;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatAddingNegativeWithLessAbsToPositiveGivesPositive)
{
    // given
    const auto negative = bignum::Integer<unsigned>(-100);
    const auto positive = bignum::Integer<unsigned>(1000);
    const auto expected = bignum::Integer<unsigned>(900);

    // when
    const auto actual = positive + negative;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatAddingNegativeWithGreaterAbsToPositiveGivesNegative)
{
    // given
    const auto negative = bignum::Integer<unsigned>(-10000);
    const auto positive = bignum::Integer<unsigned>(1000);
    const auto expected = bignum::Integer<unsigned>(-9000);

    // when
    const auto actual = positive + negative;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatAddingNegativeWithEqualAbsToPositiveGivesPositive)
{
    // given
    const auto negative = bignum::Integer<unsigned>(-1000);
    const auto positive = bignum::Integer<unsigned>(1000);
    const auto expected = bignum::Integer<unsigned>();

    // when
    const auto actual = positive + negative;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatAddingPositiveWithLessAbsToNegativeGivesNegative)
{
    // given
    const auto negative = bignum::Integer<unsigned>(-1000);
    const auto positive = bignum::Integer<unsigned>(100);
    const auto expected = bignum::Integer<unsigned>(-900);

    // when
    const auto actual = negative + positive;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatAddingPositiveWithEqualAbsToNegativeGivesPositive)
{
    // given
    const auto negative = bignum::Integer<unsigned>(-1000);
    const auto positive = bignum::Integer<unsigned>(1000);
    const auto expected = bignum::Integer<unsigned>();

    // when
    const auto actual = negative + positive;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatAddingPositiveWithGreaterAbsToNegativeGivesPositive)
{
    // given
    const auto negative = bignum::Integer<unsigned>(-1000);
    const auto positive = bignum::Integer<unsigned>(1001);
    const auto expected = bignum::Integer<unsigned>(1);

    // when
    const auto actual = negative + positive;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatAddingNegativeWithLessAbsToNegativeGivesNegative)
{
    // given
    const auto less     = bignum::Integer<unsigned>(-100);
    const auto greater  = bignum::Integer<unsigned>(-1000);
    const auto expected = bignum::Integer<unsigned>(-1100);

    // when
    const auto actual = greater + less;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatAddingNegativeWithEqualAbsToNegativeGivesNegative)
{
    // given
    const auto lhs      = bignum::Integer<unsigned>(-1000);
    const auto rhs      = bignum::Integer<unsigned>(-1000);
    const auto expected = bignum::Integer<unsigned>(-2000);

    // when
    const auto actual = lhs + rhs;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatAddingNegativeWithGreaterAbsToNegativeGivesNegative)
{
    // given
    const auto less     = bignum::Integer<unsigned>(-100);
    const auto greater  = bignum::Integer<unsigned>(-1000);
    const auto expected = bignum::Integer<unsigned>(-1100);

    // when
    const auto actual = less + greater;

    // then
    ASSERT_EQ(expected, actual);
}

// Subtraction

TEST(IntegerAddDiffTests, testThatSubtractingLessPositiveFromPositiveGivesPositive)
{
    // given
    const auto less     = bignum::Integer<unsigned>(100);
    const auto greater  = bignum::Integer<unsigned>(1000);
    const auto expected = bignum::Integer<unsigned>(900);

    // when
    const auto actual = greater - less;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatSubtractingGreaterPositiveFromPositiveGivesNegative)
{
    // given
    const auto less     = bignum::Integer<unsigned>(39857);
    const auto greater  = bignum::Integer<unsigned>(40000);
    const auto expected = bignum::Integer<unsigned>(-143);

    // when
    const auto actual = less - greater;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatSubtractingEqualPositivesGivesPositive)
{
    // given
    const auto lhs      = bignum::Integer<unsigned>(100);
    const auto rhs      = bignum::Integer<unsigned>(100);
    const auto expected = bignum::Integer<unsigned>();

    // when
    const auto actual = lhs - rhs;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatSubtractingZeroesGivesPositive)
{
    // given
    const auto zero     = bignum::Integer<unsigned>();
    const auto expected = bignum::Integer<unsigned>();

    // when
    const auto actual = zero - zero;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatSubtractingNegativeWithLessAbsFromPositiveGivesPositive)
{
    // given
    const auto negative = bignum::Integer<unsigned>(-100);
    const auto positive = bignum::Integer<unsigned>(1000);
    const auto expected = bignum::Integer<unsigned>(1100);

    // when
    const auto actual = positive - negative;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatSubtractingNegativeWithGreaterAbsFromPositiveGivesPositive)
{
    // given
    const auto negative = bignum::Integer<unsigned>(-10000);
    const auto positive = bignum::Integer<unsigned>(1000);
    const auto expected = bignum::Integer<unsigned>(11000);

    // when
    const auto actual = positive - negative;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatSubtractingNegativeWithEqualAbsFromPositiveGivesPositive)
{
    // given
    const auto negative = bignum::Integer<unsigned>(-1000);
    const auto positive = bignum::Integer<unsigned>(1000);
    const auto expected = bignum::Integer<unsigned>(2000);

    // when
    const auto actual = positive - negative;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatSubtractingPositiveWithLessAbsFromNegativeGivesNegative)
{
    // given
    const auto negative = bignum::Integer<unsigned>(-1000);
    const auto positive = bignum::Integer<unsigned>(100);
    const auto expected = bignum::Integer<unsigned>(-1100);

    // when
    const auto actual = negative - positive;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatSubtractingPositiveWithEqualAbsFromNegativeGivesNegative)
{
    // given
    const auto negative = bignum::Integer<unsigned>(-1000);
    const auto positive = bignum::Integer<unsigned>(1000);
    const auto expected = bignum::Integer<unsigned>(-2000);

    // when
    const auto actual = negative - positive;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatSubtractingPositiveWithGreaterAbsFromNegativeGivesNegative)
{
    // given
    const auto negative = bignum::Integer<unsigned>(-1000);
    const auto positive = bignum::Integer<unsigned>(1001);
    const auto expected = bignum::Integer<unsigned>(-2001);

    // when
    const auto actual = negative - positive;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatSubtractingNegativeWithLessAbsFromNegativeGivesNegative)
{
    // given
    const auto less     = bignum::Integer<unsigned>(-100);
    const auto greater  = bignum::Integer<unsigned>(-1000);
    const auto expected = bignum::Integer<unsigned>(-900);

    // when
    const auto actual = greater - less;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatSubtractingNegativeWithEqualAbsFromNegativeGivesPositive)
{
    // given
    const auto lhs      = bignum::Integer<unsigned>(-1000);
    const auto rhs      = bignum::Integer<unsigned>(-1000);
    const auto expected = bignum::Integer<unsigned>();

    // when
    const auto actual = lhs - rhs;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerAddDiffTests, testThatSubtractingNegativeWithGreaterAbsFromNegativeGivesPositive)
{
    // given
    const auto less     = bignum::Integer<unsigned>(-100);
    const auto greater  = bignum::Integer<unsigned>(-1000);
    const auto expected = bignum::Integer<unsigned>(900);

    // when
    const auto actual = less - greater;

    // then
    ASSERT_EQ(expected, actual);
}