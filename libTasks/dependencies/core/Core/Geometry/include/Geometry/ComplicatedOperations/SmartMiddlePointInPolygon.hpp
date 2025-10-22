#ifndef GEOMETRY_OPERATIONS_SMART_MIDDLE_POINT_IN_POLYGON_HPP
#define GEOMETRY_OPERATIONS_SMART_MIDDLE_POINT_IN_POLYGON_HPP
//==============================================================================
#include <Geometry/Operations/PointInPolygon.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
enum Direction
{
  North = 0,
  East = 1,
  South = 2,
  West = 3
};
//==============================================================================
/*!
 * \brief setPointInPolygon - функция, выбирающая положение точки
 * \                          в полигоне.
 * \details Если выбрано восточное приоритетное направление, поиск
 *          точки проводится в направлении против часовой стрелки, 
 *          во всех остальных случаях - по часовой
 *
 * \param[in] polygon           - полигон, в котором размещается точка;
 *
 * \param[in] priorityDirection - приоритетное направление поиска точки;
 *
 * \return Выбранная точка.
 *
 * \throws GeometryFailure # EmptyPolygon
 *
 */
Point2D setSmartMiddlePoint(
    const Polygon2D& polygon, const Direction priorityDirection = North);
//==============================================================================
} //! Geometry
//==============================================================================
#endif //! GEOMETRY_OPERATIONS_SMART_MIDDLE_POINT_IN_POLYGON_HPP