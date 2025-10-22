#include <Geometry/ComplicatedOperations/SmartMiddlePointInPolygon.hpp>

#include <Geometry/GeometryException.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Correct.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
class SmartMiddlePointInPolygonTest : public ::testing::Test
{
//==============================================================================
protected:
//==============================================================================
  void SetUp()
  {
    AbstractOperations::correct(squareWithBow); // исправляем бантик
  }
//==============================================================================
  void TearDown()
  { }
//==============================================================================
// Квадрат
  Polygon2D square{ { 
    Point2D(1, 1), Point2D(1, 2), Point2D(2, 2),
    Point2D(2, 1), Point2D(1, 1) } };

  // Прямоугольник
  Polygon2D rectangle{ { 
    Point2D(1, 1), Point2D(1, 2), Point2D(3, 2),
    Point2D(3, 1), Point2D(1, 1) } };

  // Буква П - центр масс внутри
  Polygon2D p{ {
    Point2D(1, 1), Point2D(1, 3), Point2D(4, 3),
    Point2D(4, 1), Point2D(3, 1), Point2D(3, 2),
    Point2D(2, 2), Point2D(2, 1), Point2D(1, 1) } };
  Point2D centerP;

  // Буква П - центр масс снаружи
  Polygon2D p2{ {
    Point2D(1, 0), Point2D(1, 3), Point2D(4, 3),
    Point2D(4, 0), Point2D(3, 0), Point2D(3, 2),
    Point2D(2, 2), Point2D(2, 0), Point2D(1, 0) } };

  // Полигон с вырезанным центром (квадрат в квадрате)
  Polygon2D doubleSquare{
    { Point2D(1, 1), Point2D(1, 4), Point2D(4, 4),
      Point2D(4, 1), Point2D(1, 1) },
    { Point2D(2, 2), Point2D(3, 2),  Point2D(3, 3),
      Point2D(2, 3), Point2D(2, 2) } };

  // Квадрат в квадрате, повёрнутые на 45 градусов
  Polygon2D doubleSquare2{
    { Point2D(2, 0), Point2D(0, 2), Point2D(2, 4),
      Point2D(4, 2), Point2D(2, 0) },
    { Point2D(2, 1), Point2D(3, 2), Point2D(2, 3),
      Point2D(1, 2), Point2D(2, 1) } };

  // Полигон, с центром масс на ребре
  Polygon2D squareWithBow{
    { Point2D(1, 1), Point2D(1, 5), Point2D(5, 5),
      Point2D(5, 1), Point2D(1, 1) },
    { Point2D(3, 2), Point2D(3.5, 2.5), Point2D(3, 3),
      Point2D(3.5, 3.5), Point2D(3, 4), Point2D(2.5, 3.5),
      Point2D(3, 3), Point2D(2.5, 2.5), Point2D(3, 2) } };
//==============================================================================
};
//==============================================================================
TEST_F(SmartMiddlePointInPolygonTest, setSmartMiddlePointTest)
{
  // Квадрат
  Point2D pointInPoly = setSmartMiddlePoint(square);
  ASSERT_EQ(pointInPoly, Point2D(1.5, 1.5));

  // Прямоугольник
  pointInPoly = setSmartMiddlePoint(rectangle);
  ASSERT_EQ(pointInPoly, Point2D(2, 1.5));

  // Буква П - центр масс внутри
  pointInPoly = setSmartMiddlePoint(p);
  ASSERT_EQ(pointInPoly, Point2D(2.5, 2.1));

  // Буква П - центр масс снаружи
  pointInPoly = setSmartMiddlePoint(p2);
  ASSERT_EQ(pointInPoly, Point2D(2.5, 2));

  // Полигон с вырезанным центром (квадрат в квадрате)
  pointInPoly = setSmartMiddlePoint(doubleSquare);
  ASSERT_EQ(pointInPoly, Point2D(2.5, 3));
  pointInPoly = setSmartMiddlePoint(doubleSquare, East);
  ASSERT_EQ(pointInPoly, Point2D(3, 2.5));
  pointInPoly = setSmartMiddlePoint(doubleSquare, South);
  ASSERT_EQ(pointInPoly, Point2D(2.5, 2));
  pointInPoly = setSmartMiddlePoint(doubleSquare, West);
  ASSERT_EQ(pointInPoly, Point2D(2, 2.5));

  // Квадрат в квадрате, повёрнутые на 45 градусов
  pointInPoly = setSmartMiddlePoint(doubleSquare2);
  ASSERT_EQ(pointInPoly, Point2D(2.5, 2.5));
  pointInPoly = setSmartMiddlePoint(doubleSquare2, East);
  ASSERT_EQ(pointInPoly, Point2D(2.5, 2.5));
  pointInPoly = setSmartMiddlePoint(doubleSquare2, South);
  ASSERT_EQ(pointInPoly, Point2D(1.5, 1.5));
  pointInPoly = setSmartMiddlePoint(doubleSquare2, West);
  ASSERT_EQ(pointInPoly, Point2D(1.5, 2.5));

  // Полигон, с центром масс на ребре
  pointInPoly = setSmartMiddlePoint(squareWithBow);
  ASSERT_EQ(pointInPoly, Point2D(3, 3));

  ASSERT_THROW(setSmartMiddlePoint(Polygon2D()), GeometryFailure);
  try
  {
    pointInPoly = setSmartMiddlePoint(Polygon2D());
  }
  catch (const GeometryFailure& exception)
  {
    ASSERT_EQ(GeometryEnumFailure::EmptyPolygon, exception.getCode());
    ASSERT_EQ(geomLibTitle, exception.getLib());
    ASSERT_EQ(
      "[Core::Geometry] <EmptyPolygon>: Пустой полигон"
      , std::string(exception.what()));
  }
}
//==============================================================================
} // ! Tests
} // ! Geometry
//==============================================================================