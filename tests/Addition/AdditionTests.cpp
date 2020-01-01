#include <bignum/Addition.hpp>

#include <gtest/gtest.h>

#include <array>
#include <vector>

TEST(AdditionTests, testThatAddsWithCarry)
{
    // given
    const auto operands = std::vector
    {
        std::array{1u, 1u, 0u, 2u},
        std::array{0u, 0u, 1u, 1u},
        std::array{1u, 1u, 1u, 3u},
        std::array{0u, 0u, 0u, 0u},
    };

    for (const auto [lhs, rhs, carry, expected] : operands)
    {
        // when
        const auto [actual, _] = bignum::addWithCarry(lhs, rhs, carry);

        // then
        EXPECT_EQ(actual, expected);
    }
}
