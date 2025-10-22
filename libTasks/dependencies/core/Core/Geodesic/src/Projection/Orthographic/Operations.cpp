#include <Geodesic/Projection/Orthographic/Operations.hpp>
//! MathTools
#include <MathTools/BasicMath/BaseCompare.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Union.hpp>
#include <AbstractOperations/Algorithms/Difference.hpp>
#include <AbstractOperations/Algorithms/Buffer.hpp>
//==============================================================================
namespace Geodesic
{
using namespace AbstractOperations;
//==============================================================================
MultiPolygon2D geoGeometryToProjection( const GeoMultiPolygon2D& polygons
  , const GeoPoint2D& projCenter, const bool withHoles /*= true*/ )
{
  // создаем проекцию
  // TODO_Core: ортографическая проекция в Core должна научиться жить с 2d 
  //            точками
  OrthographicProjection orthoProjection(
    std::make_shared<Ellipsoid>( EL_WGS_84 )
    , GeoPoint3D( projCenter, 0 ) );

  return geoGeometryToProjection( polygons, 
    std::make_shared<OrthographicProjection>( orthoProjection ), withHoles );
}
//==============================================================================

} // ! Geodesic
