#include <gtest/gtest.h>

#include <bignum/ArrayArithmetics/ArrayDiv.hpp>

#include "../Utils.hpp"

using DivOpParams = BinaryOpParams<
    std::vector<std::uint32_t>,
    std::vector<std::uint32_t>,
    std::pair<
        std::vector<std::uint32_t>,
        std::vector<std::uint32_t>
    >
>;

class ArrayDivisionOperatorTests : public ::testing::TestWithParam<DivOpParams> {};

TEST_P(ArrayDivisionOperatorTests, ArryDivisionTest) {
    const auto [lhs, rhs, expected] = GetParam();

    auto quotient = lhs;
    std::fill(begin(quotient), end(quotient), 0);
    auto remainder = quotient;


    const auto [quotSize, remSize] = bignum::divide(
        std::span(lhs),
        std::span(rhs),
        std::span(quotient),
        std::span(remainder)
    );

    quotient.resize(quotSize);
    remainder.resize(remSize);

    EXPECT_EQ(quotient,  expected.first);
    EXPECT_EQ(remainder, expected.second);
}

INSTANTIATE_TEST_SUITE_P(
    ArryDivisionTestParams,
    ArrayDivisionOperatorTests,
    ::testing::Values(
        DivOpParams({0x0}, {0x1}, {{0x0}, {0x0}}),
        DivOpParams({0x8}, {0x4}, {{0x2}, {0x0}}),
        DivOpParams({0x10}, {0x1}, {{0x10}, {0x0}}),
        DivOpParams({0x10}, {0x2}, {{0x8}, {0x0}}),
        DivOpParams({0xFFFFFFFF, 0xFFFFFFFF}, {0xFFFFFFFF}, {{0x1, 0x1}, {0x0}}),
        DivOpParams({0xFFFFFFFF, 0xFFFFFFFF}, {0x1}, {{0xFFFFFFFF, 0xFFFFFFFF}, {0x0}}),
        DivOpParams({0xFFFFFFFF}, {0xFFFFFFFF}, {{0x1}, {0x0}}),
        DivOpParams({0xFFFFFFFF, 0x0, 0x1}, {0x2}, {{0x7FFFFFFF, 0x80000000}, {0x1}}),
        DivOpParams({0x1}, {0x1}, {{0x1}, {0x0}}),
        DivOpParams({0x1, 0x1}, {0x2}, {{0x80000000}, {0x1}}),
        DivOpParams({0x0, 0x1, 0x1}, {0x2}, {{0x80000000, 0x80000000}, {0x0}}),
        DivOpParams({0x0, 0x1}, {0x2}, {{0x80000000}, {0x0}}),
        DivOpParams({0x0, 0x1, 0x1, 0x1}, {0x2}, {{0x80000000, 0x80000000, 0x80000000}, {0x0}}),
        DivOpParams({0x0, 0x1, 0x1, 0x0, 0x1}, {0x2}, {{0x80000000, 0x80000000, 0x0, 0x80000000}, {0x0}}),
        DivOpParams({0x25, 0x0, 0x1}, {0x00010000}, {{0x0, 0x00010000}, {0x25}})
    )
);