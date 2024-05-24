#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Ranges/Division.hpp>

#include "../Utils.hpp"

#include <gtest/gtest.h>
#include <charconv>

using namespace bignum;
using namespace std::string_literals;

TEST(DigitDivisionTests, TestDividingDoubleDigits) {
    using Unsigned = std::uint64_t;

    auto generator = RandomGenerator();
    auto result    = std::array<Unsigned, 2>();
    auto expected  = std::array<Unsigned, 3>();
    auto remainder = Unsigned();

    for (auto _ : std::views::iota(0, 1000000)) {
        const auto dvdnH   = generator.random<Unsigned>(0, Bits<Unsigned>::Mask);
        const auto dvdnL   = generator.random<Unsigned>(0, Bits<Unsigned>::Mask);
        const auto divisor = generator.random<Unsigned>(1, Bits<Unsigned>::Mask);

        std::tie(result, remainder) = div(
            dvdnL, 
            dvdnH, 
            divisor
        );

        mul(result, divisor, begin(expected)); 
        add(expected, remainder, begin(expected));

        EXPECT_EQ(dvdnH, expected[1]);
        EXPECT_EQ(dvdnL, expected[0]);
    }
}

TEST(DigitDivisionTests, TestDividingMultipleDigits) {
    using Unsigned = std::uint64_t;

    auto maxSize   = std::size_t(20);
    auto generator = RandomGenerator();
    auto dividend  = std::vector<Unsigned>(maxSize);
    auto result    = std::vector<Unsigned>(maxSize);
    auto expected  = std::vector<Unsigned>(maxSize);

    for (auto _ : std::views::iota(0, 1000)) {
        const auto size = generator.random<std::size_t>(1, maxSize);

        dividend.resize(size);
        result.resize(size);
        expected.resize(size);

        generator.random(dividend);

        const auto divisor  = generator.random<Unsigned>(1, Bits<Unsigned>::Mask);

        const auto remainder = div(
            std::ranges::reverse_view(dividend),
            divisor,
            std::reverse_iterator(end(result))
        );

        mul(result, divisor, begin(expected));
        add(expected, remainder, begin(expected));

        EXPECT_TRUE(equal(dividend, expected));
    }
}