#include <gtest/gtest.h>

#include <bignum/Arrays/Comparators.hpp>
#include <bignum/Arrays/Division.hpp>
#include <bignum/Ranges/Division.hpp>

#include "../Utils.hpp"

using namespace bignum;

template<
    std::ranges::input_range Minuend, 
    std::ranges::input_range Subtrahend, 
    std::unsigned_integral Unsigned,
    std::input_iterator Result
> requires
    std::same_as<Unsigned, std::iter_value_t<Result>>
    && std::same_as<Unsigned, std::ranges::range_value_t<Subtrahend>>
    && std::same_as<Unsigned, std::ranges::range_value_t<Minuend>>
constexpr auto mulSub(
    Minuend&& minuend,
    Subtrahend&& subtrahend,
    Unsigned multiplier,
    Result result
) -> void {
    auto minuendFirst = begin(minuend);
    auto mulCarry = std::array<Unsigned, 2>();
    auto subCarry = std::pair<Unsigned, Unsigned>();
    for (auto& s : subtrahend) {
        const auto [lower0, higher0] = mul(multiplier, s);

        const auto [addLower0, addHigher0] = add(mulCarry[0], lower0);
        const auto [addLower1, addHigher1] = add(mulCarry[1], higher0, addHigher0);
        
        subCarry = sub(*minuendFirst++, addLower0, subCarry.second);
        *result++ = subCarry.first;

        mulCarry[0] = addLower1;
        mulCarry[1] = addHigher1;
    }

    for(auto minuendLast = end(minuend); minuendFirst != minuendLast;) {
        subCarry = sub(*minuendFirst++, mulCarry[0], subCarry.second);
        *result++ = subCarry.first;

        mulCarry[0] = mulCarry[1];
        mulCarry[1] = Unsigned();
    }
}

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

    for (
        auto approxDiv = bignum::approxDiv(remainderRange, divisor);
        approxDiv.first > 0;
        approxDiv      = bignum::approxDiv(remainderRange, divisor)
    ) {
        auto  quotientOffset = std::next(quotient, approxDiv.second);
        auto remainderOffset = std::next(remainder, approxDiv.second);

        add(
            subrange(
                quotientOffset,
                quotientLast
            ),
            approxDiv.first,
            quotientOffset
        );

        mulSub(
            subrange(
                remainderOffset,
                remainderRange.end()
            ),
            divisor,
            approxDiv.first,
            remainderOffset
        );

        remainderRange = subrange(remainder, trimm(remainderRange));
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
        DivOpParams<std::uint32_t>({0x0}, {0x1}, {{}, {}}),
        DivOpParams<std::uint32_t>({0x8}, {0x4}, {{0x2}, {}}),
        DivOpParams<std::uint32_t>({0x10}, {0x1}, {{0x10}, {}}),
        DivOpParams<std::uint32_t>({0x10}, {0x2}, {{0x8}, {}}),
        DivOpParams<std::uint32_t>({0xFFFFFFFF, 0xFFFFFFFF}, {0xFFFFFFFF}, {{0x1, 0x1}, {}}),
        DivOpParams<std::uint32_t>({0xFFFFFFFF, 0xFFFFFFFF}, {0x1}, {{0xFFFFFFFF, 0xFFFFFFFF}, {}}),
        DivOpParams<std::uint32_t>({0xFFFFFFFF}, {0xFFFFFFFF}, {{0x1}, {}}),
        DivOpParams<std::uint32_t>({0xFFFFFFFF, 0x0, 0x1}, {0x2}, {{0x7FFFFFFF, 0x80000000}, {0x1}}),
        DivOpParams<std::uint32_t>({0x1}, {0x1}, {{0x1}, {}}),
        DivOpParams<std::uint32_t>({0x1, 0x1}, {0x2}, {{0x80000000}, {0x1}}),
        DivOpParams<std::uint32_t>({0x0, 0x1, 0x1}, {0x2}, {{0x80000000, 0x80000000}, {}}),
        DivOpParams<std::uint32_t>({0x0, 0x1}, {0x2}, {{0x80000000}, {}}),
        DivOpParams<std::uint32_t>({0x0, 0x1, 0x1, 0x1}, {0x2}, {{0x80000000, 0x80000000, 0x80000000}, {}}),
        DivOpParams<std::uint32_t>({0x0, 0x1, 0x1, 0x0, 0x1}, {0x2}, {{0x80000000, 0x80000000, 0x0, 0x80000000}, {}}),
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

TEST(ArrayDivisionOperator64bitTests, ArryMassiveDivisionTest) {
    using Unsigned = std::uint8_t;

    auto random = RandomGenerator();

    for (auto _ : std::views::iota(0, 1000)) {
        auto multiplicand = random.random<Unsigned>(5);
        auto multiplier   = random.random<Unsigned>(5);
        auto addend       = random.random<Unsigned>(5);

        auto product   = std::vector<Unsigned>(size(multiplicand) + size(multiplier) + 1, 0);
        auto quotient  = std::vector<Unsigned>(size(multiplicand), 0);
        auto remainder = std::vector<Unsigned>(size(product), 0);

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

        std::cout << "Quotient:        " << toString(quotient)  << "\n";
        std::cout << "expected.first:  " << toString(multiplicand) << "\n";
        std::cout << "remainder:       " << toString(remainder)  << "\n";
        std::cout << "product:         " << toString(product)  << "\n";
        std::cout << "expected.second: " << toString(addend) << "\n";

        auto [quotLast, remLast] = div3(
            product,
            multiplier,
            begin(quotient),
            begin(remainder)
        );

        quotient .erase(quotLast, end(quotient));
        remainder.erase(remLast,  end(remainder));

        ASSERT_EQ(quotient, multiplicand);
        ASSERT_EQ(remainder, addend);
    }
}