#include <bignum/BigUnsigned.hpp>

#include <bignum/Digits.hpp>
#include <bignum/Operations.hpp>
#include <bignum/ArrayArithmetics/ArraySubAdd.hpp>
#include <bignum/ArrayArithmetics/ArrayMul.hpp>
#include <bignum/ArrayArithmetics/ArrayShift.hpp>

#include <array>
#include <iostream>
#include <tuple>

namespace {

constexpr auto digitBitSize = sizeof(BigUnsigned::digit_type) * 8;

static auto powerOf10(
    unsigned power
) -> const BigUnsigned& {
    const static auto powers = std::array{
        BigUnsigned(1),
        BigUnsigned(10),
        BigUnsigned(100),
        BigUnsigned(1000),
        BigUnsigned(10000),
        BigUnsigned(100000),
        BigUnsigned(1000000),
        BigUnsigned(10000000),
        BigUnsigned(100000000),
        BigUnsigned(1000000000),
    };

    return powers[power];
}

void printDigits(
    std::span<const BigUnsigned::digit_type> value
) {
    std::cout << "{0x" << value.front();
    for (auto d = 1u; d < size(value); ++d) {
        std::cout << ", 0x" << value[d];
    }
    std::cout << "}\n";
}

auto divide(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> std::pair<BigUnsigned, BigUnsigned> {

    if (!rhs) {
        throw std::runtime_error("Division by zero is not allowed!");
    }

    if (lhs < rhs) {
        return {BigUnsigned(0), lhs};
    }

    const auto rhsTopBit = topBit(rhs.digits_);
    const auto lhsTopBit = topBit(lhs.digits_);

    auto bitDiff = lhsTopBit - rhsTopBit;
    auto divider = rhs << bitDiff;
    auto quotient  = std::vector<BigUnsigned::digit_type>(bitDiff / digitBitSize + 1); 
    auto remainder = lhs.digits_;

    while (remainder >= std::span(rhs.digits_)) {
        const auto newBitDiff = topBit(remainder) - rhsTopBit;

        divider >>= bitDiff - newBitDiff;
        bitDiff = newBitDiff;

        if (std::span(divider.digits_) > remainder) {
            divider >>= 1;
            bitDiff--;
        }

        quotient[bitDiff / digitBitSize] |= 1 << (bitDiff % digitBitSize);
        remainder -= divider.digits_;
        remainder.resize(sizeWithoutLeadingZeroes(remainder));
    }

    quotient.resize(sizeWithoutLeadingZeroes(quotient));

    return {
        BigUnsigned(std::move(quotient)), 
        BigUnsigned(std::move(remainder))
    };
}

}

BigUnsigned::BigUnsigned() : BigUnsigned(0) {}

BigUnsigned::BigUnsigned(BigUnsigned::digit_type digit) : BigUnsigned { digit } { }

BigUnsigned::BigUnsigned(
    std::initializer_list<BigUnsigned::digit_type> digits
) : BigUnsigned(std::vector<BigUnsigned::digit_type>(digits.begin(), digits.end())) {}

BigUnsigned::BigUnsigned(std::vector<BigUnsigned::digit_type> digits) : digits_(std::move(digits)) {
    digits_.resize(sizeWithoutLeadingZeroes(digits_));
}

BigUnsigned::BigUnsigned(std::string string) : BigUnsigned(0) {
    const auto maxDivisorPowerOf10 = std::size_t(9);
    
    auto digit = BigUnsigned();
    for (auto s = 0u; s < size(string); s += maxDivisorPowerOf10) {
        const auto stringDigit = std::string(
            string.begin() + s,
            string.begin() + std::min(s + maxDivisorPowerOf10, string.size())
        );

        digit[0] = std::atol(stringDigit.data());

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

    const auto maxDivisorPowerOf10 = std::size_t(9);
    const auto div = powerOf10(maxDivisorPowerOf10);

    auto result = std::string();
    result.reserve(mag() * maxDivisorPowerOf10);

    for (auto quot = *this, mod = BigUnsigned(); (bool) quot;) {
        const auto notLastDivision = quot > div;

        std::tie(quot, mod) = divide(quot, div);
        auto string = std::to_string(mod[0]);
        if (size(string) < maxDivisorPowerOf10 && notLastDivision) {
            const auto zeroes = std::string(maxDivisorPowerOf10 - size(string), '0');
            string.insert(string.begin(), zeroes.begin(), zeroes.end());
        }
        result.insert(result.begin(), string.begin(), string.end());
    }
    
    return result;
}

int BigUnsigned::mag() const {
    return size(digits_);
}

BigUnsigned::digit_type  BigUnsigned::operator[](std::size_t index) const { return digits_[index]; }
BigUnsigned::digit_type& BigUnsigned::operator[](std::size_t index)       { return digits_[index]; }

auto operator<<=(
    BigUnsigned& lhs,
    BigUnsigned::digit_type rhs
) -> BigUnsigned& {

    const auto wholeDigitsShift = rhs / digitBitSize;
    const auto bitShift = rhs % digitBitSize;
    const auto carry = (bool) (lhs.digits_.back() >> (digitBitSize - bitShift));

    if (wholeDigitsShift + carry + lhs.digits_.size() > lhs.digits_.capacity()) {
        auto shifted = std::vector<BigUnsigned::digit_type>(size(lhs.digits_) + wholeDigitsShift + carry);

        const auto& lhsConst = lhs.digits_;
        bignum::leftShift(
            std::span(lhsConst),
            rhs,
            std::span(shifted).subspan(wholeDigitsShift)
        );

        lhs.digits_ = std::move(shifted);

    } else {
        lhs.digits_.resize(size(lhs.digits_) + wholeDigitsShift + carry);

        std::fill(begin(lhs.digits_), begin(lhs.digits_) + wholeDigitsShift, 0);

        const auto& lhsConst = lhs.digits_;
        bignum::leftShift(
            std::span(lhsConst),
            rhs,
            std::span(lhs.digits_).subspan(wholeDigitsShift)
        );
    }

    lhs.digits_.resize(sizeWithoutLeadingZeroes(lhs.digits_));

    return lhs;
}

auto operator>>=(
    BigUnsigned& lhs,
    BigUnsigned::digit_type rhs
) -> BigUnsigned& {
    const auto size = bignum::rightShift(
        std::span(lhs.digits_), 
        rhs
    );

    lhs.digits_.resize(lhs.digits_.size() - size);
    if (lhs.digits_.size() > 1u && lhs.digits_.back() == 0u) {
        lhs.digits_.pop_back();
    }

    return lhs;
}

auto operator+=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {

    if (lhs.mag() < rhs.mag()) {
        lhs.digits_.resize(rhs.mag());
    } else if (lhs.mag() == rhs.mag()) {
        const auto lhsTop = lhs[lhs.mag() - 1];
        const auto rhsTop = rhs[rhs.mag() - 1];

        if (lhsTop + rhsTop + 1 < lhsTop) {
            lhs.digits_.reserve(lhs.mag() + 1);
        }
    }

    if (bignum::add(
        std::span(lhs.digits_),
        std::span(rhs.digits_)
    )) {
        lhs.digits_.push_back(1);
    }

    return lhs;
}

auto operator-=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {

    if (lhs < rhs) {
        auto tmp = rhs;
        const auto& constLhs = lhs;
        bignum::subtract(
            std::span(tmp.digits_),
            std::span(constLhs.digits_)
        );
        std::swap(lhs.digits_, tmp.digits_);
    } else {
        bignum::subtract(
            std::span(lhs.digits_),
            std::span(rhs.digits_)
        );
    }

    lhs.digits_.resize(sizeWithoutLeadingZeroes(lhs.digits_));

    return lhs;
}

auto operator*=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {
    auto result = std::vector<BigUnsigned::digit_type>(lhs.mag() + rhs.mag());

    const auto& constLhs = lhs;

    bignum::mul(
        std::span(constLhs.digits_),
        std::span(rhs.digits_),
        std::span(result)
    );

    result.resize(sizeWithoutLeadingZeroes(result));

    std::swap(lhs.digits_, result);
    return lhs;
}

auto operator/=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {
    return lhs = divide(lhs, rhs).first;
}

auto operator%=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {
    return lhs = divide(lhs, rhs).second;
}

auto operator<<(
    const BigUnsigned& lhs,
    BigUnsigned::digit_type rhs
) -> BigUnsigned {
    auto tmp = lhs;
    tmp <<= rhs;
    return tmp;
}

auto operator>>(
    const BigUnsigned& lhs,
    BigUnsigned::digit_type rhs
) -> BigUnsigned {
    auto tmp = lhs;
    tmp >>= rhs;
    return tmp;
}

auto operator+(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned {
    auto result = lhs;
    result += rhs;
    return result;
}

auto operator-(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned {
    auto result = lhs;
    result -= rhs;
    return result;
}

auto operator*(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned {
    auto result = lhs;
    result *= rhs;
    return result;
}

auto operator/(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned {
    auto result = lhs;
    result /= rhs;
    return result;
}

auto operator%(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned {
    auto result = lhs;
    result %= rhs;
    return result;
}