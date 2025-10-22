#include <Geodesic/Operations/AtPoints2D.hpp>
//! Geodesic Helpers
#include <Geodesic/GeoLineTools/OrthodromyTools.hpp>
//! Exceptions
#include <Geodesic/GeodesicException.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
double distance( const GeoPoint2D& first, const GeoPoint2D& second,
  const Ellipsoid& ellipsoid )
{
  if( first == second )
    return 0;

  OrthodromyTools orthodromyTools ( ellipsoid );
  return orthodromyTools.distance( first, second );
}
//==============================================================================
} //! Geodesic
//==============================================================================
