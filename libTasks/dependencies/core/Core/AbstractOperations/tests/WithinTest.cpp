#include <AbstractOperations/Algorithms/Within.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
//! Тесты для проверки правильности работы функции within
//==============================================================================
namespace AbstractOperations
{
namespace Tests
{
//==============================================================================
class WithinTest : public ::testing::Test
{
//==============================================================================
protected: 
//==============================================================================
  void SetUp()
  {
    //! Точечные - Dotted
    point1 = Point2D(1., 1.);
    point2 = Point2D(2., 2.);
    multiPoints = { point1, point2 };

    //! Линейный - Linear
    line1 = { { 1., 0. },{ 1., 2. },{ 2., 2. } };
    line2 = { { 2., 2. },{ 2., 3. },{ 3., 3. } };
    multiline = { line1, line2 };

    //! Площадные - Areal
    box = Box2D(Point2D(0., 0.), Point2D(2., 2.));
    ring = { { 1., 1. },{ 1., 3. },{ 3., 3. },{ 3., 1. },{ 1., 1. } };
    incorrectRing = { { 1., 1. },{ 3., 1. },{ 3., 1. },{ 3., 3. },{ 1., 3. } };
    polygon.outer() = ring;
  }
//==============================================================================
  void TearDown()
  {

  }
//==============================================================================
  //! Точечные - Dotted
  Point2D             point1;
  Point2D             point2;
  MultiPoint2D        multiPoints;

  //! Линейный - Linear
  Linestring2D      line1;
  Linestring2D      line2;
  MultiLinestring2D multiline;

  //! Площадные - Areal
  Box2D               box;
  Ring2D              ring;
  Ring2D              incorrectRing;
  Polygon2D           polygon;
  MultiPolygon2D      mpolygon;
};
//==============================================================================
TEST_F(WithinTest, Within)
{
  ASSERT_TRUE(within(point1, multiPoints));

  ASSERT_TRUE(within(line1.at(1), line1));
  ASSERT_FALSE(within(line1.at(0), line1)); //!< граничная точка

  ASSERT_TRUE(within(line1, multiline));

  ASSERT_TRUE(within(point1, box));
  ASSERT_TRUE(within(point2, ring));
  ASSERT_FALSE(within(point1, ring));

  //! Некорректная геометрия
  ASSERT_THROW(within(point2, incorrectRing), AbstractOperationsFailure);
  try
  {
    within(point2, incorrectRing);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IncorrectGeometry, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }
}
//==============================================================================
TEST_F(WithinTest, WithinWithCorrect)
{
  ASSERT_TRUE(within(point2, incorrectRing, true, false));
  ASSERT_FALSE(within(point1, incorrectRing, true, false));
}
//==============================================================================
} // ! Tests
} // ! AbstractOperations
