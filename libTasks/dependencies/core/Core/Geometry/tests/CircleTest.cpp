#include <Geometry/Objects/Circle.hpp>

#include <Geometry/GeometryException.hpp>
#include <Geometry/Objects/Line.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
using BasicMath::isEqual;
using BasicMath::isNull;
using namespace PrimaryEntities;
//==============================================================================
//==============================================================================
class CirclePrivate : public Circle
{
//==============================================================================
  FRIEND_TEST(CircleTest, Constructor);
//==============================================================================
public:
//==============================================================================
  inline
  CirclePrivate(
    const Point2D& center,
    double radius) noexcept : Circle(center, radius) {}
//==============================================================================
};
//==============================================================================
class CircleTest : public ::testing::Test
{
//==============================================================================
protected:
//==============================================================================
  void SetUp( )
  {
    pA.set( 3., 4. );
    pB.set( 4., 3. );
    pC.set( 0., 5. );
    pD.set( 5., 0. );
    pE.set( 0., 0. );
    r = 5;
  }
//==============================================================================
  void TearDown( )
  {
  }
//==============================================================================
  
  Point2D pA; // (3, 4)
  Point2D pB; // (4, 3)
  Point2D pC; // (0, 5)
  Point2D pD; // (5, 0)
  Point2D pE; // (0, 0)
  double r;
};
bool isEq( const Line& firstLine, const Line& secondLine )
{
  return firstLine.isSameTo(secondLine);
}
//==============================================================================
/*!
 * \brief Тестирование конструктора.
 */
TEST_F(CircleTest, Constructor)
{
  Point2D       expectedCenter(4., 4.);
  double        expectedRadius(37.);
  CirclePrivate circle(expectedCenter, expectedRadius);
  ASSERT_EQ(expectedCenter, circle.mCenter);
  ASSERT_EQ(expectedRadius, circle.mRadius);
}
//==============================================================================
/*!
 * \brief Тестирование геттеров.
 */
TEST_F(CircleTest, Getters)
{
  Point2D expectedCenter(12., 81.);
  double  expectedRadius(64.1146);
  Circle  circle(expectedCenter, expectedRadius);
  ASSERT_EQ(expectedCenter, circle.center());
  ASSERT_EQ(expectedRadius, circle.radius());
}
//==============================================================================
/*!
 * \brief Тестирование сеттеров.
 */
TEST_F(CircleTest, Setters)
{
  Point2D expectedCenter(56.7255, 86.7831);
  double  expectedRadius(91.2624);
  Circle  circle(Point2D(0., 0.), 0.);
  circle.setCenter(expectedCenter);
  circle.setRadius(expectedRadius);
  ASSERT_EQ(expectedCenter, circle.center());
  ASSERT_EQ(expectedRadius, circle.radius());
}
//==============================================================================
//==============================================================================
TEST_F( CircleTest, BaseTest )
{
  // можно создать окружность по центру и радиусу
  Circle circle( pE, r );
  // можно получить ее параметры
  ASSERT_EQ( pE, circle.center( ) );
  ASSERT_TRUE( isEqual( r, circle.radius( ) ) );
  ASSERT_EQ( CircleLite2D(pE, r), circle.getCircle( ) );
  // можно что-то менять
  circle.setRadius( 2 * r );
  ASSERT_TRUE( isEqual( 2 * r, circle.radius( ) ) );
  circle.setCenter( pA );
  ASSERT_EQ( pA, circle.center( ) );
  circle.setParams( pB, 3 * r );
  ASSERT_EQ( pB, circle.center( ) );
  ASSERT_TRUE( isEqual( 3 * r, circle.radius( ) ) );
  // отрицательный радиус не пройдет
  ASSERT_THROW( circle.setRadius( -1 ), GeometryFailure );
  try
  {
    circle.setRadius( -1 );
  }
  catch( const GeometryFailure& exception )
  {
    ASSERT_EQ( GeometryEnumFailure::NegativeRadius, exception.getCode( ) );
    ASSERT_EQ( geomLibTitle, exception.getLib( ) );
  }

  // можно строить окружность около полноценного треугольника
  Circle otherCircle (pA, pB, pC);
  ASSERT_EQ( pE, otherCircle.center( ) );
  ASSERT_TRUE( isEqual( r, otherCircle.radius( ) ) );
  // а около вырожденного нельзя
  ASSERT_THROW( otherCircle.buildNearTriangle( pC, Point2D(0, 7), pE ), GeometryFailure );
  try
  {
    otherCircle.buildNearTriangle( pC, pD, pE );
  }
  catch( const GeometryFailure& exception )
  {
    ASSERT_EQ( GeometryEnumFailure::CircumscribedCircleForBadTriangle, exception.getCode( ) );
    ASSERT_EQ( geomLibTitle, exception.getLib( ) );
  }
}
//==============================================================================
//==============================================================================
TEST_F( CircleTest, positionTest )
{
  Circle circle( pE, 5 );
  ASSERT_TRUE( circle.isInsideCircle( Point2D( 0, 4 ) ) );
  ASSERT_TRUE( circle.isInsideCircle( Point2D( 0, 5 ) ) );
  ASSERT_FALSE( circle.isInsideCircle( Point2D( 0, 5.1 ) ) );
  ASSERT_TRUE( circle.isInsideCircle( Point2D( 0, 5.1 ), 0.1 ) );

  ASSERT_TRUE( circle.isOnCircle( Point2D( 0, 5 ) ) );
  ASSERT_FALSE( circle.isOnCircle( Point2D( 0, 5.1 ) ) );
  ASSERT_TRUE( circle.isOnCircle( Point2D( 0, 5.1 ), 0.1 ) );
  ASSERT_TRUE( circle.isOnCircle( Point2D( 0, 4.9 ), 0.1 ) );
}
//==============================================================================
//==============================================================================
TEST_F( CircleTest, tangentTest )
{
  Circle circle( pE, r );

  // касательные через заданную точку

  // через внутренние точки касательной не получится
  ASSERT_THROW( circle.tangentPoint( Point2D( 0, 3 ), true ), GeometryFailure );
  try
  {
    circle.tangentPoint( Point2D( 0, 3 ), true );
  }
  catch( const GeometryFailure& exception )
  {
    ASSERT_EQ( GeometryEnumFailure::TangentThroughInsidePoint, exception.getCode( ) );
    ASSERT_EQ( geomLibTitle, exception.getLib( ) );
  }

  // через внешнюю точку - все хорошо
  auto p = circle.tangentPoint( Point2D( 5, 5 ), true );
  ASSERT_EQ( p, pD );
  Line l = circle.tangentLine( Point2D( 5, 5 ), true );
  ASSERT_TRUE( l.isVertical( ) );
  ASSERT_TRUE( isEq(l, Line(1, 0, -5)) );

  p = circle.tangentPoint( Point2D( 5, 5 ), false );
  ASSERT_EQ( p, pC );
  l = circle.tangentLine( Point2D( 5, 5 ), false );
  ASSERT_TRUE( isNull( l.getSlope() ) );
  ASSERT_TRUE( isEq(l, Line(0, -1, 5)) );

  // через точку на окружности - тоже нормально
  p = circle.tangentPoint( pD, true );
  ASSERT_EQ( p, pD );
  l = circle.tangentLine( pD, true );
  ASSERT_TRUE( l.isVertical( ) );
  ASSERT_TRUE( isEq(l, Line(1, 0, -5)) );
  // левая и правая касательные в этом случае совпадают
  p = circle.tangentPoint( pD, false );
  ASSERT_EQ( p, pD );
  l = circle.tangentLine( pD, false );
  ASSERT_TRUE( l.isVertical( ) );
  ASSERT_TRUE( isEq(l, Line(1, 0, -5)) );
  // касательные по заданному углу наклона

  // вертикальные касательные
  // направленные вдоль оси y
  Line tangent = circle.tangentLine(Radian(M_PI / 2.), true);
  ASSERT_TRUE( isEq(tangent, Line(1, 0, 5)) );
  ASSERT_TRUE(tangent.isForward());

  tangent = circle.tangentLine(Radian(M_PI / 2.), false);
  ASSERT_TRUE( isEq(tangent, Line(1, 0, -5)) );
  ASSERT_TRUE(tangent.isForward());

  // направленные против оси y
  tangent = circle.tangentLine(Radian (-M_PI / 2.), true);
  ASSERT_TRUE( isEq(tangent, Line(1, 0, -5)) );
  ASSERT_FALSE(tangent.isForward());

  tangent = circle.tangentLine(Radian (-M_PI / 2.), false);
  ASSERT_TRUE( isEq(tangent, Line(1, 0, 5)) );
  ASSERT_FALSE(tangent.isForward());

  // горизонтальные касательные
  // направленные вдоль оси x
  tangent = circle.tangentLine(Radian(0.), true);
  ASSERT_TRUE( isEq(tangent, Line(0, -1, 5)) );
  ASSERT_TRUE(tangent.isForward());

  tangent = circle.tangentLine(Radian(0.), false);
  ASSERT_TRUE( isEq(tangent, Line(0, -1, -5)) );
  ASSERT_TRUE(tangent.isForward());

  // направленные против оси x
  tangent = circle.tangentLine(Radian(M_PI), true);
  ASSERT_TRUE( isEq(tangent, Line(0, -1, -5)) );
  ASSERT_FALSE(tangent.isForward());

  tangent = circle.tangentLine(Radian(M_PI), false);
  ASSERT_TRUE( isEq(tangent, Line(0, -1, 5)) );
  ASSERT_FALSE(tangent.isForward());

  // угол наклона в первой четверти
  tangent = circle.tangentLine(Radian(M_PI / 4.), true);
  ASSERT_TRUE( isEq( tangent, Line( tan(M_PI / 4.), -1, 5. * sqrt(2.) ) ) );
  ASSERT_TRUE(tangent.isForward());

  tangent = circle.tangentLine(Radian(M_PI / 4.), false);
  ASSERT_TRUE( isEq( tangent, Line( tan(M_PI / 4.), -1, -5. * sqrt(2.) ) ) );
  ASSERT_TRUE(tangent.isForward());

  // угол наклона во второй четверти
  tangent = circle.tangentLine(Radian(3. * M_PI / 4.), true);
  ASSERT_TRUE( isEq( tangent, Line( tan(3. * M_PI / 4.), -1, -5. * sqrt(2.) ) ) );
  ASSERT_FALSE(tangent.isForward());

  tangent = circle.tangentLine(Radian(3. * M_PI / 4.), false);
  ASSERT_TRUE( isEq( tangent, Line( tan(3. * M_PI / 4.), -1, 5. * sqrt(2.) ) ) );
  ASSERT_FALSE(tangent.isForward());

  // угол наклона в третьей четверти
  tangent = circle.tangentLine(Radian(-3. * M_PI / 4.), true);
  ASSERT_TRUE( isEq( tangent, Line( tan(-3. * M_PI / 4.), -1, -5. * sqrt(2.) ) ) );
  ASSERT_FALSE(tangent.isForward());

  tangent = circle.tangentLine(Radian(-3. * M_PI / 4.), false);
  ASSERT_TRUE( isEq( tangent, Line( tan(-3. * M_PI / 4.), -1, 5. * sqrt(2.) ) ) );
  ASSERT_FALSE(tangent.isForward());

  // угол наклона в четвёртой четверти
  tangent = circle.tangentLine(Radian(-M_PI / 4.), true);
  ASSERT_TRUE( isEq( tangent, Line( tan(-M_PI / 4.), -1, 5. * sqrt(2.) ) ) );
  ASSERT_TRUE(tangent.isForward());

  tangent = circle.tangentLine(Radian(-M_PI / 4.), false);
  ASSERT_TRUE( isEq( tangent, Line( tan(-M_PI / 4.), -1, -5. * sqrt(2.) ) ) );
  ASSERT_TRUE(tangent.isForward());

  // ненормализованный угол наклона, направление против оси y
  tangent = circle.tangentLine(Radian(3. * M_PI / 2.), true);
  ASSERT_TRUE( isEq(tangent, Line(1, 0, -5)) );
  ASSERT_FALSE(tangent.isForward());

  tangent = circle.tangentLine(Radian(3. * M_PI / 2.), false);
  ASSERT_TRUE( isEq(tangent, Line(1, 0, 5)) );
  ASSERT_FALSE(tangent.isForward());
}
//==============================================================================
//==============================================================================
} //! Tests
} //! Geometry
//==============================================================================
