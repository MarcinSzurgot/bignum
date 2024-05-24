#pragma once

#include <cinttypes>
#include <concepts>
#include <compare>
#include <iterator>

namespace bignum {

template<std::unsigned_integral U> 
struct Bits {
    static constexpr auto Size      = U(sizeof(U) * 8);
    static constexpr auto Mask      = U(~U());
    static constexpr auto HalfSize  = U(Size / 2);
    static constexpr auto HalfMask  = U(Mask >> HalfSize);
    static constexpr auto ShiftMask = U(Size - 1);
};

}