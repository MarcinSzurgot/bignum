#pragma once

#include <bignum/Ranges/Conversions.hpp>
#include <bignum/Ranges/String.hpp>

#include <cinttypes>
#include <concepts>
#include <string_view>
#include <vector>

namespace bignum {

template<std::unsigned_integral Native = std::uint8_t>
struct BigUnsigned {
           struct Access;
    friend struct Access;

             BigUnsigned() : BigUnsigned(Native()) {}
    explicit BigUnsigned(std::string_view string) : BigUnsigned(fromChars<Native>(string)) { }
    
    explicit BigUnsigned(const std::vector<Native>&  digits) : BigUnsigned(std::vector(digits)) { }
    explicit BigUnsigned(      std::vector<Native>&& digits) : digits_(std::move(digits)) { 
        trim();
    }

    template<std::ranges::input_range Range>
    requires std::unsigned_integral<std::ranges::range_value_t<Range>>
    explicit BigUnsigned(Range&& digits) : BigUnsigned(bignum::vector<Native>(digits)) { }

    template<std::unsigned_integral Digit>
    explicit BigUnsigned(Digit digit) : BigUnsigned(bignum::vector<Native>(digit)) { }

    operator        bool() const { return size(digits_) > 1u || digits_[0] > 0u; }
    operator std::string() const { return bignum::toChars(digits_); }

    Access access() { return {*this}; }

    template<std::unsigned_integral U = Native>
    requires (sizeof(U) <= sizeof(Native))
    std::span<const U> digits() const {
        constexpr auto ratio = sizeof(Native) / sizeof(U);

        auto first = reinterpret_cast<const U*>(digits_.data());
        auto last  = reinterpret_cast<const U*>(digits_.data() + size(digits_) * ratio);
        return {first, last};
    }

private:

    void trim() {
        digits_.resize(sizeWithoutLeadingZeroes(digits_));
    }

    std::vector<Native> digits_;
};

}