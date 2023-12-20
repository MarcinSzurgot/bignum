#pragma once

#include <bignum/Utils.hpp>

#include <cinttypes>
#include <concepts>
#include <string_view>
#include <vector>

namespace bignum {

struct BigUnsigned {
    using NativeDigit = std::uint64_t;

    static constexpr auto NativeDigitBitSize = sizeof(NativeDigit) * 8;

             BigUnsigned();
    explicit BigUnsigned(std::initializer_list<NativeDigit> digits);             
    explicit BigUnsigned(std::string string);

    template<typename LessThanNative>
    requires std::unsigned_integral<std::remove_const_t<LessThanNative>>
             && (sizeof(std::remove_const_t<LessThanNative>) < sizeof(NativeDigit))
    explicit BigUnsigned(
        std::span<LessThanNative> digits
    ) : digits_(nativeDigits(digits)) {
    }

    template<typename GreaterThanNative>
    requires std::unsigned_integral<std::remove_const_t<GreaterThanNative>>
             && (sizeof(std::remove_const_t<GreaterThanNative>) > sizeof(NativeDigit))
    explicit BigUnsigned(
        std::span<GreaterThanNative> digits
    ) : BigUnsigned(nativeDigits(digits)) {
    }

    template<std::unsigned_integral NotNativeDigit>
    requires (sizeof(NotNativeDigit) != sizeof(NativeDigit))
    explicit BigUnsigned(
        const std::vector<NotNativeDigit>& digits
    ) : BigUnsigned(std::span(digits)) {

    }

    explicit BigUnsigned(const std::vector<NativeDigit>&  digits);
    explicit BigUnsigned(      std::vector<NativeDigit>&& digits);

    template<std::unsigned_integral Digit>
    explicit BigUnsigned(Digit digit);

    template<std::unsigned_integral LessThanNative>
    requires (sizeof(LessThanNative) < sizeof(NativeDigit))
    explicit BigUnsigned(LessThanNative digit) : BigUnsigned(NativeDigit(digit)) {
    }

    template<std::unsigned_integral GreaterThanNative>
    requires (sizeof(GreaterThanNative) > sizeof(NativeDigit))
    explicit BigUnsigned(
        GreaterThanNative digit
    ) : BigUnsigned(std::span(
            std::addressof(digit), 
            sizeof(GreaterThanNative) / sizeof(NativeDigit)
    )) {
    }

    explicit operator bool() const;
    explicit operator std::string() const;

    template<std::unsigned_integral U = NativeDigit>
    std::span<U> digits() { 
        auto first = reinterpret_cast<U*>(begin(digits_).base());
        auto last  = reinterpret_cast<U*>(end(digits_).base());
        return {first, last};
    }

    template<std::unsigned_integral U = NativeDigit>
    std::span<const U> digits() const {
        auto first = reinterpret_cast<const U*>(begin(digits_).base());
        auto last  = reinterpret_cast<const U*>(end(digits_).base());
        return {first, last};
    }

    template<std::unsigned_integral U = NativeDigit>
    std::span<const U> cdigits() const { return digits(); }

    void swap(std::vector<NativeDigit>& digits);

    template<typename Functor>
    requires CallableWithVectorIntRef<Functor, NativeDigit>
    void operate(Functor&& functor) {
        functor(digits_);
        trim();
    }

private:

    void trim();

    std::vector<NativeDigit> digits_;

    template<typename LessThanNative>
    requires std::unsigned_integral<std::remove_const_t<LessThanNative>>
    && (sizeof(LessThanNative) < sizeof(NativeDigit))
    static std::vector<NativeDigit> nativeDigits(std::span<LessThanNative> digits) {
        const auto nativeWholeDigitsCount = size(digits) / sizeof(NativeDigit);
        const auto remainder = size(digits) - nativeWholeDigitsCount * sizeof(NativeDigit);
        auto nativeSize = nativeWholeDigitsCount + (remainder > 0u);
        nativeSize = nativeSize ? nativeSize : 1u;

        auto nativeDigits = std::vector<NativeDigit>(nativeSize);
        std::copy(
            begin(digits),
            end(digits),
            reinterpret_cast<std::remove_const_t<LessThanNative>*>(begin(nativeDigits).base())
        );

        return nativeDigits;
    }

    template<typename GreaterThanNative>
    requires std::unsigned_integral<std::remove_const_t<GreaterThanNative>>
    && (sizeof(std::remove_const_t<GreaterThanNative>) > sizeof(NativeDigit))
    static std::vector<NativeDigit> nativeDigits(std::span<GreaterThanNative> digits) {

        auto nativeDigits = std::vector<NativeDigit>(
            size(digits) * ((sizeof(std::remove_const_t<GreaterThanNative>) / sizeof(NativeDigit)))
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