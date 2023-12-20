#include <chrono>
#include <random>
#include <string>

#include <gtest/gtest.h>

#include <bignum/bignum.hpp>
#include <bignum/ModularMultiplication.hpp>

#include "Utils.hpp"

using namespace std::string_literals;

class BigUnsignedStringConversionTest : public ::testing::TestWithParam<std::tuple<std::string, BigUnsigned>> { };

TEST_P(BigUnsignedStringConversionTest, ConstructorTests) {
    const auto string = std::get<0>(GetParam());
    const auto expected = std::get<1>(GetParam());

    const auto actual = BigUnsigned(string);

    ASSERT_EQ(actual, expected);   
}


TEST(BigUnsignedStringConversionTest, OperationPerformance) {
    using namespace std::chrono;

    const auto iterations = 100u;

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

    for (auto i = 0u; i < iterations; ++i) {
        const auto expected = rnd(100);
        const auto converted = BigUnsigned(expected);
        const auto actual = static_cast<std::string>(converted);

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
        std::make_tuple("0"s, BigUnsigned()),
        std::make_tuple("1"s, BigUnsigned(1ul)),
        std::make_tuple("123"s, BigUnsigned(123ul)),
        std::make_tuple("18446744073709551616"s, BigUnsigned(1ul) << 64),
        std::make_tuple("340282366920938463463374607431768211456"s, BigUnsigned(1ul) << 128)
    )
);