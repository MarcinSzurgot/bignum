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

TEST(IntegerStreamTests, testThatConvertsStringToPositiveInteger)
{
    // given
    const auto string = "892839";
    const auto expected = bignum::Integer<unsigned>(892839u);
    auto actual = bignum::Integer<unsigned>();
    auto stream = std::stringstream();
    stream << string;

    // when
    stream >> actual;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerStreamTests, testThatConvertsStringWithPlusToPositiveInteger)
{
    // given
    const auto string = "+892839";
    const auto expected = bignum::Integer<unsigned>(892839u);
    auto actual = bignum::Integer<unsigned>();
    auto stream = std::stringstream();
    stream << string;

    // when
    stream >> actual;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerStreamTests, testThatConvertsStringWithMinusToNegativeInteger)
{
    // given
    const auto string = "-892839";
    const auto expected = bignum::Integer<unsigned>(-892839);
    auto actual = bignum::Integer<unsigned>();
    auto stream = std::stringstream();
    stream << string;

    // when
    stream >> actual;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerStreamTests, testThatConvertsStringWithSignAndInvalidCharactersToZero)
{
    // given
    const auto string = "-i892839";
    const auto expected = bignum::Integer<unsigned>();
    auto actual = bignum::Integer<unsigned>();
    auto stream = std::stringstream();
    stream << string;

    // when
    stream >> actual;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(IntegerStreamTests, testThatIgnoresLeadingWhitespaces)
{
    // given
    const auto string = "   -892839";
    const auto expected = bignum::Integer<unsigned>(-892839);
    auto actual = bignum::Integer<unsigned>();
    auto stream = std::stringstream();
    stream << string;

    // when
    stream >> actual;

    // then
    ASSERT_EQ(expected, actual);
}
