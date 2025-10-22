#include <Geometry/Operations/AtPoints2D.hpp>

#include <Geometry/GeometryException.hpp>
//==============================================================================
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
/*!
* \brief Тестирование поиска угла  между векторами
*/
TEST(AtPoints2DTest, findAngleLine)
{
  Point2D ourPoint(1., 1.);
  Point2D point1(2., 1.);
  Point2D point2(1., 2.);

  ASSERT_TRUE(findAngle(point1, ourPoint, point2).get() - M_PI / 2
    < std::numeric_limits<double>::epsilon());

  point1 = Point2D(1., 2.);
  point2 = Point2D(2., 1.);

  ASSERT_TRUE(findAngle(point1, ourPoint, point2).get() - 3 * M_PI / 2
    < std::numeric_limits<double>::epsilon());

  point1 = Point2D(2., 1.);
  point2 = Point2D(0., 1.);

  ASSERT_TRUE(findAngle(point1, ourPoint, point2).get() - M_PI
    < std::numeric_limits<double>::epsilon());

  point1 = Point2D(2., 1.);
  point2 = Point2D(3., 1.);

  ASSERT_TRUE(findAngle(point1, ourPoint, point2).get()
    < std::numeric_limits<double>::epsilon());

  point1 = Point2D(2., 1.);
  point2 = Point2D(1., 1.);
  ASSERT_THROW(findAngle(point1, ourPoint, point2), GeometryFailure);
  try
  {
    findAngle(point1, ourPoint, point2);
  }
  catch (const GeometryFailure& exception)
  {
    ASSERT_EQ(GeometryEnumFailure::NullVectorIncorrectOperation, exception.getCode());
    ASSERT_EQ(geomLibTitle, exception.getLib());
    ASSERT_EQ(
      "[Core::Geometry] <NullVectorIncorrectOperation>: Попытка провести с нулевым вектором "
      "операцию, которая для него не определена"
      , std::string(exception.what()));
  }
}
//==============================================================================
TEST( AtPoints2DTest, baseFunctions )
{
  // различные точки
  Point2D first;
  Point2D second( 4, 3 );

  double len = 5;
  Radian angle = Radian( atan(0.75) );

  ASSERT_EQ( len, distance( first, second ) );

  // совпадающие точки
  ASSERT_EQ( 0, distance( first, first ) );
}
//==============================================================================
} // ! Tests
} // ! Geometry
