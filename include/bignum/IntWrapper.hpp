#pragma once

#include <cstdint>
#include <iosfwd>
#include <type_traits>

namespace bignum
{

template<typename IntType>
struct IntWrapper
{
    static_assert(std::is_integral_v<IntType>);

    using int_type = IntType;
    using size_type = std::size_t;

    explicit constexpr IntWrapper() : value() {}
    explicit constexpr IntWrapper(int_type value) : value(value) {}
    explicit constexpr IntWrapper(const IntWrapper& other) : value(value.value) {}

    constexpr IntWrapper& operator=(IntWrapper other) { return value = other.value, *this; }

    template<typename CastType>
    constexpr explicit operator CastType() const
    {
        return static_cast<CastType>(value);
    }

    int_type value;

    constexpr friend IntWrapper operator+(const IntWrapper& value) noexcept { return  value; }
    constexpr friend IntWrapper operator-(const IntWrapper& value) noexcept { return -value.value; }

    constexpr friend IntWrapper& operator++(IntWrapper& value) noexcept { return ++value.value, value; }
    constexpr friend IntWrapper& operator--(IntWrapper& value) noexcept { return --value.value, value; }

    constexpr friend IntWrapper operator++(IntWrapper& value, int) noexcept { return value.value++, value; }
    constexpr friend IntWrapper operator--(IntWrapper& value, int) noexcept { return value.value--, value; }

    constexpr friend IntWrapper operator+(const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value + rhs.value; }
    constexpr friend IntWrapper operator-(const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value - rhs.value; }
    constexpr friend IntWrapper operator*(const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value * rhs.value; }
    constexpr friend IntWrapper operator/(const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value / rhs.value; }
    constexpr friend IntWrapper operator%(const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value % rhs.value; }

    constexpr friend IntWrapper operator+=(IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value += rhs.value; }
    constexpr friend IntWrapper operator-=(IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value -= rhs.value; }
    constexpr friend IntWrapper operator*=(IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value *= rhs.value; }
    constexpr friend IntWrapper operator/=(IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value /= rhs.value; }
    constexpr friend IntWrapper operator%=(IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value %= rhs.value; }

    constexpr friend IntWrapper operator~(const IntWrapper& value) noexcept { return ~value.value; }

    constexpr friend IntWrapper operator&(const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value & rhs.value; }
    constexpr friend IntWrapper operator|(const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value | rhs.value; }
    constexpr friend IntWrapper operator^(const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value ^ rhs.value; }

    constexpr friend IntWrapper operator&=(const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value &= rhs.value; }
    constexpr friend IntWrapper operator|=(const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value |= rhs.value; }
    constexpr friend IntWrapper operator^=(const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value ^= rhs.value; }

    constexpr friend IntWrapper operator<<(const IntWrapper& value, size_type offset) noexcept { return value.value << offset; }
    constexpr friend IntWrapper operator>>(const IntWrapper& value, size_type offset) noexcept { return value.value >> offset; }

    constexpr friend IntWrapper operator<<=(const IntWrapper& value, size_type offset) noexcept { return value.value <<= offset; }
    constexpr friend IntWrapper operator>>=(const IntWrapper& value, size_type offset) noexcept { return value.value >>= offset; }

    constexpr friend bool operator! (const IntWrapper& value) noexcept { return !value.value; }
    constexpr friend bool operator&&(const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value && rhs.value; }
    constexpr friend bool operator||(const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value || rhs.value; }

    constexpr friend bool operator==(const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value == rhs.value; }
    constexpr friend bool operator!=(const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value != rhs.value; }
    constexpr friend bool operator< (const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value <  rhs.value; }
    constexpr friend bool operator> (const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value >  rhs.value; }
    constexpr friend bool operator<=(const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value <= rhs.value; }
    constexpr friend bool operator>=(const IntWrapper& lhs, const IntWrapper& rhs) noexcept { return lhs.value >= rhs.value; }

    friend std::ostream& operator<<(std::ostream& os, const IntWrapper& value) { return os << value.value; }
    friend std::istream& operator>>(std::istream& is,       IntWrapper& value) { return is >> value.value; }
};

}
