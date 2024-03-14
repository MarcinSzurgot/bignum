#include <gtest/gtest.h>

#include <bignum/bignum.hpp>

#include <iostream>

#include "Utils.hpp"

using namespace bignum;

using Big = BigUnsigned<NativeDigit>;

class BigUnsignedModuloTests : public ::testing::TestWithParam<std::tuple<Big, Big, Big>> {};

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
        std::make_tuple(Big("1000000000000000000000000000000000876"), Big("1000"), Big("876")),
        std::make_tuple(Big("6666666666666"), Big("20000000000000000000"), Big("6666666666666")),
        std::make_tuple(Big(1u) << std::size_t(1024), Big(1u) << 256u, Big()),
        std::make_tuple((Big(1u) << 4096u) + Big(1u), Big(2u), Big(1u))
    )
);
