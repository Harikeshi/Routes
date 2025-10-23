#include <Task/Exceptions/ValidationException.hpp>
#include <Task/Schemes/Search/InRegion/Input.hpp>

#include <gtest/gtest.h>

using Input = Schemes::Search::InRegion::Input;
using SearchRing = Entities::SearchRing;

class InputTests : public ::testing::Test
{
protected:
    Input input;
    nlohmann::json jsonObject;

    void SetUp() override
    {
        jsonObject = {
            {"time", 180000.0},
            {"form_target", {{"detection_point", {0, 0}}, {"root-mean-square_error", 99999}, {"courses", {360, 360}}, {"velocities", {{"current", 99.99}, {"max", 99.99}, {"min_noise-reduced", 99.99}, {"max_noise-reduced", 99.99}}}, {"obsolescence_time", 999999}, {"avoidance_distance", 10000}}},
            {"search_region", {{"borders", {{{0, 0}, {0, 40000}, {40000, 40000}, {40000, 8000}, {50000, 40000}, {50000, 0}}, {{14000, 16000}, {14000, 24000}, {26000, 24000}, {26000, 16000}}}}, {"entry_point", {0, 12000}}, {"exit_point", {50000, 12000}}}},
            {"border_line", {{0, 0}, {10000, 10000}, {20000, 20000}}},
            {"ships_parameters", {{"count", 99}, {"detection_range", 6000}, {"max_velocity", 9}, {"search_velocity", 6}, {"turning_radius", 400}, {"min_lenght_section", 200}}}};
    }

    void TearDown() override
    {
        // Код для очистки после каждого теста
    }
};

/*TEST_F(InputTests, fromJson_ValidInput)
{
    EXPECT_NO_THROW(input.fromJson(jsonObject));

    EXPECT_EQ(input.region.outer(), SearchRing({{0, 0}, {0, 12000}, {0, 40000}, {40000, 40000}, {40000, 8000}, {50000, 40000}, {50000, 12000}, {50000, 0}}));
    EXPECT_EQ(input.region.inners()[0], SearchRing({{14000, 16000}, {14000, 24000}, {26000, 24000}, {26000, 16000}}));

    EXPECT_EQ(input.ship.searchVelocity, 11.663077030736096);
    EXPECT_EQ(input.ship.maxVelocity, 17.494615546104143);
    EXPECT_EQ(input.ship.detectionRange, 3.2397408207343412);
    EXPECT_EQ(input.searchTime, 50);
}*/

TEST_F(InputTests, fromJson_noTime)
{
    jsonObject.erase("time");
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_emptyJson)
{
    jsonObject = {};
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_emptyRegion)
{
    jsonObject["search_region"] = {}; // "borders" отсутствует
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_emptyPolygon)
{
    jsonObject["search_region"]["borders"] = {}; // "borders" = null
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_incorrectPolygon1)
{
    jsonObject["search_region"]["borders"] = {0}; // "borders" = [0]
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_incorrectPolygon2)
{
    jsonObject["search_region"]["borders"] = {"10"}; // "borders":["10"]
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_incorrectPolygon3)
{
    jsonObject["search_region"]["borders"] = {0, 1, 2}; // "borders":[[0]]
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_emptyOuter)
{
    jsonObject["search_region"]["borders"][0] = {}; // "borders":[null, [[,], [,]]]
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_emptyInners)
{
    jsonObject["search_region"]["borders"][1] = {}; // "borders":[[[,], [,]], null]
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_incorrectOuter1)
{
    jsonObject["search_region"]["borders"][0] = {0}; // "borders":[[0], [[,], [,]]]
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_incorrectOuter2)
{
    jsonObject["search_region"]["borders"][0] = {"10"}; // "borders":[["10"], [[,], [,]]]
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_incorrect1stPoint1)
{
    jsonObject["search_region"]["borders"][0][0] = {1}; // "borders":[[[1], [,]], [,]], [[,], [,]]]
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_incorrect1stPoint2)
{
    jsonObject["search_region"]["borders"][0][0] = {1, 2, 3}; // "borders":[[[1, 2 ,3], [,]], [,]], [[,], [,]]]
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_noEntrance)
{
    jsonObject["search_region"].erase("entry_point"); // отсутствует поле "entry_point"
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_nullEntrance)
{
    jsonObject["search_region"]["entry_point"] = {}; // "entry_point":null
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_incorrectEntryPoint1)
{
    jsonObject["search_region"]["entry_point"] = {1, 2, 3}; // "entry_point":[1, 2, 3]
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_incorrectEntryPoint2)
{
    jsonObject["search_region"]["entry_point"] = {"10"}; // "entry_point":["10"]
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_outOfRangeVelocity)
{
    jsonObject["ships_parameters"]["max_velocity"] = 150;
    EXPECT_THROW(input.fromJson(jsonObject),
                 Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_nullVelocity)
{
    jsonObject["ships_parameters"]["search_velocity"] = "null";
    EXPECT_THROW(input.fromJson(jsonObject),
                 Exceptions::ValidationFailure);
}

TEST_F(InputTests, fromJson_emptyVelocity)
{
    jsonObject["ships_parameters"]["search_velocity"] = "";
    EXPECT_THROW(input.fromJson(jsonObject),
                 Exceptions::ValidationFailure);
}
