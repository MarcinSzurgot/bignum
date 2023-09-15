#include <bignum/BigUnsigned.hpp>

#include <iostream>

namespace {

void trim(std::vector<std::uint32_t>& digits) {
    for (auto d = size(digits); d > 0u; --d) {
        if (digits[d - 1]) {
            digits.resize(d);
            return;
        }
    }
    digits.resize(1);
}

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

    trim(bigger);
}

}

BigUnsigned::BigUnsigned() : BigUnsigned(0) {}

BigUnsigned::BigUnsigned(std::uint32_t digit) : BigUnsigned { digit } { }

BigUnsigned::BigUnsigned(std::initializer_list<std::uint32_t> digits) : digits_(digits) {
    trim(digits_);
}

BigUnsigned::operator bool() const {
    return size(digits_) > 1u || digits_[0] > 0u;
}

int BigUnsigned::mag() const {
    return size(digits_);
}

std::uint32_t  BigUnsigned::operator[](std::size_t index) const { return digits_[index]; }
std::uint32_t& BigUnsigned::operator[](std::size_t index)       { return digits_[index]; }

auto operator==(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> bool {
    if (lhs.mag() != rhs.mag()) {
        return false;
    }

    for (auto d = 0u; d < lhs.mag(); ++d) {
        if (lhs[d] != rhs[d]) {
            return false;
        }
    }

    return true;
}

auto operator<(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> bool {
    if (lhs.mag() != rhs.mag()) {
        return lhs.mag() < rhs.mag();
    }

    for (auto d = lhs.mag(); d > 0u; --d) {
        if (lhs[d - 1] != rhs[d - 1]) {
            return lhs[d - 1] < rhs[d - 1];
        }
    }

    return false;
}

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

    if (bitShift) {
        auto carry = 0UL;

        for (auto& digit : lhs.digits_) {
            const auto newCarry = digit >> (32 - bitShift);
            digit <<= bitShift;
            digit |= carry;
            carry = newCarry;
        }

        if (carry) {
            lhs.digits_.push_back(carry);
        }
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

    trim(result);

    std::swap(lhs.digits_, result);
    return lhs;
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