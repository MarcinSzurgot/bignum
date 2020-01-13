#include <bignum/DigitSet.hpp>

#include <gtest/gtest.h>

TEST(DigitSetTests, testThatInitsTrimmedWithSampleZero)
{
    // given
    const auto expected = 1;

    // when
    const auto digitSet = bignum::DigitSet(10, 0);

    // then
    ASSERT_EQ(expected, size(digitSet));
}

TEST(DigitSetTests, testThatTrimsLeadingZeroes)
{
    // given
    const auto zeroes = std::vector{0, 0, 0};
    const auto expected = 5;
    auto digitSet = bignum::DigitSet(size(zeroes) + expected, 1);

    // when
    digitSet.edit([&zeroes](auto& digits){
        copy(begin(zeroes), end(zeroes), rbegin(digits));
    });

    // then
    ASSERT_EQ(expected, size(digitSet));
}
