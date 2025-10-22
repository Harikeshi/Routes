#include <Point/Point2D.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование Point2D
 *        (конструктор на оcнове двух значений в double)
 */
TEST( Point2DTest, ConstructorPoint2D )
{
  ASSERT_EQ( std::make_pair( 0., 0. ), Point2D( ).get( ) );
  ASSERT_EQ( std::make_pair( 0.1, 100. ), Point2D( 0.1, 100. ).get( ) );
  ASSERT_EQ( std::make_pair( -10., -0.1 ), Point2D( -10, -0.1 ).get( ) );
  ASSERT_EQ( std::make_pair( -0.5, 0.5 ), Point2D( -0.5, 0.5 ).get( ) );
}
//==============================================================================
TEST( Point2DTest, ConstructorCopied )
{
  Point2D point;
  ASSERT_EQ( point, Point2D( point ) );
  point = Point2D( 0.1, 100. );
  ASSERT_EQ( point, Point2D( point ) );
  point = Point2D( -10., -0.1 );
  ASSERT_EQ( point, Point2D( point ) );
  point = Point2D( -0.5, 0.5 );
  ASSERT_EQ( point, Point2D( point ) );
}
//==============================================================================
/*!
 * \brief Тестирование set и get
 *        (присвоение коордиант на основе double и получение)
 */
TEST( Point2DTest, Point2DGetSet )
{
  using namespace BasicMath;

  Point2D point;
  ASSERT_EQ( std::make_pair( 0., 0. ), point.get( ) );
  ASSERT_TRUE(isEqual(0., point.getX()));
  ASSERT_TRUE(isEqual(0., point.getY()));

  point.set( 0.1, 100. );
  ASSERT_EQ( std::make_pair( 0.1, 100. ), point.get( ) );
  ASSERT_TRUE(isEqual(0.1, point.getX()));
  ASSERT_TRUE(isEqual(100., point.getY()));

  point.set( -10., -0.1 );
  ASSERT_EQ( std::make_pair( -10., -0.1 ), point.get( ) );
  ASSERT_TRUE(isEqual(-10., point.getX()));
  ASSERT_TRUE(isEqual(-0.1, point.getY()));

  point.set( -0.5, 0.5 );
  ASSERT_EQ( std::make_pair( -0.5, 0.5 ), point.get( ) );
  ASSERT_TRUE(isEqual(-0.5, point.getX()));
  ASSERT_TRUE(isEqual(0.5, point.getY()));
}
//==============================================================================
/*!
 * \brief Тестирование operator ==
 *        (сравнение точек)
 */
TEST( Point2DTest, Point2DEqual )
{
  Point2D point;
  ASSERT_TRUE( point == Point2D( ) );
  point.set( 5., 5. );
  ASSERT_TRUE( point == Point2D( 5., 5. ) );
  point.set( -5., 5. );
  ASSERT_TRUE( point == Point2D( -5., 5. ) );
  point.set( 1024.25, -0.001 );
  ASSERT_TRUE( point == Point2D( 1024.25, -0.001 ) );
  ASSERT_FALSE( point == Point2D( ) );
}
//==============================================================================
/*!
 * \brief Тестирование operator !=
 *        Покоординатное сравнение двух точек на неравенство
 */
TEST(Point2DTest, Point2DNotEqual)
{
  Point2D point1;
  Point2D point2(0.1, -10.);
  ASSERT_TRUE(point1 != point2);

  point2.set(0.0000000000000001, 0.0000000000000001);
  ASSERT_FALSE(point1 != point2);
}
//==============================================================================
} //! Tests
//==============================================================================
