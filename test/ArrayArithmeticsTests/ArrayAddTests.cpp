#include <bignum/Ranges/Additive.hpp>

#include "../Utils.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace bignum;

class ArrayAdditionTest : public ::testing::TestWithParam<ArithmeticBinaryOp> {};

TEST_P(ArrayAdditionTest, AdditionOperation) {
    auto num1 = GetParam().lhs;
    const auto num2 = GetParam().rhs;
    const auto expected = GetParam().expected;

    if (size(num1) < size(num2)) {
        num1.resize(size(num2));
    }

    const auto carry = add(num1, num2, begin(num1)).result;

    if (carry) {
        num1.push_back(1);
    }

    ASSERT_EQ(num1, expected);
}

INSTANTIATE_TEST_SUITE_P(
    ArrayAdditionTestParams,
    ArrayAdditionTest,
    ::testing::Values(
        ArithmeticBinaryOp({0xFFFFFFFF}, {0x0}, {0xFFFFFFFF}),
        ArithmeticBinaryOp({0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, {0x1}, {0x0, 0x0, 0x0, 0x1}),
        ArithmeticBinaryOp({0x0}, {0x0}, {0x0}),
        ArithmeticBinaryOp({0x1, 0x2, 0x3}, {0x0}, {0x1, 0x2, 0x3}),
        ArithmeticBinaryOp({0xFFFFFFFF, 0x0, 0x0}, {0x2}, {0x1, 0x1, 0x0}),
        ArithmeticBinaryOp({0xFFFFFFFF, 0x0, 0xFFFFFFFF}, {0x1}, {0x0, 0x1, 0xFFFFFFFF}),
        ArithmeticBinaryOp({0xF, 0xF}, {0xF, 0xF}, {0x1E, 0x1E}),
        ArithmeticBinaryOp({0xFFFFFFFF, 0xFFFFFFFF}, {0xFFFFFFFF}, {0xFFFFFFFE, 0x0, 0x1}),
        ArithmeticBinaryOp({0xFFFFFFFF, 0x0, 0x0, 0x0}, {0x1}, {0x0, 0x1, 0x0, 0x0}),
        ArithmeticBinaryOp({0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x1}),
        ArithmeticBinaryOp({0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, {0x1}, {0x1, 0x0, 0x0, 0x0, 0x0, 0x0}),
        ArithmeticBinaryOp({0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, {0x1}, {0x0, 0x0, 0x0, 0x0, 0x1}),
        ArithmeticBinaryOp({0x1}, {0xFFFFFFFF}, {0x0, 0x1}),
        ArithmeticBinaryOp({0x1, 0xFFFFFFFF, 0x1}, {0x0, 0x1, 0x0}, {0x1, 0x0, 0x2}),
        ArithmeticBinaryOp({0xFFFFFFFF, 0xFFFFFFFF}, {0xFFFFFFFF, 0xFFFFFFFF, 0x1}, {0xFFFFFFFE, 0xFFFFFFFF, 0x2}),
        ArithmeticBinaryOp({0x0, 0x1}, {0x0, 0xFFFFFFFF}, {0x0, 0x0, 0x1}),
        ArithmeticBinaryOp({0xFFFFFFFE}, {0xFFFFFFFE}, {0xFFFFFFFC, 0x1}),
        ArithmeticBinaryOp({0x1}, {0x1}, {0x2}),
        ArithmeticBinaryOp({0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1}),
        ArithmeticBinaryOp({0xFFFFFFFF}, {0x1}, {0x0, 0x1})
    )
);