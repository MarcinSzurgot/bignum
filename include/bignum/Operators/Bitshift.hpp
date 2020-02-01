#pragma once

#include "../Unsigned.hpp"
#include "../Utility.hpp"

#include <iostream>
#include <type_traits>

namespace bignum
{

template<typename DigitType, typename Integer>
Unsigned<DigitType> operator<<(const Unsigned<DigitType>& value, Integer offset)
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
            if (!result.msd() && result.magnitude() > 1)
            {
                result.digits_.pop_back();
            }
            return result;
        }
    }
}

template<typename DigitType, typename Integer>
Unsigned<DigitType> operator>>(const Unsigned<DigitType>& value, Integer offset)
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
                    result[d] |= DigitType(value[d + digitOffset + 0] >> bitOffset);
                    if (d + digitOffset + 1 < value.magnitude())
                    {
                        result[d] |= DigitType(value[d + digitOffset + 1] << (bitsize - bitOffset));
                    }
                }
                if (!result.msd() && result.magnitude() > 1)
                {
                    result.digits_.pop_back();
                }
            }
            return result;
        }
    }
}

template<typename DigitType_, typename Integer>
Unsigned<DigitType_>& operator<<(Unsigned<DigitType_>& value, Integer offset)
{
    return value = value << offset;
}

template<typename DigitType_, typename Integer>
Unsigned<DigitType_>& operator>>(Unsigned<DigitType_>& value, Integer offset)
{
    return value = value >> offset;
}


}
