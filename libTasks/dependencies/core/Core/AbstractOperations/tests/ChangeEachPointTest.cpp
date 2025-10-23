#include <AbstractOperations/ChangeEachPoint.hpp>
//! PrimaryEntities
#include <PrimaryEntities/Equalities.hpp>
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
 *\brief Увеличение координат точки в два раза
 *\param[in|out] p - изменяемая точка
 */
void twice( Point2D& p )
{
  p.set( p.getX( ) * 2, p.getY() * 2);
}
//==============================================================================
TEST( ChangeEachPointTest, Twice )
{
  Point2D p1( 1., 2. );
  Point2D p2( 2., 3. );
  Point2D p3( 3., 5. );

  Point2D t1( 2., 4. );
  Point2D t2( 4., 6. );
  Point2D t3( 6., 10. );

  MultiPoint2D multPoint( { p1, p2, p3 } );
  Linestring2D line ( { p1, p2, p3 } );
  std::vector<Point2D> vect ( { p1, p2, p3 } );
  MultiLinestring2D multLine ( { { p1, p2 },{ p1, p3 } } );
  Ring2D ring ( { p1, p2, p3, p1 } );
  Polygon2D poly ( { ring,{ p1 },{ p2 } } );
  MultiPolygon2D multPoly ( { poly,{ ring,{ p1 } } } );
  Box2D box ( p1, p2 );

  MultiPoint2D controlMultPoint( { t1, t2, t3 } );
  Linestring2D controlLine( { t1, t2, t3 } );
  std::vector<Point2D> controlVect( { t1, t2, t3 } );
  MultiLinestring2D controlMultLine( { { t1, t2 },{ t1, t3 } } );
  Ring2D controlRing( { t1, t2, t3, t1 } );
  Polygon2D controlPoly( { controlRing,{ t1 },{ t2 } } );
  MultiPolygon2D controlMultPoly( { controlPoly,{ controlRing,{ t1 } } } );
  Box2D controlBox( t1, t2 );

  changeEachPoint( multPoint, twice );
  ASSERT_EQ( multPoint, controlMultPoint );
  changeEachPoint( line, twice );
  ASSERT_EQ( line, controlLine );
  changeEachPoint( vect, twice );
  ASSERT_EQ( vect, controlVect );
  changeEachPoint( multLine, twice );
  ASSERT_EQ( multLine, controlMultLine );
  changeEachPoint( ring, twice );
  ASSERT_EQ( ring, controlRing );
  changeEachPoint( poly, twice );
  ASSERT_EQ( poly, controlPoly );
  changeEachPoint( multPoly, twice );
  ASSERT_EQ( multPoly, controlMultPoly );
  changeEachPoint( box, twice );
  ASSERT_EQ( box, controlBox );
}
//==============================================================================
} //! Tests
} //! AbstractOperations
//==============================================================================
