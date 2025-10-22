#ifndef GEODESIC_OPERATIONS_CHECK_POINTS_IN_POLYGON_HPP
#define GEODESIC_OPERATIONS_CHECK_POINTS_IN_POLYGON_HPP
//==============================================================================
#include <Geodesic/Objects/DualPolygon.hpp>
#include <Geodesic/Projection/Transition.hpp>
#include <Geodesic/Projection/Orthographic/ProjectionBuilder.hpp>
#include <Geodesic/GeoLineTools/OrthodromyTools.hpp>
#include <Geodesic/GeoLineTools/LoxodromeTools.hpp>
#include <Geodesic/GeodesicException.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/CoveredBy.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
/*!
 *\brief Проверка, входит ли набор точек (часть из которых могут быть
 *       площадными) в полигон
 *\details Решение осуществляется в проекции, оптимальной для
 *         набора точек (с учётом их площадей).
 *\param[in] points              - наборы точек с их радиусами
 *\param[in] poly                - полигон контура
 *\param[in] isOrthodromyBorders - если true - контур разбивается на
 *                                 ортодромии, иначе на локсодромии
 *\return вектор, содержащий результат проверки для каждой точки:
 *        true - точка попала в контур, false - не попала.
 *\throws GeodesicFailure # NegativeRadius
 */
template <class GeoGeometry>
std::vector<bool> checkPointsInPolygon(
  const std::vector<std::pair<GeoPoint2D, double>>& points,
  const GeoGeometry& poly,
  const bool isOrthodromyBorders)
{
  // в пустую геометрию ни одна точка попасть не может
  if (PrimaryEntities::toGeosGeometry(poly)->isEmpty())
    return std::vector<bool>(points.size(), false);

  // полигоны окружностей для площадных точек, точки с нулевым
  // радиусом записываются одной точкой
  GeoMultiPolygon2D pointsCircles;

  for (const auto& point : points)
  {
    if (point.second > ACCURACY_DOUBLE)
    {
      // радиус положительный - строим окружность
      pointsCircles.emplace_back(
        DualPolygon( point.first, point.second).getPolygon().front() );
    }
    else if (fabs(point.second) <= ACCURACY_DOUBLE)
    {
      // радиус нулевой - сохраняем точку в виде полигона
      pointsCircles.emplace_back(GeoPolygon2D({ {point.first} }));
    }
    else
    {
      // радиус отрицательный, кидаем исключение
      throw GeodesicFailure( NegativeRadius );
    }
  }

  std::vector<bool> result;
  result.reserve(points.size());

  try
  {
    // Разобъём контур на ортодромии/локсодромии
    auto contour(poly);
    if (isOrthodromyBorders)
    {
      OrthodromyTools tools;
      tools.divide(contour);
    }
    else
    {
      LoxodromeTools tools;
      tools.divide(contour);
    }

    // Спроецируем данные
    auto proj = findOptimalProjection(pointsCircles);
    MultiPolygon2D pointsProj
      = geoGeometryToProjectionForce(pointsCircles, 
        std::make_shared<OrthographicProjection>( proj ) );
    auto polyProj = geoGeometryToProjection(contour, 
      std::make_shared<OrthographicProjection>( proj ) );

    // проверим попадание точек в контур
    for (const auto& point : pointsProj)
    {
      // если точка одна, сначала нужно извлечь её из полигона
      if (point.outer().size() == 1)
        result.emplace_back(AbstractOperations::coveredBy(
          point.outer().at(0), polyProj));
      else
        result.emplace_back(AbstractOperations::coveredBy(point, polyProj));
    }
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<GeodesicFailure>
      ("checkPointsInPolygon");
  }
  
  return result;
}
//==============================================================================
} // ! Geodesic
//==============================================================================
#endif // ! GEODESIC_OPERATIONS_CHECK_POINTS_IN_POLYGON_HPP
