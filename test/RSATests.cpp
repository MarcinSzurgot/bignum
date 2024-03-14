#include <concepts>
#include <ranges>
#include <numeric>
#include <valarray>

#include <gtest/gtest.h>

#include <bignum/bignum.hpp>

#include "Utils.hpp"

using namespace bignum;

// BigUnsigned pow2(BigUnsigned::NativeDigit pow) {
//     auto result = BigUnsigned(2u);

//     result <<= pow;

//     return result;
// }

TEST(RSATests, test) {
    // auto generator = RandomGenerator();

    // auto primeCandidate = generator.bigUnsigned(2048);
    // primeCandidate.access().digits().front() |= 1;

    // auto n1 = primeCandidate - 1;

    // auto powOf2 = NativeDigit();

    // for (; powOf2 < 2048; ++powOf2) {
    //     const auto [whole, bit] = div(powOf2, Bits<NativeDigit>::Size);
    //     if (n1.digits()[whole] & (NativeDigit(1) << bit)) {
    //         break;
    //     }
    // }

    // auto s2 = BigUnsigned(2u);
    // s2 <<= powOf2;

    // auto d = n1;
    // d >>= powOf2;

    // auto a = std::vector{1, 2, 3, 4};
    // auto b = std::array{'a', 'b', 'c', 'd', 'e', 'f'};

    // for (auto&& [x, y] : std::views::zip(a, b)) {
    //     std::cout << x << ", " << y << std::endl;
    // }

    // std::cout << s << "\n\n"
    //           << (std::string) n1 << "\n\n"
    //           << (std::string) s2 << "\n\n"
    //           << (std::string) d  << "\n\n"
    //           << std::bitset<Bits<BigUnsigned::NativeDigit>::Size>(n1Digits.back()) << "\n"
            //   ;
}