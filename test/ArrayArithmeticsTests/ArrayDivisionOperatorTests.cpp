#include <gtest/gtest.h>

#include <bignum/Arrays/Comparators.hpp>
#include <bignum/Arrays/Division.hpp>

#include "../Utils.hpp"

using namespace bignum;

template<
    std::bidirectional_iterator Minuend,
    std::ranges::forward_range Subtrahend
> requires std::same_as<
    std::iter_value_t<Minuend>, 
    std::ranges::range_value_t<Subtrahend>
>
// subtrahend shouldn't be shorter than minuend.
constexpr auto cutToTop(
    Minuend minuendFirst,
    Minuend minuendLast,
    Subtrahend&& subtrahend
) -> Minuend {
    const auto minuendSize = std::distance(minuendFirst, minuendLast);
    const auto subtrahendSize = [&] {
        if constexpr (std::ranges::sized_range<Subtrahend>) {
            return size(subtrahend);
        } else {
            return std::distance(begin(subtrahend), end(subtrahend));
        }
    } ();
    const auto offset = minuendSize - subtrahendSize;

    return std::next(minuendFirst, offset);
}

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
// quotient should be at least size of the size(dividend) - size(divisor)
// remainder should be at least size of the size(divisor)
constexpr auto div3(
    Dividend&& dividend, // big-endian
    Divisor&& divisor, // big-endian
    Quotient quotient, // big-endian
    Remainder remainder // big-endian
) -> std::pair<Quotient, Remainder> {
    using Unsigned = std::ranges::range_value_t<Dividend>;

    if (empty(divisor)) {
        throw std::runtime_error("Division by zero is not allowed!");
    }

    auto remainderRange = std::ranges::subrange(
        remainder,
        std::ranges::copy(dividend, remainder).out
    );

    if (greater(divisor, dividend)) {
        return std::make_pair(quotient, end(remainderRange));
    }

    const auto quotientOffset = size(dividend) - size(divisor) + 1;

    auto quotientRange = std::ranges::subrange(
        std::next(quotient, quotientOffset < 1 ? quotientOffset : quotientOffset - 2),
        std::next(quotient, quotientOffset)
    );

    const auto topDivisor = *(end(divisor) - 1);

    std::cout << "top divisor: " << +topDivisor << "\n";

    while(less(divisor, remainderRange)) {
        const auto multiplier = approxDiv(remainderRange, topDivisor);
        auto remainderOffset = std::prev(
            end(remainderRange), 
            size(divisor) + (size(remainderRange) != size(divisor))
        );

        std::cout << "multiplier: " << +multiplier[0] << ", " << +multiplier[1] << "\n";

        mulSub(
            remainderOffset, 
            divisor,
            multiplier,
            remainderOffset
        );

        add(
            quotientRange,
            multiplier,
            begin(quotientRange)
        );

        remainderRange = std::ranges::subrange(
            begin(remainderRange),
            trimm(remainderRange)
        );
    }

    return std::make_pair(end(quotientRange), end(remainderRange));
}

TEST(ArrayDivisionOperatorTests, siema) {
    using Unsigned = std::uint8_t;

    const auto dividend = std::vector<Unsigned>{0, 0, 1, 202};
    const auto divisor  = std::vector<Unsigned>{1, 100};
    auto quotient       = std::vector(size(dividend) - size(divisor), Unsigned());
    auto remainder      = dividend;

    const auto multiplier = approxDiv(dividend, divisor[1]);

    std::cout << "multiplier: \n";
    for (const auto m : multiplier) {
        std::cout << +m << ", ";
    }

    std::cout << "\nremainder: \n";
    for (const auto r : remainder) {
        std::cout << +r << ", ";
    }

    mulSub(
        std::ranges::subrange(begin(remainder) + 1, end(remainder)),
        divisor,
        multiplier,
        begin(remainder) + 1
    );

    // auto [quotientLast, remainderLast] = div3(
    //     dividend, 
    //     divisor, 
    //     begin(quotient), 
    //     begin(remainder)
    // );

    // quotient.erase(quotientLast, end(quotient));
    // remainder.erase(remainderLast, end(remainder));
    std::cout << "\nremainder: \n";
    for (const auto r : remainder) {
        std::cout << +r << ", ";
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