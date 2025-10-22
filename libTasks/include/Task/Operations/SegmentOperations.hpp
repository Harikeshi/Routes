#pragma once

/*! Core */
#include <Geometry/Operations/AtPoints2D.hpp>
#include <Point/Point2D.hpp>

/*! geos */
#include <geos/algorithm/Intersection.h>
#include <geos/algorithm/LineIntersector.h>
#include <geos/algorithm/MinimumDiameter.h>
#include <geos/algorithm/Orientation.h>
#include <geos/geom/LineSegment.h>

namespace Operations {
// TODO: Сюда перенести пересечения из geos и всю логику geos?
/**
 * @brief Функция приближения к нулю, если точка в окрестности нуля, с точность eps.
 * TODO: Не используется.
 * @param point Точка.
 * @param eps Допустимая точность.
 * @return Point2D
*/
inline Point2D bringToZero(Point2D point, double eps = 1e-14)
{
    double x = point.getX();
    double y = point.getY();

    if (point.getX() >= -eps && point.getX() <= eps)
        x = 0;

    if (y >= -eps && y <= eps)
        y = 0;

    point.set(x, y);

    return point;
}
//==============================================================================
/**
 * @brief Функция расчета точки на заданном направлении на расстоянии от заданной точки с округлением до нуля в окрестностях нуля.
 * @param start Начальная точка.
 * @param distance Дистанция до конечной точки.
 * @param direction Направление к конечной точке.
 * @param eps Точность при округлении к нулю.
 * @return Point2D.
 */
inline Point2D pointOnDistanceBringToZero(Point2D start, double distance, Radian direction, double eps = 1e-14)
{
    // Полярный угол относительно оси x
    start.set(std::move(start.getX() + distance * cos(direction.get())),
              std::move(start.getY() + distance * sin(direction.get())));

    return bringToZero(start, eps);
}

// TODO: Вспомогательная функция, должна быть или удалена или заменена на Core
// TODO: Надо проверить, выполняется ли условие отрезка или требуется дополнительная проверка
/*!
* @brief Функция возвращает перпендикуляр до отрезка.
 * TODO: Что выводит если точка не имеет перпендикуляра на отрезок
 *\param[in] point
 *\param[in] start
 *\param[in] end
 * @return double
*/
inline double pointToSegmentDistance(const Point2D& point, const Point2D& start, const Point2D& end)
{
    geos::geom::LineSegment segment{geos::geom::Coordinate{start.getX(), start.getY()}, geos::geom::Coordinate{end.getX(), end.getY()}};

    geos::geom::Coordinate project;
    geos::geom::Coordinate from{point.getX(), point.getY()};
    project.setNull();

    segment.closestPoint(from, project);

    // TODO: в geos как будто всегда значение меняется, проверка бесполезна, как будто, надо проверять

    // TODO: Что возвращать когда точка не на отрезке
    if (project.isNull())
    {
        std::min(Geometry::distance(point, start), Geometry::distance(point, end));
        // throw std::runtime_error("точка не на отрезке");
    }

    return from.distance(project);
}
//==============================================================================
/*!
 * @brief Функция проверяет пересечение двух отрезков. TODO: Вынести из класса, рабочий метод из Core.
 *\param[in] a1 Точка начала первого отрезка.
 *\param[in] a2 Точка конца первого отрезка.
 *\param[in] b1 Точка начала второго отрезка.
 *\param[in] b2 Точка конца второго отрезка.
 * @return bool
*/
inline bool segmentsIntersect(const Point2D& a1, const Point2D& a2, const Point2D& b1, const Point2D& b2)
{
    return !geos::algorithm::Intersection::intersection(a1.getCoordinates(), a2.getCoordinates(), b1.getCoordinates(), b2.getCoordinates()).isNull();
}
} // namespace Operations
