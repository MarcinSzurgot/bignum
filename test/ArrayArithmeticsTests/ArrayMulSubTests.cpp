#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Ranges/Multiplication.hpp>

#include "../Utils.hpp"

#include <gtest/gtest.h>
#include <array>
#include <concepts>
#include <ranges>
#include <vector>

using namespace bignum;

struct ArrayMulSubParams {
    using Unsigned = std::uint8_t;

    std::vector<Unsigned> minuend;
    std::vector<Unsigned> subtrahend;
    Unsigned multiplier;
    std::vector<Unsigned> expected;
};

template<
    std::ranges::input_range Minuend,
    std::ranges::input_range Subtrahend,
    std::unsigned_integral Multiplier,
    std::ranges::input_range Buffer,
    std::input_iterator Difference
> requires 
    std::same_as<Multiplier, std::ranges::range_value_t<Minuend>>
    && std::same_as<Multiplier, std::ranges::range_value_t<Subtrahend>>
    && std::same_as<Multiplier, std::ranges::range_value_t<Buffer>>
    && std::same_as<Multiplier, std::iter_value_t<Difference>>
constexpr auto testingMulSub(
    Minuend&& minuend,
    Subtrahend&& subtrahend,
    Multiplier multiplier,
    Buffer&& buffer,
    Difference difference
) {
    using std::ranges::subrange;

    const auto result = mul(subtrahend, multiplier, begin(buffer));
    add(subrange(result.out, end(buffer)), result.result, result.out);

    auto bufferLast = end(buffer);
    if (auto bufferBeforeLast = prev(bufferLast); *bufferBeforeLast == Multiplier()) {
        bufferLast = bufferBeforeLast;
    }

    sub(
        minuend, 
        subrange(begin(buffer), bufferLast), 
        difference
    );
}

template<std::unsigned_integral Unsigned>
void testMulSub(std::size_t iterations, std::size_t minuendMaxSize) {
    auto random = RandomGenerator();

    auto multiplier = Unsigned();
    auto minuend    = std::vector<Unsigned>();
    auto subtrahend = std::vector<Unsigned>();
    auto expected   = std::vector<Unsigned>();
    auto actual     = std::vector<Unsigned>();
    auto buffer     = std::vector<Unsigned>();

    for (auto _ : std::views::iota(std::size_t(0), iterations)) {
        const auto    minuendSize = random.random(std::size_t(2), minuendMaxSize);
        const auto subtrahendSize = random.random(std::size_t(1), minuendSize - 1);

        minuend   .resize(   minuendSize);
        subtrahend.resize(subtrahendSize);
        expected  .resize(   minuendSize);
        actual    .resize(   minuendSize);
        buffer    .resize(   minuendSize + 1);

        std::ranges::fill(minuend,    0);
        std::ranges::fill(subtrahend, 0);
        std::ranges::fill(expected,   0);
        std::ranges::fill(actual,     0);
        std::ranges::fill(buffer,     0);
    
        random.random(minuend);
        random.random(subtrahend);
        multiplier = random.random<Unsigned>(Unsigned(), Unsigned(~Unsigned()));

        testingMulSub(
            minuend,
            subtrahend,
            multiplier,
            buffer,
            begin(expected)
        );

        mulSub(
            minuend, 
            subtrahend, 
            multiplier, 
            begin(actual)
        );

        EXPECT_EQ(expected, actual) 
            << "minuend:    " << toString(minuend) << "\n"
            << "subtrahend: " << toString(subtrahend) << "\n"
            << "multiplier: " << +multiplier << "\n"
            << "expected:   " << toString(expected)   << "\n"
            << "actual:     " << toString(actual)     << "\n";
    }
}


class ArrayMulSubTests : public ::testing::TestWithParam<ArrayMulSubParams> { };

TEST(ArrayMulSubTests, ArrayMulSubMassiveTests) {
    testMulSub<std::uint8_t >(1000000, 2);
    testMulSub<std::uint16_t>(100000, 2);
    testMulSub<std::uint32_t>(100000, 2);
    testMulSub<std::uint64_t>(100000, 2);

    testMulSub<std::uint8_t >(100000, 3);
    testMulSub<std::uint16_t>(100000, 3);
    testMulSub<std::uint32_t>(100000, 3);
    testMulSub<std::uint64_t>(100000, 3);

    testMulSub<std::uint8_t >(100000, 7);
    testMulSub<std::uint16_t>(100000, 7);
    testMulSub<std::uint32_t>(100000, 7);
    testMulSub<std::uint64_t>(100000, 7);
}

TEST_P(ArrayMulSubTests, ArrayMulSubSimpleTests) {

    using Unsigned = ArrayMulSubParams::Unsigned;

    const auto [
        minuend,
        subtrahend,
        multiplier,
        expected
    ] = GetParam();

    auto buffer = std::vector<Unsigned>();
    auto actual = std::vector<Unsigned>();

    buffer.resize(size(subtrahend) + 1);
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
            {0, 0, 5},
            {0, 0, 1},
            1,
            {0, 0, 4}
        ),
        ArrayMulSubParams(
            {0, 0, 5},
            {0, 1, 1},
            1,
            {0, 255, 3}
        ),
        ArrayMulSubParams(
            {0, 0, 0, 2},
            {0, 1, 1},
            1,
            {0, 255, 254, 1}
        ),
        ArrayMulSubParams(
            {0, 0, 4},
            {0, 0, 1},
            2,
            {0, 0, 2}
        ),
        ArrayMulSubParams(
            {0,     0, 4},
            {255, 255, 0},
            2,
            {2,     0, 2}
        ),
        ArrayMulSubParams(
            {0,     0, 4},
            {255, 255},
            2,
            {2,     0, 2}
        ),
        ArrayMulSubParams(
            {0,     0, 4},
            {255, 255, 0},
            2,
            {2,     0, 2}
        ),
        ArrayMulSubParams(
            {255, 255, 255},
            {255, 255},
            255,
            {254,   0,   1}
        ),
        ArrayMulSubParams(
            {0xbe, 0x42, 0xd6},
            {0xfd, 0xa7},
            64,
            {0x7e, 0x43, 0xac}
        )
    )
);