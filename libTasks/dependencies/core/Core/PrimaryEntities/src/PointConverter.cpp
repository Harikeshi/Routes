#include <PrimaryEntities/PointConverter.hpp>
//==============================================================================
namespace PrimaryEntities
{
//==============================================================================
  const geos::geom::Point* convertToPoint(
    const geos::geom::Geometry* geosGeometry)
  {
    // Проверим, что точка есть
    if (geosGeometry == nullptr)
      throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                   ": Empty pointer to point",
                                   "convertToPoint");

    if (geosGeometry->getGeometryTypeId() != geos::geom::GEOS_POINT)
      throw PrimaryEntitiesFailure(IncorrectGeometryType,
                                   ": Geometry is not a point",
                                   "convertToPoint");

    return dynamic_cast<const geos::geom::Point*>(geosGeometry);
  }
//==============================================================================
/*!
 *\brief Преобразование из точки geos
 *\param[in] geosGeometry - точка geos
 *\return 2D точка в декартовых координатах.
 *\throws PrimaryEntitiesFailure # IncorrectGeometryType
 */
template<>
Point2D fromGeosPoint<Point2D>(
  const geos::geom::Geometry* geosGeometry)
{
  const geos::geom::Point* geosPoint(convertToPoint(geosGeometry));
  return Point2D(geosPoint->getX(), geosPoint->getY());
}
//==============================================================================
/*!
 *\brief Преобразование из точки geos
 *\param[in] geosGeometry - точка geos
 *\return 3D точка в декартовых координатах.
 *\throws PrimaryEntitiesFailure # IncorrectGeometryType
 */
template<>
Point3D fromGeosPoint<Point3D>(
  const geos::geom::Geometry* geosGeometry)
{
  const geos::geom::Point* geosPoint(convertToPoint(geosGeometry));
  return Point3D(geosPoint->getX(), geosPoint->getY(), geosPoint->getZ());
}
//==============================================================================
/*!
 *\brief Преобразование из точки geos
 *\param[in] geosGeometry - точка geos
 *\return Геодезическая 2D точка.
 *\throws PrimaryEntitiesFailure # IncorrectGeometryType
 */
template<>
GeoPoint2D fromGeosPoint<GeoPoint2D>(
  const geos::geom::Geometry* geosGeometry)
{
  const geos::geom::Point* geosPoint(convertToPoint(geosGeometry));
  return GeoPoint2D(Radian(geosPoint->getY()),
                    Radian(geosPoint->getX()));
}
//==============================================================================
/*!
 *\brief Преобразование из точки geos
 *\param[in] geosGeometry - точка geos
 *\return Геодезическая 3D точка.
 *\throws PrimaryEntitiesFailure # IncorrectGeometryType
 */
template<>
GeoPoint3D fromGeosPoint<GeoPoint3D>(
  const geos::geom::Geometry* geosGeometry)
{
  const geos::geom::Point* geosPoint(convertToPoint(geosGeometry));
  return GeoPoint3D(Radian(geosPoint->getY()),
                    Radian(geosPoint->getX()),
                    geosPoint->getZ());
}
//==============================================================================
}
//==============================================================================