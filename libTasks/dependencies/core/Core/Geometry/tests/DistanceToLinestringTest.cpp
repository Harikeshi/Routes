#include <Geometry/Operations/DistanceToLinestring.hpp>

#include <Geometry/Operations/AtPoints2D.hpp>
#include <Geometry/GeometryException.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
  TEST(DistanceToLinestringTest, distanceBetweenPointAndLinestring)
  {
    Point2D pointFirst(0., 0.);
    Point2D pointSecond(10., 20.);

    Linestring2D firstLinestring(
    { Point2D(1., -1.), Point2D(1., 1.), Point2D(2., 2.) });
    Linestring2D secondLinestring(
    { Point2D(9., 18.), Point2D(9., 19.), Point2D(8., 19.) });

    // Кратчайшее расстояние - до середины одного из отрезков ломаной
    ASSERT_EQ(
      distanceBetweenPointAndLinestring(pointFirst, firstLinestring), 1.);

    // Кратчайшее расстояние - до вершины ломаной
    ASSERT_EQ(
      distanceBetweenPointAndLinestring(pointSecond, secondLinestring),
      Geometry::distance(pointSecond, Point2D(9., 19.)));

    // Ещё один пример (просто отрезок)
    Linestring2D line({ Point2D(1., 1.), Point2D(3., 2.) });
    ASSERT_EQ(
      distanceBetweenPointAndLinestring(Point2D(2.5, 0.5), line),
      Geometry::distance(Point2D(2.5, 0.5), Point2D(2., 1.5)));

    // Попробуем какой-нибудь чуть более сложный пример
    Linestring2D twoPeaks(
    {
      Point2D(20., 10.), Point2D(30., 50.),
      Point2D(40., 10.), Point2D(50., 50.), Point2D(60., 10.)
    });

    // Кратчайшее расстояние достигается двумя путями
    ASSERT_EQ(
      distanceBetweenPointAndLinestring(Point2D(30., 0.), twoPeaks),
      Geometry::distance(Point2D(30., 0.), Point2D(20., 10.)));
    ASSERT_EQ(
      distanceBetweenPointAndLinestring(Point2D(30., 0.), twoPeaks),
      Geometry::distance(Point2D(30., 0.), Point2D(40., 10.)));

    // Если точка на ломаной линии
    ASSERT_EQ(
      distanceBetweenPointAndLinestring(Point2D(25., 30.), twoPeaks), 0.);

    // Если точка в вершине
    ASSERT_EQ(
      distanceBetweenPointAndLinestring(twoPeaks.at(0), twoPeaks), 0.);
    ASSERT_EQ(
      distanceBetweenPointAndLinestring(twoPeaks.at(1), twoPeaks), 0.);

    // Если ломаная самопересекающаяся, всё также корректно посчитается
    Linestring2D selfIntersectionLine(
    {
      Point2D(10., 10.), Point2D(30., 40.),
      Point2D(10., 40.), Point2D(30., 10.)
    });

    ASSERT_EQ(distanceBetweenPointAndLinestring(
      Point2D(20., 10.), selfIntersectionLine), 8.3205029433784379);

    // Если ломаная пуста, будет ошибка
    ASSERT_THROW(distanceBetweenPointAndLinestring(
      Point2D(20., 10.), Linestring2D()), GeometryFailure);

    try
    {
      double dist = distanceBetweenPointAndLinestring(
        Point2D(20., 10.), Linestring2D());
    }
    catch (const GeometryFailure& exception)
    {
      ASSERT_EQ(GeometryEnumFailure::EmptyLinestring, exception.getCode());
      ASSERT_EQ(geomLibTitle, exception.getLib());
      ASSERT_EQ(
        "[Core::Geometry] <EmptyLinestring>: Пустая ломаная"
        , std::string(exception.what()));
    }
  }
//==============================================================================
} // ! Tests
} // ! Geometry