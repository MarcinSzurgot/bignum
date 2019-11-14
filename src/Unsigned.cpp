#include "Unsigned.hpp"

namespace bignum
{

Unsigned::Unsigned()
: Unsigned({0})
{
    
}

Unsigned::Unsigned(std::vector<digit_type> digits)
: digits_(digits)
{

}



}
