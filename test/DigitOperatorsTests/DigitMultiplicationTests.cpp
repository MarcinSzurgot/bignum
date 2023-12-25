#include <iostream>

#include <gtest/gtest.h>

#include <bignum/bignum.hpp>

#include "../Utils.hpp"

using namespace bignum;

TEST(DigitMultiplicationTests, MassiveTest) {
    const auto minSize = 1u;
    const auto maxSize = 10u;

    auto generator = RandomGenerator();

    for (auto i = 0u; i < 100000u; ++i) {
        const auto size = generator.random(minSize, maxSize);
        const auto data = generator.random<BigUnsigned::NativeDigit>(size);        
        const auto big  = BigUnsigned(data);
        const auto dig  = generator.random(
             BigUnsigned::NativeDigit(), 
            ~BigUnsigned::NativeDigit()
        );

        const auto actual   = big * dig;
        const auto expected = big * BigUnsigned(dig);

        EXPECT_EQ(actual, expected);
    }
}