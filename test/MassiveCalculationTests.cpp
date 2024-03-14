#include <gtest/gtest.h>

#include <bignum/bignum.hpp>

#include "Utils.hpp"

using namespace bignum;

TEST(MassiveCalculationTests, MassiveMultiplicationTests) {
    const auto minSize = 1;
    const auto maxSize = 3;

    auto rnd = RandomGenerator();

    for (auto i = 0u; i < 20000u; ++i) {

        const auto a = BigUnsigned(rnd.random<NativeDigit>(rnd.random(minSize, maxSize)));
        const auto b = BigUnsigned(rnd.random<NativeDigit>(rnd.random(minSize, maxSize)));

        if (!a || !b) {
            continue;
        }

        const auto c = a * b;
        const auto s = (std::string) c;
        const auto da = BigUnsigned(s) / b;
        const auto db = BigUnsigned(s) / a;

        EXPECT_EQ(a, da) 
            << " a: "   << toString( a.digits()) 
            << "\nda: " << toString(da.digits())
            << "\n c: " << toString( c.digits());
        EXPECT_EQ(b, db) 
            << " b: "   << toString( b.digits()) 
            << "\ndb: " << toString(db.digits())
            << "\n c: " << toString( c.digits());
    }
}

TEST(MassiveCalculationTests, MassiveAdditionTests) {
    const auto minSize = 1;
    const auto maxSize = 30;

    auto rnd = RandomGenerator();

    for (auto i = 0u; i < 100000u; ++i) {

        const auto a = BigUnsigned(rnd.random<NativeDigit>(rnd.random(minSize, maxSize)));
        const auto b = BigUnsigned(rnd.random<NativeDigit>(rnd.random(minSize, maxSize)));
        const auto c = a + b;
        const auto da = c - b;
        const auto db = c - a;

        EXPECT_EQ(a, da);
        EXPECT_EQ(b, db);
    }
}