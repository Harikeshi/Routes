#include <PrimaryEntities/PointConverter.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace PrimaryEntities
{
namespace Tests
{
//==============================================================================
TEST(PointConverterTest, ToFromGeosPoint)
{
  // 2D точка в декартовых координатах
  Point2D point2D(5., 4.);

  // Конвертация в точку geos
  auto geosPoint = PrimaryEntities::toGeosGeometry(point2D);

  ASSERT_EQ(geosPoint->getX(), point2D.getX());
  ASSERT_EQ(geosPoint->getY(), point2D.getY());

  // Конвертация из geos
  Point2D resPoint2D
    = PrimaryEntities::fromGeosPoint<Point2D>(geosPoint.get());

  ASSERT_EQ(resPoint2D, point2D);

  // Геодезическая 2D точка
  GeoPoint2D geoPoint2D(Radian(5.), Radian(4.));

  // Конвертация в точку geos
  geosPoint = PrimaryEntities::toGeosGeometry(geoPoint2D);

  ASSERT_EQ(geosPoint->getX(), geoPoint2D.getLongitude().get());
  ASSERT_EQ(geosPoint->getY(), geoPoint2D.getLatitude().get());

  // Конвертация из geos
  GeoPoint2D resGeoPoint2D
    = PrimaryEntities::fromGeosPoint<GeoPoint2D>(geosPoint.get());

  ASSERT_EQ(resGeoPoint2D, geoPoint2D);

  // 3D точка в декартовых координатах
  Point3D point3D(5., 4., 8.);

  // Конвертация в точку geos
  geosPoint = PrimaryEntities::toGeosGeometry(point3D);

  ASSERT_EQ(geosPoint->getX(), point3D.getX());
  ASSERT_EQ(geosPoint->getY(), point3D.getY());
  ASSERT_EQ(geosPoint->getZ(), point3D.getZ());

  // Конвертация из geos
  Point3D resPoint3D
    = PrimaryEntities::fromGeosPoint<Point3D>(geosPoint.get());

  ASSERT_EQ(resPoint3D, point3D);

  // Геодезическая 3D точка
  GeoPoint3D geoPoint3D(Radian(5.), Radian(4.), 8.);

  // Конвертация в точку geos
  geosPoint = PrimaryEntities::toGeosGeometry(geoPoint3D);

  ASSERT_EQ(geosPoint->getX(), geoPoint3D.getLongitude().get());
  ASSERT_EQ(geosPoint->getY(), geoPoint3D.getLatitude().get());
  ASSERT_EQ(geosPoint->getZ(), geoPoint3D.getHeight());

  // Конвертация из geos
  GeoPoint3D resGeoPoint3D
    = PrimaryEntities::fromGeosPoint<GeoPoint3D>(geosPoint.get());

  ASSERT_EQ(resGeoPoint3D, geoPoint3D);
}
//==============================================================================
} //! Tests
} //! PrimaryEntities
//==============================================================================