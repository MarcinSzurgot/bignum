#pragma once

#include <type_traits>
#include <utility>

namespace bignum
{

template<typename UnsignedType>
std::pair<UnsignedType, UnsignedType> addWithCarry(UnsignedType lhs, UnsignedType rhs, UnsignedType carry)
{
    static_assert(std::is_unsigned_v<UnsignedType>);
    
    return {lhs + rhs + carry, UnsignedType()};
}

}
