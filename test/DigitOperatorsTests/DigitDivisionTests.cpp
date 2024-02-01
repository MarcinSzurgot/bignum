#include <gtest/gtest.h>

#include <bignum/Digits/Arithmetics.hpp>

#include "../Utils.hpp"

using namespace bignum;

template<std::unsigned_integral U>
constexpr auto div(
    std::array<U, 2> dividend,
    U divisor
) -> std::tuple<U, U, U> {
    constexpr auto base_1 = Bits<U>::Mask;

    if (!dividend[1]) {
        const auto [quotient, remainder] = div(dividend[0], divisor);
        return {quotient, U(), remainder};
    }

    auto remainingDividend = std::array<U, 2>();
    auto remainingResult   = std::array<U, 2>();
    auto result            = std::array<U, 2>();
    auto remainder         = U();

    const auto [qBase_1, rBase_1] = div(base_1,      divisor);
    const auto [q0,      r0     ] = div(dividend[0], divisor);

    std::tie(result[0], result[1]) = mul(qBase_1, dividend[1]);
    add(result, q0, begin(result));

    std::tie(remainingDividend[0], remainingDividend[1]) = mul<U>(rBase_1 + 1, dividend[1]);
    add(remainingDividend, r0, begin(remainingDividend));

    std::tie(remainingResult[0], remainingResult[1], remainder) = div(remainingDividend, divisor);
    add(result, remainingResult, begin(result));

    return {result[0], result[1], remainder};
}

TEST(DigitDivisionTests, TestDividingDoubleDigits) {
    using Unsigned = std::uint8_t;

    auto generator = RandomGenerator();
    auto result    = std::array<Unsigned, 2>();
    auto expected  = std::array<Unsigned, 3>();
    auto remainder = Unsigned();

    for (auto _ : std::views::iota(0, 10000)) {
        const auto dvdnH   = generator.random<Unsigned>(0, 255);
        const auto dvdnL   = generator.random<Unsigned>(0, 255);
        const auto divisor = generator.random<Unsigned>(1, 255);

        std::tie(result[0], result[1], remainder) = div(
            std::array{(Unsigned)dvdnL, (Unsigned)dvdnH}, 
            Unsigned(divisor)
        );

        mul(result, Unsigned(divisor), begin(expected)); 
        add(expected, remainder, begin(expected));

        EXPECT_EQ(dvdnH, expected[1]);
        EXPECT_EQ(dvdnL, expected[0]);
    }
}