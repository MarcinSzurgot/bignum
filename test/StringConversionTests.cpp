#include <chrono>
#include <random>
#include <string>

#include <gtest/gtest.h>

#include <bignum/BigUnsigned.hpp>
#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Ranges/String.hpp>

#include "Utils.hpp"

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

    auto rnd = [
        generator = std::mt19937(
            high_resolution_clock::now()
            .time_since_epoch()
            .count()
        )
    ] (int size) mutable -> std::string {
        if (size == 0) {
            return ""s;
        }

        auto dist = std::uniform_int_distribution<char>('0', '9');

        if (size == 1) {
            return std::string(1, dist(generator));
        }

        auto string = std::string();
        string.resize(size);

        while ((string[0] = dist(generator)) == '0');

        for (auto c = 1u; c < size; ++c) {
            string[c] = dist(generator);
        }

        return string;
    };

    const auto start = high_resolution_clock::now();

    for (auto i : std::views::iota(0, 1000)) {
        const auto expected = rnd(100);
        const auto converted = bignum::fromChars<std::uint64_t>(expected);
        const auto actual = bignum::toChars(converted);

        ASSERT_EQ(actual, expected);
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