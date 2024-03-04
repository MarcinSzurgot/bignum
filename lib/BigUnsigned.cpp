#include <bignum/BigUnsigned.hpp>

#include <bignum/Arrays/Division.hpp>
#include <bignum/Arrays/Comparators.hpp>
#include <bignum/Access.hpp>
#include <bignum/LogicOperators.hpp>
#include <bignum/ArithmeticOperators.hpp>
#include <bignum/Ranges/Division.hpp>

#include <array>
#include <iostream>
#include <tuple>

namespace bignum {

template<>
BigUnsigned::BigUnsigned<BigUnsigned::NativeDigit>(
    NativeDigit digit
) : BigUnsigned { digit } { 
}

namespace {

constexpr auto digitBitSize = sizeof(bignum::BigUnsigned::NativeDigit) * 8;

static auto powerOf10(
    unsigned power
) -> const bignum::BigUnsigned& {
    const static auto powers = std::array {
        bignum::BigUnsigned(1u),
        bignum::BigUnsigned(10u),
        bignum::BigUnsigned(100u),
        bignum::BigUnsigned(1000u),
        bignum::BigUnsigned(10000u),
        bignum::BigUnsigned(100000u),
        bignum::BigUnsigned(1000000u),
        bignum::BigUnsigned(10000000u),
        bignum::BigUnsigned(100000000u),
        bignum::BigUnsigned(1000000000u),
        bignum::BigUnsigned(10000000000u),
        bignum::BigUnsigned(100000000000u),
        bignum::BigUnsigned(1000000000000u),
        bignum::BigUnsigned(10000000000000u),
        bignum::BigUnsigned(100000000000000u),
        bignum::BigUnsigned(1000000000000000u),
        bignum::BigUnsigned(10000000000000000u),
        bignum::BigUnsigned(100000000000000000u),
        bignum::BigUnsigned(1000000000000000000u),
        bignum::BigUnsigned(10000000000000000000u)
    };

    return powers[power];
}

}

BigUnsigned::BigUnsigned() : BigUnsigned(NativeDigit()) {}

BigUnsigned::BigUnsigned(
    std::initializer_list<BigUnsigned::NativeDigit> digits
) : BigUnsigned(std::vector(begin(digits), end(digits))) {}

BigUnsigned::BigUnsigned(
    const std::vector<NativeDigit>& digits
) : BigUnsigned(std::vector(digits)) {
}

BigUnsigned::BigUnsigned(
    std::vector<NativeDigit>&& digits
) : digits_(std::move(digits)) {
    trim();
}

BigUnsigned::BigUnsigned(std::string string) : BigUnsigned(NativeDigit()) {
    const auto maxDivisorPowerOf10 = std::size_t(18);
    
    auto digit = BigUnsigned();
    for (auto s = 0u; s < size(string); s += maxDivisorPowerOf10) {
        const auto stringDigit = std::string(
            string.begin() + s,
            string.begin() + std::min(s + maxDivisorPowerOf10, string.size())
        );

        digit.digits_[0] = std::atol(stringDigit.data());

        *this += digit;
        if (s + size(stringDigit) < size(string)) {
            *this *= powerOf10(std::min(maxDivisorPowerOf10, size(string) - (s + size(stringDigit))));
        }
    }
}

BigUnsigned::operator bool() const {
    return size(digits_) > 1u || digits_[0] > 0u;
}

BigUnsigned::operator std::string() const {

    if (!*this) {
        return "0";
    }

    const auto maxDivisorPowerOf10 = std::size_t(18);
    const auto divisor = powerOf10(maxDivisorPowerOf10).digits()[0];

    auto result = std::string();
    result.reserve(size(digits_) * maxDivisorPowerOf10);

    auto quot = digits_;

    for (auto quotSpan = std::span(quot); !!quotSpan;) {
        const auto notLastDivision = (size(quotSpan) > 1) || (quotSpan[0] > divisor);
        const auto mod = div(
            std::ranges::reverse_view(quotSpan), 
            divisor,
            std::reverse_iterator(end(quotSpan))
        );

        if (size(quotSpan) > 1 && !quotSpan.back()) {
            quotSpan = quotSpan.subspan(0, size(quotSpan) - 1);
        }

        auto string = std::to_string(mod);
        if (size(string) < maxDivisorPowerOf10 && notLastDivision) {
            const auto zeroes = std::string(maxDivisorPowerOf10 - size(string), '0');
            string.insert(string.begin(), zeroes.begin(), zeroes.end());
        }
        result.insert(result.begin(), string.begin(), string.end());
    }
    
    return result;
}

BigUnsigned::Access BigUnsigned::access() { return {*this}; }

void BigUnsigned::trim() {
    digits_.resize(sizeWithoutLeadingZeroes(std::span(digits_)));
}

}