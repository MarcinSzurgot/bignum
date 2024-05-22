#include <gtest/gtest.h>

#include <bignum/Ranges/Comparators.hpp>
#include <bignum/Ranges/Division.hpp>

#include "../Utils.hpp"

#include <chrono>
#include <thread>

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
    const auto [dividend, divisor, expected] = GetParam();

    auto quotient  = std::vector<std::uint32_t>(size(dividend) - size(divisor) + 1, 0);
    auto remainder = std::vector<std::uint32_t>(size(dividend), 0);

    const auto [quotLast, remLast] = div(
        dividend,
        divisor,
        begin(quotient),
        begin(remainder)
    );

    quotient .erase(quotLast, end(quotient));
    remainder.erase( remLast, end(remainder));

    EXPECT_EQ(quotient,  expected.first);
    EXPECT_EQ(remainder, expected.second);
}

INSTANTIATE_TEST_SUITE_P(
    ArryDivisionTestParams,
    ArrayDivisionOperatorTests,
    ::testing::Values(
        DivOpParams<std::uint32_t>(
            {0x0}, 
            {0x1}, 
            {
                {}, 
                {}
            }
        ),
        DivOpParams<std::uint32_t>(
            {0x8}, 
            {0x4}, 
            {
                {0x2}, 
                {}
            }
        ),
        DivOpParams<std::uint32_t>(
            {0x10}, 
            {0x1}, 
            {
                {0x10}, 
                {}
            }
        ),
        DivOpParams<std::uint32_t>(
            {0x10}, 
            {0x2}, 
            {
                {0x8}, 
                {}
            }
        ),
        DivOpParams<std::uint32_t>(
            {0xFFFFFFFF, 0xFFFFFFFF}, 
            {0xFFFFFFFF}, 
            {
                {0x1, 0x1}, 
                {}
            }
        ),
        DivOpParams<std::uint32_t>(
            {0xFFFFFFFF, 0xFFFFFFFF}, 
            {0x1}, 
            {
                {0xFFFFFFFF, 0xFFFFFFFF}, 
                {}
            }
        ),
        DivOpParams<std::uint32_t>(
            {0xFFFFFFFF}, 
            {0xFFFFFFFF}, 
            {
                {0x1}, 
                {}
            }
        ),
        DivOpParams<std::uint32_t>(
            {0xFFFFFFFF, 0x0, 0x1}, 
            {0x2},
            {
                {0x7FFFFFFF, 0x80000000}, 
                {0x1}
            }
        ),
        DivOpParams<std::uint32_t>(
            {0x1}, 
            {0x1}, 
            {
                {0x1}, 
                {}
            }
        ),
        DivOpParams<std::uint32_t>(
            {0x1, 0x1}, 
            {0x2}, 
            {
                {0x80000000}, 
                {0x1}
            }
        ),
        DivOpParams<std::uint32_t>(
            {0x0, 0x1, 0x1}, 
            {0x2}, 
            {
                {0x80000000, 0x80000000}, 
                {}
            }
        ),
        DivOpParams<std::uint32_t>(
            {0x0, 0x1}, 
            {0x2}, 
            {
                {0x80000000}, 
                {}
            }
        ),
        DivOpParams<std::uint32_t>(
            {0x0, 0x1, 0x1, 0x1}, 
            {0x2}, 
            {
                {0x80000000, 0x80000000, 0x80000000}, 
                {}
            }
        ),
        DivOpParams<std::uint32_t>(
            {0x0, 0x1, 0x1, 0x0, 0x1}, 
            {0x2}, 
            {
                {0x80000000, 0x80000000, 0x0, 0x80000000}, 
                {}
            }
        ),
        DivOpParams<std::uint32_t>(
            {0x25, 0x0, 0x1}, 
            {0x00010000}, 
            {
                {0x0, 0x00010000}, 
                {0x25}
            }
        ),
        DivOpParams<std::uint32_t>(
            {0, 0, 1},
            {0, 1},
            {
                {0, 1},
                {}
            }
        ),
        DivOpParams<std::uint32_t>(
            {0xbc7912ff, 0xe71361cd, 0xce04705b, 0x2143b6a9},
            {0x903d603f, 0x31b656fc},
            {
                {0xfd9d78e7, 0xab4cd02e},
                {0x8656b226, 0x2e64e736}
            }
        ),
        DivOpParams<std::uint32_t>(
            {0x149ea29e, 0x21a2408},
            {0x3d4fa99},
            {
                {0x8C6F6D8A},
                {0xC56724}
            }
        )
    )
);

template<std::unsigned_integral Unsigned>
void testingMassiveDivision(
    std::size_t iterations,
    std::size_t maxMultiplicandSize,
    std::size_t maxMultiplierSize
) {
    auto random = RandomGenerator();

    auto multiplicand = std::vector<Unsigned>();
    auto multiplier   = std::vector<Unsigned>();
    auto addend       = std::vector<Unsigned>();
    auto product      = std::vector<Unsigned>();
    auto quotient     = std::vector<Unsigned>();
    auto remainder    = std::vector<Unsigned>();

    const auto emptyRemainder = std::vector<Unsigned>();

    for (auto _ : std::views::iota(std::size_t(), iterations)) {
        const auto multiplicandSize = random.random(std::size_t(1), maxMultiplicandSize);
        const auto multiplierSize   = random.random(std::size_t(1), maxMultiplierSize);
        const auto addendSize       = random.random(std::size_t(1), multiplierSize);

        multiplicand.resize(multiplicandSize);
        multiplier  .resize(multiplierSize);
        addend      .resize(addendSize);

        random.random(multiplicand);
        random.nonZero(multiplier);
        random.random(addend);

        multiplicand.erase(trimm(multiplicand), end(multiplicand));
        multiplier  .erase(trimm(multiplier),   end(multiplier));
        addend      .erase(trimm(addend),       end(addend));

        // addend has to be less than multiplier if later it is supposed to be a remainder.
        while(greaterEqual(addend, multiplier)) {
            addend.resize(addendSize);
            random.random(addend);
            addend.erase(trimm(addend), end(addend));
        }

        product  .resize(size(multiplicand) + size(multiplier) + 1, Unsigned());
        quotient .resize(size(multiplicand) + 1, Unsigned());
        remainder.resize(size(product), Unsigned());

        std::ranges::fill(product,   Unsigned());
        std::ranges::fill(quotient,  Unsigned());
        std::ranges::fill(remainder, Unsigned());

        mul(
            multiplicand,
            multiplier,
            begin(product)
        );

        add(
            product,
            addend,
            begin(product)
        );

        product.erase(trimm(product), end(product));

        const auto [quotLast, remLast] = div(
            product,
            multiplier,
            begin(quotient),
            begin(remainder)
        );

        quotient .erase(quotLast, end(quotient));
        remainder.erase(remLast,  end(remainder));

        EXPECT_EQ(quotient, multiplicand);
        EXPECT_EQ(remainder, addend);
    }
}

TEST(ArrayDivisionOperatorTests, ArrayMassiveDivisionTest) {
    testingMassiveDivision<std::uint8_t >(10000, 1, 1);
    testingMassiveDivision<std::uint8_t >(10000, 2, 2);
    testingMassiveDivision<std::uint16_t>(10000, 1, 1);
    testingMassiveDivision<std::uint32_t>(10000, 1, 1);
    testingMassiveDivision<std::uint64_t>(10000, 1, 1);

    testingMassiveDivision<std::uint8_t >(1000, 100, 100);
    testingMassiveDivision<std::uint16_t>(1000, 20, 20);
    testingMassiveDivision<std::uint32_t>(1000, 20, 20);
    testingMassiveDivision<std::uint64_t>(1000, 20, 20);
}