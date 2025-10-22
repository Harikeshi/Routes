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
TEST(MultiLinestringTest, MultiLinestring2DTest)
{
  // Создание пустой линии
  MultiLinestring2D multiLine;
  ASSERT_TRUE(multiLine.empty());

  // Создание линии через вектор
  Point2D p1(1., 2.);
  Point2D p2(2., 3.);
  Point2D p3(3., 5.);
  Point2D p4(8., 7.);
  Point2D p5(4., 6.);

  Linestring2D linestring1({ p1, p2 });
  Linestring2D linestring2({ p3, p4, p5 });
  Linestring2D linestring3;
  std::vector<Linestring2D> vec({ linestring1, linestring2, linestring3 });

  MultiLinestring2D multiLine2(vec.begin(), vec.end());
  ASSERT_EQ(multiLine2.size(), 3);

  ASSERT_EQ(multiLine2.at(0).size(), 2);
  ASSERT_EQ(multiLine2.at(0).at(0), p1);
  ASSERT_EQ(multiLine2.at(0).at(1), p2);

  ASSERT_EQ(multiLine2.at(1).size(), 3);
  ASSERT_EQ(multiLine2.at(1).at(0), p3);
  ASSERT_EQ(multiLine2.at(1).at(1), p4);
  ASSERT_EQ(multiLine2.at(1).at(2), p5);
 
  ASSERT_TRUE(multiLine2.at(2).empty());

  // Создание линии через список инициализации
  MultiLinestring2D multiLine3({ linestring1, linestring2, linestring3 });
  ASSERT_EQ(multiLine3.size(), 3);

  ASSERT_EQ(multiLine3.at(0).size(), 2);
  ASSERT_EQ(multiLine3.at(0).at(0), p1);
  ASSERT_EQ(multiLine3.at(0).at(1), p2);

  ASSERT_EQ(multiLine3.at(1).size(), 3);
  ASSERT_EQ(multiLine3.at(1).at(0), p3);
  ASSERT_EQ(multiLine3.at(1).at(1), p4);
  ASSERT_EQ(multiLine3.at(1).at(2), p5);

  ASSERT_TRUE(multiLine3.at(2).empty());

  // Добавление новой линии
  multiLine3.push_back(linestring1);
  ASSERT_EQ(multiLine3.size(), 4);

  ASSERT_EQ(multiLine3.at(3).size(), 2);
  ASSERT_EQ(multiLine3.at(3).at(0), p1);
  ASSERT_EQ(multiLine3.at(3).at(1), p2);

  // Добавление точки к линии
  multiLine3.at(1).push_back(p2);
  ASSERT_EQ(multiLine3.size(), 4);

  ASSERT_EQ(multiLine3.at(1).size(), 4);
  ASSERT_EQ(multiLine3.at(1).at(0), p3);
  ASSERT_EQ(multiLine3.at(1).at(1), p4);
  ASSERT_EQ(multiLine3.at(1).at(2), p5);
  ASSERT_EQ(multiLine3.at(1).at(3), p2);
  
  // Удаление линии
  multiLine3.erase(multiLine3.begin());
  ASSERT_EQ(multiLine3.size(), 3);

  ASSERT_EQ(multiLine3.at(0).size(), 4);
  ASSERT_EQ(multiLine3.at(0).at(0), p3);
  ASSERT_EQ(multiLine3.at(0).at(1), p4);
  ASSERT_EQ(multiLine3.at(0).at(2), p5);
  ASSERT_EQ(multiLine3.at(0).at(3), p2);

  ASSERT_TRUE(multiLine3.at(1).empty());

  ASSERT_EQ(multiLine3.at(2).size(), 2);
  ASSERT_EQ(multiLine3.at(2).at(0), p1);
  ASSERT_EQ(multiLine3.at(2).at(1), p2);

  // Удаление точки из линии
  multiLine3.at(0).pop_back();
  ASSERT_EQ(multiLine3.size(), 3);

  ASSERT_EQ(multiLine3.at(0).size(), 3);
  ASSERT_EQ(multiLine3.at(0).at(0), p3);
  ASSERT_EQ(multiLine3.at(0).at(1), p4);
  ASSERT_EQ(multiLine3.at(0).at(2), p5);
}
//==============================================================================
TEST(MultiLinestringTest, MultiLinestring3DTest)
{
// Создание пустой линии
MultiLinestring3D multiLine;
ASSERT_TRUE(multiLine.empty());

// Создание линии через вектор
Point3D p1(1., 2., 3.);
Point3D p2(2., 3., 8.);
Point3D p3(3., 5., 0.);
Point3D p4(8., 7., 5.);
Point3D p5(4., 6., 3.);

Linestring3D linestring1({ p1, p2 });
Linestring3D linestring2({ p3, p4, p5 });
Linestring3D linestring3;
std::vector<Linestring3D> vec({ linestring1, linestring2, linestring3 });

MultiLinestring3D multiLine2(vec.begin(), vec.end());
ASSERT_EQ(multiLine2.size(), 3);

ASSERT_EQ(multiLine2.at(0).size(), 2);
ASSERT_EQ(multiLine2.at(0).at(0), p1);
ASSERT_EQ(multiLine2.at(0).at(1), p2);

ASSERT_EQ(multiLine2.at(1).size(), 3);
ASSERT_EQ(multiLine2.at(1).at(0), p3);
ASSERT_EQ(multiLine2.at(1).at(1), p4);
ASSERT_EQ(multiLine2.at(1).at(2), p5);

ASSERT_TRUE(multiLine2.at(2).empty());

// Создание линии через список инициализации
MultiLinestring3D multiLine3({ linestring1, linestring2, linestring3 });
ASSERT_EQ(multiLine3.size(), 3);

ASSERT_EQ(multiLine3.at(0).size(), 2);
ASSERT_EQ(multiLine3.at(0).at(0), p1);
ASSERT_EQ(multiLine3.at(0).at(1), p2);

ASSERT_EQ(multiLine3.at(1).size(), 3);
ASSERT_EQ(multiLine3.at(1).at(0), p3);
ASSERT_EQ(multiLine3.at(1).at(1), p4);
ASSERT_EQ(multiLine3.at(1).at(2), p5);

ASSERT_TRUE(multiLine3.at(2).empty());

// Добавление новой линии
multiLine3.push_back(linestring1);
ASSERT_EQ(multiLine3.size(), 4);

ASSERT_EQ(multiLine3.at(3).size(), 2);
ASSERT_EQ(multiLine3.at(3).at(0), p1);
ASSERT_EQ(multiLine3.at(3).at(1), p2);

// Добавление точки к линии
multiLine3.at(1).push_back(p2);
ASSERT_EQ(multiLine3.size(), 4);

ASSERT_EQ(multiLine3.at(1).size(), 4);
ASSERT_EQ(multiLine3.at(1).at(0), p3);
ASSERT_EQ(multiLine3.at(1).at(1), p4);
ASSERT_EQ(multiLine3.at(1).at(2), p5);
ASSERT_EQ(multiLine3.at(1).at(3), p2);

// Удаление линии
multiLine3.erase(multiLine3.begin());
ASSERT_EQ(multiLine3.size(), 3);

ASSERT_EQ(multiLine3.at(0).size(), 4);
ASSERT_EQ(multiLine3.at(0).at(0), p3);
ASSERT_EQ(multiLine3.at(0).at(1), p4);
ASSERT_EQ(multiLine3.at(0).at(2), p5);
ASSERT_EQ(multiLine3.at(0).at(3), p2);

ASSERT_TRUE(multiLine3.at(1).empty());

ASSERT_EQ(multiLine3.at(2).size(), 2);
ASSERT_EQ(multiLine3.at(2).at(0), p1);
ASSERT_EQ(multiLine3.at(2).at(1), p2);

// Удаление точки из линии
multiLine3.at(0).pop_back();
ASSERT_EQ(multiLine3.size(), 3);

ASSERT_EQ(multiLine3.at(0).size(), 3);
ASSERT_EQ(multiLine3.at(0).at(0), p3);
ASSERT_EQ(multiLine3.at(0).at(1), p4);
ASSERT_EQ(multiLine3.at(0).at(2), p5);
}
//==============================================================================
TEST(MultiLinestringTest, GeoMultiLinestring2DTest)
{
  // Создание пустой линии
  GeoMultiLinestring2D multiLine;
  ASSERT_TRUE(multiLine.empty());

  // Создание линии через вектор
  GeoPoint2D p1(Degree(1.), Degree(2.));
  GeoPoint2D p2(Degree(2.), Degree(3.));
  GeoPoint2D p3(Degree(3.), Degree(5.));
  GeoPoint2D p4(Degree(8.), Degree(7.));
  GeoPoint2D p5(Degree(4.), Degree(6.));

  GeoLinestring2D linestring1({ p1, p2 });
  GeoLinestring2D linestring2({ p3, p4, p5 });
  GeoLinestring2D linestring3;
  std::vector<GeoLinestring2D> vec({ linestring1, linestring2, linestring3 });

  GeoMultiLinestring2D multiLine2(vec.begin(), vec.end());
  ASSERT_EQ(multiLine2.size(), 3);

  ASSERT_EQ(multiLine2.at(0).size(), 2);
  ASSERT_EQ(multiLine2.at(0).at(0), p1);
  ASSERT_EQ(multiLine2.at(0).at(1), p2);

  ASSERT_EQ(multiLine2.at(1).size(), 3);
  ASSERT_EQ(multiLine2.at(1).at(0), p3);
  ASSERT_EQ(multiLine2.at(1).at(1), p4);
  ASSERT_EQ(multiLine2.at(1).at(2), p5);

  ASSERT_TRUE(multiLine2.at(2).empty());

  // Создание линии через список инициализации
  GeoMultiLinestring2D multiLine3({ linestring1, linestring2, linestring3 });
  ASSERT_EQ(multiLine3.size(), 3);

  ASSERT_EQ(multiLine3.at(0).size(), 2);
  ASSERT_EQ(multiLine3.at(0).at(0), p1);
  ASSERT_EQ(multiLine3.at(0).at(1), p2);

  ASSERT_EQ(multiLine3.at(1).size(), 3);
  ASSERT_EQ(multiLine3.at(1).at(0), p3);
  ASSERT_EQ(multiLine3.at(1).at(1), p4);
  ASSERT_EQ(multiLine3.at(1).at(2), p5);

  ASSERT_TRUE(multiLine3.at(2).empty());

  // Добавление новой линии
  multiLine3.push_back(linestring1);
  ASSERT_EQ(multiLine3.size(), 4);

  ASSERT_EQ(multiLine3.at(3).size(), 2);
  ASSERT_EQ(multiLine3.at(3).at(0), p1);
  ASSERT_EQ(multiLine3.at(3).at(1), p2);

  // Добавление точки к линии
  multiLine3.at(1).push_back(p2);
  ASSERT_EQ(multiLine3.size(), 4);

  ASSERT_EQ(multiLine3.at(1).size(), 4);
  ASSERT_EQ(multiLine3.at(1).at(0), p3);
  ASSERT_EQ(multiLine3.at(1).at(1), p4);
  ASSERT_EQ(multiLine3.at(1).at(2), p5);
  ASSERT_EQ(multiLine3.at(1).at(3), p2);

  // Удаление линии
  multiLine3.erase(multiLine3.begin());
  ASSERT_EQ(multiLine3.size(), 3);

  ASSERT_EQ(multiLine3.at(0).size(), 4);
  ASSERT_EQ(multiLine3.at(0).at(0), p3);
  ASSERT_EQ(multiLine3.at(0).at(1), p4);
  ASSERT_EQ(multiLine3.at(0).at(2), p5);
  ASSERT_EQ(multiLine3.at(0).at(3), p2);

  ASSERT_TRUE(multiLine3.at(1).empty());

  ASSERT_EQ(multiLine3.at(2).size(), 2);
  ASSERT_EQ(multiLine3.at(2).at(0), p1);
  ASSERT_EQ(multiLine3.at(2).at(1), p2);

  // Удаление точки из линии
  multiLine3.at(0).pop_back();
  ASSERT_EQ(multiLine3.size(), 3);

  ASSERT_EQ(multiLine3.at(0).size(), 3);
  ASSERT_EQ(multiLine3.at(0).at(0), p3);
  ASSERT_EQ(multiLine3.at(0).at(1), p4);
  ASSERT_EQ(multiLine3.at(0).at(2), p5);
}
//==============================================================================
TEST(MultiLinestringTest, GeoMultiLinestring3DTest)
{
  // Создание пустой линии
  GeoMultiLinestring3D multiLine;
  ASSERT_TRUE(multiLine.empty());

  // Создание линии через вектор
  GeoPoint3D p1(Degree(1.), Degree(2.), 3.);
  GeoPoint3D p2(Degree(2.), Degree(3.), 8.);
  GeoPoint3D p3(Degree(3.), Degree(5.), 0.);
  GeoPoint3D p4(Degree(8.), Degree(7.), 5.);
  GeoPoint3D p5(Degree(4.), Degree(6.), 3.);

  GeoLinestring3D linestring1({ p1, p2 });
  GeoLinestring3D linestring2({ p3, p4, p5 });
  GeoLinestring3D linestring3;
  std::vector<GeoLinestring3D> vec({ linestring1, linestring2, linestring3 });

  GeoMultiLinestring3D multiLine2(vec.begin(), vec.end());
  ASSERT_EQ(multiLine2.size(), 3);

  ASSERT_EQ(multiLine2.at(0).size(), 2);
  ASSERT_EQ(multiLine2.at(0).at(0), p1);
  ASSERT_EQ(multiLine2.at(0).at(1), p2);

  ASSERT_EQ(multiLine2.at(1).size(), 3);
  ASSERT_EQ(multiLine2.at(1).at(0), p3);
  ASSERT_EQ(multiLine2.at(1).at(1), p4);
  ASSERT_EQ(multiLine2.at(1).at(2), p5);

  ASSERT_TRUE(multiLine2.at(2).empty());

  // Создание линии через список инициализации
  GeoMultiLinestring3D multiLine3({ linestring1, linestring2, linestring3 });
  ASSERT_EQ(multiLine3.size(), 3);

  ASSERT_EQ(multiLine3.at(0).size(), 2);
  ASSERT_EQ(multiLine3.at(0).at(0), p1);
  ASSERT_EQ(multiLine3.at(0).at(1), p2);

  ASSERT_EQ(multiLine3.at(1).size(), 3);
  ASSERT_EQ(multiLine3.at(1).at(0), p3);
  ASSERT_EQ(multiLine3.at(1).at(1), p4);
  ASSERT_EQ(multiLine3.at(1).at(2), p5);

  ASSERT_TRUE(multiLine3.at(2).empty());

  // Добавление новой линии
  multiLine3.push_back(linestring1);
  ASSERT_EQ(multiLine3.size(), 4);

  ASSERT_EQ(multiLine3.at(3).size(), 2);
  ASSERT_EQ(multiLine3.at(3).at(0), p1);
  ASSERT_EQ(multiLine3.at(3).at(1), p2);

  // Добавление точки к линии
  multiLine3.at(1).push_back(p2);
  ASSERT_EQ(multiLine3.size(), 4);

  ASSERT_EQ(multiLine3.at(1).size(), 4);
  ASSERT_EQ(multiLine3.at(1).at(0), p3);
  ASSERT_EQ(multiLine3.at(1).at(1), p4);
  ASSERT_EQ(multiLine3.at(1).at(2), p5);
  ASSERT_EQ(multiLine3.at(1).at(3), p2);

  // Удаление линии
  multiLine3.erase(multiLine3.begin());
  ASSERT_EQ(multiLine3.size(), 3);

  ASSERT_EQ(multiLine3.at(0).size(), 4);
  ASSERT_EQ(multiLine3.at(0).at(0), p3);
  ASSERT_EQ(multiLine3.at(0).at(1), p4);
  ASSERT_EQ(multiLine3.at(0).at(2), p5);
  ASSERT_EQ(multiLine3.at(0).at(3), p2);

  ASSERT_TRUE(multiLine3.at(1).empty());

  ASSERT_EQ(multiLine3.at(2).size(), 2);
  ASSERT_EQ(multiLine3.at(2).at(0), p1);
  ASSERT_EQ(multiLine3.at(2).at(1), p2);

  // Удаление точки из линии
  multiLine3.at(0).pop_back();
  ASSERT_EQ(multiLine3.size(), 3);

  ASSERT_EQ(multiLine3.at(0).size(), 3);
  ASSERT_EQ(multiLine3.at(0).at(0), p3);
  ASSERT_EQ(multiLine3.at(0).at(1), p4);
  ASSERT_EQ(multiLine3.at(0).at(2), p5);
}
//==============================================================================
} //! Tests
} // ! PrimaryEntities
//==============================================================================
