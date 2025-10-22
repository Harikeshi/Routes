#ifndef GEOMETRY_OPERATIONS_DISTANCE_TO_LINESTRING_HPP
#define GEOMETRY_OPERATIONS_DISTANCE_TO_LINESTRING_HPP
//==============================================================================
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
/*!
 * \brief distanceBetweenPointAndLinestring - поиск расстояния от точки до
 *                                            ломаной линии.
 *
 * \param[in] point      - точка, расстояние от которой ищется;
 *
 * \param[in] linestring - ломаная линия, до которой ищется расстояние.
 *
 * \return Расстояние от точки до ломаной [м].
 *
 * \throws GeometryFailure # EmptyLinestring
 */
double distanceBetweenPointAndLinestring(
  const Point2D& point, const Linestring2D& linestring);
//==============================================================================
} //! Geometry
//==============================================================================
#endif //! GEOMETRY_OPERATIONS_DISTANCE_TO_LINESTRING_HPP