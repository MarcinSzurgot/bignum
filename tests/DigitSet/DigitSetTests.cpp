#include <bignum/DigitSet.hpp>

#include <gtest/gtest.h>

TEST(DigitSetTests, testThatDefaultConstructsWithOneZero)
{
    // when
    const auto digits = bignum::DigitSet<int>();

    // then
    ASSERT_EQ(digits.size(), 1);
    ASSERT_EQ(digits[0], 0);
}

TEST(DigitSetTests, testThatVectorConstructsWithAtLeastOneZero)
{
    // given
    const auto vector = std::vector<int>();

    // when
    const auto digits = bignum::DigitSet(vector);

    // then
    ASSERT_EQ(digits.size(), 1);
    ASSERT_EQ(digits[0], 0);
}

TEST(DigitSetTests, testThatInitializerListConstructsWithAtLeastOneZero)
{
    // given
    const auto initializerList = std::initializer_list<int>();

    // when
    const auto digits = bignum::DigitSet(initializerList);

    // then
    ASSERT_EQ(digits.size(), 1);
    ASSERT_EQ(digits[0], 0);
}

TEST(DigitSetTests, testThatInitializerListConstructTrims)
{
    // given
    const auto initializerList = std::initializer_list<int>{1, 1, 1, 0};

    // when
    const auto digits = bignum::DigitSet(initializerList);

    // then
    ASSERT_EQ(digits.size(), 3);
}

TEST(DigitSetTests, testThatSizeConstructsWithAtLeatOneZero)
{
    // given
    const auto size = 0u;

    // when
    const auto digits = bignum::DigitSet(size, 0);

    // then
    ASSERT_EQ(digits.size(), 1);
    ASSERT_EQ(digits[0], 0);
}

TEST(DigitSetTests, testThatTrimsLeadingZeros)
{
    // given
    const auto zeroes = std::vector(3, 0);
    const auto nonZeroes = std::vector(3, 1);
    auto digits = bignum::DigitSet
    (
        [&]()
        {
            auto digits = nonZeroes;
            digits.insert(end(digits), begin(zeroes), end(zeroes));
            return digits;
        }()
    );

    // when
    digits.trim();

    // then
    ASSERT_EQ(digits.size(), nonZeroes.size());
    for (auto digit = 0u; digit < digits.size(); ++digit)
    {
        ASSERT_EQ(digits[digit], nonZeroes[digit]);
    }
}

TEST(DigitSetTests, testThatDoesNotTrimsLastZero)
{
    // given
    auto digits = bignum::DigitSet<int>();

    // when
    digits.trim();

    // then
    ASSERT_EQ(digits.size(), 1);
    ASSERT_EQ(digits[0], 0);
}

TEST(DigitSetTests, testThatDoesNotPopBackLastDigit)
{
    // given
    auto digits = bignum::DigitSet(1, 1);

    // when
    digits.pop_back();

    // then
    ASSERT_EQ(digits.size(), 1);
}

TEST(DigitSetTests, testThatDoesNotPopFrontLastDigit)
{
    // given
    auto digits = bignum::DigitSet(1, 1);

    // when
    digits.pop_front();

    // then
    ASSERT_EQ(digits.size(), 1);
}

TEST(DigitSetTests, testThatClearsToDefaultValue)
{
    // given
    auto digits = bignum::DigitSet(3, 1);

    // when
    digits.clear();

    // then
    ASSERT_EQ(digits.size(), 1);
    ASSERT_EQ(digits[0], 0);
}

TEST(DigitSetTests, testThatCannotResizeToZero)
{
    // given
    auto digits = bignum::DigitSet(3, 1);

    // when
    digits.resize(0);

    // then
    ASSERT_EQ(digits.size(), 1);
}
