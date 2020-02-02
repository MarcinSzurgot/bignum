#include <bignum/Unsigned.hpp>
#include <gtest/gtest.h>

#include <sstream>

TEST(UnsignedStreamTests, testThatConvertsToString)
{
    // given
    const auto value = bignum::Unsigned
    {
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(1),
    };
    const auto expected = "65536";
    auto stream = std::ostringstream();

    // when
    stream << value;
    const auto actual = stream.str();

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedStreamTests, testThatConvertsZeroToString)
{
    // given
    const auto value = bignum::Unsigned
    {
        std::uint8_t(),
    };
    const auto expected = "0";
    auto stream = std::ostringstream();

    // when
    stream << value;
    const auto actual = stream.str();

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedStreamTests, testThatConvertsToUnsigned)
{
    // given
    const auto string = "16777216";
    const auto expected = bignum::Unsigned
    {
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(),
        std::uint8_t(1),
    };
    auto stream = std::istringstream(string);

    // when
    auto actual = bignum::Unsigned<std::uint8_t>();
    stream >> actual;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedStreamTests, testThatConvertsZeroStringToUnsigned)
{
    // given
    const auto string = "0";
    const auto expected = bignum::Unsigned<std::uint32_t>();
    auto stream = std::istringstream(string);

    // when
    auto actual = bignum::Unsigned<std::uint32_t>();
    stream >> actual;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedStreamTests, testThatConvertsUntilInvalidCharacter)
{
    // given
    const auto string = "65k536";
    const auto expected = bignum::Unsigned{std::uint8_t(65)};
    auto stream = std::istringstream(string);

    // when
    auto actual = bignum::Unsigned<std::uint8_t>();
    stream >> actual;

    // then
    ASSERT_EQ(expected, actual);
}

TEST(UnsignedStreamTests, testThatConvertsFromStringAndBackReallyBigValue)
{
    // given
    const auto expected = "9389479187349823498723548976109348"
                          "0938498378463983747823094809238427"
                          "2983749813098498465092834098203498"
                          "8974928374876394878726387123413454"
                          "1203918203985098389679278359827358"
                          "9399918273885746301019823892384734"
                          "2398238747263019239846137182740393"
                          "7092734879057868409813879623748954"
                          "8934919348983401110003889595837734"
                          "9399918273885746301019823892384734"
                          "2398238747263019239846137182740393"
                          "3982938751924736979304829398409458"
                          "1111111110000000000011110101010101"
                          "3393838989328923893289328932892323"
                          "2340239083409823049820576786756565"
                          "1203918203985098389679278359827358"
                          "9399918273885746301019823892384734"
                          "2398238747263019239846137182740393"
                          "3982938751924736979304829398409458"
                          "1111111110000000000011110101010101"
                          "1203918203985098389679278359827358"
                          "0938498378463983747823094809238427"
                          "2983749813098498465092834098203498"
                          "8974928374876394878726387123413454"
                          "1203918203985098389679278359827358"
                          "9399918273885746301019823892384734"
                          "2398238747263019239846137182740393"
                          "7092734879057868409813879623748954"
                          "8934919348983401110003889595837734"
                          "9399918273885746301019823892384734"
                          "2398238747263019239846137182740393"
                          "3982938751924736979304829398409458"
                          "1111111110000000000011110101010101"
                          "3393838989328923893289328932892323"
                          "2340239083409823049820576786756565"
                          "1203918203985098389679278359827358"
                          "9399918273885746301019823892384734"
                          "2398238747263019239846137182740393"
                          "3982938751924736979304829398409458"
                          "1111111110000000000011110101010101"
                          "9399918273885746301019823892384734"
                          "2398238747263019239846137182740393";
    auto stream = std::stringstream(expected);
    auto value = bignum::Unsigned<std::uint32_t>();

    // when
    stream >> value;
    stream = std::stringstream();
    stream << value;
    const auto actual = stream.str();

    // then
    ASSERT_EQ(expected, actual);
}
