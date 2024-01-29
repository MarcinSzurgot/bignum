#include <concepts>

#include <gtest/gtest.h>

#include <bignum/Arrays/Multiplication.hpp>
#include <bignum/Digits/Arithmetics.hpp>

#include "Utils.hpp"

using namespace bignum;

template<std::unsigned_integral U> 
struct ModularMulArgs {
    U lhs;
    U rhs;
    std::pair<U, U> result;
};

class BigUnsignedModularMultiplicationTest : public ::testing::TestWithParam<ModularMulArgs<std::uint32_t>> {};

TEST_P(BigUnsignedModularMultiplicationTest, ModularMultiplication) {
    const auto [lhs, rhs, expected] = GetParam();

    const auto [lower, upper] = mul(lhs, rhs);

    ASSERT_EQ(lower, expected.first);
    ASSERT_EQ(upper, expected.second);
}

INSTANTIATE_TEST_SUITE_P(
    BigUnsignedTests,
    BigUnsignedModularMultiplicationTest,
    ::testing::Values(
        ModularMulArgs(0x0u, 0x0u, {0x0u, 0x0u}),
        ModularMulArgs(0x1u, 0x1u, {0x1u, 0x0u}),
        ModularMulArgs(0x0u, 0x1u, {0x0u, 0x0u}),
        ModularMulArgs(0x1u, 0x0u, {0x0u, 0x0u}),
        ModularMulArgs(0x1u, 0x2u, {0x2u, 0x0u}),
        ModularMulArgs(0x80000000u, 0x2u, {0x0u, 0x1u}),
        ModularMulArgs(0x80000000u, 0xFFFFFFFFu, {0x80000000u, 0x7FFFFFFFu}),
        ModularMulArgs(0xFFFFFFFFu, 0xFFFFFFFFu, {0x1u, 0xFFFFFFFEu}),
        ModularMulArgs(0xFFFF0000u, 0x80000000u, {0x0u, 0x7FFF8000u}),
        ModularMulArgs(0x0000FFFFu, 0x0000FFFFu, {0xFFFE0001u, 0x0u}),
        ModularMulArgs(0xFFFF0000u, 0xFFFF0000u, {0x0u, 0xFFFE0001u}),
        ModularMulArgs(0xFFFF0000u, 0x0000FFFFu, {0x00010000u, 0x0000FFFEu}),
        ModularMulArgs(0x0000FFFFu, 0xFFFF0000u, {0x00010000u, 0x0000FFFEu}),
        ModularMulArgs(0x0000FFFFu, 0xFFFFFFFFu, {0xFFFF0001u, 0x0000FFFEu}),
        ModularMulArgs(0xFFFF0000u, 0xFFFFFFFFu, {0x00010000u, 0xFFFEFFFFu}),
        ModularMulArgs(0xFFFFFFFFu, 0xFFFF0000u, {0x00010000u, 0xFFFEFFFFu}),
        ModularMulArgs(0xFFFFFFFFu, 0xFFFFFFF0u, {0x00000010u, 0xFFFFFFEFu})
    )
);

TEST(BigUnsignedModularMultiplicationTest, ModularMultiplicationAllCases) {
    using mul_type = std::uint8_t;
    using containing_type = std::uint16_t;

    constexpr auto mulTypeBitSize = sizeof(mul_type) * 8;
    constexpr auto baseSize = containing_type(1) << mulTypeBitSize;

    for (auto l = 0u; l < baseSize; ++l) {
        for (auto r = 0u; r < baseSize; ++r) {
            const auto expected = l * r;
            const auto [lower, higher] = mul<mul_type>(l, r);
            const auto actual = lower + (containing_type) higher * baseSize;

            ASSERT_EQ(expected, actual) << "for " << (int) l << " * " << (int) r;
        }
    }
}