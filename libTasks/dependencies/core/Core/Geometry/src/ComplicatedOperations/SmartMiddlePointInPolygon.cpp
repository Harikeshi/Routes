#include <Geometry/ComplicatedOperations/SmartMiddlePointInPolygon.hpp>

#include <Geometry/Objects/BorderedLine.hpp>
#include <Geometry/Operations/AtPoints2D.hpp>
#include <Geometry/GeometryException.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/CoveredBy.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
  Point2D setSmartMiddlePoint(
  const Polygon2D& polygon, const Direction priorityDirection /*= North*/)
{
  if (polygon.outer().empty())
    throw GeometryFailure(GeometryEnumFailure::EmptyPolygon);

  // Ищем центр масс
  Point2D center = findCentroid(polygon);

  // Если центр масс оказался внутри полигона или на его границе, 
  // решение найдено
  if (AbstractOperations::coveredBy(center, polygon))
  {
    return center;
  }

  // Если центр масс вне полигона, ищем ближайшую к нему точку 
  // полигона
  double nearestDist;
  // Точки, расстояние до которых оказалось кратчайшим
  std::vector<Point2D> nearestPoints;

  findNearestPoints(polygon, center, nearestPoints, nearestDist);

  // Если найдена всего одна точка - просто возвращаем её
  if (nearestPoints.size() == 1)
    return nearestPoints[0];

  // Если найдено больше одной точки, выбираем подходящую в 
  // соответствии с приоритетным направлением
  // Задаём наклон приоритетного направления
  Radian slope((1 - priorityDirection) * M_PI / 2);

  // Зададим линию, от которой будем считать угол
  Geometry::Line startAngleLine(center, slope);

  // Найдём на ней произвольную точку (для вычисления угла по
  // трём точкам)
  Point2D pointOnAzimuth = startAngleLine.findPoint(center, nearestDist, true);

  // Вектор углов, соответствующий вектору ближайших точек
  std::vector<Radian> angles;

  for (const auto& point : nearestPoints)
  {
    angles.emplace_back(
      findAngle(pointOnAzimuth, center, point, !(priorityDirection == East)));
  }

  // Возвращаем точку с наименьшим углом
  size_t position = 0;
  for (size_t i = 1; i < angles.size(); ++i)
  {
    if (angles[i] < angles[position])
      position = i;
  }

  return nearestPoints[position];
}
//==============================================================================
} //! Geometry
//==============================================================================