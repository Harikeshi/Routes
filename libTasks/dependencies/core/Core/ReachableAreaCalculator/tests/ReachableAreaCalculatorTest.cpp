#include <ReachableAreaCalculator.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Area.hpp>
//! IOTools
#include <IOTools/ShpIO/ShpIO.hpp>
//! Geometry
#include <Geometry/Operations/AtPoints2D.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace ReachableAreaCalculator
{
namespace Tests
{
//==============================================================================
class ReachableAreaPrivate : public ReachableArea
{
  friend class ReachableAreaCalculatorTest;
  FRIEND_TEST( ReachableAreaCalculatorTest, calculate );
};
//==============================================================================
class ReachableAreaCalculatorTest : public ::testing::Test
{
protected:
  //==============================================================================
  void SetUp( )
  {
    Polygon2D poly1;
    poly1.outer( ).emplace_back( Point2D( -1, 1 ) );
    poly1.outer( ).emplace_back( Point2D( -2, 1 ) );
    poly1.outer( ).emplace_back( Point2D( -2, 2 ) );
    poly1.outer( ).emplace_back( Point2D( -1, 2 ) );
    poly1.outer( ).emplace_back( Point2D( -1, 1 ) );

    Polygon2D poly2;
    poly2.outer( ).emplace_back( Point2D( 1, -1 ) );
    poly2.outer( ).emplace_back( Point2D( 2, -1 ) );
    poly2.outer( ).emplace_back( Point2D( 2, -2 ) );
    poly2.outer( ).emplace_back( Point2D( 1, -2 ) );
    poly2.outer( ).emplace_back( Point2D( 1, -1 ) );

    Polygon2D poly3;
    poly3.outer( ).emplace_back( Point2D( 0, -1 ) );
    poly3.outer( ).emplace_back( Point2D( -1, -1 ) );
    poly3.outer( ).emplace_back( Point2D( -1, 0 ) );
    poly3.outer( ).emplace_back( Point2D( 0, -1 ) );

    Polygon2D poly4;
    poly4.outer( ).emplace_back( Point2D( 0, -1.5 ) );
    poly4.outer( ).emplace_back( Point2D( -1.5, -1.5 ) );
    poly4.outer( ).emplace_back( Point2D( -1.5, 0 ) );
    poly4.outer( ).emplace_back( Point2D( 0, -1.5 ) );

    withTwoPolygons.addPolygon( poly1 );
    withTwoPolygons.addPolygon( poly2 );

    withSmallerTriangle.addPolygon( poly1 );
    withSmallerTriangle.addPolygon( poly2 );
    withSmallerTriangle.addPolygon( poly3 );

    withGreaterTriangler.addPolygon( poly1 );
    withGreaterTriangler.addPolygon( poly2 );
    withGreaterTriangler.addPolygon( poly4 );
  }
  //============================================================================
  void TearDown( )
  { }
  //============================================================================
  Environment withTwoPolygons;
  Environment withSmallerTriangle;
  Environment withGreaterTriangler;

  ReachableAreaResult res;
  ReachableArea calculator;

  double distance = 2.8;

  std::shared_ptr<ReachableAreaPrivate> reachableAreaPrivate;
};
//==============================================================================
TEST_F( ReachableAreaCalculatorTest, calculate )
{
  //! Пример с препятствиями в виде двух квадратов
  calculator.calculate( withTwoPolygons, Point2D( 0, 0 ), distance, res, 128 );

  // преобразование результата в полигон boost
  Polygon2D result;
  Ring2D outer;

  for( const auto& p : res.area.outer( ) )
    outer.push_back( Point2D( p.getX( ), p.getY( ) ) );

  result.outer( ) = outer;
  result.inners( ).clear( );

  for( size_t i = 0; i < res.area.inners( ).size( ); ++i )
  {
    Ring2D inner;

    for( const auto& p : res.area.inners( )[i] )
      inner.push_back( Point2D( p.getX( ), p.getY( ) ) );

    result.inners( ).push_back( inner );
  }

  AbstractOperations::correct( result );

  // проверяем корректность полигона
  ASSERT_TRUE(AbstractOperations::isCorrect( result ) );
  // проверяем площадь
  ASSERT_NEAR(AbstractOperations::area( result ), 22.2811, 0.0001 );

  // В ГЗДЦ должны содержаться все точки препятствий, попавшие в
  // область досягаемости, в данном случае их 6
  std::vector<std::pair<Point2D, bool>> barierPoints = {
    { Point2D( -2., 1. ), false},
    { Point2D( -1., 1. ), false },
    { Point2D( -1., 2. ), false },
    { Point2D( 2., -1. ), false },
    { Point2D( 1., -1. ), false },
    { Point2D( 1., -2. ), false } };

  for( auto point : result.outer( ) )
  {
    for( size_t i = 0; i < barierPoints.size( ); ++i )
    {
      if( point == barierPoints[i].first )
      {
        barierPoints[i].second = true;
        break;
      }
    }
  }

  ASSERT_TRUE( barierPoints[0].second );
  ASSERT_TRUE( barierPoints[1].second );
  ASSERT_TRUE( barierPoints[2].second );
  ASSERT_TRUE( barierPoints[3].second );
  ASSERT_TRUE( barierPoints[4].second );
  ASSERT_TRUE( barierPoints[5].second );

  //! Пример с препятствиями в виде двух квадратов и треугольника
  calculator.calculate(
    withSmallerTriangle, Point2D( 0, 0 ), distance, res, 128 );

  // преобразование результата в полигон boost
  result.clear( );
  outer.clear( );

  for( const auto& p : res.area.outer( ) )
    outer.push_back( Point2D( p.getX( ), p.getY( ) ) );

  result.outer( ) = outer;
  result.inners( ).clear( );

  for( size_t i = 0; i < res.area.inners( ).size( ); ++i )
  {
    Ring2D inner;

    for( const auto& p : res.area.inners( )[i] )
      inner.push_back( Point2D( p.getX( ), p.getY( ) ) );

    result.inners( ).push_back( inner );
  }

  AbstractOperations::correct( result );

  // проверяем корректность полигона
  ASSERT_TRUE(AbstractOperations::isCorrect( result ) );
  // проверяем площадь
  ASSERT_NEAR(AbstractOperations::area(result), 21.147767336541541, 0.0001 );

  // В ГЗДЦ должны содержаться все точки препятствий, попавшие в
  // область досягаемости, в данном случае их 6 внешних и 3 внутренние
  barierPoints.clear( );
  barierPoints = {
    { Point2D( -2., 1. ), false },
    { Point2D( -1., 1. ), false },
    { Point2D( -1., 2. ), false },
    { Point2D( 2., -1. ), false },
    { Point2D( 1., -1. ), false },
    { Point2D( 1., -2. ), false } };

  std::vector<std::pair<Point2D, bool>> innerBarierPoints = {
  { Point2D( -1., 0. ), false },
  { Point2D( -1., -1. ), false },
  { Point2D( 0., -1. ), false } };

  for( auto point : result.outer( ) )
  {
    for( size_t i = 0; i < barierPoints.size( ); ++i )
    {
      if( point == barierPoints[i].first )
      {
        barierPoints[i].second = true;
        break;
      }
    }
  }

  for( auto point : result.inners( )[0] )
  {
    for( size_t i = 0; i < innerBarierPoints.size( ); ++i )
    {
      if( point == innerBarierPoints[i].first )
      {
        innerBarierPoints[i].second = true;
        break;
      }
    }
  }

  ASSERT_TRUE( barierPoints[0].second );
  ASSERT_TRUE( barierPoints[1].second );
  ASSERT_TRUE( barierPoints[2].second );
  ASSERT_TRUE( barierPoints[3].second );
  ASSERT_TRUE( barierPoints[4].second );
  ASSERT_TRUE( barierPoints[5].second );

  ASSERT_TRUE( innerBarierPoints[0].second );
  ASSERT_TRUE( innerBarierPoints[1].second );
  ASSERT_TRUE( innerBarierPoints[2].second );

  //! Пример с препятствиями в виде двух квадратов и чуть более
  // крупного треугольника
  calculator.calculate(
    withGreaterTriangler, Point2D( 0, 0 ), distance, res, 128 );

  // преобразование результата в полигон boost
  result.clear( );
  outer.clear( );

  for( const auto& p : res.area.outer( ) )
    outer.push_back( Point2D( p.getX( ), p.getY( ) ) );

  result.outer( ) = outer;
  result.inners( ).clear( );

  for( size_t i = 0; i < res.area.inners( ).size( ); ++i )
  {
    Ring2D inner;

    for( const auto& p : res.area.inners( )[i] )
      inner.push_back( Point2D( p.getX( ), p.getY( ) ) );

    result.inners( ).push_back( inner );
  }

  AbstractOperations::correct( result );

  // проверяем корректность полигона
  ASSERT_TRUE(AbstractOperations::isCorrect( result ) );
  // проверяем площадь
  ASSERT_NEAR(AbstractOperations::area(result), 19.9047, 0.0001);

  // В ГЗДЦ должны содержаться все точки препятствий, попавшие в
  // область досягаемости, в данном случае их 8
  barierPoints.clear( );
  barierPoints = {
    { Point2D( -2., 1. ), false },
    { Point2D( -1., 1. ), false },
    { Point2D( -1., 2. ), false },
    { Point2D( 2., -1. ), false },
    { Point2D( 1., -1. ), false },
    { Point2D( 1., -2. ), false },
    { Point2D( 0., -1.5 ), false },
    { Point2D( -1.5, 0. ), false } };

  for( auto point : result.outer( ) )
  {
    for( size_t i = 0; i < barierPoints.size( ); ++i )
    {
      if( point == barierPoints[i].first )
      {
        barierPoints[i].second = true;
        break;
      }
    }
  }

  ASSERT_TRUE( barierPoints[0].second );
  ASSERT_TRUE( barierPoints[1].second );
  ASSERT_TRUE( barierPoints[2].second );
  ASSERT_TRUE( barierPoints[3].second );
  ASSERT_TRUE( barierPoints[4].second );
  ASSERT_TRUE( barierPoints[5].second );
  ASSERT_TRUE( barierPoints[6].second );
  ASSERT_TRUE( barierPoints[7].second );

  //! Проверим, как создаётся область достижимости в среде без препятствий.

  ASSERT_TRUE(calculator.calculate(
                       Environment( ), Point2D( 0, 0 ), distance, res, 128 ) );
  ASSERT_EQ( 129, res.area.outer( ).size( ) );
  ASSERT_TRUE( res.area.inners( ).empty( ) );
  ASSERT_EQ( res.area.outer( ).front( ), res.area.outer( ).back( ) );
  std::size_t resAreaSize( res.area.outer( ).size( ) );
  //! У нас должно получиться приближение окружности.
  ASSERT_NEAR(
    distance,
    Geometry::distance( Point2D( 0., 0. ), res.area.outer( )[0] ),
    ACCURACY_DOUBLE );
  ASSERT_NEAR(
    distance,
    Geometry::distance( Point2D( 0., 0. ), res.area.outer( )[1] ),
    ACCURACY_DOUBLE );
  ASSERT_FALSE( res.area.outer( )[0] == res.area.outer( )[1] );
  for( std::size_t i( 2 ); i < resAreaSize; ++i )
  {
    ASSERT_NEAR(
      distance,
      Geometry::distance( Point2D( 0., 0. ), res.area.outer( )[i] ),
      ACCURACY_DOUBLE );
    ASSERT_FALSE( res.area.outer( )[i] == res.area.outer( )[i - 1] );
    ASSERT_FALSE( res.area.outer( )[i] == res.area.outer( )[i - 2] );
  }
  ASSERT_NEAR(24.6200399, AbstractOperations::area(res.area), 0.01 );

  //! Баг, исправленный в октябре 2021-ого года.

  MultiPolygon2D let(
    importMultiPolygon2DFromShapeFile(
      "ReachableAreaCalculatorTestRes/RA-test-zero2-withoutInners.shp" ) );
  Environment badEnv;
  for( const Polygon2D& polygon : let )
    badEnv.addPolygon( polygon );

  ASSERT_NO_THROW(calculator.calculate(
                      badEnv, Point2D( 16051., -4408. ), 80000., res, 128U ) );

  ASSERT_TRUE(AbstractOperations::isCorrect( res.area ) );

  ASSERT_NEAR(AbstractOperations::area(res.area),
              9039314109.5332279, 0.0001);
  //! Проверим некоторые граничные точки сформированной области достижимости.

  barierPoints = {
    { Point2D( -55409.76340595554, -36366.98981434602 ), false },
    { Point2D( 27465.0596203803, 60999.7924177243 ), false },
    { Point2D( 59482.4491358940, 60941.2191101959 ), false },
    { Point2D( -53885.17700580628, -38777.28751300320 ), false },
    { Point2D( -14697.6485343853, -73437.4074975101 ), false },
    { Point2D( 20784.14959550054, -78320.9070273912 ), false },
    { Point2D( 66313.81242410043, 47426.47879216728 ), false },
    { Point2D( 29948.79134337610, 62638.06894197941 ), false } };

  for( auto& barrierPoint : barierPoints )
  {
    for( const auto& point : res.area.outer( ) )
    {
      if(Geometry::distance(point, barrierPoint.first) < .000001 )
      {
        barrierPoint.second = true;
        break;
      }
    }
  }

  for( const auto& barrierPoint : barierPoints )
    ASSERT_TRUE( barrierPoint.second );
}
//==============================================================================
} //! Tests
} //! ReachableAreaCalculator
//==============================================================================
