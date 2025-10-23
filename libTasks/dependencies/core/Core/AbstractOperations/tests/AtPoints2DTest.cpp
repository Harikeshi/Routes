#include <AbstractOperations/AtPoints2D.hpp>
//! Exception
#include <AbstractOperations/AbstractOperationsException.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace AbstractOperations
{
namespace Tests
{
//==============================================================================
/*!
* \brief Тестирование проверки,
* что три точки образуют треугольник c ненулевой площадью.
*/
TEST( Point2DOperationsTest, IsNonDegenerateTriangleTest )
{
  //! Невырожденный треугольник.

  Point2D a( 0., 0. );
  Point2D b( -1., 1. );
  Point2D c( 1., 1. );

  ASSERT_TRUE( isNonDegenerateTriangle( a, b, c ) );

  //! Две из трёх точек совпадают.

  b = c;

  ASSERT_FALSE( isNonDegenerateTriangle( b, c, a ) );
  ASSERT_FALSE( isNonDegenerateTriangle( b, a, c ) );
  ASSERT_FALSE( isNonDegenerateTriangle( a, b, c ) );

  //! Три точки попарно различны, но лежат на одной прямой.

  b = std::move( c );
  c = Point2D( 2., 2. );

  ASSERT_FALSE( isNonDegenerateTriangle( b, a, c ) );
  ASSERT_FALSE( isNonDegenerateTriangle( a, b, c ) );
  ASSERT_FALSE( isNonDegenerateTriangle( a, c, b ) );
}
//==============================================================================
} // ! Tests
} // ! AbstractOperations
