#pragma once

#include <cinttypes>
#include <vector>

struct BigUnsigned {
    BigUnsigned();
    BigUnsigned(std::uint32_t digit);
    BigUnsigned(std::initializer_list<std::uint32_t> digits);
    BigUnsigned(std::vector<std::uint32_t> digits);

    operator bool() const;

    int mag() const;

    std::uint32_t  operator[](std::size_t index) const;
    std::uint32_t& operator[](std::size_t index);

    friend auto operator==(
        const BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> bool;

    friend auto operator<(
        const BigUnsigned& lhs,
        const BigUnsigned& rhs
    ) -> bool;

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

private:
    std::vector<std::uint32_t> digits_;
};