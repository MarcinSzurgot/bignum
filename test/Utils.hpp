#include <bignum/bignum.hpp>

#include <chrono>
#include <concepts>
#include <iostream>
#include <random>
#include <ranges>
#include <span>
#include <sstream>
#include <vector>

using NativeDigit = std::uint8_t;

template<std::ranges::forward_range Range>
requires std::unsigned_integral<std::ranges::range_value_t<Range>>
std::string toString(Range&& range) {
    auto first = begin(range);
    auto last = end(range);

    if (first == last) {
        return "{}";
    }

    auto builder = std::stringstream();

    builder << std::hex << "{0x" << +(*first);
    ++first;
    for(; first != last; ++first) {
        builder << ", 0x" << +(*first);
    }
    builder << "}";
    return builder.str();
}

template<std::unsigned_integral U>
inline std::ostream& operator<<(std::ostream& os, const bignum::BigUnsigned<U>& num) {
    return os << toString(num.digits());
}

struct RandomGenerator {
    RandomGenerator() : generator_(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
    ) {

    }

    template<std::integral I>
    I random(I from, I to) {
        return std::uniform_int_distribution(from, to)(generator_);
    }

    template<std::unsigned_integral U>
    std::vector<U> random(std::size_t size) {
        auto result = std::vector<U>(size);
        random(result);
        return result;
    }

    template<std::ranges::forward_range OutputRange>
    auto random(OutputRange&& output) -> void {
        using U = std::ranges::range_value_t<OutputRange>;

        auto dist = std::uniform_int_distribution(U(), U(~U()));
        std::ranges::generate(output, [this, &dist]{
            return dist(generator_);
        });
    }

    template<std::unsigned_integral U>
    bignum::BigUnsigned<U> bigUnsigned(std::size_t bitSize) {
        using namespace bignum;

        const auto [whole, bit] = div<U>(bitSize, Bits<U>::Size);
        auto number = BigUnsigned(random<U>(whole));
        number.access().digits().back() &= lshift(Bits<U>::Mask, bit).second;
        return number;
    }

private:
    std::mt19937 generator_;
};

template<
    typename LHS, 
    typename RHS, 
    typename Expected
>
struct BinaryOpParams {
    LHS lhs;
    RHS rhs;
    Expected expected;
};

using ArithmeticBinaryOp = BinaryOpParams<
    std::vector<std::uint32_t>,
    std::vector<std::uint32_t>,
    std::vector<std::uint32_t>
>;

using LogicBinaryOp = BinaryOpParams<
    std::vector<std::uint32_t>,
    std::vector<std::uint32_t>,
    bool
>;

using ShiftOp = BinaryOpParams<
    std::vector<std::uint32_t>,
    std::size_t,
    std::vector<std::uint32_t>
>;