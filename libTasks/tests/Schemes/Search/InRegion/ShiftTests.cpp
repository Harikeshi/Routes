#include <Task/Exceptions/AlgorithmException.hpp>
#include <Task/Operations/RouteOperations.hpp>
//#include <Task/Operations/UnitConverter.hpp>
#include <Task/Schemes/Search/InRegion/Shift.hpp>

#include <gtest/gtest.h>

using InputShift = Schemes::Search::InRegion::Input;
using Shift = Schemes::Search::InRegion::Shift;
using namespace Operations;

class ShiftTest : public ::testing::Test
{
protected:
    InputShift input;

    void SetUp() override
    {
        Polygon2D polygon = {{{0, 0}, {0, 40000}, {50000, 40000}, {50000, 0}, {0, 0}}};
        //        input.region.setPolygon(polygon);
        input.region.setFromPolygon(polygon);
        input.region.setEntrance({0, 12000});
        input.region.setExit({50000, 12000});
        input.searchTime = 180000.;
        input.ship.detectionRange = 6000.;
        input.ship.searchVelocity = 9;
        input.ship.searchVelocity = 6.;
    }

    void TearDown() override
    {
        // Код для очистки после каждого теста
    }
};

TEST_F(ShiftTest, checkTime)
{
    //    metricToNauticalInput(input);
    Shift shift{input};
    const double setTime = input.searchTime;
    Outputs::Route route;
    try
    {
        route = shift.calculate();
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        FAIL();
    }
    EXPECT_NEAR(routeDuration(route.points, route.velocities), setTime, ACCURACY_DOUBLE * setTime);
}

TEST_F(ShiftTest, checkTime_vertexDoors)
{
    Polygon2D polygon = {{{0, 0}, {0, 40000}, {50000, 40000}, {50000, 0}, {0, 0}}};
    //    input.region.setPolygon(polygon);
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({0, 0});
    input.region.setExit({50000, 40000});
    //    metricToNauticalInput(input);
    Shift shift{input};
    const double setTime = input.searchTime;
    Outputs::Route route;
    try
    {
        route = shift.calculate();
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        FAIL();
    }
    EXPECT_NEAR(routeDuration(route.points, route.velocities), setTime, ACCURACY_DOUBLE * setTime);
}

TEST_F(ShiftTest, calculate_no_throw)
{
    Polygon2D polygon = {{{0, 0}, {0, 40000}, {50000, 40000}, {50000, 0}, {0, 0}}, {{10000, 10000}, {20000, 10000}, {10000, 20000}}};

    input.region.setFromPolygon(polygon);
    input.region.setEntrance({0, 12000});
    input.region.setExit({50000, 12000});
    Shift shift{input};
    EXPECT_NO_THROW(shift.calculate());
}
