#include <Task/Operations/JsonOperations.hpp>

#include <gtest/gtest.h>

class JsonOperationsTest : public ::testing::Test
{
protected:
    nlohmann::json jsonObject;
    //    Schemes::Search::InRegion::Region region{};
    Entities::SearchRegion region{};

    Point2D begin{0, 6};
    Point2D end{25, 6};
    Point2D test{0, 20};

    void SetUp() override
    {
        jsonObject = {{"ships_parameters",
                       {{"search_velocity", 10.0},
                        {"max_velocity", 20.0},
                        {"detection_range", 5000.0}}},
                      {"time", 3600.0},
                      {"search_region",
                       {{"borders",
                         {{{0, 0}, {0, 20}, {20, 20}, {20, 4}, {25, 20}, {25, 0}},
                          {{7, 8}, {7, 12}, {13, 12}, {13, 8}}}},
                        {"entry_point", {0, 6}},
                        {"exit_point", {25, 6}}}}};
    }

    void TearDown() override
    {
        // Код для очистки после каждого теста
    }
};

/*TEST_F(JsonOperationsTest, setRegionfromJson)
{
    EXPECT_NO_THROW(Operations::setRegionFromJson(region, jsonObject["search_region"]));

    EXPECT_EQ(region.getEntrance().point, begin);

    EXPECT_EQ(region.getExit().point, end);
    EXPECT_EQ(region.getPolygon().outer()[1], test);
}*/

TEST_F(JsonOperationsTest, validateRegionJsonBorderMissed)
{
    jsonObject["search_region"].erase("borders");
    EXPECT_THROW(Operations::setRegionFromJson(region, jsonObject["search_region"]),
                 std::runtime_error);
}

TEST_F(JsonOperationsTest, validateRegionJsonBorderNoArray)
{
    jsonObject["search_region"]["borders"] = "null";

    EXPECT_THROW(Operations::setRegionFromJson(region, jsonObject["search_region"]),
                 std::runtime_error);
}

TEST_F(JsonOperationsTest, validateRegionJsonBorderEmpty)
{
    jsonObject["search_region"]["borders"] = nlohmann::json::array();
    EXPECT_THROW(Operations::setRegionFromJson(region, jsonObject["search_region"]),
                 std::runtime_error);
}

TEST_F(JsonOperationsTest, validateRegionJsonEntranceMissed)
{
    jsonObject["search_region"].erase("entry_point");
    EXPECT_THROW(Operations::setRegionFromJson(region, jsonObject["search_region"]),
                 std::runtime_error);
}

TEST_F(JsonOperationsTest, validateRegionJsonExitMissed)
{
    jsonObject["search_region"].erase("exit_point");
    EXPECT_THROW(Operations::setRegionFromJson(region, jsonObject["search_region"]),
                 std::runtime_error);
}
