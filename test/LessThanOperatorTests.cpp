#include <gtest/gtest.h>

#include "bignum/BigUnsigned.hpp"

class BigUnsignedComparisonTest : public ::testing::TestWithParam<std::tuple<BigUnsigned, BigUnsigned, bool>> {};

TEST_P(BigUnsignedComparisonTest, LessThanOperation) {
    BigUnsigned num1 = std::get<0>(GetParam());
    BigUnsigned num2 = std::get<1>(GetParam());
    bool expected_result = std::get<2>(GetParam());

    ASSERT_EQ(num1 < num2, expected_result);
}

INSTANTIATE_TEST_SUITE_P(
    BigUnsignedTests,
    BigUnsignedComparisonTest,
    ::testing::Values(
        std::make_tuple(BigUnsigned({0x12345678}), BigUnsigned({0x12345678}), false),
        std::make_tuple(BigUnsigned({0x12345677}), BigUnsigned({0x12345678}), true),
        std::make_tuple(BigUnsigned({0x12345679}), BigUnsigned({0x12345678}), false),
        std::make_tuple(BigUnsigned({0x0, 0x0}), BigUnsigned({0x1}), true),
        std::make_tuple(BigUnsigned({0x1}), BigUnsigned({0x0, 0x0}), false),
        std::make_tuple(BigUnsigned({0x0, 0x1}), BigUnsigned({0x1, 0x0}), false),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF}), BigUnsigned({0xFFFFFFFF, 0x0}), false),
        std::make_tuple(BigUnsigned({0x0, 0xFFFFFFFF}), BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF}), true),
        std::make_tuple(BigUnsigned({0x0, 0x0}), BigUnsigned({0x1, 0x0}), true),
        std::make_tuple(BigUnsigned({0x1, 0x0}), BigUnsigned({0x0, 0x0}), false),
        std::make_tuple(BigUnsigned({0xFFFFFFFE}), BigUnsigned({0xFFFFFFFF}), true),
        std::make_tuple(BigUnsigned({0xFFFFFFFF}), BigUnsigned({0xFFFFFFFE}), false),
        std::make_tuple(BigUnsigned({0x7FFFFFFF}), BigUnsigned({0xFFFFFFFF}), true),
        std::make_tuple(BigUnsigned({0xFFFFFFFF}), BigUnsigned({0x7FFFFFFF}), false),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF}), BigUnsigned({0xFFFFFFFF}), false),
        std::make_tuple(BigUnsigned({0xFFFFFFFF}), BigUnsigned({0xFFFFFFFF, 0x1}), true),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0xFFFFFFFE}), BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF}), true),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF}), BigUnsigned({0xFFFFFFFF, 0xFFFFFFFE}), false),
        std::make_tuple(BigUnsigned({0x1, 0x1, 0x1}), BigUnsigned({0x1, 0x1}), false),
        std::make_tuple(BigUnsigned({0x1, 0x1}), BigUnsigned({0x1, 0x1, 0x1}), true),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0x1}), BigUnsigned({0x1, 0x0}), false),
        std::make_tuple(BigUnsigned({0x1, 0x0}), BigUnsigned({0xFFFFFFFF, 0x1}), true)
    )
);
