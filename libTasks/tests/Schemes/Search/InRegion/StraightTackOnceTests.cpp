#include <Task/Exceptions/AlgorithmException.hpp>
#include <Task/Exceptions/ValidationException.hpp>
//#include <Task/Operations/UnitConverter.hpp>
#include <PrimaryEntities/Entities/Ring.hpp>
#include <Task/Schemes/Search/Config.hpp>
#include <Task/Schemes/Search/InRegion/StraightTackOnce.hpp>

#include <gtest/gtest.h>

using Input = Schemes::Search::InRegion::Input;
using Straight = Schemes::Search::InRegion::StraightTackOnce;
//using Region = Schemes::Search::InRegion::Region;
using namespace Operations;
using namespace PrimaryEntities;

TEST(straightOnce, calculate_HorizontalPoly)
{
    Polygon2D polygon = {{{0, 0}, {0, 3}, {3, 6}, {15, 6}, {15, 0}, {0, 0}}};
    Input input;
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({1, 0});
    input.region.setExit({15, 1});
    input.searchTime = 100;
    input.ship.detectionRange = 3;
    input.ship.searchVelocity = 6;
    Straight straight{input};

    Outputs::Route route = straight.calculate();
    EXPECT_EQ(route.points.size(), 4);

    std::vector<Point2D> standart = {{1, 0}, {3, 3}, {12, 3}, {15, 1}};
    EXPECT_EQ(route.points, standart);
}

TEST(straightOnce, calculate_Poly45)
{
    Polygon2D polygon = {{{4, 1}, {2, 3}, {7, 8}, {9, 6}, {4, 1}}};
    Input input;
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({4, 1});
    input.region.setExit({9, 6});
    input.searchTime = 100;
    input.ship.detectionRange = sqrt(2);
    input.ship.searchVelocity = 6;
    Straight straight{input};

    Outputs::Route route = straight.calculate();
    EXPECT_EQ(route.points.size(), 4);

    std::vector<Point2D> standart = {{4, 1}, {4, 3}, {7, 6}, {9, 6}};
    EXPECT_EQ(route.points, standart);
}

TEST(straightOnce, calculate_VerticalPoly)
{
    Polygon2D polygon = {{{1, 0}, {1, 10}, {5, 10}, {5, 0}, {1, 0}}};
    Input input;
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({2, 10});
    input.region.setExit({4, 0});
    input.searchTime = 100;
    input.ship.detectionRange = 2;
    input.ship.searchVelocity = 6;
    Straight straight{input};

    Outputs::Route route = straight.calculate();
    EXPECT_EQ(route.points.size(), 4);

    std::vector<Point2D> standart = {{2, 10}, {3, 8}, {3, 2}, {4, 0}};
    EXPECT_EQ(route.points, standart);
}

TEST(straightOnce, calculate_polyHome1)
{
    Polygon2D polygon = {{{0, 0}, {0, 4}, {5, 6}, {10, 4}, {10, 0}, {0, 0}}};
    Input input;
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({5, 0});
    input.region.setExit({5, 6});
    input.searchTime = 100;
    input.ship.detectionRange = 2;
    input.ship.searchVelocity = 6;
    Straight straight{input};

    Outputs::Route route = straight.calculate();
    EXPECT_EQ(route.points.size(), 4);

    std::vector<Point2D> standart = {{5, 0}, {2, 3}, {8, 3}, {5, 6}};
    EXPECT_EQ(route.points, standart);
}

TEST(straightOnce, calculate_PolyHome2)
{
    Polygon2D polygon = {{{0, 0}, {0, 1}, {5, 6}, {10, 1}, {10, 0}, {0, 0}}};
    Input input;
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({5, 0});
    input.region.setExit({5, 6});
    input.searchTime = 100;
    input.ship.detectionRange = 2;
    input.ship.searchVelocity = 6;
    Straight straight{input};

    Outputs::Route route = straight.calculate();
    EXPECT_EQ(route.points.size(), 4);

    std::vector<Point2D> standart = {{5, 0}, {2, 3}, {8, 3}, {5, 6}};
    EXPECT_EQ(route.points, standart);
}

TEST(straightOnce, calculate_trapezoid)
{
    Polygon2D polygon = {{{0, 0}, {3, 6}, {15, 6}, {15, 0}, {0, 0}}};
    Input input;
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({1, 0});
    input.region.setExit({15, 1});
    input.searchTime = 100;
    input.ship.detectionRange = 3;
    input.ship.searchVelocity = 6;
    Straight straight{input};

    Outputs::Route route = straight.calculate();
    EXPECT_EQ(route.points.size(), 4);

    std::vector<Point2D> standart = {{1, 0}, {3, 3}, {12, 3}, {15, 1}};
    EXPECT_EQ(route.points, standart);
}

TEST(straightOnce, calculate_parallelogram)
{
    Polygon2D polygon = {{{0, 0}, {3, 6}, {15, 6}, {14, 0}, {0, 0}}};
    Input input;
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({1, 0});
    input.region.setExit({13, 0});
    input.searchTime = 100;
    input.ship.detectionRange = 3;
    input.ship.searchVelocity = 6;
    Straight straight{input};

    Outputs::Route route = straight.calculate();
    EXPECT_EQ(route.points.size(), 4);

    std::vector<Point2D> standart = {{1, 0}, {3, 3}, {12, 3}, {13, 0}};
    EXPECT_EQ(route.points, standart);
}

TEST(straightOnce, calculate_VertexEntry)
{
    Polygon2D polygon = {{{2, 0}, {0, 2}, {2, 4}, {14, 4}, {16, 2}, {14, 0}, {2, 0}}};
    Input input;
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({0, 2});
    input.region.setExit({16, 2});
    input.searchTime = 100;
    input.ship.detectionRange = 2;
    input.ship.searchVelocity = 6;
    Straight straight{input};

    Outputs::Route route = straight.calculate();
    EXPECT_EQ(route.points.size(), 4);

    std::vector<Point2D> standart = {{0, 2}, {2, 2}, {14, 2}, {16, 2}};
    EXPECT_EQ(route.points, standart);
}
