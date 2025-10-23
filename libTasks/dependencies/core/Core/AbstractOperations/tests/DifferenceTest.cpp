#include <AbstractOperations/Algorithms/Difference.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
//! Тесты для проверки правильности работы функции вычитания (difference)
//==============================================================================
namespace AbstractOperations
{
namespace Tests
{
//==============================================================================
class DifferenceTest : public ::testing::Test
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
    incorrectRing = { { 1., 1. },{ 2., 2. },{ 2., 1. },{ 1., 2. },{ 1., 1. } };
    polygon.outer() = ring;

    polygonBox = { { { 0., 0. },{ 0., 2.1 },{ 2.1, 2.1 },{ 2.1, 0. },
                     { 0., 0. } } };
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
  Polygon2D           polygonBox;
  MultiPolygon2D      mpolygon;
};
//==============================================================================
TEST_F(DifferenceTest, Difference)
{
  //difference работает только с площадными геометриями
  ASSERT_THROW(difference(multiPoints, point2, multiPoints),
               AbstractOperationsFailure);
  try
  {
    difference(multiPoints, point2, multiPoints);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IrrelevantGeometryType, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }

  ASSERT_THROW(difference(multiline, line1, multiline),
               AbstractOperationsFailure);
  try
  {
    difference(multiline, line1, multiline);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IrrelevantGeometryType, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }

  ASSERT_THROW(difference(multiline, box, multiline),
               AbstractOperationsFailure);
  try
  {
    difference(multiline, box, multiline);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IrrelevantGeometryType, failure.getCode());
    ASSERT_EQ( abstrLibTitle, failure.getLib());
  }

  ASSERT_THROW(difference(multiline, polygonBox, multiline),
               AbstractOperationsFailure);
  try
  {
    difference(multiline, polygonBox, multiline);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IrrelevantGeometryType, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }

  difference(ring, box, mpolygon);
  ASSERT_EQ(1, mpolygon.size());
  ASSERT_EQ(7, mpolygon.at(0).outer().size());
  ASSERT_EQ(0, mpolygon.at(0).inners().size());
  ASSERT_EQ(Point2D(1., 3.), mpolygon.at(0).outer().at(0));
  ASSERT_EQ(Point2D(3., 3.), mpolygon.at(0).outer().at(1));
  ASSERT_EQ(Point2D(3., 1.), mpolygon.at(0).outer().at(2));
  ASSERT_EQ(Point2D(2., 1.), mpolygon.at(0).outer().at(3));
  ASSERT_EQ(Point2D(2., 2.), mpolygon.at(0).outer().at(4));
  ASSERT_EQ(Point2D(1., 2.), mpolygon.at(0).outer().at(5));
  ASSERT_EQ(Point2D(1., 3.), mpolygon.at(0).outer().at(6));

  difference(box, polygon, mpolygon);
  ASSERT_EQ(1, mpolygon.size());
  ASSERT_EQ(7, mpolygon.at(0).outer().size());
  ASSERT_EQ(0, mpolygon.at(0).inners().size());
  ASSERT_EQ(Point2D(0., 2.), mpolygon.at(0).outer().at(0));
  ASSERT_EQ(Point2D(1., 2.), mpolygon.at(0).outer().at(1));
  ASSERT_EQ(Point2D(1., 1.), mpolygon.at(0).outer().at(2));
  ASSERT_EQ(Point2D(2., 1.), mpolygon.at(0).outer().at(3));
  ASSERT_EQ(Point2D(2., 0.), mpolygon.at(0).outer().at(4));
  ASSERT_EQ(Point2D(0., 0.), mpolygon.at(0).outer().at(5));
  ASSERT_EQ(Point2D(0., 2.), mpolygon.at(0).outer().at(6));

  //! Некорректная геометрия
  ASSERT_THROW(difference(box, incorrectRing, mpolygon),
               AbstractOperationsFailure);
  try
  {
    difference(box, incorrectRing, mpolygon);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IncorrectGeometry, failure.getCode());
    ASSERT_EQ( abstrLibTitle, failure.getLib());
  }
}
//==============================================================================
TEST_F(DifferenceTest, DifferenceWithCorrect)
{
  ring = { { 1., 1. },{ 3., 1. },{ 3., 1. },{ 3., 3. },{ 1., 3. } };

  //! Вычитание площадных объектов
  difference(box, ring, mpolygon, true);
  ASSERT_EQ(1, mpolygon.size());
  ASSERT_EQ(7, mpolygon.at(0).outer().size());
  ASSERT_EQ(0, mpolygon.at(0).inners().size());
  ASSERT_EQ(Point2D(0., 2.), mpolygon.at(0).outer().at(0));
  ASSERT_EQ(Point2D(1., 2.), mpolygon.at(0).outer().at(1));
  ASSERT_EQ(Point2D(1., 1.), mpolygon.at(0).outer().at(2));
  ASSERT_EQ(Point2D(2., 1.), mpolygon.at(0).outer().at(3));
  ASSERT_EQ(Point2D(2., 0.), mpolygon.at(0).outer().at(4));
  ASSERT_EQ(Point2D(0., 0.), mpolygon.at(0).outer().at(5));
  ASSERT_EQ(Point2D(0., 2.), mpolygon.at(0).outer().at(6));
}
//==============================================================================
} // ! Tests
} // ! AbstractOperations
