#include <bignum/BigUnsigned.hpp>

#include <bignum/Arrays/Division.hpp>
#include <bignum/Arrays/Comparators.hpp>
#include <bignum/Access.hpp>
#include <bignum/LogicOperators.hpp>
#include <bignum/ArithmeticOperators.hpp>
#include <bignum/Ranges/Division.hpp>
#include <bignum/Ranges/String.hpp>

#include <array>
#include <iostream>
#include <tuple>

namespace bignum {

template<>
BigUnsigned::BigUnsigned<BigUnsigned::NativeDigit>(
    NativeDigit digit
) : BigUnsigned { digit } { 
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
    const auto maxDivisorPowerOf10 = maxPower<NativeDigit>(10);
    
    auto digit = BigUnsigned();
    for (auto s = 0u; s < size(string); s += maxDivisorPowerOf10) {
        const auto stringDigit = std::string(
            string.begin() + s,
            string.begin() + std::min(s + maxDivisorPowerOf10, string.size())
        );

        digit.digits_[0] = std::atol(stringDigit.data());

        *this += digit;
        if (s + size(stringDigit) < size(string)) {
            *this *= powers<NativeDigit>(std::min(
                maxDivisorPowerOf10, 
                size(string) - (s + size(stringDigit))
            ));
        }
    }
}

BigUnsigned::operator bool() const {
    return size(digits_) > 1u || digits_[0] > 0u;
}

BigUnsigned::operator std::string() const {
          auto result = std::string(size(digits_) * 30, '0');
    const auto range = bignum::string(digits_, result.data());

    result.erase(
        begin(result), 
        begin(result) + (range.data() - result.data())
    );
    
    return result;
}

BigUnsigned::Access BigUnsigned::access() { return {*this}; }

void BigUnsigned::trim() {
    digits_.resize(sizeWithoutLeadingZeroes(std::span(digits_)));
}

}