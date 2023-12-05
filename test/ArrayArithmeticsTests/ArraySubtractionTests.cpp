#include <gtest/gtest.h>

#include <bignum/ArrayArithmetics/ArraySubAdd.hpp>
#include <bignum/Operations.hpp>

#include <vector>

#include "../Utils.hpp"

class ArraySubtractionTest : public ::testing::TestWithParam<ArithmeticBinaryOp> {};

TEST_P(ArraySubtractionTest, SubtractionOperation) {
    auto [num1, num2, expected] = GetParam();

    const auto isNum1LessThanNum2 = std::span(num1) < std::span(num2);

    auto& result = isNum1LessThanNum2 ? num2 : num1;
    const auto& smaller = isNum1LessThanNum2 ? num1 : num2;

    bignum::subtract(
        std::span(result),
        std::span(smaller)
    );

    ASSERT_EQ(result, expected);
}

INSTANTIATE_TEST_SUITE_P(
    ArraySubtractionTestParams,
    ArraySubtractionTest,
    ::testing::Values(
        ArithmeticBinaryOp({0x5}, {0x3}, {0x2}),
        ArithmeticBinaryOp({0x5}, {0x0}, {0x5}),
        ArithmeticBinaryOp({0x5}, {0x5}, {0x0}),
        ArithmeticBinaryOp({0x0}, {0x3}, {0x3}),
        ArithmeticBinaryOp({0x0, 0x2}, {0x1}, {0xFFFFFFFF, 0x1}),
        ArithmeticBinaryOp({0x0, 0x0, 0x3}, {0x2}, {0xFFFFFFFE, 0xFFFFFFFF, 0x2}),
        ArithmeticBinaryOp({0x3}, {0xFFFFFFFF}, {0xFFFFFFFC}),
        ArithmeticBinaryOp({0xFFFFFFFF, 0x0}, {0xFFFFFFFF}, {0x0, 0x0}),
        ArithmeticBinaryOp({0xFFFFFFFF, 0xFFFFFFFF}, {0xFFFFFFFE, 0xFFFFFFFE}, {0x1, 0x1}),
        ArithmeticBinaryOp({0x1, 0x0, 0x0, 0x0}, {0x1}, {0x0, 0x0, 0x0, 0x0}),
        ArithmeticBinaryOp({0x0, 0x0, 0x0, 0x1}, {0x1}, {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}),
        ArithmeticBinaryOp({0x7}, {0x3}, {0x4}),
        ArithmeticBinaryOp({0x2, 0x1}, {0x2}, {0x0, 0x1}),
        ArithmeticBinaryOp({0x3, 0x4, 0x5}, {0x2, 0x3}, {0x1, 0x1, 0x5}),
        ArithmeticBinaryOp({0x3, 0x4, 0x5}, {0x4, 0x5, 0x6}, {0x1, 0x1, 0x1}),
        ArithmeticBinaryOp({0x4}, {0x8}, {0x4}),
        ArithmeticBinaryOp({0x1, 0x0, 0x1}, {0x1}, {0x0, 0x0, 0x1}),
        ArithmeticBinaryOp({0x1, 0x0, 0x0}, {0x1}, {0x0, 0x0, 0x0}),
        ArithmeticBinaryOp({0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, {0xFFFFFFFE, 0xFFFFFFFE, 0xFFFFFFFE}, {0x1, 0x1, 0x1}),
        ArithmeticBinaryOp({0xFFFFFFFE, 0xFFFFFFFE, 0xFFFFFFFE}, {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, {0x1, 0x1, 0x1})
    )
);