#include <Geodesic/Projection/Orthographic/Orthographic.hpp>

#include <Geodesic/GeodesicException.hpp>
//! Projection 
#include "OrthographicInner.hpp"
//==============================================================================
namespace Geodesic
{
//==============================================================================
OrthographicProjection::OrthographicProjection(
  const std::shared_ptr<Ellipsoid>& ellipsoid 
                                   /*= std::make_shared<Ellipsoid>(EL_WGS_84)*/,
  const GeoPoint3D& center /*= GeoPoint3D(Radian(0.), Radian(0.), 0.f)*/, 
  const Radian& azimuth /*= Radian(0.)*/)
: mInner(std::make_shared<OrthographicInner>(ellipsoid, center, azimuth))
{
}
//==============================================================================
OrthographicProjection::OrthographicProjection(
  const GeoPoint2D& center, 
  const std::shared_ptr<Ellipsoid>& ellipsoid 
                                   /*= std::make_shared<Ellipsoid>(EL_WGS_84)*/,
  const Radian& azimuth /*= Radian(0.)*/)
: mInner(std::make_shared<OrthographicInner>(ellipsoid, GeoPoint3D(center, 0.f), 
                                             azimuth))
{
}
//==============================================================================
Point3D OrthographicProjection::toProjection(const GeoPoint3D& geoPoint) const
{
  return mInner->toProjection(geoPoint);
}
//==============================================================================
Point2D OrthographicProjection::toProjection(const GeoPoint2D& geoPoint) const
{
  //! Переводим как 3D
  Point3D point = toProjection(GeoPoint3D(geoPoint, 0.f));

  //! Отбрасываем третью координату
  return Point2D(point.getX(), point.getY());
}
//==============================================================================
GeoPoint3D OrthographicProjection::fromProjection(const Point3D& point) const
{
  GeoPoint3D visibleGeoPoint;   //!< видимая точка
  GeoPoint3D invisibleGeoPoint; //!< невидимая точка

  //! Переводим из проекции
  if (mInner->fromProjection(point, visibleGeoPoint, invisibleGeoPoint, true))
    return visibleGeoPoint;

  //! Если входной параметр не лежит в проекции эллипсоида, бросаем исключение.
  throw GeodesicFailure(GeodesicEnumFailure::PointOutOfProjection);
}
//==============================================================================
GeoPoint2D OrthographicProjection::fromProjection(const Point2D& point) const
{
  return fromProjection(Point3D(point.getX(), point.getY(), 0.)).get().first;
}
//==============================================================================
void OrthographicProjection::setProjectionPoint(
                const GeoPoint3D center, const Radian azimuth /*= Radian(0.)*/)
{
  mInner->setProjectionPoint(center, azimuth);
}
//==============================================================================
void OrthographicProjection::setProjectionPoint(
                const GeoPoint2D center, const Radian azimuth /*= Radian(0.)*/)
{
  mInner->setProjectionPoint(GeoPoint3D(center, 0.f), azimuth);
}
//==============================================================================
GeoPoint3D OrthographicProjection::getCenterProjection() const
{
  return mInner->getCenterProjection();
}
//==============================================================================
GeoPoint2D OrthographicProjection::getCenterProjection2D() const
{
  return mInner->getCenterProjection().get2D();
}
//==============================================================================
Radian OrthographicProjection::getAzimuth() const
{
  return mInner->getAzimuth();
}
//==============================================================================
bool OrthographicProjection::setEllipsoid(
                                    const std::shared_ptr<Ellipsoid>& ellipsoid)
{
  return mInner->setEllipsoid(ellipsoid);
}
//==============================================================================
std::shared_ptr<Ellipsoid> OrthographicProjection::getEllipsoid() 
const
{
  return mInner->getEllipsoid();
}
//==============================================================================
} //! Geodesic
//==============================================================================

