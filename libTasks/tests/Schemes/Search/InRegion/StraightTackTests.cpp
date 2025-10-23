#include <PrimaryEntities/Entities/Ring.hpp>
#include <Task/Exceptions/AlgorithmException.hpp>
#include <Task/Exceptions/ValidationException.hpp>
#include <Task/Operations/UnitConverter.hpp>
#include <Task/Schemes/Search/Config.hpp>
#include <Task/Schemes/Search/InRegion/StraightTack.hpp>

#include <gtest/gtest.h>

using Input = Schemes::Search::InRegion::Input;
using StraightTackRepeated = Schemes::Search::InRegion::StraightTack;
//using Region = Schemes::Search::InRegion::Region;
using namespace Operations;
using namespace PrimaryEntities;

class straightTestFixture : public testing::Test
{
protected:
    Polygon2D polygon;

    void SetUp() override
    {
        polygon = {{{0, 0}, {0, 3000}, {3000, 6000}, {15000, 6000}, {15000, 0}, {0, 0}}};
    }
};

TEST_F(straightTestFixture, calculate_patrollingNumberOdd)
{
    Input input;
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({0, 1000});
    input.region.setExit({15000, 1000});
    input.searchTime = 9000;
    input.ship.detectionRange = 3000;
    input.ship.searchVelocity = 6;
    StraightTackRepeated straight{input};

    Outputs::Route route = straight.calculate();
    EXPECT_EQ(route.points.size(), 7);

    std::vector<Point2D> standart = {{0, 1000}, {500, 3000}, {14500, 3000}, {500, 3000}, {14500, 3000}, {9972.375785520322, 3000}, {15000, 1000}};
    EXPECT_EQ(route.points, standart);
}

TEST_F(straightTestFixture, calculate_patrollingNumberEven)
{
    Input input;
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({7000, 6000});
    input.region.setExit({2000, 0});
    input.searchTime = 7200;
    input.ship.detectionRange = 3000;
    input.ship.searchVelocity = 6;
    StraightTackRepeated straight{input};

    Outputs::Route route = straight.calculate();
    EXPECT_EQ(route.points.size(), 6);

    std::vector<Point2D> standart = {{7000, 6000}, {500, 3000}, {14500, 3000}, {500, 3000}, {4582.5859435596, 3000}, {2000, 0}};
    EXPECT_EQ(route.points, standart);
}

TEST_F(straightTestFixture, calculate_patrollingNumberOdd_SwapStartEnd)
{
    Input input;
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({15000, 1000});
    input.region.setExit({0, 1000});
    input.searchTime = 6000;
    input.ship.detectionRange = 3000;
    input.ship.searchVelocity = 6;
    StraightTackRepeated straight{input};

    Outputs::Route route = straight.calculate();
    EXPECT_EQ(route.points.size(), 5);

    std::vector<Point2D> standart = {{15000, 1000}, {14500, 3000}, {500, 3000}, {10121.368801512679, 3000}, {0, 1000}};
    EXPECT_EQ(route.points, standart);
}

TEST_F(straightTestFixture, calculate_patrollingNumberOdd_MoreTime)
{
    Input input;
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({0, 1000});
    input.region.setExit({15000, 1000});
    input.searchTime = 27000;
    input.ship.detectionRange = 3000;
    input.ship.searchVelocity = 6;
    StraightTackRepeated straight{input};

    Outputs::Route route = straight.calculate();
    EXPECT_EQ(route.points.size(), 15);

    std::vector<Point2D> standart = {{0, 1000}, {500, 3000}, {14500, 3000}, {500, 3000}, {14500, 3000}, {500, 3000}, {14500, 3000}, {500, 3000}, {14500, 3000}, {500, 3000}, {14500, 3000}, {500, 3000}, {14500, 3000}, {12091.4103126635, 3000}, {15000, 1000}};
    EXPECT_EQ(route.points, standart);
}
