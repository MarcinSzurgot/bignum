#include <vector>

#include <gtest/gtest.h>

#include <bignum/Arrays/Comparators.hpp>
#include <bignum/Ranges/Additive.hpp>
#include <bignum/Utils.hpp>

#include "../Utils.hpp"

using namespace bignum;

class ArraySubtractionTest : public ::testing::TestWithParam<ArithmeticBinaryOp> {};

TEST_P(ArraySubtractionTest, SubtractionOperation) {
    using namespace bignum;

    auto [num1, num2, expected] = GetParam();
    auto [result, smaller] = num1 < num2
    ? std::make_pair(num2, num1) 
    : std::make_pair(num1, num2);

    sub(result, smaller, result);

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