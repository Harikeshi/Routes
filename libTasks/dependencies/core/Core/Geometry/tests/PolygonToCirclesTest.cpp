#include <Geometry/Operations/PolygonToCircles.hpp>

#include <Geometry/GeometryException.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Area.hpp>
#include <AbstractOperations/Algorithms/Correct.hpp>
#include <AbstractOperations/Algorithms/CoveredBy.hpp>
#include <AbstractOperations/Algorithms/Union.hpp>
//! std
#include <ctime>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование построения полигонального приближения круга
 * (с помощью описанного многоугольника).
 */
TEST( PolygonToCirclesTest, ToPolygon )
{
  Circle    circle( Point2D( 0., 0. ), 30. );
  Polygon2D circleAsPolygon( std::move( toPolygon( circle, 10U ) ) );

  ASSERT_EQ( 11, circleAsPolygon.outer( ).size( ) );
  ASSERT_TRUE( AbstractOperations::isCorrect( circleAsPolygon ) );
  //! Проверяем 4 случайно выбранные точки.
  ASSERT_EQ(
    Point2D(
      18.541019662496845446137605030969,
      -25.519524250561197965446214911891 ),
    circleAsPolygon.outer( )[4] );
  ASSERT_EQ(
    Point2D(
      18.541019662496845446137605030969,
      25.519524250561197965446214911891 ),
    circleAsPolygon.outer( )[1] );
  ASSERT_EQ(
    Point2D( 0., 31.543866727148016361540145090873 ),
    circleAsPolygon.outer( )[0] );
  ASSERT_EQ(
    Point2D( 30., 9.7475908869871897846761423664542 ),
    circleAsPolygon.outer( )[2] );

  //! Круг с нулевым радиусом.
  circle.setRadius( 0. );
  circleAsPolygon = std::move( toPolygon( circle, 10U ) );

  ASSERT_EQ( 2, circleAsPolygon.outer( ).size( ) );
  ASSERT_EQ( Point2D( 0., 0. ), circleAsPolygon.outer( )[0] );
  ASSERT_EQ( Point2D( 0., 0. ), circleAsPolygon.outer( )[1] );
}
//==============================================================================
/*!
 * \brief Тестирование построения описанного около треугольника круга.
 */
TEST( PolygonToCirclesTest, CircumscribedCircle )
{
  Point2D a( 0., 0. );
  Point2D b( 0., 1. );
  Point2D c( 1., 0. );
  Circle  circle( a, b, c );

  ASSERT_EQ( Point2D( .5, .5 ), circle.center( ) );
  ASSERT_TRUE( BasicMath::isEqual( std::sqrt( 0.5 ), circle.radius( ) ) );

  //! Вершины треугольника расположены на одной прямой и не совпадают попарно.
  c = std::move( Point2D( 0., 2. ) );

  bool isFailureThrown( false );
  try
  {
    circle.buildNearTriangle( a, b, c );
  }
  catch( const Failure& ex )
  {
    ASSERT_EQ( geomLibTitle, ex.getLib( ) );
    ASSERT_EQ(
      GeometryEnumFailure::CircumscribedCircleForBadTriangle,
      ex.getCode( ) );
    isFailureThrown = true;
  }
  ASSERT_TRUE( isFailureThrown );

  //! Вершины треугольника попарно совпадают.
  circle.buildNearTriangle( a, a, a );

  ASSERT_EQ( a, circle.center( ) );
  ASSERT_TRUE( BasicMath::isEqual( 0., circle.radius( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование построения минимального круга, содержащего многоугольник.
 */
TEST( PolygonToCirclesTest, MinimalCoveringCircle )
{
  Polygon2D polygon;
  polygon.outer( ).reserve( 4 );
  polygon.outer( ).emplace_back( -1., 0. );
  polygon.outer( ).emplace_back( 0., std::sqrt( 3. ) );
  polygon.outer( ).emplace_back( Point2D( 1., 0. ) );
  polygon.outer( ).emplace_back( polygon.outer( )[0] );

  Circle circle( std::move( minimalCoveringCircle( polygon ) ) );

  ASSERT_EQ( Point2D( 0., std::sqrt( 1. / 3. ) ), circle.center( ) );
  ASSERT_TRUE( BasicMath::isEqual( 2. * std::sqrt( 1. / 3. ), circle.radius( ) ) );

  //! Пустой многоугольник.
  polygon.clear( );

  bool isFailureThrown( false );
  try
  {
    circle = std::move( minimalCoveringCircle( polygon ) );
  }
  catch( const Failure& ex )
  {
    ASSERT_EQ( geomLibTitle, ex.getLib( ) );
    ASSERT_EQ(
      GeometryEnumFailure::MinimalCircleCoveringEmptyPolygon,
      ex.getCode( ) );
    isFailureThrown = true;
  }
  ASSERT_TRUE( isFailureThrown );

  //! Многоугольник с одной вершиной.
  polygon.outer( ).emplace_back( 1., 0. );

  circle = std::move( minimalCoveringCircle( polygon ) );

  ASSERT_EQ( Point2D( 1., 0. ), circle.center( ) );
  ASSERT_TRUE( BasicMath::isEqual( 0., circle.radius( ) ) );

  //! Многоугольник с двумя вершинами.
  polygon.outer( ).emplace_back( 3., 0. );

  circle = std::move( minimalCoveringCircle( polygon ) );

  ASSERT_EQ( Point2D( 2., 0. ), circle.center( ) );
  ASSERT_TRUE( BasicMath::isEqual( 1., circle.radius( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование построения покрытия многоугольника кругами.
 *
 * \details Тестирование этой функции отличается от тестирования,
 * к которому мы привыкли, так как аналитическое решение этой задачи не найдено.
 * Мы будем просто проверять, что функция ведёт себя корректно.
 *
 * В дальнейшем рекомендуется пополнять тесты
 * после случаев падений и их исправлений.
 */
TEST( PolygonToCirclesTest, PolygonToCircles )
{
  //! Треугольник.

  Polygon2D polygon;
  polygon.outer( ).reserve( 4 );
  polygon.outer( ).emplace_back( -100000., 0. );
  polygon.outer( ).emplace_back( 0., 30000. );
  polygon.outer( ).emplace_back( 100000., 0. );
  polygon.outer( ).emplace_back( polygon.outer( )[0] );

  double accuracy( .7 );
  int start( std::clock( ) );
  std::vector<Circle> result( std::move( polygonToCircles( polygon, accuracy ) ) );
  int finish( std::clock( ) );
  int t( finish - start );
  std::cout
    << "Actual time of polygonToCircles processing Triangle (accuracy = "
    << accuracy
    << " ): "
    << t
    << " ms"
    << std::endl;

  MultiPolygon2D               circlesUnion;
  MultiPolygon2D               buffer;
  for( const Circle& circle : result )
  {
    ASSERT_EQ( 0U, static_cast< unsigned int >( circle.radius( ) ) % 1000U );
    AbstractOperations::unionOf( circlesUnion, toPolygon( circle ), buffer );
    circlesUnion = std::move( buffer );
  }
  std::cout << "(Count of circles: " << result.size( ) << ")." << std::endl;
  ASSERT_TRUE( AbstractOperations::coveredBy( polygon, circlesUnion ) );
  ASSERT_GE(
    AbstractOperations::area(polygon) / AbstractOperations::area(circlesUnion),
    accuracy );

  //! Ромб.

  polygon.outer( ).insert(
    polygon.outer( ).begin( ) + 3,
    std::move( Point2D( 0., -30000. ) ) );

  start = std::clock( );
  result = std::move( polygonToCircles( polygon, .7 ) );
  finish = std::clock( );
  t = finish - start;
  std::cout
    << "Actual time of polygonToCircles processing Rhombus (accuracy = "
    << accuracy
    << " ): "
    << t
    << " ms"
    << std::endl;

  circlesUnion.clear( );
  for( const Circle& circle : result )
  {
    ASSERT_EQ( 0U, static_cast< unsigned int >( circle.radius( ) ) % 1000U );
    AbstractOperations::unionOf( circlesUnion, toPolygon( circle ), buffer );
    circlesUnion = std::move( buffer );
  }
  std::cout << "(Count of circles: " << result.size( ) << ")." << std::endl;
  ASSERT_TRUE( AbstractOperations::coveredBy( polygon, circlesUnion ) );
  ASSERT_GE(
    AbstractOperations::area(polygon) / AbstractOperations::area(circlesUnion),
    accuracy );

  //! Случайная фигура.

  polygon.clear( );
  polygon.outer( ).reserve( 17 );
  polygon.outer( ).emplace_back( -91882., 14192. );
  polygon.outer( ).emplace_back( -78567., 24261. );
  polygon.outer( ).emplace_back( -47966., 40571. );
  polygon.outer( ).emplace_back( -20681., 44491. );
  polygon.outer( ).emplace_back( 59265., 56007. );
  polygon.outer( ).emplace_back( 31271., -24234. );
  polygon.outer( ).emplace_back( 49758., -37126. );
  polygon.outer( ).emplace_back( 99089., -11365. );
  polygon.outer( ).emplace_back( 69748., -69298. );
  polygon.outer( ).emplace_back( 53640., -98021. );
  polygon.outer( ).emplace_back( -83974., -90999. );
  polygon.outer( ).emplace_back( -95179., -65139. );
  polygon.outer( ).emplace_back( -87482., -51853. );
  polygon.outer( ).emplace_back( -45210., -51067. );
  polygon.outer( ).emplace_back( -93903., -1550. );
  polygon.outer( ).emplace_back( -59819., 1128. );
  polygon.outer( ).emplace_back( polygon.outer( )[0] );

  start = std::clock( );
  result = std::move( polygonToCircles( polygon, .7 ) );
  finish = std::clock( );
  t = finish - start;
  std::cout
    << "Actual time of polygonToCircles processing RandomPolygon (accuracy = "
    << accuracy
    << " ): "
    << t
    << " ms"
    << std::endl;

  circlesUnion.clear( );
  for( const Circle& circle : result )
  {
    ASSERT_EQ( 0U, static_cast< unsigned int >( circle.radius( ) ) % 1000U );
    AbstractOperations::unionOf( circlesUnion, toPolygon( circle ), buffer );
    circlesUnion = std::move( buffer );
  }
  std::cout << "(Count of circles: " << result.size( ) << ")." << std::endl;
  ASSERT_TRUE( AbstractOperations::coveredBy( polygon, circlesUnion ) );
  ASSERT_GE(
    AbstractOperations::area(polygon) / AbstractOperations::area(circlesUnion),
    accuracy );

  //! Звезда.

  polygon.clear( );
  polygon.outer( ).reserve( 9 );
  polygon.outer( ).emplace_back( -100000., 0. );
  polygon.outer( ).emplace_back( -20000., 20000. );
  polygon.outer( ).emplace_back( 0., 100000. );
  polygon.outer( ).emplace_back( 20000., 20000. );
  polygon.outer( ).emplace_back( 100000., 0. );
  polygon.outer( ).emplace_back( 20000., -20000. );
  polygon.outer( ).emplace_back( 0., -100000. );
  polygon.outer( ).emplace_back( -20000., -20000. );
  polygon.outer( ).emplace_back( polygon.outer( )[0] );

  start = std::clock( );
  result = std::move( polygonToCircles( polygon, .7 ) );
  finish = std::clock( );
  t = finish - start;
  std::cout
    << "Actual time of polygonToCircles processing Star (accuracy = "
    << accuracy
    << " ): "
    << t
    << " ms"
    << std::endl;

  circlesUnion.clear( );
  for( const Circle& circle : result )
  {
    ASSERT_EQ( 0U, static_cast< unsigned int >( circle.radius( ) ) % 1000U );
    AbstractOperations::unionOf( circlesUnion, toPolygon( circle ), buffer );
    circlesUnion = std::move( buffer );
  }
  std::cout << "(Count of circles: " << result.size( ) << ")." << std::endl;
  ASSERT_TRUE( AbstractOperations::coveredBy( polygon, circlesUnion ) );
  ASSERT_GE(
    AbstractOperations::area(polygon) / AbstractOperations::area(circlesUnion),
    accuracy );
}
//==============================================================================
/*!
* \brief Тестирование поиска максимального количества вершин для описанного
*        многоугольника.
*/
TEST(PolygonToCirclesTest, PointsCountForCircumscribedPolygon)
{
  // Квадрат
  double radius(1.);
  double side(2.);
  ASSERT_EQ(pointsCountForCircumscribedPolygon(radius, side), 4);

  // Если чуть увеличить сторону, количество сторон придётся уменьшить
  side = 3.;
  ASSERT_EQ(pointsCountForCircumscribedPolygon(radius, side), 3);

  // Если уменьшить сторону, их количество можно будет увеличить
  side = 1.;
  ASSERT_EQ(pointsCountForCircumscribedPolygon(radius, side), 6);

  // Если сторона очень большая, найти многоугольник не можем
  side = 10.;
  ASSERT_THROW(pointsCountForCircumscribedPolygon(radius, side),
               GeometryFailure);
  try
  {
    pointsCountForCircumscribedPolygon(radius, side);
  }
  catch (const Failure& ex)
  {
    ASSERT_EQ(geomLibTitle, ex.getLib());
    ASSERT_EQ(GeometryEnumFailure::PointsCountForPolygonNotValid,
              ex.getCode());
  }

  // Если задать нулевой радиус, многоугольника нет
  radius = 0.;
  ASSERT_THROW(pointsCountForCircumscribedPolygon(radius, side),
               GeometryFailure);
  try
  {
    pointsCountForCircumscribedPolygon(radius, side);
  }
  catch (const Failure& ex)
  {
    ASSERT_EQ(geomLibTitle, ex.getLib());
    ASSERT_EQ(GeometryEnumFailure::PointsCountForPolygonNotValid,
              ex.getCode());
  }

  // Если задать нулевую сторону, максимальное количество точек найти
  // не получится
  radius = 1.;
  side = 0.;
  ASSERT_THROW(pointsCountForCircumscribedPolygon(radius, side),
               GeometryFailure);
  try
  {
    pointsCountForCircumscribedPolygon(radius, side);
  }
  catch (const Failure& ex)
  {
    ASSERT_EQ(geomLibTitle, ex.getLib());
    ASSERT_EQ(GeometryEnumFailure::NoMaximumPointsCountForPolygon,
              ex.getCode());
  }

  // Отрицательный радиус
  radius = -5.;
  side = 2.;
  ASSERT_THROW(pointsCountForCircumscribedPolygon(radius, side),
               GeometryFailure);
  try
  {
    pointsCountForCircumscribedPolygon(radius, side);
  }
  catch (const Failure& ex)
  {
    ASSERT_EQ(geomLibTitle, ex.getLib());
    ASSERT_EQ(GeometryEnumFailure::NegativeRadius, ex.getCode());
  }

  // Отрицательная длина стороны
  radius = 1.;
  side = -5.;
  ASSERT_THROW(pointsCountForCircumscribedPolygon(radius, side),
               GeometryFailure);
  try
  {
    pointsCountForCircumscribedPolygon(radius, side);
  }
  catch (const Failure& ex)
  {
    ASSERT_EQ(geomLibTitle, ex.getLib());
    ASSERT_EQ(GeometryEnumFailure::NegativePolygonSideLength, ex.getCode());
  }
}
//==============================================================================
} //! Tests
} //! Geometry
//==============================================================================