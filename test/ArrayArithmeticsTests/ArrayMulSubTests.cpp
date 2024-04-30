#include <gtest/gtest.h>

#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Arrays/Multiplication.hpp>

#include <array>
#include <concepts>
#include <ranges>
#include <vector>

using namespace bignum;

struct ArrayMulSubParams {
    using Unsigned = std::uint8_t;

    std::vector<Unsigned> minuend;
    std::vector<Unsigned> subtrahend;
    std::array<Unsigned, 2> multiplier;
    std::vector<Unsigned> expected;
};

class ArrayMulSubTests : public ::testing::TestWithParam<ArrayMulSubParams> { };

TEST_P(ArrayMulSubTests, ArrayMulSubTests) {
    const auto [
        minuend,
        subtrahend,
        multiplier,
        expected
    ] = GetParam();

    auto actual = expected;
    mulSub(
        minuend,
        subtrahend,
        multiplier,
        begin(actual)
    );

    EXPECT_EQ(actual, expected);
}

INSTANTIATE_TEST_SUITE_P(
    ArrayMulSubTestsParams,
    ArrayMulSubTests,
    ::testing::Values(
        ArrayMulSubParams(
            {0, 0, 0, 1, 1}, 
            {0, 0, 0, 1}, 
            std::array<ArrayMulSubParams::Unsigned, 2> {1, 1}, 
            {0, 0, 0, 0, 0}
        ),
        ArrayMulSubParams(
            {100, 56, 0, 5, 1}, 
            {10, 0, 0, 1}, 
            std::array<ArrayMulSubParams::Unsigned, 2> {5, 0},
            {50, 56, 0, 0, 1}
        ),
        ArrayMulSubParams(
            {70, 120, 0, 1, 1, 80}, 
            {0,    0, 0, 0, 1}, 
            std::array<ArrayMulSubParams::Unsigned, 2> {50, 50}, 
            {70, 120, 0, 1, 207, 29}
        ),
        ArrayMulSubParams(
            {0, 0, 0, 1, 1}, 
            {0, 0, 0, 1}, 
            std::array<ArrayMulSubParams::Unsigned, 2> {1, 1}, 
            {0, 0, 0, 0, 0}
        )
    )
);