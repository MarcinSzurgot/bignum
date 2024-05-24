#include <bignum/bignum.hpp>

#include "Utils.hpp"

#include <gtest/gtest.h>
#include <tuple>

using namespace bignum;

TEST(ConstructorsTests, SingleDigit) {
    auto uint8Constructor = BigUnsigned<NativeDigit>(std::uint8_t(0xFE));
    auto uint16Constructor = BigUnsigned<NativeDigit>(std::uint16_t(0xFEDC));
    auto uint32Constructor = BigUnsigned<NativeDigit>(std::uint32_t(0xFEDCBA98));
    auto uint64Constructor = BigUnsigned<NativeDigit>(std::uint64_t(0xFEDCBA9876543210));

    ASSERT_EQ(size(uint8Constructor.digits()), 1u);
    // ASSERT_EQ(size(uint16Constructor.digits()), );
    // ASSERT_EQ(size(uint32Constructor.digits()), 1u);
    // ASSERT_EQ(size(uint64Constructor.digits()), 1u);

    ASSERT_EQ(uint8Constructor.digits<std::uint8_t>().front(), 0xFE);
    // ASSERT_EQ(uint16Constructor.digits<std::uint16_t>().front(), 0xFEDC);
    // ASSERT_EQ(uint32Constructor.digits<std::uint32_t>().front(), 0xFEDCBA98);
    // ASSERT_EQ(uint64Constructor.digits<std::uint64_t>().front(), 0xFEDCBA9876543210);
}

TEST(ConstructorsTests, Array8bitEqualToTwoDigits) {
    auto array8bitConstructor = BigUnsigned(
        std::vector {
            std::uint8_t(0x10),
            std::uint8_t(0x32),
            std::uint8_t(0x54),
            std::uint8_t(0x76),
            std::uint8_t(0x98),
            std::uint8_t(0xBA),
            std::uint8_t(0xDC),
            std::uint8_t(0xFE),

            std::uint8_t(0xFE),
            std::uint8_t(0xDC),
            std::uint8_t(0xBA),
            std::uint8_t(0x98),
            std::uint8_t(0x76),
            std::uint8_t(0x54),
            std::uint8_t(0x32),
            std::uint8_t(0x10),
        }
    );

    const auto uint64Constructor = BigUnsigned<NativeDigit>(
        std::vector {
            std::uint64_t(0xFEDCBA9876543210),
            std::uint64_t(0x1032547698BADCFE)
        }
    );

    ASSERT_EQ(array8bitConstructor, uint64Constructor)
        << "8bit:  " << toString(array8bitConstructor.digits()) << "\n"
        << "64bit: " << toString(uint64Constructor.digits()) << "\n";
}