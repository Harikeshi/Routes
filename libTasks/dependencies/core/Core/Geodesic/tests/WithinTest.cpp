#include <Geodesic/Operations/Algorithms/Within.hpp>

#include <Geodesic/GeodesicException.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeodesicEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
/* Построить квадрат с центром в данной точке и длиной стороны в градусах (!)
*\param[in] center - центр квадрата (географическая точка)
*\param[in] size - длина стороны квадрата [градусы]
*\return - построенный квадрат
*/
GeoPolygon2D buildGSquare( const GeoPoint2D center, const Degree size )
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
TEST( WithinTest, WithinTest )
{
  // маленький квадратик
  auto small = buildGSquare( GeoPoint2D( ), Degree( 1 ) );
  // большой квадратик
  auto big = buildGSquare( GeoPoint2D( ), Degree( 4 ) );
  // средний квадратик
  auto middle = buildGSquare( GeoPoint2D( ), Degree( 2 ) );
  // касающийся квадратик
  auto shifted = buildGSquare( GeoPoint2D( Degree( 1. ), Degree( 1. ) ), Degree( 2 ) );

  GeoLinestring2D line;
  line.push_back( GeoPoint2D( ) );
  line.push_back( GeoPoint2D( Degree( 1 ), Degree( 1 ) ) );

  // не полностью входит
  ASSERT_FALSE( withinInProj( line, small ) );
  ASSERT_FALSE( withinInProj( line, small, false ) );
  // полностью входит
  ASSERT_TRUE( withinInProj( line, big ) );
  ASSERT_TRUE( withinInProj( line, big, false ) );
  // касание допустимо
  ASSERT_TRUE( withinInProj( line, middle ) );
  ASSERT_TRUE( withinInProj( line, middle, false ) );
  // касания несоразмерных геометрий обрабатываются по-разному
  GeoLinestring2D line_( { middle.outer( )[0], middle.outer( )[1] } );
  ASSERT_FALSE( withinInProj( line_, middle, false ) );
  ASSERT_TRUE( withinInProj( line_, middle, true ) );
  ASSERT_FALSE( withinInProj( line.at(0), line, false ) );
  ASSERT_TRUE( withinInProj( line.at(0), line, true ) );

  // полноценные геометрии тоже могут включаться
  ASSERT_TRUE( withinInProj( small, middle ) );
  ASSERT_TRUE( withinInProj( small, middle, false ) );
  // и не включаться
  ASSERT_FALSE( withinInProj( big, middle ) );
  ASSERT_FALSE( withinInProj( big, middle, false ) );
  // при касании однотипных геометрий флаг не важен
  ASSERT_TRUE( withinInProj( big, big ) ); 
  ASSERT_TRUE( withinInProj( big, big, false ) ); 

  // при проецировании геометрия может слегка расползтись, от этого спасет надувательство
  ASSERT_FALSE( withinInProj( shifted, big ) ); // квадрат должен касаться, но слегка уплыл
  ASSERT_TRUE( withinInProj( shifted, big, true, 100 ) );// буфер всех спас

  // для кривых геометрий из глубин поднимаются исключения
  middle.outer( ).push_back( GeoPoint2D( Degree( 0 ), Degree( 0 ) ) );
  middle.outer( ).push_back( GeoPoint2D( Degree( -1 ), Degree( 1 ) ) );
  ASSERT_THROW( withinInProj( line, middle ), GeodesicFailure );
  try
  {
    withinInProj( line, middle );
  }
  catch( const GeodesicFailure& exception )
  {
    ASSERT_EQ( ExceptionTools::TransitFailure, exception.getCode( ) );
    ASSERT_EQ( geodesicLibTitle, exception.getLib( ) );
    ASSERT_FALSE( std::string( exception.what( ) ).find( "withinInProj" )
      == std::string::npos );
  }
  ASSERT_THROW( withinInProj( line, middle, false ), GeodesicFailure );
  try
  {
    withinInProj( line, middle, false );
  }
  catch( const GeodesicFailure& exception )
  {
    ASSERT_EQ( ExceptionTools::TransitFailure, exception.getCode( ) );
    ASSERT_EQ( geodesicLibTitle, exception.getLib( ) );
    ASSERT_FALSE( std::string( exception.what( ) ).find( "withinInProj" )
      == std::string::npos );
  }
}
//==============================================================================
} //! Tests
} //! Geodesic
//==============================================================================