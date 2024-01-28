#pragma once 

#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Concepts/Concepts.hpp>
#include <bignum/Ranges/Algorithms.hpp>

#include <concepts>
#include <ranges>
#include <tuple>

namespace bignum {

template<
    typename InputRange1,
    typename InputRange2,
    typename OutputIterator,
    std::unsigned_integral Unsigned = std::iter_value_t<OutputIterator>
>
requires TransformableToIter<OutputIterator, InputRange1, InputRange2>
constexpr auto sub(
    InputRange1&& lhs,
    InputRange2&& rhs,
    OutputIterator output
) -> Unsigned { 
    const auto [carry, continuation] = cascade(
        lhs, rhs, Unsigned(), output, sub<Unsigned, Unsigned>
    );

    return cascade(
        std::ranges::subrange(begin(lhs) + size(rhs), end(lhs)),
        carry, 
        continuation,
        sub<Unsigned>
    ).first;
}

template<
    std::ranges::input_range InputRange1,
    std::ranges::input_range InputRange2,
    typename OutputIterator,
    std::unsigned_integral Unsigned = std::iter_value_t<OutputIterator>
> 
requires TransformableToIter<OutputIterator, 
    std::remove_reference_t<InputRange1>, 
    std::remove_reference_t<InputRange2>
>
constexpr auto add(
    InputRange1&& lhs,
    InputRange2&& rhs,
    OutputIterator output
) -> Unsigned { 
    const auto [carry, continuation] = cascade(
        lhs, rhs, Unsigned(), output, add<Unsigned, Unsigned>
    );
        
    return cascade(
        std::ranges::subrange(begin(lhs) + size(rhs), end(lhs)),
        carry, 
        continuation,
        add<Unsigned>
    ).first;
}

template<
    typename InputRange,
    typename OutputIterator, 
    std::unsigned_integral Unsigned
> 
requires TransformableToIter<OutputIterator, InputRange, Unsigned>
constexpr auto sub(
    InputRange&& lhs, 
    Unsigned rhs,
    OutputIterator output
) -> Unsigned { return cascade(lhs, rhs, output, sub<Unsigned>).first; }

template<
    typename InputRange,
    std::unsigned_integral Unsigned,
    typename OutputIterator
>
requires TransformableToIter<OutputIterator,
    std::remove_reference_t<InputRange>, 
    Unsigned
>
constexpr auto add(
    InputRange&& lhs, 
    Unsigned rhs,
    OutputIterator output
) -> Unsigned { return cascade(lhs, rhs, output, add<Unsigned>).first; }

}