#include <bignum/Utils.hpp>

#include <gtest/gtest.h>

using Int = int;

struct LeadingZeroesParams {
    std::vector<Int> leadingZeroes;
    std::vector<Int> trimmed;
};

class LeadingZeroesTests : public ::testing::TestWithParam<LeadingZeroesParams> {};

TEST_P(LeadingZeroesTests, LeadingZeroesTests) {
    auto [values, expected] = GetParam();

    values.erase(
        bignum::trimm(values), 
        end(values)
    );

    ASSERT_EQ(values, expected);
}

INSTANTIATE_TEST_SUITE_P(
    LeadingZeroesTestsParams,
    LeadingZeroesTests,
    ::testing::Values(
        LeadingZeroesParams({0, 0, 1, 1, 1, 0}, {0, 0, 1, 1, 1}),
        LeadingZeroesParams({0, 0, 1, 1, 1, 0, 0}, {0, 0, 1, 1, 1}),
        LeadingZeroesParams({0}, {0}),
        LeadingZeroesParams({0, 0}, {0}),
        LeadingZeroesParams({}, {}),
        LeadingZeroesParams({1}, {1}),
        LeadingZeroesParams({666, 666}, {666, 666})
    )
);