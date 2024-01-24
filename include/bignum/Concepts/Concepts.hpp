#pragma once 

#include <concepts>
#include <ranges>
#include <tuple>

namespace bignum {

template<typename Op, typename U> 
concept BinarySymmetricOperator = 
    std::unsigned_integral<U>
    && std::invocable<Op, U, U>
    && std::same_as<
        std::invoke_result_t<Op, U, U>, 
        std::pair<U, U>
    >;

template<typename OutputRange, typename ValueType>
concept TransformableValueToRange = 
    std::ranges::output_range<
        OutputRange, 
        std::ranges::range_value_t<OutputRange>
    > 
    && std::same_as<
        std::ranges::range_value_t<OutputRange>,
        ValueType
    >;

template<typename OutputRange, typename InputRange>
concept TransformableRangeToRange = 
    std::ranges::output_range<
        OutputRange, 
        std::ranges::range_value_t<OutputRange>
    > 
    && std::ranges::input_range<InputRange>
    && std::same_as<
        std::ranges::range_value_t<OutputRange>,
        std::ranges::range_value_t<InputRange>
    >;

template<
    typename OutputRange,
    typename ...RangesOrValues
> concept TransformableToRange = (
    (TransformableValueToRange<OutputRange, RangesOrValues> 
    || TransformableRangeToRange<OutputRange, RangesOrValues>)
    && ...
);

}