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

namespace bignum {

template<>
BigUnsigned::BigUnsigned<BigUnsigned::digit_type>(
    digit_type digit
) : BigUnsigned { digit } { 
}

namespace {

constexpr auto digitBitSize = sizeof(bignum::BigUnsigned::digit_type) * 8;

static auto powerOf10(
    unsigned power
) -> const bignum::BigUnsigned& {
    const static auto powers = std::array{
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

auto divide(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> std::pair<BigUnsigned, BigUnsigned> {
    if (lhs < rhs) {
        return {BigUnsigned(), lhs};
    }

    const auto rhsTopBit = topBit(rhs.digits());
    const auto lhsTopBit = topBit(lhs.digits());

    auto bitDiff = lhsTopBit - rhsTopBit;
    auto divider = rhs << bitDiff;
    auto quotient  = std::vector<BigUnsigned::digit_type>(bitDiff / digitBitSize + 1); 
    auto remainder = std::vector<BigUnsigned::digit_type>(size(lhs.digits()));

    const auto [quotSize, remSize] = bignum::divide(
        lhs.digits(),
        rhs.digits(),
        std::span(quotient),
        std::span(remainder)
    );

    quotient.resize(quotSize);
    remainder.resize(remSize);

    return {
        BigUnsigned(std::move(quotient)), 
        BigUnsigned(std::move(remainder))
    };
}

}

BigUnsigned::BigUnsigned() : BigUnsigned(digit_type()) {}

BigUnsigned::BigUnsigned(
    std::initializer_list<BigUnsigned::digit_type> digits
) : BigUnsigned(std::vector(digits.begin(), digits.end())) {}

BigUnsigned::BigUnsigned(
    const std::vector<digit_type>& digits
) : BigUnsigned(std::vector(digits)) {
}

BigUnsigned::BigUnsigned(std::vector<digit_type>&& digits) : digits_(std::move(digits)) {
    digits_.resize(sizeWithoutLeadingZeroes(std::span(digits_)));
}

BigUnsigned::BigUnsigned(std::string string) : BigUnsigned(digit_type()) {
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

auto operator<<=(
    BigUnsigned& lhs,
    BigUnsigned::digit_type rhs
) -> BigUnsigned& {

    const auto wholeDigitsShift = rhs / digitBitSize;
    const auto bitShift = rhs % digitBitSize;
    const auto carry = (bool) (lhs.digits().back() >> (digitBitSize - bitShift));

    if (wholeDigitsShift + carry + size(lhs.digits()) > lhs.digits_.capacity()) {
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
        lhs.digits(), 
        rhs
    );

    lhs.digits_.resize(lhs.digits().size() - size);
    if (lhs.digits_.size() > 1u && lhs.digits_.back() == 0u) {
        lhs.digits_.pop_back();
    }

    return lhs;
}

auto operator+=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {
    const auto lhsSize = size(lhs.digits());
    const auto rhsSize = size(rhs.digits());

    if (lhsSize < rhsSize) {
        lhs.digits_.resize(rhsSize);
    } else if (lhsSize == rhsSize) {
        const auto lhsTop = lhs.digits()[lhsSize - 1];
        const auto rhsTop = rhs.digits()[rhsSize - 1];

        if (lhsTop + rhsTop + 1 < lhsTop) {
            lhs.digits_.reserve(lhsSize + 1);
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
    using MultiplicationType = std::uint32_t;
    using MultiplicationContainingType = std::uint64_t;

    static_assert(sizeof(MultiplicationType) <= sizeof(BigUnsigned::digit_type));
    static_assert(sizeof(MultiplicationContainingType) == sizeof(MultiplicationType) * 2);

    auto result = std::vector<BigUnsigned::digit_type>(size(lhs.digits()) + size(rhs.digits()));
    auto resultSpan = std::span(
        reinterpret_cast<std::uint32_t*>(result.begin().base()),
        reinterpret_cast<std::uint32_t*>(result.end().base())
    );

    const auto& constLhs = lhs;

    bignum::mul<std::uint64_t>(
        constLhs.digits<std::uint32_t>(),
        rhs.digits<std::uint32_t>(),
        resultSpan
    );

    result.resize(sizeWithoutLeadingZeroes(std::span(result)));

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

}