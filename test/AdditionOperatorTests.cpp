#include <gtest/gtest.h>
#include "bignum/BigUnsigned.hpp"

#include "Utils.hpp"

class BigUnsignedAdditionTest : public ::testing::TestWithParam<std::tuple<BigUnsigned, BigUnsigned, BigUnsigned>> {};

TEST_P(BigUnsignedAdditionTest, AdditionOperation) {
    auto num1 = std::get<0>(GetParam());
    const auto num2 = std::get<1>(GetParam());
    const auto expected = std::get<2>(GetParam());

    num1 += num2;
    ASSERT_EQ(num1, expected);
}

INSTANTIATE_TEST_SUITE_P(
    BigUnsignedTests,
    BigUnsignedAdditionTest,
    ::testing::Values(
        std::make_tuple(BigUnsigned({0xFFFFFFFF}), BigUnsigned({0x0}), BigUnsigned({0xFFFFFFFF})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}), BigUnsigned({0x1}), BigUnsigned({0x0, 0x0, 0x0, 0x1})),
        std::make_tuple(BigUnsigned({0x0}), BigUnsigned({0x0}), BigUnsigned({0x0})),
        std::make_tuple(BigUnsigned({0x1, 0x2, 0x3}), BigUnsigned({0x0}), BigUnsigned({0x1, 0x2, 0x3})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0x0, 0x0}), BigUnsigned({0x2}), BigUnsigned({0x1, 0x1, 0x0})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0x0, 0xFFFFFFFF}), BigUnsigned({0x1}), BigUnsigned({0x0, 0x1, 0xFFFFFFFF})),
        std::make_tuple(BigUnsigned({0xF, 0xF}), BigUnsigned({0xF, 0xF}), BigUnsigned({0x1E, 0x1E})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF}), BigUnsigned({0xFFFFFFFF}), BigUnsigned({0xFFFFFFFE, 0x0, 0x1})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0x0, 0x0, 0x0}), BigUnsigned({0x1}), BigUnsigned({0x0, 0x1, 0x0, 0x0})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}), BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}), BigUnsigned({0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x1})),
        std::make_tuple(BigUnsigned({0x0, 0x0, 0x0, 0x0, 0x0, 0x0}), BigUnsigned({0x1}), BigUnsigned({0x1, 0x0, 0x0, 0x0, 0x0, 0x0})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}), BigUnsigned({0x1}), BigUnsigned({0x0, 0x0, 0x0, 0x0, 0x1})),
        std::make_tuple(BigUnsigned({0x1}), BigUnsigned({0xFFFFFFFF}), BigUnsigned({0x0, 0x1})),
        std::make_tuple(BigUnsigned({0x1, 0xFFFFFFFF, 0x1}), BigUnsigned({0x0, 0x1, 0x0}), BigUnsigned({0x1, 0x0, 0x2})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF}), BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF, 0x1}), BigUnsigned({0xFFFFFFFE, 0xFFFFFFFF, 0x2})),
        std::make_tuple(BigUnsigned({0x0, 0x1}), BigUnsigned({0x0, 0xFFFFFFFF}), BigUnsigned({0x0, 0x0, 0x1})),
        std::make_tuple(BigUnsigned({0xFFFFFFFE}), BigUnsigned({0xFFFFFFFE}), BigUnsigned({0xFFFFFFFC, 0x1})),
        std::make_tuple(BigUnsigned({0x1}), BigUnsigned({0x1}), BigUnsigned({0x2})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}), BigUnsigned({0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}), BigUnsigned({0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF}), BigUnsigned({0x1}), BigUnsigned({0x0, 0x1}))
    )
);

