#pragma once

#include <cinttypes>
#include <concepts>
#include <compare>
#include <iterator>

namespace bignum {

template<std::unsigned_integral U> 
struct Bits {

    template<std::unsigned_integral U2> struct BitsPointer;
    template<std::unsigned_integral U2> struct BitsReference;

    using value_type      = U;
    using       pointer   = BitsPointer<      U>;
    using const_pointer   = BitsPointer<const U>;
    using       iterator  =       pointer;
    using const_iterator  = const_pointer;
    using       reference = BitsReference<      U>;
    using const_reference = BitsReference<const U>;
    using       size_type = std::uint8_t;
    using difference_type = std:: int8_t;

    static constexpr auto Size      = value_type(sizeof(value_type) * 8);
    static constexpr auto Mask      = value_type(~value_type());
    static constexpr auto HalfSize  = value_type(Size / 2);
    static constexpr auto HalfMask  = value_type(Mask >> HalfSize);
    static constexpr auto ShiftMask = value_type(Size - 1);

    Bits() : Bits(0u) { }

    Bits(value_type value) : value_(value) { }

    auto size() const -> size_type { return Bits<value_type>::Size; }

    auto empty() const -> bool { return true; }

    operator bool() const { return static_cast<bool>(value_); }

    auto begin() -> iterator {
        return {std::addressof(value_), 0};
    }

    auto begin() const -> const_iterator {
        return {std::addressof(value_), 0};
    }

    auto end() -> iterator {
        return {
            std::addressof(value_), 
            Bits<U>::Size
        };
    }

    auto end() const -> const_iterator {
        return {
            std::addressof(value_), 
            Bits<U>::Size
        };
    }

    auto value()       ->       reference { return value_; }
    auto value() const -> const_reference { return value_; }

    auto operator[](int index)       ->       reference { return begin()[index]; }
    auto operator[](int index) const -> const_reference { return begin()[index]; }

    friend auto begin(
        Bits& bits
    ) -> iterator { return bits.begin(); }


    friend auto begin(
        const Bits& bits
    ) -> const_iterator { return bits.begin(); }

    friend auto end(
        Bits& bits
    ) -> iterator { return bits.end(); }

    friend auto end(
        const Bits& bits
    ) -> const_iterator { return bits.end(); }

private:
    value_type value_;
};

template<std::unsigned_integral U >
template<std::unsigned_integral U2>
struct Bits<U>::BitsPointer {

    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = bool;
    using difference_type   = std::int8_t;
    using pointer           = BitsPointer<U2>;
    using reference         = BitsReference<U2>;

    BitsPointer() : BitsPointer(nullptr, Bits<value_type>::Size) {

    }

    BitsPointer(
        U2* base, 
        std::uint8_t position
    ) : 
    base_(base), 
    position_(position) {
            
    }

    friend auto operator!=(
        const BitsPointer& lhs,
        const BitsPointer& rhs
    ) -> bool {
        return lhs.position_ != rhs.position_;
    }

    friend auto operator==(
        const BitsPointer& lhs,
        const BitsPointer& rhs
    ) -> bool {
        return !(lhs != rhs);
    }

    auto operator++() -> pointer& {
        ++position_;
        return *this;
    }

    auto operator--() -> pointer& {
        --position_;
        return *this;
    }

    auto operator++(int) -> pointer {
        auto tmp = *this;
        ++position_;
        return tmp;
    }

    auto operator--(int) -> pointer {
        auto tmp = *this;
        --position_;
        return tmp;
    }

    auto operator*() const -> reference {
        return {base_, position_};
    }

    auto operator->() const -> reference {
        return {base_, position_};
    }

    auto operator[](difference_type bit) -> reference {
        return {base_, difference_type(position_ + bit)};
    }
    
private:
    U2* base_;
    size_type position_;
};

template<std::unsigned_integral U>
template<std::unsigned_integral U2>
struct Bits<U>::BitsReference {
    using value_type = U2;
    using pointer = U2*;

    BitsReference(
        pointer base, 
        std::uint8_t position
    ) : base_(base), position_(position) {

    }

    operator bool() const {
        return (*base_ >> position_) & value_type(1);
    }

    BitsReference& operator=(bool b) {
        if (b) {
            *base_ |=   value_type(1) << position_;
        } else {
            *base_ &= ~(value_type(1) << position_);
        }

        return *this;
    }

private:
    pointer base_;
    std::uint8_t position_;
};

}