#include <concepts>

#include <gtest/gtest.h>
#include <bignum/Arrays/Shifts.hpp>
#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Utils.hpp>

#include "../Utils.hpp"

using namespace bignum;

class ArrayRightShiftTest : public ::testing::TestWithParam<ShiftOp> {};

TEST_P(ArrayRightShiftTest, RightShiftOperation) {
    auto [lhs, rhs, expected] = GetParam();

    rshift(lhs, rhs, begin(lhs));

    ASSERT_EQ(lhs, expected);
}

INSTANTIATE_TEST_SUITE_P(
    ArrayRightShiftTestParams,
    ArrayRightShiftTest,
    ::testing::Values(
        ShiftOp({0x12345678}, 0, {0x12345678}),
        ShiftOp({0x12345678}, 4, {0x01234567}),
        ShiftOp({0x81234567, 0xFEDCBA98}, 4, {0x88123456, 0x0FEDCBA9}),
        ShiftOp({0x81234567, 0xFEDCBA98}, 36, {0x88123456, 0x0FEDCBA9}),
        ShiftOp({0x81234567, 0xFEDCBA98}, 64, {0x81234567, 0xFEDCBA98}),
        ShiftOp({0x0, 0x0, 0x0}, 0, {0x0, 0x0, 0x0}),
        ShiftOp({0x0, 0x0, 0x1}, 32, {0x0, 0x0, 0x1})
    )
);