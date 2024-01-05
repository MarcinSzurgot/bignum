#include <gtest/gtest.h>
#include <bignum/Arrays/Shifts.hpp>

#include "../Utils.hpp"

using namespace bignum;

class ArrayLeftShiftOperatorTests : public ::testing::TestWithParam<ShiftOp> {};

TEST_P(ArrayLeftShiftOperatorTests, LeftShiftOperation) {
    auto [lhs, rhs, expected] = GetParam();

    lhs.push_back(0);

    lhs.back() |= lshift(
        std::span(lhs),
        rhs,
        std::span(lhs)
    );

    if (lhs.back() == 0) {
        lhs.pop_back();
    }

    ASSERT_EQ(lhs, expected);
}

INSTANTIATE_TEST_SUITE_P(
    ArrayLeftShiftOperatorTestsParams,
    ArrayLeftShiftOperatorTests,
    ::testing::Values(
        ShiftOp({0x12345678}, 0, {0x12345678}),
        ShiftOp({0x12345678}, 4, {0x23456780, 0x1}),
        ShiftOp({0xF2345678}, 4, {0x23456780, 0xF}),
        ShiftOp({0xF2345678, 0x9ABCDEF0}, 4, {0x23456780, 0xABCDEF0F, 0x9}),
        ShiftOp({0x12345678}, 32, {0x12345678}),
        ShiftOp({0x12345678}, 64, {0x12345678}),
        ShiftOp({0xF2345678, 0x9ABCDEF0}, 36, {0x23456780, 0xABCDEF0F, 0x9}),
        ShiftOp({0xF2345678, 0x9ABCDEF0}, 100, {0x23456780, 0xABCDEF0F, 0x9}),
        ShiftOp({0xF2345678, 0x9ABCDEF0}, 31, {0x0, 0x791A2B3C, 0x4D5E6F78}),
        ShiftOp({0x0}, 4, {0x0}),
        ShiftOp({0x0}, 32, {0x0}),
        ShiftOp({0x1}, 128, {0x1})
    )
);