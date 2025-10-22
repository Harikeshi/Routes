#include <PrimaryEntities/AtEntitiesType.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
//! Тесты для проверки правильности работы функционала определения типа 
//! геометрии (сущности)
//==============================================================================
namespace PrimaryEntities
{
namespace Tests
{
//==============================================================================
class OperationsAtEntitiesTypeTest : public ::testing::Test
{
//==============================================================================
protected:
//==============================================================================
  void SetUp( )
  {
  }
//==============================================================================
  void TearDown( )
  {
  }
//==============================================================================
  //! GeometryEntities
  Point2D           point2D;
  MultiPoint2D      multiPoint2D;
  Linestring2D      line2D;
  MultiLinestring2D multiline2D;
  Box2D             box2D;
  Ring2D            ring2D;
  Polygon2D         polygon2D;
  MultiPolygon2D    multiPolygon2D;

  Point3D           point3D;
  MultiPoint3D      multiPoint3D;
  Linestring3D      line3D;
  MultiLinestring3D multiline3D;
  Box3D             box3D;
  Ring3D            ring3D;
  Polygon3D         polygon3D;
  MultiPolygon3D    multiPolygon3D;

  //! GeodesicEntities
  GeoPoint2D            geoPoint2D;
  GeoMultiPoint2D       geoMultiPoint2D;
  GeoLinestring2D       geoLine2D;
  GeoMultiLinestring2D  geoMultiline2D;
  GeoBox2D              geoBox2D;
  GeoRing2D             geoRing2D;
  GeoPolygon2D          geoPolygon2D;
  GeoMultiPolygon2D     geoMultiPolygon2D;

  GeoPoint3D            geoPoint3D;
  //==============================================================================
};
//==============================================================================
/*!
* \brief Тестирование функции determineEntityType
*/
TEST_F( OperationsAtEntitiesTypeTest, DetermineEntityType )
{
  //! Проверка на Geometry
  ASSERT_EQ( determineEntityType( point2D )        & GeometryEntity, GeometryEntity );
  ASSERT_EQ( determineEntityType( multiPoint2D )   & GeometryEntity, GeometryEntity );
  ASSERT_EQ( determineEntityType( line2D )         & GeometryEntity, GeometryEntity );
  ASSERT_EQ( determineEntityType( multiline2D )    & GeometryEntity, GeometryEntity );
  ASSERT_EQ( determineEntityType( box2D )          & GeometryEntity, GeometryEntity );
  ASSERT_EQ( determineEntityType( ring2D )         & GeometryEntity, GeometryEntity );
  ASSERT_EQ( determineEntityType( polygon2D )      & GeometryEntity, GeometryEntity );
  ASSERT_EQ( determineEntityType( multiPolygon2D ) & GeometryEntity, GeometryEntity );
  ASSERT_EQ( determineEntityType( point3D )        & GeometryEntity, GeometryEntity );
  ASSERT_EQ( determineEntityType( multiPoint3D )   & GeometryEntity, GeometryEntity );
  ASSERT_EQ( determineEntityType( line3D )         & GeometryEntity, GeometryEntity );
  ASSERT_EQ( determineEntityType( multiline3D )    & GeometryEntity, GeometryEntity );
  ASSERT_EQ( determineEntityType( box3D )          & GeometryEntity, GeometryEntity );
  ASSERT_EQ( determineEntityType( ring3D )         & GeometryEntity, GeometryEntity );
  ASSERT_EQ( determineEntityType( polygon3D )      & GeometryEntity, GeometryEntity );
  ASSERT_EQ( determineEntityType( multiPolygon3D ) & GeometryEntity, GeometryEntity );

  //! Проверка на GeodesicEntity
  ASSERT_EQ( determineEntityType( geoPoint2D )        & GeodesicEntity, GeodesicEntity );
  ASSERT_EQ( determineEntityType( geoMultiPoint2D )   & GeodesicEntity, GeodesicEntity );
  ASSERT_EQ( determineEntityType( geoLine2D )         & GeodesicEntity, GeodesicEntity );
  ASSERT_EQ( determineEntityType( geoMultiline2D )    & GeodesicEntity, GeodesicEntity );
  ASSERT_EQ( determineEntityType( geoBox2D )          & GeodesicEntity, GeodesicEntity );
  ASSERT_EQ( determineEntityType( geoRing2D )         & GeodesicEntity, GeodesicEntity );
  ASSERT_EQ( determineEntityType( geoPolygon2D )      & GeodesicEntity, GeodesicEntity );
  ASSERT_EQ( determineEntityType( geoMultiPolygon2D ) & GeodesicEntity, GeodesicEntity );
  ASSERT_EQ( determineEntityType( geoPoint3D )        & GeodesicEntity, GeodesicEntity );

  //! Проверка на 2D
  ASSERT_EQ( determineEntityType( point2D )           & Dim2DEntity, Dim2DEntity );
  ASSERT_EQ( determineEntityType( multiPoint2D )      & Dim2DEntity, Dim2DEntity );
  ASSERT_EQ( determineEntityType( line2D )            & Dim2DEntity, Dim2DEntity );
  ASSERT_EQ( determineEntityType( multiline2D )       & Dim2DEntity, Dim2DEntity );
  ASSERT_EQ( determineEntityType( box2D )             & Dim2DEntity, Dim2DEntity );
  ASSERT_EQ( determineEntityType( ring2D )            & Dim2DEntity, Dim2DEntity );
  ASSERT_EQ( determineEntityType( polygon2D )         & Dim2DEntity, Dim2DEntity );
  ASSERT_EQ( determineEntityType( multiPolygon2D )    & Dim2DEntity, Dim2DEntity );
  ASSERT_EQ( determineEntityType( geoPoint2D )        & Dim2DEntity, Dim2DEntity );
  ASSERT_EQ( determineEntityType( geoMultiPoint2D )   & Dim2DEntity, Dim2DEntity );
  ASSERT_EQ( determineEntityType( geoLine2D )         & Dim2DEntity, Dim2DEntity );
  ASSERT_EQ( determineEntityType( geoMultiline2D )    & Dim2DEntity, Dim2DEntity );
  ASSERT_EQ( determineEntityType( geoBox2D )          & Dim2DEntity, Dim2DEntity );
  ASSERT_EQ( determineEntityType( geoRing2D )         & Dim2DEntity, Dim2DEntity );
  ASSERT_EQ( determineEntityType( geoPolygon2D )      & Dim2DEntity, Dim2DEntity );
  ASSERT_EQ( determineEntityType( geoMultiPolygon2D ) & Dim2DEntity, Dim2DEntity );

  //! Проверка на 3D
  ASSERT_EQ( determineEntityType( point3D )        & Dim3DEntity, Dim3DEntity );
  ASSERT_EQ( determineEntityType( multiPoint3D )   & Dim3DEntity, Dim3DEntity );
  ASSERT_EQ( determineEntityType( line3D )         & Dim3DEntity, Dim3DEntity );
  ASSERT_EQ( determineEntityType( multiline3D )    & Dim3DEntity, Dim3DEntity );
  ASSERT_EQ( determineEntityType( box3D )          & Dim3DEntity, Dim3DEntity );
  ASSERT_EQ( determineEntityType( ring3D )         & Dim3DEntity, Dim3DEntity );
  ASSERT_EQ( determineEntityType( polygon3D )      & Dim3DEntity, Dim3DEntity );
  ASSERT_EQ( determineEntityType( multiPolygon3D ) & Dim3DEntity, Dim3DEntity );
  ASSERT_EQ( determineEntityType( geoPoint3D )     & Dim3DEntity, Dim3DEntity );

  //! Проверка, что точечный
  ASSERT_EQ( determineEntityType( point2D )          & DottedEntity, DottedEntity );
  ASSERT_EQ( determineEntityType( multiPoint2D )     & DottedEntity, DottedEntity );
  ASSERT_EQ( determineEntityType( point3D )          & DottedEntity, DottedEntity );
  ASSERT_EQ( determineEntityType( multiPoint3D )     & DottedEntity, DottedEntity );
  ASSERT_EQ( determineEntityType( geoPoint2D )       & DottedEntity, DottedEntity );
  ASSERT_EQ( determineEntityType( geoMultiPoint2D )  & DottedEntity, DottedEntity );
  ASSERT_EQ( determineEntityType( geoPoint3D )       & DottedEntity, DottedEntity );

  //! Проверка, что линейный
  ASSERT_EQ( determineEntityType( line2D )         & LinearEntity, LinearEntity );
  ASSERT_EQ( determineEntityType( line3D )         & LinearEntity, LinearEntity );
  ASSERT_EQ( determineEntityType( geoLine2D )      & LinearEntity, LinearEntity );
  ASSERT_EQ( determineEntityType( multiline2D )    & LinearEntity, LinearEntity );
  ASSERT_EQ( determineEntityType( multiline3D )    & LinearEntity, LinearEntity );
  ASSERT_EQ( determineEntityType( geoMultiline2D ) & LinearEntity, LinearEntity );

  //! Проверка, что площадной
  ASSERT_EQ( determineEntityType( box2D )             & ArealEntity, ArealEntity );
  ASSERT_EQ( determineEntityType( ring2D )            & ArealEntity, ArealEntity );
  ASSERT_EQ( determineEntityType( polygon2D )         & ArealEntity, ArealEntity );
  ASSERT_EQ( determineEntityType( multiPolygon2D )    & ArealEntity, ArealEntity );
  ASSERT_EQ( determineEntityType( box3D )             & ArealEntity, ArealEntity );
  ASSERT_EQ( determineEntityType( ring3D )            & ArealEntity, ArealEntity );
  ASSERT_EQ( determineEntityType( polygon3D )         & ArealEntity, ArealEntity );
  ASSERT_EQ( determineEntityType( multiPolygon3D )    & ArealEntity, ArealEntity );
  ASSERT_EQ( determineEntityType( geoBox2D )          & ArealEntity, ArealEntity );
  ASSERT_EQ( determineEntityType( geoRing2D )         & ArealEntity, ArealEntity );
  ASSERT_EQ( determineEntityType( geoPolygon2D )      & ArealEntity, ArealEntity );
  ASSERT_EQ( determineEntityType( geoMultiPolygon2D ) & ArealEntity, ArealEntity );

  //! Проверка, что одиночная
  ASSERT_EQ( determineEntityType( point2D )      & SingleEntity, SingleEntity );
  ASSERT_EQ( determineEntityType( line2D )       & SingleEntity, SingleEntity );
  ASSERT_EQ( determineEntityType( box2D )        & SingleEntity, SingleEntity );
  ASSERT_EQ( determineEntityType( ring2D )       & SingleEntity, SingleEntity );
  ASSERT_EQ( determineEntityType( polygon2D )    & SingleEntity, SingleEntity );
  ASSERT_EQ( determineEntityType( point3D )      & SingleEntity, SingleEntity );
  ASSERT_EQ( determineEntityType( line3D )       & SingleEntity, SingleEntity );
  ASSERT_EQ( determineEntityType( box3D )        & SingleEntity, SingleEntity );
  ASSERT_EQ( determineEntityType( ring3D )       & SingleEntity, SingleEntity );
  ASSERT_EQ( determineEntityType( polygon3D )    & SingleEntity, SingleEntity );
  ASSERT_EQ( determineEntityType( geoPoint2D )   & SingleEntity, SingleEntity );
  ASSERT_EQ( determineEntityType( geoLine2D )    & SingleEntity, SingleEntity );
  ASSERT_EQ( determineEntityType( geoBox2D )     & SingleEntity, SingleEntity );
  ASSERT_EQ( determineEntityType( geoRing2D )    & SingleEntity, SingleEntity );
  ASSERT_EQ( determineEntityType( geoPolygon2D ) & SingleEntity, SingleEntity );
  ASSERT_EQ( determineEntityType( geoPoint3D )   & SingleEntity, SingleEntity );

  //! Проверка, что коллекция
  ASSERT_EQ( determineEntityType( multiPoint2D )      & CollectionEntity, CollectionEntity );
  ASSERT_EQ( determineEntityType( multiPolygon2D )    & CollectionEntity, CollectionEntity );
  ASSERT_EQ( determineEntityType( multiPoint3D )      & CollectionEntity, CollectionEntity );
  ASSERT_EQ( determineEntityType( multiPolygon3D )    & CollectionEntity, CollectionEntity );
  ASSERT_EQ( determineEntityType( geoMultiPoint2D )   & CollectionEntity, CollectionEntity );
  ASSERT_EQ( determineEntityType( geoMultiPolygon2D ) & CollectionEntity, CollectionEntity );
  ASSERT_EQ( determineEntityType( multiline2D )       & CollectionEntity, CollectionEntity );
  ASSERT_EQ( determineEntityType( multiline3D )       & CollectionEntity, CollectionEntity );
  ASSERT_EQ( determineEntityType( geoMultiline2D )    & CollectionEntity, CollectionEntity );
}
//==============================================================================
/*!
* \brief Тестирование функции isEntityType
*/
TEST_F( OperationsAtEntitiesTypeTest, isEntityType )
{
  ASSERT_TRUE( isEntityType( point2D, GeometryEntity ) );
  ASSERT_TRUE( isEntityType( point2D, Dim2DEntity ) );
  ASSERT_TRUE( isEntityType( point2D, DottedEntity ) );
  ASSERT_TRUE( isEntityType( point2D, SingleEntity ) );
  ASSERT_TRUE( isEntityType( point2D, GeometryEntity | Dim2DEntity ) );
  ASSERT_TRUE( isEntityType( point2D, GeometryEntity | DottedEntity ) );
  ASSERT_TRUE( isEntityType( point2D, GeometryEntity | SingleEntity ) );
  ASSERT_TRUE( isEntityType( point2D, Dim2DEntity | DottedEntity ) );
  ASSERT_TRUE( isEntityType( point2D, Dim2DEntity | SingleEntity ) );
  ASSERT_TRUE( isEntityType( point2D, DottedEntity | SingleEntity ) );
  ASSERT_TRUE( isEntityType( point2D, GeometryEntity | Dim2DEntity | DottedEntity ) );
  ASSERT_TRUE( isEntityType( point2D, GeometryEntity | Dim2DEntity | SingleEntity ) );
  ASSERT_TRUE( isEntityType( point2D, GeometryEntity | DottedEntity | SingleEntity ) );
  ASSERT_TRUE( isEntityType( point2D, Dim2DEntity | DottedEntity | SingleEntity ) );
  ASSERT_TRUE( isEntityType( point2D, GeometryEntity | Dim2DEntity | DottedEntity | SingleEntity ) );

  ASSERT_FALSE( isEntityType( point2D, GeodesicEntity ) );
  ASSERT_FALSE( isEntityType( point2D, GeodesicEntity | Dim2DEntity ) );
  ASSERT_FALSE( isEntityType( point2D, GeometryEntity | Dim3DEntity ) );
  ASSERT_FALSE( isEntityType( point2D, GeometryEntity | Dim3DEntity | DottedEntity ) );
  ASSERT_FALSE( isEntityType( point2D, GeometryEntity | Dim2DEntity | DottedEntity | CollectionEntity ) );
}
//==============================================================================
} //! Tests
} //! PrimaryEntities
//==============================================================================

