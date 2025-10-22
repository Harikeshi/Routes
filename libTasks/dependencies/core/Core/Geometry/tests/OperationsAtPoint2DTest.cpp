#include <Geometry/Operations/AtPoints2D.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
/*!
* \brief Тестирование нахождение дистанции между точками
*/
TEST( Point2DOperationsTest, distanceTest )
{
  using namespace BasicMath;
  using namespace Geometry;

  Point2D p1( 0., 0. );
  Point2D p2( 0., 2. );
  ASSERT_TRUE( isEqual( 2., distance( p1, p2 ) ) );

  p2 = Point2D( 0., 0. );
  ASSERT_TRUE( isEqual( 0., distance( p1, p2 ) ) );

  p1 = Point2D( 1., 1. );
  p2 = Point2D( 4., 5. );
  ASSERT_TRUE( isEqual( 5., distance( p1, p2 ) ) );
}
//==============================================================================
/*!
* \brief Тестирование cosAngle
*        Проверка нахождения косинуса угла
*/
TEST( Point2DOperationsTest, CosAngle )
{
  using namespace BasicMath;
  using namespace Geometry;

  Point2D p1( 6., 0. );
  Point2D p2( 0., 2. );
  Point2D p( 0., 0. );
  ASSERT_TRUE( isEqual( 0., cosAngle( p1, p, p2 ) ) );

  p1 = Point2D( 0., 8. );
  p2 = Point2D( 0., 2. );
  ASSERT_TRUE( isEqual( 1., cosAngle( p1, p, p2 ) ) );

  p1 = Point2D( 0., -4. );
  p2 = Point2D( 0., 2. );
  ASSERT_TRUE( isEqual( -1., cosAngle( p1, p, p2 ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование поиска середины отрезка,
 * проведённого между двумя точками.
 */
TEST( Point2DOperationsTest, Middle )
{
  Point2D a( 135.388446, 896.011693 );
  Point2D b( -39.926264, 279.892051 );
  ASSERT_EQ( Point2D( 47.731091, 587.951872 ), middle( a, b ) );
}
//==============================================================================
} //! Tests
} //! Geometry
//==============================================================================