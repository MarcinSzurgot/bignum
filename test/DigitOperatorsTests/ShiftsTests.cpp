#include <bitset>
#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <bignum/Digits/Arithmetics.hpp>

#include "../Utils.hpp"

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

// TEST(DigitShiftsTests, RightShiftAllCases) {
//     using BaseUInt       = std::uint8_t;
//     using ContainingUInt = std::uint16_t;

//     constexpr auto baseMax = BaseUInt(~BaseUInt());

//     for (auto value = ContainingUInt(); value <= ContainingUInt(baseMax); ++value) {
//         for (auto shift = ContainingUInt(); shift < ContainingUInt(Bits<BaseUInt>::Size); ++shift) {
//             const auto expected = halve<ContainingUInt>(value >> shift);
//             const auto actual   = rshift<BaseUInt>(value, shift);

//             EXPECT_EQ(+actual.first,  +expected.first);
//             EXPECT_EQ(+actual.second, +expected.second);
//         }
//     }
// }