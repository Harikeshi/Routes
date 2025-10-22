#include <Geometry/Operations/LinesIntersectionTools.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace AbstractOperations
{
namespace Tests
{
//==============================================================================
TEST( GeomTrajIntersectionTest, baseFunctionsTest )
{
  // различные точки
  Point2D first;
  Point2D second( 4, 3 );

  double len = 5;
  Radian angle = Radian( atan( 0.75 ) );

  ASSERT_EQ( std::make_pair( angle, angle ), 
    IntersectorHelper<Point2D>::inclinations( first, second ) );
  ASSERT_EQ( std::tie( angle, angle, len ),
    IntersectorHelper<Point2D>::inclinationsAndDistance( first, second ) );

  // совпадающие точки
  ASSERT_THROW( IntersectorHelper<Point2D>::inclinations( first, first ), 
    AbstractOperationsFailure );
  try
  {
    IntersectorHelper<Point2D>::inclinations( first, first );
  }
  catch( AbstractOperationsFailure& ex )
  {
    ASSERT_EQ( ex.getCode( ), LineThroughOnePoint );
    ASSERT_EQ( ex.getLib( ), abstrLibTitle );
  }
  ASSERT_THROW( IntersectorHelper<Point2D>::inclinationsAndDistance( first, first ), 
    AbstractOperationsFailure );
  try
  {
    IntersectorHelper<Point2D>::inclinationsAndDistance( first, first );
  }
  catch( AbstractOperationsFailure& ex )
  {
    ASSERT_EQ( ex.getCode( ), LineThroughOnePoint );
    ASSERT_EQ( ex.getLib( ), abstrLibTitle );
  }
}
//==============================================================================
// определить точку по соотношению
TEST( GeomTrajIntersectionTest, portionsTest )
{
  double eps = ACCURACY_DOUBLE;

  ASSERT_EQ(
    Point2D( 3, 6 ), 
    IntersectorHelper<Point2D>::pointByPortion( Point2D( ), Point2D( 10, 20 ), 0.3 ) );
  ASSERT_THROW( 
    IntersectorHelper<Point2D>::pointByPortion( Point2D( ), Point2D( 10, 20 ), -1 ), 
    AbstractOperationsFailure );
  try
  {
    IntersectorHelper<Point2D>::pointByPortion( Point2D( ), Point2D( 10, 20 ), -1 );
  }
  catch( AbstractOperationsFailure & ex )
  {
    ASSERT_EQ( ex.getCode( ), PointNotInInterval );
    ASSERT_EQ( ex.getLib( ), abstrLibTitle );
  }
}
//==============================================================================
// поиск точки простого пересечения отрезков
TEST( GeomTrajIntersectionTest, findSingleIntersectionTest )
{
  std::vector<double> resPortions;
  Point2D resCrossPoint;

  std::pair<Point2D, Point2D> base 
    = std::make_pair<Point2D, Point2D>( Point2D( ), Point2D( 10, 0 ) );
  std::pair<Point2D, Point2D> other 
    = std::make_pair( Point2D( 1, 1 ), Point2D( 1, -1 ) );
  Point2D crossPoint ( 1, 0 );

  IntersectorHelper<Point2D>::findSingleIntersection(
    base.first, base.second, other.first, other.second,
    resCrossPoint );
  ASSERT_EQ( crossPoint, resCrossPoint );

  // искать пересечение у отрезков на не пересекающихся прямых не выйдет
  other = std::make_pair( Point2D( 0, 2 ), Point2D( 2, 2 ) );
  ASSERT_THROW( IntersectorHelper<Point2D>::findSingleIntersection(
    base.first, base.second, other.first, other.second,
    resCrossPoint ), AbstractOperationsFailure );
  try
  {
    IntersectorHelper<Point2D>::findSingleIntersection(
      base.first, base.second, other.first, other.second,
      resCrossPoint );
  }
  catch( AbstractOperationsFailure& ex )
  {
    ASSERT_EQ( ex.getCode( ), FakeIntersection );
    ASSERT_EQ( ex.getLib( ), abstrLibTitle );
  }
}
//==============================================================================
} //! Tests
} //! AbstractOperations