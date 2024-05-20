#include <gtest/gtest.h>

#include <bignum/Arrays/Comparators.hpp>
#include <bignum/Arrays/Division.hpp>
#include <bignum/Ranges/Division.hpp>

#include "../Utils.hpp"

#include <chrono>
#include <thread>

using namespace bignum;

template<
    std::ranges::bidirectional_range Dividend,
    std::ranges::bidirectional_range Divisor,
    std::bidirectional_iterator Quotient,
    std::bidirectional_iterator Remainder,
    std::unsigned_integral Unsigned = std::ranges::range_value_t<Dividend>
> requires 
    std::same_as<Unsigned, std::ranges::range_value_t<Divisor>>
    && std::same_as<Unsigned, std::iter_value_t<Quotient>>
    && std::same_as<Unsigned, std::iter_value_t<Remainder>>
// quotient should be at least size of the size(dividend) - size(divisor) + 1
// remainder should be at least size of the size(dividend)
constexpr auto div3(
    Dividend&& dividend, // big-endian
    Divisor&& divisor, // big-endian
    Quotient quotient, // big-endian
    Remainder remainder // big-endian
) -> std::pair<Quotient, Remainder> {
    using namespace std::ranges;

    if (empty(divisor)) {
        throw std::runtime_error("Cannot divide by zero.");
    }

    auto remainderRange = subrange(
        remainder, 
        copy(dividend, remainder).out
    );

    auto quotientLast = std::next(quotient, size(dividend) - size(divisor) + 1);

    fill(subrange(quotient, quotientLast), Unsigned());

    for (
        auto approxDiv = bignum::approxDiv(remainderRange, divisor);
        approxDiv.first > 0;
        approxDiv      = bignum::approxDiv(remainderRange, divisor)
    ) {
        auto  quotientOffset = std::next(quotient, approxDiv.second);
        auto remainderOffset = std::next(remainder, approxDiv.second);
        auto quotientRange = subrange(
            quotientOffset,
            quotientLast
        );
        auto remainderRange2 = subrange(
            remainderOffset,
            remainderRange.end()
        );
        
        // std::cout << "divisor:                " << toString(divisor) << "\n";
        // std::cout << "approx div:             " << +approxDiv.first << ", " << approxDiv.second << "\n";
        // std::cout << "before remainder range: " << toString(remainderRange) << "\n";
        // std::cout << "before quotient range:  " << toString(quotientRange) << "\n";

        // std::this_thread::sleep_for(std::chrono::milliseconds(500));

        add(
            quotientRange,
            approxDiv.first,
            quotientRange.begin()
        );

        mulSub(
            remainderRange2,
            divisor,
            approxDiv.first,
            remainderRange2.begin()
        );

        remainderRange = subrange(remainder, trimm(remainderRange));

        // std::cout << "after remainder range:  " << toString(remainderRange) << "\n";
        // std::cout << "after quotient range:   " << toString(quotientRange) << "\n";
    }

    return {
        trimm(subrange(quotient, quotientLast)), 
        trimm(remainderRange)
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
    const auto [dividend, divisor, expected] = GetParam();

    auto quotient  = std::vector<std::uint32_t>(size(dividend) - size(divisor) + 1, 0);
    auto remainder = std::vector<std::uint32_t>(size(dividend), 0);

    const auto [quotLast, remLast] = div3(
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

TEST(ArrayDivisionOperatorTests, ArryMassiveDivisionTest) {
    using Unsigned = std::uint32_t;

    auto random = RandomGenerator();

    for (auto _ : std::views::iota(0, 1000)) {
        auto multiplicand = random.random<Unsigned>(1);
        auto multiplier   = random.random<Unsigned>(1);
        auto addend       = random.random<Unsigned>(1);

        auto product   = std::vector(size(multiplicand) + size(multiplier) + 1, Unsigned());
        auto quotient  = std::vector(size(multiplicand), Unsigned());
        auto remainder = std::vector(size(product), Unsigned());

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

        // std::cout << "siema dividend:  " << toString(product)  << "\n";
        // std::cout << "siema divisor:   " << toString(multiplier) << "\n";
        // std::cout << "siema quotient:  " << toString(multiplicand)  << "\n";
        // std::cout << "siema remainder: " << toString(addend)  << "\n";

        auto [quotLast, remLast] = div3(
            product,
            multiplier,
            begin(quotient),
            begin(remainder)
        );

        quotient .erase(quotLast, end(quotient));
        remainder.erase(remLast,  end(remainder));

        EXPECT_EQ(quotient, multiplicand)
            << "multiplicand: " << toString(multiplicand) << "\n"
            << "multiplier:   " << toString(multiplier) << "\n"
            << "addend:       " << toString(addend) << "\n"
            << "product:      " << toString(product) << "\n"
            << "quotient:     " << toString(quotient) << "\n"
            << "remainder:    " << toString(remainder) << "\n";
        EXPECT_EQ(remainder, addend);
    }
}