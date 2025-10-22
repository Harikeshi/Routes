#include <Geometry/Operations/CircleIntersection.hpp>
//! Geometry
#include <Geometry/Objects/BorderedLine.hpp>
#include <Geometry/Objects/Circle.hpp>

#include <Geometry/GeometryException.hpp>
//! MathTools
#include <MathTools/BasicMath/Arithmetic.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
  std::pair<std::shared_ptr<Point2D>, std::shared_ptr<Point2D>>
    findLineAndCircleIntersection(const Circle& circle, const Line& line)
  {
    // Расстояние от центра окружности до ближайшей точки линии
    double shortestDistToLineFromCenter
      = line.distanceFromPoint(circle.center());

    // Если оно больше радиуса, точек пересечения нет
    if (shortestDistToLineFromCenter > circle.radius())
      return { nullptr, nullptr };

    // Ближайшая к центру окружности точка линии
    Point2D nearestPoint(line.findNearestLinePoint(circle.center()));

    // Если расстояние до ближайшей точки равно радиусу, точка пересечения
    // линии и окружности одна и является ближайшей к центру окружности
    if (fabs(shortestDistToLineFromCenter - circle.radius()) < ACCURACY_DOUBLE)
      return { std::make_shared<Point2D>(nearestPoint), nullptr };

    // Пришли сюда, точки пересечения две

    // Расстояние от ближайшей точки до точки пересечения с окружностью
    double distFromNearestToIntersectionPoint
      = sqrt(BasicMath::sqr(circle.radius())
        - BasicMath::sqr(line.distanceFromPoint(circle.center())));

    // Точки пересечения окружности и линии
    Point2D firstIntersectionPoint(
      line.findPoint(nearestPoint, distFromNearestToIntersectionPoint,
        true, false));
    Point2D secondIntersectionPoint(
      line.findPoint(nearestPoint, distFromNearestToIntersectionPoint, true));

    return { std::make_shared<Point2D>(firstIntersectionPoint),
             std::make_shared<Point2D>(secondIntersectionPoint) };
  }
//==============================================================================
  std::pair<std::shared_ptr<Point2D>, std::shared_ptr<Point2D>>
    findBorderedLineAndCircleIntersection(
      const Circle& circle, const BorderedLine& line)
  {
    // Найдём точки пересечения линии и окружности
    std::pair<std::shared_ptr<Point2D>, std::shared_ptr<Point2D>>
      intersectionPoints = findLineAndCircleIntersection(
        circle, line);

    // Если точек пересечения с линией нет, то и с отрезком тоже нет
    if (!intersectionPoints.first && !intersectionPoints.second)
      return intersectionPoints;

    // Если точка пересечения одна, проверим, что она принадлежит отрезку
    if (!intersectionPoints.second)
    {
      if (line.atBorderLine(*intersectionPoints.first))
        return intersectionPoints;

      return { nullptr, nullptr };
    }

    // Пришли сюда, точек пересечения две
    if (line.atBorderLine(*intersectionPoints.first))
    {
      if (line.atBorderLine(*intersectionPoints.second))
        return intersectionPoints;

      return { intersectionPoints.first, nullptr };
    }

    // Пришли сюда, первая точка не принадлежит отрезку
    if (line.atBorderLine(*intersectionPoints.second))
      return{ intersectionPoints.second, nullptr };

    // Ни одна из найденных точек не принадлежит отрезку
    return { nullptr, nullptr };
  }
//==============================================================================
} //! Geometry
//==============================================================================