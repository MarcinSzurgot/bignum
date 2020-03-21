#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

#include <sstream>

namespace
{

template<typename T>
void consumeType()
{
    // It just helps us to use decltype in a given context.
}

}

TEST(UnsignedAllOperatorsPresentTests, testThatAllOperatorsCompiles)
{
    // given
    auto oneValue     = bignum::Unsigned<unsigned>();
    auto anotherValue = bignum::Unsigned<unsigned>();
    auto stream       = std::stringstream();

    // then

    // assignment
    consumeType<decltype(bignum::operator+=(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator-=(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator*=(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator/=(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator%=(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator|=(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator&=(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator^=(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator<<=(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator>>=(oneValue, anotherValue))>();

    // arithmetic
    consumeType<decltype(bignum::operator+(oneValue))>();
    consumeType<decltype(bignum::operator-(anotherValue))>();
    consumeType<decltype(bignum::operator+(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator-(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator*(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator/(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator%(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator<<(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator>>(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator~(oneValue))>();
    consumeType<decltype(bignum::operator|(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator&(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator^(oneValue, anotherValue))>();

    // logical
    consumeType<decltype(bignum::operator!(oneValue))>();
    consumeType<decltype(bignum::operator||(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator&&(oneValue, anotherValue))>();

    // comparison
    consumeType<decltype(bignum::operator!=(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator==(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator< (oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator> (oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator<=(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator>=(oneValue, anotherValue))>();

    // increment/decrement
    consumeType<decltype(bignum::operator++(oneValue))>();
    consumeType<decltype(bignum::operator--(oneValue))>();
    consumeType<decltype(bignum::operator++(oneValue, 1))>();
    consumeType<decltype(bignum::operator--(oneValue, 1))>();

    // stream
    consumeType<decltype(bignum::operator<<(stream, oneValue))>();
    consumeType<decltype(bignum::operator>>(stream, oneValue))>();

    // cast
    consumeType<decltype(static_cast<bool>(oneValue))>();
}