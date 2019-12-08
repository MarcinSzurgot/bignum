#include "Utility.hpp"

#include "Unsigned.hpp"

namespace bignum
{

std::optional<std::size_t> highestBitNumber(const Unsigned& value)
{
    if (!value)
    {
        return std::nullopt;
    }

    const auto msdHighestBit = highestBitNumber(value.msd());
    return value.magnitude() * bitsPerDigitType + msdHighestBit.value();
}

}
