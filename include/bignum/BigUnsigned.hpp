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

template<typename T>
auto mul(T lhs, T rhs) -> std::pair<T, T> {
    constexpr auto halfBitSize = sizeof(T) * 8 / 2;
    constexpr auto halfMask = (1 << halfBitSize) - 1;

    const T lowerLeft  =  lhs                 & halfMask;
    const T lowerRight =  rhs                 & halfMask;
    const T upperLeft  = (lhs >> halfBitSize) & halfMask;
    const T upperRight = (rhs >> halfBitSize) & halfMask;

    const T lowerResult      = lowerLeft * lowerRight;
    const T middleResultPart = lowerLeft * upperRight;
    const T middleResult     = middleResultPart + lowerRight * upperLeft;
    const T upperResult      = upperLeft * upperRight + (
        middleResult < middleResultPart 
        ? (T(1) << halfBitSize)
        :  T(0)
    );

    const T finalLowerResult = lowerResult + (middleResult << halfBitSize);
    const T finalUpperResult = upperResult + (middleResult >> halfBitSize) + (
        finalLowerResult < lowerResult 
        ? T(1)
        : T(0)
    );

    return {finalLowerResult, finalUpperResult};
}