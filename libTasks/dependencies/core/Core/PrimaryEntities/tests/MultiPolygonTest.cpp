#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
#include <PrimaryEntities/GeometryConverter.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace PrimaryEntities
{
namespace Tests
{
//==============================================================================
TEST(MultiPolygonTest, MultiPolygon2DTest)
{
  // Создание пустого мультиполигона
  MultiPolygon2D multiPolygon;
  ASSERT_TRUE(multiPolygon.empty());

  // Создание непустого мультиполигона
  
  // Первый полигон
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

  // Второй полигон
  // Точки внешнего кольца
  Point2D secondOuter1(6., 0.);
  Point2D secondOuter2(6., 1.);
  Point2D secondOuter3(7., 1.);
  Point2D secondOuter4(7., 0.);


  Ring2D outerRing1({ outer1, outer2, outer3, outer4, outer1 });
  Ring2D inner1({ firstInner1, firstInner2, firstInner3,
                  firstInner4, firstInner1 });
  Ring2D inner2({ secondInner1, secondInner2, secondInner3,
                  secondInner4, secondInner1 });

  Ring2D outerRing2({ secondOuter1, secondOuter2, secondOuter3,
                      secondOuter4, secondOuter1 });

  MultiPolygon2D multiPolygon2({ { outerRing1, inner1, inner2 },
                                 { outerRing2 } });

  ASSERT_EQ(multiPolygon2.size(), 2);
  ASSERT_EQ(multiPolygon2.at(0).outer().size(), 5);
  ASSERT_EQ(multiPolygon2.at(0).inners().size(), 2);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(0).size(), 5);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(1).size(), 5);

  ASSERT_EQ(multiPolygon2.at(0).outer().at(0), outer1);
  ASSERT_EQ(multiPolygon2.at(0).outer().at(1), outer2);
  ASSERT_EQ(multiPolygon2.at(0).outer().at(2), outer3);
  ASSERT_EQ(multiPolygon2.at(0).outer().at(3), outer4);
  ASSERT_EQ(multiPolygon2.at(0).outer().at(4), outer1);

  ASSERT_EQ(multiPolygon2.at(0).inners().at(0).at(0), firstInner1);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(0).at(1), firstInner2);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(0).at(2), firstInner3);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(0).at(3), firstInner4);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(0).at(4), firstInner1);

  ASSERT_EQ(multiPolygon2.at(0).inners().at(1).at(0), secondInner1);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(1).at(1), secondInner2);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(1).at(2), secondInner3);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(1).at(3), secondInner4);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(1).at(4), secondInner1);

  ASSERT_EQ(multiPolygon2.at(1).outer().at(0), secondOuter1);
  ASSERT_EQ(multiPolygon2.at(1).outer().at(1), secondOuter2);
  ASSERT_EQ(multiPolygon2.at(1).outer().at(2), secondOuter3);
  ASSERT_EQ(multiPolygon2.at(1).outer().at(3), secondOuter4);
  ASSERT_EQ(multiPolygon2.at(1).outer().at(4), secondOuter1);
}
//==============================================================================
TEST(MultiPolygonTest, GeoMultiPolygon2DTest)
{
  // Создание пустого полигона
  GeoMultiPolygon2D multiPolygon;
  ASSERT_TRUE(multiPolygon.empty());

  // Создание непустого мультиполигона

  // Первый полигон
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

  // Второй полигон
  // Точки внешнего кольца
  GeoPoint2D secondOuter1(Degree(6.), Degree(0.));
  GeoPoint2D secondOuter2(Degree(6.), Degree(1.));
  GeoPoint2D secondOuter3(Degree(7.), Degree(1.));
  GeoPoint2D secondOuter4(Degree(7.), Degree(0.));


  GeoRing2D outerRing1({ outer1, outer2, outer3, outer4, outer1 });
  GeoRing2D inner1({ firstInner1, firstInner2, firstInner3,
                     firstInner4, firstInner1 });
  GeoRing2D inner2({ secondInner1, secondInner2, secondInner3,
                     secondInner4, secondInner1 });

  GeoRing2D outerRing2({ secondOuter1, secondOuter2, secondOuter3,
                         secondOuter4, secondOuter1 });

  GeoMultiPolygon2D multiPolygon2({ { outerRing1, inner1, inner2 },
                                    { outerRing2 } });

  ASSERT_EQ(multiPolygon2.size(), 2);
  ASSERT_EQ(multiPolygon2.at(0).outer().size(), 5);
  ASSERT_EQ(multiPolygon2.at(0).inners().size(), 2);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(0).size(), 5);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(1).size(), 5);

  ASSERT_EQ(multiPolygon2.at(0).outer().at(0), outer1);
  ASSERT_EQ(multiPolygon2.at(0).outer().at(1), outer2);
  ASSERT_EQ(multiPolygon2.at(0).outer().at(2), outer3);
  ASSERT_EQ(multiPolygon2.at(0).outer().at(3), outer4);
  ASSERT_EQ(multiPolygon2.at(0).outer().at(4), outer1);

  ASSERT_EQ(multiPolygon2.at(0).inners().at(0).at(0), firstInner1);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(0).at(1), firstInner2);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(0).at(2), firstInner3);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(0).at(3), firstInner4);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(0).at(4), firstInner1);

  ASSERT_EQ(multiPolygon2.at(0).inners().at(1).at(0), secondInner1);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(1).at(1), secondInner2);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(1).at(2), secondInner3);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(1).at(3), secondInner4);
  ASSERT_EQ(multiPolygon2.at(0).inners().at(1).at(4), secondInner1);

  ASSERT_EQ(multiPolygon2.at(1).outer().at(0), secondOuter1);
  ASSERT_EQ(multiPolygon2.at(1).outer().at(1), secondOuter2);
  ASSERT_EQ(multiPolygon2.at(1).outer().at(2), secondOuter3);
  ASSERT_EQ(multiPolygon2.at(1).outer().at(3), secondOuter4);
  ASSERT_EQ(multiPolygon2.at(1).outer().at(4), secondOuter1);
}
//==============================================================================
} //! Tests
} // ! PrimaryEntities
//==============================================================================
