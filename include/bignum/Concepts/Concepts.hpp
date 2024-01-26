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

template<typename OutputIter, typename ValueType>
concept TransformableValueToIter = 
    std::output_iterator<
        OutputIter, 
        std::iter_value_t<OutputIter>
    > 
    && std::same_as<
        std::iter_value_t<OutputIter>,
        ValueType
    >;

template<typename OutputIter, typename InputRange>
concept TransformableRangeToIter = 
    std::output_iterator<
        OutputIter, 
        std::iter_value_t<OutputIter>
    > 
    && std::ranges::input_range<InputRange>
    && std::same_as<
        std::iter_value_t<OutputIter>,
        std::ranges::range_value_t<InputRange>
    >;

template<
    typename OutputIter,
    typename ...RangesOrValues
> concept TransformableToIter = (
    (TransformableValueToIter<OutputIter, RangesOrValues> 
    || TransformableRangeToIter<OutputIter, RangesOrValues>)
    && ...
);

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