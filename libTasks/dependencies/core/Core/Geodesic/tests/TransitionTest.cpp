#include <Geodesic/Projection/Transition.hpp>

#include <Geodesic/Projection/Orthographic/ProjectionBuilder.hpp>
#include <Geodesic/Projection/Orthographic/Operations.hpp>
#include <Geodesic/Helper/Ellipsoid.hpp>
//! PrimaryEntities
#include <PrimaryEntities/Equalities.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Union.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
using BasicMath::isEqual;
using namespace AbstractOperations;
//==============================================================================
/* Построить квадрат с центром в данной точке и длиной стороны в градусах (!)
*\param[in] center - центр квадрата (географическая точка)
*\param[in] size - длина стороны квадрата [градусы]
*\return - построенный квадрат
*/
GeoPolygon2D buildGeogSquare( const GeoPoint2D center, const Degree size )
{
  GeoPolygon2D retVal;
  Radian lat = center.getLatitude( );
  Radian lon = center.getLongitude( );
  Radian shift = size.toRadians( ) / Radian( 2 );
  retVal.outer( ).push_back( GeoPoint2D( lat + shift, lon + shift ) );
  retVal.outer( ).push_back( GeoPoint2D( lat - shift, lon + shift ) );
  retVal.outer( ).push_back( GeoPoint2D( lat - shift, lon - shift ) );
  retVal.outer( ).push_back( GeoPoint2D( lat + shift, lon - shift ) );
  retVal.outer( ).push_back( GeoPoint2D( lat + shift, lon + shift ) );
  return retVal;
}
//==============================================================================
/* Построить на плоскости квадрат с центром в данной точке и данной длиной 
*  стороны
*\param[in] center - центр квадрата (географическая точка)
*\param[in] size - длина стороны квадрата [градусы]
*\return - построенный квадрат
*/
Polygon2D buildFlatSquare( const Point2D center, const double size )
{
  Polygon2D retVal;
  double x = center.getX( );
  double y = center.getY( );
  double shift = size / 2;
  retVal.outer( ).push_back( Point2D( x + shift, y + shift ) );
  retVal.outer( ).push_back( Point2D( x - shift, y + shift ) );
  retVal.outer( ).push_back( Point2D( x - shift, y - shift ) );
  retVal.outer( ).push_back( Point2D( x + shift, y - shift ) );
  retVal.outer( ).push_back( Point2D( x + shift, y + shift ) );
  return retVal;
}
//==============================================================================
double error_( const double value, const double etalon )
{
  return fabs( ( fabs(value) - fabs(etalon) ) / etalon );
}

// допустимое отклонение при сравнении величин: 0.013%
const double eps = 0.00013; 
//==============================================================================
//==============================================================================
class TransitionTest : public ::testing::Test
{
protected:
  void SetUp( )
  {
    Geodesic::Ellipsoid ell( Geodesic::EL_WGS_84 );
    earthR = ell.getMajorRadius();

    point.set( 100, 100 );
    center.set( Degree( 0 ), Degree( 100 ) );
    center180.set( Degree( 0 ), Degree( 179.99 ) );

    smallSquare = buildGeogSquare( center, Degree( 1 ) );
    gpolyg_2 = buildGeogSquare( center, Degree( 2 ) );
    gpolyg_3 = buildGeogSquare( center, Degree( 3 ) );
    bigSquare = buildGeogSquare( center, Degree( 4 ) );

    polyg_1 = buildFlatSquare( point, ( 1 ) );
    polyg_2 = buildFlatSquare( point, ( 2 ) );
    polyg_3 = buildFlatSquare( point, ( 3 ) );
    polyg_4 = buildFlatSquare( point, ( 4 ) );
  }
//==============================================================================
  void TearDown( )
  {
  }
//==============================================================================

  double earthR; // экваториальный радиус Земли

  Point2D point;

  GeoPoint2D center; // обычный центр
  GeoPoint2D center180; // центр возле 180-го меридиана

  GeoPolygon2D smallSquare; // обычный квадрат со стороной 1
  GeoPolygon2D gpolyg_2; // обычный квадрат со стороной 2
  GeoPolygon2D gpolyg_3; // обычный квадрат со стороной 3
  GeoPolygon2D bigSquare; // обычный квадрат со стороной 4

  Polygon2D polyg_1; // квадрат на плоскости со стороной 1
  Polygon2D polyg_2; // квадрат на плоскости со стороной 2
  Polygon2D polyg_3; // квадрат на плоскости со стороной 3
  Polygon2D polyg_4; // квадрат на плоскости со стороной 4
};
//==============================================================================
GeoPolygon2D createBow( )
{
  GeoPolygon2D polyg;

  polyg.outer( ).push_back( GeoPoint2D( Degree( 4 ), Degree( 104 ) ) );
  polyg.outer( ).push_back( GeoPoint2D( Degree( 4 ), Degree( 104 ) ) );
  polyg.outer( ).push_back( GeoPoint2D( Degree( -4 ), Degree( 96 ) ) );
  polyg.outer( ).push_back( GeoPoint2D( Degree( 4 ), Degree( 96 ) ) );
  polyg.outer( ).push_back( GeoPoint2D( Degree( -4 ), Degree( 104 ) ) );
  polyg.outer( ).push_back( GeoPoint2D( Degree( 4 ), Degree( 104 ) ) );

  return polyg;
}
//==============================================================================
GeoPolygon2D createBridge( )
{
  GeoPolygon2D polyg;

  polyg.outer( ).push_back( GeoPoint2D( Degree( 0 ), Degree( 100 ) ) );
  polyg.outer( ).push_back( GeoPoint2D( Degree( 0 ), Degree( 101 ) ) );
  polyg.outer( ).push_back( GeoPoint2D( Degree( 0 ), Degree( 103 ) ) );
  polyg.outer( ).push_back( GeoPoint2D( Degree( 0 ), Degree( 104 ) ) );
  polyg.outer( ).push_back( GeoPoint2D( Degree( -1 ), Degree( 104 ) ) );
  polyg.outer( ).push_back( GeoPoint2D( Degree( -1 ), Degree( 103 ) ) );
  polyg.outer( ).push_back( GeoPoint2D( Degree( 1 ), Degree( 102 ) ) );
  polyg.outer( ).push_back( GeoPoint2D( Degree( -1 ), Degree( 101 ) ) );
  polyg.outer( ).push_back( GeoPoint2D( Degree( -1 ), Degree( 100 ) ) );
  polyg.outer( ).push_back( GeoPoint2D( Degree( 0 ), Degree( 100 ) ) );

  return polyg;
}
//==============================================================================
TEST_F( TransitionTest, differentGeometriesToFromProjectionTest )
{
  // выбираем оптимальную проекцию для тестовой фигуры 
  // (проверено в ComplicatedOperations)
  auto proj = std::make_shared<OrthographicProjection>( 
    findOptimalProjection<GeoMultiPolygon2D>( { bigSquare } ) );
  // запоминаем правильный результат проецирования тестовой фигуры 
  // (проверен в ComplicatedOperations)
  GeoMultiPolygon2D geo( { bigSquare } );
  ASSERT_EQ( GeoPoint3D(center, 0), proj->getCenterProjection( ) );
  auto etalon = geoGeometryToProjection( geo, center );
  auto result = etalon[0];

  // мультиполигон проецируется как надо
  auto resMult = geoGeometryToProjection( geo, proj );
  ASSERT_EQ( resMult, etalon );
  // и нормально возвращается из проекции
  GeoMultiPolygon2D restoredMult(geoGeometryFromProjection(resMult, proj));
  ASSERT_TRUE(
    areEqualUpToRotate( restoredMult.at(0).outer(), geo.at(0).outer() ) );

  // запоминаем правильный результат проецирования тестовой фигуры 
  etalon = geoGeometryToProjection(geo, center);

  // point проецируется как надо
  GeoPoint2D geoPoint( bigSquare.outer()[0] );
  auto resPoint = geoGeometryToProjection( geoPoint, proj );
  // При проецировании мультиполигона нулевая вершина геометрии
  // оказалась под другим индексом
  ASSERT_EQ(resPoint, result.outer()[3]);
  // и нормально возвращается из проекции
  GeoPoint2D restoredPoint = geoGeometryFromProjection( resPoint, proj );
  ASSERT_EQ(restoredPoint, geoPoint);

  // коллекция точек проецируется как надо
  GeoMultiPoint2D geoMultiPoint{ bigSquare.outer()[0], bigSquare.outer()[1] };
  auto resMultiPoint = geoGeometryToProjection( geoMultiPoint, proj );
  ASSERT_EQ( resMultiPoint[0], result.outer( )[3] );
  ASSERT_EQ( resMultiPoint[1], result.outer( )[4] );
  // и нормально возвращается из проекции
  GeoMultiPoint2D restoredMultiPoints(geoGeometryFromProjection(resMultiPoint,
                                                                proj));
  ASSERT_EQ(restoredMultiPoints, geoMultiPoint);

  // box проецируется как надо
  GeoBox2D geoBox ( bigSquare.outer( )[2], bigSquare.outer( )[0] );
  auto resBox = geoGeometryToProjection( geoBox, proj );
  ASSERT_EQ( resBox.min_corner( ), result.outer( )[1] );
  ASSERT_EQ( resBox.max_corner( ), result.outer( )[3] );
  // и нормально возвращается из проекции
  GeoBox2D restoredBox = geoGeometryFromProjection( resBox, proj );
  ASSERT_EQ( restoredBox, geoBox );

  // linestring проецируется как надо
  GeoLinestring2D geoLine{bigSquare.outer( )[0], bigSquare.outer( )[1], 
                          bigSquare.outer( )[2], bigSquare.outer( )[3] };
  auto resLine = geoGeometryToProjection( geoLine, proj );
  for( size_t i = 0; i < 4; i++ )
    ASSERT_EQ( resLine[i], result.outer( )[(i + 3) % 4] );
  // и нормально возвращается из проекции
  GeoLinestring2D restoredLine = geoGeometryFromProjection( resLine, proj );
  ASSERT_EQ( restoredLine, geoLine );
    
  // std::vector проецируется как надо
  std::vector<GeoPoint2D> 
    geoVect{ bigSquare.outer( )[0], bigSquare.outer( )[1], 
             bigSquare.outer( )[2], bigSquare.outer( )[3] };
  auto resVect = geoGeometryToProjection( geoVect, proj );
  for( size_t i = 0; i < 4; i++ )
    ASSERT_EQ( resVect[i], result.outer( )[(i + 3) % 4] );
  // и нормально возвращается из проекции
  std::vector<GeoPoint2D> restoredVect 
    = geoGeometryFromProjection( resVect, proj );
  ASSERT_EQ( restoredVect, geoVect );

  // ring проецируется как надо
  GeoRing2D geoRing = bigSquare.outer( );
  auto resRing = geoGeometryToProjection( geoRing, proj );
  ASSERT_TRUE( areEqualUpToRotate( resRing, result.outer( ) ) );
  // и нормально возвращается из проекции
  GeoRing2D restoredRing(geoGeometryFromProjection( resRing, proj ));
  ASSERT_EQ( restoredRing, geoRing );

  // polygon проецируется как надо
  GeoPolygon2D geoPolyg = bigSquare;
  auto resPolyg = geoGeometryToProjection( geoPolyg, proj );
  ASSERT_TRUE( areEqualUpToRotate( resPolyg.outer(), result.outer() ) );
  // и нормально возвращается из проекции
  GeoPolygon2D restoredPolyg(geoGeometryFromProjection(resPolyg, proj));
  ASSERT_TRUE( areEqualUpToRotate( restoredPolyg.outer(), geoPolyg.outer() ) );
}
//==============================================================================
TEST_F( TransitionTest, ForceProjTest )
{
  auto small = buildGeogSquare( GeoPoint2D( ), Degree( 1 ) ).outer();
  auto big = buildGeogSquare( GeoPoint2D( ), Degree( 3 ) ).outer();

  GeoPolygon2D poly;
  poly.outer() = big;
  poly.inners( ).push_back( small );
  GeoMultiPolygon2D mult( { poly } );

  // выбираем оптимальную проекцию для тестовой фигуры (проверено выше)
  auto proj = findOptimalProjection<GeoMultiPolygon2D>( { poly } );
  
  // для корректных геометрий работает так же, как уже проверенные функции
  // с точностью до направления
  auto ring1 = geoGeometryToProjection( small, 
    std::make_shared<OrthographicProjection>( proj ) );
  auto ring2 = geoGeometryToProjectionForce( small, 
    std::make_shared<OrthographicProjection>( proj ) );
  ASSERT_EQ( ring1, ring2 );

  auto pol1 = geoGeometryToProjection( poly, 
    std::make_shared<OrthographicProjection>( proj ) );
  auto pol2 = geoGeometryToProjectionForce( poly, 
    std::make_shared<OrthographicProjection>( proj ) );
  ASSERT_TRUE( areEqualUpToRotate( pol1.outer(), pol2.outer() ) );
  correct( pol2 );
  ASSERT_EQ( pol1.inners().size(), 1 );
  ASSERT_EQ( pol2.inners().size(), 1 );
  ASSERT_TRUE(
    areEqualUpToRotate( pol1.inners( ).at(0), pol2.inners( ).at(0) ) );
  pol1 = geoGeometryToProjection( poly, 
    std::make_shared<OrthographicProjection>( proj ), false );
  pol2 = geoGeometryToProjectionForce( poly, 
    std::make_shared<OrthographicProjection>( proj ), false );
  ASSERT_TRUE(
    areEqualUpToRotate( pol1.outer( ), pol2.outer( ) ) );
  correct( pol2 );
  ASSERT_EQ( pol1.inners().size(), 0 );
  ASSERT_EQ( pol2.inners().size(), 0 );

  auto mul1 = geoGeometryToProjection( mult, 
    std::make_shared<OrthographicProjection>( proj ) );
  auto mul2 = geoGeometryToProjectionForce( mult, 
    std::make_shared<OrthographicProjection>( proj ) );
  ASSERT_TRUE( areEqualUpToRotate( mul1[0].outer( ), mul2[0].outer( ) ) );
  correct( mul2[0] );
  ASSERT_EQ( mul1[0].inners().size(), 1 );
  ASSERT_EQ( mul2[0].inners().size(), 1 );
  ASSERT_TRUE(
    areEqualUpToRotate( mul1[0].inners().at(0), mul2[0].inners().at(0) ) );
  mul1 = geoGeometryToProjection( mult, 
    std::make_shared<OrthographicProjection>( proj ), false );
  mul2 = geoGeometryToProjectionForce( mult, 
    std::make_shared<OrthographicProjection>( proj ), false );
  ASSERT_TRUE( areEqualUpToRotate(mul1[0].outer( ), mul2[0].outer( ) ) );
  correct( mul2[0] );
  ASSERT_EQ( mul1[0].inners().size(), 0 );
  ASSERT_EQ( mul2[0].inners().size(), 0 );

  // также можно проецировать сильно некорректные конструкции
  auto bow = createBow( );
  proj = findOptimalProjection<GeoMultiPolygon2D>( { bow } );
  ASSERT_NO_THROW( geoGeometryToProjectionForce( bow, 
    std::make_shared<OrthographicProjection>( proj ) ) );
}
//==============================================================================
} //! Tests
} //! Geodesic
//==============================================================================