#include <bignum/Integer.hpp>

#include <gtest/gtest.h>

#include <sstream>

namespace
{

template<typename T>
void consumeType()
{
    
}

}

TEST(IntegerAllOperatorsPresentTests, testThatAllOperatorsCompiles)
{
    // given
    auto oneValue     = bignum::Integer<unsigned>();
    auto anotherValue = bignum::Integer<unsigned>();
    auto stream       = std::stringstream();

    // then
    // bignum::operator+(oneValue);
    // bignum::operator-(anotherValue);

    consumeType<decltype(bignum::operator+(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator-(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator*(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator/(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator%(oneValue, anotherValue))>();

    // consumeType<decltype(bignum::operator+=(oneValue, anotherValue))>();
    // consumeType<decltype(bignum::operator-=(oneValue, anotherValue))>();
    // consumeType<decltype(bignum::operator*=(oneValue, anotherValue))>();
    // consumeType<decltype(bignum::operator/=(oneValue, anotherValue))>();
    // consumeType<decltype(bignum::operator%=(oneValue, anotherValue))>();

    // bignum::operator++(oneValue);
    // bignum::operator--(oneValue);

    // bignum::operator++(oneValue, 1);
    // bignum::operator--(oneValue, 1);

    consumeType<decltype(bignum::operator<<(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator>>(oneValue, anotherValue))>();

    consumeType<decltype(bignum::operator<<=(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator>>=(oneValue, anotherValue))>();

    consumeType<decltype(bignum::operator<<(stream, oneValue))>();
    consumeType<decltype(bignum::operator>>(stream, oneValue))>();

    consumeType<decltype(bignum::operator~(oneValue))>();
    consumeType<decltype(bignum::operator|(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator&(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator^(oneValue, anotherValue))>();

    consumeType<decltype(bignum::operator|=(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator&=(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator^=(oneValue, anotherValue))>();

    // bignum::operator||(oneValue, anotherValue);
    // bignum::operator&&(oneValue, anotherValue);

    consumeType<decltype(bignum::operator!=(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator==(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator< (oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator> (oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator<=(oneValue, anotherValue))>();
    consumeType<decltype(bignum::operator>=(oneValue, anotherValue))>();

    // consumeType<decltype(static_cast<bool>(oneValue))>();
    // consumeType<decltype(!oneValue)>();
}