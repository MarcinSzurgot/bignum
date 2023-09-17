#pragma once

#include <bignum/Digits.hpp>

#include <cinttypes>
#include <string_view>
#include <vector>

struct BigUnsigned {
             BigUnsigned();
    explicit BigUnsigned(std::uint32_t digit);
    explicit BigUnsigned(std::initializer_list<std::uint32_t> digits);
    explicit BigUnsigned(std::vector<std::uint32_t> digits);
    explicit BigUnsigned(std::string string);

    explicit operator bool() const;
    explicit operator std::string() const;

    int mag() const;

    std::uint32_t  operator[](std::size_t index) const;
    std::uint32_t& operator[](std::size_t index);

    friend auto operator==(
        const BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> bool { return std::span(lhs.digits_) == std::span(rhs.digits_); }

    friend auto operator!=(
        const BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> bool { return !(lhs == rhs); }

    friend auto operator<(
        const BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> bool {  return std::span(lhs.digits_) < std::span(rhs.digits_); }

    friend auto operator>=(
        const BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> bool { return !(lhs < rhs); }

    friend auto operator>(
        const BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> bool { return rhs < lhs; }

    friend auto operator<=(
        const BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> bool { return !(lhs > rhs); }

    friend auto operator<<=(
        BigUnsigned& lhs,
        std::uint32_t rhs
    ) -> BigUnsigned&;

    friend auto operator>>=(
        BigUnsigned& lhs,
        std::uint32_t rhs
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
        std::uint32_t rhs
    ) -> BigUnsigned;

    friend auto operator>>(
        const BigUnsigned& lhs,
        std::uint32_t rhs
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

// private:

    std::vector<std::uint32_t> digits_;
};