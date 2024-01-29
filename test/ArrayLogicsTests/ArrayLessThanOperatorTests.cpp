#include <gtest/gtest.h>

#include <bignum/Arrays/Comparators.hpp>

#include "../Utils.hpp"

using namespace bignum;

class ArrayComparisonTest : public ::testing::TestWithParam<LogicBinaryOp> {};

TEST_P(ArrayComparisonTest, LessThanOperation) {
    const auto& [lhs, rhs, expected] = GetParam();

    auto lhsSpan = std::span(lhs);
    auto rhsSpan = std::span(rhs); 

    // ASSERT_EQ(lhs < rhs, expected);
    ASSERT_EQ(lhsSpan < rhsSpan, expected);
}

INSTANTIATE_TEST_SUITE_P(
    ArrayTestsParams,
    ArrayComparisonTest,
    ::testing::Values(
        LogicBinaryOp({0x12345678}, {0x12345678}, false),
        LogicBinaryOp({0x12345677}, {0x12345678}, true),
        LogicBinaryOp({0x12345679}, {0x12345678}, false),
        LogicBinaryOp({0x0, 0x0}, {0x1}, false),
        LogicBinaryOp({0x1}, {0x0, 0x0}, true),
        LogicBinaryOp({0x0, 0x1}, {0x1, 0x0}, false),
        LogicBinaryOp({0xFFFFFFFF, 0xFFFFFFFF}, {0xFFFFFFFF, 0x0}, false),
        LogicBinaryOp({0x0, 0xFFFFFFFF}, {0xFFFFFFFF, 0xFFFFFFFF}, true),
        LogicBinaryOp({0x0, 0x0}, {0x1, 0x0}, true),
        LogicBinaryOp({0x1, 0x0}, {0x0, 0x0}, false),
        LogicBinaryOp({0xFFFFFFFE}, {0xFFFFFFFF}, true),
        LogicBinaryOp({0xFFFFFFFF}, {0xFFFFFFFE}, false),
        LogicBinaryOp({0x7FFFFFFF}, {0xFFFFFFFF}, true),
        LogicBinaryOp({0xFFFFFFFF}, {0x7FFFFFFF}, false),
        LogicBinaryOp({0xFFFFFFFF, 0xFFFFFFFF}, {0xFFFFFFFF}, false),
        LogicBinaryOp({0xFFFFFFFF}, {0xFFFFFFFF, 0x1}, true),
        LogicBinaryOp({0xFFFFFFFF, 0xFFFFFFFE}, {0xFFFFFFFF, 0xFFFFFFFF}, true),
        LogicBinaryOp({0xFFFFFFFF, 0xFFFFFFFF}, {0xFFFFFFFF, 0xFFFFFFFE}, false),
        LogicBinaryOp({0x1, 0x1, 0x1}, {0x1, 0x1}, false),
        LogicBinaryOp({0x1, 0x1}, {0x1, 0x1, 0x1}, true),
        LogicBinaryOp({0xFFFFFFFF, 0x1}, {0x1, 0x0}, false),
        LogicBinaryOp({0x1, 0x0}, {0xFFFFFFFF, 0x1}, true)
    )
);