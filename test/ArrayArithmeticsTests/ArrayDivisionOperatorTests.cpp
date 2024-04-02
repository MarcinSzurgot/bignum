#include <gtest/gtest.h>

#include <bignum/Arrays/Division.hpp>

#include "../Utils.hpp"

using namespace bignum;

template<
    typename U1, 
    typename U2, 
    typename U3, 
    typename U4
> requires 
    std::unsigned_integral<std::remove_const_t<U1>>
    && std::unsigned_integral<std::remove_const_t<U2>>
    && std::unsigned_integral<U3>
    && std::unsigned_integral<U4>
    && std::same_as<std::remove_const_t<U1>, std::remove_const_t<U2>>
    && std::same_as<std::remove_const_t<U2>, U3>
    && std::same_as<U3, U4>
auto div2(
    std::span<U1> lhs,
    std::span<U2> rhs,
    std::span<U3> quotient,
    std::span<U4> remainder
) -> std::pair<
    typename std::span<U3>::size_type, 
    typename std::span<U4>::size_type
> {
    if (!rhs) {
        throw std::runtime_error("Division by zero is not allowed!");
    }

    std::ranges::fill(quotient, 0);
    std::ranges::copy(lhs, begin(remainder));

    if (lhs < rhs) {
        return {size(quotient), size(remainder)};
    }

    const auto rhsTopBit = topBit(rhs);
    const auto lhsTopBit = topBit(lhs);

    auto bitDiff = lhsTopBit - rhsTopBit;
    auto divider = std::vector<U3>(size(lhs));
    auto divSpan = std::span(divider);
    divSpan.back() |= lshift(
        rhs, 
        bitDiff, 
        divSpan.subspan(bitDiff / Bits<U1>::Size).begin()
    );

    while (remainder >= rhs) {
        const auto newBitDiff = topBit(remainder) - rhsTopBit;
        const auto [wholeDigitShift, bitShift] = div<U1>(bitDiff - newBitDiff, Bits<U1>::Size);

        rshift(
            divSpan.subspan(wholeDigitShift),
            bitShift, 
            divSpan.begin()
        );

        divSpan = divSpan.subspan(0, size(divSpan) - wholeDigitShift);
        divSpan = {begin(divSpan), trimm(divSpan)};

        bitDiff = newBitDiff;

        if (divSpan > remainder) {
            rshift(divSpan, 1, divSpan.begin());
            divSpan.subspan(0, size(divSpan) * (bool) divSpan.back());
            bitDiff--;
        }

        quotient[bitDiff / Bits<U1>::Size] |= U3(1) << (bitDiff & Bits<U1>::ShiftMask);

        sub(remainder, divSpan, remainder.begin());

        remainder = {begin(remainder), trimm(remainder)};     
    }

    return {
        // 0u,0u
        size(quotient) - (quotient.back() == 0u), 
        size(remainder)
    };
}

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


    const auto [quotSize, remSize] = div2(
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