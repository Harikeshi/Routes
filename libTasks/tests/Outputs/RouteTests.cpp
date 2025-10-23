#include <Task/Outputs/Route.hpp>

#include <gtest/gtest.h>

class RouteTest : public ::testing::Test
{};

TEST_F(RouteTest, ToJson)
{
    Outputs::Route route;
    route.points = {{0, 0}, {1, 1}};
    route.velocities = {10.0, 20.0};
    auto json = route.toJson();

    EXPECT_EQ(json["points"].size(), 2);
    EXPECT_EQ(json["velocities"].size(), 2);
}
