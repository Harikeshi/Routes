#ifndef GEODESIC_PROJECTION_ORTHOGRAPHIC_PROJECTION_BUILDER_HPP
#define GEODESIC_PROJECTION_ORTHOGRAPHIC_PROJECTION_BUILDER_HPP
//==============================================================================
#include <Geodesic/Projection/Orthographic/Orthographic.hpp>
#include <Geodesic/Operations/MiddlePoints.hpp>
#include <Geodesic/GeodesicException.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Union.hpp>
#include <AbstractOperations/ExtractPointsFromGeoms.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
// Выбор проекции, оптимальной для данной геометрии
//==============================================================================
/*!
 *\brief Поиск оптимального центра проекции для заданного типа геометрии
 *\details Оптимальная проекция - с центром в центре масс описанного параллелепипеда.
 *\param[in] geometry - исходная геометрия (в географических координатах)
 *\return - центр оптимальной проекции (географическая точка)
 */
template <class GeoGeometry>
GeoPoint2D findOptimalProjCenter( const GeoGeometry& geometry )
{
  GeoPoint2D center;

  //! Вытаскиваем из геометрии ее граничные точки.

  auto points = AbstractOperations::getBorderPoints(geometry);

  //! Ищем центр масс объемлющего параллелепипеда.
  center = cartesianMiddlePoint( points );

  return center;
}
//==============================================================================
/*!
 *\brief Поиск оптимальной проекции для заданного типа геометрии
 *\details Оптимальная проекция - с центром в центре масс описанного параллелепипеда.
 *\param[in] geometry - исходная геометрия (в географических координатах)
 *\return - оптимальная проекция
 */
template <class GeoGeometry>
OrthographicProjection
findOptimalProjection( const GeoGeometry& geometry )
{
  return OrthographicProjection(
    std::make_shared<Ellipsoid>( EL_WGS_84 )
    , GeoPoint3D( findOptimalProjCenter<GeoGeometry>( geometry ), 0 ) );
}
//==============================================================================
} // ! Geodesic
//==============================================================================
#endif // ! GEODESIC_PROJECTION_ORTHOGRAPHIC_PROJECTION_BUILDER_HPP
