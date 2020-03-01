#pragma once

#include "../Integer.hpp"

namespace bignum
{

template<typename DigitType_, typename IntegerType>
Integer<DigitType_> operator>>(const Integer<DigitType_>& value, IntegerType offset)
{
    return Integer(value.sign(), value.abs() >> offset);
}

template<typename DigitType_, typename IntegerType>
Integer<DigitType_> operator<<(const Integer<DigitType_>& value, IntegerType offset)
{
    return Integer(value.sign(), value.abs() << offset);
}

template<typename DigitType_, typename IntegerType> 
Integer<DigitType_>& operator>>=(Integer<DigitType_>& value, IntegerType offset)
{
    return value = value >> offset;
}

template<typename DigitType_, typename IntegerType>
Integer<DigitType_>& operator<<=(Integer<DigitType_>& value, IntegerType offset)
{
    return value = value << offset;
}

}