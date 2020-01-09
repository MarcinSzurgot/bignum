#include <bignum/Unsigned.hpp>

#include <gtest/gtest.h>

TEST(UnsignedBitshiftOperatorsTests, testThatIncreasesSizeByWholeDigits)
{
    using unsigned_type = bignum::Unsigned<int>;
    using digit_type = typename unsigned_type::digit_type;

    // given
    constexpr auto bitsPerDigit = sizeof(digit_type) * 8u;
    const auto initial = unsigned_type{1, 1, 1};
    const auto digits = 10u;

    for (auto digit = 0u; digit <= digits; ++digit)
    {
        // given
        const auto offset = digit * bitsPerDigit;
        const auto expected = initial.magnitude() + digit;

        // when
        const auto actual = bignum::operator<<(initial, offset).magnitude();

        // then
        EXPECT_EQ(expected, actual);
    }
}
