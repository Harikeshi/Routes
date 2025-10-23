#include <AbstractOperations/Algorithms/Simplify.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace AbstractOperations
{
namespace Tests
{
//==============================================================================
TEST(SimplifyTest, Flat)
{
  Polygon2D result;

  Polygon2D polygon;
  polygon.outer( ) = {
    Point2D( ), Point2D(0, 0.5), Point2D( 0,1 ), Point2D( 1,1 ), 
    Point2D( 1.05, 0.7 ), Point2D( 1,0 ), Point2D( )
  };

  // по умолчанию удаляется только точка, лежащая на отрезке
  simplify( polygon, result );
  ASSERT_EQ( result.outer( ).size( ), 6 );

  // разрешено также убирать выступы
  simplify( polygon, result, 0.1 );
  ASSERT_EQ( result.outer( ).size( ), 5 );

  // можно преобразовывать исходную геометрию
  simplify( polygon, 0.2 );
  ASSERT_EQ( polygon.outer( ).size( ), 5 );
}
//==============================================================================
TEST( SimplifyTest, Geo )
{
  GeoPolygon2D result;

  GeoPolygon2D polygon;
  polygon.outer( ) = {
    GeoPoint2D( ), 
    GeoPoint2D( Degree( 1 ),Degree( 0 ) ), 
    GeoPoint2D( Degree( 1.05 ), Degree( 0.7 ) ), 
    GeoPoint2D( Degree( 1 ),Degree( 1 ) ), 
    GeoPoint2D( Degree( 0 ),Degree( 1 ) ), 
    GeoPoint2D( Degree( 0 ), Degree( 0.5 ) ), 
    GeoPoint2D( )
  };

  // по умолчанию удаляется только точка, лежащая на отрезке
  simplify( polygon, result );
  ASSERT_EQ( result.outer( ).size( ), 6 );

  // разрешено также убирать выступы
  simplify( polygon, result, Degree( 0.1 ).toRadians( ).get( ) );
  ASSERT_EQ( result.outer( ).size( ), 5 );

  // можно преобразовывать исходную геометрию
  simplify( polygon, Degree( 0.2 ).toRadians( ).get( ) );
  ASSERT_EQ( polygon.outer( ).size( ), 5 );
}
//==============================================================================
TEST( SimplifyTest, Geo180 )
{
  GeoPolygon2D result;

  GeoPolygon2D polygon;
  polygon.outer( ) = {
    GeoPoint2D( Degree( 0 ),Degree( 179 ) ),
    GeoPoint2D( Degree( 1 ),Degree( 179 ) ),
    GeoPoint2D( Degree( 1.05 ), Degree( 179.7 ) ),
    GeoPoint2D( Degree( 1 ),Degree( -179 ) ),
    GeoPoint2D( Degree( 0 ),Degree( -179 ) ),
    GeoPoint2D( Degree( 0 ), Degree( 179.5 ) ),
    GeoPoint2D( Degree( 0 ),Degree( 179 ) )
  };

  // по умолчанию все само нормализуется
  simplify( polygon, Degree( 0.2 ).toRadians( ).get( ) );
  ASSERT_EQ( polygon.outer( ).size( ), 5 );

  // если отказаться, прилетит исключение
  ASSERT_THROW(
    simplify( polygon, Degree( 0.2 ).toRadians( ).get( ), false, false ),
    AbstractOperationsFailure );
  try
  {
    simplify( polygon, Degree( 0.2 ).toRadians( ).get( ), false, false );
  }
  catch( AbstractOperationsFailure& ex )
  {
    ASSERT_EQ( ex.getCode( ), IncorrectGeometry );
    ASSERT_NE( std::string( ex.what( ) ).find( "Wrong orientation" ),
               std::string::npos );
  }
}
//==============================================================================
} // ! Tests
} // ! AbstractOperations
