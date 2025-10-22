#include <PrimaryEntities/CircleLite.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace PrimaryEntities
{
namespace Tests
{
//==============================================================================
TEST( CircleLiteTest, BaseFunctionsTest )
{
  // окружность по умолчанию в классическом пространстве
  CircleLite <Point2D> circle;
  ASSERT_EQ( circle.center, Point2D( ) );
  ASSERT_EQ( circle.radius, 1. );

  // заданная окружность в классическом пространстве
  CircleLite<Point2D> otherCircle( { 1., 2. }, 3. );
  ASSERT_EQ( otherCircle.center, Point2D( 1, 2 ) );
  ASSERT_EQ( otherCircle.radius, 3. );

  // окружность по умолчанию на эллипсоиде
  CircleLite<GeoPoint2D> geoCircle;
  ASSERT_EQ( geoCircle.center, GeoPoint2D( ) );
  ASSERT_EQ( geoCircle.radius, 1 );

  // заданная точка в классическом пространстве
  CircleLite<GeoPoint2D> otherGeoCircle( { Degree( 1. ), Degree( 2. ) }, 3. );
  ASSERT_TRUE( BasicMath::isEqual(
    otherGeoCircle.center.getLatitude( ).toDegrees( ).get( ), 1. ) );
  ASSERT_TRUE( BasicMath::isEqual(
    otherGeoCircle.center.getLongitude( ).toDegrees( ).get( ), 2. ) );
  ASSERT_EQ( otherGeoCircle.radius, 3 );

  // псевдонимы распознаются и нормально компилируются
  // простое сравнение тоже работает
  circle = CircleLite2D( { 1., 2. }, 3. );
  ASSERT_TRUE( circle == otherCircle );
  geoCircle = GeoCircleLite2D( { Degree( 1. ), Degree( 2. ) }, 3. );
  ASSERT_TRUE( geoCircle == otherGeoCircle );
}
//==============================================================================
} //! Tests
} // ! PrimaryEntities
//==============================================================================
