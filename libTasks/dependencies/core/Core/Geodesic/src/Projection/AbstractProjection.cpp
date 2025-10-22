#include <Geodesic/Projection/AbstractProjection.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
AbstractProjection::AbstractProjection( const Geodesic::Ellipsoid& ellipsoid )
  : mEllipsoid( ellipsoid )
{
}
//==============================================================================
} //! Geodesic
