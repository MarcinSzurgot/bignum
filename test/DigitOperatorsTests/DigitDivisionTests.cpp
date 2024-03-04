#include <gtest/gtest.h>

#include <bignum/Digits/Arithmetics.hpp>

#include "../Utils.hpp"

using namespace bignum;

template<std::unsigned_integral U>
constexpr auto div(
    U lower,
    U higher,
    U divisor
) -> std::pair<std::array<U, 2>, U> {
    const auto base = Bits<U>::Mask;
    const auto [bq, br] = div(base, divisor);

    auto [higherQuotient, higherRemaining] = div(higher, divisor);
    auto lowerQuotientFinal = U();

    while(higherRemaining) {
        const auto [lowerQuotient, lowerRemainder] = div(lower, divisor);
        const auto [lowerRemainingNextPartial, higherRemainingNext] = mul<U>(br + 1, higherRemaining);
        const auto [lowerRemainingNext, carry] = add(lowerRemainingNextPartial, lowerRemainder);

        lowerQuotientFinal += bq * higherRemaining + lowerQuotient;
        lower = lowerRemainingNext;
        higherRemaining = higherRemainingNext + carry;
    }

    const auto [lastQuotient, lastRemainder] = div(lower, divisor);

    lowerQuotientFinal += lastQuotient;

    return std::make_pair(
        std::array {
            lowerQuotientFinal, 
            higherQuotient
        }, lastRemainder
    );
}

template<
    std::ranges::forward_range InputRange,
    std::unsigned_integral U,
    std::forward_iterator OutputIterator
>
constexpr auto div(
    InputRange&& dividend,
    U divisor,
    OutputIterator quotient
) -> U {
    return cascade(dividend, U(), quotient, [divisor](auto&& d, auto&& carry) {
        const auto [quotient, remainder] = div(d, carry, divisor);
        return std::make_pair(quotient[0], remainder);
    });
}

TEST(DigitDivisionTests, TestDividingDoubleDigits) {
    using Unsigned = std::uint64_t;

    auto generator = RandomGenerator();
    auto result    = std::array<Unsigned, 2>();
    auto expected  = std::array<Unsigned, 3>();
    auto remainder = Unsigned();

    for (auto _ : std::views::iota(0, 100000)) {
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

    auto maxSize         = std::size_t(20);
    auto generator       = RandomGenerator();
    auto dividendStorage = std::vector<Unsigned>(maxSize);
    auto resultStorage   = std::vector<Unsigned>(maxSize);
    auto expectedStorage = std::vector<Unsigned>(maxSize);

    for (auto _ : std::views::iota(0, 100000)) {
        const auto size = generator.random<std::size_t>(1, maxSize);

        auto dividend = std::span(begin(dividendStorage), size);
        auto result   = std::span(begin(resultStorage), size);
        auto expected = std::span(begin(expectedStorage), size);

        generator.random(dividend);

        const auto divisor  = generator.random<Unsigned>(1, Bits<Unsigned>::Mask);

        const auto remainder = div(
            std::ranges::reverse_view(dividend),
            divisor,
            std::reverse_iterator(end(result))
        );

        mul(result, divisor, begin(expected));
        add(expected, remainder, begin(expected));

        EXPECT_TRUE(dividend == expected);
    }
}