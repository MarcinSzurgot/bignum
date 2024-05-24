#include <bignum/Digits/Arithmetics.hpp>

#include "../Utils.hpp"

#include <gtest/gtest.h>
#include <bitset>
#include <cstdint>
#include <iostream>

using namespace bignum;

TEST(DigitShiftsTests, LeftShiftAllCases) {
    using BaseUInt       = std::uint8_t;
    using ContainingUInt = std::uint16_t;

    constexpr auto baseMax = BaseUInt(~BaseUInt());

    for (auto value = ContainingUInt(); value <= ContainingUInt(baseMax); ++value) {
        for (auto shift = ContainingUInt(); shift < ContainingUInt(Bits<BaseUInt>::Size); ++shift) {
            const auto expected = halve<ContainingUInt>(value << shift);
            const auto actual   = lshift<BaseUInt>(value, shift);

            EXPECT_EQ(+actual.first,  +expected.first);
            EXPECT_EQ(+actual.second, +expected.second);
        }
    }
}