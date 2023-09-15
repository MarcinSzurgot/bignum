#include <gtest/gtest.h>
#include "bignum/BigUnsigned.hpp"

#include "Utils.hpp"

class BigUnsignedLeftShiftTest : public ::testing::TestWithParam<std::tuple<BigUnsigned, std::size_t, BigUnsigned>> {};

TEST_P(BigUnsignedLeftShiftTest, LeftShiftOperation) {
    auto num = std::get<0>(GetParam());
    const auto shift = std::get<1>(GetParam());
    const auto expected = std::get<2>(GetParam());

    num <<= shift;
    ASSERT_EQ(num, expected);
}

INSTANTIATE_TEST_SUITE_P(
    BigUnsignedTests,
    BigUnsignedLeftShiftTest,
    ::testing::Values(
        std::make_tuple(BigUnsigned({0x12345678}), 0, BigUnsigned({0x12345678})),
        std::make_tuple(BigUnsigned({0x12345678}), 4, BigUnsigned({0x23456780, 0x1})),
        std::make_tuple(BigUnsigned({0xF2345678}), 4, BigUnsigned({0x23456780, 0xF})),
        std::make_tuple(BigUnsigned({0xF2345678, 0x9ABCDEF0}), 4, BigUnsigned({0x23456780, 0xABCDEF0F, 0x9})),
        std::make_tuple(BigUnsigned({0x12345678}), 32, BigUnsigned({0x0, 0x12345678})),
        std::make_tuple(BigUnsigned({0x12345678}), 64, BigUnsigned({0x0, 0x0, 0x12345678})),
        std::make_tuple(BigUnsigned({0xF2345678, 0x9ABCDEF0}), 36, BigUnsigned({0x0, 0x23456780, 0xABCDEF0F, 0x9})),
        std::make_tuple(BigUnsigned({0xF2345678, 0x9ABCDEF0}), 31, BigUnsigned({0x0, 0x791A2B3C, 0x4D5E6F78})),
        std::make_tuple(BigUnsigned({0x0}), 4, BigUnsigned({0x0})),
        std::make_tuple(BigUnsigned({0x0}), 32, BigUnsigned({0x0})),
        std::make_tuple(BigUnsigned({0x1}), 128, BigUnsigned({0x0, 0x0, 0x0, 0x0, 0x1}))
    )
);