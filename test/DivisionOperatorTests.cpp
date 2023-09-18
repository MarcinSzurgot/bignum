#include <gtest/gtest.h>
#include "bignum/BigUnsigned.hpp"

class BigUnsignedDivisionTests : public ::testing::TestWithParam<std::tuple<BigUnsigned, BigUnsigned, BigUnsigned>> {};

TEST_P(BigUnsignedDivisionTests, DivisionAssignmentOperator) {
          auto num1     = std::get<0>(GetParam());
    const auto num2     = std::get<1>(GetParam());
    const auto expected = std::get<2>(GetParam());

    num1 /= num2;

    EXPECT_EQ(num1, expected);
}

INSTANTIATE_TEST_SUITE_P(
    DivisionTests,
    BigUnsignedDivisionTests,
    ::testing::Values(
        std::make_tuple(BigUnsigned({0x0}), BigUnsigned({0x1}), BigUnsigned({0x0})),
        std::make_tuple(BigUnsigned({0x8}), BigUnsigned({0x4}), BigUnsigned({0x2})),
        std::make_tuple(BigUnsigned({0x10, 0x0}), BigUnsigned({0x1}), BigUnsigned({0x10, 0x0})),
        std::make_tuple(BigUnsigned({0x10, 0x0}), BigUnsigned({0x2}), BigUnsigned({0x8, 0x0})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF}), BigUnsigned({0xFFFFFFFF}), BigUnsigned({0x1, 0x1})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF}), BigUnsigned({0x1}), BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0x0}), BigUnsigned({0xFFFFFFFF}), BigUnsigned({0x1})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0x0, 0x1}), BigUnsigned({0x2}), BigUnsigned({0x7FFFFFFF, 0x80000000})),
        std::make_tuple(BigUnsigned({0x1, 0x0, 0x0}), BigUnsigned({0x1, 0x0}), BigUnsigned({0x1})),
        std::make_tuple(BigUnsigned({0x1, 0x1}), BigUnsigned({0x2}), BigUnsigned({0x80000000})),
        std::make_tuple(BigUnsigned({0x0, 0x1, 0x1}), BigUnsigned({0x2}), BigUnsigned({0x80000000, 0x80000000})),
        std::make_tuple(BigUnsigned({0x0, 0x1}), BigUnsigned({0x2}), BigUnsigned({0x80000000})),
        std::make_tuple(BigUnsigned({0x0, 0x1, 0x1, 0x1}), BigUnsigned({0x2}), BigUnsigned({0x80000000, 0x80000000, 0x80000000})),
        std::make_tuple(BigUnsigned({0x0, 0x1, 0x1, 0x0, 0x1}), BigUnsigned({0x2}), BigUnsigned({0x80000000, 0x80000000, 0x0, 0x80000000}))
    )
);