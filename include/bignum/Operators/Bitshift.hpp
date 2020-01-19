#pragma once

#include "../Unsigned.hpp"
#include "../Utility.hpp"

#include <iostream>

namespace bignum
{

template<typename DigitType>
Unsigned<DigitType> operator<<(const Unsigned<DigitType>& value, typename Unsigned<DigitType>::size_type offset)
{
    if (!value)
    {
        return {};
    }
    else
    {
        const auto bitsize = sizeof(DigitType) * CHAR_BIT;
        const auto digitOffset = offset / bitsize;
        const auto bitOffset = offset % bitsize;
        const auto newSize = value.magnitude() + digitOffset;

        if (!bitOffset)
        {
            auto result = Unsigned(newSize, DigitType());
            for (auto d = 0u; d < value.magnitude(); ++d)
            {
                result[d + digitOffset] = value[d];
            }
            return result;
        }
        else
        {
            auto result = Unsigned(newSize + 1, DigitType());
            for (auto d = 0u; d < value.magnitude(); ++d)
            {
                result[d + digitOffset + 0] |= value[d] << bitOffset;
                result[d + digitOffset + 1] |= value[d] >> (bitsize - bitOffset);
            }
            result.trim();
            return result;
        }
    }
}

template<typename DigitType>
Unsigned<DigitType> operator>>(const Unsigned<DigitType>& value, typename Unsigned<DigitType>::size_type offset)
{
    if (!value)
    {
        return {};
    }
    else
    {
        const auto bitsize = sizeof(DigitType) * CHAR_BIT;
        const auto digitOffset = offset / bitsize;

        if (digitOffset >= value.magnitude())
        {
            return {};
        }
        else
        {
            const auto bitOffset = offset % bitsize;

            auto result = bignum::Unsigned(value.magnitude() - digitOffset, DigitType());
            if (!bitOffset)
            {
                for (auto d = 0u; d < result.magnitude(); ++d)
                {
                    result[d] = value[d + digitOffset];
                }
            }
            else
            {
                for (auto d = 0u; d < result.magnitude(); ++d)
                {
                    result[d] |= value[d + digitOffset + 0] >> bitOffset;
                    result[d] |= value[d + digitOffset + 1] << (bitsize - bitOffset);
                }
                result.trim();
            }
            return result;
        }
    }
}

}
