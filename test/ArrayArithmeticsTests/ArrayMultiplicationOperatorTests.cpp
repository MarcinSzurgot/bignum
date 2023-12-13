#include <gtest/gtest.h>

#include <bignum/ArrayArithmetics/ArrayMul.hpp>
#include <bignum/Operations.hpp>

#include "../Utils.hpp"

class ArrayMultiplicationOperatorTests : public ::testing::TestWithParam<ArithmeticBinaryOp> {};

TEST_P(ArrayMultiplicationOperatorTests, MultiplicationOperation) {
    const auto [lhs, rhs, expected] = GetParam();

    auto result = lhs;
    result.resize(size(lhs) + size(rhs));
    std::fill(begin(result), end(result), 0);

    bignum::mul<std::uint64_t>(
        std::span(lhs),
        std::span(rhs),
        std::span(result)
    );

    result.resize(sizeWithoutLeadingZeroes(std::span(result)));

    ASSERT_EQ(result, expected);
}

INSTANTIATE_TEST_SUITE_P(
    MultiplicationOperationParams,
    ArrayMultiplicationOperatorTests,
    ::testing::Values(
        ArithmeticBinaryOp({0x5}, {0x3}, {0xF}),
        ArithmeticBinaryOp({0x5}, {0x0}, {0x0}),
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