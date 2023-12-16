#pragma once

#include <bignum/ArrayLogic/ArrayLogic.hpp>

#include <cinttypes>
#include <concepts>
#include <string_view>
#include <vector>

namespace bignum {

struct BigUnsigned {
    using digit_type = std::uint64_t;

             BigUnsigned();
    explicit BigUnsigned(digit_type digit);
    explicit BigUnsigned(std::initializer_list<digit_type> digits);
    explicit BigUnsigned(std::vector<digit_type> digits);
    explicit BigUnsigned(std::string string);

    explicit operator bool() const;
    explicit operator std::string() const;

    template<std::unsigned_integral U = digit_type>
    std::span<U> digits() { 
        auto first = reinterpret_cast<U*>(digits_.begin().base());
        auto last  = reinterpret_cast<U*>(digits_.end().base());
        return {first, last};
    }

    template<std::unsigned_integral U = digit_type>
    std::span<const U> digits() const { 
        auto first = reinterpret_cast<const U*>(digits_.begin().base());
        auto last  = reinterpret_cast<const U*>(digits_.end().base());
        return {first, last};
    }

    int mag() const { return size(digits_); }

    friend auto operator<<=(
        BigUnsigned& lhs,
        digit_type rhs
    ) -> BigUnsigned&;

    friend auto operator>>=(
        BigUnsigned& lhs,
        digit_type rhs
    ) -> BigUnsigned&;

    friend auto operator+=(
              BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> BigUnsigned&;

    friend auto operator-=(
              BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> BigUnsigned&;

    friend auto operator*=(
              BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> BigUnsigned&;

    friend auto operator/=(
              BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> BigUnsigned&;

    friend auto operator%=(
              BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> BigUnsigned&;

    friend auto operator<<(
        const BigUnsigned& lhs,
        digit_type rhs
    ) -> BigUnsigned;

    friend auto operator>>(
        const BigUnsigned& lhs,
        digit_type rhs
    ) -> BigUnsigned;

    friend auto operator+(
        const BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> BigUnsigned;

    friend auto operator-(
        const BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> BigUnsigned;

    friend auto operator*(
        const BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> BigUnsigned;

    friend auto operator/(
        const BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> BigUnsigned;

    friend auto operator%(
        const BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> BigUnsigned;

private:
    std::vector<digit_type> digits_;
};

}