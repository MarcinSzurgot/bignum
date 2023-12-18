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
    explicit BigUnsigned(std::initializer_list<digit_type> digits);             
    explicit BigUnsigned(std::string string);

    template<typename LessThanNative>
    requires std::unsigned_integral<std::remove_const_t<LessThanNative>>
             && (sizeof(std::remove_const_t<LessThanNative>) < sizeof(digit_type))
    explicit BigUnsigned(
        std::span<LessThanNative> digits
    ) : digits_(nativeDigits(digits)) {
    }

    template<typename GreaterThanNative>
    requires std::unsigned_integral<std::remove_const_t<GreaterThanNative>>
             && (sizeof(std::remove_const_t<GreaterThanNative>) > sizeof(digit_type))
    explicit BigUnsigned(
        std::span<GreaterThanNative> digits
    ) : BigUnsigned(nativeDigits(digits)) {
    }

    template<std::unsigned_integral NotNativeDigit>
    requires (sizeof(NotNativeDigit) != sizeof(digit_type))
    explicit BigUnsigned(
        const std::vector<NotNativeDigit>& digits
    ) : BigUnsigned(std::span(digits)) {

    }

    explicit BigUnsigned(const std::vector<digit_type>&  digits);
    explicit BigUnsigned(      std::vector<digit_type>&& digits);

    template<std::unsigned_integral Digit>
    explicit BigUnsigned(Digit digit);

    template<std::unsigned_integral LessThanNative>
    requires (sizeof(LessThanNative) < sizeof(digit_type))
    explicit BigUnsigned(LessThanNative digit) : BigUnsigned(digit_type(digit)) {
    }

    template<std::unsigned_integral GreaterThanNative>
    requires (sizeof(GreaterThanNative) > sizeof(digit_type))
    explicit BigUnsigned(
        GreaterThanNative digit
    ) : BigUnsigned(std::span(
            std::addressof(digit), 
            sizeof(GreaterThanNative) / sizeof(digit_type)
    )) {
    }

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

    template<std::unsigned_integral Digit> 
    static std::vector<digit_type> nativeDigits(std::span<Digit> digits);

    template<typename LessThanNative>
    requires std::unsigned_integral<std::remove_const_t<LessThanNative>>
    && (sizeof(LessThanNative) < sizeof(digit_type))
    static std::vector<digit_type> nativeDigits(std::span<LessThanNative> digits) {
        const auto nativeWholeDigitsCount = size(digits) / sizeof(digit_type);
        const auto remainder = size(digits) - nativeWholeDigitsCount * sizeof(digit_type);
        auto nativeSize = nativeWholeDigitsCount + (remainder > 0u);
        nativeSize = nativeSize ? nativeSize : 1u;

        auto nativeDigits = std::vector<digit_type>(nativeSize);
        std::copy(
            begin(digits),
            end(digits),
            reinterpret_cast<const LessThanNative*>(begin(nativeDigits).base())
        );

        return nativeDigits;
    }

    template<typename GreaterThanNative>
    requires std::unsigned_integral<std::remove_const_t<GreaterThanNative>>
    && (sizeof(std::remove_const_t<GreaterThanNative>) > sizeof(digit_type))
    static std::vector<digit_type> nativeDigits(std::span<GreaterThanNative> digits) {

        auto nativeDigits = std::vector<digit_type>(
            size(digits) * ((sizeof(std::remove_const_t<GreaterThanNative>) / sizeof(digit_type)))
        );
        
        std::copy(
            reinterpret_cast<GreaterThanNative*>(begin(digits).base()),
            reinterpret_cast<GreaterThanNative*>(end(digits).base()),
            begin(nativeDigits)
        );

        return nativeDigits;
    }
};

}