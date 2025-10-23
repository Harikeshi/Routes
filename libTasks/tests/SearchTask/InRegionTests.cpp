#include <Task/Entities/Efficiency.hpp>
#include <Task/Operations/JsonOperations.hpp>
#include <Task/Operations/MatrixSerializer.hpp>
#include <Task/Operations/Table.hpp>
#include <Task/Outputs/Message.hpp>
#include <Task/Schemes/Search/InRegion/DeterminedShift.hpp>
#include <Task/Schemes/Search/InRegion/Input.hpp>
#include <Task/Schemes/Search/InRegion/StraightTackOnce.hpp>
#include <Task/SearchTask.hpp>

#include <AbstractOperations/Algorithms/Helper/BufferHelper.hpp>

#include <gtest/gtest.h>
#include <iomanip>
#include <map>

using Input = Schemes::Search::InRegion::Input;
using namespace Operations;

class SearchTaskInRegionTest : public ::testing::Test
{
protected:
    nlohmann::json request_nautical;
    nlohmann::json request_zigzag_base;
    nlohmann::json request_rectangle;
    nlohmann::json request_zigzag_rectangle_sloped;
    nlohmann::json request_square;

    nlohmann::json request_rectangle_inners;
    nlohmann::json request_irregular;
    nlohmann::json request_shape_z;
    nlohmann::json request_concave;
    nlohmann::json request_numerous;
    nlohmann::json request_trapezoid;

    std::string path;
    std::string path_zigzag;
    std::string path_shift;
    std::string path_straight_tack;

    std::string path_rectangle_inners;
    std::string path_irregular;
    std::string path_concave;
    std::string path_shape_z;
    std::string path_trapezoid;
    std::string path_numerous;

    SearchTask task;

    SearchTaskInRegionTest()
        : task{request_zigzag_base}
    {
    }

    void SetUp() override
    {
        request_nautical = {
            {"time", 100.0},
            {"form_target", {{"detection_point", {0, 0}}, {"root-mean-square_error", 99999}, {"courses", {360, 360}}, {"velocities", {{"current", 99.99}, {"max", 99.99}, {"min_noise-reduced", 99.99}, {"max_noise-reduced", 99.99}}}, {"obsolescence_time", 999999}, {"avoidance_distance", 999999}}},
            {"search_region", {{"borders", {{{0, 0}, {0, 20}, {20, 20}, {20, 4}, {25, 20}, {25, 0}, {0, 0}}, {{7, 8}, {7, 12}, {13, 12}, {13, 8}, {7, 8}}}}, {"entry_point", {0, 6}}, {"exit_point", {25, 6}}}},
            {"border_line", {{0, 0}, {1, 1}, {2, 2}}},
            {"ships_parameters", {{"count", 99}, {"detection_range", 3}, {"max_velocity", 18}, {"search_velocity", 12}, {"turning_radius", 0.2}, {"min_lenght_section", 2}}}};

        // в файле request_zigzag.json
        request_zigzag_base = {
            {"time", 180000.0},
            {"form_target", {{"detection_point", {0, 0}}, {"root-mean-square_error", 99999}, {"courses", {360, 360}}, {"velocities", {{"current", 99.99}, {"max", 99.99}, {"min_noise-reduced", 99.99}, {"max_noise-reduced", 99.99}}}, {"obsolescence_time", 999999}, {"avoidance_distance", 10000}}},
            {"search_region", {{"borders", {{{0, 0}, {0, 40000}, {40000, 40000}, {40000, 8000}, {50000, 40000}, {50000, 0}, {0, 0}}, {{14000, 16000}, {14000, 24000}, {26000, 24000}, {26000, 16000}, {14000, 16000}}}}, {"entry_point", {0, 12000}}, {"exit_point", {50000, 12000}}}},
            {"border_line", {{0, 0}, {10000, 10000}, {20000, 20000}}},
            {"ships_parameters", {{"count", 99}, {"detection_range", 6000}, {"max_velocity", 9}, {"search_velocity", 6}, {"turning_radius", 400}, {"min_lenght_section", 200}}}};

        request_rectangle = {
            {"time", 180000.0},
            {"form_target", {{"detection_point", {0, 0}}, {"root-mean-square_error", 99999}, {"courses", {360, 360}}, {"velocities", {{"current", 99.99}, {"max", 99.99}, {"min_noise-reduced", 99.99}, {"max_noise-reduced", 99.99}}}, {"obsolescence_time", 999999}, {"avoidance_distance", 10000}}},
            {"search_region", {{"borders", {{{0, 0}, {0, 30000}, {55000, 30000}, {55000, 0}, {0, 0}}}}, {"entry_point", {0, 12000}}, {"exit_point", {55000, 12000}}}},
            {"border_line", {{0, 0}, {10000, 10000}, {20000, 20000}}},
            {"ships_parameters", {{"count", 99}, {"detection_range", 6000}, {"max_velocity", 9}, {"search_velocity", 6}, {"turning_radius", 400}, {"min_lenght_section", 200}}}};

        request_zigzag_rectangle_sloped = {
            {"time", 180000},
            {"form_target", {{"detection_point", {0, 0}}, {"root-mean-square_error", 99999}, {"courses", {360, 360}}, {"velocities", {{"current", 99.99}, {"max", 99.99}, {"min_noise-reduced", 99.99}, {"max_noise-reduced", 99.99}}}, {"obsolescence_time", 999999}, {"avoidance_distance", 10000}}},
            {"search_region", {{"borders", {{{-32005.3361985711, -25214.3937600496}, {-31160.5435120901, 25227.0013002307}, {30340.8061712153, 25221.6656113184}, {31172.9604263196, -25220.6846885126}, {-32005.3361985711, -25214.3937600496}}, {{-13211.5081638632, -9791.11322013737}, {12377.18039422075, -9794.758885906}, {12170.3353688776, 9479.9499885496}, {-12998.60753279395, 9482.3564027351}, {-13211.5081638632, -9791.11322013737}}}}, {"entry_point", {-32005.3361985711, -25214.3937600496}}, {"exit_point", {30340.8061712153, 25221.6656113184}}}},
            {"border_line", {{0, 0}, {10000, 10000}, {20000, 20000}}},
            {"ships_parameters",
             {{"count", 99}, {"detection_range", 6000}, {"max_velocity", 9}, {"search_velocity", 6}, {"turning_radius", 400}, {"min_lenght_section", 200}}}};

        request_square = {
            {"time", 180000.0},
            {"form_target", {{"detection_point", {0, 0}}, {"root-mean-square_error", 99999}, {"courses", {360, 360}}, {"velocities", {{"current", 99.99}, {"max", 99.99}, {"min_noise-reduced", 99.99}, {"max_noise-reduced", 99.99}}}, {"obsolescence_time", 999999}, {"avoidance_distance", 10000}}},
            {"search_region", {{"borders", {{{0, 0}, {0, 50000}, {50000, 50000}, {50000, 0}, {0, 0}}}}, {"entry_point", {0, 0}}, {"exit_point", {50000, 50000}}}},
            {"border_line", {{0, 0}, {10000, 10000}, {20000, 20000}}},
            {"ships_parameters", {{"count", 99}, {"detection_range", 6000}, {"max_velocity", 9}, {"search_velocity", 6}, {"turning_radius", 400}, {"min_lenght_section", 200}}}};

        request_rectangle_inners = {
            {"time", 180000.0},
            {"form_target", {{"detection_point", {0, 0}}, {"root-mean-square_error", 99999}, {"courses", {360, 360}}, {"velocities", {{"current", 99.99}, {"max", 99.99}, {"min_noise-reduced", 99.99}, {"max_noise-reduced", 99.99}}}, {"obsolescence_time", 999999}, {"avoidance_distance", 10000}}},
            {"search_region", {{"borders", {{{0, 0}, {0, 30000}, {55000, 30000}, {55000, 0}, {0, 0}}, {{7000, 9000}, {7000, 21000}, {28000, 21000}, {28000, 9000}, {7000, 9000}}}}, {"entry_point", {0, 12000}}, {"exit_point", {55000, 12000}}}},
            {"border_line", {{0, 0}, {10000, 10000}, {20000, 20000}}},
            {"ships_parameters", {{"count", 99}, {"detection_range", 6000}, {"max_velocity", 9}, {"search_velocity", 6}, {"turning_radius", 400}, {"min_lenght_section", 200}}}};

        request_irregular = {
            {"time", 180000.0},
            {"form_target", {{"detection_point", {0, 0}}, {"root-mean-square_error", 99999}, {"courses", {360, 360}}, {"velocities", {{"current", 99.99}, {"max", 99.99}, {"min_noise-reduced", 99.99}, {"max_noise-reduced", 99.99}}}, {"obsolescence_time", 999999}, {"avoidance_distance", 10000}}},
            {"search_region", {{"borders", {{{0, 0}, {-10000, 40000}, {25000, 30000}, {60000, 30000}, {50000, 5000}, {25000, 0}, {0, 0}}}}, {"entry_point", {-5000, 20000}}, {"exit_point", {60000, 30000}}}},
            {"border_line", {{0, 0}, {10000, 10000}, {20000, 20000}}},
            {"ships_parameters", {{"count", 99}, {"detection_range", 6000}, {"max_velocity", 9}, {"search_velocity", 6}, {"turning_radius", 400}, {"min_lenght_section", 200}}}};

        request_concave = {
            {"time", 180000.0},
            {"form_target", {{"detection_point", {0, 0}}, {"root-mean-square_error", 99999}, {"courses", {360, 360}}, {"velocities", {{"current", 99.99}, {"max", 99.99}, {"min_noise-reduced", 99.99}, {"max_noise-reduced", 99.99}}}, {"obsolescence_time", 999999}, {"avoidance_distance", 10000}}},
            {"search_region", {{"borders", {{{0, 0}, {0, 35000}, {15000, 35000}, {25000, 25000}, {32000, 5000}, {37000, 25000}, {42000, 35000}, {60000, 35000}, {60000, 0}, {20000, -5000}, {0, 0}}}}, {"entry_point", {0, 12000}}, {"exit_point", {60000, 12000}}}},
            {"border_line", {{0, 0}, {10000, 10000}, {20000, 20000}}},
            {"ships_parameters", {{"count", 99}, {"detection_range", 6000}, {"max_velocity", 9}, {"search_velocity", 6}, {"turning_radius", 400}, {"min_lenght_section", 200}}}};

        request_shape_z = {
            {"time", 180000.0},
            {"form_target", {{"detection_point", {0, 0}}, {"root-mean-square_error", 99999}, {"courses", {360, 360}}, {"velocities", {{"current", 99.99}, {"max", 99.99}, {"min_noise-reduced", 99.99}, {"max_noise-reduced", 99.99}}}, {"obsolescence_time", 999999}, {"avoidance_distance", 10000}}},
            {"search_region", {{"borders", {{{0, 20000}, {0, 40000}, {45000, 40000}, {45000, 20000}, {70000, 20000}, {70000, 0}, {25000, 0}, {25000, 20000}, {0, 20000}}}}, {"entry_point", {25000, 10000}}, {"exit_point", {0, 30000}}}},
            {"border_line", {{0, 0}, {10000, 10000}, {20000, 20000}}},
            {"ships_parameters", {{"count", 99}, {"detection_range", 6000}, {"max_velocity", 9}, {"search_velocity", 6}, {"turning_radius", 400}, {"min_lenght_section", 200}}}};

        request_numerous = {
            {"time", 180000.0},
            {"form_target", {{"detection_point", {0, 0}}, {"root-mean-square_error", 99999}, {"courses", {360, 360}}, {"velocities", {{"current", 99.99}, {"max", 99.99}, {"min_noise-reduced", 99.99}, {"max_noise-reduced", 99.99}}}, {"obsolescence_time", 999999}, {"avoidance_distance", 10000}}},
            {"search_region", {{"borders", {{{2000, 4000}, {0, 8000}, {-2000, 14000}, {-2000, 20000}, {0, 26000}, {2000, 28000}, {6000, 30000}, {14000, 32000}, {22000, 32000}, {28000, 30000}, {32000, 26000}, {34000, 22000}, {34000, 18000}, {32000, 12000}, {28000, 6000}, {24000, 2000}, {16000, -2000}, {14000, -2000}, {8000, 0}, {2000, 4000}}}}, {"entry_point", {18000, 32000}}, {"exit_point", {2000, 4000}}}},
            {"border_line", {{0, 0}, {1000, 1000}, {2000, 2000}}},
            {"ships_parameters", {{"count", 99}, {"detection_range", 3000}, {"max_velocity", 9}, {"search_velocity", 6}, {"turning_radius", 400}, {"min_lenght_section", 200}}}};
        // добавление внутреннего полигона
        //        request_numerous_var["search_region"]["borders"].push_back({{12000, 12000}, {12000, 18000}, {14000, 20000}, {18000, 20000}, {20000, 14000}, {16000, 12000}, {12000, 12000}});

        request_trapezoid = {
            {"time", 180000.0},
            {"form_target", {{"detection_point", {0, 0}}, {"root-mean-square_error", 99999}, {"courses", {360, 360}}, {"velocities", {{"current", 99.99}, {"max", 99.99}, {"min_noise-reduced", 99.99}, {"max_noise-reduced", 99.99}}}, {"obsolescence_time", 999999}, {"avoidance_distance", 10000}}},
            {"search_region", {{"borders", {{{0, 0}, {5000, 10000}, {55000, 10000}, {55000, 0}, {0, 0}}}}, {"entry_point", {250, 500}}, {"exit_point", {55000, 500}}}},
            {"border_line", {{0, 0}, {1000, 1000}, {2000, 2000}}},
            {"ships_parameters", {{"count", 99}, {"detection_range", 5100}, {"max_velocity", 9}, {"search_velocity", 6}, {"turning_radius", 400}, {"min_lenght_section", 200}}}};

        task = SearchTask{request_zigzag_base};

        path = "report_zigzag_base.json";
        path_zigzag = "data/zigzag/";
        path_shift = "data/shift/";
        path_straight_tack = "data/straight_tack/";

        path_rectangle_inners = "data/rectangle_inners/";
        path_concave = "data/concave/";
        path_irregular = "data/irregular/";
        path_shape_z = "data/shape_z/";
        path_numerous = "data/numerous/";
        path_trapezoid = "data/trapezoid/";
    }
};

const std::map<SearchScheme, std::string> scheme = {{SearchScheme::Zigzag, "Зигзаг"},
                                                    {SearchScheme::Shift, "Шифт"},
                                                    {SearchScheme::DeterminedShift, "Д. Шифт"}};

TEST_F(SearchTaskInRegionTest, computeRoute_rectangle_inners)
{
    task = SearchTask{request_rectangle_inners};

    PrimaryEntities::Polygon<Point2D> polygon;
    setPolygonFromJson(polygon, request_rectangle_inners);

    auto result_zigzag = task.computeRoute(SearchScheme::Zigzag);
    auto result_shift = task.computeRoute(SearchScheme::Shift);
    auto result_shiftD = task.computeRoute(SearchScheme::DeterminedShift);

    toFile(path_rectangle_inners + "request.json", request_rectangle_inners);
    toFile(path_rectangle_inners + "report_zigzag.json", result_zigzag);
    toFile(path_rectangle_inners + "report_shift.json", result_shift);
    toFile(path_rectangle_inners + "report_shiftD.json", result_shiftD);

    EXPECT_FALSE(result_zigzag["routes"].empty());
    EXPECT_FALSE(result_shift["routes"].empty());
    EXPECT_FALSE(result_shiftD["routes"].empty());
    EXPECT_EQ(result_zigzag["messages"][0]["type"], "Info");
    EXPECT_EQ(result_shift["messages"][0]["type"], "Info");
    EXPECT_EQ(result_shiftD["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskInRegionTest, computeRoute_concave)
{
    task = SearchTask{request_concave};

    PrimaryEntities::Polygon<Point2D> polygon;
    setPolygonFromJson(polygon, request_concave);

    auto result_zigzag = task.computeRoute(SearchScheme::Zigzag);
    auto result_shift = task.computeRoute(SearchScheme::Shift);
    auto result_shiftD = task.computeRoute(SearchScheme::DeterminedShift);

    toFile(path_concave + "request.json", request_concave);
    toFile(path_concave + "report_zigzag.json", result_zigzag);
    toFile(path_concave + "report_shift.json", result_shift);
    toFile(path_concave + "report_shiftD.json", result_shiftD);

    EXPECT_FALSE(result_zigzag["routes"].empty());
    EXPECT_FALSE(result_shift["routes"].empty());
    EXPECT_FALSE(result_shiftD["routes"].empty());
    EXPECT_EQ(result_zigzag["messages"][0]["type"], "Info");
    EXPECT_EQ(result_shift["messages"][0]["type"], "Info");
    EXPECT_EQ(result_shiftD["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskInRegionTest, computeRoute_irregular)
{
    task = SearchTask{request_irregular};

    PrimaryEntities::Polygon<Point2D> polygon;
    setPolygonFromJson(polygon, request_irregular);

    auto result_zigzag = task.computeRoute(SearchScheme::Zigzag);
    auto result_shift = task.computeRoute(SearchScheme::Shift);
    auto result_shiftD = task.computeRoute(SearchScheme::DeterminedShift);

    toFile(path_irregular + "request.json", request_irregular);
    toFile(path_irregular + "report_zigzag.json", result_zigzag);
    toFile(path_irregular + "report_shift.json", result_shift);
    toFile(path_irregular + "report_shiftD.json", result_shiftD);

    EXPECT_FALSE(result_zigzag["routes"].empty());
    EXPECT_FALSE(result_shift["routes"].empty());
    EXPECT_FALSE(result_shiftD["routes"].empty());
    EXPECT_EQ(result_zigzag["messages"][0]["type"], "Info");
    EXPECT_EQ(result_shift["messages"][0]["type"], "Info");
    EXPECT_EQ(result_shiftD["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskInRegionTest, computeRoute_shape_z)
{
    task = SearchTask{request_shape_z};

    PrimaryEntities::Polygon<Point2D> polygon;
    setPolygonFromJson(polygon, request_shape_z);

    auto result_zigzag = task.computeRoute(SearchScheme::Zigzag);
    auto result_shift = task.computeRoute(SearchScheme::Shift);
    auto result_shiftD = task.computeRoute(SearchScheme::DeterminedShift);

    toFile(path_shape_z + "request.json", request_shape_z);
    toFile(path_shape_z + "report_zigzag.json", result_zigzag);
    toFile(path_shape_z + "report_shift.json", result_shift);
    toFile(path_shape_z + "report_shiftD.json", result_shiftD);

    EXPECT_FALSE(result_zigzag["routes"].empty());
    EXPECT_FALSE(result_shift["routes"].empty());
    EXPECT_FALSE(result_shiftD["routes"].empty());
    EXPECT_EQ(result_zigzag["messages"][0]["type"], "Info");
    EXPECT_EQ(result_shift["messages"][0]["type"], "Info");
    EXPECT_EQ(result_shiftD["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskInRegionTest, computeRoute_numerous)
{
    task = SearchTask{request_numerous};

    PrimaryEntities::Polygon<Point2D> polygon;
    setPolygonFromJson(polygon, request_numerous);

    auto result_zigzag = task.computeRoute(SearchScheme::Zigzag);
    auto result_shift = task.computeRoute(SearchScheme::Shift);
    auto result_shiftD = task.computeRoute(SearchScheme::DeterminedShift);

    toFile(path_numerous + "request.json", request_numerous);
    toFile(path_numerous + "report_zigzag.json", result_zigzag);
    toFile(path_numerous + "report_shift.json", result_shift);
    toFile(path_numerous + "report_shiftD.json", result_shiftD);

    EXPECT_FALSE(result_zigzag["routes"].empty());
    EXPECT_FALSE(result_shift["routes"].empty());
    EXPECT_FALSE(result_shiftD["routes"].empty());
    EXPECT_EQ(result_zigzag["messages"][0]["type"], "Info");
    EXPECT_EQ(result_shift["messages"][0]["type"], "Info");
    EXPECT_EQ(result_shiftD["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskInRegionTest, computeRoute_trapezoid)
{
    task = SearchTask{request_trapezoid};

    PrimaryEntities::Polygon<Point2D> polygon;
    setPolygonFromJson(polygon, request_trapezoid);

    auto result_zigzag = task.computeRoute(SearchScheme::Zigzag);
    auto result_shift = task.computeRoute(SearchScheme::Shift);
    auto result_shiftD = task.computeRoute(SearchScheme::DeterminedShift);
    auto result_straight = task.computeRoute(SearchScheme::StraightTackRegion);

    toFile(path_trapezoid + "request.json", request_trapezoid);
    toFile(path_trapezoid + "report_zigzag.json", result_zigzag);
    toFile(path_trapezoid + "report_shift.json", result_shift);
    toFile(path_trapezoid + "report_shiftD.json", result_shiftD);
    toFile(path_trapezoid + "report_straight.json", result_straight);

    EXPECT_FALSE(result_zigzag["routes"].empty());
    EXPECT_FALSE(result_shift["routes"].empty());
    EXPECT_FALSE(result_shiftD["routes"].empty());
    EXPECT_FALSE(result_straight["routes"].empty());
    EXPECT_EQ(result_zigzag["messages"][0]["type"], "Info");
    EXPECT_EQ(result_shift["messages"][0]["type"], "Info");
    EXPECT_EQ(result_shiftD["messages"][0]["type"], "Info");
    EXPECT_EQ(result_straight["messages"][0]["type"], "Info");
}

namespace // Shift
{
TEST_F(SearchTaskInRegionTest, computeRoute_Shift_rectangle)
{
    task = SearchTask{request_rectangle};
    auto result = task.computeRoute(SearchScheme::Shift);

    toFile(path_shift + "request_base.json", request_rectangle);
    toFile(path_shift + "report_base.json", result);

    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskInRegionTest, computeRoute_Shift_VertexDoors)
{
    request_rectangle["search_region"]["entry_point"] = {0, 0};
    request_rectangle["search_region"]["exit_point"] = {55000, 30000};
    task = SearchTask{request_rectangle};

    auto result = task.computeRoute(SearchScheme::Shift);

    toFile("report_shift_vertex_doors.json", result);

    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskInRegionTest, computeRoute_Shift_VertexDoorsVar_SlopedPolygon)
{
    request_rectangle["search_region"] = {{"borders", {{{0, 5000}, {5000, 40000}, {50000, 45000}, {40000, 0}, {0, 5000}}}}, {"entry_point", {50000, 45000}}, {"exit_point", {50000, 45000}}};
    task = SearchTask{request_rectangle};

    auto result = task.computeRoute(SearchScheme::Shift);

    toFile("request_shift_polygon_var.json", request_rectangle);
    toFile("report_shift_vertex_doors_polygon_var.json", result);

    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskInRegionTest, Shift_MinInner)
{
    auto innerRing = nlohmann::json::array({{20000, 20000}, {20000, 30000}, {30000, 30000}, {30000, 20000}, {20000, 20000}});
    request_square["search_region"]["borders"].push_back(innerRing);
    task = SearchTask{request_square};

    auto result = task.computeRoute(SearchScheme::Shift);

    toFile(path_shift + "request_min_inner.json", request_square);
    toFile(path_shift + "report_min_inner.json", result);

    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskInRegionTest, Shift_MidInner)
{
    auto innerRing = nlohmann::json::array({{15000, 15000}, {15000, 35000}, {35000, 35000}, {35000, 15000}, {15000, 15000}});
    request_square["search_region"]["borders"].push_back(innerRing);
    task = SearchTask{request_square};

    auto result = task.computeRoute(SearchScheme::Shift);

    toFile(path_shift + "request_mid_inner.json", request_square);
    toFile(path_shift + "report_mid_inner.json", result);

    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskInRegionTest, Shift_MaxInner)
{
    auto innerRing = nlohmann::json::array({{10000, 10000}, {10000, 40000}, {40000, 40000}, {40000, 10000}, {10000, 10000}});
    request_square["search_region"]["borders"].push_back(innerRing);
    task = SearchTask{request_square};

    auto result = task.computeRoute(SearchScheme::Shift);

    toFile(path_shift + "request_max_inner.json", request_square);
    toFile(path_shift + "report_max_inner.json", result);

    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

// DeterminedShift
TEST_F(SearchTaskInRegionTest, computeRoute_ShiftD_rectangle)
{
    request_rectangle["time"] = 999000;
    task = SearchTask{request_rectangle};

    auto result = task.computeRoute(SearchScheme::DeterminedShift);

    //  toFile(path_shift + "request_shiftD_base.json", request_rectangle);
    toFile(path_shift + "report_shiftD_base.json", result);

    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}
} // namespace

namespace // Zigzag
{
TEST_F(SearchTaskInRegionTest, computeRoute_Zigzag_base)
{
    auto result = task.computeRoute(SearchScheme::Zigzag);

    toFile(path_zigzag + "request_base.json", request_zigzag_base);
    toFile(path_zigzag + "report_base.json", result);

    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskInRegionTest, computeRoute_Zigzag_rectangleSloped)
{
    task.setTask(request_zigzag_rectangle_sloped);
    auto result = task.computeRoute(SearchScheme::Zigzag);

    toFile(path_zigzag + "request_rectangle_sloped.json", request_zigzag_rectangle_sloped);
    toFile(path_zigzag + "report_rectangle_sloped.json", result);

    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskInRegionTest, computeRoute_Zigzag_EntranceVariation)
{
    request_zigzag_base["search_region"]["entry_point"] = {24000, 0};
    task.setTask(request_zigzag_base);

    auto result = task.computeRoute(SearchScheme::Zigzag);

    ASSERT_TRUE(toFile("report_zigzag_entrance_variation.json", result));

    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskInRegionTest, computeRoute_Zigzag_ExitVariation)
{
    request_zigzag_base["search_region"]["exit_point"] = {24000, 0};
    task.setTask(request_zigzag_base);

    auto result = task.computeRoute(SearchScheme::Zigzag);

    ASSERT_TRUE(toFile("report_zigzag_exit_variation.json", result));

    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskInRegionTest, computeRoute_Zigzag_InnersVariation)
{
    request_zigzag_base["search_region"]["borders"] = {{{0, 0}, {0, 40000}, {40000, 40000}, {40000, 8000}, {50000, 40000}, {50000, 0}, {0, 0}}, {{10000, 14000}, {10000, 24000}, {30000, 24000}, {30000, 14000}, {10000, 14000}}};
    request_zigzag_base["search_region"]["exit_point"] = {24000, 0};
    task.setTask(request_zigzag_base);

    auto result = task.computeRoute(SearchScheme::Zigzag);

    ASSERT_TRUE(toFile("report_zigzag_inners_variation.json", result));

    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskInRegionTest, computeRoute_Zigzag_EntranceVertex)
{
    request_zigzag_base["search_region"]["entry_point"] = {50000, 40000};
    task.setTask(request_zigzag_base);

    auto result = task.computeRoute(SearchScheme::Zigzag);

    toFile("report_zigzag_entrance_vertex.json", result);

    EXPECT_FALSE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskInRegionTest, computeRoute_Zigzag_InvalidInput)
{
    request_nautical.erase("time");
    task.setTask(request_nautical);

    auto result = task.computeRoute(SearchScheme::Zigzag);

    EXPECT_TRUE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Error");
}

TEST_F(SearchTaskInRegionTest, computeRoute_Zigzag_PolygonVariationFromGeojson)
{
    request_zigzag_base["time"] = 360000;
    request_zigzag_base["search_region"]["borders"] = {{{-56005.3361985711, -55214.3937600496}, {-55160.5435120901, 55227.0013002307}, {54340.8061712153, 55221.6656113184}, {55172.9604263196, -55220.6846885126}, {-56005.3361985711, -55214.3937600496}}, {{-28211.5081638632, -27791.11322013737}, {27377.18039422075, -27794.758885906}, {27170.3353688776, 27479.9499885496}, {-27998.60753279395, 27482.3564027351}, {-28211.5081638632, -27791.11322013737}}};

    request_zigzag_base["search_region"]["entry_point"] = {-56005.3361985711, -55214.3937600496};
    request_zigzag_base["search_region"]["exit_point"] = {54340.8061712153, 55221.6656113184};

    task.setTask(request_zigzag_base);

    auto result = task.computeRoute(SearchScheme::Zigzag);

    toFile("request_zigzag_geojson_polygon.json", request_zigzag_base);
    toFile("report_zigzag_geojson_polygon.json", result);

    ASSERT_FALSE(result["routes"].empty());
}

/// @todo ASSERT_THROW

//TEST_F(SearchTaskTest, computeRoute_PolygonVariationSmaller)
//{
//    nlohmann::json smaller_region = request_zigzag;
//
//    smaller_region["search_region"]["borders"] = {{{-2782.9877073585326,-2764.356817414998},{-2782.986654769348,2764.356812313416},{2782.985603113636,2764.3568123132177},{2782.9866557024216,-2764.3568174151974},{-2782.9877073585326,-2764.356817414998}},
//        {{-1669.7928451458047,-1658.6142968848578},{1669.7917916461079,-1658.6142968849774},{1669.791412714178,1658.6139486136863},{-1669.792466213635,1658.6139486138063},{-1669.7928451458047,-1658.6142968848578}}};
//
//      smaller_region["search_region"]["entry_point"] = {-2782.987665254963,-1658.6141284705602};
//      smaller_region["search_region"]["exit_point"] = {2782.985982045584,1658.6141170278665};
//
//    task.setTask(smaller_region);
//
//    auto result = task.computeRoute(SearchScheme::Zigzag);
//
//    Operations::toFile("report_smaller_region.json", result);
//
//    ASSERT_FALSE(result["routes"].empty());
//}

TEST_F(SearchTaskInRegionTest, computeRoute_Zigzag_PolygonVariationBigger)
{
    request_zigzag_base["time"] = 900000;

    request_zigzag_base["search_region"]["borders"] = {{{-560053.3361985711, -552142.3937600496}, {-551600.5435120901, 552277.0013002307}, {543401.8061712153, 552214.6656113184}, {551728.9604263196, -552205.6846885126}, {-560053.3361985711, -552142.3937600496}}, {{-282117.5081638632, -277912.11322013737}, {273777.18039422075, -277943.758885906}, {271707.3353688776, 274793.9499885496}, {-279984.60753279395, 274825.3564027351}, {-282117.5081638632, -277912.11322013737}}};

    request_zigzag_base["search_region"]["entry_point"] = {-560053.3361985711, -552142.3937600496};

    request_zigzag_base["search_region"]["exit_point"] = {543401.8061712153, 552214.6656113184};

    task.setTask(request_zigzag_base);
    auto result = task.computeRoute(SearchScheme::Zigzag);

    toFile("report_zigzag_variation_bigger_poly.json", result);

    ASSERT_FALSE(result["routes"].empty());
}

TEST_F(SearchTaskInRegionTest, computeRoute_Zigzag_FromAns)
{
    request_zigzag_base["search_region"]["borders"] = {{{-10716.020087560639, 10226.79643592704}, {16070.658381502144, 3573.912757185288}, {13187.36916116532, -7768.42884708615}, {-13786.492786057759, -8195.346820888575}, {-10716.020087560639, 10226.79643592704}}};
    request_zigzag_base["search_region"]["entry_point"] = {1479.784554488957, 7183.266851938795};
    request_zigzag_base["search_region"]["exit_point"] = {-12251.851853407454, 1005.6134254708886};

    task.setTask(request_zigzag_base);

    auto result = task.computeRoute(SearchScheme::Zigzag);

    toFile("report_zigzag_ans_input.json", result);

    ASSERT_FALSE(result["routes"].empty());
}

TEST_F(SearchTaskInRegionTest, computeRoute_Zigzag_04_21)
{
    request_zigzag_base["search_region"]["borders"] = {
        {{-33314.915744409881881, 34493.871912075730506},
         {-17549.641688913186954, 34091.696553517140273},
         {-20043.128911976440577, 51626.54218667162786},
         {-33556.220959545033111, 51385.236971536476631},
         {-33314.915744409881881, 34493.871912075730506}},
        {{-29051.856943688835599, 37630.839708832725591},
         {-19801.823696841289348, 35056.917414057759743},
         {-22697.486278463125927, 42698.249226670952339},
         {-30017.077804229447793, 43100.424585229542572},
         {-29051.856943688835599, 37630.839708832725591}}};
    request_zigzag_base["search_region"]["entry_point"] = {-33314.915744409881881, 34493.871912075730506};
    request_zigzag_base["search_region"]["exit_point"] = {-33314.915744409881881, 34493.871912075730506};

    task.setTask(request_zigzag_base);
    nlohmann::json result;
    try
    {
        result = task.computeRoute(SearchScheme::Zigzag);
    }
    catch (ExceptionTools::Failure& f)
    {
        std::cout << f.getCode() << '\t' << f.what() << std::endl;
        FAIL();
    }

    toFile("report_zigzag_04_21.json", result);

    ASSERT_FALSE(result["routes"].empty());
}

// inflation
/*TEST_F(SearchTaskTest, figure)
{
    Polygon2D polygon;
    polygon.outer() = {{{50000, 20000}, {50000, 30000}, {40000, 30000}, {50000, 40000}, {40000, 40000}, {50000, 50000}, {40000, 50000}, {60000, 70000},
                        {80000, 50000}, {70000, 50000}, {80000, 40000}, {70000, 40000}, {80000, 30000}, {70000, 30000}, {70000, 20000}, {50000, 20000}}};
    nlohmann::json figure = {
    {"time", 180000.0},
    {"form_target", {{"detection_point", {0, 0}}, {"root-mean-square_error", 99999}, {"courses", {360, 360}}, {"velocities", {{"current", 99.99}, {"max", 99.99}, {"min_noise-reduced", 99.99}, {"max_noise-reduced", 99.99}}}, {"obsolescence_time", 999999}, {"avoidance_distance", 10000}}},
        {"search_region", {{"borders", {}}, {"entry_point", {50000, 20000}}, {"exit_point", {60000, 70000}}}},
    {"border_line", {{0, 0}, {1000, 1000}, {2000, 2000}}},
    {"ships_parameters", {{"count", 99}, {"detection_range", 5000}, {"max_velocity", 9}, {"search_velocity", 6}, {"turning_radius", 400}, {"min_lenght_section", 200}}}};

    auto innerRing = nlohmann::json::array(
    {nlohmann::json::array({polygon.outer()[0].getX(), polygon.outer()[0].getY()}),
     nlohmann::json::array({polygon.outer()[1].getX(), polygon.outer()[1].getY()}),
     nlohmann::json::array({polygon.outer()[2].getX(), polygon.outer()[2].getY()}),
     nlohmann::json::array({polygon.outer()[3].getX(), polygon.outer()[3].getY()}),
     nlohmann::json::array({polygon.outer()[4].getX(), polygon.outer()[4].getY()}),
     nlohmann::json::array({polygon.outer()[5].getX(), polygon.outer()[5].getY()}),
     nlohmann::json::array({polygon.outer()[6].getX(), polygon.outer()[6].getY()}),
     nlohmann::json::array({polygon.outer()[7].getX(), polygon.outer()[7].getY()}),
     nlohmann::json::array({polygon.outer()[8].getX(), polygon.outer()[8].getY()}),
     nlohmann::json::array({polygon.outer()[9].getX(), polygon.outer()[9].getY()}),
     nlohmann::json::array({polygon.outer()[10].getX(), polygon.outer()[10].getY()}),
     nlohmann::json::array({polygon.outer()[11].getX(), polygon.outer()[11].getY()}),
     nlohmann::json::array({polygon.outer()[12].getX(), polygon.outer()[12].getY()}),
     nlohmann::json::array({polygon.outer()[13].getX(), polygon.outer()[13].getY()}),
     nlohmann::json::array({polygon.outer()[14].getX(), polygon.outer()[14].getY()}),
     nlohmann::json::array({polygon.outer()[15].getX(), polygon.outer()[15].getY()}),});

    figure["search_region"]["borders"].push_back(innerRing);

    Operations::toFile("figure.json", figure);
}*/

TEST_F(SearchTaskInRegionTest, figureInflate)
{
    //    Polygon2D polygon;
    MultiPolygon2D multiPoly;
    Ring2D target;
    const Ring2D source = {{{20000, 20000}, {50000, 40000}, {80000, 40000}, {80000, 20000}, {20000, 20000}}};
    nlohmann::json figureInflate = {
        {"time", 180000.0},
        {"form_target", {{"detection_point", {0, 0}}, {"root-mean-square_error", 99999}, {"courses", {360, 360}}, {"velocities", {{"current", 99.99}, {"max", 99.99}, {"min_noise-reduced", 99.99}, {"max_noise-reduced", 99.99}}}, {"obsolescence_time", 999999}, {"avoidance_distance", 10000}}},
        {"search_region", {{"borders", {}}, {"entry_point", {20000, 20000}}, {"exit_point", {80000, 40000}}}},
        {"border_line", {{0, 0}, {1000, 1000}, {2000, 2000}}},
        {"ships_parameters", {{"count", 99}, {"detection_range", 5000}, {"max_velocity", 9}, {"search_velocity", 6}, {"turning_radius", 400}, {"min_lenght_section", 200}}}};
    auto innerRing = nlohmann::json::array({{20000, 20000}, {50000, 40000}, {80000, 40000}, {80000, 20000}, {20000, 20000}});

    // CAP_FLAT, JOIN_MITRE

    AbstractOperations::Helper::buffer(
        source, multiPoly, 1000, geos::operation::buffer::BufferParameters::DEFAULT_QUADRANT_SEGMENTS, geos::operation::buffer::BufferParameters::CAP_FLAT, geos::operation::buffer::BufferParameters::JOIN_MITRE,
        5., // значение по-умолчанию, взятое из geos (DEFAULT_MITRE_LIMIT)
        true,
        true);
    //    std::swap(result[0].outer(), ring);
    target = multiPoly[0].outer();
    auto outerRing = nlohmann::json::array(
        {
            nlohmann::json::array({target[0].getX(), target[0].getY()}),
            nlohmann::json::array({target[1].getX(), target[1].getY()}),
            nlohmann::json::array({target[2].getX(), target[2].getY()}),
            nlohmann::json::array({target[3].getX(), target[3].getY()}),
            nlohmann::json::array({target[4].getX(), target[4].getY()}),
        });

    figureInflate["search_region"]["borders"].push_back(outerRing);
    figureInflate["search_region"]["borders"].push_back(innerRing);

    toFile("figureInflate_FLAT_MITRE.json", figureInflate);

    figureInflate["search_region"]["borders"].erase(figureInflate["search_region"]["borders"].begin());
    figureInflate["search_region"]["borders"].erase(figureInflate["search_region"]["borders"].begin());
    multiPoly[0].outer().clear();
    outerRing.clear();

    // CAP_SQUARE, JOIN_MITRE

    AbstractOperations::Helper::buffer(
        source, multiPoly, 1000, geos::operation::buffer::BufferParameters::DEFAULT_QUADRANT_SEGMENTS, geos::operation::buffer::BufferParameters::CAP_SQUARE, geos::operation::buffer::BufferParameters::JOIN_MITRE,
        5., // значение по-умолчанию, взятое из geos (DEFAULT_MITRE_LIMIT)
        true,
        true);
    //    std::swap(multiPoly[0].outer(), source);
    target = multiPoly[0].outer();

    outerRing = nlohmann::json::array(
        {
            nlohmann::json::array({target[0].getX(), target[0].getY()}),
            nlohmann::json::array({target[1].getX(), target[1].getY()}),
            nlohmann::json::array({target[2].getX(), target[2].getY()}),
            nlohmann::json::array({target[3].getX(), target[3].getY()}),
            nlohmann::json::array({target[4].getX(), target[4].getY()}),
        });
    figureInflate["search_region"]["borders"].push_back(outerRing);
    figureInflate["search_region"]["borders"].push_back(innerRing);

    toFile("figureInflate_SQUARE_MITRE.json", figureInflate);

    figureInflate["search_region"]["borders"].erase(figureInflate["search_region"]["borders"].begin());
    figureInflate["search_region"]["borders"].erase(figureInflate["search_region"]["borders"].begin());
    multiPoly[0].outer().clear();
    outerRing.clear();

    // CAP_FLAT, JOIN_BEVEL

    AbstractOperations::Helper::buffer(
        source, multiPoly, 1000, geos::operation::buffer::BufferParameters::DEFAULT_QUADRANT_SEGMENTS, geos::operation::buffer::BufferParameters::CAP_FLAT, geos::operation::buffer::BufferParameters::JOIN_BEVEL,
        5., // значение по-умолчанию, взятое из geos (DEFAULT_MITRE_LIMIT)
        true,
        true);
    //    std::swap(multiPoly[0].outer(), source);
    target = multiPoly[0].outer();

    outerRing = nlohmann::json::array(
        {
            nlohmann::json::array({target[0].getX(), target[0].getY()}),
            nlohmann::json::array({target[1].getX(), target[1].getY()}),
            nlohmann::json::array({target[2].getX(), target[2].getY()}),
            nlohmann::json::array({target[3].getX(), target[3].getY()}),
            nlohmann::json::array({target[4].getX(), target[4].getY()}),
            nlohmann::json::array({target[5].getX(), target[5].getY()}),
            nlohmann::json::array({target[6].getX(), target[6].getY()}),
            nlohmann::json::array({target[7].getX(), target[7].getY()}),
        });
    figureInflate["search_region"]["borders"].push_back(outerRing);
    figureInflate["search_region"]["borders"].push_back(innerRing);

    toFile("figureInflate_FLAT_BEVEL.json", figureInflate);

    figureInflate["search_region"]["borders"].erase(figureInflate["search_region"]["borders"].begin());
    figureInflate["search_region"]["borders"].erase(figureInflate["search_region"]["borders"].begin());
    multiPoly[0].outer().clear();
    outerRing.clear();

    // CAP_SQUARE, JOIN_BEVEL

    AbstractOperations::Helper::buffer(
        source, multiPoly, 1000, geos::operation::buffer::BufferParameters::DEFAULT_QUADRANT_SEGMENTS, geos::operation::buffer::BufferParameters::CAP_SQUARE, geos::operation::buffer::BufferParameters::JOIN_BEVEL,
        5., // значение по-умолчанию, взятое из geos (DEFAULT_MITRE_LIMIT)
        true,
        true);
    //    std::swap(multiPoly[0].outer(), source);
    target = multiPoly[0].outer();

    outerRing = nlohmann::json::array(
        {
            nlohmann::json::array({target[0].getX(), target[0].getY()}),
            nlohmann::json::array({target[1].getX(), target[1].getY()}),
            nlohmann::json::array({target[2].getX(), target[2].getY()}),
            nlohmann::json::array({target[3].getX(), target[3].getY()}),
            nlohmann::json::array({target[4].getX(), target[4].getY()}),
            nlohmann::json::array({target[5].getX(), target[5].getY()}),
            nlohmann::json::array({target[6].getX(), target[6].getY()}),
            nlohmann::json::array({target[7].getX(), target[7].getY()}),
        });
    figureInflate["search_region"]["borders"].push_back(outerRing);
    figureInflate["search_region"]["borders"].push_back(innerRing);

    toFile("figureInflate_SQUARE_BEVEL.json", figureInflate);
}

} // namespace

// StraightTackOnce
TEST_F(SearchTaskInRegionTest, calculate_StraightTackOnce)
{
    Schemes::Search::InRegion::Input input;
    Polygon2D polygon = {{{0, 0}, {0, 10000}, {25000, 10000}, {25000, 0}, {0, 0}}};
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({500, 0});
    input.region.setExit({25000, 500});
    input.searchTime = 100;
    input.ship.detectionRange = 5100;
    input.ship.searchVelocity = 6;
    Schemes::Search::InRegion::StraightTackOnce straight(input);
    auto route = straight.calculate();

    nlohmann::json result;
    result["routes"].push_back(route.toJson());
    result["messages"].push_back(Outputs::Message{}.toJson());

    toFile(path_straight_tack + "request.json", request_trapezoid);
    toFile(path_straight_tack + "report_once.json", result);
}

TEST_F(SearchTaskInRegionTest, chooseScheme_InRegion_ValidInput)
{
    auto result = task.chooseScheme(SearchType::InRegion);

    EXPECT_FALSE(result["schemes_efficiencies"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Info");
}

TEST_F(SearchTaskInRegionTest, chooseScheme_InRegion_InvalidInput)
{
    request_nautical.erase("time");
    task = SearchTask{request_nautical};

    auto result = task.chooseScheme(SearchType::InRegion);

    EXPECT_TRUE(result["routes"].empty());
    EXPECT_EQ(result["messages"][0]["type"], "Error");
}
