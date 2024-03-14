#include <gtest/gtest.h>

#include <bignum/Arrays/Division.hpp>

#include "Utils.hpp"

using namespace bignum;

using Big = BigUnsigned<NativeDigit>;

class BigUnsignedDivisionTests : public ::testing::TestWithParam<std::tuple<Big, Big, Big>> {};

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
        std::make_tuple(Big({0x0}), Big({0x1}), Big({0x0})),
        std::make_tuple(Big({0x8}), Big({0x4}), Big({0x2})),
        std::make_tuple(Big({0x10, 0x0}), Big({0x1}), Big({0x10, 0x0})),
        std::make_tuple(Big({0x10, 0x0}), Big({0x2}), Big({0x8, 0x0})),
        std::make_tuple(Big({0xFFFFFFFF, 0xFFFFFFFF}), Big({0xFFFFFFFF}), Big({0x1, 0x1})),
        std::make_tuple(Big({0xFFFFFFFF, 0xFFFFFFFF}), Big({0x1}), Big({0xFFFFFFFF, 0xFFFFFFFF})),
        std::make_tuple(Big({0xFFFFFFFF, 0x0}), Big({0xFFFFFFFF}), Big({0x1})),
        std::make_tuple(Big({0xFFFFFFFF, 0x0, 0x1}), Big({0x2}), Big({0x7FFFFFFF, 0x8000000000000000})),
        std::make_tuple(Big({0x1, 0x0, 0x0}), Big({0x1, 0x0}), Big({0x1})),
        std::make_tuple(Big({0x1, 0x1}), Big({0x2}), Big({0x8000000000000000})),
        std::make_tuple(Big({0x0, 0x1, 0x1}), Big({0x2}), Big({0x8000000000000000, 0x8000000000000000})),
        std::make_tuple(Big({0x0, 0x1}), Big({0x2}), Big({0x8000000000000000})),
        std::make_tuple(Big({0x0, 0x1, 0x1, 0x1}), Big({0x2}), Big({0x8000000000000000, 0x8000000000000000, 0x8000000000000000})),
        std::make_tuple(Big({0x0, 0x1, 0x1, 0x0, 0x1}), Big({0x2}), Big({0x8000000000000000, 0x8000000000000000, 0x0, 0x8000000000000000}))
    )
);