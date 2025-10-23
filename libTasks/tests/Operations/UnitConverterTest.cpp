#include <gtest/gtest.h>

#include <Task/Operations/UnitConverter.hpp>
#include <Task/Schemes/Search/InRegion/Input.hpp>
#include <Task/Outputs/Route.hpp>

using namespace Operations;
using Schemes::Search::InRegion::Input;
using Outputs::Route;

/*TEST(InRegion, metricToNauticalInput)
{
    double factor = 1 / mileToMeter;
    Point2D benchmark2{40000 * factor, 40000 * factor};
    Point2D benchmark4{50000 * factor, 40000 * factor};
    Point2D benchmarkInn{14000 * factor, 24000 * factor};
    Point2D benchmarkExit = Point2D{50000 * factor, 12000 * factor};

    Zigzag input;
    Polygon2D polygon({{Point2D(0, 0), Point2D(0, 40000), Point2D(40000, 40000), Point2D(40000, 8000), Point2D(50000, 40000), Point2D(50000, 0)}, {Point2D(14000, 16000), Point2D(26000, 16000), Point2D(26000, 24000), Point2D(14000, 24000)}});
    input.region.setPolygon(polygon); // в метрах, в прямоугольнике с приближенными размерами 25х20 миль
    input.region.setEntrance({0, 12000});
    input.region.setExit({50000, 12000});
//    input.initialPosition = {-8000, 8000};
    input.vSearch = 6; // в м/с, приближенно равно 12 узлам
    input.vMax = 9;  // в м/с, приближенно равно 18 узлам
    input.visionDist = 6000;  // в метрах, приближенно 3 мили
    input.searchTime = 180000; // в секундах, равно 50 часам

    metricToNauticalInput(input);
    // проверка района
    Point2D res2 = input.region.getPolygon().outer()[2];
    EXPECT_NEAR(res2.getX(), benchmark2.getX(), ACCURACY_DOUBLE);
    EXPECT_NEAR(res2.getY(), benchmark2.getY(), ACCURACY_DOUBLE);
    Point2D res4 = input.region.getPolygon().outer()[4];
    EXPECT_NEAR(res4.getX(), benchmark4.getX(), ACCURACY_DOUBLE);
    EXPECT_NEAR(res4.getY(), benchmark4.getY(), ACCURACY_DOUBLE);
    Point2D resInn = input.region.getPolygon().inners().at(0)[3];
    EXPECT_NEAR(resInn.getX(), benchmarkInn.getX(), ACCURACY_DOUBLE);
    EXPECT_NEAR(resInn.getY(), benchmarkInn.getY(), ACCURACY_DOUBLE);

    Point2D resExit = input.region.getExit().point;
    EXPECT_NEAR(resExit.getX(), benchmarkExit.getX(), ACCURACY_DOUBLE);
    EXPECT_NEAR(resExit.getY(), benchmarkExit.getY(), ACCURACY_DOUBLE);

    // скорость, дистанция обнаружения и время
    EXPECT_NEAR(input.visionDist, 6000 * factor, ACCURACY_DOUBLE);
    EXPECT_NEAR(input.vMax, 9 / knotToMps, ACCURACY_DOUBLE);
    EXPECT_NEAR(input.vSearch, 6 / knotToMps, ACCURACY_DOUBLE);
    EXPECT_NEAR(input.searchTime, 50, ACCURACY_DOUBLE);

}*/

TEST(InRegion, nauticalToMetricOutput)
{
    std::vector<Point2D> points{{10, -10}, {15, -1}, {12, 20}};
    std::vector<double> vels{5, 7};
//    Route route{points, vels};
    nauticalToMetricOutput(points, vels);
    EXPECT_NEAR(points[0].getX(),  10 * mileToMeter, ACCURACY_DOUBLE);
    EXPECT_NEAR(points[0].getY(), -10 * mileToMeter, ACCURACY_DOUBLE);
    EXPECT_NEAR(points[1].getX(), 15 * mileToMeter, ACCURACY_DOUBLE);
    EXPECT_NEAR(points[1].getY(), -1 * mileToMeter, ACCURACY_DOUBLE);
    EXPECT_NEAR(points[2].getX(), 12 * mileToMeter, ACCURACY_DOUBLE);
    EXPECT_NEAR(points[2].getY(), 20 * mileToMeter, ACCURACY_DOUBLE);
    EXPECT_NEAR(vels[0], 5 * knotToMps, ACCURACY_DOUBLE);
    EXPECT_NEAR(vels[1], 7 * knotToMps, ACCURACY_DOUBLE);
}
