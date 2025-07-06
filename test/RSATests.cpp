#include <bignum/bignum.hpp>

#include "Utils.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <concepts>
#include <ranges>
#include <numeric>
#include <valarray>

using namespace bignum;

template<
    std::ranges::input_range Base,
    std::input_iterator Power,
    std::unsigned_integral Unsigned = std::ranges::range_value_t<Base>
> requires std::same_as<Unsigned, std::iter_value_t<Power>>
constexpr auto exp(
    Base&& base,
    std::size_t exponent,
    Power power
) {

}

TEST(RSATests, test) {
    using namespace std::string_view_literals;

    auto container = std::string();
    const auto insert = "siema XD "sv;
    const auto insert2 = "elo XD"sv;

    auto containerFirst = std::back_inserter(container);

    auto containerLast = std::copy(
        begin(insert),
        end(insert),
        containerFirst
    );

    // containerLast = std::copy(
    //     std::reverse_iterator(containerLast),
    //     std::reverse_iterator(containerFirst),
    //     std::reverse_iterator(std::next(containerLast, size(insert)))
    // ).base();

    // std::ranges::copy(insert2, containerFirst);

    std::cout << container << "\n";
}