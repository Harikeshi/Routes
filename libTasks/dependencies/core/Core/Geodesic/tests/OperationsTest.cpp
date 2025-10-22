#include <Geodesic/Projection/Orthographic/Operations.hpp>

#include <Geodesic/Projection/Orthographic/ProjectionBuilder.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Union.hpp>
#include <AbstractOperations/Algorithms/Helper/CorrectHelper.hpp>
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
GeoPolygon2D buildGeodesicSquare( const GeoPoint2D center, const Degree size )
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
double aberration( const double value, const double etalon )
{
  return fabs( ( fabs(value) - fabs(etalon) ) / etalon );
}

// допустимое отклонение при сравнении величин: 0.013%
const double eps = 0.00013; 
//==============================================================================
//==============================================================================
class ComplicatedOperationsTest : public ::testing::Test
{
protected:
  void SetUp( )
  {
    center.set( Degree( 0 ), Degree( 100 ) );

    smallSquare = buildGeodesicSquare( center, Degree( 1 ) );
    bigSquare = buildGeodesicSquare( center, Degree( 4 ) );
  }
//==============================================================================
  void TearDown( )
  {
  }
//==============================================================================
  GeoPoint2D center; // обычный центр

  GeoPolygon2D smallSquare; // обычный квадрат со стороной 1
  GeoPolygon2D bigSquare; // обычный квадрат со стороной 4
};
//==============================================================================
TEST_F( ComplicatedOperationsTest, geoGeometryToProjectionTest )
{
  GeoMultiPolygon2D geo( { bigSquare } );

  // если центр проекции в центре фигуры - получим симметричный квадрат
  auto flat = geoGeometryToProjection( geo, center, true );
  ASSERT_EQ( flat[0].outer( )[0].getX( ), -flat[0].outer( )[2].getX( ) );
  ASSERT_EQ( flat[0].outer( )[0].getY( ), -flat[0].outer( )[2].getY( ) );
  ASSERT_TRUE( 
    aberration(flat[0].outer( )[0].getX( ), -flat[0].outer( )[0].getY( )) < 0.01 );

  // если сместить центр проекции - квадрат сплющится по оси X (направлена вдоль экватора)
  auto flat2 = geoGeometryToProjection( geo, GeoPoint2D( Degree(0), Degree(75) ), true );
  ASSERT_EQ( flat[0].outer( )[0].getY( ), -flat[0].outer( )[2].getY( ) );
  double shrink = aberration(
    flat2[0].outer( )[0].getX( ) - flat2[0].outer( )[2].getX( ),
    flat[0].outer( )[0].getX( ) - flat[0].outer( )[2].getX( ) );
  ASSERT_TRUE( ( shrink > 0.085 ) && ( shrink < 0.095 ) );

  // дырки можно проецировать
  geo[0].inners( ).push_back( smallSquare.outer( ) );
  auto flat3 = geoGeometryToProjection( geo, center, true );
  ASSERT_EQ( 1, flat3[0].inners( ).size( ) );
  ASSERT_EQ( 5, flat3[0].inners( )[0].size( ) );
  
  // или нет
  auto flat4 = geoGeometryToProjection( geo, center, false );
  ASSERT_EQ( 0, flat4[0].inners( ).size( ) );
}
//==============================================================================
TEST_F( ComplicatedOperationsTest, geoGeometryToCorrectFlatTest )
{
  // берём полигон с дыркой
  GeoMultiPolygon2D geo( { bigSquare } );
  geo[0].inners( ).push_back( smallSquare.outer( ) );

  // в проекции получаем полигон с дыркой, с исправленными направлениями обхода
  MultiPolygon2D flat;
  geoGeometryToCorrectFlat( geo, flat, true );
  // есть полигон и в нем дырка
  ASSERT_EQ( 1, flat.size( ) );
  ASSERT_EQ( 1, flat[0].inners( ).size( ) );
  auto border = flat[0].outer( );
  auto hole = flat[0].inners( )[0];

  // полигон обходится по часовой, а дырка - против (внешнее кольцо будет
  // корректным отдельно, а внутреннее - нет)
  ASSERT_TRUE(AbstractOperations::Helper::hasCorrectOrientation(
                               PrimaryEntities::toGeosGeometry(border).get()));
  ASSERT_FALSE(AbstractOperations::Helper::hasCorrectOrientation(
                                 PrimaryEntities::toGeosGeometry(hole).get()));
  // оба полигона получились симметричными и близкими к квадрату
  ASSERT_EQ( border[0].getX( ), -border[2].getX( ) );
  ASSERT_EQ( border[0].getY( ), -border[2].getY( ) );
  ASSERT_TRUE( aberration( border[0].getX( ), border[0].getY( ) ) < 0.01 );
  ASSERT_EQ( hole[0].getX( ), -hole[2].getX( ) );
  ASSERT_EQ( hole[0].getY( ), -hole[2].getY( ) );
  ASSERT_TRUE( aberration( hole[0].getX( ), hole[0].getY( ) ) < 0.01 );
}
//==============================================================================
} //! Tests
} //! Geodesic
//==============================================================================