#ifndef GEODESIC_OPERATIONS_MIDDLE_POINTS_HPP
#define GEODESIC_OPERATIONS_MIDDLE_POINTS_HPP
//==============================================================================
#include <Geodesic/GeodesicException.hpp>

#include <Geodesic/Helper/Ellipsoid.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeodesicEntities.hpp>
//==============================================================================
namespace Geodesic 
{
//==============================================================================
 /*!
  *\brief Центр масс минимального описанного параллелепипеда
  *\details Строится содержащий все поданные точки прямоугольный параллелепипед,
  *         стороны которого параллельны координатным осям декартовой 
  *         геоцентрической системы координат
  *\param[in] points - точки, для которых строится параллелепипед
  *\return искомый центр параллелепипеда
  *        ( в исходной географической системе координат )
  *\throws GeodesicFailure # NullGeometryIncorrectOperation
  */
  GeoPoint2D cartesianMiddlePoint(const GeoMultiPoint2D& points, 
    const Ellipsoid& el = Ellipsoid ( EL_WGS_84 ) );
//==============================================================================
} //! Geodesic
//==============================================================================
#endif //! GEODESIC_OPERATIONS_MIDDLE_POINTS_HPP