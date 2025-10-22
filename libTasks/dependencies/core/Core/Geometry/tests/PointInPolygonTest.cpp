#include <Geometry/Operations/PointInPolygon.hpp>

#include <Geometry/GeometryException.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
class PointInPolygonTest : public ::testing::Test
{
//==============================================================================
protected:
//==============================================================================
  void SetUp()
  {
    centerSquare = Point2D(1.5, 1.5);
    centerRectangle = Point2D(2, 1.5);
    centerP = Point2D(2.5, 2.1);
    centerP2 = Point2D(2.5, 1.6428571428571428);
    centerDoubleSquare = Point2D(2.5, 2.5);
    centerDoubleSquare2 = Point2D(2, 2);
    centerSquareWithBow = Point2D(3, 3);
  }
//==============================================================================
  void TearDown()
  {

  }
//==============================================================================
// Квадрат
  Polygon2D square{ { 
    Point2D(1, 1), Point2D(1, 2), Point2D(2, 2),
    Point2D(2, 1), Point2D(1, 1) } };
  Point2D centerSquare;

  // Ещё один квадрат
  Polygon2D otherSquare{ {
      Point2D(3, 3), Point2D(3, 4), Point2D(4, 4),
      Point2D(4, 3), Point2D(3, 3) } };

  // Прямоугольник
  Polygon2D rectangle{ { 
    Point2D(1, 1), Point2D(1, 2), Point2D(3, 2),
    Point2D(3, 1), Point2D(1, 1) } };
  Point2D centerRectangle;

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
  Point2D centerP2;

  // Полигон с вырезанным центром (квадрат в квадрате)
  Polygon2D doubleSquare{
    { Point2D(1, 1), Point2D(1, 4), Point2D(4, 4),
      Point2D(4, 1), Point2D(1, 1) },
    { Point2D(2, 2), Point2D(3, 2),  Point2D(3, 3),
      Point2D(2, 3), Point2D(2, 2) } };
  Point2D centerDoubleSquare;

  // Квадрат в квадрате, повёрнутые на 45 градусов
  Polygon2D doubleSquare2{
    { Point2D(2, 0), Point2D(0, 2), Point2D(2, 4),
      Point2D(4, 2), Point2D(2, 0) },
    { Point2D(2, 1), Point2D(3, 2), Point2D(2, 3),
      Point2D(1, 2), Point2D(2, 1) } };
  Point2D centerDoubleSquare2;

  // Полигон, с центром масс на ребре
  Polygon2D squareWithBow{
    { Point2D(1, 1), Point2D(1, 5), Point2D(5, 5),
      Point2D(5, 1), Point2D(1, 1) },
    { Point2D(3, 2), Point2D(3.5, 2.5), Point2D(3, 3),
      Point2D(3.5, 3.5), Point2D(3, 4), Point2D(2.5, 3.5),
      Point2D(3, 3), Point2D(2.5, 2.5), Point2D(3, 2) } };
  Point2D centerSquareWithBow;

  // Линия (замкнутая, как полигон; не корректна, для полигона - мало точек,
  // будет ошибка)
  Polygon2D line{ { Point2D(1, 1), Point2D(5, 1), Point2D(1, 1) } };
//==============================================================================
};
//==============================================================================
TEST_F(PointInPolygonTest, findCentroidTest)
{
  // Квадрат
  Point2D center = findCentroid(square);
  ASSERT_EQ(center, centerSquare);

  // Прямоугольник
  center = findCentroid(rectangle);
  ASSERT_EQ(center, centerRectangle);

  // Буква П - центр масс внутри
  center = findCentroid(p);
  ASSERT_EQ(center, centerP);

  // Буква П - центр масс снаружи
  center = findCentroid(p2);
  ASSERT_EQ(center, centerP2);

  // Полигон с вырезанным центром (квадрат в квадрате)
  center = findCentroid(doubleSquare);
  ASSERT_EQ(center, centerDoubleSquare);

  // Квадрат в квадрате, повёрнутые на 45 градусов
  center = findCentroid(doubleSquare2);
  ASSERT_EQ(center, centerDoubleSquare2);

  // Полигон, с центром масс на ребре
  center = findCentroid(squareWithBow);
  ASSERT_EQ(center, centerSquareWithBow);

  // Линия
  ASSERT_THROW(findCentroid(Polygon2D()), GeometryFailure);

  try
  {
  center = findCentroid(line);
  }
  catch (const GeometryFailure& exception)
  {
    ASSERT_EQ(ExceptionTools::TransitFailure, exception.getCode());
    ASSERT_EQ(geomLibTitle, exception.getLib());
    ASSERT_FALSE(
      std::string(exception.what()).find("Too few points")
      == std::string::npos);
  }

  // Пустая геометрия
  ASSERT_THROW(findCentroid(Polygon2D()), GeometryFailure);

  try
  {
    center = findCentroid(Polygon2D());
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
TEST_F(PointInPolygonTest, findNearestPointsTest)
{
  double nearestDist;
  std::vector<Point2D> nearestPoints;
  double eps = 0.0000005;

  // Буква П - центр масс снаружи
  findNearestPoints(p2, centerP2, nearestPoints, nearestDist);
  ASSERT_EQ(nearestPoints[0], Point2D(2.5, 2));
  ASSERT_NEAR(nearestDist, 0.357143, eps);

  // Полигон с вырезанным центром (квадрат в квадрате)
  findNearestPoints(doubleSquare, centerDoubleSquare, nearestPoints, nearestDist);
  ASSERT_EQ(nearestPoints[0], Point2D(2.5, 2));
  ASSERT_EQ(nearestPoints[1], Point2D(3, 2.5));
  ASSERT_EQ(nearestPoints[2], Point2D(2.5, 3));
  ASSERT_EQ(nearestPoints[3], Point2D(2, 2.5));
  ASSERT_NEAR(nearestDist, 0.5, eps);

  // Квадрат в квадрате, повёрнутые на 45 градусов
  findNearestPoints(doubleSquare2, centerDoubleSquare2, nearestPoints, nearestDist);
  ASSERT_EQ(nearestPoints[0], Point2D(2.5, 1.5));
  ASSERT_EQ(nearestPoints[1], Point2D(2.5, 2.5));
  ASSERT_EQ(nearestPoints[2], Point2D(1.5, 2.5));
  ASSERT_EQ(nearestPoints[3], Point2D(1.5, 1.5));
  ASSERT_NEAR(nearestDist, 0.707107, eps);

  // Нулевой полигон
  findNearestPoints(Polygon2D(), Point2D(), nearestPoints, nearestDist);
  ASSERT_TRUE(nearestPoints.empty());
  ASSERT_EQ(nearestDist, 0);
}
//==============================================================================
TEST_F(PointInPolygonTest, checkPointsConnectivityTest)
{
  Point2D firstPointInPoly(3.5, 3.5);
  Point2D secondPointInPoly(1.5, 1.5);
  Point2D pointOutOfPoly(4.5, 4.5);
  Point2D pointInHole(2.5, 2.5);
  Point2D pointOnBorder(1., 1.);

  // Оценка связности без учёта границ
  // Обе точки попадают в один полигон
  ASSERT_TRUE(checkPointsConnectivity(
    firstPointInPoly, secondPointInPoly, { doubleSquare }, false));
  // Одна из точек за пределами полигона
  ASSERT_FALSE(checkPointsConnectivity(
    firstPointInPoly, pointOutOfPoly, { doubleSquare }, false));
  // Одна из точек в дырке полигона
  ASSERT_FALSE(checkPointsConnectivity(
    firstPointInPoly, pointInHole, { doubleSquare }, false));
  // Точки в разных полигонах мультиполигона
  ASSERT_FALSE(checkPointsConnectivity(
    firstPointInPoly, secondPointInPoly, { square, otherSquare }, false));
  // Одна из точек попала на границу, связности не будет
  ASSERT_FALSE(checkPointsConnectivity(
    pointOnBorder, secondPointInPoly, { square, otherSquare }, false));

  // Оценка связности с учётом границ
  // Обе точки попадают в один полигон
  ASSERT_TRUE(checkPointsConnectivity(
    firstPointInPoly, secondPointInPoly, { doubleSquare }, true));
  // Одна из точек за пределами полигона
  ASSERT_FALSE(checkPointsConnectivity(
    firstPointInPoly, pointOutOfPoly, { doubleSquare }, true));
  // Одна из точек в дырке полигона
  ASSERT_FALSE(checkPointsConnectivity(
    firstPointInPoly, pointInHole, { doubleSquare }, true));
  // Точки в разных полигонах мультиполигона
  ASSERT_FALSE(checkPointsConnectivity(
    firstPointInPoly, secondPointInPoly, { square, otherSquare }, true));
  // Одна из точек попала на границу, точки связны
  ASSERT_TRUE(checkPointsConnectivity(
    pointOnBorder, secondPointInPoly, { square, otherSquare }, true));

  // При пустом полигоне - ошибка
  ASSERT_THROW(checkPointsConnectivity(
    pointOnBorder, secondPointInPoly, MultiPolygon2D(), true),
    GeometryFailure);

  try
  {
    bool connectivity = checkPointsConnectivity(
      pointOnBorder, secondPointInPoly, MultiPolygon2D(), true);
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