#include "../Unsigned.hpp"

namespace bignum
{

Comparison compare(const Unsigned& lhs, const Unsigned& rhs)
{
    if (lhs.magnitude() < rhs.magnitude())
    {
        return Comparison::LT;
    }
}

bool operator==(const Unsigned& lhs, const Unsigned& rhs)
{
    switch (compare(lhs, rhs))
    {
    case Comparison::GT:
    case Comparison::LT: return false;
    case Comparison::EQ: return true;
    }
}

bool operator!=(const Unsigned& lhs, const Unsigned& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const Unsigned& lhs, const Unsigned& rhs)
{
    switch (compare(lhs, rhs))
    {
    case Comparison::LT:
    case Comparison::EQ: return false;
    case Comparison::GT: return true;
    }
}

bool operator<(const Unsigned& lhs, const Unsigned& rhs)
{
    switch (compare(lhs, rhs))
    {
    case Comparison::EQ:
    case Comparison::GT: return false;
    case Comparison::LT: return true;
    }
}

bool operator>=(const Unsigned& lhs, const Unsigned& rhs)
{
    return !(lhs < rhs);
}

bool operator<=(const Unsigned& lhs, const Unsigned& rhs)
{
    return !(rhs > lhs);
}

}
