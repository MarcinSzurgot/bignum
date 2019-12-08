#pragma once

#include <cstdint>

namespace bignum
{
    using digit_type = std::uint8_t;

    constexpr auto bitsPerDigitType = sizeof(digit_type) * 8u;
}
