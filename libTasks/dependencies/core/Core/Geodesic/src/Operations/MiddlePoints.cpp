#include <Geodesic/Operations/MiddlePoints.hpp>

#include <Geodesic/GeodesicCartesianTransition/Transition.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Envelope.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//==============================================================================
namespace Geodesic 
{
//==============================================================================
GeoPoint2D 
cartesianMiddlePoint( const GeoMultiPoint2D& points, const Ellipsoid& el )
{
  if( points.size( ) == 0 )
    throw GeodesicFailure( GeodesicEnumFailure::NullGeometryIncorrectOperation );

  if( points.size( ) == 1 )
    return points[0];

  Transition transition( std::make_shared<Ellipsoid>( el ) );

  MultiPoint3D pointsInCartesian;
  pointsInCartesian.reserve( points.size( ) );

  for( const GeoPoint2D& geoPoint : points )
    pointsInCartesian.emplace_back(
      transition.fromGeodesicToCartesian( GeoPoint3D( geoPoint, 0. ) ) );

  Box3D box3D(pointsInCartesian.at(0), pointsInCartesian.at(0));

  for (size_t i = 1; i < pointsInCartesian.size(); ++i)
    box3D.expandToPoint(pointsInCartesian.at(i));

  Point3D boxCenter(
    ( box3D.min_corner( ).getX( ) + box3D.max_corner( ).getX( ) ) / 2.,
    ( box3D.min_corner( ).getY( ) + box3D.max_corner( ).getY( ) ) / 2.,
    ( box3D.min_corner( ).getZ( ) + box3D.max_corner( ).getZ( ) ) / 2. );

  GeoPoint3D geoBoxCenter( transition.fromCartesianToGeodesic( boxCenter ) );

  return GeoPoint2D( 
    geoBoxCenter.getLatitude( ), geoBoxCenter.getLongitude( ) );
}
//==============================================================================
} //! Geodesic
//==============================================================================