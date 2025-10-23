#include <Task/Exceptions/AlgorithmException.hpp>
#include <Task/Operations/RouteOperations.hpp>
//#include <Task/Operations/UnitConverter.hpp>
#include <Task/Schemes/Search/InRegion/DeterminedShift.hpp>

#include <PrimaryEntities/Entities/Ring.hpp>

#include <gtest/gtest.h>

using InputDeterminedShift = Schemes::Search::InRegion::Input;
using DeterminedShift = Schemes::Search::InRegion::DeterminedShift;
using namespace Operations;

class DeterminedShiftTest : public ::testing::Test
{
protected:
    InputDeterminedShift input;

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

TEST_F(DeterminedShiftTest, checkTime)
{
    //    metricToNauticalInput(input);
    DeterminedShift determinedShift{input};
    const double setTime = input.searchTime;
    Outputs::Route route;
    try
    {
        route = determinedShift.calculate();
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        FAIL();
    }
    EXPECT_NEAR(routeDuration(route.points, route.velocities), setTime, ACCURACY_DOUBLE * setTime);
}

TEST_F(DeterminedShiftTest, checkTime_vertexDoors)
{
    Polygon2D polygon = {{{0, 0}, {0, 40000}, {50000, 40000}, {50000, 0}, {0, 0}}};
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({0, 0});
    input.region.setExit({50000, 40000});
    //    metricToNauticalInput(input);
    DeterminedShift determinedShift{input};
    const double setTime = input.searchTime;
    Outputs::Route route;
    try
    {
        route = determinedShift.calculate();
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        FAIL();
    }
    EXPECT_NEAR(routeDuration(route.points, route.velocities), setTime, ACCURACY_DOUBLE * setTime);
}

TEST_F(DeterminedShiftTest, calculate_no_throw)
{
    Polygon2D polygon = {{{0, 0}, {0, 40000}, {50000, 40000}, {50000, 0}, {0, 0}}, {{10000, 10000}, {20000, 10000}, {10000, 20000}}};
    input.region.setFromPolygon(polygon);
    DeterminedShift determinedShift{input};
    EXPECT_NO_THROW(determinedShift.calculate());
}

TEST_F(DeterminedShiftTest, standardCalculate)
{
    Polygon2D polygon = {{{0, 0}, {0, 10000}, {10000, 10000}, {10000, 0}, {0, 0}}};
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({2000, 0});
    input.region.setExit({2000, 10000});
    input.searchTime = 5000;
    DeterminedShift determinedShift{input};
    auto result = determinedShift.calculate();
    //    std::vector<Point2D> standard = {{2000, 0}, {9500, 7499.999999999999}, {5350.087684383223, 3350.087684383221}, {500, 8200.175368766446}, {2501.7621695374364, 6198.4131992290095}, {2000, 10000}};
    std::vector<Point2D> standard = {{2000, 0}, {9500, 7499.999999999999}, {7499.999999999998, 9500.0}, {881.8277444899686, 2881.8277444899704}, {2000, 10000}};
    EXPECT_EQ(result.points, standard);
}

TEST_F(DeterminedShiftTest, standardCalculateVertex)
{
    Polygon2D polygon = {{{0, 0}, {0, 10000}, {10000, 10000}, {10000, 0}, {0, 0}}};
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({0, 0});
    input.region.setExit({10000, 10000});
    input.searchTime = 5000;
    DeterminedShift determinedShift{input};
    auto result = determinedShift.calculate();
    std::vector<Point2D> standard = {{0, 0}, {9500, 9500}, {4164.398451349857, 4164.398451349856}, {3645.4290557121503, 4683.367846987561}, {10000, 10000}};
    EXPECT_EQ(result.points, standard);
}

TEST_F(DeterminedShiftTest, standardCalculateTriangle)
{
    Polygon2D polygon = {{{0, 0}, {0, 10000}, {10000, 0}, {0, 0}}};
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({2000, 0});
    input.region.setExit({0, 10000});
    input.searchTime = 3000;
    DeterminedShift determinedShift{input};
    auto result = determinedShift.calculate();
    std::vector<Point2D> standard = {{2000, 0}, {5646.446609406727, 3646.446609406725}, {499.9999999999991, 8792.893218813453}, {2027.6858461169506, 7265.207372696501}, {0, 10000}};
    EXPECT_EQ(result.points, standard);
}

TEST_F(DeterminedShiftTest, standardCalculateNoConvex)
{
    Polygon2D polygon = {{{0, 0}, {0, 10000}, {8000, 8000}, {10000, 10000}, {10000, 0}, {0, 0}}};
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({2000, 0});
    input.region.setExit({8000, 8000});
    input.searchTime = 5000;
    DeterminedShift determinedShift{input};
    auto result = determinedShift.calculate();
    std::vector<Point2D> standard = {{2000, 0}, {9500, 7499.999999999999}, {5350.087684383223, 3350.087684383221}, {846.2890832926951, 7853.886285473753}, {8000, 8000}};
    EXPECT_EQ(result.points, standard);
}
