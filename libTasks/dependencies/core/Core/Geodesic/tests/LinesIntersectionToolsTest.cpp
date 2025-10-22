#include <Geodesic/Operations/LinesIntersectionTools.hpp>
#include <Geodesic/GeoLineTools/OrthodromyTools.hpp>
//! AbstractOperations
#include <AbstractOperations/LinesIntersectionTools.hpp>
//! Geometry
#include <Geometry/Operations/LinesIntersectionTools.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace AbstractOperations
{
namespace Tests
{
//==============================================================================
TEST( GeodTrajIntersectionTest, baseFunctionsTest )
{
  // различные точки
  GeoPoint2D first;
  GeoPoint2D second( Degree( 1 ), Degree( 2 ) );

  Geodesic::OrthodromyTools orth;

  auto line = orth.createOrthodromy( first, second );
  auto len = line.getLength( );
  Radian begAzi = line.getAzimuthAtStart( );
  Radian endAzi = line.getAzimuthAtFinish( );

  ASSERT_EQ( std::make_pair( begAzi, endAzi ), 
    IntersectorHelper<GeoPoint2D>::inclinations( first, second ) );
  ASSERT_EQ( std::tie( begAzi, endAzi, len ),
    IntersectorHelper<GeoPoint2D>::inclinationsAndDistance( first, second ) );

  // совпадающие точки
  ASSERT_EQ( 0, IntersectorHelper<GeoPoint2D>::distance( first, first ) );
  ASSERT_THROW( IntersectorHelper<GeoPoint2D>::inclinations( first, first ), 
    AbstractOperationsFailure );
  try
  {
    IntersectorHelper<GeoPoint2D>::inclinations( first, first );
  }
  catch( AbstractOperationsFailure& ex )
  {
    ASSERT_EQ( ex.getCode( ), LineThroughOnePoint );
    ASSERT_EQ( ex.getLib( ), abstrLibTitle );
  }
  ASSERT_THROW( 
    IntersectorHelper<GeoPoint2D>::inclinationsAndDistance( first, first ), 
    AbstractOperationsFailure );
  try
  {
    IntersectorHelper<GeoPoint2D>::inclinationsAndDistance( first, first );
  }
  catch( AbstractOperationsFailure& ex )
  {
    ASSERT_EQ( ex.getCode( ), LineThroughOnePoint );
    ASSERT_EQ( ex.getLib( ), abstrLibTitle );
  }
}
//==============================================================================
// определить точку по соотношению
TEST( GeodTrajIntersectionTest, portionsTest )
{
  // проверяем расстояние до сильно примерного значения, чтобы не лезть в геодезию
  Geodesic::OrthodromyTools orth;
  ASSERT_NEAR( 0, orth.distance(
    GeoPoint2D( Radian( 0.003 ), Radian( 0.03 ) ),
    IntersectorHelper<GeoPoint2D>::pointByPortion( GeoPoint2D( ),
      GeoPoint2D( Radian( 0.01 ), Radian( 0.1 ) ), 0.3 ) ), 50 );
  ASSERT_THROW( IntersectorHelper<GeoPoint2D>::pointByPortion(
    GeoPoint2D( ), GeoPoint2D( Degree( 10 ), Degree( 20 ) ), -1 ), AbstractOperationsFailure );
  try
  {
    IntersectorHelper<GeoPoint2D>::pointByPortion( 
      GeoPoint2D( ), GeoPoint2D( Degree( 10 ), Degree( 20 ) ), -1 );
  }
  catch( AbstractOperationsFailure & ex )
  {
    ASSERT_EQ( ex.getCode( ), PointNotInInterval );
    ASSERT_EQ( ex.getLib( ), abstrLibTitle );
  }
}
//==============================================================================
// проверяем, что с самого верха доступны варианты и с географической точкой, 
// и с плоскостью
TEST( GeodTrajIntersectionTest, integratedTest )
{
  Point2D p00;
  Point2D p01( 0, 1 );
  Point2D p10( 1, 0 );
  Point2D p11( 1, 1 );

  auto type = checkIntersection( p00, p11, p01, p10 );
  ASSERT_EQ( type, Intersection );

  GeoPoint2D gp00;
  GeoPoint2D gp01( Degree( 0 ), Degree( 1 ) );
  GeoPoint2D gp10( Degree( 1 ), Degree( 0 ) );
  GeoPoint2D gp11( Degree( 1 ), Degree( 1 ) );

  auto gtype = checkIntersection( gp00, gp11, gp01, gp10 );
  ASSERT_EQ( gtype, Intersection );
}
//==============================================================================
} //! Tests
} //! AbstractOperations