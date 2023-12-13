#pragma once

#include <concepts>
#include <tuple>

namespace bignum {

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

}