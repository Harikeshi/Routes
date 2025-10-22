//#include <PrimaryEntities/Box.hpp>

#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Tests
{
//==============================================================================
TEST( BoxTest, ExpandBoxToPoint2DTest )
{
  Box2D box ( Point2D(), Point2D(1, 1) );

  // правый угол двигается
  Point2D maxP( 3., 2. );
  box.expandToPoint( maxP );
  ASSERT_EQ( box.min_corner( ), Point2D( ) );
  ASSERT_EQ( box.max_corner( ), maxP );

  // левый угол двигается
  Point2D minP( -2., -3. );
  box.expandToPoint( minP );
  ASSERT_EQ( box.min_corner( ), minP );
  ASSERT_EQ( box.max_corner( ), maxP );

  // внутренняя точка не меняет коробку
  box.expandToPoint( Point2D( ) );
  ASSERT_EQ( box.min_corner( ), minP );
  ASSERT_EQ( box.max_corner( ), maxP );
}
//==============================================================================
TEST(BoxTest, ExpandBoxToGeoPoint3DTest)
{
  GeoBox3D box(GeoPoint3D(), GeoPoint3D(Degree(1), Degree(1), 1));

  // правый угол двигается
  GeoPoint3D maxP(Degree(3.), Degree(2.), 5.);
  box.expandToPoint(maxP);
  ASSERT_EQ(box.min_corner(), GeoPoint3D());
  ASSERT_EQ(box.max_corner(), maxP);

  // левый угол двигается
  GeoPoint3D minP(Degree(-2.), Degree(-3.));
  box.expandToPoint(minP);
  ASSERT_EQ(box.min_corner(), minP);
  ASSERT_EQ(box.max_corner(), maxP);

  // внутренняя точка не меняет коробку
  box.expandToPoint(GeoPoint3D());
  ASSERT_EQ(box.min_corner(), minP);
  ASSERT_EQ(box.max_corner(), maxP);

  // можно получать копию растягиваемой box без изменения оригинала
  box = GeoBox3D(GeoPoint3D(), GeoPoint3D(Degree(1), Degree(1), 1));
  auto other = box.getBoxExpandedToPoint(maxP);
  ASSERT_EQ(box.min_corner(), GeoPoint3D());
  ASSERT_EQ(box.max_corner(), GeoPoint3D(Degree(1), Degree(1), 1));
  ASSERT_EQ(other.max_corner(), maxP);
}
//==============================================================================
TEST( BoxTest, ExpandToBoxTest )
{
  Box2D box( Point2D( ), Point2D( 1, 1 ) );
  Box2D maxBox( Point2D( 1, 1 ), Point2D( 2, 3 ) );
  Box2D minBox( Point2D( -2, -3 ), Point2D( ) );

  // правый угол двигается
  box.expandToBox( maxBox );
  ASSERT_EQ( box.min_corner( ), Point2D( ) );
  ASSERT_EQ( box.max_corner( ), maxBox.max_corner( ) );

  // левый угол двигается
  box.expandToBox( minBox );
  ASSERT_EQ( box.min_corner( ), minBox.min_corner( ) );
  ASSERT_EQ( box.max_corner( ), maxBox.max_corner( ) );

  // внутренняя коробка не меняет коробку
  box.expandToBox( Box2D( Point2D( ), Point2D( 1, 1 ) ) );
  ASSERT_EQ( box.min_corner( ), minBox.min_corner( ) );
  ASSERT_EQ( box.max_corner( ), maxBox.max_corner( ) );

  // можно получить увеличенный box, не испортив оригинал
  box = Box2D( Point2D( ), Point2D( 1, 1 ) );
  auto other = box.getBoxExpandedToBox(
    Box2D( minBox.min_corner( ), maxBox.max_corner( ) ) );
  ASSERT_EQ( other.min_corner( ), minBox.min_corner( ) );
  ASSERT_EQ( other.max_corner( ), maxBox.max_corner( ) );
  ASSERT_EQ( box.min_corner( ), Point2D( ) );
}
//==============================================================================
TEST( BoxTest, BoxIntersectionTest )
{
  Box2D box( Point2D( ), Point2D( 1, 2 ) );

  // коробки умеют пересекаться
  Box2D closeBox( Point2D( 0.5, 1 ), Point2D( 2, 3 ) );
  ASSERT_TRUE( box.hasIntersectionWith( closeBox ) );

  // могут не пересекаться
  Box2D farBox( Point2D( 9, 10 ), Point2D( 11, 12 ) );
  ASSERT_FALSE( box.hasIntersectionWith( farBox ) );

  // касающиеся коробки пересекаются
  Box2D tangentBox( Point2D( 1, 2 ), Point2D( 3, 4 ) );
  ASSERT_TRUE( box.hasIntersectionWith( tangentBox ) );

  // точки можно использовать разные
  GeoBox3D otherBox(GeoPoint3D(), GeoPoint3D(Degree(1.), Degree(2.), 3.));
  GeoBox3D close(GeoPoint3D(Degree(1.), Degree(2.), 3.),
    GeoPoint3D(Degree(2.), Degree(3.), 4.));
  GeoBox3D far(GeoPoint3D(Degree(1.), Degree(2.), 5.),
    GeoPoint3D(Degree(2.), Degree(3.), 10.));
  ASSERT_TRUE(otherBox.hasIntersectionWith(close));
  ASSERT_FALSE(otherBox.hasIntersectionWith(far));
}
//==============================================================================
/*!
 * \brief Тестирование перевода box -> polygon
 */
TEST( BoxTest, BoxToPolygon )
{
  //! Вариант для эллипсоида
  GeoBox2D box( GeoPoint2D( Degree( 0 ), Degree( 0 ) ),
    GeoPoint2D( Degree( 10 ), Degree( 20 ) ) );
  GeoPolygon2D polygon = box.toPolygon();

  ASSERT_EQ( 5, polygon.outer( ).size( ) );
  ASSERT_EQ( 0, polygon.inners( ).size( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 0 ) ),
             polygon.outer( ).at( 0 ) );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ),
             polygon.outer( ).at( 1 ) );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 20 ) ),
             polygon.outer( ).at( 2 ) );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 20 ) ),
             polygon.outer( ).at( 3 ) );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 0 ) ),
             polygon.outer( ).at( 4 ) );

  //! Вариант на плоскости
  Box2D otherBox( Point2D( ), Point2D( 20, 10 ) );
  Polygon2D otherPoly = otherBox.toPolygon();

  ASSERT_EQ( 5, otherPoly.outer( ).size( ) );
  ASSERT_EQ( 0, otherPoly.inners( ).size( ) );
  ASSERT_EQ( Point2D( 0, 0 ), otherPoly.outer( ).at( 0 ) );
  ASSERT_EQ( Point2D( 0, 10 ), otherPoly.outer( ).at( 1 ) );
  ASSERT_EQ( Point2D( 20, 10 ), otherPoly.outer( ).at( 2 ) );
  ASSERT_EQ( Point2D( 20, 0 ), otherPoly.outer( ).at( 3 ) );
  ASSERT_EQ( Point2D( 0, 0 ), otherPoly.outer( ).at( 4 ) );
}
//==============================================================================
} // ! Tests
//==============================================================================
