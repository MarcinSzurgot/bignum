#include <gtest/gtest.h>

#include <bignum/Arrays/Multiplication.hpp>
#include <bignum/Utils.hpp>

#include "../Utils.hpp"

using namespace bignum;

class ArrayMultiplicationOperatorTests : public ::testing::TestWithParam<ArithmeticBinaryOp> {};

TEST_P(ArrayMultiplicationOperatorTests, MultiplicationOperation) {
    const auto [lhs, rhs, expected] = GetParam();

    auto result = lhs;
    result.resize(size(lhs) + size(rhs));
    std::ranges::fill(result, 0);

    mul(lhs, rhs, begin(result));

    result.erase(trimm(result), end(result));

    ASSERT_EQ(result, expected);
}

INSTANTIATE_TEST_SUITE_P(
    MultiplicationOperationParams,
    ArrayMultiplicationOperatorTests,
    ::testing::Values(
        ArithmeticBinaryOp({0x5}, {0x3}, {0xF}),
        ArithmeticBinaryOp({0x5}, {0x0}, {}),
        ArithmeticBinaryOp({0x5}, {0x5}, {0x19}),
        ArithmeticBinaryOp({0x5}, {0x1}, {0x5}),
        ArithmeticBinaryOp({0xFFFFFFFF}, {0x2}, {0xFFFFFFFE, 0x1}),
        ArithmeticBinaryOp({0xFFFFFFFF, 0x1}, {0xFFFFFFFF, 0x1}, {0x1, 0xFFFFFFFC, 0x3}),
        ArithmeticBinaryOp({0xFFFFFFFE}, {0xFFFFFFFE}, {0x4, 0xFFFFFFFC}),
        ArithmeticBinaryOp({0x0, 0x1}, {0x0, 0x2}, {0x0, 0x0, 0x2}),
        ArithmeticBinaryOp({0x1, 0x0}, {0x2, 0x0}, {0x2}),
        ArithmeticBinaryOp({0xFFFFFFFF}, {0xFFFFFFFF}, {0x1, 0xFFFFFFFE}),
        ArithmeticBinaryOp({0xFFFFFFF0, 0x0, 0x0, 0x1}, {0xF}, {0xFFFFFF10, 0xE, 0x0, 0xF}),
        ArithmeticBinaryOp({0xFFFFFFFE, 0xFFFFFFFE}, {0xFFFFFFFD, 0xFFFFFFFD}, {0x6, 0x7, 0xFFFFFFFD, 0xFFFFFFFC}),
        ArithmeticBinaryOp({0x7}, {0xFFFFFFFD}, {0xFFFFFFEB, 0x6}),
        ArithmeticBinaryOp({0xFFFFFFFC, 0x3}, {0x4, 0x4}, {0xFFFFFFF0, 0xFFFFFFFF, 0xF}),
        ArithmeticBinaryOp({0xFFFFFFFE}, {0xFFFFFFFE}, {0x4, 0xFFFFFFFC})
    )
);