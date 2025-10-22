#include <AbstractOperations/Algorithms/Helper/CorrectHelper.hpp>
//! Geos
#include <geos/algorithm/Orientation.h>
//==============================================================================
namespace AbstractOperations
{
namespace Helper
{
//==============================================================================
void correctOrientation(std::unique_ptr<geos::geom::Geometry>& geometry)
{
  if (geometry == nullptr)
    return;

  if (geometry->getGeometryTypeId() == geos::geom::GEOS_LINEARRING)
  {
    geos::geom::LinearRing* geosRing
      = dynamic_cast<geos::geom::LinearRing*>(geometry.get());
    geosRing->orient(true);
  }
  else if (geometry->getGeometryTypeId() == geos::geom::GEOS_POLYGON)
  {
    geos::geom::Polygon* geosPolygon
      = dynamic_cast<geos::geom::Polygon*>(geometry.get());
    geosPolygon->orientRings(true);
  }
  else if (geometry->getGeometryTypeId() == geos::geom::GEOS_MULTIPOLYGON)
  {
    geos::geom::MultiPolygon* geosMultiPolygon
      = dynamic_cast<geos::geom::MultiPolygon*>(geometry.get());

    std::vector<std::unique_ptr<geos::geom::Geometry>> polygons
                                       = geosMultiPolygon->releaseGeometries();
    geometry.reset();
    
    for (size_t i = 0; i < polygons.size(); ++i)
    {
      geos::geom::Polygon* geosPolygon
        = dynamic_cast<geos::geom::Polygon*>(polygons.at(i).get());
      geosPolygon->orientRings(true);
    }

    geos::geom::GeometryFactory::Ptr geometryFactory
      = geos::geom::GeometryFactory::create();
    geometry = geometryFactory->createMultiPolygon(std::move(polygons));
  }
}
//==============================================================================
bool hasCorrectOrientation(const geos::geom::Geometry* geometry)
{
  if (geometry == nullptr)
    return true;

  // Пустая геометрия корректна
  if (geometry->isEmpty())
    return true;

  if (geometry->getGeometryTypeId() == geos::geom::GEOS_LINEARRING)
  {
    const geos::geom::LinearRing* geosRing
      = dynamic_cast<const geos::geom::LinearRing*>(geometry);
    return !geos::algorithm::Orientation::isCCW(geosRing->getCoordinatesRO());
  }
  else if (geometry->getGeometryTypeId() == geos::geom::GEOS_POLYGON)
  {
    const geos::geom::Polygon* geosPolygon
      = dynamic_cast<const geos::geom::Polygon*>(geometry);

    if (geos::algorithm::Orientation::isCCW(
      geosPolygon->getExteriorRing()->getCoordinatesRO()))
      return false;

    for (size_t i = 0; i < geosPolygon->getNumInteriorRing(); ++i)
      if (!geos::algorithm::Orientation::isCCW(
        geosPolygon->getInteriorRingN(i)->getCoordinatesRO()))
        return false;

    return true;
  }
  else if (geometry->getGeometryTypeId() == geos::geom::GEOS_MULTIPOLYGON)
  {
    const geos::geom::MultiPolygon* geosMultiPolygon
      = dynamic_cast<const geos::geom::MultiPolygon*>(geometry);

    for (size_t i = 0; i < geosMultiPolygon->getNumGeometries(); ++i)
    {
      const geos::geom::Polygon* geosPolygon
                                           = geosMultiPolygon->getGeometryN(i);

      if (geos::algorithm::Orientation::isCCW(
                           geosPolygon->getExteriorRing()->getCoordinatesRO()))
        return false;

      for (size_t j = 0; j < geosPolygon->getNumInteriorRing(); ++j)
        if (!geos::algorithm::Orientation::isCCW(
                         geosPolygon->getInteriorRingN(j)->getCoordinatesRO()))
          return false;

      return true;
    }
  }

  // Если не кольцо, не полигон и не мультиполигон, ориентация всегда корректна
  return true;
}
//==============================================================================
} //! Helper 
} // ! AbstractOperations
