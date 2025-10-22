#include <PrimaryEntities/WayPoint.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace PrimaryEntities
{
namespace Tests
{
//==============================================================================
TEST( WayPointTest, BaseFunctionsTest )
{
  // точка по умолчанию в классическом пространстве
  WayPoint<Point2D> point;
  ASSERT_EQ( point.t, 0 );
  ASSERT_EQ( point.h, 0 );
  ASSERT_EQ( point.coord, Point2D( ) );

  // заданная точка в классическом пространстве
  WayPoint<Point2D> otherPoint( 1., { 2., 3. }, 4. );
  ASSERT_EQ( otherPoint.t, 1 );
  ASSERT_EQ( otherPoint.h, 4 );
  ASSERT_EQ( otherPoint.coord, Point2D( 2, 3 ) );

  // точка по умолчанию на эллипсоиде
  WayPoint<GeoPoint2D> geoPoint;
  ASSERT_EQ( geoPoint.t, 0 );
  ASSERT_EQ( geoPoint.h, 0 );
  ASSERT_EQ( geoPoint.coord, GeoPoint2D( ) );

  // заданная точка на эллипсоиде
  WayPoint<GeoPoint2D> otherGeoPoint( 1., { Degree( 2. ), Degree( 3. ) }, 4. );
  ASSERT_EQ( otherGeoPoint.t, 1 );
  ASSERT_EQ( otherGeoPoint.h, 4 );
  ASSERT_TRUE( BasicMath::isEqual(
    otherGeoPoint.coord.getLatitude( ).toDegrees( ).get( ), 2. ) );
  ASSERT_TRUE( BasicMath::isEqual(
    otherGeoPoint.coord.getLongitude( ).toDegrees( ).get( ), 3. ) );

  // псевдонимы распознаются и нормально компилируются
  // простое сравнение тоже работает
  point = WayFlPoint( 1., { 2., 3. }, 4. );
  ASSERT_TRUE( point == otherPoint );
  geoPoint = WayGeoPoint( 1., { Degree( 2. ), Degree( 3. ) }, 4. );
  ASSERT_TRUE( geoPoint == otherGeoPoint );
}
//==============================================================================
} //! Tests
} // ! PrimaryEntities
//==============================================================================
