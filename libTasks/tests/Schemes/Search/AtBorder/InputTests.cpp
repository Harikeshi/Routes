#include <Task/Exceptions/ValidationException.hpp>
#include <Task/Schemes/Search/AtBorder/Input.hpp>

#include <gtest/gtest.h>

using InputAtBorder = Schemes::Search::AtBorder::Input;

class InputAtBorderTest : public ::testing::Test
{
protected:
    InputAtBorder input;
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

TEST_F(InputAtBorderTest, fromJsonMissingField)
{
    jsonObject.erase("border_line");
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputAtBorderTest, fromJsonNotDigitField)
{
    jsonObject["ships_parameters"]["search_velocity"] = "null";
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputAtBorderTest, fromJsonEmptyField)
{
    jsonObject["ships_parameters"]["search_velocity"] = "";
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputAtBorderTest, fromJsonInvalidValueField)
{
    jsonObject["ships_parameters"]["max_velocity"] = 150;
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}
