#include <bignum/BigUnsigned.hpp>

#include <bignum/Utils.hpp>
#include <bignum/ArrayArithmetics/ArrayDiv.hpp>
#include <bignum/ArrayArithmetics/ArraySubAdd.hpp>
#include <bignum/ArrayArithmetics/ArrayMul.hpp>
#include <bignum/ArrayArithmetics/ArrayShift.hpp>
#include <bignum/ArrayLogic/ArrayLogic.hpp>
#include <bignum/LogicOperators.hpp>
#include <bignum/ShiftOperators.hpp>
#include <bignum/ArithmeticOperators.hpp>

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

        digit.digits()[0] = std::atol(stringDigit.data());

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
    const auto div = powerOf10(maxDivisorPowerOf10);

    auto result = std::string();
    result.reserve(size(digits_) * maxDivisorPowerOf10);

    for (auto quot = *this, mod = BigUnsigned(); (bool) quot;) {
        const auto notLastDivision = quot > div;

        std::tie(quot, mod) = divide(quot, div);
        auto string = std::to_string(mod.digits()[0]);
        if (size(string) < maxDivisorPowerOf10 && notLastDivision) {
            const auto zeroes = std::string(maxDivisorPowerOf10 - size(string), '0');
            string.insert(string.begin(), zeroes.begin(), zeroes.end());
        }
        result.insert(result.begin(), string.begin(), string.end());
    }
    
    return result;
}

void BigUnsigned::swap(std::vector<NativeDigit>& digits) {
    std::swap(digits_, digits);
    trim();
}

void BigUnsigned::trim() {
    digits_.resize(sizeWithoutLeadingZeroes(std::span(digits_)));
}

}