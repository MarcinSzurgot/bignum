#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

TEST(UnsignedLogicOperatorTests, testThatLogicalOrWithTwoZeroesGivesFalse)
{
    // given
    const auto zero1 = bignum::Unsigned<unsigned>();
    const auto zero2 = bignum::Unsigned<unsigned>();

    // when-then
    ASSERT_FALSE(zero1 || zero2);
    ASSERT_FALSE(zero2 || zero1);
}

TEST(UnsignedLogicOperatorTests, testThatLogicalOrWithZeroAndNonZeroGivesTrue)
{
    // given
    const auto zero    = bignum::Unsigned<unsigned>();
    const auto nonzero = bignum::Unsigned<unsigned>(9458u);

    // when-then
    ASSERT_TRUE(zero || nonzero);
    ASSERT_TRUE(nonzero || zero);
}

TEST(UnsignedLogicOperatorTests, testThatLogicalOrWithTwoNonZeroesGivesTrue)
{
    // given
    const auto nonzero1 = bignum::Unsigned<unsigned>(2398u);
    const auto nonzero2 = bignum::Unsigned<unsigned>(394u);

    // when-then
    ASSERT_TRUE(nonzero1 || nonzero2);
    ASSERT_TRUE(nonzero2 || nonzero1);
}

TEST(UnsignedLogicOperatorTests, testThatLogicalAndWithTwoZeroesGivesFalse)
{
    // given
    const auto zero1 = bignum::Unsigned<unsigned>();
    const auto zero2 = bignum::Unsigned<unsigned>();

    // when-then
    ASSERT_FALSE(zero1 && zero2);
    ASSERT_FALSE(zero2 && zero1);
}

TEST(UnsignedLogicOperatorTests, testThatLogicalAndWithZeroAndNonZeroGivesFalse)
{
    // given
    const auto zero    = bignum::Unsigned<unsigned>();
    const auto nonzero = bignum::Unsigned<unsigned>(9458u);

    // when-then
    ASSERT_FALSE(zero && nonzero);
    ASSERT_FALSE(nonzero && zero);
}

TEST(UnsignedLogicOperatorTests, testThatLogicalAndWithTwoNonZeroesGivesTrue)
{
    // given
    const auto nonzero1 = bignum::Unsigned<unsigned>(2398u);
    const auto nonzero2 = bignum::Unsigned<unsigned>(394u);

    // when-then
    ASSERT_TRUE(nonzero1 && nonzero2);
    ASSERT_TRUE(nonzero2 && nonzero1);
}