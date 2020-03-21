#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

TEST(UnsignedIncrementDecrement, testThatPreIncrementationReturnsIncrementedValue)
{
    // given
          auto initial  = bignum::Unsigned<unsigned>(111u);
    const auto expected = bignum::Unsigned<unsigned>(112u);

    // when
    const auto actual = ++initial;

    // then
    ASSERT_EQ(actual, expected);
    ASSERT_EQ(initial, expected);
}

TEST(UnsignedIncrementDecrement, testThatPreDecrementationReturnsDecrementedValue)
{
    // given
          auto initial  = bignum::Unsigned<unsigned>(111u);
    const auto expected = bignum::Unsigned<unsigned>(110u);

    // when
    const auto actual = --initial;

    // then
    ASSERT_EQ(actual, expected);
    ASSERT_EQ(initial, expected);
}

TEST(UnsignedIncrementDecrement, testThatPostIncrementationReturnsOldValue)
{
    // given
          auto initial  = bignum::Unsigned<unsigned>(111u);
    const auto oldValue = initial;
    const auto expected = bignum::Unsigned<unsigned>(112u);

    // when
    const auto actual = initial++;

    // then
    ASSERT_EQ(actual, oldValue);
    ASSERT_EQ(initial, expected);
}

TEST(UnsignedIncrementDecrement, testThatPostDecrementationReturnsOldValue)
{
    // given
          auto initial  = bignum::Unsigned<unsigned>(111u);
    const auto oldValue = initial;
    const auto expected = bignum::Unsigned<unsigned>(110u);

    // when
    const auto actual = initial--;

    // then
    ASSERT_EQ(actual, oldValue);
    ASSERT_EQ(initial, expected);
}