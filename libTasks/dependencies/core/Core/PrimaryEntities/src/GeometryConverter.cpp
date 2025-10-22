#include <PrimaryEntities/GeometryConverter.hpp>
//! std
#include <limits>
//==============================================================================
namespace PrimaryEntities
{
//==============================================================================
//==============================================================================
/*!
 *\brief Преобразование из точки geos
 *\param[in] geosGeometry - точка geos
 *\return 2D точка в декартовых координатах.
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template<>
Point2D fromGeosGeometry(const geos::geom::Geometry* geosGeometry)
{
  return fromGeosPoint<Point2D>(geosGeometry);
}
//==============================================================================
/*!
 *\brief Преобразование из точки geos
 *\param[in] geosGeometry - точка geos
 *\return 3D точка в декартовых координатах.
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template<>
Point3D fromGeosGeometry(const geos::geom::Geometry* geosGeometry)
{
  return fromGeosPoint<Point3D>(geosGeometry);
}
//==============================================================================
/*!
 *\brief Преобразование из точки geos
 *\param[in] geosGeometry - точка geos
 *\return Геодезическая 2D точка.
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template<>
GeoPoint2D fromGeosGeometry(const geos::geom::Geometry* geosGeometry)
{
  return fromGeosPoint<GeoPoint2D>(geosGeometry);
}
//==============================================================================
/*!
 *\brief Преобразование из точки geos
 *\param[in] geosGeometry - точка geos
 *\return Геодезическая 3D точка.
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template<>
GeoPoint3D fromGeosGeometry(const geos::geom::Geometry* geosGeometry)
{
  return fromGeosPoint<GeoPoint3D>(geosGeometry);
}
//==============================================================================
std::unique_ptr<geos::geom::MultiPolygon> choosePolygonsFromCollection(
  const geos::geom::GeometryCollection* collection)
{
  // Проверим, что коллекция есть
  if (collection == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                 ": Empty pointer to collection",
                                 "choosePolygonsFromCollection");

  std::vector<const geos::geom::Geometry*> polygons;
  for (size_t i = 0; i < collection->getNumGeometries(); ++i)
  {
    auto curGeom = collection->getGeometryN(i);
    if (curGeom->getGeometryTypeId() == geos::geom::GEOS_POLYGON)
      polygons.push_back(curGeom);
  }

  return collection->getFactory()->createMultiPolygon(polygons);
}
//==============================================================================
}
//==============================================================================