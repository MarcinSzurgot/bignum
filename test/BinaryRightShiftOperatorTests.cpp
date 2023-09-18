#include <gtest/gtest.h>
#include "bignum/BigUnsigned.hpp"

#include "Utils.hpp"

class BigUnsignedRightShiftTest : public ::testing::TestWithParam<std::tuple<BigUnsigned, std::size_t, BigUnsigned>> {};

TEST_P(BigUnsignedRightShiftTest, RightShiftOperation) {
    auto num = std::get<0>(GetParam());
    const auto shift_amount = std::get<1>(GetParam());
    const auto expected_result = std::get<2>(GetParam());

    num >>= shift_amount;
    ASSERT_EQ(num, expected_result);
}

TEST(BigUnsignedRightShiftTest, RightShiftOperationSymmetricity) {
    const auto num = BigUnsigned(1) << 128;
    const auto shift1 = 147;
    const auto shift2 = 85;

    const auto shifted = num << shift1;

    const auto expected = num << shift2;
    auto actual = shifted;
    actual >>= shift1 - shift2;

    ASSERT_EQ(expected, actual);
}

INSTANTIATE_TEST_SUITE_P(
    BigUnsignedTests,
    BigUnsignedRightShiftTest,
    ::testing::Values(
        std::make_tuple(BigUnsigned({0x12345678}), 0, BigUnsigned({0x12345678})),
        std::make_tuple(BigUnsigned({0x12345678}), 4, BigUnsigned({0x01234567})),
        std::make_tuple(BigUnsigned({0x81234567, 0xFEDCBA98}), 4, BigUnsigned({0x88123456, 0x0FEDCBA9})),
        std::make_tuple(BigUnsigned({0x81234567, 0xFEDCBA98}), 36, BigUnsigned({0x0FEDCBA9})),
        std::make_tuple(BigUnsigned({0x81234567, 0xFEDCBA98}), 64, BigUnsigned({0x0})),
        std::make_tuple(BigUnsigned("1092498765764287562983764"), 0, BigUnsigned("1092498765764287562983764")),
        std::make_tuple(BigUnsigned(1) << 1024, 0, BigUnsigned(1) << 1024),
        std::make_tuple(BigUnsigned({0x0, 0x0, 0x1}), 32, BigUnsigned({0x0, 0x1}))
    )
);