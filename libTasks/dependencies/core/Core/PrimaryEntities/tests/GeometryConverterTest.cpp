#include <PrimaryEntities/GeometryConverter.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace PrimaryEntities
{
namespace Tests
{
//==============================================================================
TEST(GeometryConverterTest, ToFromGeosPoint)
{
  // 2D точка в декартовых координатах
  Point2D point2D(5., 4.);

  // Конвертация в точку geos
  auto geosPoint = PrimaryEntities::toGeosGeometry(point2D);

  ASSERT_EQ(geosPoint->getX(), point2D.getX());
  ASSERT_EQ(geosPoint->getY(), point2D.getY());

  // Конвертация из geos
  Point2D resPoint2D
    = PrimaryEntities::fromGeosGeometry<Point2D>(geosPoint.get());

  ASSERT_EQ(resPoint2D, point2D);

  // Геодезическая 2D точка
  GeoPoint2D geoPoint2D(Radian(5.), Radian(4.));

  // Конвертация в точку geos
  geosPoint = PrimaryEntities::toGeosGeometry(geoPoint2D);

  ASSERT_EQ(geosPoint->getX(), geoPoint2D.getLongitude().get());
  ASSERT_EQ(geosPoint->getY(), geoPoint2D.getLatitude().get());

  // Конвертация из geos
  GeoPoint2D resGeoPoint2D
    = PrimaryEntities::fromGeosGeometry<GeoPoint2D>(geosPoint.get());

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
    = PrimaryEntities::fromGeosGeometry<Point3D>(geosPoint.get());

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
    = PrimaryEntities::fromGeosGeometry<GeoPoint3D>(geosPoint.get());

  ASSERT_EQ(resGeoPoint3D, geoPoint3D);
}
//==============================================================================
TEST(GeometryConverterTest, ToFromGeosLinestring)
{
  // Конвертация пустой линии
  Linestring2D linestring;
  auto geosLinestring = PrimaryEntities::toGeosGeometry(linestring);
  ASSERT_TRUE(geosLinestring->isEmpty());

  // 2D линия в декартовых координатах
  Linestring2D linestring2D({ { 5., 4. }, {2., 3.} });

  // Конвертация в линию geos
  geosLinestring = PrimaryEntities::toGeosGeometry(linestring2D);
  ASSERT_EQ(geosLinestring->getNumPoints(), 2);

  ASSERT_EQ(geosLinestring->getCoordinateN(0).x, 5.);
  ASSERT_EQ(geosLinestring->getCoordinateN(0).y, 4.);
  ASSERT_EQ(geosLinestring->getCoordinateN(1).x, 2.);
  ASSERT_EQ(geosLinestring->getCoordinateN(1).y, 3.);

  // Конвертация из geos
  Linestring2D resLinestring2D
    = PrimaryEntities::fromGeosGeometry<Linestring2D>(geosLinestring.get());

  ASSERT_EQ(resLinestring2D.at(0), linestring2D.at(0));
  ASSERT_EQ(resLinestring2D.at(1), linestring2D.at(1));

  // Геодезическая 2D линия
  GeoLinestring2D geoLinestring2D(
    { { Radian(5.), Radian(4.) }, { Radian(2.), Radian(3.) } });

  // Конвертация в линию geos
  geosLinestring = PrimaryEntities::toGeosGeometry(geoLinestring2D);

  ASSERT_EQ(geosLinestring->getCoordinateN(0).x, 4.);
  ASSERT_EQ(geosLinestring->getCoordinateN(0).y, 5.);
  ASSERT_EQ(geosLinestring->getCoordinateN(1).x, 3.);
  ASSERT_EQ(geosLinestring->getCoordinateN(1).y, 2.);

  // Конвертация из geos
  GeoLinestring2D resGeoLinestring2D
    = PrimaryEntities::fromGeosGeometry<GeoLinestring2D>(geosLinestring.get());

  ASSERT_EQ(resGeoLinestring2D.at(0), geoLinestring2D.at(0));
  ASSERT_EQ(resGeoLinestring2D.at(1), geoLinestring2D.at(1));

  // 3D линия в декартовых координатах
  Linestring3D linestring3D({ {5., 4., 8.}, {2., 3., 1.} });

  // Конвертация в линию geos
  geosLinestring = PrimaryEntities::toGeosGeometry(linestring3D);

  ASSERT_EQ(geosLinestring->getCoordinateN(0).x, 5.);
  ASSERT_EQ(geosLinestring->getCoordinateN(0).y, 4.);
  ASSERT_EQ(geosLinestring->getCoordinateN(0).z, 8.);
  ASSERT_EQ(geosLinestring->getCoordinateN(1).x, 2.);
  ASSERT_EQ(geosLinestring->getCoordinateN(1).y, 3.);
  ASSERT_EQ(geosLinestring->getCoordinateN(1).z, 1.);

  // Конвертация из geos
  Linestring3D resLinestring3D
    = PrimaryEntities::fromGeosGeometry<Linestring3D>(geosLinestring.get());

  ASSERT_EQ(resLinestring3D.at(0), linestring3D.at(0));
  ASSERT_EQ(resLinestring3D.at(1), linestring3D.at(1));

  // Геодезическая 3D линия
  GeoLinestring3D geoLinestring3D(
    { {Radian(5.), Radian(4.), 8.}, {Radian(2.), Radian(3.), 1.} });

  // Конвертация в линию geos
  geosLinestring = PrimaryEntities::toGeosGeometry(geoLinestring3D);

  ASSERT_EQ(geosLinestring->getCoordinateN(0).x, 4.);
  ASSERT_EQ(geosLinestring->getCoordinateN(0).y, 5.);
  ASSERT_EQ(geosLinestring->getCoordinateN(0).z, 8.);
  ASSERT_EQ(geosLinestring->getCoordinateN(1).x, 3.);
  ASSERT_EQ(geosLinestring->getCoordinateN(1).y, 2.);
  ASSERT_EQ(geosLinestring->getCoordinateN(1).z, 1.);

  // Конвертация из geos
  GeoLinestring3D resGeoLinestring3D
    = PrimaryEntities::fromGeosGeometry<GeoLinestring3D>(geosLinestring.get());

  ASSERT_EQ(resGeoLinestring3D.at(0), geoLinestring3D.at(0));
  ASSERT_EQ(resGeoLinestring3D.at(1), geoLinestring3D.at(1));

  // В линии не может быть только одна точка
  linestring2D.pop_back();

  ASSERT_THROW(
    PrimaryEntities::toGeosGeometry(linestring2D), PrimaryEntitiesFailure);

  try
  {
    geosLinestring = PrimaryEntities::toGeosGeometry(linestring2D);
  }
  catch (const PrimaryEntitiesFailure& exception)
  {
    ASSERT_EQ(
      PrimaryEntitiesEnumFailure::IncorrectLinestring, exception.getCode());
    ASSERT_EQ(primaryLibTitle, exception.getLib());
  }
}
//==============================================================================
TEST(GeometryConverterTest, ToFromGeosMultiLinestring)
{
  // Конвертация пустой мультилинии
  MultiLinestring2D multiLine;
  auto geosmultiLine = PrimaryEntities::toGeosGeometry(multiLine);
  ASSERT_TRUE(geosmultiLine->isEmpty());
  
  // 2D мультилиния в декартовых координатах
  MultiLinestring2D linestring2D(
    { { { 5., 4. }, {2., 3.} }, { {6., 1.}, {4., 8.} } });

  // Конвертация в мультилинию geos
  geosmultiLine = PrimaryEntities::toGeosGeometry(linestring2D);
  ASSERT_EQ(geosmultiLine->getNumGeometries(), 2);

  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getNumPoints(), 2);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(0).x, 5.);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(0).y, 4.);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(1).x, 2.);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(1).y, 3.);

  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getNumPoints(), 2);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(0).x, 6.);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(0).y, 1.);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(1).x, 4.);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(1).y, 8.);

  // Конвертация из geos
  MultiLinestring2D resLinestring2D
    = PrimaryEntities::fromGeosGeometry<MultiLinestring2D>(
      geosmultiLine.get());

  ASSERT_EQ(resLinestring2D.at(0).at(0), linestring2D.at(0).at(0));
  ASSERT_EQ(resLinestring2D.at(0).at(1), linestring2D.at(0).at(1));
  ASSERT_EQ(resLinestring2D.at(1).at(0), linestring2D.at(1).at(0));
  ASSERT_EQ(resLinestring2D.at(1).at(1), linestring2D.at(1).at(1));

  // Геодезическая 2D мультилиния
  GeoMultiLinestring2D geoLinestring2D(
    { { { Radian(5.), Radian(4.) }, { Radian(2.), Radian(3.) } },
      { { Radian(6.), Radian(1.) }, { Radian(4.), Radian(8.) } } });

  // Конвертация в мультилинию geos
  geosmultiLine = PrimaryEntities::toGeosGeometry(geoLinestring2D);
  ASSERT_EQ(geosmultiLine->getNumGeometries(), 2);

  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getNumPoints(), 2);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(0).x, 4.);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(0).y, 5.);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(1).x, 3.);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(1).y, 2.);

  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getNumPoints(), 2);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(0).x, 1.);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(0).y, 6.);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(1).x, 8.);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(1).y, 4.);

  // Конвертация из geos
  GeoMultiLinestring2D resGeoLinestring2D
    = PrimaryEntities::fromGeosGeometry<GeoMultiLinestring2D>(
      geosmultiLine.get());

  ASSERT_EQ(resGeoLinestring2D.at(0).at(0), geoLinestring2D.at(0).at(0));
  ASSERT_EQ(resGeoLinestring2D.at(0).at(1), geoLinestring2D.at(0).at(1));
  ASSERT_EQ(resGeoLinestring2D.at(1).at(0), geoLinestring2D.at(1).at(0));
  ASSERT_EQ(resGeoLinestring2D.at(1).at(1), geoLinestring2D.at(1).at(1));

  // 3D мультилиния в декартовых координатах
  MultiLinestring3D linestring3D(
    { {{5., 4., 8.}, {2., 3., 1.} }, { {6., 1., 2.}, {4., 8., 3.} } });

  // Конвертация в мультилинию geos
  geosmultiLine = PrimaryEntities::toGeosGeometry(linestring3D);
  ASSERT_EQ(geosmultiLine->getNumGeometries(), 2);

  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getNumPoints(), 2);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(0).x, 5.);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(0).y, 4.);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(0).z, 8.);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(1).x, 2.);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(1).y, 3.);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(1).z, 1.);

  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getNumPoints(), 2);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(0).x, 6.);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(0).y, 1.);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(0).z, 2.);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(1).x, 4.);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(1).y, 8.);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(1).z, 3.);

  // Конвертация из geos
  MultiLinestring3D resLinestring3D
    = PrimaryEntities::fromGeosGeometry<MultiLinestring3D>(
      geosmultiLine.get());

  ASSERT_EQ(resLinestring3D.at(0).at(0), linestring3D.at(0).at(0));
  ASSERT_EQ(resLinestring3D.at(0).at(1), linestring3D.at(0).at(1));
  ASSERT_EQ(resLinestring3D.at(1).at(0), linestring3D.at(1).at(0));
  ASSERT_EQ(resLinestring3D.at(1).at(1), linestring3D.at(1).at(1));

  // Геодезическая 3D мультилиния
  GeoMultiLinestring3D geoLinestring3D(
    { { { Radian(5.), Radian(4.), 8. }, { Radian(2.), Radian(3.), 1. } },
      { { Radian(6.), Radian(1.), 2. }, { Radian(4.), Radian(8.), 3. } } });

  // Конвертация в мультилинию geos
  geosmultiLine = PrimaryEntities::toGeosGeometry(geoLinestring3D);
  ASSERT_EQ(geosmultiLine->getNumGeometries(), 2);

  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getNumPoints(), 2);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(0).x, 4.);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(0).y, 5.);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(0).z, 8.);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(1).x, 3.);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(1).y, 2.);
  ASSERT_EQ(geosmultiLine->getGeometryN(0)->getCoordinateN(1).z, 1.);

  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getNumPoints(), 2);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(0).x, 1.);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(0).y, 6.);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(0).z, 2.);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(1).x, 8.);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(1).y, 4.);
  ASSERT_EQ(geosmultiLine->getGeometryN(1)->getCoordinateN(1).z, 3.);

  // Конвертация из geos
  GeoMultiLinestring3D resGeoLinestring3D
    = PrimaryEntities::fromGeosGeometry<GeoMultiLinestring3D>(
      geosmultiLine.get());

  ASSERT_EQ(resGeoLinestring3D.at(0).at(0), geoLinestring3D.at(0).at(0));
  ASSERT_EQ(resGeoLinestring3D.at(0).at(1), geoLinestring3D.at(0).at(1));
  ASSERT_EQ(resGeoLinestring3D.at(1).at(0), geoLinestring3D.at(1).at(0));
  ASSERT_EQ(resGeoLinestring3D.at(1).at(1), geoLinestring3D.at(1).at(1));
}
//==============================================================================
TEST(GeometryConverterTest, ToFromGeosMultiPoint)
{
  // Конвертация пустого набора точек
  MultiPoint2D multiPoint;
  auto geosMultiPoint = PrimaryEntities::toGeosGeometry(multiPoint);
  ASSERT_TRUE(geosMultiPoint->isEmpty());

  // 2D набор точек в декартовых координатах
  MultiPoint2D multiPoint2D({ { 5., 4. }, {2., 3.} });

  // Конвертация в набор точек geos
  geosMultiPoint = PrimaryEntities::toGeosGeometry(multiPoint2D);
  ASSERT_EQ(geosMultiPoint->getNumPoints(), 2);

  ASSERT_EQ(geosMultiPoint->getGeometryN(0)->getX(), 5.);
  ASSERT_EQ(geosMultiPoint->getGeometryN(0)->getY(), 4.);
  ASSERT_EQ(geosMultiPoint->getGeometryN(1)->getX(), 2.);
  ASSERT_EQ(geosMultiPoint->getGeometryN(1)->getY(), 3.);

  // Конвертация из geos
  MultiPoint2D resMultiPoint2D
    = PrimaryEntities::fromGeosGeometry<MultiPoint2D>(geosMultiPoint.get());

  ASSERT_EQ(resMultiPoint2D.at(0), multiPoint2D.at(0));
  ASSERT_EQ(resMultiPoint2D.at(1), multiPoint2D.at(1));

  // Геодезический 2D набор точек
  GeoMultiPoint2D geoMultiPoint2D(
    { { Radian(5.), Radian(4.) }, { Radian(2.), Radian(3.) } });

  // Конвертация в набор точек geos
  geosMultiPoint = PrimaryEntities::toGeosGeometry(geoMultiPoint2D);
  ASSERT_EQ(geosMultiPoint->getNumPoints(), 2);

  ASSERT_EQ(geosMultiPoint->getGeometryN(0)->getX(), 4.);
  ASSERT_EQ(geosMultiPoint->getGeometryN(0)->getY(), 5.);
  ASSERT_EQ(geosMultiPoint->getGeometryN(1)->getX(), 3.);
  ASSERT_EQ(geosMultiPoint->getGeometryN(1)->getY(), 2.);

  // Конвертация из geos
  GeoMultiPoint2D resGeoMultiPoint2D
    = PrimaryEntities::fromGeosGeometry<GeoMultiPoint2D>(geosMultiPoint.get());

  ASSERT_EQ(resGeoMultiPoint2D.at(0), geoMultiPoint2D.at(0));
  ASSERT_EQ(resGeoMultiPoint2D.at(1), geoMultiPoint2D.at(1));

  // 3D набор точек в декартовых координатах
  MultiPoint3D multiPoint3D({ {5., 4., 8.}, {2., 3., 1.} });

  // Конвертация в набор точек geos
  geosMultiPoint = PrimaryEntities::toGeosGeometry(multiPoint3D);
  ASSERT_EQ(geosMultiPoint->getNumPoints(), 2);

  ASSERT_EQ(geosMultiPoint->getGeometryN(0)->getX(), 5.);
  ASSERT_EQ(geosMultiPoint->getGeometryN(0)->getY(), 4.);
  ASSERT_EQ(geosMultiPoint->getGeometryN(0)->getZ(), 8.);
  ASSERT_EQ(geosMultiPoint->getGeometryN(1)->getX(), 2.);
  ASSERT_EQ(geosMultiPoint->getGeometryN(1)->getY(), 3.);
  ASSERT_EQ(geosMultiPoint->getGeometryN(1)->getZ(), 1.);

  // Конвертация из geos
  MultiPoint3D resMultiPoint3D
    = PrimaryEntities::fromGeosGeometry<MultiPoint3D>(geosMultiPoint.get());

  ASSERT_EQ(resMultiPoint3D.at(0), multiPoint3D.at(0));
  ASSERT_EQ(resMultiPoint3D.at(1), multiPoint3D.at(1));

  // Геодезический 3D набор точек
  GeoMultiPoint3D geoMultiPoint3D(
    { {Radian(5.), Radian(4.), 8.}, {Radian(2.), Radian(3.), 1.} });

  // Конвертация в набор точек geos
  geosMultiPoint = PrimaryEntities::toGeosGeometry(geoMultiPoint3D);
  ASSERT_EQ(geosMultiPoint->getNumPoints(), 2);

  ASSERT_EQ(geosMultiPoint->getGeometryN(0)->getX(), 4.);
  ASSERT_EQ(geosMultiPoint->getGeometryN(0)->getY(), 5.);
  ASSERT_EQ(geosMultiPoint->getGeometryN(0)->getZ(), 8.);
  ASSERT_EQ(geosMultiPoint->getGeometryN(1)->getX(), 3.);
  ASSERT_EQ(geosMultiPoint->getGeometryN(1)->getY(), 2.);
  ASSERT_EQ(geosMultiPoint->getGeometryN(1)->getZ(), 1.);

  // Конвертация из geos
  GeoMultiPoint3D resGeoMultiPoint3D
    = PrimaryEntities::fromGeosGeometry<GeoMultiPoint3D>(geosMultiPoint.get());

  ASSERT_EQ(resGeoMultiPoint3D.at(0), geoMultiPoint3D.at(0));
  ASSERT_EQ(resGeoMultiPoint3D.at(1), geoMultiPoint3D.at(1));
}
//==============================================================================
TEST(GeometryConverterTest, ToFromGeosRing)
{
  // Конвертация пустого кольца
  Ring2D ring;
  auto geosRing = PrimaryEntities::toGeosGeometry(ring);
  ASSERT_TRUE(geosRing->isEmpty());

  // 2D кольцо в декартовых координатах
  Ring2D ring2D({ { 5., 4. }, {2., 3.}, {1., 5.}, {5., 4.} });

  // Конвертация в кольцо geos
  geosRing = PrimaryEntities::toGeosGeometry(ring2D);
  ASSERT_EQ(geosRing->getExteriorRing()->getNumPoints(), 4);
  ASSERT_EQ(geosRing->getNumInteriorRing(), 0);

  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(0).x, 5.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(0).y, 4.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(1).x, 2.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(1).y, 3.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(2).x, 1.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(2).y, 5.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(3).x, 5.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(3).y, 4.);

  // Конвертация из geos
  Ring2D resRing2D = PrimaryEntities::fromGeosGeometry<Ring2D>(geosRing.get());

  ASSERT_EQ(resRing2D.at(0), ring2D.at(0));
  ASSERT_EQ(resRing2D.at(1), ring2D.at(1));
  ASSERT_EQ(resRing2D.at(2), ring2D.at(2));
  ASSERT_EQ(resRing2D.at(3), ring2D.at(3));

  // Геодезическое 2D кольцо
  GeoRing2D geoRing2D({ { Radian(5.), Radian(4.) },
                        { Radian(2.), Radian(3.) },
                        { Radian(1.), Radian(5.) },
                        { Radian(5.), Radian(4.) } });

  // Конвертация в кольцо geos
  geosRing = PrimaryEntities::toGeosGeometry(geoRing2D);
  ASSERT_EQ(geosRing->getExteriorRing()->getNumPoints(), 4);
  ASSERT_EQ(geosRing->getNumInteriorRing(), 0);

  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(0).x, 4.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(0).y, 5.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(1).x, 3.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(1).y, 2.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(2).x, 5.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(2).y, 1.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(3).x, 4.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(3).y, 5.);

  // Конвертация из geos
  GeoRing2D resGeoRing2D
    = PrimaryEntities::fromGeosGeometry<GeoRing2D>(geosRing.get());

  ASSERT_EQ(resGeoRing2D.at(0), geoRing2D.at(0));
  ASSERT_EQ(resGeoRing2D.at(1), geoRing2D.at(1));
  ASSERT_EQ(resGeoRing2D.at(2), geoRing2D.at(2));
  ASSERT_EQ(resGeoRing2D.at(3), geoRing2D.at(3));

  // 3D кольцо в декартовых координатах
  Ring3D ring3D({ {5., 4., 8.}, {2., 3., 1.}, {1., 5., 1.}, {5., 4., 8.} });

  // Конвертация в кольцо geos
  geosRing = PrimaryEntities::toGeosGeometry(ring3D);
  ASSERT_EQ(geosRing->getExteriorRing()->getNumPoints(), 4);
  ASSERT_EQ(geosRing->getNumInteriorRing(), 0);

  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(0).x, 5.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(0).y, 4.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(0).z, 8.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(1).x, 2.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(1).y, 3.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(1).z, 1.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(2).x, 1.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(2).y, 5.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(2).z, 1.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(3).x, 5.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(3).y, 4.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(3).z, 8.);

  // Конвертация из geos
  Ring3D resRing3D = PrimaryEntities::fromGeosGeometry<Ring3D>(geosRing.get());

  ASSERT_EQ(resRing3D.at(0), ring3D.at(0));
  ASSERT_EQ(resRing3D.at(1), ring3D.at(1));
  ASSERT_EQ(resRing3D.at(2), ring3D.at(2));
  ASSERT_EQ(resRing3D.at(3), ring3D.at(3));

  // Геодезическое 3D кольцо
  GeoRing3D geoRing3D(
    { { Radian(5.), Radian(4.), 8. }, { Radian(2.), Radian(3.), 1. },
      { Radian(1.), Radian(5.), 1. }, { Radian(5.), Radian(4.), 8. } });

  // Конвертация в кольцо geos
  geosRing = PrimaryEntities::toGeosGeometry(geoRing3D);
  ASSERT_EQ(geosRing->getExteriorRing()->getNumPoints(), 4);
  ASSERT_EQ(geosRing->getNumInteriorRing(), 0);

  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(0).x, 4.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(0).y, 5.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(0).z, 8.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(1).x, 3.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(1).y, 2.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(1).z, 1.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(2).x, 5.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(2).y, 1.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(2).z, 1.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(3).x, 4.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(3).y, 5.);
  ASSERT_EQ(geosRing->getExteriorRing()->getCoordinateN(3).z, 8.);

  // Конвертация из geos
  GeoRing3D resGeoRing3D
    = PrimaryEntities::fromGeosGeometry<GeoRing3D>(geosRing.get());

  ASSERT_EQ(resGeoRing3D.at(0), geoRing3D.at(0));
  ASSERT_EQ(resGeoRing3D.at(1), geoRing3D.at(1));
  ASSERT_EQ(resGeoRing3D.at(2), geoRing3D.at(2));
  ASSERT_EQ(resGeoRing3D.at(3), geoRing3D.at(3));

  // Удаление элемента с конца (кольцо не может быть незамкнутым)
  ring2D.pop_back();

  ASSERT_THROW(
    PrimaryEntities::toGeosGeometry(ring2D), PrimaryEntitiesFailure);

  try
  {
    geosRing = PrimaryEntities::toGeosGeometry(ring2D);
  }
  catch (const PrimaryEntitiesFailure& failure)
  {
    ASSERT_EQ(IncorrectRing, failure.getCode());
    ASSERT_EQ(primaryLibTitle, failure.getLib());
  }

  // В кольце не может быть меньше 4 вершин
  ring2D.pop_back();
  ring2D.push_back(ring2D.at(0));

  ASSERT_THROW(
    PrimaryEntities::toGeosGeometry(ring2D), PrimaryEntitiesFailure);

  try
  {
    geosRing = PrimaryEntities::toGeosGeometry(ring2D);
  }
  catch (const PrimaryEntitiesFailure& failure)
  {
    ASSERT_EQ(IncorrectRing, failure.getCode());
    ASSERT_EQ(primaryLibTitle, failure.getLib());
  }
}
//==============================================================================
TEST(GeometryConverterTest, ToFromGeosPolygon)
{
  // Конвертация пустого полигона
  Polygon2D polygon;
  auto geosPolygon = PrimaryEntities::toGeosGeometry(polygon);
  ASSERT_TRUE(geosPolygon->isEmpty());

  // 2D полигон в декартовых координатах
  Polygon2D polygon2D(
    { { { 0., 0. }, { 3., 0. }, { 3., 3. }, { 0., 3. }, { 0., 0. } },
      { { 1., 1. }, { 2., 1. }, { 2., 2. }, { 1., 2. }, { 1., 1. } } });

  // Конвертация в полигон geos
  geosPolygon = PrimaryEntities::toGeosGeometry(polygon2D);

  ASSERT_EQ(geosPolygon->getExteriorRing()->getNumPoints(), 5);
  ASSERT_EQ(geosPolygon->getNumInteriorRing(), 1);
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getNumPoints(), 5);

  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(0).x,
            polygon2D.outer().at(0).getX());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(0).y,
            polygon2D.outer().at(0).getY());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(1).x,
            polygon2D.outer().at(1).getX());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(1).y,
            polygon2D.outer().at(1).getY());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(2).x,
            polygon2D.outer().at(2).getX());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(2).y,
            polygon2D.outer().at(2).getY());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(3).x,
            polygon2D.outer().at(3).getX());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(3).y,
            polygon2D.outer().at(3).getY());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(4).x,
            polygon2D.outer().at(4).getX());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(4).y,
            polygon2D.outer().at(4).getY());

  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(0).x,
            polygon2D.inners().at(0).at(0).getX());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(0).y,
            polygon2D.inners().at(0).at(0).getY());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(1).x,
            polygon2D.inners().at(0).at(1).getX());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(1).y,
            polygon2D.inners().at(0).at(1).getY());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(2).x,
            polygon2D.inners().at(0).at(2).getX());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(2).y,
            polygon2D.inners().at(0).at(2).getY());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(3).x,
            polygon2D.inners().at(0).at(3).getX());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(3).y,
            polygon2D.inners().at(0).at(3).getY());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(4).x,
            polygon2D.inners().at(0).at(4).getX());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(4).y,
            polygon2D.inners().at(0).at(4).getY());

  // Конвертация из geos
  Polygon2D resPolygon2D
    = PrimaryEntities::fromGeosGeometry<Polygon2D>(geosPolygon.get());

  ASSERT_EQ(resPolygon2D.outer().size(), polygon2D.outer().size());
  ASSERT_EQ(resPolygon2D.inners().size(), polygon2D.inners().size());
  ASSERT_EQ(resPolygon2D.inners().at(0).size(),
            polygon2D.inners().at(0).size());

  ASSERT_EQ(resPolygon2D.outer().at(0), polygon2D.outer().at(0));
  ASSERT_EQ(resPolygon2D.outer().at(1), polygon2D.outer().at(1));
  ASSERT_EQ(resPolygon2D.outer().at(2), polygon2D.outer().at(2));
  ASSERT_EQ(resPolygon2D.outer().at(3), polygon2D.outer().at(3));
  ASSERT_EQ(resPolygon2D.outer().at(4), polygon2D.outer().at(4));

  ASSERT_EQ(resPolygon2D.inners().at(0).at(0), polygon2D.inners().at(0).at(0));
  ASSERT_EQ(resPolygon2D.inners().at(0).at(1), polygon2D.inners().at(0).at(1));
  ASSERT_EQ(resPolygon2D.inners().at(0).at(2), polygon2D.inners().at(0).at(2));
  ASSERT_EQ(resPolygon2D.inners().at(0).at(3), polygon2D.inners().at(0).at(3));
  ASSERT_EQ(resPolygon2D.inners().at(0).at(4), polygon2D.inners().at(0).at(4));

  // Геодезический 2D полигон
  GeoPolygon2D geoPolygon2D(
    { { { Radian(0.), Radian(0.) }, { Radian(3.), Radian(0.) },
        { Radian(3.), Radian(3.) }, { Radian(0.), Radian(3.) },
        { Radian(0.), Radian(0.) } },
      { { Radian(1.), Radian(1.) }, { Radian(2.), Radian(1.) },
        { Radian(2.), Radian(2.) }, { Radian(1.), Radian(2.) },
        { Radian(1.), Radian(1.) } } });

  // Конвертация в полигон geos
  geosPolygon = PrimaryEntities::toGeosGeometry(geoPolygon2D);

  ASSERT_EQ(geosPolygon->getExteriorRing()->getNumPoints(), 5);
  ASSERT_EQ(geosPolygon->getNumInteriorRing(), 1);
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getNumPoints(), 5);

  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(0).x,
            geoPolygon2D.outer().at(0).getLongitude().get());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(0).y,
            geoPolygon2D.outer().at(0).getLatitude().get());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(1).x,
            geoPolygon2D.outer().at(1).getLongitude().get());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(1).y,
            geoPolygon2D.outer().at(1).getLatitude().get());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(2).x,
            geoPolygon2D.outer().at(2).getLongitude().get());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(2).y,
            geoPolygon2D.outer().at(2).getLatitude().get());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(3).x,
            geoPolygon2D.outer().at(3).getLongitude().get());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(3).y,
            geoPolygon2D.outer().at(3).getLatitude().get());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(4).x,
            geoPolygon2D.outer().at(4).getLongitude().get());
  ASSERT_EQ(geosPolygon->getExteriorRing()->getCoordinateN(4).y,
            geoPolygon2D.outer().at(4).getLatitude().get());

  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(0).x,
            geoPolygon2D.inners().at(0).at(0).getLongitude().get());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(0).y,
            geoPolygon2D.inners().at(0).at(0).getLatitude().get());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(1).x,
            geoPolygon2D.inners().at(0).at(1).getLongitude().get());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(1).y,
            geoPolygon2D.inners().at(0).at(1).getLatitude().get());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(2).x,
            geoPolygon2D.inners().at(0).at(2).getLongitude().get());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(2).y,
            geoPolygon2D.inners().at(0).at(2).getLatitude().get());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(3).x,
            geoPolygon2D.inners().at(0).at(3).getLongitude().get());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(3).y,
            geoPolygon2D.inners().at(0).at(3).getLatitude().get());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(4).x,
            geoPolygon2D.inners().at(0).at(4).getLongitude().get());
  ASSERT_EQ(geosPolygon->getInteriorRingN(0)->getCoordinateN(4).y,
            geoPolygon2D.inners().at(0).at(4).getLatitude().get());

  // Конвертация из geos
  GeoPolygon2D resGeoPolygon2D
    = PrimaryEntities::fromGeosGeometry<GeoPolygon2D>(geosPolygon.get());

  ASSERT_EQ(resGeoPolygon2D.outer().size(), geoPolygon2D.outer().size());
  ASSERT_EQ(resGeoPolygon2D.inners().size(), geoPolygon2D.inners().size());
  ASSERT_EQ(resGeoPolygon2D.inners().at(0).size(),
            geoPolygon2D.inners().at(0).size());

  ASSERT_EQ(resGeoPolygon2D.outer().at(0), geoPolygon2D.outer().at(0));
  ASSERT_EQ(resGeoPolygon2D.outer().at(1), geoPolygon2D.outer().at(1));
  ASSERT_EQ(resGeoPolygon2D.outer().at(2), geoPolygon2D.outer().at(2));
  ASSERT_EQ(resGeoPolygon2D.outer().at(3), geoPolygon2D.outer().at(3));
  ASSERT_EQ(resGeoPolygon2D.outer().at(4), geoPolygon2D.outer().at(4));

  ASSERT_EQ(resGeoPolygon2D.inners().at(0).at(0),
            geoPolygon2D.inners().at(0).at(0));
  ASSERT_EQ(resGeoPolygon2D.inners().at(0).at(1),
            geoPolygon2D.inners().at(0).at(1));
  ASSERT_EQ(resGeoPolygon2D.inners().at(0).at(2),
            geoPolygon2D.inners().at(0).at(2));
  ASSERT_EQ(resGeoPolygon2D.inners().at(0).at(3),
            geoPolygon2D.inners().at(0).at(3));
  ASSERT_EQ(resGeoPolygon2D.inners().at(0).at(4),
            geoPolygon2D.inners().at(0).at(4));
}
//==============================================================================
TEST(GeometryConverterTest, ToFromGeosMultiPolygon)
{
  // Конвертация пустого мультиполигона
  MultiPolygon2D multiPolygon;
  auto geosMultiPolygon = PrimaryEntities::toGeosGeometry(multiPolygon);
  ASSERT_TRUE(geosMultiPolygon->isEmpty());

    // 2D мультиполигон в декартовых координатах
  MultiPolygon2D polygons2D(
    { { { { 0., 0. }, { 3., 0. }, { 3., 3. }, { 0., 3. }, { 0., 0. } },
      { { 1., 1. }, { 2., 1. }, { 2., 2. }, { 1., 2. }, { 1., 1. } } },
    { { { 4., 0. }, { 5., 0. }, { 5., 1. }, { 4., 1. }, { 4., 0. } } } });

  // Конвертация в мультиполигон geos
  auto geosPolygons = PrimaryEntities::toGeosGeometry(polygons2D);

  ASSERT_EQ(geosPolygons->getNumGeometries(), 2);
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getNumPoints(), 5);
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getNumInteriorRing(), 1);
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getNumPoints(), 5);
  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getNumPoints(), 5);

  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(0).x,
    polygons2D.at(0).outer().at(0).getX());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(0).y,
    polygons2D.at(0).outer().at(0).getY());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(1).x,
    polygons2D.at(0).outer().at(1).getX());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(1).y,
    polygons2D.at(0).outer().at(1).getY());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(2).x,
    polygons2D.at(0).outer().at(2).getX());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(2).y,
    polygons2D.at(0).outer().at(2).getY());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(3).x,
    polygons2D.at(0).outer().at(3).getX());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(3).y,
    polygons2D.at(0).outer().at(3).getY());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(4).x,
    polygons2D.at(0).outer().at(4).getX());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(4).y,
    polygons2D.at(0).outer().at(4).getY());

  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getCoordinateN(0).x,
    polygons2D.at(0).inners().at(0).at(0).getX());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getCoordinateN(0).y,
    polygons2D.at(0).inners().at(0).at(0).getY());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getCoordinateN(1).x,
    polygons2D.at(0).inners().at(0).at(1).getX());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getCoordinateN(1).y,
    polygons2D.at(0).inners().at(0).at(1).getY());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getCoordinateN(2).x,
    polygons2D.at(0).inners().at(0).at(2).getX());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getCoordinateN(2).y,
    polygons2D.at(0).inners().at(0).at(2).getY());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getCoordinateN(3).x,
    polygons2D.at(0).inners().at(0).at(3).getX());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getCoordinateN(3).y,
    polygons2D.at(0).inners().at(0).at(3).getY());

  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getCoordinateN(0).x,
    polygons2D.at(1).outer().at(0).getX());
  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getCoordinateN(0).y,
    polygons2D.at(1).outer().at(0).getY());
  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getCoordinateN(1).x,
    polygons2D.at(1).outer().at(1).getX());
  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getCoordinateN(1).y,
    polygons2D.at(1).outer().at(1).getY());
  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getCoordinateN(2).x,
    polygons2D.at(1).outer().at(2).getX());
  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getCoordinateN(2).y,
    polygons2D.at(1).outer().at(2).getY());
  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getCoordinateN(3).x,
    polygons2D.at(1).outer().at(3).getX());
  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getCoordinateN(3).y,
    polygons2D.at(1).outer().at(3).getY());

  // Конвертация из geos
  MultiPolygon2D resPolygons2D
    = PrimaryEntities::fromGeosGeometry<MultiPolygon2D>(geosPolygons.get());

  ASSERT_EQ(resPolygons2D.size(), polygons2D.size());
  ASSERT_EQ(resPolygons2D.at(0).outer().size(),
            polygons2D.at(0).outer().size());
  ASSERT_EQ(resPolygons2D.at(0).inners().size(),
            polygons2D.at(0).inners().size());
  ASSERT_EQ(resPolygons2D.at(0).inners().at(0).size(),
            polygons2D.at(0).inners().at(0).size());
  ASSERT_EQ(resPolygons2D.at(1).outer().size(),
            polygons2D.at(1).outer().size());
  ASSERT_EQ(resPolygons2D.at(1).inners().size(),
            polygons2D.at(1).inners().size());

  ASSERT_EQ(resPolygons2D.at(0).outer().at(0), polygons2D.at(0).outer().at(0));
  ASSERT_EQ(resPolygons2D.at(0).outer().at(1), polygons2D.at(0).outer().at(1));
  ASSERT_EQ(resPolygons2D.at(0).outer().at(2), polygons2D.at(0).outer().at(2));
  ASSERT_EQ(resPolygons2D.at(0).outer().at(3), polygons2D.at(0).outer().at(3));
  ASSERT_EQ(resPolygons2D.at(0).outer().at(4), polygons2D.at(0).outer().at(4));

  ASSERT_EQ(resPolygons2D.at(0).inners().at(0).at(0),
            polygons2D.at(0).inners().at(0).at(0));
  ASSERT_EQ(resPolygons2D.at(0).inners().at(0).at(1),
            polygons2D.at(0).inners().at(0).at(1));
  ASSERT_EQ(resPolygons2D.at(0).inners().at(0).at(2),
            polygons2D.at(0).inners().at(0).at(2));
  ASSERT_EQ(resPolygons2D.at(0).inners().at(0).at(3),
            polygons2D.at(0).inners().at(0).at(3));
  ASSERT_EQ(resPolygons2D.at(0).inners().at(0).at(4),
            polygons2D.at(0).inners().at(0).at(4));

  ASSERT_EQ(resPolygons2D.at(1).outer().at(0), polygons2D.at(1).outer().at(0));
  ASSERT_EQ(resPolygons2D.at(1).outer().at(1), polygons2D.at(1).outer().at(1));
  ASSERT_EQ(resPolygons2D.at(1).outer().at(2), polygons2D.at(1).outer().at(2));
  ASSERT_EQ(resPolygons2D.at(1).outer().at(3), polygons2D.at(1).outer().at(3));
  ASSERT_EQ(resPolygons2D.at(1).outer().at(4), polygons2D.at(1).outer().at(4));

  // Геодезический 2D мультиполигон
  GeoMultiPolygon2D geoPolygons2D(
    { { { { Radian(0.), Radian(0.) }, { Radian(3.), Radian(0.) },
          { Radian(3.), Radian(3.) }, { Radian(0.), Radian(3.) },
          { Radian(0.), Radian(0.) } },
        { { Radian(1.), Radian(1.) }, { Radian(2.), Radian(1.) },
          { Radian(2.), Radian(2.) }, { Radian(1.), Radian(2.) },
          { Radian(1.), Radian(1.) } } },
      { { { Radian(4.), Radian(0.) }, { Radian(5.), Radian(0.) },
          { Radian(5.), Radian(1.) }, { Radian(4.), Radian(1.) },
          { Radian(4.), Radian(0.) } } } });

  // Конвертация в мультиполигон geos
  geosPolygons = PrimaryEntities::toGeosGeometry(geoPolygons2D);

  ASSERT_EQ(geosPolygons->getNumGeometries(), 2);
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getNumPoints(), 5);
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getNumInteriorRing(), 1);
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getNumPoints(), 5);
  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getNumPoints(), 5);

  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(0).x,
    geoPolygons2D.at(0).outer().at(0).getLongitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(0).y,
    geoPolygons2D.at(0).outer().at(0).getLatitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(1).x,
    geoPolygons2D.at(0).outer().at(1).getLongitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(1).y,
    geoPolygons2D.at(0).outer().at(1).getLatitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(2).x,
    geoPolygons2D.at(0).outer().at(2).getLongitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(2).y,
    geoPolygons2D.at(0).outer().at(2).getLatitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(3).x,
    geoPolygons2D.at(0).outer().at(3).getLongitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(3).y,
    geoPolygons2D.at(0).outer().at(3).getLatitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(4).x,
    geoPolygons2D.at(0).outer().at(4).getLongitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getExteriorRing()->getCoordinateN(4).y,
    geoPolygons2D.at(0).outer().at(4).getLatitude().get());

  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getCoordinateN(0).x,
    geoPolygons2D.at(0).inners().at(0).at(0).getLongitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getCoordinateN(0).y,
    geoPolygons2D.at(0).inners().at(0).at(0).getLatitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getCoordinateN(1).x,
    geoPolygons2D.at(0).inners().at(0).at(1).getLongitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getCoordinateN(1).y,
    geoPolygons2D.at(0).inners().at(0).at(1).getLatitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getCoordinateN(2).x,
    geoPolygons2D.at(0).inners().at(0).at(2).getLongitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getCoordinateN(2).y,
    geoPolygons2D.at(0).inners().at(0).at(2).getLatitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getCoordinateN(3).x,
    geoPolygons2D.at(0).inners().at(0).at(3).getLongitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(0)->getInteriorRingN(0)->getCoordinateN(3).y,
    geoPolygons2D.at(0).inners().at(0).at(3).getLatitude().get());

  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getCoordinateN(0).x,
    geoPolygons2D.at(1).outer().at(0).getLongitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getCoordinateN(0).y,
    geoPolygons2D.at(1).outer().at(0).getLatitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getCoordinateN(1).x,
    geoPolygons2D.at(1).outer().at(1).getLongitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getCoordinateN(1).y,
    geoPolygons2D.at(1).outer().at(1).getLatitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getCoordinateN(2).x,
    geoPolygons2D.at(1).outer().at(2).getLongitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getCoordinateN(2).y,
    geoPolygons2D.at(1).outer().at(2).getLatitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getCoordinateN(3).x,
    geoPolygons2D.at(1).outer().at(3).getLongitude().get());
  ASSERT_EQ(
    geosPolygons->getGeometryN(1)->getExteriorRing()->getCoordinateN(3).y,
    geoPolygons2D.at(1).outer().at(3).getLatitude().get());

  // Конвертация из geos
  GeoMultiPolygon2D resGeoPolygons2D
    = PrimaryEntities::fromGeosGeometry<GeoMultiPolygon2D>(geosPolygons.get());

  ASSERT_EQ(resGeoPolygons2D.size(), geoPolygons2D.size());
  ASSERT_EQ(resGeoPolygons2D.at(0).outer().size(),
            geoPolygons2D.at(0).outer().size());
  ASSERT_EQ(resGeoPolygons2D.at(0).inners().size(),
            geoPolygons2D.at(0).inners().size());
  ASSERT_EQ(resGeoPolygons2D.at(0).inners().at(0).size(),
            geoPolygons2D.at(0).inners().at(0).size());
  ASSERT_EQ(resGeoPolygons2D.at(1).outer().size(),
            geoPolygons2D.at(1).outer().size());
  ASSERT_EQ(resGeoPolygons2D.at(1).inners().size(),
            geoPolygons2D.at(1).inners().size());

  ASSERT_EQ(resGeoPolygons2D.at(0).outer().at(0),
            geoPolygons2D.at(0).outer().at(0));
  ASSERT_EQ(resGeoPolygons2D.at(0).outer().at(1),
            geoPolygons2D.at(0).outer().at(1));
  ASSERT_EQ(resGeoPolygons2D.at(0).outer().at(2),
            geoPolygons2D.at(0).outer().at(2));
  ASSERT_EQ(resGeoPolygons2D.at(0).outer().at(3),
            geoPolygons2D.at(0).outer().at(3));
  ASSERT_EQ(resGeoPolygons2D.at(0).outer().at(4),
            geoPolygons2D.at(0).outer().at(4));

  ASSERT_EQ(resGeoPolygons2D.at(0).inners().at(0).at(0),
            geoPolygons2D.at(0).inners().at(0).at(0));
  ASSERT_EQ(resGeoPolygons2D.at(0).inners().at(0).at(1),
            geoPolygons2D.at(0).inners().at(0).at(1));
  ASSERT_EQ(resGeoPolygons2D.at(0).inners().at(0).at(2),
            geoPolygons2D.at(0).inners().at(0).at(2));
  ASSERT_EQ(resGeoPolygons2D.at(0).inners().at(0).at(3),
            geoPolygons2D.at(0).inners().at(0).at(3));
  ASSERT_EQ(resGeoPolygons2D.at(0).inners().at(0).at(4),
            geoPolygons2D.at(0).inners().at(0).at(4));

  ASSERT_EQ(resGeoPolygons2D.at(1).outer().at(0),
            geoPolygons2D.at(1).outer().at(0));
  ASSERT_EQ(resGeoPolygons2D.at(1).outer().at(1),
            geoPolygons2D.at(1).outer().at(1));
  ASSERT_EQ(resGeoPolygons2D.at(1).outer().at(2),
            geoPolygons2D.at(1).outer().at(2));
  ASSERT_EQ(resGeoPolygons2D.at(1).outer().at(3),
            geoPolygons2D.at(1).outer().at(3));
  ASSERT_EQ(resGeoPolygons2D.at(1).outer().at(4),
            geoPolygons2D.at(1).outer().at(4));
}
//==============================================================================
TEST(GeometryConverterTest, ToFromGeosBox)
{
  // 2D коробка в декартовых координатах
  Box2D box2D({1., 2.}, {4., 3.});

  // Конвертация в коробку geos
  auto geosBox = PrimaryEntities::toGeosGeometry(box2D);
  ASSERT_EQ(geosBox->getExteriorRing()->getNumPoints(), 5);
  ASSERT_EQ(geosBox->getNumInteriorRing(), 0);

  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(0).x, 1.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(0).y, 2.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(1).x, 1.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(1).y, 3.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(2).x, 4.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(2).y, 3.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(3).x, 4.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(3).y, 2.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(4).x, 1.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(4).y, 2.);

  // Конвертация из geos
  Box2D resBox2D = PrimaryEntities::fromGeosGeometry<Box2D>(geosBox.get());

  ASSERT_EQ(resBox2D.min_corner(), box2D.min_corner());
  ASSERT_EQ(resBox2D.max_corner(), box2D.max_corner());

  // Геодезическая 2D коробка
  GeoBox2D geoBox2D({Radian(1.), Radian(2.)}, {Radian(4.), Radian(3.)});

  // Конвертация в коробку geos
  geosBox = PrimaryEntities::toGeosGeometry(geoBox2D);
  ASSERT_EQ(geosBox->getExteriorRing()->getNumPoints(), 5);
  ASSERT_EQ(geosBox->getNumInteriorRing(), 0);

  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(0).x, 2.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(0).y, 1.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(1).x, 2.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(1).y, 4.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(2).x, 3.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(2).y, 4.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(3).x, 3.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(3).y, 1.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(4).x, 2.);
  ASSERT_EQ(geosBox->getExteriorRing()->getCoordinateN(4).y, 1.);

  // Конвертация из geos
  GeoBox2D resGeoBox2D
    = PrimaryEntities::fromGeosGeometry<GeoBox2D>(geosBox.get());

  ASSERT_EQ(resBox2D.min_corner(), box2D.min_corner());
  ASSERT_EQ(resBox2D.max_corner(), box2D.max_corner());
}
//==============================================================================
} //! Tests
} //! PrimaryEntities
//==============================================================================