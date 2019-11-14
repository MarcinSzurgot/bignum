#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

TEST(Unsigned, DigitsConstructor)
{
    // given
    const auto expectedDigits = std::vector<bignum::digit_type>
    {
        1, 2, 3, 4, 5
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

TEST(Unsigned, DefaultConstructor)
{
    // given
    const auto expectedDigits = std::vector<bignum::digit_type>{0};

    // when
    const auto value = bignum::Unsigned();

    // then
    ASSERT_EQ(expectedDigits.size(), value.magnitude());
    ASSERT_EQ(expectedDigits.front(), value.digit(0));
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
