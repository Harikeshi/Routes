#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
#include <PrimaryEntities/GeometryConverter.hpp>
#include <PrimaryEntities/PrimaryEntitiesExceptions.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace PrimaryEntities
{
namespace Tests
{
//==============================================================================
TEST(PolygonTest, Polygon2DTest)
{
  // Создание пустого полигона
  Polygon2D polygon;
  ASSERT_TRUE(polygon.outer().empty());
  ASSERT_TRUE(polygon.inners().empty());

  // Создание непустого полигона
  // Точки внешнего кольца
  Point2D outer1(0., 0.);
  Point2D outer2(0., 5.);
  Point2D outer3(5., 5.);
  Point2D outer4(5., 0.);

  // Точки первого внутреннего контура
  Point2D firstInner1(1., 1.);
  Point2D firstInner2(2., 1.);
  Point2D firstInner3(2., 2.);
  Point2D firstInner4(1., 2.);

  // Точки второго внутреннего контура
  Point2D secondInner1(3., 3.);
  Point2D secondInner2(4., 3.);
  Point2D secondInner3(4., 4.);
  Point2D secondInner4(3., 4.);

  Ring2D outerRing({ outer1, outer2, outer3, outer4, outer1 });
  Ring2D inner1({ firstInner1, firstInner2, firstInner3,
                  firstInner4, firstInner1 });
  Ring2D inner2({ secondInner1, secondInner2, secondInner3,
                  secondInner4, secondInner1 });

  Polygon2D polygon2({ outerRing, inner1, inner2 });
  ASSERT_EQ(polygon2.outer().size(), 5);
  ASSERT_EQ(polygon2.inners().size(), 2);
  ASSERT_EQ(polygon2.inners().at(0).size(), 5);
  ASSERT_EQ(polygon2.inners().at(1).size(), 5);

  ASSERT_EQ(polygon2.outer().at(0), outer1);
  ASSERT_EQ(polygon2.outer().at(1), outer2);
  ASSERT_EQ(polygon2.outer().at(2), outer3);
  ASSERT_EQ(polygon2.outer().at(3), outer4);

  ASSERT_EQ(polygon2.inners().at(0).at(0), firstInner1);
  ASSERT_EQ(polygon2.inners().at(0).at(1), firstInner2);
  ASSERT_EQ(polygon2.inners().at(0).at(2), firstInner3);
  ASSERT_EQ(polygon2.inners().at(0).at(3), firstInner4);

  ASSERT_EQ(polygon2.inners().at(1).at(0), secondInner1);
  ASSERT_EQ(polygon2.inners().at(1).at(1), secondInner2);
  ASSERT_EQ(polygon2.inners().at(1).at(2), secondInner3);
  ASSERT_EQ(polygon2.inners().at(1).at(3), secondInner4);
}
//==============================================================================
TEST(PolygonTest, GeoPolygon2DTest)
{
  // Создание пустого кольца
  GeoPolygon2D polygon;
  ASSERT_TRUE(polygon.outer().empty());
  ASSERT_TRUE(polygon.inners().empty());

  // Создание непустого полигона
  // Точки внешнего кольца
  GeoPoint2D outer1(Degree(0.), Degree(0.));
  GeoPoint2D outer2(Degree(0.), Degree(5.));
  GeoPoint2D outer3(Degree(5.), Degree(5.));
  GeoPoint2D outer4(Degree(5.), Degree(0.));

  // Точки первого внутреннего контура
  GeoPoint2D firstInner1(Degree(1.), Degree(1.));
  GeoPoint2D firstInner2(Degree(2.), Degree(1.));
  GeoPoint2D firstInner3(Degree(2.), Degree(2.));
  GeoPoint2D firstInner4(Degree(1.), Degree(2.));

  // Точки второго внутреннего контура
  GeoPoint2D secondInner1(Degree(3.), Degree(3.));
  GeoPoint2D secondInner2(Degree(4.), Degree(3.));
  GeoPoint2D secondInner3(Degree(4.), Degree(4.));
  GeoPoint2D secondInner4(Degree(3.), Degree(4.));

  GeoRing2D outerRing({ outer1, outer2, outer3, outer4, outer1 });
  GeoRing2D inner1({ firstInner1, firstInner2, firstInner3,
                     firstInner4, firstInner1 });
  GeoRing2D inner2({ secondInner1, secondInner2, secondInner3,
                     secondInner4, secondInner1 });

  GeoPolygon2D polygon2({ outerRing, inner1, inner2 });
  ASSERT_EQ(polygon2.outer().size(), 5);
  ASSERT_EQ(polygon2.inners().size(), 2);
  ASSERT_EQ(polygon2.inners().at(0).size(), 5);
  ASSERT_EQ(polygon2.inners().at(1).size(), 5);

  ASSERT_EQ(polygon2.outer().at(0), outer1);
  ASSERT_EQ(polygon2.outer().at(1), outer2);
  ASSERT_EQ(polygon2.outer().at(2), outer3);
  ASSERT_EQ(polygon2.outer().at(3), outer4);

  ASSERT_EQ(polygon2.inners().at(0).at(0), firstInner1);
  ASSERT_EQ(polygon2.inners().at(0).at(1), firstInner2);
  ASSERT_EQ(polygon2.inners().at(0).at(2), firstInner3);
  ASSERT_EQ(polygon2.inners().at(0).at(3), firstInner4);

  ASSERT_EQ(polygon2.inners().at(1).at(0), secondInner1);
  ASSERT_EQ(polygon2.inners().at(1).at(1), secondInner2);
  ASSERT_EQ(polygon2.inners().at(1).at(2), secondInner3);
  ASSERT_EQ(polygon2.inners().at(1).at(3), secondInner4);
}
//==============================================================================
} //! Tests
} // ! PrimaryEntities
//==============================================================================
