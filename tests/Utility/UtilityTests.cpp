#include <bignum/Utility.hpp>

#include <gtest/gtest.h>

#include <limits>
#include <vector>

TEST(Utility, testThatThereIsNoHighestBit)
{
    // given
    const auto zero = 0u;
    const auto expected = std::nullopt;

    // when
    const auto actual = bignum::highestBitNumber(zero);

    // then
    ASSERT_EQ(actual, expected);
}

TEST(Utility, testThatNegativeIntegerAlwaysHasHighestBit)
{
    // given
    const auto expected = sizeof(int) * 8 - 1;
    const auto negatives = []()
    {
        auto negatives = std::vector(sizeof(int) * 8, -1);
        for(auto i = 0u; i < negatives.size(); ++i)
        {
            negatives[i] <<= i;
        }
        return negatives;
    }();

    for(const auto negative : negatives)
    {
        // when
        const auto given = bignum::highestBitNumber(negative);

        // then
        ASSERT_EQ(given, expected);
    }
}

TEST(Utility, testThatIsHighestBit)
{
    // given
    const auto unsigneds = []()
    {
        auto unsigneds = std::vector(sizeof(unsigned) * 8, 0u);
        for(auto i = 0u; i < unsigneds.size(); ++i)
        {
            unsigneds[i] = 1u << i;
        }
        return unsigneds;
    }();

    for(auto expected = 0u; expected < unsigneds.size(); ++expected)
    {
        // when
        const auto given = bignum::highestBitNumber(unsigneds[expected]);

        // then
        ASSERT_EQ(given, expected);
    }
}
