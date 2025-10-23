#include <AbstractOperations/ConvertEachPoint.hpp>
//! PrimaryEntities
#include <PrimaryEntities/Equalities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
#include <PrimaryEntities/GeometryEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace AbstractOperations 
{
namespace Tests
{
//==============================================================================
/*!
 *\brief Перегонка классической точки в географическую 
 *\details Координаты обычной точки вопринимаются как радианы с поворотом осей
 *\param[in] p - классическая точка
 *\return Географическая точка
 */
GeoPoint2D toGeo( const Point2D& p )
{
  return GeoPoint2D( Radian( p.getY( ) ), Radian( p.getX( ) ) );
}
//==============================================================================
/*!
 *\brief Расширение плоской точки до трехмерной
 *\param[in] p - двумерная точка
 *\return трехмерная точка
 */
Point3D toSpatial( const Point2D& p, const double h )
{
  return Point3D( p.getX( ), p.getY( ), h );
}
//==============================================================================
/*!
 * \brief Тестирование перегонки геометрий
 */
TEST( WithEachPointTest, FlatToGeoConvert )
{
  Point2D p1( 1., 2. );
  Point2D p2( 2., 3. );
  Point2D p3( 3., 5. );

  GeoPoint2D g1( Radian( 2. ), Radian( 1. ) );
  GeoPoint2D g2( Radian( 3. ), Radian( 2. ) );
  GeoPoint2D g3( Radian( 5. ), Radian( 3. ) );

  GeoMultiPoint2D multPoint;
  GeoLinestring2D line;
  std::vector<GeoPoint2D> vect;
  GeoMultiLinestring2D multLine;
  GeoRing2D ring;
  GeoPolygon2D poly;
  GeoMultiPolygon2D multPoly;
  GeoBox2D box;

  GeoMultiPoint2D controlMultPoint( { g1, g2, g3 } );
  GeoLinestring2D controlLine( { g1, g2, g3 } );
  std::vector<GeoPoint2D> controlVect( { g1, g2, g3 } );
  GeoMultiLinestring2D controlMultLine( { { g1, g2 }, { g1, g3 } } );
  GeoRing2D controlRing( { g1, g2, g3, g1 } );
  GeoPolygon2D controlPoly( { controlRing, {g1}, {g2} } );
  GeoMultiPolygon2D controlMultPoly( { controlPoly,{ controlRing,{ g1 } } } );
  GeoBox2D controlBox( g1, g2 );

  convertEachPoint( MultiPoint2D ( { p1, p2, p3 } ), multPoint, toGeo );
  ASSERT_EQ( multPoint, controlMultPoint );
  convertEachPoint( Linestring2D ( { p1, p2, p3 } ), line, toGeo );
  ASSERT_EQ( line, controlLine );
  convertEachPoint( std::vector<Point2D> ( { p1, p2, p3 } ), vect, toGeo );
  ASSERT_EQ( vect, controlVect );
  convertEachPoint( MultiLinestring2D ( { { p1, p2 },{ p1, p3 } } ), multLine, toGeo );
  ASSERT_EQ( multLine, controlMultLine );
  Ring2D flatRing( { p1, p2, p3, p1 } );
  convertEachPoint( flatRing, ring, toGeo );
  ASSERT_EQ( ring, controlRing );
  Polygon2D flatPoly( { flatRing,{ p1 },{ p2 } } );
  convertEachPoint( flatPoly, poly, toGeo );
  ASSERT_EQ( poly, controlPoly );
  convertEachPoint( MultiPolygon2D ( { flatPoly,{ flatRing,{ p1 } } } ), multPoly, toGeo );
  ASSERT_EQ( multPoly, controlMultPoly );
  convertEachPoint( Box2D( p1, p2 ), box, toGeo );
  ASSERT_EQ( box, controlBox );
}
//==============================================================================
TEST( WithEachPointTest, FlatToSpatialConvert )
{
  Point2D p1( 1., 2. );
  Point2D p2( 2., 3. );
  Point2D p3( 3., 5. );

  Point3D s1( 1., 2., 1 );
  Point3D s2( 2., 3., 1 );
  Point3D s3( 3., 5., 1 );

  MultiPoint3D multPoint;
  Linestring3D line;
  std::vector<Point3D> vect;
  MultiLinestring3D multLine;
  Ring3D ring;
  Polygon3D poly;
  MultiPolygon3D multPoly;
  Box3D box;

  MultiPoint3D controlMultPoint( { s1, s2, s3 } );
  Linestring3D controlLine( { s1, s2, s3 } );
  std::vector<Point3D> controlVect( { s1, s2, s3 } );
  MultiLinestring3D controlMultLine( { { s1, s2 },{ s1, s3 } } );
  Ring3D controlRing( { s1, s2, s3, s1 } );
  Polygon3D controlPoly( { controlRing,{ s1 },{ s2 } } );
  MultiPolygon3D controlMultPoly( { controlPoly,{ controlRing,{ s1 } } } );
  Box3D controlBox( s1, s2 );

  // если функция преобразования точки требует несколько параметров, нужна лямбда
  double h = 1;
  convertEachPoint( MultiPoint2D( { p1, p2, p3 } ), multPoint,
    [h] ( const Point2D& p ) { return toSpatial( p, h ); } );
  ASSERT_EQ( multPoint, controlMultPoint );
  convertEachPoint( Linestring2D( { p1, p2, p3 } ), line,
    [h] ( const Point2D& p ) { return toSpatial( p, h ); } );
  ASSERT_EQ( line, controlLine );
  convertEachPoint( std::vector<Point2D>( { p1, p2, p3 } ), vect,
    [h] ( const Point2D& p ) { return toSpatial( p, h ); } );
  ASSERT_EQ( vect, controlVect );
  convertEachPoint( MultiLinestring2D( { { p1, p2 },{ p1, p3 } } ), multLine,
    [h] ( const Point2D& p ) { return toSpatial( p, h ); } );
  ASSERT_EQ( multLine, controlMultLine );
  Ring2D flatRing( { p1, p2, p3, p1 } );
  convertEachPoint( flatRing, ring,
    [h] ( const Point2D& p ) { return toSpatial( p, h ); } );
  ASSERT_EQ( ring, controlRing );
  Polygon2D flatPoly( { flatRing,{ p1 },{ p2 } } );
  convertEachPoint( flatPoly, poly,
    [h] ( const Point2D& p ) { return toSpatial( p, h ); } );
  ASSERT_EQ( poly, controlPoly );
  convertEachPoint( MultiPolygon2D( { flatPoly,{ flatRing,{ p1 } } } ), multPoly,
    [h] ( const Point2D& p ) { return toSpatial( p, h ); } );
  ASSERT_EQ( multPoly, controlMultPoly );
  convertEachPoint( Box2D( p1, p2 ), box,
    [h] ( const Point2D& p ) { return toSpatial( p, h ); } );
  ASSERT_EQ( box, controlBox );
}
//==============================================================================
} //! Tests
} //! AbstractOperations
//==============================================================================
