#include <bignum/BigUnsigned.hpp>

#include <bignum/Digits.hpp>
#include <bignum/Operations.hpp>

#include <array>
#include <iostream>

namespace {

void subtract(
          std::vector<std::uint32_t>& bigger,
    const std::vector<std::uint32_t>& smaller
) {
    auto carry = false;
    for (auto d = 0u; d < size(smaller); ++d) {
        const auto old = bigger[d];
        bigger[d] -= smaller[d];
        bigger[d] -= carry;

        if (carry) {
            carry = bigger[d] >= old;
        } else {
            carry = bigger[d] >  old;
        }
    }

    for (auto d = size(smaller); d < size(bigger) && carry; ++d) {
        bigger[d] -= carry;
        carry = !(bigger[d] + 1);
    }

    bigger.resize(sizeWithoutLeadingZeroes(bigger));
}

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

auto divide(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> std::pair<BigUnsigned, BigUnsigned> {

    if (!rhs) {
        throw std::runtime_error("Division by zero is not allowed!");
    }

    if (lhs < rhs) {
        return {BigUnsigned(0), rhs};
    }

    const auto rhsTopBit = topBit(rhs.digits_);
    const auto lhsTopBit = topBit(lhs.digits_);

    auto bitDiff = lhsTopBit - rhsTopBit;
    auto divider = rhs << bitDiff;
    auto quotient  = std::vector<std::uint32_t>(bitDiff / 32 + 1);
    auto remainder = lhs.digits_;

    while (remainder >= std::span(rhs.digits_)) {
        const auto newBitDiff = topBit(remainder) - rhsTopBit;

        divider >>= bitDiff - newBitDiff;
        bitDiff = newBitDiff;

        if (std::span(divider.digits_) > remainder) {
            divider >>= 1;
            bitDiff--;
        }

        quotient[bitDiff / 32] |= 1 << (bitDiff % 32);
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

BigUnsigned::BigUnsigned(std::uint32_t digit) : BigUnsigned { digit } { }

BigUnsigned::BigUnsigned(
    std::initializer_list<std::uint32_t> digits
) : BigUnsigned(std::vector<std::uint32_t>(digits.begin(), digits.end())) {}

BigUnsigned::BigUnsigned(std::vector<std::uint32_t> digits) : digits_(std::move(digits)) {
    digits_.resize(sizeWithoutLeadingZeroes(digits_));
}

BigUnsigned::BigUnsigned(std::string string) {
    const auto maxDivisorPowerOf10 = std::size_t(9);

    auto result = BigUnsigned();
    for (auto s = 0u; s < size(string); s += maxDivisorPowerOf10) {
        const auto digits = std::string(
            string.begin() + s,
            string.begin() + std::min(s + maxDivisorPowerOf10, string.size())
        );

        result += BigUnsigned(std::atol(digits.data()));
        if (s + size(digits) < size(string)) {
            result *= powerOf10(std::min(maxDivisorPowerOf10, size(string) - (s + size(digits))));
        }
    }
    *this = result;
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

    for (auto tmp = *this; (bool) tmp; tmp /= div) {
        const auto mod = tmp % div;
        auto string = std::to_string(mod[0]);
        if (size(string) < maxDivisorPowerOf10 && tmp > div) {
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

std::uint32_t  BigUnsigned::operator[](std::size_t index) const { return digits_[index]; }
std::uint32_t& BigUnsigned::operator[](std::size_t index)       { return digits_[index]; }

auto operator<<=(
    BigUnsigned& lhs,
    std::uint32_t rhs
) -> BigUnsigned& {
    if (!lhs) {
        return lhs;
    }

    const auto wholeDigitsShift = rhs / 32;
    const auto bitShift = rhs % 32;

    lhs.digits_.insert(lhs.digits_.begin(), wholeDigitsShift, 0);

    if (!bitShift) {
        return lhs;
    }

    auto carry = std::uint32_t();

    for (auto& digit : lhs.digits_) {
        const auto newCarry = digit >> (32 - bitShift);
        digit <<= bitShift;
        digit |= carry;
        carry = newCarry;
    }

    if (carry) {
        lhs.digits_.push_back(carry);
    }

    return lhs;
}

auto operator>>=(
    BigUnsigned& lhs,
    std::uint32_t rhs
) -> BigUnsigned& {
    if (!lhs || !rhs) {
        return lhs;
    }

    const auto wholeDigitShift = rhs / 32;
    const auto bitShift = rhs % 32;

    if (wholeDigitShift >= lhs.mag()) {
        lhs.digits_.resize(1);
        lhs.digits_[0] = 0;
        return lhs;
    }

    if (bitShift) {
        lhs[0] = lhs[wholeDigitShift] >> bitShift;
        for (auto d = wholeDigitShift + 1; d < lhs.digits_.size(); ++d) {
            lhs[d - wholeDigitShift - 1] |= lhs[d] << (32 - bitShift);
            lhs[d - wholeDigitShift    ]  = lhs[d] >>       bitShift;
        }
    } else {
        for (auto d = wholeDigitShift; d < lhs.digits_.size(); ++d) {
            lhs[d - wholeDigitShift] = lhs[d];
        }
    }

    lhs.digits_.resize(lhs.digits_.size() - wholeDigitShift);
    if (lhs.digits_.back() == 0u) {
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
    }

    const auto common = std::min(lhs.mag(), rhs.mag());

    auto carry = false;
    for (auto d = 0u; d < common; ++d) {
        const auto old = lhs[d];
        lhs[d] += rhs[d];
        lhs[d] += carry;

        if (carry) {
            carry = lhs[d] <= old;
        } else {
            carry = lhs[d] <  old;
        }
    }

    for (auto d = common; d < lhs.mag() && carry; ++d) {
        lhs[d] += carry;
        carry = !lhs[d];
    }

    if (carry) {
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
        subtract(tmp.digits_, lhs.digits_);
        std::swap(lhs.digits_, tmp.digits_);
    } else {
        subtract(lhs.digits_, rhs.digits_);
    }

    return lhs;
}

auto operator*=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {
    auto result = std::vector<std::uint32_t>(lhs.mag() + rhs.mag());

    for (auto l = 0u; l < lhs.mag(); ++l) {
        for (auto r = 0u; r < rhs.mag(); ++r) {
            const auto mul = (std::uint64_t) lhs[l] * rhs[r];
            const auto lower = std::uint32_t(mul & 0xFFFFFFFF);
            const auto higher = std::uint32_t(mul >> 32);

            result[l + r] += lower;

            auto carry = result[l + r] < lower;
            result[l + r + 1] += carry;
            result[l + r + 1] += higher;
            
            if (carry) { 
                carry = result[l + r + 1] <= higher;
            } else {
                carry = result[l + r + 1] <  higher;
            }

            for (auto c = l + r + 2; c < result.size() && carry; ++c) {
                result[c] += carry;
                carry = !result[c];
            }

            if (carry) {
                result.push_back(1);
            }
        }
    }

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
    // return lhs = divide(lhs, rhs).second;
    const auto quotient = lhs / rhs;
    lhs -= quotient * rhs;
    return lhs;
}

auto operator<<(
    const BigUnsigned& lhs,
    std::uint32_t rhs
) -> BigUnsigned {
    auto tmp = lhs;
    tmp <<= rhs;
    return tmp;
}

auto operator>>(
    const BigUnsigned& lhs,
    std::uint32_t rhs
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