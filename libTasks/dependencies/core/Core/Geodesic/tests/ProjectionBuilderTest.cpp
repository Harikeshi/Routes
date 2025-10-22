#include <Geodesic/Projection/Orthographic/ProjectionBuilder.hpp>

#include <Geodesic/Helper/Ellipsoid.hpp>
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
GeoPolygon2D buildGeoSquare( const GeoPoint2D center, const Degree size )
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
TEST( ProjectionBuilderTest, findOptimalProjTest )
{
  GeoPoint2D center( Degree( 0 ), Degree( 100 ) );
  GeoMultiPolygon2D geo( { buildGeoSquare( center, Degree( 4 ) ) } );

  // у обычного квадрата центр в центре
  auto p = findOptimalProjCenter( geo );
  ASSERT_EQ( center, p );

  // у настоящего мультиполигона центр тоже нормальный
  geo.push_back( 
    buildGeoSquare( GeoPoint2D(Degree(0), Degree(106) ), Degree( 4 ) ) );
  p = findOptimalProjCenter( geo );
  GeoPoint2D res( Degree( 0 ), Degree( 103 ) );
  ASSERT_EQ( res, p );

  // оптимальная проекция - та, у которой центр в оптимальном месте
  auto proj = findOptimalProjection( geo );
  ASSERT_EQ( res, proj.getCenterProjection().get().first );
}
//==============================================================================
} //! Tests
} //! Geodesic
//==============================================================================