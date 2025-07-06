#include <bignum/bignum.hpp>

#include <bignum/Ranges/Comparators.hpp>

#include <chrono>
#include <concepts>
#include <iostream>
#include <random>
#include <ranges>
#include <span>
#include <sstream>
#include <vector>

using NativeDigit = std::uint8_t;

template<std::ranges::input_range Range>
requires std::unsigned_integral<std::ranges::range_value_t<Range>>
std::string toString(Range&& range) {
    using std::begin;
    using std::end;

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

    template<std::integral Integer>
    auto random(
        Integer from =  Integer(), 
        Integer to   = ~Integer()
    ) -> Integer {
        return std::uniform_int_distribution(from, to)(generator_);
    }

    template<std::unsigned_integral U>
    std::vector<U> random(std::size_t size) {
        auto result = std::vector<U>(size);
        random(result);
        return result;
    }

    template<std::ranges::input_range OutputRange>
    auto random(OutputRange&& output) -> void {
        using U = std::ranges::range_value_t<OutputRange>;

        auto dist = std::uniform_int_distribution(U(), U(~U()));
        std::ranges::generate(output, [this, &dist]{
            return dist(generator_);
        });
    }

    template<std::ranges::bidirectional_range Range>
    auto nonZero(Range&& output) -> void {
        for(
            random(output);
            bignum::zero(output);
            random(output)
        );
    }

    template<std::unsigned_integral Unsigned, std::size_t Size>
    auto random() -> std::array<Unsigned, Size> {
        auto result = std::array<Unsigned, Size>();
        random(result);
        return result;
    }

    template<std::unsigned_integral U>
    bignum::BigUnsigned<U> bigUnsigned(std::size_t bitSize) {
        using namespace bignum;

        const auto [whole, bit] = div<U>(bitSize, Bits<U>::Size);
        auto number = BigUnsigned(random<U>(whole));
        number.access().digits().back() &= lshift(Bits<U>::Mask, bit).second;
        return number;
    }

    template<std::ranges::input_range String>
    requires 
        std::same_as<char, std::ranges::range_value_t<String>>
        && std::ranges::sized_range<String>
    auto string(String&& string) {
        using namespace std::string_literals;

        auto first = std::begin(string);
        auto last  = std::end(string);

        if (first == last) {
            return;
        }

        *first++ = random<char>(
            '0' + (size(string) > 1),
            '9'
        );

        std::generate(first, last, [this] { 
            return random<char>('0', '9'); 
        });
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