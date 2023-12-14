#include <bignum/BigUnsigned.hpp>

#include <bignum/Operations.hpp>
#include <bignum/ArrayArithmetics/ArrayDiv.hpp>
#include <bignum/ArrayArithmetics/ArraySubAdd.hpp>
#include <bignum/ArrayArithmetics/ArrayMul.hpp>
#include <bignum/ArrayArithmetics/ArrayShift.hpp>
#include <bignum/ArrayLogic/ArrayLogic.hpp>
#include <bignum/LogicOperators.hpp>

#include <array>
#include <iostream>
#include <tuple>

namespace {

constexpr auto digitBitSize = sizeof(bignum::BigUnsigned::digit_type) * 8;

static auto powerOf10(
    unsigned power
) -> const bignum::BigUnsigned& {
    const static auto powers = std::array{
        bignum::BigUnsigned(1),
        bignum::BigUnsigned(10),
        bignum::BigUnsigned(100),
        bignum::BigUnsigned(1000),
        bignum::BigUnsigned(10000),
        bignum::BigUnsigned(100000),
        bignum::BigUnsigned(1000000),
        bignum::BigUnsigned(10000000),
        bignum::BigUnsigned(100000000),
        bignum::BigUnsigned(1000000000),
    };

    return powers[power];
}

void printDigits(
    std::span<const bignum::BigUnsigned::digit_type> value
) {
    std::cout << "{0x" << value.front();
    for (auto d = 1u; d < size(value); ++d) {
        std::cout << ", 0x" << value[d];
    }
    std::cout << "}\n";
}

auto divide(
    const bignum::BigUnsigned& lhs,
    const bignum::BigUnsigned& rhs
) -> std::pair<bignum::BigUnsigned, bignum::BigUnsigned> {

    using namespace bignum;

    if (!rhs) {
        throw std::runtime_error("Division by zero is not allowed!");
    }

    if (lhs < rhs) {
        return {bignum::BigUnsigned(0), lhs};
    }

    const auto rhsTopBit = topBit(rhs.digits<std::uint32_t>());
    const auto lhsTopBit = topBit(lhs.digits<std::uint32_t>());

    auto bitDiff = lhsTopBit - rhsTopBit;
    auto divider = rhs << bitDiff;
    auto quotient  = std::vector<BigUnsigned::digit_type>(bitDiff / digitBitSize + 1); 
    auto remainder = std::vector(begin(lhs.digits<std::uint32_t>()), end(lhs.digits<std::uint32_t>()));

    while (std::span(remainder) >= rhs.digits<std::uint32_t>()) {
        const auto newBitDiff = topBit(std::span(remainder)) - rhsTopBit;

        divider >>= bitDiff - newBitDiff;
        bitDiff = newBitDiff;

        if (divider.digits<std::uint32_t>() > std::span(remainder)) {
            divider >>= 1;
            bitDiff--;
        }

        quotient[bitDiff / digitBitSize] |= 1 << (bitDiff % digitBitSize);
        bignum::subtract(
            std::span(remainder),
            divider.digits<std::uint32_t>()
        );
        remainder.resize(sizeWithoutLeadingZeroes(std::span(remainder)));
    }

    quotient.resize(sizeWithoutLeadingZeroes(std::span(quotient)));

    return {
        bignum::BigUnsigned(std::move(quotient)), 
        bignum::BigUnsigned(std::move(remainder))
    };
}

}

namespace bignum {

BigUnsigned::BigUnsigned() : BigUnsigned(0) {}

BigUnsigned::BigUnsigned(BigUnsigned::digit_type digit) : BigUnsigned { digit } { }

BigUnsigned::BigUnsigned(
    std::initializer_list<BigUnsigned::digit_type> digits
) : BigUnsigned(std::vector<BigUnsigned::digit_type>(digits.begin(), digits.end())) {}

BigUnsigned::BigUnsigned(std::vector<BigUnsigned::digit_type> digits) : digits_(std::move(digits)) {
    digits_.resize(sizeWithoutLeadingZeroes(std::span(digits_)));
}

BigUnsigned::BigUnsigned(std::string string) : BigUnsigned(0) {
    const auto maxDivisorPowerOf10 = std::size_t(9);
    
    auto digit = BigUnsigned();
    for (auto s = 0u; s < size(string); s += maxDivisorPowerOf10) {
        const auto stringDigit = std::string(
            string.begin() + s,
            string.begin() + std::min(s + maxDivisorPowerOf10, string.size())
        );

        digit.digits<std::uint32_t>()[0] = std::atol(stringDigit.data());

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
    result.reserve(size(digits_) * maxDivisorPowerOf10);

    for (auto quot = *this, mod = BigUnsigned(); (bool) quot;) {
        const auto notLastDivision = quot > div;

        std::tie(quot, mod) = ::divide(quot, div);
        auto string = std::to_string(mod.digits<std::uint32_t>()[0]);
        if (size(string) < maxDivisorPowerOf10 && notLastDivision) {
            const auto zeroes = std::string(maxDivisorPowerOf10 - size(string), '0');
            string.insert(string.begin(), zeroes.begin(), zeroes.end());
        }
        result.insert(result.begin(), string.begin(), string.end());
    }
    
    return result;
}

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

    lhs.digits_.resize(sizeWithoutLeadingZeroes(std::span(lhs.digits_)));

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
        const auto lhsTop = lhs.digits<std::uint32_t>()[lhs.mag() - 1];
        const auto rhsTop = rhs.digits<std::uint32_t>()[rhs.mag() - 1];

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

    lhs.digits_.resize(sizeWithoutLeadingZeroes(std::span(lhs.digits_)));

    return lhs;
}

auto operator*=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {
    auto result = std::vector<BigUnsigned::digit_type>(lhs.mag() + rhs.mag());

    const auto& constLhs = lhs;

    bignum::mul<std::uint64_t>(
        std::span(constLhs.digits_),
        std::span(rhs.digits_),
        std::span(result)
    );

    result.resize(sizeWithoutLeadingZeroes(std::span(result)));

    std::swap(lhs.digits_, result);
    return lhs;
}

auto operator/=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {
    return lhs = ::divide(lhs, rhs).first;
}

auto operator%=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {
    return lhs = ::divide(lhs, rhs).second;
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

}