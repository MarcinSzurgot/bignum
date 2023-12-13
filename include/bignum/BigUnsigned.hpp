#pragma once

#include <bignum/ArrayLogic/ArrayLogic.hpp>

#include <cinttypes>
#include <concepts>
#include <string_view>
#include <vector>

struct BigUnsigned {
    using digit_type = std::uint32_t;

             BigUnsigned();
    explicit BigUnsigned(digit_type digit);
    explicit BigUnsigned(std::initializer_list<digit_type> digits);
    explicit BigUnsigned(std::vector<digit_type> digits);
    explicit BigUnsigned(std::string string);

    explicit operator bool() const;
    explicit operator std::string() const;

    template<std::unsigned_integral U>
    std::span<U> digits() { 
        auto first = reinterpret_cast<U*>(digits_.begin().base());
        auto last  = reinterpret_cast<U*>(digits_.end().base());
        return {first, last};
    }

    template<std::unsigned_integral U>
    std::span<const U> digits() const { 
        auto first = reinterpret_cast<const U*>(digits_.begin().base());
        auto last  = reinterpret_cast<const U*>(digits_.end().base());
        return {first, last};
    }

    int mag() const { return size(digits_); }

    friend auto operator==(
        const BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> bool { 
        using namespace bignum;
        return std::span(lhs.digits_) == std::span(rhs.digits_); 
    }

    friend auto operator!=(
        const BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> bool { return !(lhs == rhs); }

    friend auto operator<(
        const BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> bool {  
        using namespace bignum;
        return std::span(lhs.digits_) < std::span(rhs.digits_); 
    }

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

private:
    std::vector<digit_type> digits_;
};