#include <gtest/gtest.h>

#include <bignum/Arrays/Division.hpp>

#include "../Utils.hpp"

using namespace bignum;

template<
    std::ranges::bidirectional_range Dividend,
    std::ranges::bidirectional_range Divisor,
    std::bidirectional_iterator Quotient,
    std::bidirectional_iterator Remainder
> requires 
    std::unsigned_integral<std::ranges::range_value_t<Dividend>>
    && std::same_as<std::ranges::range_value_t<Dividend>, std::ranges::range_value_t<Divisor>>
    && std::same_as<std::ranges::range_value_t<Dividend>, std::iter_value_t<Quotient>>
    && std::same_as<std::ranges::range_value_t<Dividend>, std::iter_value_t<Remainder>>
constexpr auto div(
    Dividend&& dividend, // big-endian
    Divisor&& divisor, // big-endian
    Quotient quotient, // big-endian
    Remainder remainder // big-endian
) -> std::pair<Quotient, Remainder> {
    using Unsigned = std::ranges::range_value_t<Dividend>;

    if (!divisor) {
        throw std::runtime_error("Division by zero is not allowed!");
    }

    auto remainderRange = std::ranges::subrange(
        remainder,
        std::ranges::copy(dividend, remainder).out
    );

    if (divisor > dividend) {
        return std::make_pair(quotient, end(remainderRange));
    }

    const auto dvdSize = std::distance(begin(dividend), end(dividend));
    const auto divSize = std::distance(begin(divisor), end(divisor));
    const auto topDiv = *begin(divisor);

    while(divisor < remainderRange) {
        const auto topDvd = [&] {
            auto top = std::array<Unsigned, 2>();

            const auto it = std::reverse_iterator(end(remainderRange));
            if (*it < topDiv) {
                top[0] = *it;   
            } else {
                top[1] = *  it;
                top[0] = *--it;
            }
            return top;
        } ();

        const auto multiplier = div(topDvd[0], topDvd[1], topDiv).first;
        // const auto 
    }
}

template<
    std::ranges::bidirectional_range Range,
    std::unsigned_integral Unsigned
> requires std::same_as<Unsigned, std::ranges::range_value_t<Range>>
constexpr auto approxDiv(
    Range&& dividend, // big-endian
    Unsigned divisor
) -> std::array<Unsigned, 2> {
    auto first = std::reverse_iterator(end(dividend));
    auto last = std::reverse_iterator(begin(dividend));

    if (first == last) {
        return std::array<Unsigned, 2>();
    }

    const auto topDvd = *first;
    if(++first == last) {
        return std::array<Unsigned, 2>{
            div(topDvd, divisor).first,
            Unsigned()
        };
    }
    
    return div(*first, topDvd, divisor).first;
}

template<
    std::unsigned_integral Unsigned,
    std::size_t MultiplierSize
>
constexpr auto multiply(
    const std::array<Unsigned, MultiplierSize>& multiplier
)  {
    return std::views::transform([
            carry = std::array<Unsigned, MultiplierSize + 1>(),
            multiplier
        ](auto value) mutable -> Unsigned {
            mul(
                multiplier, 
                value, 
                begin(carry), 
                begin(carry)
            );

            const auto result = carry[0];
            std::copy(begin(carry) + 1, end(carry), begin(carry));
            return result;
    });
}

TEST(ArrayDivisionOperatorTests, siema) {
    using Unsigned = std::uint8_t;

    const auto values = std::vector<Unsigned>{50, 60, 3, 4};
    const auto multiplier = std::array {
        Unsigned(100),
        Unsigned(4),
        Unsigned(3),
        Unsigned(1)
    };

    for (const auto& v : values | multiply(multiplier)) {
        std::cout << +v << "\n";
    }

    // const auto dvd = std::vector<Unsigned>{0, 2, 0, 255, 254};
    // const auto div = std::vector<Unsigned>{0, 250};

    // const auto ratio = approxDiv(dvd, div.back());

    // std::cout << +ratio[1] << ", " << +ratio[0] << std::endl;
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