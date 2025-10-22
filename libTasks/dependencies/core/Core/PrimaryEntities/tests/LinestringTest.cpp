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
TEST(LinestringTest, Linestring2DTest)
{
  // Создание пустой линии
  Linestring2D linestring;
  ASSERT_TRUE(linestring.empty());

  // Создание линии через вектор
  Point2D p1(1., 2.);
  Point2D p2(2., 3.);
  Point2D p3(3., 5.);
  std::vector<Point2D> vec({ p1, p2, p3 });

  Linestring2D linestring2(vec.begin(), vec.end());
  ASSERT_EQ(linestring2.size(), 3);

  ASSERT_EQ(linestring2.at(0), vec.at(0));
  ASSERT_EQ(linestring2.at(1), vec.at(1));
  ASSERT_EQ(linestring2.at(2), vec.at(2));

  // Создание линии через список инициализации
  Linestring2D linestring3({ p1, p2, p3 });
  ASSERT_EQ(linestring3.size(), 3);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), p2);
  ASSERT_EQ(linestring3.at(2), p3);

  // Добавление элемента в конец
  linestring3.push_back(Point2D(7., 4.));
  ASSERT_EQ(linestring3.size(), 4);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), p2);
  ASSERT_EQ(linestring3.at(2), p3);
  ASSERT_EQ(linestring3.at(3), Point2D(7., 4.));

  // Добавление элемента в середину
  linestring3.insert(linestring3.begin() + 2, Point2D(5., 5.));
  ASSERT_EQ(linestring3.size(), 5);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), p2);
  ASSERT_EQ(linestring3.at(2), Point2D(5., 5.));
  ASSERT_EQ(linestring3.at(3), p3);
  ASSERT_EQ(linestring3.at(4), Point2D(7., 4.));

  // Удаление элемента с конца
  linestring3.pop_back();
  ASSERT_EQ(linestring3.size(), 4);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), p2);
  ASSERT_EQ(linestring3.at(2), Point2D(5., 5.));
  ASSERT_EQ(linestring3.at(3), p3);

  // Удаление элемента с середины
  linestring3.erase(linestring3.begin() + 1);
  ASSERT_EQ(linestring3.size(), 3);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), Point2D(5., 5.));
  ASSERT_EQ(linestring3.at(2), p3);
}
//==============================================================================
TEST(LinestringTest, Linestring3DTest)
{
  // Создание пустой линии
  Linestring3D linestring;
  ASSERT_TRUE(linestring.empty());

  // Создание линии через вектор
  Point3D p1(1., 2., 3.);
  Point3D p2(2., 3., 8.);
  Point3D p3(3., 5., 0.);
  std::vector<Point3D> vec({ p1, p2, p3 });

  Linestring3D linestring2(vec.begin(), vec.end());
  ASSERT_EQ(linestring2.size(), 3);

  ASSERT_EQ(linestring2.at(0), vec.at(0));
  ASSERT_EQ(linestring2.at(1), vec.at(1));
  ASSERT_EQ(linestring2.at(2), vec.at(2));

  // Создание линии через список инициализации
  Linestring3D linestring3({ p1, p2, p3 });
  ASSERT_EQ(linestring3.size(), 3);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), p2);
  ASSERT_EQ(linestring3.at(2), p3);

  // Добавление элемента в конец
  linestring3.push_back(Point3D(7., 4., 7.));
  ASSERT_EQ(linestring3.size(), 4);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), p2);
  ASSERT_EQ(linestring3.at(2), p3);
  ASSERT_EQ(linestring3.at(3), Point3D(7., 4., 7.));

  // Добавление элемента в середину
  linestring3.insert(linestring3.begin() + 2, Point3D(5., 5., 4.));
  ASSERT_EQ(linestring3.size(), 5);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), p2);
  ASSERT_EQ(linestring3.at(2), Point3D(5., 5., 4.));
  ASSERT_EQ(linestring3.at(3), p3);
  ASSERT_EQ(linestring3.at(4), Point3D(7., 4., 7.));

  // Удаление элемента с конца
  linestring3.pop_back();
  ASSERT_EQ(linestring3.size(), 4);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), p2);
  ASSERT_EQ(linestring3.at(2), Point3D(5., 5., 4.));
  ASSERT_EQ(linestring3.at(3), p3);

  // Удаление элемента с середины
  linestring3.erase(linestring3.begin() + 1);
  ASSERT_EQ(linestring3.size(), 3);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), Point3D(5., 5., 4.));
  ASSERT_EQ(linestring3.at(2), p3);
}
//==============================================================================
TEST(LinestringTest, GeoLinestring2DTest)
{
  // Создание пустой линии
  GeoLinestring2D linestring;
  ASSERT_TRUE(linestring.empty());

  // Создание линии через вектор
  GeoPoint2D p1(Degree(1.), Degree(2.));
  GeoPoint2D p2(Degree(2.), Degree(3.));
  GeoPoint2D p3(Degree(3.), Degree(5.));
  std::vector<GeoPoint2D> vec({ p1, p2, p3 });

  GeoLinestring2D linestring2(vec.begin(), vec.end());
  ASSERT_EQ(linestring2.size(), 3);

  ASSERT_EQ(linestring2.at(0), vec.at(0));
  ASSERT_EQ(linestring2.at(1), vec.at(1));
  ASSERT_EQ(linestring2.at(2), vec.at(2));

  // Создание линии через список инициализации
  GeoLinestring2D linestring3({ p1, p2, p3 });
  ASSERT_EQ(linestring3.size(), 3);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), p2);
  ASSERT_EQ(linestring3.at(2), p3);

  // Добавление элемента в конец
  linestring3.push_back(GeoPoint2D(Degree(7.), Degree(4.)));
  ASSERT_EQ(linestring3.size(), 4);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), p2);
  ASSERT_EQ(linestring3.at(2), p3);
  ASSERT_EQ(linestring3.at(3), GeoPoint2D(Degree(7.), Degree(4.)));

  // Добавление элемента в середину
  linestring3.insert(linestring3.begin() + 2,
                     GeoPoint2D(Degree(5.), Degree(5.)));
  ASSERT_EQ(linestring3.size(), 5);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), p2);
  ASSERT_EQ(linestring3.at(2), GeoPoint2D(Degree(5.), Degree(5.)));
  ASSERT_EQ(linestring3.at(3), p3);
  ASSERT_EQ(linestring3.at(4), GeoPoint2D(Degree(7.), Degree(4.)));

  // Удаление элемента с конца
  linestring3.pop_back();
  ASSERT_EQ(linestring3.size(), 4);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), p2);
  ASSERT_EQ(linestring3.at(2), GeoPoint2D(Degree(5.), Degree(5.)));
  ASSERT_EQ(linestring3.at(3), p3);

  // Удаление элемента с середины
  linestring3.erase(linestring3.begin() + 1);
  ASSERT_EQ(linestring3.size(), 3);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), GeoPoint2D(Degree(5.), Degree(5.)));
  ASSERT_EQ(linestring3.at(2), p3);
}
//==============================================================================
TEST(LinestringTest, GeoLinestring3DTest)
{
  // Создание пустой линии
  GeoLinestring3D linestring;
  ASSERT_TRUE(linestring.empty());

  // Создание линии через вектор
  GeoPoint3D p1(Degree(1.), Degree(2.), 3.);
  GeoPoint3D p2(Degree(2.), Degree(3.), 8.);
  GeoPoint3D p3(Degree(3.), Degree(5.), 0.);
  std::vector<GeoPoint3D> vec({ p1, p2, p3 });

  GeoLinestring3D linestring2(vec.begin(), vec.end());
  ASSERT_EQ(linestring2.size(), 3);

  ASSERT_EQ(linestring2.at(0), vec.at(0));
  ASSERT_EQ(linestring2.at(1), vec.at(1));
  ASSERT_EQ(linestring2.at(2), vec.at(2));

  // Создание линии через список инициализации
  GeoLinestring3D linestring3({ p1, p2, p3 });
  ASSERT_EQ(linestring3.size(), 3);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), p2);
  ASSERT_EQ(linestring3.at(2), p3);

  // Добавление элемента в конец
  linestring3.push_back(GeoPoint3D(Degree(7.), Degree(4.), 7.));
  ASSERT_EQ(linestring3.size(), 4);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), p2);
  ASSERT_EQ(linestring3.at(2), p3);
  ASSERT_EQ(linestring3.at(3), GeoPoint3D(Degree(7.), Degree(4.), 7.));

  // Добавление элемента в середину
  linestring3.insert(linestring3.begin() + 2,
                     GeoPoint3D(Degree(5.), Degree(5.), 4.));
  ASSERT_EQ(linestring3.size(), 5);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), p2);
  ASSERT_EQ(linestring3.at(2), GeoPoint3D(Degree(5.), Degree(5.), 4.));
  ASSERT_EQ(linestring3.at(3), p3);
  ASSERT_EQ(linestring3.at(4), GeoPoint3D(Degree(7.), Degree(4.), 7.));

  // Удаление элемента с конца
  linestring3.pop_back();
  ASSERT_EQ(linestring3.size(), 4);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), p2);
  ASSERT_EQ(linestring3.at(2), GeoPoint3D(Degree(5.), Degree(5.), 4.));
  ASSERT_EQ(linestring3.at(3), p3);

  // Удаление элемента с середины
  linestring3.erase(linestring3.begin() + 1);
  ASSERT_EQ(linestring3.size(), 3);

  ASSERT_EQ(linestring3.at(0), p1);
  ASSERT_EQ(linestring3.at(1), GeoPoint3D(Degree(5.), Degree(5.), 4.));
  ASSERT_EQ(linestring3.at(2), p3);

}
//==============================================================================
} //! Tests
} // ! PrimaryEntities
//==============================================================================
