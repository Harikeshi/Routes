#include <Geodesic/ComplicatedOperations/SectorsCalculator.hpp>

#include <Geodesic/Projection/Transition.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//! Geometry
#include <Geometry/Objects/Line.hpp>
#include <Geometry/Operations/AtPoints2D.hpp>
#include <Geometry/Objects/SectorsUnity.hpp>
#include <Geometry/ComplicatedOperations/CakeProblem.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
  std::vector<Geometry::Sector> getSectors(
    const GeoMultiPolygon2D& geoPolygons, 
    const Point2D& center, 
    const double radius, 
    const Geodesic::OrthographicProjection& projection, 
    const Radian startAzimuth,
    const bool unionSectors /*= false */)
  {
    if (geoPolygons.size() < 1)
      return std::vector<Geometry::Sector>();

    // Спроецируем на плоскость
    MultiPolygon2D polygons = 
      Geodesic::geoGeometryToProjection(geoPolygons, 
        std::make_shared<OrthographicProjection>(projection));

    // Зададим сектора, ограничивающие полигоны по их крайним точкам
    std::vector<Geometry::SectorPoly> sectorsInPoints
      = Geometry::getSectorsInPoints(polygons, center, false);

    // Найдём через азимут угол наклона
    Radian slope(startAzimuth.get() > 3 * M_PI / 2
      ? 5 * M_PI / 2 - startAzimuth.get()
      : M_PI / 2 - startAzimuth.get());
    
    // Зададим отрезок, от которого будем считать угол
    Geometry::Line startAngleLine(center, slope);

    // Найдём на нём произвольную точку (для вычисления угла по
    // трём точкам
    Point2D pointOnAzimuth = startAngleLine.findPoint(center, radius, true);

    std::vector<Geometry::Sector> sectors;
    for (const auto& sectorInPoints : sectorsInPoints)
    {
      Radian beginAngle 
        = Geometry::findAngle(
          pointOnAzimuth, center, sectorInPoints.leftPoint, true);

      Radian endAngle
        = Geometry::findAngle(
          pointOnAzimuth, center, sectorInPoints.rightPoint, true);

      sectors.emplace_back(beginAngle, endAngle);
    }

    // Если необходимо, объёдиняем сектора
    if (unionSectors)
      return Geometry::SectorsUnity(sectors).getVector();

    return sectors;
  }
//==============================================================================
} //! Geodesic
//==============================================================================
