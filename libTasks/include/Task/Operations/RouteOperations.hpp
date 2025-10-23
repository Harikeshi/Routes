#pragma once

/*! Core */
#include <Geometry/Operations/AtPoints2D.hpp>

/*! std */
#include <vector>

#include <AbstractOperations/Algorithms/Area.hpp>
#include <AbstractOperations/Algorithms/Intersection.hpp>
#include <AbstractOperations/Algorithms/Union.hpp>
#include <Geometry/Objects/BorderedLine.hpp>
#include <Geometry/Objects/Circle.hpp>
#include <Geometry/Objects/Line.hpp>

#include <Geometry/Operations/CheckPointsInPolygon.hpp>
#include <Geometry/Operations/PolygonToCircles.hpp>
#include <PrimaryEntities/Entities/Box.hpp>
#include <Task/Entities/SearchRing.hpp>
#include <Task/Operations/RouteOperations.hpp>

namespace Operations {

using Line = Geometry::Line;
using BorderedLine = Geometry::BorderedLine;
using Circle = Geometry::Circle;
using Ring2D = PrimaryEntities::Ring<Point2D>;
using SearchRing = Entities::SearchRing;

//// TODO: изменения произведены из-за двойного включения файла , из-за использования двух регионов
/*!
 * \brief pathLength
 * \details Считает длину маршрута
 * \param points [in] маршрут по точкам
 * \return [out] длину
 */
inline double pathLength(const std::vector<Point2D>& points)
{
    if (points.empty())
        throw std::runtime_error("Вектор пустой");
    if (points.size() == 1)
        return 0.0;
    double dist = 0;
    for (size_t i = 0; i < points.size() - 1; i++)
    {
        dist += Geometry::distance(points[i], points[i + 1]);
    }
    return dist;
}

/*!
 * \brief routeDuration
 * \details Считает продолжительность маршрута
 * \param route [in] маршрут
 * \return [out] Время прохождения маршрута
 */
//double routeDuration(const Route& route);
inline double routeDuration(const std::vector<Point2D>& points, const std::vector<double>& velocities)
{
    if (points.size() < 2 || velocities.size() != points.size() - 1)
        throw std::runtime_error("Некорректный маршрут");
    double duration = 0;
    for (size_t i = 0; i < velocities.size(); ++i)
    {
        if (velocities.at(i) != 0)
        {
            duration += Geometry::distance(points.at(i), points.at(i + 1)) / velocities.at(i);
        }
        else
        {
            throw std::runtime_error("Скорость равна нулю");
        }
    }
    return duration;
}

/*!
 * \brief checkTime
 * \details Проверяет время прокладки маршрута с точностью timeAccuracy
 * \param route [in] маршрут
 * \param setTime [in] время поиска
 * \param timeAccuracy [in]
 * \return [out] если укладываемся по времени вернуть true. иначе false
 */
//bool checkTime(const Route& route, double setTime, double timeAccuracy = ACCURACY_DOUBLE);
inline bool checkTime(const std::vector<Point2D>& points, const std::vector<double>& velocities, double setTime, double timeAccuracy = ACCURACY_DOUBLE)
{
    return std::abs(routeDuration(points, velocities) - setTime) < timeAccuracy;
}

/*!
 * \brief printRoute
 * \details Вывод маршрута
 * \param route [in] маршрут
 */
inline void printRoute(const std::vector<Point2D>& points, const std::vector<double>& velocities)
{
    for (size_t i = 0; i < points.size(); ++i)
    {
        std::cout << "Point " << i << ": X{" << points[i].getX() << "}, Y{" << points[i].getY() << "};\n";
        if (i != points.size() - 1)
        {
            std::cout << "Vel_to_next: {" << velocities[i] << "}\n";
        }
    }
}

/*!
 * \brief tackDetectionRing
 * \param position
 * \param nextPosition
 * \param detectionRange
 * \param countOfVertices
 * \return
 */
inline Ring2D tackDetectionRing(const Point2D& position, const Point2D& nextPosition, const double& detectionRange, const unsigned int countOfVertices = 360)
{
    Ring2D tack, tackTmp, rectangle;
    Polygon2D polyCircleFirst, polyCircleSecond;
    Radian forward, leftRad, rightRad;

    polyCircleFirst = toPolygon(Circle{position, detectionRange}, countOfVertices);
    Line lineFirst(position, nextPosition);
    forward = lineFirst.getSlope().normalize();
    leftRad = (forward + Radian{M_PI / 2}).normalize();
    rightRad = (forward - Radian{M_PI / 2}).normalize();
    Line lineLeftFirst(position, leftRad);
    Line lineRightFirst(position, rightRad);
    rectangle.push_back(lineLeftFirst.findPoint(position, detectionRange));
    rectangle.push_back(lineRightFirst.findPoint(position, detectionRange));

    polyCircleSecond = toPolygon(Circle{nextPosition, detectionRange}, countOfVertices);
    Line lineSecond(nextPosition, position);
    forward = lineSecond.getSlope().normalize();
    leftRad = (forward + Radian{M_PI / 2}).normalize();
    rightRad = (forward - Radian{M_PI / 2}).normalize();
    Line lineLeftSecond(nextPosition, leftRad);
    Line lineRightSecond(nextPosition, rightRad);
    rectangle.push_back(lineLeftSecond.findPoint(nextPosition, detectionRange));
    rectangle.push_back(lineRightSecond.findPoint(nextPosition, detectionRange));

    AbstractOperations::unionOf(polyCircleFirst, rectangle, tackTmp, true, true);
    AbstractOperations::unionOf(tackTmp, polyCircleSecond, tack, true, true);
    return tack;
}

/*!
 * \brief detectionRing
 * \details
 * \param route [in] маршрут
 * \param detectionRange [in] дальность действия ГАС, [0-999999, м]
 * \return
 */
inline std::vector<Ring2D> detectionRing(const std::vector<Point2D>& route, const double& detectionRange, const unsigned int countOfVertices = 360)
{
    std::vector<Ring2D> result;
    Ring2D tack;
    if (route.empty())
        return result;
    for (size_t i = 0; i < route.size() - 1; i++)
    {
        result.push_back(tackDetectionRing(route[i], route[i + 1], detectionRange, countOfVertices));
    }
    return result;
}

/*!
 * \brief detectionRing
 * \details Считает площадь маршрута с учетом detectionRange
 * \param polygon [in] полигон
 * \param route [in] маршрут
 * \param detectionRange [in] дальность действия ГАС, [0-999999, м]
 * \return [out] площадь фигуры
 */
inline double routeSquare(const Polygon2D& polygon, const std::vector<Point2D>& route, const double& detectionRange, const unsigned int countOfVertices = 360)
{
    Polygon2D geometry;
    auto detRing = detectionRing(route, detectionRange, countOfVertices);
    if (detRing.empty())
        return 0.0;
    for (size_t i = 0; i < detRing.size(); i++)
    {
        AbstractOperations::unionOf(detRing[i], geometry, geometry, true, true);
    }
    AbstractOperations::intersection(geometry, polygon, geometry);
    return AbstractOperations::area(geometry, true, true);
}

/*!
 * \brief countPoints
 * \details Считает кол-во точек во входном полигоне
 * \param polygon [in] полигон
 * \param route [in] маршрут
 * \return [out] количество точек
 */
inline int countPoints(const Polygon2D& polygon, const std::vector<Point2D>& route)
{
    std::vector<std::pair<Point2D, double>> pointsWithRange;
    for (const auto& point : route)
    {
        pointsWithRange.push_back(std::make_pair(point, 0));
    }
    auto boolPoints = Geometry::checkPointsInPolygon(pointsWithRange, polygon);
    int countPoint = std::count(boolPoints.begin(), boolPoints.end(), true);
    return countPoint;
}

/*!
 * \brief centerLine
 * \details строит осевую линию для многоугольника
 * \param ring [in] кольцо
 * \param centerPoint [in] центральная точка коробки
 * \return [out] возвращает осевую линию
 */
inline Line centerLine(const Ring2D& ring, const Point2D& centerPoint)
{
    auto box = static_cast<SearchRing>(ring).getOrientedBoundingBoxGEOS();
    std::vector<std::pair<double, BorderedLine>> segments;
    Point2D pointLine;
    double dist = 0;

    for (size_t i = 0; i < box.size() - 3; i++)
    {
        BorderedLine borderLine(box[i], box[i + 1]);
        pointLine = borderLine.findNearestLinePoint(centerPoint);
        dist = Geometry::distance(pointLine, centerPoint);
        segments.push_back(std::make_pair(dist, BorderedLine{pointLine, centerPoint}));
    }

    auto maxSegment = std::max_element(segments.begin(), segments.end(), [](const std::pair<double, BorderedLine> a, const std::pair<double, BorderedLine> b) {
        return a.first < b.first;
    });

    return maxSegment->second.parallelLine(centerPoint);
}

inline Radian angleSum(const std::vector<Point2D>& points)
{
    if (points.size() < 3)
    {
        throw std::runtime_error("Слишком мало маршрутных точек!");
    }
    Radian angleSum{0};
    for (size_t i = 0; i < points.size() - 2; ++i)
    {
        BorderedLine currSegment(points[i], points[i + 1]);
        BorderedLine nextSegment(points[i + 1], points[i + 2]);
        angleSum += fabs(nextSegment.getAngleTo(currSegment));
    }
    return angleSum;
}

} // namespace Operations
