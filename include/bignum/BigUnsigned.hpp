#pragma once

#include <bignum/Digits.hpp>

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

    int mag() const;

    digit_type  operator[](std::size_t index) const;
    digit_type& operator[](std::size_t index);

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

    std::vector<digit_type> digits_;
};

template<std::unsigned_integral U>
auto mul(U lhs, U rhs) -> std::pair<U, U> {
    constexpr auto halfBitSize = sizeof(U) * 8 / 2;
    constexpr auto halfMask = (1 << halfBitSize) - 1;

    const U lowerLeft  =  lhs                 & halfMask;
    const U lowerRight =  rhs                 & halfMask;
    const U upperLeft  = (lhs >> halfBitSize) & halfMask;
    const U upperRight = (rhs >> halfBitSize) & halfMask;

    const U lowerResult      = lowerLeft * lowerRight;
    const U middleResultPart = lowerLeft * upperRight;
    const U middleResult     = middleResultPart + lowerRight * upperLeft;
    const U upperResult      = upperLeft * upperRight + (
        middleResult < middleResultPart 
        ? (U(1) << halfBitSize)
        :  U(0)
    );

    const U finalLowerResult = lowerResult + (middleResult << halfBitSize);
    const U finalUpperResult = upperResult + (middleResult >> halfBitSize) + (
        finalLowerResult < lowerResult 
        ? U(1)
        : U(0)
    );

    return {finalLowerResult, finalUpperResult};
}