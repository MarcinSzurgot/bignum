#pragma once

#include <algorithm>
#include <array>
#include <ranges>
#include <vector>

#include <bignum/Digits/Arithmetics.hpp>

namespace bignum {

template<
    std::unsigned_integral Destination,
    std::unsigned_integral Source
> requires (sizeof(Destination) <= sizeof(Source))
constexpr auto array_cast(
    Source& value
) {
    if constexpr (std::is_const_v<Source>) {
        return *reinterpret_cast<
            const std::array<Destination, sizeof(Source) / sizeof(Destination)>*
        >(std::addressof(value));
    } else {
        return *reinterpret_cast<
            std::array<Destination, sizeof(Source) / sizeof(Destination)>*
        >(std::addressof(value));
    }

}

template<
    std::unsigned_integral Native,
    std::ranges::input_range Range,
    std::unsigned_integral Lesser = std::ranges::range_value_t<Range>
> requires (sizeof(Lesser) < sizeof(Native))
constexpr auto vector(
    Range&& range
) -> std::vector<Native> {
    constexpr auto ratio = sizeof(Native) / sizeof(Lesser);

    auto native = std::vector<Native>();

    if constexpr (std::ranges::sized_range<Range>) {
        const auto s = div<std::ranges::range_size_t<Range>>(size(range), ratio);
        native.reserve(s.first + static_cast<bool>(s.second));
    }

    auto digit = Native();
    auto& splitted = array_cast<Lesser>(digit);
    for (auto first = begin(range), last = end(range); first != last;) {
        for (auto& s : splitted) {
            s = *first++;
        }
        native.push_back(digit);
    }

    return native;
}

template<
    std::unsigned_integral Native,
    std::ranges::input_range Range,
    std::unsigned_integral Greater = std::ranges::range_value_t<Range>
> requires (sizeof(Greater) >= sizeof(Native))
constexpr auto vector(
    Range&& range
) -> std::vector<Native> {
    auto native = std::vector<Native>();

    if constexpr (std::ranges::sized_range<Range>) {
        native.reserve(size(range) * sizeof(Greater) / sizeof(Native));
    }

    for (const auto& greater : range) {
        const auto& splitted = array_cast<const Native>(greater);

        native.insert(
            end(native), 
            begin(splitted), 
            end(splitted)
        );
    }

    return native;
}

template<
    std::unsigned_integral Native,
    std::unsigned_integral Unsigned
>
constexpr auto vector(
    Unsigned value
) -> std::vector<Native> {
    if constexpr (sizeof(Unsigned) <= sizeof(Native)) {
        return std::vector { Native(value) };
    } else {
        return bignum::vector<Native>(std::array { value });
    }
}

}