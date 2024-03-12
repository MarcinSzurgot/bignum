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

BigUnsigned::BigUnsigned(std::string_view string) 
    : BigUnsigned(fromChars<NativeDigit>(string)) {
}

BigUnsigned::operator bool() const {
    return size(digits_) > 1u || digits_[0] > 0u;
}

BigUnsigned::operator std::string() const { return bignum::toChars(digits_); }

BigUnsigned::Access BigUnsigned::access() { return {*this}; }

void BigUnsigned::trim() {
    digits_.resize(sizeWithoutLeadingZeroes(digits_));
}

}