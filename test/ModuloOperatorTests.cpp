#include <gtest/gtest.h>

#include <bignum/bignum.hpp>

#include <iostream>

using namespace bignum;

class BigUnsignedModuloTests : public ::testing::TestWithParam<std::tuple<BigUnsigned, BigUnsigned, BigUnsigned>> {};

TEST_P(BigUnsignedModuloTests, ModuloAssignmentOperator) {
          auto num1     = std::get<0>(GetParam());
    const auto num2     = std::get<1>(GetParam());
    const auto expected = std::get<2>(GetParam());

    num1 %= num2;

    EXPECT_EQ(num1, expected);
}

INSTANTIATE_TEST_SUITE_P(
    ModuloTests,
    BigUnsignedModuloTests,
    ::testing::Values(
        std::make_tuple(BigUnsigned("1000000000000000000000000000000000876"), BigUnsigned("1000"), BigUnsigned("876")),
        std::make_tuple(BigUnsigned("6666666666666"), BigUnsigned("20000000000000000000"), BigUnsigned("6666666666666")),
        std::make_tuple(BigUnsigned(1u) << 1024, BigUnsigned(1u) << 256, BigUnsigned()),
        std::make_tuple((BigUnsigned(1u) << 4096) + BigUnsigned(1u), BigUnsigned(2u), BigUnsigned(1u))
    )
);
