#ifndef GEODESIC_COMPLICATED_OPERATIONS_POINTINPOLYGON_HPP
#define GEODESIC_COMPLICATED_OPERATIONS_POINTINPOLYGON_HPP
//==============================================================================
//! PrimaryEntities
#include <PrimaryEntities/GeodesicEntities.hpp>
//! Geometry
#include <Geometry/ComplicatedOperations/SmartMiddlePointInPolygon.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
/*!
* \brief Получение положения точки в области.
* \details Если выбрано восточное приоритетное направление, поиск
*          точки проводится в направлении против часовой стрелки,
*          во всех остальных случаях - по часовой.
*
* \param[in] geoPolygon - полигон, задающий область;
*
* \param[in] priorityDirection - приоритетное направление поиска точки;
*
* \return Положение точки в области
*/
  GeoPoint2D getPositionInArea(
    const GeoPolygon2D& geoPolygon,
    const Geometry::Direction priorityDirection = Geometry::North);
//==============================================================================
} //! Geodesic
//==============================================================================
#endif //! GEODESIC_COMPLICATED_OPERATIONS_POINTINPOLYGON_HPP