#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

TEST(UnsignedConstructorsTests, DigitsConstructor)
{
    // given
    const auto expectedDigits = std::vector<bignum::digit_type>
    {
        bignum::digit_type(3434),
        bignum::digit_type(3847),
        bignum::digit_type(18),
        bignum::digit_type(6969),
        bignum::digit_type(3875),
        bignum::digit_type(8589),
        bignum::digit_type(1),
        bignum::digit_type(2),
        bignum::digit_type(3),
        bignum::digit_type(4),
        bignum::digit_type(5),
        bignum::digit_type(0),
        bignum::digit_type(0),
        bignum::digit_type(12),
        bignum::digit_type(3)
    };

    // when
    const auto value = bignum::Unsigned(expectedDigits);

    // then
    ASSERT_EQ(expectedDigits.size(), value.magnitude());
    for(auto digit = 0u; digit < value.magnitude(); ++digit)
    {
        ASSERT_EQ(expectedDigits[digit], value.digit(digit));
    }
}

TEST(UnsignedConstructorsTests, DefaultConstructor)
{
    // given
    const auto expectedDigits = std::vector<bignum::digit_type>{0};

    // when
    const auto value = bignum::Unsigned();

    // then
    ASSERT_EQ(expectedDigits.size(), value.magnitude());
    ASSERT_EQ(expectedDigits.front(), value.digit(0));
}

TEST(UnsignedConstructorsTests, TrimLeadingZeros)
{
    // given
    const auto nonZero = std::vector
    {
        bignum::digit_type(1),
        bignum::digit_type(234),
        bignum::digit_type(5)
    };
    const auto leadingZeros = [&nonZero]()
    {
        auto leadingZeros = std::vector
        {
            bignum::digit_type(0),
            bignum::digit_type(0),
            bignum::digit_type(0),
            bignum::digit_type(0)
        };
        leadingZeros.insert(begin(leadingZeros), begin(nonZero), end(nonZero));
        return leadingZeros;
    }();

    // when
    const auto value = bignum::Unsigned(leadingZeros);

    // then
    ASSERT_EQ(value.magnitude(), nonZero.size()) << "Leading zeros are not removed.";
    for(auto digit = 0u; digit < nonZero.size(); ++digit)
    {
        ASSERT_EQ(value.digit(digit), nonZero[digit]);
    }
}
