#include <bignum/Unsigned.hpp>
#include <gtest/gtest.h>

#include <sstream>

TEST(UnsignedStreamTests, testThatConvertsToString)
{
    // given
    const auto value = bignum::Unsigned
    {
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(1),
    };
    const auto expected = "65536";
    auto stream = std::ostringstream();

    // when
    stream << value;
    const auto actual = stream.str();

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedStreamTests, testThatConvertsZeroToString)
{
    // given
    const auto value = bignum::Unsigned
    {
        std::uint8_t(),
    };
    const auto expected = "0";
    auto stream = std::ostringstream();

    // when
    stream << value;
    const auto actual = stream.str();

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedStreamTests, testThatConvertsToUnsigned)
{
    // given
    const auto string = "16777216";
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(1),
    };
    auto stream = std::istringstream(string);

    // when
    auto actual = bignum::Unsigned<std::uint8_t>();
    stream >> actual;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedStreamTests, testThatConvertsZeroStringToUnsigned)
{
    // given
    const auto string = "0";
    const auto expected = bignum::Unsigned<std::uint32_t>();
    auto stream = std::istringstream(string);

    // when
    auto actual = bignum::Unsigned<std::uint32_t>();
    stream >> actual;

    // then
    ASSERT_EQ(expected, actual);
}
