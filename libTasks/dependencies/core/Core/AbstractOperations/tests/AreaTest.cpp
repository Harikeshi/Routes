#include <AbstractOperations/Algorithms/Area.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
//! Тесты для проверки правильности работы функции area
//==============================================================================
namespace AbstractOperations
{
namespace Tests
{
//==============================================================================
class AreaTest : public ::testing::Test
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
    mpolygon.push_back(polygon);
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
TEST_F(AreaTest, Area)
{
  // У не площадных геометрий площадь 0
  ASSERT_EQ(area(point1), 0);
  ASSERT_EQ(area(multiPoints), 0);

  ASSERT_EQ(area(line1), 0);
  ASSERT_EQ(area(multiline), 0);

  ASSERT_EQ(area(box), 4);
  ASSERT_EQ(area(ring), 4);
  ASSERT_EQ(area(polygon), 4);
  ASSERT_EQ(area(mpolygon), 4);

  //! Некорректная геометрия
  ASSERT_THROW(area(incorrectRing), AbstractOperationsFailure);
  try
  {
    area(incorrectRing);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IncorrectGeometry, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }

  //! Если выполнить коррекцию геометрии, площадь посчитается
  ASSERT_EQ(area(incorrectRing, true, false), 4);
}
//==============================================================================
} // ! Tests
} // ! AbstractOperations
