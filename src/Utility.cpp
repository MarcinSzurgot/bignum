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

    const auto msdHighestBit = highestBitNumber(value.msd()).value();
    return (value.magnitude() - 1) * bitsPerDigitType + msdHighestBit;
}

}
