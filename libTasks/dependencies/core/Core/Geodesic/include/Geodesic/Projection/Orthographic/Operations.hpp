#ifndef GEODESIC_PROJECTION_ORTHOGRAPHIC_OPERATIONS_HPP
#define GEODESIC_PROJECTION_ORTHOGRAPHIC_OPERATIONS_HPP
//==============================================================================
#include <Geodesic/Projection/Orthographic/Orthographic.hpp>
#include <Geodesic/Projection/Orthographic/ProjectionBuilder.hpp>
#include <Geodesic/Projection/Transition.hpp>
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
/*!
 *\brief Проецирование заданной геометрии в оптимальной для неё проекции
 *\details Опцимальная проекция - с центром в центре масс описанного
 *         параллелепипеда. Дырки могут не проецироваться.
 *\param[in] geoGeometry  - исходная геометрия (в географических координатах)
 *\param[out] flatGeometry - спроецированная геометрия (на плоскости)
 *\param[in] withHoles - обрабатывать ли дырки?
 *                true - дырки попадают в итоговые полигоны
 *               false - дырки игнорируются
 */
template <class GeoGeometry, class FlatGeometry>
void geoGeometryToOptimalProjection( const GeoGeometry& geoGeometry
  , FlatGeometry& flatGeometry, const bool withHoles = true )
{
  OrthographicProjection proj = findOptimalProjection( geoGeometry );

  flatGeometry = geoGeometryToProjection( geoGeometry, 
    std::make_shared<OrthographicProjection>( proj ), withHoles );
}
//==============================================================================
/*!
 *\brief Проецирование и причесывание заданной геометрии 
 *\details Спрецировать в оптимальную проекцию, удалить повторяющиеся точки,
 *         поправить направления, объединить
 *\param[in] geoGeometry  - исходная геометрия (в географических координатах)
 *\param[out] flatGeometry - спроецированная и причесанная геометрия (на плоскости)
 *\param[in] withHoles - обрабатывать ли дырки
 *                true - дырки участвуют в выборе центра проекции и потом
 *                       попадают в итоговые полигоны
 *               false - дырки игнорируются на всех стадиях
 */
template <class GeoGeometry, class FlatGeometry>
void geoGeometryToCorrectFlat( const GeoGeometry& geoGeometry
  , FlatGeometry& flatGeometry, const bool withHoles = true )
{
  try
  {
    // проецируем  
    geoGeometryToOptimalProjection( geoGeometry, flatGeometry, withHoles );

    // причесываем и объединяем (при необходимости)
    if( typeid( flatGeometry ) == typeid( MultiPolygon2D ) )
      AbstractOperations::unionInternal( flatGeometry, true );
    else
      AbstractOperations::correct( flatGeometry );

    return;
  }
  catch( ... )
  {
    ExceptionTools::FailureHandler<GeodesicFailure>
      ( "<GeoPolyTools::geoPolygToCorrectFlat> " );
  }
}
//==============================================================================
/*!
 *\brief Проецирование мультиполигона в заданной проекции
 *\details Возможен режим работы только с внешними границами полигонов, 
 *         без дырок.
 *         Последовательность полигонов в результирующем мультиполигоне 
 *         может отличаться от исходной!
 *\param[in] polygons - исходный мультиполигон (географические координаты)
 *\param[in] projCenter - центр проекции (географическая точка)
 *\param[in] withHoles - обрабатывать ли дырки?
 *                true - дырки попадают в итоговые полигоны
 *               false - дырки игнорируются
 *\return - спроецированный мультиполигон (на плоскости)
 */
MultiPolygon2D geoGeometryToProjection( const GeoMultiPolygon2D& polygons
  , const GeoPoint2D& projCenter, const bool withHoles = true );
//==============================================================================
} // ! Geodesic
//==============================================================================
#endif // ! GEODESIC_PROJECTION_ORTHOGRAPHIC_OPERATIONS_HPP
