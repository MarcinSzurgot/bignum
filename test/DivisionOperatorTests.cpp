#include <gtest/gtest.h>

#include <bignum/Ranges/Division.hpp>

#include "Utils.hpp"

using namespace bignum;

using Big = BigUnsigned<NativeDigit>;

using Unsigned = std::uint64_t;
using ArithmeticParams = BinaryOpParams<
    std::vector<Unsigned>,
    std::vector<Unsigned>,
    std::vector<Unsigned>
>;


class BigUnsignedDivisionTests : public ::testing::TestWithParam<ArithmeticParams> {};

TEST_P(BigUnsignedDivisionTests, DivisionAssignmentOperator) {
    const auto [num1, num2, expected] = GetParam();

    auto quotient  = std::vector<Unsigned>(size(num1) - size(num2) + 1);
    auto remainder = std::vector<Unsigned>(size(num1));

    const auto [quotLast, _] = bignum::div(
        num1, 
        num2, 
        begin(quotient),
        begin(remainder)
    );

    quotient.erase(quotLast, end(quotient));

    EXPECT_EQ(quotient, expected) 
        << "quotient: " << toString(quotient) << "\n"
        << "expected: " << toString(expected) << "\n";
}

INSTANTIATE_TEST_SUITE_P(
    DivisionTests,
    BigUnsignedDivisionTests,
    ::testing::Values(
        ArithmeticParams({0x0}, {0x1}, {}),
        ArithmeticParams({0x8}, {0x4}, {0x2}),
        ArithmeticParams({0x10}, {0x1}, {0x10}),
        ArithmeticParams({0x10}, {0x2}, {0x8}),
        ArithmeticParams({0xFFFFFFFF, 0xFFFFFFFF}, {0xFFFFFFFF}, {0x1, 0x1}),
        ArithmeticParams({0xFFFFFFFF, 0xFFFFFFFF}, {0x1}, {0xFFFFFFFF, 0xFFFFFFFF}),
        ArithmeticParams({0x0, 0xFFFFFFFF}, {0xFFFFFFFF}, {0x0, 0x1}),
        ArithmeticParams({0xFFFFFFFF, 0x0, 0x1}, {0x2}, {0x7FFFFFFF, 0x8000000000000000}),
        ArithmeticParams({0x0, 0x0, 0x1}, {0x0, 0x1}, {0x0, 0x1}),
        ArithmeticParams({0x1, 0x1}, {0x2}, {0x8000000000000000}),
        ArithmeticParams({0x0, 0x1, 0x1}, {0x2}, {0x8000000000000000, 0x8000000000000000}),
        ArithmeticParams({0x0, 0x1}, {0x2}, {0x8000000000000000}),
        ArithmeticParams({0x0, 0x1, 0x1, 0x1}, {0x2}, {0x8000000000000000, 0x8000000000000000, 0x8000000000000000}),
        ArithmeticParams({0x0, 0x1, 0x1, 0x0, 0x1}, {0x2}, {0x8000000000000000, 0x8000000000000000, 0x0, 0x8000000000000000})
    )
);