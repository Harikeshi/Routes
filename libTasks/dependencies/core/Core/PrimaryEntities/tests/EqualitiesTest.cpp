#include <PrimaryEntities/Equalities.hpp>
#include <PrimaryEntities/PrimaryEntitiesExceptions.hpp>
#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
#include <MathTools/BasicMath/Compare.hpp>
#include <SamplesForTests/BaseGeoFigures.hpp>
#include <SamplesForTests/BaseFlatFigures.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace PrimaryEntities
{
namespace Tests
{
//==============================================================================
TEST( EqualitiesTest, BoxEquality )
{
  Box2D first( Point2D( ), Point2D( 2., 3. ) );
  Box2D second( first );
  ASSERT_EQ( first, second );
  ASSERT_TRUE( first == second );
  ASSERT_TRUE( first != Box2D( ) );

  GeoBox3D one( GeoPoint3D( ), GeoPoint3D( Degree( 1 ), Degree( 2 ), 3 ) );
  GeoBox3D other( one );
  // box равен сам себе
  ASSERT_EQ( one, other );
  // небольшие отклонения отбрасываются
  other.max_corner( ).setHeight( 3. + ACCURACY_DOUBLE / 2 );
  ASSERT_EQ( one, other );
  // бОльшие отклонения приводят к неравенству
  other.max_corner( ).setHeight( 3. + ACCURACY_DOUBLE * 2 );
  ASSERT_NE( one, other );
}
//==============================================================================
TEST( EqualitiesTest, PolygonEquality )
{
  // возьмем некорректный, но синтаксически верно заданный полигон
  Polygon3D first(
  { {Point3D( ), Point3D( 1., 2., 3. ), Point3D( 4., 5., 6. ), Point3D( )},
  {Point3D( 3., 2., 1. ), Point3D( ) }, { Point3D( 2., 3., 4. ) } } );
  Polygon3D second( first );
  // полигон равен сам себе
  ASSERT_EQ( first, second );
  // небольшие отклонения отбрасываются
  second.inners( ).back( ).back( ).set(( 2. + ACCURACY_DOUBLE / 2),
                                       second.inners().back().back().getY(),
                                       second.inners().back().back().getZ());
  ASSERT_EQ( first, second );
  // бОльшие отклонения приводят к неравенству
  second.inners( ).back( ).back( ).set((2. + ACCURACY_DOUBLE * 2),
                                       second.inners().back().back().getY(),
                                       second.inners().back().back().getZ());
  ASSERT_NE( first, second );
  // число колец важно
  first.inners( ).pop_back( );
  ASSERT_NE( first, second );
  // внешняя граница также проверяется на равенство
  first = second;
  first.outer( ).back( ).set( 100,
                              second.inners().back().back().getY(),
                              second.inners().back().back().getZ());
  ASSERT_NE( first, second );
}
//==============================================================================
TEST( EqualityTest, MultyPolygonEquality )
{
  // странный модельный мультиполигон
  MultiPolygon2D first( { { {Point2D( ), Point2D( 1., 1. )} },
    { {Point2D( 3., 6. )} },
    { { Point2D( 10., 20. )} } } );
  MultiPolygon2D second( first );
  ASSERT_EQ( first, second );
  // если один из полигонов отличается, мультиполигоны различны
  second[2].outer( ).push_back( Point2D( ) );
  ASSERT_NE( first, second );
  // полигонов должно быть одинаковое количество
  first.pop_back( );
  ASSERT_NE( first, second );
}
//==============================================================================
TEST(EqualitiesTest, AreEqualUpToRotateSquares)
{
  // квадраты с одинаковыми координатами(по сути, но не по факту),
  // обходами вершин, но с разными стартовыми вершинами(считаются равными)
  GeoRing2D firstRing(
  {
    GeoPoint2D(Degree(0.), Degree(0.)),
    GeoPoint2D(Degree(1.), Degree(0.)),
    GeoPoint2D(Degree(1.), Degree(1.)),
    GeoPoint2D(Degree(0.), Degree(1.)),
    GeoPoint2D(Degree(0.), Degree(0.))
  });

  GeoRing2D secondRing(
  {
    GeoPoint2D(Degree(1.), Degree(1.)),
    GeoPoint2D(Degree(0.), Degree(1.)),
    GeoPoint2D(Degree(0.), Degree(0.)),
    GeoPoint2D(Degree(1.), Degree(0.)),
    GeoPoint2D(Degree(1.), Degree(1.))
  });

  // сравниваем квадраты => равны
  ASSERT_TRUE(areEqualUpToRotate(firstRing, secondRing));
}
//==============================================================================
TEST(EqualitiesTest, AreEqualUpToRotateWithoutSameFirstCoord)
{
  // четырехугольники с разными стартовыми вершинами и,
  // соответсвенно, последними (считаются разными).
  // (во второй фигуре нет начальной координаты первой фигуры,
  // как было при сравнении квадратов) 
  GeoRing2D firstRing(
  {
    GeoPoint2D(Degree(1.), Degree(1.)),
    GeoPoint2D(Degree(1.), Degree(3.)),
    GeoPoint2D(Degree(0.), Degree(3.)),
    GeoPoint2D(Degree(0.), Degree(1.)),
    GeoPoint2D(Degree(1.), Degree(1.))
  });

  GeoRing2D secondRing(
  {
    GeoPoint2D(Degree(1.), Degree(0.)),
    GeoPoint2D(Degree(1.), Degree(3.)),
    GeoPoint2D(Degree(0.), Degree(3.)),
    GeoPoint2D(Degree(0.), Degree(1.)),
    GeoPoint2D(Degree(1.), Degree(0.))
  });

  // сравниваем домики => не равны
  ASSERT_FALSE(areEqualUpToRotate(firstRing, secondRing));
}
//==============================================================================
TEST(EqualitiesTest, AreEqualUpToRotateTriangle)
{
  // треугольники с одинаковыми координатами,
  // одинаковыми обходами вершин и стартовыми вершинами(полностью равны)
  GeoRing2D firstRing(
  {
    GeoPoint2D(Degree(0.), Degree(0.)),
    GeoPoint2D(Degree(2.), Degree(1.)), 
    GeoPoint2D(Degree(0.), Degree(2.)), 
    GeoPoint2D(Degree(0.), Degree(0.)) 
  });

  // сравниваем треугольники => равны
  ASSERT_TRUE(areEqualUpToRotate(firstRing, firstRing));
}
//==============================================================================
TEST(EqualitiesTest, AreEqualUpToRotateFigures)
{
  // фигуры с одинаковыми координатами, разными последовательностями вершин
  // и одинаковыми стартовыми вершинами(фигуры не равны)
  GeoRing2D firstRing(
  {
    GeoPoint2D(Degree(2.), Degree(0.)),
    GeoPoint2D(Degree(2.), Degree(2.)),
    GeoPoint2D(Degree(4.), Degree(3.)),
    GeoPoint2D(Degree(2.), Degree(4.)),
    GeoPoint2D(Degree(2.), Degree(6.)),
    GeoPoint2D(Degree(0.), Degree(3.)),
    GeoPoint2D(Degree(2.), Degree(0.))
  });

  GeoRing2D secondRing(
  {
    GeoPoint2D(Degree(2.), Degree(0.)),
    GeoPoint2D(Degree(4.), Degree(3.)),
    GeoPoint2D(Degree(2.), Degree(2.)),
    GeoPoint2D(Degree(2.), Degree(4.)),
    GeoPoint2D(Degree(2.), Degree(6.)),
    GeoPoint2D(Degree(0.), Degree(3.)),
    GeoPoint2D(Degree(2.), Degree(0.))
  });

  // сравниваем фигуры => не равны
  ASSERT_FALSE(areEqualUpToRotate(firstRing, secondRing));
}
//==============================================================================
TEST(EqualitiesTest, AreEqualUpToRotateBow)
{
  // бантик сравнивается с самим собой и показывает,
  // что некорректные фигуры тоже обрабатываются в данных тестах
  GeoRing2D firstRing = SamplesForTests::buildBow( 
  GeoPoint2D(Degree(60.), Degree(120.)), Degree(10.), Degree(30.) 
  ).outer();

  // сравниваем бантик => равен
  ASSERT_TRUE(areEqualUpToRotate(firstRing, firstRing));
}
//==============================================================================
TEST(EqualitiesTest, AreEqualUpToRotateNotRing)
{
  // незамкнутая фигура сравнивается с самой собой,
  // так как первая точка не равна последней,
  // то кидается исключение, что ринг некорректный
  GeoRing2D firstRing(
  {
    GeoPoint2D(Degree(0.), Degree(1.)),
    GeoPoint2D(Degree(0.), Degree(0.)),
    GeoPoint2D(Degree(1.), Degree(0.)),
    GeoPoint2D(Degree(1.), Degree(1.))
  });

  // кидаем исключение
  ASSERT_THROW(areEqualUpToRotate(firstRing, firstRing),
               PrimaryEntitiesFailure);
  try
  {
    areEqualUpToRotate(firstRing, firstRing);
  }
  catch (const PrimaryEntitiesFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), IncorrectRing);
    ASSERT_EQ(primaryLibTitle, exception.getLib());
  }
}
//==============================================================================
TEST(EqualitiesTest, AreEqualUpToRotateFlatRectangle)
{
  // простые прямоугольники с разными длинами сторон показывают,
  // что фигуры на плоскоси тоже обрабатываются (не равны)
  Ring2D firstRing = SamplesForTests::buildRectangle(
  Point2D(1, 2), 3, 4).outer();

  Ring2D secondRing = SamplesForTests::buildRectangle(
  Point2D(1, 2), 4, 5).outer();

  // сравниваем прямоугольники => не равны
  ASSERT_FALSE(areEqualUpToRotate(firstRing, secondRing));
}
//==============================================================================
} //! Tests
} //! PrimaryEntities
//==============================================================================
