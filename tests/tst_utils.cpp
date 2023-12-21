#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <QString>
#include "utils.h"

using namespace testing;

//Hostname

TEST(ValidHostname, SuccessWhenValid)
{
    EXPECT_EQ(Utils::isValidHostname("host.name"),true);
    EXPECT_EQ(Utils::isValidHostname("host.name.ag4in.121"),true);
    EXPECT_EQ(Utils::isValidHostname("123.name.ag4in.121"),true);
    EXPECT_EQ(Utils::isValidHostname("ho-st.name"),true);
}

TEST(ValidHostname, FailsOnConsecutiveDots)
{
    EXPECT_EQ(Utils::isValidHostname("host..name"), false);
}

TEST(ValidHostname, FailsOnStartingDash)
{
    EXPECT_EQ(Utils::isValidHostname("-host.name"), false);
}

TEST(ValidHostname, FailsOnSpecialCharacters)
{
    EXPECT_EQ(Utils::isValidHostname("host.name?"), false);
    EXPECT_EQ(Utils::isValidHostname("host.n^ame"), false);
    EXPECT_EQ(Utils::isValidHostname("ho&st.name"), false);
    EXPECT_EQ(Utils::isValidHostname("ho st.name"), false);
    EXPECT_EQ(Utils::isValidHostname("host.name/"), false);
    EXPECT_EQ(Utils::isValidHostname("host_name.org"), false);
}

TEST(ValidHostname, FailsWhenEmpty){
    EXPECT_EQ(Utils::isValidHostname(""), false);
}

//IP

TEST(ValidIP, SuccessWhenIP4Valid)
{
    EXPECT_EQ(Utils::isValidAddress("123.22.1.1"), true);
}


TEST(ValidIP, SuccessWhenIP4Cut){
    EXPECT_EQ(Utils::isValidAddress("1.0.1"), true);
}

TEST(ValidIP, FailsWhenIP4OutOfRange){
    EXPECT_EQ(Utils::isValidAddress("321.0.1.2"), false);
}

TEST(ValidIP, FailsWhenInvalid){
    EXPECT_EQ(Utils::isValidAddress("host.name"), false);
}

TEST(ValidIP, FailsWhenEmpty){
    EXPECT_EQ(Utils::isValidAddress(""), false);
}

//Latitude

TEST(ValidLatitude, SuccessWhenValid){
    EXPECT_EQ(Utils::isValidLatitude("90"), true);
    EXPECT_EQ(Utils::isValidLatitude("0"), true);
    EXPECT_EQ(Utils::isValidLatitude("-90"), true);
}

TEST(ValidLatitude, FailsWhenOutOfRange){
    EXPECT_EQ(Utils::isValidLatitude("91"), false);
    EXPECT_EQ(Utils::isValidLatitude("-91"), false);
}

TEST(ValidLatitude, FailsWhenText){
    EXPECT_EQ(Utils::isValidLatitude("text"), false);
}

TEST(ValidLatitude, FailsWhenEmpty){
    EXPECT_EQ(Utils::isValidLatitude(""), false);
}

//Longitude

TEST(ValidLongitude, SuccessWhenValid){
    EXPECT_EQ(Utils::isValidLongitude("180"), true);
    EXPECT_EQ(Utils::isValidLongitude("0"), true);
    EXPECT_EQ(Utils::isValidLongitude("-180"), true);
}

TEST(ValidLongitude, FailsWhenOutOfRange){
    EXPECT_EQ(Utils::isValidLongitude("181"), false);
    EXPECT_EQ(Utils::isValidLongitude("-181"), false);
}

TEST(ValidLongitude, FailsWhenText){
    EXPECT_EQ(Utils::isValidLongitude("text"), false);
}

TEST(ValidLongitude, FailsWhenEmpty){
    EXPECT_EQ(Utils::isValidLongitude(""), false);
}
