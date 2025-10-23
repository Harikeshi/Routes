#include <Task/Operations/RouteOperations.hpp>
#include <gtest/gtest.h>

#include <Task/SchemeManager.hpp>
using namespace Schemes::Search::InRegion;
using namespace Operations;

// TODO: without Route
namespace Route {

// checkTime
namespace {

TEST(routeOperations, checkTimeTrue)
{
    std::vector<Point2D> points{{-5, 0}, {0, 0}, {0, 5}};
    std::vector<double> velocities{1, 1};
    EXPECT_TRUE(checkTime(points, velocities, 10));
}

TEST(routeOperations, checkTimeFalse)
{
    std::vector<Point2D> points{{-5, 0}, {0, 0}, {0, 5}};
    std::vector<double> velocities{1, 1};
    EXPECT_FALSE(checkTime(points, velocities, 9));
}

} // namespace

// pathLength
namespace {

TEST(routeOperations, pathLength_Empty)
{
    std::vector<Point2D> points;
    EXPECT_THROW(pathLength(points), std::runtime_error);
}

TEST(routeOperations, pathLength_SizeOne)
{
    std::vector<Point2D> points{{0, 0}};
    EXPECT_EQ(pathLength(points), 0.0);
}

TEST(routeOperations, pathLength_Dist)
{
    std::vector<Point2D> points{{0, 0}, {0, 2}, {0, 4}, {0, 6}};
    EXPECT_EQ(pathLength(points), 6.0);
}

} // namespace

// routeDuration

namespace {

TEST(routeOperations, routeDuration_PointsSizeOne)
{
    std::vector<Point2D> points{{0, 0}};
    std::vector<double> velocities;
    EXPECT_THROW(routeDuration(points, velocities), std::runtime_error);
}

TEST(routeOperations, routeDuration_ThrowVelocities)
{
    std::vector<Point2D> points{{0, 0}, {1, 1}, {2, 2}, {3, 3}};
    std::vector<double> velocities = {1, 2};
    EXPECT_THROW(routeDuration(points, velocities), std::runtime_error);
}

TEST(routeOperations, routeDuration_VelocitiesZero)
{
    std::vector<Point2D> points{{0, 0}, {1, 1}, {2, 2}, {3, 3}};
    std::vector<double> velocities = {1, 2, 0};
    EXPECT_THROW(routeDuration(points, velocities), std::runtime_error);
}

TEST(routeOperations, routeDuration_Normal)
{
    std::vector<Point2D> points{{0, 0}, {0, 1}, {0, 2}};
    std::vector<double> velocities = {1, 1};
    EXPECT_EQ(routeDuration(points, velocities), 2.0);
}

} // namespace
} // namespace Route

//// TODO: with Route
//namespace Route {
//// checkTime
//namespace {
//
//TEST(routeOperations, checkTimeTrue)
//{
//    std::vector<Point2D> points{{-5, 0}, {0, 0}, {0, 5}};
//    std::vector<double> velocities{1, 1};
//    EXPECT_TRUE(checkTime(Route{points, velocities}, 10));
//}
//
//TEST(routeOperations, checkTimeFalse)
//{
//    std::vector<Point2D> points{{-5, 0}, {0, 0}, {0, 5}};
//    std::vector<double> velocities{1, 1};
//    EXPECT_FALSE(checkTime(Route{points, velocities}, 9));
//}
//
//}
//
//// pathLength
//namespace {
//
//TEST(routeOperations, pathLength_Empty)
//{
//    std::vector<Point2D> points;
//    EXPECT_THROW(pathLength(points), std::runtime_error);
//}
//
//TEST(routeOperations, pathLength_SizeOne)
//{
//    std::vector<Point2D> points{{0, 0}};
//    EXPECT_EQ(pathLength(points), 0.0);
//}
//
//TEST(routeOperations, pathLength_Dist)
//{
//    std::vector<Point2D> points{{0, 0}, {0, 2}, {0, 4}, {0, 6}};
//    EXPECT_EQ(pathLength(points), 6.0);
//}
//
//}
//
//
//// routeDuration
//
//namespace {
//
//TEST(routeOperations, routeDuration_PointsSizeOne)
//{
//    std::vector<Point2D> points{{0, 0}};
//    std::vector<double> velocities;
//    Route route(points, velocities);
//    EXPECT_THROW(routeDuration(route), std::runtime_error);
//}
//
//TEST(routeOperations, routeDuration_ThrowVelocities)
//{
//    std::vector<Point2D> points{{0, 0}, {1, 1}, {2, 2}, {3, 3}};
//    std::vector<double> velocities = {1, 2};
//    Route route(points, velocities);
//    EXPECT_THROW(routeDuration(route), std::runtime_error);
//}
//
//TEST(routeOperations, routeDuration_VelocitiesZero)
//{
//    std::vector<Point2D> points{{0, 0}, {1, 1}, {2, 2}, {3, 3}};
//    std::vector<double> velocities = {1, 2, 0};
//    Route route(points, velocities);
//    EXPECT_THROW(routeDuration(route), std::runtime_error);
//}
//
//TEST(routeOperations, routeDuration_Normal)
//{
//    std::vector<Point2D> points{{0, 0}, {0, 1}, {0, 2}};
//    std::vector<double> velocities = {1, 1};
//    Route route(points, velocities);
//    EXPECT_EQ(routeDuration(route), 2.0);
//}
//
//}
//}

// checkTime
namespace {

TEST(routeOperations, checkTimeTrue)
{
    std::vector<Point2D> points{{-5, 0}, {0, 0}, {0, 5}};
    std::vector<double> velocities{1, 1};
    EXPECT_TRUE(checkTime(points, velocities, 10));
}

TEST(routeOperations, checkTimeFalse)
{
    std::vector<Point2D> points{{-5, 0}, {0, 0}, {0, 5}};
    std::vector<double> velocities{1, 1};
    EXPECT_FALSE(checkTime(points, velocities, 9));
}

} // namespace

// tackDetectionRing

TEST(routeOperations, tackDetection)
{
    Point2D position{0, 0};
    Point2D nextPosition{0, 10};
    double detectionRange = 2;
    unsigned int countOfVertices = 4;
    auto result = tackDetectionRing(position, nextPosition, detectionRange, countOfVertices);
}

// detectionRing

TEST(routeOperations, detRing)
{
    std::vector<Point2D> route = {{2, 1}, {2, 3}, {4, 3}, {4, 1}, {2, 1}};
    double detectionRange = 0.5;
    EXPECT_EQ(detectionRing(route, detectionRange, 360).size(), 4);
}

// routeSquare

namespace {

TEST(routeOperations, squareOneNode)
{
    Polygon2D polygon = {{{0, 0}, {0, 10}, {10, 10}, {10, 0}, {0, 0}}};
    std::vector<Point2D> route = {{2, 1}, {2, 4}};
    double detectionRange = 1;
    auto result = routeSquare(polygon, route, detectionRange, 360);
    EXPECT_DOUBLE_EQ(result, 9.14167257082252);
}

TEST(routeOperations, squareIntersectPoly)
{
    Polygon2D polygon = {{{0, 0}, {0, 10}, {10, 10}, {10, 0}, {0, 0}}};
    std::vector<Point2D> route = {{3, 0}, {3, 3}};
    double detectionRange = 1;
    auto result = routeSquare(polygon, route, detectionRange, 360);
    EXPECT_DOUBLE_EQ(result, 7.570836534635293);
}

TEST(routeOperations, squareIntersectPolyTwoPoint)
{
    Polygon2D polygon = {{{0, 0}, {0, 10}, {10, 10}, {10, 0}, {0, 0}}};
    std::vector<Point2D> route = {{3, 0}, {3, 3}, {6, 3}};
    double detectionRange = 1;
    auto result = routeSquare(polygon, route, detectionRange, 360);
    EXPECT_DOUBLE_EQ(result, 13.35625480195294);
}

TEST(routeOperations, squareTwoNode)
{
    Polygon2D polygon = {{{0, 0}, {0, 10}, {10, 10}, {10, 0}, {0, 0}}};
    std::vector<Point2D> route = {{2, 1}, {2, 4}, {5, 4}};
    double detectionRange = 1;
    auto result = routeSquare(polygon, route, detectionRange, 360);
    EXPECT_DOUBLE_EQ(result, 14.927090838140165);
}

TEST(routeOperations, squarePolygon)
{
    Polygon2D polygon = {{{0, 0}, {0, 10}, {10, 10}, {10, 0}, {0, 0}}};
    std::vector<Point2D> route = {{2, 1}, {2, 3}, {4, 3}, {4, 1}, {2, 1}};
    double detectionRange = 0.5;
    auto result = routeSquare(polygon, route, detectionRange, 360);
    EXPECT_DOUBLE_EQ(result, 7.785418267317656);
}

} // namespace

// countPoints

namespace {

TEST(routeOperations, pointsInPoly)
{
    Polygon2D polygon = {{{0, 0}, {0, 10}, {10, 10}, {10, 0}, {0, 0}}};
    std::vector<Point2D> route = {{2, 1}, {2, 3}, {4, 3}, {4, 1}, {2, 1}};
    EXPECT_EQ(countPoints(polygon, route), 5);
}

TEST(routeOperations, zeroPointsInPoly)
{
    Polygon2D polygon = {{{0, 0}, {0, 10}, {10, 10}, {10, 0}, {0, 0}}};
    std::vector<Point2D> route = {{20, 1}, {2, 33}, {47, 3}, {11, 11}, {52, 27}};
    EXPECT_EQ(countPoints(polygon, route), 0);
}

} // namespace

namespace {

TEST(routeOperations, angleSum_rectangle)
{
    std::vector<Point2D> points{{0, 1}, {3, 1}, {3, 3}, {0, 3}, {0, 1}};
    Radian result = angleSum(points);
    EXPECT_EQ(result.get(), 3 * M_PI / 2);
}

TEST(routeOperations, angleSum_star)
{
    std::vector<Point2D> points{{0, 0}, {1, 2}, {-1, 3}, {1, 3}, {3, 5}, {3, 3}, {5, 2}, {3, 2}, {4, 0}, {2, 2}};
    Radian result = angleSum(points);
    EXPECT_EQ(result.get(), 16.029713822345606);
}

} // namespace
