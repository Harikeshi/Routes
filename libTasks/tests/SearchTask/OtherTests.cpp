#include "Task/Outputs/Message.hpp"
#include <Task/Operations/JsonOperations.hpp>
#include <Task/SearchTask.hpp>

#include <gtest/gtest.h>

class SearchTaskTest : public ::testing::Test
{
protected:
    nlohmann::json request_by_call;
    nlohmann::json request_at_border;

    nlohmann::json request_restore;
    nlohmann::json request_region_restore;

    SearchTask task;

    std::string path;

    SearchTaskTest()
        : task{request_by_call}
    {
    }

    void SetUp() override
    {
        request_by_call = {
            {"time", 180000.0},
            {"form_target", {{"detection_point", {10000, 10000}}, {"root-mean-square_error", 3000}, {"courses", {65, 90}}, {"velocities", {{"current", 7}, {"max", 15}, {"min_noise-reduced", 2}, {"max_noise-reduced", 7}}}, {"obsolescence_time", 7200}, {"avoidance_distance", 10000}}},
            {"search_region", {{"borders", {{{-35000, -10000}, {-35000, 15000}, {15000, 15000}, {15000, -10000}, {-35000, -10000}}}}, {"entry_point", {-35000, 6000}}, {"exit_point", {15000, 6000}}}},
            {"border_line", {{0, 0}, {10000, 10000}, {20000, 20000}}},
            {"ships_parameters", {{"count", 99}, {"detection_range", 4000}, {"max_velocity", 15}, {"search_velocity", 10}, {"turning_radius", 400}, {"min_lenght_section", 200}}}};

        request_at_border = {
            {"time", 180000.0},
            {"form_target", {{"detection_point", {0, 0}}, {"root-mean-square_error", 99999}, {"courses", {360, 360}}, {"velocities", {{"current", 99.99}, {"max", 99.99}, {"min_noise-reduced", 99.99}, {"max_noise-reduced", 99.99}}}, {"obsolescence_time", 999999}, {"avoidance_distance", 10000}}},
            {"search_region", {{"borders", {{{0, 0}, {0, 40000}, {40000, 40000}, {40000, 8000}, {50000, 40000}, {50000, 0}, {0, 0}}, {{14000, 16000}, {14000, 24000}, {26000, 24000}, {26000, 16000}, {14000, 16000}}}}, {"entry_point", {0, 12000}}, {"exit_point", {50000, 12000}}}},
            {"border_line", {{0, 0}, {10000, 10000}}},
            {"ships_parameters", {{"count", 99}, {"detection_range", 6000}, {"max_velocity", 9}, {"search_velocity", 6}, {"turning_radius", 400}, {"min_lenght_section", 200}}}};

        request_restore = {
            {"Tpk", {0, 0}},
            {"Tbek_0", {6000, 4000}},
            {"Tbek_exit", {9000, 9000}},

            {"Dpgas", 8000},
            {"Dbgas", 8000},
            {"Kt_deg", 180},
            {"Kbek_deg", -60},
            {"Ut", 5},
            {"Ukp", 7},
            {"Ukph", 9},
            {"Ukp_r", 7}};

        request_region_restore = {
            {"time", 180000.0},
            {"form_target", {{"detection_point", {0, 0}}, {"root-mean-square_error", 99999}, {"courses", {360, 360}}, {"velocities", {{"current", 5}, {"max", 99.99}, {"min_noise-reduced", 99.99}, {"max_noise-reduced", 99.99}}}, {"obsolescence_time", 999999}, {"avoidance_distance", 10000}}},
            {"search_region", {{"borders", {{{-35000, -10000}, {-35000, 15000}, {15000, 15000}, {15000, -10000}, {-35000, -10000}}}}, {"entry_point", {-35000, 6000}}, {"exit_point", {15000, 6000}}}},
            {"border_line", {{0, 0}, {10000, 10000}, {20000, 20000}}},
            {"ships_parameters", {{"count", 99}, {"detection_range", 4000}, {"max_velocity", 9}, {"search_velocity", 6}, {"turning_radius", 400}, {"min_lenght_section", 200}}}};

        task = SearchTask{request_by_call};

        path = "report_by_call.json";
    }
};

// Petlya
TEST_F(SearchTaskTest, computeRoute_Petlya_ValidInput)
{
    task = SearchTask{request_restore};

    auto result = task.computeRoute(SearchScheme::PetljaK);

    Operations::toFile("report_petlya.json", result);
    Operations::toFile("request_region_restore.json", request_region_restore);
    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

// Proryv
TEST_F(SearchTaskTest, computeRoute_Proryv_ValidInput)
{
    task = SearchTask{request_restore};

    auto result = task.computeRoute(SearchScheme::ProryvK);

    Operations::toFile("report_proryv.json", result);
    //    Operations::toFile("request_region_restore.json", request_region_restore);
    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

//AtBorder

TEST_F(SearchTaskTest, computeRoute_CrossPatrolling_ValidInput)
{
    request_at_border["form_target"]["velocities"]["max"] = 5;
    request_at_border["border_line"] = {{0, 0}, {9000, 9000}};

    task = SearchTask{request_at_border};
    auto result = task.computeRoute(SearchScheme::CrossPatrolling);

    Operations::toFile("report_cross_patrolling.json", result);
    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskTest, computeRoute_LinearPatrolling_ValidInput)
{
    task = SearchTask{request_at_border};
    auto result = task.computeRoute(SearchScheme::LinearPatrolling);

    Operations::toFile("report_linear_patrolling.json", result);
    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

//ByCall
TEST_F(SearchTaskTest, computeRoute_SectorSpiral_ValidInput)
{
    task = SearchTask{request_by_call};
    nlohmann::json result = task.computeRoute(SearchScheme::SectorSpiral);

    Operations::toFile("report_sector.json", result);
    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskTest, computeRoute_StraightTack_ValidInput)
{
    task = SearchTask{request_by_call};

    nlohmann::json result = task.computeRoute(SearchScheme::StraightTackCall);

    Operations::toFile("report_straightByCall.json", result);
    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}
