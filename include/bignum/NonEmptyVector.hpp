#pragma once

#include "Defines.hpp"

#include <vector>

namespace bignum
{

template<typename DigitType>
struct NonEmptyVector
{
    void clear()
    {
        std::vector<DigitType>::resize(1);
        (*this)[0] = DigitType();
    }

    void pop_back()
    {
        if (size() > 1)
        {
            std::vector<DigitType>::pop_back();
        }
    }

private:
    std::vector<DigitType> vector_;
};

}
