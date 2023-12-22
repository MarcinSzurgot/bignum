#include <iostream>

#include <gtest/gtest.h>

#include <bignum/bignum.hpp>

#include "../Utils.hpp"

using namespace bignum;

TEST(DigitMultiplicationTests, SingleDigit) {
    auto a = BigUnsigned("99982349874598119000192834794191985798357496000032859237958137598982173948275");
    auto b = 999u;
    auto c = a * BigUnsigned(b);

    a *= b;

    ASSERT_EQ(a, c);
}