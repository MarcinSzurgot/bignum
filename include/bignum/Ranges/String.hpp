#pragma once

#include <bignum/Ranges/Additive.hpp>
#include <bignum/Ranges/Comparators.hpp>
#include <bignum/Ranges/Division.hpp>
#include <bignum/Ranges/Multiplication.hpp>
#include <bignum/Utils.hpp>

#include <algorithm>
#include <array>
#include <charconv>
#include <ranges>
#include <string_view>
#include <vector>

namespace bignum {

template<std::unsigned_integral U>
constexpr static auto maxPower(U base) -> std::size_t {
    U prev = base;
    auto pow = std::size_t();

    for (U acc = U(base * base); acc > prev; ++pow) {
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

template<std::unsigned_integral U>
constexpr auto digitFromChars(std::string_view chars) -> U {
    auto value = U();
    std::from_chars(begin(chars), end(chars), value);
    return value;
}


template<std::unsigned_integral U>
constexpr auto fromChars(
    std::string_view chars,
    std::vector<U>& digits
) -> void {
    constexpr auto base = U(10);
    constexpr auto maxDivisorPowerOf10 = maxPower(base);
    constexpr auto divisor = powers<U>(maxDivisorPowerOf10);

    digits.resize(0);
    digits.reserve(size(chars) / maxDivisorPowerOf10);

    for(; not empty(chars); chars = chars.substr(std::min(maxDivisorPowerOf10, size(chars)))) {
        const auto stringDigit = chars.substr(0, maxDivisorPowerOf10);
        const auto digit = digitFromChars<U>(stringDigit);
        
        if (const auto carry = add(digits, digit, begin(digits)).result) {
            digits.push_back(carry);
        }

        if (size(stringDigit) < size(chars)) {
            const auto power = powers<U>(std::min(
                maxDivisorPowerOf10, 
                size(chars) - size(stringDigit)
            ));

            if (const auto carry = mul(
                digits,
                power,
                begin(digits)
            ).result) {
                digits.push_back(carry);
            }

        }
    }
}

template<std::unsigned_integral U>
constexpr auto fromChars(std::string_view chars) -> std::vector<U> {
    auto digits = std::vector<U>();
    fromChars(chars, digits);
    return digits;
}

template<
    std::ranges::input_range Input,
    std::unsigned_integral Unsigned = std::ranges::range_value_t<Input>
> requires std::ranges::sized_range<Input>
constexpr auto toChars(
    Input&& digits,
    std::string& chars
) -> void {
    if (empty(digits)) {
        chars.resize(1);
        chars[0] = '0';
        return;
    }

    constexpr auto base = Unsigned(10);
    constexpr auto maxDivisorPowerOf10 = maxPower(base);
    constexpr auto divisor = powers<Unsigned>(maxDivisorPowerOf10);

    auto converter = ZeroLeadingCharConv<maxDivisorPowerOf10>();
    auto quot = digits;

    chars.resize(0);
    chars.reserve(size(digits) * Bits<Unsigned>::Size / 3);

    while (not empty(quot)) {
        const auto notLastDivision = size(quot) > 1 || quot[0] >= divisor;
        const auto mod = div(std::views::reverse(quot), divisor, rbegin(quot));
        const auto converted = converter.convert(mod, notLastDivision);

        chars.insert(
            begin(chars),
            begin(converted),
            end(converted)
        );

        quot.erase(trimm(quot), end(quot));
    }
}

template<
    std::ranges::input_range Input,
    std::unsigned_integral Unsigned = std::ranges::range_value_t<Input>
> requires std::ranges::sized_range<Input>
constexpr auto toChars(Input&& digits) -> std::string {
    auto chars = std::string();
    toChars(digits, chars);
    return chars;
}

}