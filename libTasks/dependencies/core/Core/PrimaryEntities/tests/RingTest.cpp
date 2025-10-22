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
TEST(RingTest, Ring2DTest)
{
  // Создание пустого кольца
  Ring2D ring;
  ASSERT_TRUE(ring.empty());

  // Создание кольца через вектор
  Point2D p1(1., 2.);
  Point2D p2(2., 3.);
  Point2D p3(3., 5.);
  std::vector<Point2D> vec({ p1, p2, p3, p1 });

  Ring2D ring2(vec.begin(), vec.end());
  ASSERT_EQ(ring2.size(), 4);

  ASSERT_EQ(ring2.at(0), vec.at(0));
  ASSERT_EQ(ring2.at(1), vec.at(1));
  ASSERT_EQ(ring2.at(2), vec.at(2));
  ASSERT_EQ(ring2.at(3), vec.at(3));

  // Создание линии через список инициализации
  Ring2D ring3({ p1, p2, p3, p1 });
  ASSERT_EQ(ring3.size(), 4);

  ASSERT_EQ(ring3.at(0), p1);
  ASSERT_EQ(ring3.at(1), p2);
  ASSERT_EQ(ring3.at(2), p3);
  ASSERT_EQ(ring3.at(3), p1);

  // Удаление элемента с конца
  ring3.pop_back();

  // Добавление элемента в конец
  ring3.push_back(Point2D(7., 4.));

  // Замыкание кольца
  ring3.push_back(p1);
  ASSERT_EQ(ring3.size(), 5);

  ASSERT_EQ(ring3.at(0), p1);
  ASSERT_EQ(ring3.at(1), p2);
  ASSERT_EQ(ring3.at(2), p3);
  ASSERT_EQ(ring3.at(3), Point2D(7., 4.));
  ASSERT_EQ(ring3.at(4), p1);

  // Добавление элемента в середину
  ring3.insert(ring3.begin() + 2, Point2D(5., 5.));
  ASSERT_EQ(ring3.size(), 6);

  ASSERT_EQ(ring3.at(0), p1);
  ASSERT_EQ(ring3.at(1), p2);
  ASSERT_EQ(ring3.at(2), Point2D(5., 5.));
  ASSERT_EQ(ring3.at(3), p3);
  ASSERT_EQ(ring3.at(4), Point2D(7., 4.));
  ASSERT_EQ(ring3.at(5), p1);

  // Удаление элемента с середины
  ring3.erase(ring3.begin() + 1);
  ASSERT_EQ(ring3.size(), 5);

  ASSERT_EQ(ring3.at(0), p1);
  ASSERT_EQ(ring3.at(1), Point2D(5., 5.));
  ASSERT_EQ(ring3.at(2), p3);
  ASSERT_EQ(ring3.at(3), Point2D(7., 4.));
  ASSERT_EQ(ring3.at(4), p1);
}
//==============================================================================
TEST(RingTest, GeoRing2DTest)
{
  // Создание пустой линии
  GeoRing2D ring;
  ASSERT_TRUE(ring.empty());

  // Создание линии через вектор
  GeoPoint2D p1(Degree(1.), Degree(2.));
  GeoPoint2D p2(Degree(2.), Degree(3.));
  GeoPoint2D p3(Degree(3.), Degree(5.));
  std::vector<GeoPoint2D> vec({ p1, p2, p3, p1 });

  GeoRing2D ring2(vec.begin(), vec.end());
  ASSERT_EQ(ring2.size(), 4);

  ASSERT_EQ(ring2.at(0), vec.at(0));
  ASSERT_EQ(ring2.at(1), vec.at(1));
  ASSERT_EQ(ring2.at(2), vec.at(2));
  ASSERT_EQ(ring2.at(3), vec.at(3));

  // Создание линии через список инициализации
  GeoRing2D ring3({ p1, p2, p3, p1 });
  ASSERT_EQ(ring3.size(), 4);

  ASSERT_EQ(ring3.at(0), p1);
  ASSERT_EQ(ring3.at(1), p2);
  ASSERT_EQ(ring3.at(2), p3);
  ASSERT_EQ(ring3.at(3), p1);

  // Удаление элемента с конца
  ring3.pop_back();

  // Добавление элемента в конец
  ring3.push_back(GeoPoint2D(Degree(7.), Degree(4.)));

  // Замыкание кольца
  ring3.push_back(p1);
  ASSERT_EQ(ring3.size(), 5);

  ASSERT_EQ(ring3.at(0), p1);
  ASSERT_EQ(ring3.at(1), p2);
  ASSERT_EQ(ring3.at(2), p3);
  ASSERT_EQ(ring3.at(3), GeoPoint2D(Degree(7.), Degree(4.)));
  ASSERT_EQ(ring3.at(4), p1);

  // Добавление элемента в середину
  ring3.insert(ring3.begin() + 2, GeoPoint2D(Degree(5.), Degree(5.)));
  ASSERT_EQ(ring3.size(), 6);

  ASSERT_EQ(ring3.at(0), p1);
  ASSERT_EQ(ring3.at(1), p2);
  ASSERT_EQ(ring3.at(2), GeoPoint2D(Degree(5.), Degree(5.)));
  ASSERT_EQ(ring3.at(3), p3);
  ASSERT_EQ(ring3.at(4), GeoPoint2D(Degree(7.), Degree(4.)));
  ASSERT_EQ(ring3.at(5), p1);

  // Удаление элемента с середины
  ring3.erase(ring3.begin() + 1);
  ASSERT_EQ(ring3.size(), 5);

  ASSERT_EQ(ring3.at(0), p1);
  ASSERT_EQ(ring3.at(1), GeoPoint2D(Degree(5.), Degree(5.)));
  ASSERT_EQ(ring3.at(2), p3);
  ASSERT_EQ(ring3.at(3), GeoPoint2D(Degree(7.), Degree(4.)));
  ASSERT_EQ(ring3.at(4), p1);
}
//==============================================================================
} //! Tests
} // ! PrimaryEntities
//==============================================================================
