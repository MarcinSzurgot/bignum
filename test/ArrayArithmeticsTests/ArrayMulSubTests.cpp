#include <gtest/gtest.h>

#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Arrays/Multiplication.hpp>

#include "../Utils.hpp"

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

TEST(ArrayMulSubTests, ArrayMulSubMassiveTests) {
    using Unsigned = std::uint64_t;

    const auto minuendMaxSize = 7u;

    auto random = RandomGenerator();

    auto multiplier = std::array<Unsigned, 2>();
    auto minuend    = std::vector<Unsigned>();
    auto subtrahend = std::vector<Unsigned>();
    auto expected   = std::vector<Unsigned>();
    auto actual     = std::vector<Unsigned>();
    auto buffer     = std::vector<Unsigned>();

    for (auto _ : std::views::iota(0, 100000)) {
        const auto    minuendSize = random.random(3u, minuendMaxSize);
        const auto subtrahendSize = random.random(1u,    minuendSize - 2);

        minuend   .resize(   minuendSize);
        subtrahend.resize(subtrahendSize);
        expected  .resize(   minuendSize);
        actual    .resize(   minuendSize);
        buffer    .resize(   minuendSize);

        std::ranges::fill(minuend,    0);
        std::ranges::fill(subtrahend, 0);
        std::ranges::fill(expected,   0);
        std::ranges::fill(actual,     0);
        std::ranges::fill(buffer,     0);
    
        random.random(minuend);
        random.random(subtrahend);
        random.random(multiplier);

        mul(multiplier, subtrahend, begin(buffer));
        sub(minuend, buffer, begin(expected));
        mulSub(minuend, subtrahend, multiplier, begin(actual));

        EXPECT_EQ(expected, actual) 
            << "expected:   " << toString(expected)   << "\n"
            << "actual:     " << toString(actual)     << "\n";
    }
}

TEST_P(ArrayMulSubTests, ArrayMulSubTests) {

    using Unsigned = ArrayMulSubParams::Unsigned;

    const auto [
        minuend,
        subtrahend,
        multiplier,
        expected
    ] = GetParam();

    auto buffer = std::vector<Unsigned>();
    buffer.resize(size(subtrahend) + 2);

    auto actual = std::vector<Unsigned>();
    actual.resize(size(expected));
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
            {0, 0, 0, 1, 1, 0},
            {0, 0, 0, 1}, 
            std::array<ArrayMulSubParams::Unsigned, 2> {1, 1}, 
            {0, 0, 0, 0, 0, 0}
        ),
        ArrayMulSubParams(
            {100, 56, 0, 5, 1, 0}, 
            {10,   0, 0, 1}, 
            std::array<ArrayMulSubParams::Unsigned, 2> {5, 0},
            {50, 56, 0, 0, 1}
        ),
        ArrayMulSubParams(
            {70, 120, 0, 1, 1, 80, 0}, 
            {0,    0, 0, 0, 1}, 
            std::array<ArrayMulSubParams::Unsigned, 2> {50, 50}, 
            {70, 120, 0, 1, 207, 29}
        ),
        ArrayMulSubParams(
            {0, 0, 0, 1, 1, 0}, 
            {0, 0, 0, 1}, 
            std::array<ArrayMulSubParams::Unsigned, 2> {1, 1}, 
            {0, 0, 0, 0, 0, 0}
        ),
        ArrayMulSubParams(
            {0x55, 0x72, 0xee, 0xb8, 0x17},
            {0x60, 0xe9},
            std::array<ArrayMulSubParams::Unsigned, 2> {0x4c, 0x8f},
            {0xd5, 0x89, 0x4c, 0x36, 0x17}
        )
    )
);