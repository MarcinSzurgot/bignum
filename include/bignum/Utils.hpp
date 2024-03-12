#pragma once

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <ranges>
#include <span>

namespace bignum {

template<std::unsigned_integral U> 
struct Bits {
    static constexpr U Size = sizeof(U) * 8;
    static constexpr U Mask = ~U();
    static constexpr U HalfSize = Size / 2;
    static constexpr U HalfMask = Mask >> HalfSize;
    static constexpr U ShiftMask = Size - 1;
};

namespace {

template<std::ranges::input_range InputRange>
requires std::integral<std::ranges::range_value_t<InputRange>>
auto leadingZeroes(
    InputRange&& digits
) -> std::ranges::range_difference_t<InputRange> {
    auto reversed = std::views::reverse(digits);

    return std::ranges::find_if(reversed, [](auto&& digit) { 
        return digit > 0; 
    }) - begin(reversed);
}

}

template<std::ranges::input_range InputRange>
requires std::integral<std::ranges::range_value_t<InputRange>>
auto sizeWithoutLeadingZeroes(
    InputRange&& digits
) -> std::ranges::range_difference_t<InputRange> {
    return std::max(
        ssize(digits) - leadingZeroes(digits), 
        typename std::ranges::range_difference_t<InputRange>(1)
    );
}

template<typename InputRange>
requires std::unsigned_integral<std::ranges::range_value_t<InputRange>>
    && std::ranges::sized_range<InputRange>
    && std::ranges::bidirectional_range<InputRange>
auto topBit(
    InputRange&& digits
) -> std::ranges::range_size_t<InputRange> {
    using Size     = std::ranges::range_size_t<InputRange>;
    using Unsigned = std::ranges::range_value_t<InputRange>;

    if (empty(digits) || (size(digits) == Size(1) && *begin(digits) == Unsigned())) { 
        return 0u;
    }

    const auto topDigit = *(end(digits) - 1);

    auto bit = Bits<Unsigned>::Size - 1;
    for (; bit > 0u && !((Unsigned(1) << bit) & topDigit); --bit) {

    }

    return bit + (size(digits) - 1) * Bits<Unsigned>::Size;
}

}