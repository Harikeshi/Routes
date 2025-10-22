#include <Geometry/Operations/CheckPointsInPolygon.hpp>
#include <Geometry/GeometryException.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
TEST(PointsInPolygon, CheckPointsInPolygonTest)
{
  std::vector<std::pair<Point2D, double>> points({
    { Point2D(2, 2), 0 }, // точка в области
    { Point2D(2.5, 2.5), 0.1 }, // площадная точка в области
    { Point2D(4, 4), 0 }, // точка вне области
    { Point2D(3, 3), 0.1 }, // площадная точка, частично попавшая в область
    { Point2D(1, 1), 0 }, // точка на границе области
    { Point2D(1, 1), 0.1 } // площадная точка на границе области
  });

  MultiPolygon2D poly;
  Polygon2D polyInner({ { Point2D(1, 1), Point2D(1, 3), Point2D(3, 3),
                          Point2D(3, 1), Point2D(1, 1) } });
  poly.emplace_back(polyInner);

  // тестирование на мультиполигоне
  auto result = checkPointsInPolygon(points, poly);
  ASSERT_EQ(result.at(0), true);
  ASSERT_EQ(result.at(1), true);
  ASSERT_EQ(result.at(2), false);
  ASSERT_EQ(result.at(3), false);
  ASSERT_EQ(result.at(4), true);
  ASSERT_EQ(result.at(5), false);

  // тестирование на полигоне
  result = checkPointsInPolygon(points, polyInner);
  ASSERT_EQ(result.at(0), true);
  ASSERT_EQ(result.at(1), true);
  ASSERT_EQ(result.at(2), false);
  ASSERT_EQ(result.at(3), false);
  ASSERT_EQ(result.at(4), true);
  ASSERT_EQ(result.at(5), false);

  // проверка для пустой геометрии
  result = checkPointsInPolygon(points, Polygon2D());
  ASSERT_EQ(result.at(0), false);
  ASSERT_EQ(result.at(1), false);
  ASSERT_EQ(result.at(2), false);
  ASSERT_EQ(result.at(3), false);
  ASSERT_EQ(result.at(4), false);
  ASSERT_EQ(result.at(5), false);

  // отрицательный радиус
  points.emplace_back(std::make_pair(Point2D(3, 3), -100));

  ASSERT_THROW(checkPointsInPolygon(points, polyInner), GeometryFailure);

  try
  {
    result = checkPointsInPolygon(points, polyInner);
  }
  catch (const GeometryFailure& exception)
  {
    ASSERT_EQ(NegativeRadius, exception.getCode());
    ASSERT_EQ(geomLibTitle, exception.getLib());
  }
}
//==============================================================================
} //! Tests
} //! Geometry
//==============================================================================