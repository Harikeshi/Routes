#include <AbstractOperations/Algorithms/Distance.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
//! Тесты для проверки правильности работы функции расчёта расстояния
//! между геометриями
//==============================================================================
namespace AbstractOperations
{
namespace Tests
{
//==============================================================================
class DistanceTest : public ::testing::Test
{
//==============================================================================
protected: 
//==============================================================================
  void SetUp()
  {
    //! Точечные - Dotted
    point1 = Point2D(1., 1.);
    point2 = Point2D(3., 3.);
    multiPoints = { point1, point2 };

    //! Линейные - Linear
    line1 = { { 1., 0. },{ 1., 2. },{ 2., 2. } };
    line2 = { { 2., 1. },{ 3., 1. } };
    multiline = { line1, line2 };

    //! Площадные - Areal
    box = Box2D(Point2D(0., 0.), Point2D(2., 2.));
    ring = { { 3., 3. },{ 3., 5. },{ 5., 5. },{ 5., 3. },{ 3., 3. } };
    incorrectRing = { { 3., 3. },{ 5., 3. },{ 5., 5. },{ 3., 5. },{ 3., 3. } };
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

  //! Линейные - Linear
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
TEST_F(DistanceTest, DistanceBetweenGeometries)
{
  ASSERT_EQ(distanceBetweenGeometries(point1, point2), sqrt(8));
  ASSERT_EQ(distanceBetweenGeometries(point1, multiPoints), 0);

  ASSERT_EQ(distanceBetweenGeometries(line1.at(1), line1), 0);
  ASSERT_EQ(distanceBetweenGeometries(line1, line2), 1);
  ASSERT_EQ(distanceBetweenGeometries(line2, multiPoints), 1);
  ASSERT_EQ(distanceBetweenGeometries(line1, multiline), 0);

  ASSERT_EQ(distanceBetweenGeometries(point1, box), 0);
  ASSERT_EQ(distanceBetweenGeometries(point2, box), sqrt(2));

  ASSERT_EQ(distanceBetweenGeometries(point1, ring), sqrt(8));
  ASSERT_EQ(distanceBetweenGeometries(point2, ring), 0);

  ASSERT_EQ(distanceBetweenGeometries(point1, polygon), sqrt(8));
  ASSERT_EQ(distanceBetweenGeometries(point2, polygon), 0);

  ASSERT_EQ(distanceBetweenGeometries(point1, mpolygon), sqrt(8));
  ASSERT_EQ(distanceBetweenGeometries(point2, mpolygon), 0);

  ASSERT_EQ(distanceBetweenGeometries(line2, polygon), 2);

  //! Некорректная геометрия
  ASSERT_THROW(distanceBetweenGeometries(point1, incorrectRing),
               AbstractOperationsFailure);
  try
  {
    distanceBetweenGeometries(point1, incorrectRing);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IncorrectGeometry, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }

  ASSERT_THROW(distanceBetweenGeometries(point2, incorrectRing),
               AbstractOperationsFailure);
  try
  {
    distanceBetweenGeometries(point2, incorrectRing);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IncorrectGeometry, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }

  //! Если выполнить коррекцию геометрии, расстояние посчитается
  ASSERT_EQ(distanceBetweenGeometries(point1, incorrectRing, true, false),
            sqrt(8));
  ASSERT_EQ(distanceBetweenGeometries(point2, incorrectRing, true, false), 0);
}
//==============================================================================
} // ! Tests
} // ! AbstractOperations
