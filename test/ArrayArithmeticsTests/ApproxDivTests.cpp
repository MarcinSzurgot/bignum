#include <bignum/Ranges/Division.hpp>

#include <gtest/gtest.h>
#include <concepts>
#include <iterator>
#include <ranges>
#include <vector>

struct ApproxDivParams {
    using Unsigned = std::uint8_t;

    std::vector<Unsigned> dividend;
    std::vector<Unsigned> divisor;
    std::pair<Unsigned, std::ranges::range_difference_t<std::vector<Unsigned>>> expected;
};

class ApproxDivTests : public ::testing::TestWithParam<ApproxDivParams> { };

TEST_P(ApproxDivTests, ApproxDivTests) {
    const auto [dividend, divisor, expected] = GetParam();

    const auto actual = bignum::approxDiv(dividend, divisor);

    EXPECT_EQ(actual, expected);
}

INSTANTIATE_TEST_SUITE_P(
    ApproxDivTestsParams,
    ApproxDivTests,
    ::testing::Values(
        ApproxDivParams(
            {0, 4, 2}, 
            {1}, 
            {2, 2}
        ),
        ApproxDivParams(
            {},
            {1},
            {0, 0}
        ),
        ApproxDivParams(
            {2}, 
            {1},
            {2, 0}
        ),
        ApproxDivParams(
            {0, 100, 2}, 
            {10}, 
            {61, 1}
        ),
        ApproxDivParams(
            {0, 100, 121}, 
            {10}, 
            {12, 2}
        ),
        ApproxDivParams(
            {0xFF}, 
            {0xFF}, 
            {1, 0}
        ),
        ApproxDivParams(
            {0xFF, 0xFF}, 
            {0xFF}, 
            {1, 1}
        ),
        ApproxDivParams(
            {0, 0, 0, 0, 0, 10},
            {0xFF, 0xFF, 5},
            {1, 3}
        ),
        ApproxDivParams(
            {0, 0, 5, 0, 0, 77},
            {3, 42, 51, 76},
            {1, 2}
        ),
        ApproxDivParams(
            {0,  0,  5,  0, 0, 77},
            {3, 42, 51, 77},
            {252, 1}
        ),
        ApproxDivParams(
            {0, 0, 5, 0, 0, 77},
            {3, 42, 51, 0xFF},
            {77, 1}
        ),
        ApproxDivParams(
            {0x0, 0x1},
            {0x0, 0x1},
            {1, 0}
        )
    )
);