#include <Task/Outputs/SchemeEfficiency.hpp>

#include <gtest/gtest.h>

class SchemeEfficiencyTest : public ::testing::Test
{};

TEST_F(SchemeEfficiencyTest, ToJson)
{
    Outputs::SchemeEfficiency schemeEfficiency{1, 0.9, 45.0};
    auto json = schemeEfficiency.toJson();

    EXPECT_EQ(json["scheme"], 1);
    EXPECT_EQ(json["efficiency"], 0.9);
    EXPECT_EQ(json["approach_course"], 45.0);
}
