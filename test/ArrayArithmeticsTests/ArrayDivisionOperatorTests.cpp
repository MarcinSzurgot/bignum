#include <gtest/gtest.h>

#include <bignum/Arrays/Division.hpp>

#include "../Utils.hpp"

using namespace bignum;

template<std::unsigned_integral U>
using DivOpParams = BinaryOpParams<
    std::vector<U>,
    std::vector<U>,
    std::pair<
        std::vector<U>,
        std::vector<U>
    >
>;

class ArrayDivisionOperatorTests : public ::testing::TestWithParam<DivOpParams<std::uint32_t>> {};

TEST_P(ArrayDivisionOperatorTests, ArryDivisionTest) {
    const auto [lhs, rhs, expected] = GetParam();

    auto quotient = lhs;
    std::ranges::fill(quotient, 0);
    auto remainder = quotient;


    const auto [quotSize, remSize] = div(
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
        DivOpParams<std::uint32_t>({0x0}, {0x1}, {{0x0}, {0x0}}),
        DivOpParams<std::uint32_t>({0x8}, {0x4}, {{0x2}, {0x0}}),
        DivOpParams<std::uint32_t>({0x10}, {0x1}, {{0x10}, {0x0}}),
        DivOpParams<std::uint32_t>({0x10}, {0x2}, {{0x8}, {0x0}}),
        DivOpParams<std::uint32_t>({0xFFFFFFFF, 0xFFFFFFFF}, {0xFFFFFFFF}, {{0x1, 0x1}, {0x0}}),
        DivOpParams<std::uint32_t>({0xFFFFFFFF, 0xFFFFFFFF}, {0x1}, {{0xFFFFFFFF, 0xFFFFFFFF}, {0x0}}),
        DivOpParams<std::uint32_t>({0xFFFFFFFF}, {0xFFFFFFFF}, {{0x1}, {0x0}}),
        DivOpParams<std::uint32_t>({0xFFFFFFFF, 0x0, 0x1}, {0x2}, {{0x7FFFFFFF, 0x80000000}, {0x1}}),
        DivOpParams<std::uint32_t>({0x1}, {0x1}, {{0x1}, {0x0}}),
        DivOpParams<std::uint32_t>({0x1, 0x1}, {0x2}, {{0x80000000}, {0x1}}),
        DivOpParams<std::uint32_t>({0x0, 0x1, 0x1}, {0x2}, {{0x80000000, 0x80000000}, {0x0}}),
        DivOpParams<std::uint32_t>({0x0, 0x1}, {0x2}, {{0x80000000}, {0x0}}),
        DivOpParams<std::uint32_t>({0x0, 0x1, 0x1, 0x1}, {0x2}, {{0x80000000, 0x80000000, 0x80000000}, {0x0}}),
        DivOpParams<std::uint32_t>({0x0, 0x1, 0x1, 0x0, 0x1}, {0x2}, {{0x80000000, 0x80000000, 0x0, 0x80000000}, {0x0}}),
        DivOpParams<std::uint32_t>({0x25, 0x0, 0x1}, {0x00010000}, {{0x0, 0x00010000}, {0x25}})
    )
);

class ArrayDivisionOperator64bitTests : public ::testing::TestWithParam<DivOpParams<std::uint64_t>> {};

TEST_P(ArrayDivisionOperator64bitTests, ArryDivisionTest) {
    const auto [lhs, rhs, expected] = GetParam();

    auto quotient = lhs;
    std::ranges::fill(quotient, 0);
    auto remainder = quotient;

    const auto [quotSize, remSize] = div(
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
    ArrayDivisionOperator64bitTests,
    ::testing::Values(
        DivOpParams<std::uint64_t>({0xFFFFFFFF, 0x0, 0x1}, {0x2}, {{0x7FFFFFFF, 0x8000000000000000}, {0x1}})
    )
);