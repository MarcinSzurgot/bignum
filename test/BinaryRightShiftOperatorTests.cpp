#include <gtest/gtest.h>
#include "bignum/BigUnsigned.hpp"

class BigUnsignedRightShiftTest : public ::testing::TestWithParam<std::tuple<BigUnsigned, std::size_t, BigUnsigned>> {};

TEST_P(BigUnsignedRightShiftTest, RightShiftOperation) {
    auto num = std::get<0>(GetParam());
    const auto shift_amount = std::get<1>(GetParam());
    const auto expected_result = std::get<2>(GetParam());

    num >>= shift_amount;
    ASSERT_EQ(num, expected_result);
}

INSTANTIATE_TEST_SUITE_P(
    BigUnsignedTests,
    BigUnsignedRightShiftTest,
    ::testing::Values(
        std::make_tuple(BigUnsigned({0x12345678}), 0, BigUnsigned({0x12345678})),
        std::make_tuple(BigUnsigned({0x12345678}), 4, BigUnsigned({0x01234567})),
        std::make_tuple(BigUnsigned({0x81234567, 0xFEDCBA98}), 4, BigUnsigned({0x88123456, 0x0FEDCBA9})),
        std::make_tuple(BigUnsigned({0x81234567, 0xFEDCBA98}), 36, BigUnsigned({0x0FEDCBA9})),
        std::make_tuple(BigUnsigned({0x81234567, 0xFEDCBA98}), 64, BigUnsigned({0x0}))
    )
);