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
        std::make_tuple(BigUnsigned({0x0, 0x1}), BigUnsigned({0x0, 0x2}), BigUnsigned({0x0, 0x0, 0x2})), // 7
        std::make_tuple(BigUnsigned({0x1, 0x0}), BigUnsigned({0x2, 0x0}), BigUnsigned({0x2, 0x0, 0x0})),
        std::make_tuple(BigUnsigned({0xFFFFFFFF}), BigUnsigned({0xFFFFFFFF}), BigUnsigned({0x1, 0xFFFFFFFE})),
        std::make_tuple(BigUnsigned({0xFFFFFFF0, 0x0, 0x0, 0x1}), BigUnsigned({0xF}), BigUnsigned({0xFFFFFF10, 0xE, 0x0, 0xF})),
        std::make_tuple(BigUnsigned({0xFFFFFFFE, 0xFFFFFFFE}), BigUnsigned({0xFFFFFFFD, 0xFFFFFFFD}), BigUnsigned({0x6, 0x7, 0xFFFFFFFD, 0xFFFFFFFC})),
        std::make_tuple(BigUnsigned({0x7}), BigUnsigned({0xFFFFFFFD}), BigUnsigned({0xFFFFFFEB, 0x6})),
        std::make_tuple(BigUnsigned({0xFFFFFFFC, 0x3}), BigUnsigned({0x4, 0x4}), BigUnsigned({0xFFFFFFF0, 0xFFFFFFFF, 0xF})),
        std::make_tuple(BigUnsigned({0xFFFFFFFE}), BigUnsigned({0xFFFFFFFE}), BigUnsigned({0x4, 0xFFFFFFFC}))
    )
);

class BigUnsignedModularMultiplicationTest : public ::testing::TestWithParam<std::tuple<std::uint32_t, std::uint32_t, std::pair<std::uint32_t, std::uint32_t>>> {};

TEST_P(BigUnsignedModularMultiplicationTest, ModularMultiplication) {
    const auto lhs      = std::get<0>(GetParam());
    const auto rhs      = std::get<1>(GetParam());
    const auto expected = std::get<2>(GetParam());

    const auto [lower, upper] = mul(lhs, rhs);

    ASSERT_EQ(lower, expected.first);
    ASSERT_EQ(upper, expected.second);
}

INSTANTIATE_TEST_SUITE_P(
    BigUnsignedTests,
    BigUnsignedModularMultiplicationTest,
    ::testing::Values(
        std::make_tuple(0x0, 0x0, std::make_pair(0x0, 0x0)),
        std::make_tuple(0x1, 0x1, std::make_pair(0x1, 0x0)),
        std::make_tuple(0x0, 0x1, std::make_pair(0x0, 0x0)),
        std::make_tuple(0x1, 0x0, std::make_pair(0x0, 0x0)),
        std::make_tuple(0x1, 0x2, std::make_pair(0x2, 0x0)),
        std::make_tuple(0x80000000, 0x2, std::make_pair(0x0, 0x1)),
        std::make_tuple(0x80000000, 0xFFFFFFFF, std::make_pair(0x80000000, 0x7FFFFFFF)),
        std::make_tuple(0xFFFFFFFF, 0xFFFFFFFF, std::make_pair(0x1, 0xFFFFFFFE)),
        std::make_tuple(0xFFFF0000, 0x80000000, std::make_pair(0x0, 0x7FFF8000)),
        std::make_tuple(0x0000FFFF, 0x0000FFFF, std::make_pair(0xFFFE0001, 0x0)),
        std::make_tuple(0xFFFF0000, 0xFFFF0000, std::make_pair(0x0, 0xFFFE0001)),
        std::make_tuple(0xFFFF0000, 0x0000FFFF, std::make_pair(0x00010000, 0x0000FFFE)),
        std::make_tuple(0x0000FFFF, 0xFFFF0000, std::make_pair(0x00010000, 0x0000FFFE)),
        std::make_tuple(0x0000FFFF, 0xFFFFFFFF, std::make_pair(0xFFFF0001, 0x0000FFFE)),
        std::make_tuple(0xFFFF0000, 0xFFFFFFFF, std::make_pair(0x00010000, 0xFFFEFFFF)),
        std::make_tuple(0xFFFFFFFF, 0xFFFF0000, std::make_pair(0x00010000, 0xFFFEFFFF)),
        std::make_tuple(0xFFFFFFFF, 0xFFFFFFF0, std::make_pair(0x00000010, 0xFFFFFFEF))
        // std::make_tuple()
        // BigUnsigned({0x0, 0x1}), BigUnsigned({0x0, 0x2}), BigUnsigned({0x0, 0x0, 0x2})
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