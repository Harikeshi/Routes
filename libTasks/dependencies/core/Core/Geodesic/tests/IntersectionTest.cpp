#include <Geodesic/Operations/Algorithms/Intersection.hpp>

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
GeoPolygon2D buildGeodSquare( const GeoPoint2D center, const Degree size )
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
TEST( IntersectionsTest, HasIntersectionTest )
{
  auto sq0 = buildGeodSquare( GeoPoint2D( ), Degree( 2 ) );
  auto sq1 = buildGeodSquare( GeoPoint2D( Degree( 1 ), Degree( 1 ) ), Degree( 2 ) );
  auto sq3 = buildGeodSquare( GeoPoint2D( Degree( 3 ), Degree( 3 ) ), Degree( 2 ) );

  GeoLinestring2D line;
  line.push_back( GeoPoint2D( ) );
  line.push_back( GeoPoint2D( Degree( 1 ), Degree( 1 ) ) );

  // полноценное пересечение
  ASSERT_TRUE( hasIntersectionInProj( line, sq0 ) );
  // касание - тоже пересечение
  ASSERT_TRUE( hasIntersectionInProj( line, sq1 ) );
  // нет пересечения
  ASSERT_FALSE( hasIntersectionInProj( line, sq3 ) );

  // полноценные геометрии тоже могут пересекаться
  ASSERT_TRUE( hasIntersectionInProj( sq0, sq1 ) );
  // и не пересекаться
  ASSERT_FALSE( hasIntersectionInProj( sq0, sq3 ) );

  // для кривых геометрий из глубин поднимаются исключения
  sq0.outer( ).push_back( GeoPoint2D(Degree( 0 ), Degree( 0 ) ));
  sq0.outer( ).push_back( GeoPoint2D( Degree( -1 ), Degree( 1 ) ) );
  ASSERT_THROW( hasIntersectionInProj( line, sq0 ), GeodesicFailure );
  try
  {
    hasIntersectionInProj( line, sq0 );
  }
  catch( const GeodesicFailure& exception )
  {
    ASSERT_EQ( ExceptionTools::TransitFailure, exception.getCode( ) );
    ASSERT_EQ( geodesicLibTitle, exception.getLib( ) );
    ASSERT_FALSE( std::string( exception.what( ) ).find( "hasInteresectionInProj" ) 
      == std::string::npos );
  }
}
//==============================================================================
} //! Tests
} //! Geodesic
//==============================================================================