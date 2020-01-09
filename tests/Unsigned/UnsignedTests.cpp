#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

TEST(UnsignedTests, testThatCastsToFloat)
{
    using unsigned_type = bignum::Unsigned<std::uint8_t>;
    using digit_type    = typename unsigned_type::digit_type;

    constexpr auto order = std::numeric_limits<digit_type>::max() + 1.f;

    // given
    const auto value = unsigned_type
    {
        digit_type(2),
        digit_type(2),
        digit_type(2),
    };
    const auto expected = (2 * order + 2) * order + 2;

    // when
    const auto actual = static_cast<float>(value);

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedTests, testThatNonZeroCastsToTrue)
{
    using unsigned_type = bignum::Unsigned<int>;
    using digit_type = typename unsigned_type::digit_type;

    // given
    const auto value = unsigned_type
    {
        digit_type(0),
        digit_type(0),
        digit_type(1),
    };

    // when
    const auto actual = static_cast<bool>(value);

    // then
    ASSERT_TRUE(actual);
}

TEST(UnsignedTests, testThatZeroCastsToFalse)
{
    // given
    const auto value = bignum::Unsigned<int>();

    // when
    const auto actual = static_cast<bool>(value);

    // then
    ASSERT_FALSE(actual);
}

TEST(UnsignedTests, testThatReturnsMostSignificantDigit)
{
    using unsigned_type = bignum::Unsigned<int>;
    using digit_type = typename unsigned_type::digit_type;

    // given
    const auto expected = digit_type(7);
    const auto value = unsigned_type
    {
        digit_type(),
        digit_type(),
        digit_type(),
        digit_type(),
        expected
    };

    // when
    const auto actual = value.msd();

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedTests, testThatReturnsLeastSignificantDigit)
{
    using unsigned_type = bignum::Unsigned<int>;
    using digit_type = typename unsigned_type::digit_type;

    // given
    const auto expected = digit_type(7);
    const auto value = unsigned_type
    {
        expected,
        digit_type(),
        digit_type(),
        digit_type(),
        digit_type(),
    };

    // when
    const auto actual = value.lsd();

    // then
    ASSERT_EQ(expected, actual);
}
