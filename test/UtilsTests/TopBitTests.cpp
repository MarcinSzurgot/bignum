#include <bignum/Bits.hpp>
#include <bignum/Utils.hpp>

#include <gtest/gtest.h>

#include <ranges>
#include <iostream>
#include <tuple>
#include <vector>

using namespace bignum;

using Unsigned = std::uint16_t;

TEST(BitsTests, tests) {
    const auto bits = Bits<std::uint8_t>(121u);

    std::cout << "0b";
    for (const auto& bit : std::ranges::reverse_view(bits)) {
        std::cout <<  static_cast<bool>(bit);
    }
    std::cout << "\n";
}

struct TopBitParams {
    std::vector<Unsigned> bits;
    std::size_t expectedTopBit;
};

class TopBitTests : public ::testing::TestWithParam<TopBitParams> {};

TEST_P(TopBitTests, TopBitTests) {
    const auto [bits, expectedTopBit] = GetParam();

    const auto actual = bignum::topBit(bits);

    ASSERT_EQ(actual, expectedTopBit);
}

INSTANTIATE_TEST_SUITE_P(
    TopBitTestsParams,
    TopBitTests,
    ::testing::Values(
        TopBitParams({123, 123, 1 << 5}, Bits<Unsigned>::Size * 2 + 5),
        TopBitParams({0}, 0),
        TopBitParams({1}, 0),
        TopBitParams({2}, 1),
        TopBitParams({3}, 1),
        TopBitParams({4}, 2),
        TopBitParams({5}, 2),
        TopBitParams({Bits<Unsigned>::Mask}, Bits<Unsigned>::Size - 1)
    )
);