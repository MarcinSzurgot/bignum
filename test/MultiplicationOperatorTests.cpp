#include <gtest/gtest.h>
#include "bignum/BigUnsigned.hpp"

#include "Utils.hpp"

class BigUnsignedMultiplicationTest : public ::testing::TestWithParam<std::tuple<BigUnsigned, BigUnsigned, BigUnsigned>> {};

TEST_P(BigUnsignedMultiplicationTest, MultiplicationOperation) {
    auto num1 = std::get<0>(GetParam());
    const auto num2 = std::get<1>(GetParam());
    const auto expected = std::get<2>(GetParam());

    num1 *= num2;
    ASSERT_EQ(num1, expected);
}

INSTANTIATE_TEST_SUITE_P(
    BigUnsignedTests,
    BigUnsignedMultiplicationTest,
    ::testing::Values(
        std::make_tuple(BigUnsigned({0x5}), BigUnsigned({0x3}), BigUnsigned({0xF})),
        std::make_tuple(BigUnsigned({0x5}), BigUnsigned({0x0}), BigUnsigned({0x0})),
        std::make_tuple(BigUnsigned({0x5}), BigUnsigned({0x5}), BigUnsigned({0x19})),
        std::make_tuple(BigUnsigned({0x5}), BigUnsigned({0x1}), BigUnsigned({0x5})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF}), BigUnsigned({0x2}), BigUnsigned({0xFFFFFFFE, 0x1})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0x1}), BigUnsigned({0xFFFFFFFF, 0x1}), BigUnsigned({0x1, 0xFFFFFFFC, 0x3})),
        std::make_tuple(BigUnsigned({0xFFFFFFFE}), BigUnsigned({0xFFFFFFFE}), BigUnsigned({0x4, 0xFFFFFFFC})),
        std::make_tuple(BigUnsigned({0x0, 0x1}), BigUnsigned({0x0, 0x2}), BigUnsigned({0x0, 0x0, 0x2})),
        std::make_tuple(BigUnsigned({0x1, 0x0}), BigUnsigned({0x2, 0x0}), BigUnsigned({0x2, 0x0, 0x0})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF}), BigUnsigned({0xFFFFFFFF}), BigUnsigned({0x1, 0xFFFFFFFE})),
        std::make_tuple(BigUnsigned({0xFFFFFFF0, 0x0, 0x0, 0x1}), BigUnsigned({0xF}), BigUnsigned({0xFFFFFF10, 0xE, 0x0, 0xF})),
        std::make_tuple(BigUnsigned({0xFFFFFFFE, 0xFFFFFFFE}), BigUnsigned({0xFFFFFFFD, 0xFFFFFFFD}), BigUnsigned({0x6, 0x7, 0xFFFFFFFD, 0xFFFFFFFC})),
        std::make_tuple(BigUnsigned({0x7}), BigUnsigned({0xFFFFFFFD}), BigUnsigned({0xFFFFFFEB, 0x6})),
        std::make_tuple(BigUnsigned({0xFFFFFFFC, 0x3}), BigUnsigned({0x4, 0x4}), BigUnsigned({0xFFFFFFF0, 0xFFFFFFFF, 0xF})),
        std::make_tuple(BigUnsigned({0xFFFFFFFE}), BigUnsigned({0xFFFFFFFE}), BigUnsigned({0x4, 0xFFFFFFFC}))
    )
);