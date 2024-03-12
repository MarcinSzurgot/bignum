#pragma once

#include <array>
#include <charconv>
#include <ranges>
#include <string_view>

namespace bignum {

template<std::unsigned_integral U>
constexpr static auto maxPower(U base) -> std::size_t {
    auto prev = base;
    auto pow = std::size_t();

    for (auto acc = U(base * base); acc > prev; ++pow) {
        prev = acc;
        acc *= base;
    }

    return pow;
}

template<std::unsigned_integral U>
constexpr static auto powers(std::size_t power) -> U {
    static constexpr auto powers = []() {
        constexpr auto base = U(10);
        auto array = std::array<U, maxPower(base) + 1>();
        array[0] = 1;
        for (auto p = 1; p < size(array); ++p) {
            array[p] = array[p - 1] * base;
        }
        return array;
    } ();
    return powers[power];
}

template<std::size_t MaxDigitSize>
struct ZeroLeadingCharConv {
    ZeroLeadingCharConv() { std::ranges::fill(buffer_, '0'); }

    template<std::unsigned_integral U>
    std::string_view convert(U value, bool useLeadingZeroes) {
        const auto charresult = std::to_chars(
            begin(buffer_) + MaxDigitSize,
            end(buffer_),
            value
        );

        const auto backwardsSize = useLeadingZeroes 
        ? end(buffer_) - charresult.ptr 
        : 0u;

        return {
            begin(buffer_) + (MaxDigitSize - backwardsSize),
            charresult.ptr
        };
    }

private:
    std::array<char, 2 * MaxDigitSize> buffer_;
};

template<std::ranges::input_range InputRange>
requires std::unsigned_integral<std::ranges::range_value_t<InputRange>>
    && std::ranges::sized_range<InputRange>
constexpr auto string(
    InputRange&& digits,
    char* output
) -> std::string_view {
    using Unsigned = std::ranges::range_value_t<InputRange>;

    constexpr auto base = Unsigned(10);
    constexpr auto maxDivisorPowerOf10 = maxPower(base);
    constexpr auto divisor = powers<Unsigned>(maxDivisorPowerOf10);

    auto converter = ZeroLeadingCharConv<maxDivisorPowerOf10>();
    auto quotFirst = reinterpret_cast<Unsigned*>(output);
    auto quotLast  = std::ranges::copy(digits, quotFirst).out;
    auto quot = std::span(quotFirst, quotLast);
    auto outputLast = output + 30 * size(digits);
    auto outputIt   = outputLast;

    while (!!quot) {
        const auto notLastDivision = (size(quot) > 1) || (quot[0] > divisor);
        const auto mod = div(
            std::ranges::reverse_view(quot), 
            divisor, 
            std::reverse_iterator(end(quot))
        );
        const auto converted = converter.convert(mod, notLastDivision);

        outputIt -= size(converted);
        std::ranges::copy(
            converted, 
            outputIt
        );

        if (size(quot) > 1 && !quot.back()) {
            quot = quot.subspan(0, size(quot) - 1);
        }
    }

    const auto it = std::find_if(outputIt, outputLast, [] (auto&& c) { 
        return c != '0'; 
    });

    return {it, outputLast};
}

}