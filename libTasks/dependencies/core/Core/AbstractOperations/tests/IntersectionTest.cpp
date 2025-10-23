#include <AbstractOperations/Algorithms/Intersection.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
//! Тесты для проверки правильности работы функции пересечения (intersection)
//==============================================================================
namespace AbstractOperations
{
namespace Tests
{

//==============================================================================
class IntersectionTest : public ::testing::Test
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
    line1 = { { 1., 0. },{ 1., 2. } };
    line2 = { { 2., 3. },{ 3., 3. } };
    multiline = { line1, line2 };

    //! Площадные - Areal
    box = Box2D(Point2D(0., 0.), Point2D(2., 2.));
    ring = { { 1., 1. },{ 1., 3. },{ 3., 3. },{ 3., 1. },{ 1., 1. } };
    incorrectRing = { { 1., 1. },{ 2., 2. },{ 2., 1. },{ 1., 2. },{ 1., 1. } };
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
TEST_F(IntersectionTest, Intersection)
{
  //intersection работает только с площадными геометриями
  ASSERT_THROW(intersection(multiPoints, point2, multiPoints),
               AbstractOperationsFailure);
  try
  {
    intersection(multiPoints, point2, multiPoints);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IrrelevantGeometryType, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }

  ASSERT_THROW(intersection(line1, multiline, multiline),
               AbstractOperationsFailure);
  try
  {
    intersection(line1, multiline, multiline);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IrrelevantGeometryType, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }

  ASSERT_THROW(intersection(multiline, box, multiline),
               AbstractOperationsFailure);
  try
  {
    intersection(multiline, box, multiline);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IrrelevantGeometryType, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }

  line1 = { { 1., 0. },{ 1., 2. },{ 2., 2. } };
  line2 = { { 2., 3. },{ 3., 3. },{ 2., 2. } };
  Linestring2D linePoint = { { 2., 2. },{ 2., 2. } };
  multiline = { line1, line2 };

  ASSERT_THROW(intersection(line1, multiline, multiline),
               AbstractOperationsFailure);
  try
  {
    intersection(line1, multiline, multiline);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IrrelevantGeometryType, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }

  line1 = { { 0., 0. },{ 2., 2. } };
  line2 = { { 2., 0. },{ 0., 2. } };
  ASSERT_THROW(intersection(line1, line2, multiPoints),
               AbstractOperationsFailure);
  try
  {
    intersection(line1, line2, multiPoints);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IrrelevantGeometryType, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }

  intersection(ring, box, mpolygon);
  ASSERT_EQ(1, mpolygon.size());
  ASSERT_EQ(5, mpolygon.at(0).outer().size());
  ASSERT_EQ(0, mpolygon.at(0).inners().size());
  ASSERT_EQ(Point2D(1., 2.), mpolygon.at(0).outer().at(0));
  ASSERT_EQ(Point2D(2., 2.), mpolygon.at(0).outer().at(1));
  ASSERT_EQ(Point2D(2., 1.), mpolygon.at(0).outer().at(2));
  ASSERT_EQ(Point2D(1., 1.), mpolygon.at(0).outer().at(3));
  ASSERT_EQ(Point2D(1., 2.), mpolygon.at(0).outer().at(4));

  intersection(box, polygon, mpolygon);
  ASSERT_EQ(1, mpolygon.size());
  ASSERT_EQ(5, mpolygon.at(0).outer().size());
  ASSERT_EQ(0, mpolygon.at(0).inners().size());
  ASSERT_EQ(Point2D(2., 2.), mpolygon.at(0).outer().at(0));
  ASSERT_EQ(Point2D(2., 1.), mpolygon.at(0).outer().at(1));
  ASSERT_EQ(Point2D(1., 1.), mpolygon.at(0).outer().at(2));
  ASSERT_EQ(Point2D(1., 2.), mpolygon.at(0).outer().at(3));
  ASSERT_EQ(Point2D(2., 2.), mpolygon.at(0).outer().at(4));

  //! Некорректная геометрия
  ASSERT_THROW(intersection(box, incorrectRing, mpolygon),
               AbstractOperationsFailure);
  try
  {
    intersection(box, incorrectRing, mpolygon);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IncorrectGeometry, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }
}
//==============================================================================
TEST_F(IntersectionTest, IntersectionWithCorrect)
{
  ring = { { 1., 1. },{ 3., 1. },{ 3., 1. },{ 3., 3. },{ 1., 3. } };

  //! Пересечение площадных объектов
  intersection(box, ring, mpolygon, true);
  ASSERT_EQ(1, mpolygon.size());
  ASSERT_EQ(5, mpolygon.at(0).outer().size());
  ASSERT_EQ(0, mpolygon.at(0).inners().size());
  ASSERT_EQ(Point2D(2., 2.), mpolygon.at(0).outer().at(0));
  ASSERT_EQ(Point2D(2., 1.), mpolygon.at(0).outer().at(1));
  ASSERT_EQ(Point2D(1., 1.), mpolygon.at(0).outer().at(2));
  ASSERT_EQ(Point2D(1., 2.), mpolygon.at(0).outer().at(3));
  ASSERT_EQ(Point2D(2., 2.), mpolygon.at(0).outer().at(4));
}
//==============================================================================
TEST_F(IntersectionTest, HasIntersection)
{
  ASSERT_FALSE(hasIntersection(line1, line2));

  line1 = { { 0., 0. },{ 2., 2. } };
  line2 = { { 2., 0. },{ 0., 2. } };
  ASSERT_TRUE(hasIntersection(line1, line2));

  ASSERT_TRUE(hasIntersection(box, ring));

  ring = { { 1., 1. },{ 3., 1. },{ 3., 1. },{ 3., 3. },{ 1., 3. } };

  //! Некорректная геометрия
  ASSERT_THROW(hasIntersection(box, incorrectRing),
               AbstractOperationsFailure);
  try
  {
    hasIntersection(box, incorrectRing);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IncorrectGeometry, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }
}
//==============================================================================  
TEST_F(IntersectionTest, HasIntersectionWithCorrect)
{
  ring = { { 1., 1. },{ 3., 1. },{ 3., 1. },{ 3., 3. },{ 1., 3. } };

  ASSERT_TRUE(hasIntersection(box, ring, true));
}
//==============================================================================
} // ! Tests
} // ! AbstractOperations
