#include <bignum/IntWrapper.hpp>

#include <gtest/gtest.h>

#include <iostream>
#include <type_traits>

using bignum::IntWrapper;

TEST(IntWrapperConversionTests, testThatAdditionPreserveIntType)
{
    // given
    using expected_type = char;

    // when
    using actual_type = decltype(IntWrapper<expected_type>() + IntWrapper<expected_type>())::int_type;

    // then
    ASSERT_EQ(typeid(actual_type), typeid(expected_type));
}
