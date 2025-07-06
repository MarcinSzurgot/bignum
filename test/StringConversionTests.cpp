#include <bignum/BigUnsigned.hpp>
#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Ranges/String.hpp>

#include "Utils.hpp"

#include <gtest/gtest.h>
#include <chrono>
#include <random>
#include <string>

using namespace bignum;
using namespace std::string_literals;
using namespace std::string_view_literals;

class BigUnsignedStringConversionTest : public ::testing::TestWithParam<std::tuple<std::string_view, BigUnsigned<NativeDigit>>> { };

TEST_P(BigUnsignedStringConversionTest, ConstructorTests) {
    const auto string = std::get<0>(GetParam());
    const auto expected = std::get<1>(GetParam());

    const auto actual = BigUnsigned<NativeDigit>(string);

    ASSERT_EQ(actual, expected) 
        << "actual:   " << toString(actual.digits()) << "\n"
        << "expected: " << toString(expected.digits()) << "\n";
}


TEST(BigUnsignedStringConversionTest, OperationPerformance) {
    using namespace std::chrono;

    auto random = RandomGenerator();
    auto expected = std::string();

    auto actual8  = std::string();
    auto actual16 = std::string();
    auto actual32 = std::string();
    auto actual64 = std::string();

    auto converted8  = std::vector<std::uint8_t>();
    auto converted16 = std::vector<std::uint16_t>();
    auto converted32 = std::vector<std::uint32_t>();
    auto converted64 = std::vector<std::uint64_t>();

    const auto start = high_resolution_clock::now();

    for (auto i : std::views::iota(0, 1000)) {
        expected.resize(random.random(1u, 100u));
        random.string(expected);

        bignum::fromChars(expected, converted8);
        bignum::fromChars(expected, converted16);
        bignum::fromChars(expected, converted32);
        bignum::fromChars(expected, converted64);

        bignum::toChars(converted8, actual8);
        bignum::toChars(converted16, actual16);
        bignum::toChars(converted32, actual32);
        bignum::toChars(converted64, actual64);

        ASSERT_EQ(actual8,  expected);
        ASSERT_EQ(actual16, expected);
        ASSERT_EQ(actual32, expected);
        ASSERT_EQ(actual64, expected);
    }

    const auto end = high_resolution_clock::now();
    const auto elapsed = duration_cast<milliseconds>(end - start).count();

    std::cout << "BigUnsignedStringConversionTest, OperationPerformance, elapsed time: " << elapsed << std::endl;
    EXPECT_LT(elapsed, 50);
}

INSTANTIATE_TEST_SUITE_P(
    BigUnsignedTests,
    BigUnsignedStringConversionTest,
    ::testing::Values(
        std::make_tuple("0"sv, BigUnsigned<NativeDigit>()),
        std::make_tuple("1"sv, BigUnsigned<NativeDigit>(1ul)),
        std::make_tuple("123"sv, BigUnsigned<NativeDigit>(123ul)),
        std::make_tuple("18446744073709551616"sv, BigUnsigned<NativeDigit>(1ul) << 64u),
        std::make_tuple("340282366920938463463374607431768211456"sv, BigUnsigned<NativeDigit>(1ul) << 128u)
    )
);