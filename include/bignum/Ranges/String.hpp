#pragma once

#include <array>
#include <charconv>
#include <ranges>
#include <string_view>
#include <vector>

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

template<std::unsigned_integral U>
constexpr auto digitFromChars(std::string_view chars) -> U {
    auto value = U();
    std::from_chars(begin(chars), end(chars), value);
    return value;
}


template<std::unsigned_integral U>
constexpr auto fromChars(
    std::string_view chars
) -> std::vector<U> {
    constexpr auto base = U(10);
    constexpr auto maxDivisorPowerOf10 = maxPower(base);
    constexpr auto divisor = powers<U>(maxDivisorPowerOf10);

    auto digits = std::vector<U>();
    digits.reserve(size(chars) / maxDivisorPowerOf10);

    for(; not empty(chars); chars = chars.substr(std::min(maxDivisorPowerOf10, size(chars)))) {
        const auto stringDigit = chars.substr(0, maxDivisorPowerOf10);
        const auto digit = digitFromChars<U>(stringDigit);
        
        if (const auto carry = add(digits, digit, begin(digits))) {
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
            )) {
                digits.push_back(carry);
            }

        }
    }

    return digits;
}

template<std::ranges::input_range InputRange>
requires std::unsigned_integral<std::ranges::range_value_t<InputRange>>
    && std::ranges::sized_range<InputRange>
constexpr auto toChars(InputRange&& digits) -> std::string {
    using Unsigned = std::ranges::range_value_t<InputRange>;

    constexpr auto base = Unsigned(10);
    constexpr auto maxDivisorPowerOf10 = maxPower(base);
    constexpr auto divisor = powers<Unsigned>(maxDivisorPowerOf10);

    auto converter = ZeroLeadingCharConv<maxDivisorPowerOf10>();
    auto buffer = std::vector(
        std::reverse_iterator(end(digits)),
        std::reverse_iterator(begin(digits))
    );
    auto quot = std::span(buffer);
    auto result = std::string();
    result.reserve(size(digits) * Bits<Unsigned>::Size / 3);

    while (!!quot) {
        const auto notLastDivision = (size(quot) > 1) || (quot[0] > divisor);
        const auto mod = div(quot, divisor, begin(quot));
        const auto converted = converter.convert(mod, notLastDivision);

        result.insert(
            begin(result),
            begin(converted),
            end(converted)
        );

        if (size(quot) > 1 && !quot.front()) {
            quot = quot.subspan(1);
        }
    }

    return result;
}

}