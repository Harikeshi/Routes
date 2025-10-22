#include <Geodesic/Operations/DistanceToLinestring.hpp>

#include <Geodesic/GeodesicException.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
  TEST(DistanceToLinestringTest, distanceBetweenPointAndLinestring)
  {
    GeoPoint2D pointFirst(Degree(0.), Degree(0.));
    GeoPoint2D pointSecond(Degree(0.), Degree(-1.));
    GeoPoint2D pointAtTop(Degree(1.), Degree(-1.));
    GeoPoint2D pointAtMiddleTop(Degree(1.), Degree(1.));

    GeoLinestring2D linestring(
    {
      GeoPoint2D(Degree(1.), Degree(-1.)),
      GeoPoint2D(Degree(1.), Degree(1.)),
      GeoPoint2D(Degree(2.),Degree(2.))
    });

    // Кратчайшее расстояние - где-то на одной из линий ломаной (проекцию
    // ищем по всем точкам)
    ASSERT_EQ(
      distanceBetweenPointAndLinestring(pointFirst, linestring, true, true),
      110585.83092279448);

    // Кратчайшее расстояние - где-то на одной из линий ломаной (проекцию
    // ищем по одной точке)
    ASSERT_EQ(
      distanceBetweenPointAndLinestring(pointFirst, linestring, true),
      110585.04784986719);

    // Кратчайшее расстояние - в вершине ломаной (проекцию ищем по всем точкам)
    ASSERT_EQ(
      distanceBetweenPointAndLinestring(pointSecond, linestring, true, true),
      110568.87820181661);

    // Кратчайшее расстояние - в вершине ломаной (проекцию ищем по одной точке)
    ASSERT_EQ(
      distanceBetweenPointAndLinestring(pointSecond, linestring, true),
      110568.77482455365);

    // Если точка в вершине
    ASSERT_EQ(
      distanceBetweenPointAndLinestring(pointAtTop, linestring, true), 0.);
    ASSERT_EQ(
      distanceBetweenPointAndLinestring(
        pointAtMiddleTop, linestring, true), 0.);

    // Если ломаная самопересекающаяся, всё также корректно посчитается
    GeoLinestring2D selfIntersectionLine
    ({
      GeoPoint2D(Degree(10.), Degree(10.)),
      GeoPoint2D(Degree(30.), Degree(40.)),
      GeoPoint2D(Degree(10.), Degree(40.)),
      GeoPoint2D(Degree(30.), Degree(10.))
    });

    // По ортодромии (проекция по одной точке)
    ASSERT_EQ(distanceBetweenPointAndLinestring(
      GeoPoint2D(Degree(20.), Degree(10.)), selfIntersectionLine, true),
      846510.31750478747);

    // По ортодромии (проекция по всем точкам)
    ASSERT_EQ(distanceBetweenPointAndLinestring(
      GeoPoint2D(Degree(20.), Degree(10.)), selfIntersectionLine, true, true),
      845092.98908543505);

    // По локсодромии (проекция по одной точке)
    ASSERT_EQ(distanceBetweenPointAndLinestring(
      GeoPoint2D(Degree(20.), Degree(10.)), selfIntersectionLine, false),
      892519.39322290546);

    // По локсодромии (проекция по всем точкам)
    ASSERT_EQ(distanceBetweenPointAndLinestring(
      GeoPoint2D(Degree(20.), Degree(10.)), selfIntersectionLine, false, true),
      892373.51632915810);

    // Если ломаная пуста, будет ошибка
    ASSERT_THROW(distanceBetweenPointAndLinestring(
      GeoPoint2D(Degree(20.), Degree(10.)), GeoLinestring2D(), true),
      GeodesicFailure);

    ASSERT_THROW(distanceBetweenPointAndLinestring(
                   GeoPoint2D(Degree(20.), Degree(10.)),
                   GeoLinestring2D(), true),
                 GeodesicFailure);
    try
    {
      double dist = distanceBetweenPointAndLinestring(
        GeoPoint2D(Degree(20.), Degree(10.)), GeoLinestring2D(), true);
    }
    catch (const GeodesicFailure& exception)
    {
      ASSERT_EQ(GeodesicEnumFailure::EmptyLinestring, exception.getCode());
      ASSERT_EQ(geodesicLibTitle, exception.getLib());
      ASSERT_EQ(
        "[Core::Geodesic] <EmptyLinestring>: Пустая ломаная"
        , std::string(exception.what()));
    }
  }
//==============================================================================
} // ! Tests
} // ! Geodesic