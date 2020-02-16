#include <bignum/Integer.hpp>

#include <gtest/gtest.h>

#include <sstream>

TEST(IntegerStreamTests, testThatConvertsNegativeInteger)
{
    // given
    const auto abs = bignum::Unsigned
    {
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(1)
    };
    const auto negative = bignum::Integer(bignum::Sign::Minus, abs);
    const auto expected = "-65536";
    auto stream = std::stringstream();

    // when
    stream << negative;
    const auto actual = stream.str();

    // then
    ASSERT_EQ(expected, actual);
}
