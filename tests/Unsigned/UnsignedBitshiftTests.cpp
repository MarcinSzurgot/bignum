#include <bignum/Operators/Bitshift.hpp>
#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

#include <sstream>

template<typename DigitType>
std::string print(const bignum::Unsigned<DigitType>& value)
{
    auto stream = std::stringstream();
    stream << "{";
    for (auto d = 0u; d < value.magnitude(); ++d)
    {
        stream << +value[d] << ", ";
    }
    stream << "}";
    return stream.str();
}

TEST(UnsignedBitshiftTests, testThatShiftsByWholeDigits)
{
    // given
    const auto offset = 5u;
    const auto bitoffset = offset * sizeof(unsigned) * CHAR_BIT;
    const auto initial = bignum::Unsigned{1u, 1u, 1u};
    const auto expected = initial.magnitude() + offset;

    // when
    const auto actual = (initial << bitoffset).magnitude();

    // then
    ASSERT_EQ(expected, actual) << "MSD: " << print(initial << bitoffset);
}

TEST(UnsignedBitshiftTests, testThatShiftsDigits)
{
    // given
    const auto offset = 4u;
    const auto bitoffset = offset * sizeof(unsigned) * CHAR_BIT;
    const auto initial = bignum::Unsigned{1u, 2u, 3u};
    const auto expected = bignum::Unsigned{0u, 0u, 0u, 0u, 1u, 2u, 3u};

    // when
    const auto actual = initial << bitoffset;

    // then
    ASSERT_EQ(expected, actual);
}
