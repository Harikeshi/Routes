#ifndef GEOMETRY_OPERATIONS_CHECK_POINTS_IN_POLYGON_HPP
#define GEOMETRY_OPERATIONS_CHECK_POINTS_IN_POLYGON_HPP
//==============================================================================
#include <Geometry/Objects/Circle.hpp>
#include <Geometry/Operations/PolygonToCircles.hpp>
#include <Geometry/GeometryException.hpp>

#include <AbstractOperations/Algorithms/CoveredBy.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
/*!
 *\brief Проверка, входит ли набор точек (часть из которых могут быть
 *       площадными) в полигон
 *\param[in] points - наборы точек с их радиусами
 *\param[in] poly   - полигон контура
 *\return вектор, содержащий результат проверки для каждой точки:
 *        true - точка попала в контур, false - не попала.
 *\throws GeometryFailure # NegativeRadius
 */
template<class FlatGeometry>
std::vector<bool> checkPointsInPolygon(
  const std::vector<std::pair<Point2D, double>>& points,
  const FlatGeometry& poly)
{
  // В пустую геометрию ни одна точка попасть не может
  if (PrimaryEntities::toGeosGeometry(poly)->isEmpty())
    return std::vector<bool>(points.size(), false);

  std::vector<bool> result;
  result.reserve(points.size());
  for (const auto& point : points)
  {
    // Если у точки нулевой радиус, сразу проверяем её, иначе сначала
    // сформируем полигон
    // TODO: вернуться к AbstractOperations после появления частичной проверки
    // корректности (SEC03-1420)
    if (fabs(point.second) <= ACCURACY_DOUBLE)
      result.emplace_back(AbstractOperations::coveredBy(point.first, poly));
    else
      result.emplace_back(AbstractOperations::coveredBy(
        toPolygon(Circle(point.first, point.second)), poly));
  }

  return result;
}
//==============================================================================
} // ! Geometry
//==============================================================================
#endif // ! GEOMETRY_OPERATIONS_CHECK_POINTS_IN_POLYGON_HPP
