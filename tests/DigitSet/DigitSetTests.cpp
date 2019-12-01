#include <bignum/DigitSet.hpp>

#include <gtest/gtest.h>

TEST(DigitSetTests, testThatInitsWithGivenDigits)
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
    const auto value = bignum::DigitSet(expectedDigits);

    // then
    ASSERT_EQ(expectedDigits.size(), value.size());
    for(auto digit = 0u; digit < value.size(); ++digit)
    {
        ASSERT_EQ(expectedDigits[digit], value[digit]);
    }
}

TEST(DigitSetTests, testThatByDefaultInitsWithZero)
{
    // given
    const auto expectedDigits = std::vector<bignum::digit_type>{0};

    // when
    const auto value = bignum::DigitSet();

    // then
    ASSERT_EQ(expectedDigits.size(), value.size());
    ASSERT_EQ(expectedDigits.front(), value[0]);
}

TEST(DigitSetTests, testThatInitsWithZero)
{
    // given
    const auto nothing = std::vector<bignum::digit_type>();
    const auto expected = std::vector{bignum::digit_type()};

    // when
    const auto value = bignum::DigitSet(nothing);

    // then
    ASSERT_EQ(expected.size(), value.size());
    ASSERT_EQ(expected.front(), value[0]);
}

TEST(DigitSetTests, testThatInitsWithSizeIsNotZero)
{
    // given
    const auto zeroSize = 0u;
    const auto expected = 1u;

    // when
    const auto given = bignum::DigitSet(zeroSize);

    // then
    ASSERT_EQ(given.size(), expected);
}

TEST(DigitSetTests, testThatTrimsLeadingZeros)
{
    // given
    const auto noLeadingZeroDigits = std::vector
    {
        bignum::digit_type(0),
        bignum::digit_type(1),
        bignum::digit_type(234),
        bignum::digit_type(5),
    };
    const auto zeroes = std::vector
    {
        bignum::digit_type(0),
        bignum::digit_type(0),
        bignum::digit_type(0),
        bignum::digit_type(0),
    };
    auto digits = bignum::DigitSet(noLeadingZeroDigits);
    for(const auto zero : zeroes)
    {
        digits.push_back(zero);
    }

    // when
    digits.trim();

    // then
    ASSERT_EQ(digits.size(), noLeadingZeroDigits.size()) << "Leading zeros are not removed.";
    for(auto digit = 0u; digit < noLeadingZeroDigits.size(); ++digit)
    {
        ASSERT_EQ(digits[digit], noLeadingZeroDigits[digit]);
    }
}

TEST(DigitSetTests, testThatDoesNotTrim)
{
    // given
    const auto noLeadingZeroDigits = std::vector
    {
        bignum::digit_type(0),
        bignum::digit_type(1),
        bignum::digit_type(234),
        bignum::digit_type(5),
        bignum::digit_type(0),
        bignum::digit_type(0),
        bignum::digit_type(0),
        bignum::digit_type(0),
        bignum::digit_type(1)
    };
    auto digits = bignum::DigitSet(noLeadingZeroDigits);

    // when
    digits.trim();

    // then
    ASSERT_EQ(digits.size(), noLeadingZeroDigits.size());
    for(auto digit = 0u; digit < noLeadingZeroDigits.size(); ++digit)
    {
        ASSERT_EQ(digits[digit], noLeadingZeroDigits[digit]);
    }
}

TEST(DigitSetTests, testThatDoesNotTrimZero)
{
    // given
    auto digits = bignum::DigitSet();

    // when
    digits.trim();

    // then
    ASSERT_EQ(digits.size(), 1);
    ASSERT_EQ(digits[0], bignum::digit_type());
}

TEST(DigitSetTests, testThatDoesNotPopBackOneDigit)
{
    // given
    auto digits = bignum::DigitSet();

    // when
    digits.pop_back();

    // then
    ASSERT_EQ(digits.size(), 1);
    ASSERT_EQ(digits[0], bignum::digit_type());
}

TEST(DigitSetTests, testThatDoesNotPopFrontOneDigit)
{
    // given
    auto digits = bignum::DigitSet();

    // when
    digits.pop_front();

    // then
    ASSERT_EQ(digits.size(), 1);
    ASSERT_EQ(digits[0], bignum::digit_type());
}

TEST(DigitSetTests, testThatDoesClearToOneDigit)
{
    // given
    auto digits = bignum::DigitSet
    ({
        bignum::digit_type(13),
        bignum::digit_type(4),
        bignum::digit_type(65),
        bignum::digit_type(123),
        bignum::digit_type(6),
    });

    // when
    digits.clear();

    // then
    ASSERT_EQ(digits.size(), 1);
    ASSERT_EQ(digits[0], bignum::digit_type());
}

TEST(DigitSetTests, testThatCannotResizeToZero)
{
    // given
    auto digits = bignum::DigitSet
    ({
        bignum::digit_type(0),
        bignum::digit_type(5),
        bignum::digit_type(123),
        bignum::digit_type(675),
        bignum::digit_type(5),
    });

    // when
    digits.resize(0);

    // then
    ASSERT_EQ(digits.size(), 1);
    ASSERT_EQ(digits[0], bignum::digit_type());
}
