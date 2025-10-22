#include <Geodesic/ComplicatedOperations/PositionInArea.hpp>

#include <Geodesic/GeodesicException.hpp>
#include <Geodesic/Projection/Orthographic/Orthographic.hpp>
#include <Geodesic/Projection/Transition.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Envelope.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
  GeoPoint2D getPositionInArea(
    const GeoPolygon2D& geoPolygon,
    const Geometry::Direction priorityDirection /*= Geometry::North*/)
  {
    // создадим коробку для полигона
    GeoBox2D box;
    AbstractOperations::envelope(geoPolygon, box);

    // вычислим центр проекции
    Radian lonCenter
      = (box.max_corner().getLongitude() + box.min_corner().getLongitude()) / 2;
    Radian latCenter
      = (box.max_corner().getLatitude() + box.min_corner().getLatitude()) / 2;

    // задаём проекцию
    OrthographicProjection projection(
      std::make_shared<Ellipsoid>(EL_WGS_84),
      GeoPoint3D(latCenter, lonCenter));

    // спроецируем полигон на плоскость
    Polygon2D polygon = geoGeometryToProjection(
      geoPolygon, std::make_shared<OrthographicProjection>( projection) );

    // найдём положение точки в полигоне
    Point2D point = setSmartMiddlePoint(polygon, priorityDirection);

    // переведём точку в геодезические координаты
    return projection.fromProjection(point);
  }
//==============================================================================
} //! Geodesic
//==============================================================================