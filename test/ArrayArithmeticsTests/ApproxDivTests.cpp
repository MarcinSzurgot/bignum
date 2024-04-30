#include <gtest/gtest.h>

#include <bignum/Ranges/Division.hpp>

#include <concepts>
#include <iterator>
#include <ranges>
#include <vector>

struct ApproxDivParams {
    using Unsigned = std::uint8_t;

    std::vector<Unsigned> dividend;
    Unsigned divisor;
    std::array<Unsigned, 2> expected;
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
        ApproxDivParams({0, 4, 2}, 1, std::array<ApproxDivParams::Unsigned, 2> {2, 1}),
        ApproxDivParams({}, 1, std::array<ApproxDivParams::Unsigned, 2> {0, 0}),
        ApproxDivParams({2}, 1, std::array<ApproxDivParams::Unsigned, 2> {1, 0}),
        ApproxDivParams({0, 100, 2}, 10, std::array<ApproxDivParams::Unsigned, 2> {55, 0}),
        ApproxDivParams({0, 100, 121}, 10, std::array<ApproxDivParams::Unsigned, 2> {9, 11})
    )
);