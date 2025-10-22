#include <AbstractOperations/Algorithms/Area.hpp>
#include <AbstractOperations/Algorithms/Intersection.hpp>
#include <AbstractOperations/Algorithms/Union.hpp>
#include <Geometry/Objects/Circle.hpp>
#include <Geometry/Objects/Line.hpp>

#include <Geometry/Operations/CheckPointsInPolygon.hpp>
#include <Geometry/Operations/PolygonToCircles.hpp>
#include <Task/Operations/RouteOperations.hpp>

#include <cmath>
//
//#include <Geometry/Operations/AtPoints2D.hpp>
//
namespace Operations {

using Line = Geometry::Line;
using Circle = Geometry::Circle;
//
//inline PrimaryEntities::Ring<Point2D> Operations::tackDetectionRing(const Point2D& position, const Point2D& nextPosition, const double& detectionRange, const unsigned int countOfVertices)
//{
//    Ring2D tack, tackTmp, rectangle;
//    Polygon2D polyCircleFirst, polyCircleSecond;
//    Radian forward, leftRad, rightRad;
//
//    polyCircleFirst = toPolygon(Circle{position, detectionRange}, countOfVertices);
//    Line lineFirst(position, nextPosition);
//    forward = lineFirst.getSlope().normalize();
//    leftRad = (forward + Radian{M_PI / 2}).normalize();
//    rightRad = (forward - Radian{M_PI / 2}).normalize();
//    Line lineLeftFirst(position, leftRad);
//    Line lineRightFirst(position, rightRad);
//    rectangle.push_back(lineLeftFirst.findPoint(position, detectionRange));
//    rectangle.push_back(lineRightFirst.findPoint(position, detectionRange));
//
//    polyCircleSecond = toPolygon(Circle{nextPosition, detectionRange}, countOfVertices);
//    Line lineSecond(nextPosition, position);
//    forward = lineSecond.getSlope().normalize();
//    leftRad = (forward + Radian{M_PI / 2}).normalize();
//    rightRad = (forward - Radian{M_PI / 2}).normalize();
//    Line lineLeftSecond(nextPosition, leftRad);
//    Line lineRightSecond(nextPosition, rightRad);
//    rectangle.push_back(lineLeftSecond.findPoint(nextPosition, detectionRange));
//    rectangle.push_back(lineRightSecond.findPoint(nextPosition, detectionRange));
//
//    AbstractOperations::unionOf(polyCircleFirst, rectangle, tackTmp, true, true);
//    AbstractOperations::unionOf(tackTmp, polyCircleSecond, tack, true, true);
//    return tack;
//}
//
//inline std::vector<PrimaryEntities::Ring<Point2D>> Operations::detectionRing(std::vector<Point2D>& route, const double& detectionRange, const unsigned int countOfVertices)
//{
//    std::vector<Ring2D> result;
//    Ring2D tack;
//    if (route.empty())
//        return result;
//    for (size_t i = 0; i < route.size() - 1; i++)
//    {
//        result.push_back(tackDetectionRing(route[i], route[i + 1], detectionRange, countOfVertices));
//    }
//    return result;
//}
//
//inline double Operations::routeSquare(const Polygon2D& polygon, std::vector<Point2D>& route, const double& detectionRange, const unsigned int countOfVertices)
//{
//    Polygon2D geometry;
//    auto detRing = detectionRing(route, detectionRange, countOfVertices);
//    if (detRing.empty())
//        return 0.0;
//    for (size_t i = 0; i < detRing.size(); i++)
//    {
//        AbstractOperations::unionOf(detRing[i], geometry, geometry, true, true);
//    }
//    AbstractOperations::intersection(geometry, polygon, geometry);
//    return AbstractOperations::area(geometry, true, true);
//}
//
//inline int Operations::countPoints(const Polygon2D& polygon, std::vector<Point2D>& route)
//{
//    std::vector<std::pair<Point2D, double>> pointsWithRange;
//    for (const auto& point : route)
//    {
//        pointsWithRange.push_back(std::make_pair(point, 0));
//    }
//    auto boolPoints = Geometry::checkPointsInPolygon(pointsWithRange, polygon);
//    int countPoint = std::count(boolPoints.begin(), boolPoints.end(), true);
//    return countPoint;
//}
} // namespace Operations
