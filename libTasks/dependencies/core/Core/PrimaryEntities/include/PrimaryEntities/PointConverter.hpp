#ifndef PRIMARY_ENTITIES_POINT_CONVERTER_HPP
#define PRIMARY_ENTITIES_POINT_CONVERTER_HPP
//==============================================================================
#include <PrimaryEntities/PrimaryEntitiesExceptions.hpp>
//! Point
#include <Point/GeoPoint2D.hpp>
#include <Point/GeoPoint3D.hpp>
#include <Point/Point2D.hpp>
#include <Point/Point3D.hpp>
//==============================================================================
namespace PrimaryEntities
{
//==============================================================================
/*!
 *\brief Преобразование из точки geos
 *\param[in] geosGeometry - геометрия geos
 *\return Геометрия PrimaryEntities.
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template <typename PointType>
PointType fromGeosPoint(const geos::geom::Geometry* geosGeometry);
//==============================================================================
/*!
 *\brief Преобразование точки в точку geos
 *\param[in] point - точка
 *\return Указатель на точку geos.
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr
 */
template <typename PointType>
std::unique_ptr<geos::geom::Point> toGeosGeometry(const PointType& point)
{
  geos::geom::GeometryFactory::Ptr geometryFactory
    = geos::geom::GeometryFactory::create();

  std::unique_ptr<geos::geom::Point> geosPoint(
    geometryFactory->createPoint(point.getCoordinates()));

  // Проверим, что точка действительно есть
  if (geosPoint == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                 ": Point was not converted",
                                 "toGeosGeometry");

  return geosPoint;
}
//==============================================================================
} // ! PrimaryEntities
//==============================================================================
#endif //! PRIMARY_ENTITIES_POINT_CONVERTER_HPP
