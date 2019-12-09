#include <bignum/Addition.hpp>

#include <gtest/gtest.h>

#include <array>
#include <vector>

TEST(AdditionTests, testThatAddsWithCarry)
{
    // given
    const auto operands = std::vector<std::array<bignum::digit_type, 4>>
    {
        {1, 1, 0, 2},
        {0, 0, 1, 1},
        {1, 1, 1, 3},
        {0, 0, 0, 0},
    };

    for (const auto [lhs, rhs, carry, expected] : operands)
    {
        // when
        const auto [actual, _] = bignum::addWithCarry(lhs, rhs, carry);

        // then
        EXPECT_EQ(actual, expected);
    }
}
