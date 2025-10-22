#ifndef GEODESIC_OPERATIONS_AT_POINTS2D_HPP
#define GEODESIC_OPERATIONS_AT_POINTS2D_HPP
//==============================================================================
#include <Geodesic/Helper/Ellipsoid.hpp>
//! MathTools
#include <MathTools/AngularUnits/Radian.hpp>
//! Point
#include <Point/GeoPoint2D.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
/*!
 *\brief расстояние между точками на эллипсоиде (по ортодромии)
 *\param[in] first - первая точка
 *\param[in] second - вторая точка
 *\param[in] ellipsoid - эллипсоид, на котором решается задача
 *\return расстояние между двумя точками [м]
*/
double distance( const GeoPoint2D& first, const GeoPoint2D& second,
  const Ellipsoid& ellipsoid = Ellipsoid( EL_WGS_84 ) );
//==============================================================================
} //! Geodesic
//==============================================================================
#endif //! GEODESIC_OPERATIONS_AT_POINTS2D_HPP