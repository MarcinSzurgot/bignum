#include <bignum/bignum.hpp>

#include <chrono>
#include <concepts>
#include <iostream>
#include <random>
#include <ranges>
#include <span>
#include <vector>

template<typename U>
requires std::unsigned_integral<std::remove_const_t<U>>
std::ostream& operator<<(std::ostream& os, std::span<const U> num) {
    os << std::hex << "{0x" << num[0];
    for (auto d = 1u; d < size(num); ++d) {
        os << ", 0x" << num[d];
    }
    return os << "}";
}

template<std::unsigned_integral U>
std::ostream& operator<<(std::ostream& os, const std::vector<U>& num) {
    return os << std::span(num);
}

inline std::ostream& operator<<(std::ostream& os, const bignum::BigUnsigned& num) {
    return os << num.digits();
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

        auto dist = std::uniform_int_distribution(U(), ~U());
        std::ranges::generate(output, [this, &dist]{
            return dist(generator_);
        });
    }

    bignum::BigUnsigned bigUnsigned(std::size_t bitSize) {
        using namespace bignum;

        using DigitBits = Bits<BigUnsigned::NativeDigit>;

        const auto [whole, bit] = div<BigUnsigned::NativeDigit>(bitSize, DigitBits::Size);
        auto number = BigUnsigned(random<BigUnsigned::NativeDigit>(whole));
        number.access().digits().back() &= lshift(DigitBits::Mask, bit).second;
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