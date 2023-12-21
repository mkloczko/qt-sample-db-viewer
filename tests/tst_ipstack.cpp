#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "ipstack.h"

using namespace testing;

TEST(IpStackJSON, ErrorWhenEmpty)
{
    //Given
    IpStackResponse resp;
    QJsonDocument doc;
    ASSERT_THAT(doc.isArray(), false);
    ASSERT_THAT(doc.isObject(), false);

    //When
    resp.parseResponse(doc);

    //Then
    EXPECT_NE(resp.main_error, "");
    EXPECT_EQ(resp.coordinates.size(),0);
    EXPECT_EQ(resp.errors.size(),0);
}

TEST(IpStackJSON, ErrorWhenInvalid)
{
    //Given
    IpStackResponse resp;
    QJsonDocument doc = QJsonDocument::fromJson("invalidJson = 312}");
    ASSERT_THAT(doc.isArray(), false);
    ASSERT_THAT(doc.isObject(), false);

    //When
    resp.parseResponse(doc);

    //Then
    EXPECT_NE(resp.main_error, "");
    EXPECT_EQ(resp.coordinates.size(),0);
    EXPECT_EQ(resp.errors.size(),0);
}

TEST(IpStackJSON, ValidParse){
    //Given
    IpStackResponse resp;
    QJsonDocument doc = QJsonDocument::fromJson("{\"latitude\": 31.0, \"longitude\":16.12}");
    ASSERT_THAT(doc.isObject(), true);
    ASSERT_THAT(doc["latitude"].isDouble(), true);
    ASSERT_THAT(doc["longitude"].isDouble(), true);

    //When
    resp.parseIndividual(3,doc.object());

    //Then
    EXPECT_EQ(resp.main_error, "");
    EXPECT_EQ(resp.coordinates.size(),1);
    EXPECT_EQ(resp.errors.size(),0);
    EXPECT_DOUBLE_EQ(resp.coordinates[3].latitude(), 31.0);
    EXPECT_DOUBLE_EQ(resp.coordinates[3].longitude(), 16.12);
}

TEST(IpStackJSON, ValidErrorResponseParse){
    //Given
    IpStackResponse resp;
    QJsonDocument doc = QJsonDocument::fromJson("{\"error\": { \"code\":123,\"info\":\"Error\"}}");
    ASSERT_THAT(doc.isObject(), true);
    ASSERT_THAT(doc["error"].isObject(), true);
    ASSERT_THAT(doc["error"]["code"].isDouble(), true);
    ASSERT_THAT(doc["error"]["info"].isString(), true);

    //When
    resp.parseIndividual(5,doc.object());

    //Then
    EXPECT_EQ(resp.main_error, "");
    EXPECT_EQ(resp.coordinates.size(),0);
    EXPECT_EQ(resp.errors.size(),1);
    EXPECT_NE(resp.errors[5], "");
}

TEST(IpStackJSON, ValidError){
    //Given
    IpStackResponse resp;
    QJsonDocument doc = QJsonDocument::fromJson("{\"random\":1234}");
    ASSERT_THAT(doc.isObject(), true);
    ASSERT_THAT(doc["error"].isUndefined(), true);
    ASSERT_THAT(doc["latitude"].isUndefined(), true);
    ASSERT_THAT(doc["longitude"].isUndefined(), true);

    //When
    resp.parseIndividual(4,doc.object());

    //Then
    EXPECT_EQ(resp.main_error, "");
    EXPECT_EQ(resp.coordinates.size(),0);
    EXPECT_EQ(resp.errors.size(),1);
    EXPECT_NE(resp.errors[4], "");
}
