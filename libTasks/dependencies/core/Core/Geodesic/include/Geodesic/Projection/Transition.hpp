#ifndef GEODESIC_PROJECTION_TRANSITION_HPP
#define GEODESIC_PROJECTION_TRANSITION_HPP
//==============================================================================
#include <Geodesic/GeodesicException.hpp>
#include <Geodesic/Projection/AbstractProjection.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Union.hpp>
#include <AbstractOperations/ExtractPointsFromGeoms.hpp>
#include <AbstractOperations/ConvertEachPoint.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
//==============================================================================
//   Проецирование различных геометрических примитивов в известную проекцию
//==============================================================================
/*!
 *\brief Проецирование геометрии в заданной проекции
 *\param[in] source - проецируемая геометрия (географические координаты)
 *\param[out] target - полученная геометрия (на плоскости)
 *\param[in] proj - применяемая проекция
 */
template <typename GeoGeometry, typename FlatGeometry>
void toProjection( const GeoGeometry& source, FlatGeometry& target,
  const std::shared_ptr<const AbstractProjection> projection )
{
  AbstractOperations::convertEachPoint( source, target,
    [projection] ( const GeoPoint2D& p ) { return projection->toProjection( p ); } );
}
//==============================================================================
/*!
 *\brief Восстановление геометрии из заданной проекции
 *\param[in] source - восстанавливаемая геометрия (на плоскости)
 *\param[out] target - полученная геометрия (географические координаты)
 *\param[in] proj - применяемая проекция
 */
template <typename GeoGeometry, typename FlatGeometry>
void fromProjection( const FlatGeometry& source, GeoGeometry& target,
  const std::shared_ptr<const AbstractProjection> projection )
{
  AbstractOperations::convertEachPoint( source, target,
    [projection] ( const Point2D& p ) 
    { return projection->fromProjection( p ); } );
}
//==============================================================================
/*!
 *\brief Проецирование точки в заданной проекции
 *\details Добавлено для унификации интерфейса
 *\param[in] point - точка (географические координаты)
 *\param[in] proj - требуемая проекция
 *\param[in] withHoles - фиктивный не используемый параметр, требуется для 
 *                       универсальности интерфейса вызова
 *\return - спроецированная точка (на плоскости)
 */
Point2D geoGeometryToProjection( const GeoPoint2D& point
  , const std::shared_ptr<const AbstractProjection> proj
  , const bool withHoles = true );
//==============================================================================
/*!
 *\brief Проецирование box в заданной проекции
 *\param[in] box - исходная геометрия (географические координаты)
 *\param[in] proj - требуемая проекция
 *\param[in] withHoles - фиктивный не используемый параметр, требуется для 
 *                       универсальности интерфейса вызова
 *\return - спроецированный box (на плоскости)
 */
Box2D geoGeometryToProjection( const GeoBox2D& box
  , const std::shared_ptr<const AbstractProjection> proj
  , const bool withHoles = true );
//==============================================================================
/*!
 *\brief Проецирование линии в заданной проекции
 *\param[in] line - исходная линия (географические координаты)
 *\param[in] proj - требуемая проекция
 *\param[in] withHoles - фиктивный не используемый параметр, требуется для
 *                       универсальности интерфейса вызова
 *\return - спроецированная линия (на плоскости)
 */
Linestring2D geoGeometryToProjection( const GeoLinestring2D& line
  , const std::shared_ptr<const AbstractProjection> proj
  , const bool withHoles = true );
//==============================================================================
/*!
 *\brief Проецирование линии в заданной проекции
 *\param[in] line - исходная линия (географические координаты)
 *\param[in] proj - требуемая проекция
 *\param[in] withHoles - фиктивный не используемый параметр, требуется для
 *                       универсальности интерфейса вызова
 *\return - спроецированная линия (на плоскости)
 */
std::vector<Point2D> geoGeometryToProjection( const std::vector<GeoPoint2D>& line
  , const std::shared_ptr<const AbstractProjection> proj
  , const bool withHoles = true );
//==============================================================================
/*!
 *\brief Проецирование кольца в заданной проекции
 *\details Возможно добавление некоторого (неотрицательного!) буфера к
 *         спроецированному кольцу.
 *\param[in] ring - исходное кольцо (географические координаты)
 *\param[in] proj - требуемая проекция
 *\param[in] withHoles - фиктивный не используемый параметр, требуется для
 *                       универсальности интерфейса вызова
 *\return - спроецированное кольцо (на плоскости)
 */
Ring2D geoGeometryToProjection( const GeoRing2D& ring
  , const std::shared_ptr<const AbstractProjection> proj
  , const bool withHoles = true );
//==============================================================================
/*!
 *\brief Проецирование полигона в заданной проекции
 *\details Возможен режим работы только с внешними границами полигона, без дырок.
 *         Возможно добавление некоторого (неотрицательного!) буфера к
 *         спроецированному полигону.
 *\param[in] polygon - исходный полигон (географические координаты)
 *\param[in] proj - требуемая проекция
 *\param[in] withHoles - обрабатывать ли дырки?
 *                true - дырки попадают в итоговые полигоны
 *               false - дырки игнорируются
 *\return - спроецированный полигон (на плоскости)
 *\throws GeodesicFailure # DisConnectedPolygon
 */
Polygon2D geoGeometryToProjection( const GeoPolygon2D& polygon
  , const std::shared_ptr<const AbstractProjection> proj
  , const bool withHoles = true );
//==============================================================================
/*!
 *\brief Проецирование мультиполигона в заданной проекции
 *\details Возможен режим работы только с внешними границами полигона, без дырок.
 *         Возможно добавление некоторого (неотрицательного!) буфера к 
 *         спроецированным полигонам перед их объединением.
 *\param[in] polygons - исходный мультиполигон (географические координаты)
 *\param[in] proj - требуемая проекция
 *\param[in] withHoles - обрабатывать ли дырки?
 *                true - дырки попадают в итоговые полигоны
 *               false - дырки игнорируются
 *\return - спроецированный мультиполигон (на плоскости)
 */
MultiPolygon2D geoGeometryToProjection( const GeoMultiPolygon2D& polygons
  , const std::shared_ptr<const AbstractProjection> proj
  , const bool withHoles = true );
//==============================================================================
/*!
 *\brief Проецирование множества точек в заданной проекции
 *\param[in] points - исходные точки (географические координаты)
 *\param[in] proj - требуемая проекция
 *\param[in] withHoles - фиктивный не используемый параметр, требуется для
 *                       универсальности интерфейса вызова
 *\return - спроецированные точки (на плоскости)
 */
MultiPoint2D geoGeometryToProjection( const GeoMultiPoint2D & points
  , const std::shared_ptr<const AbstractProjection> proj
  , const bool withHoles = true );
//==============================================================================
/*!
 *\brief Проецирование кольца в заданной проекции
 *\details Кольцо не исправляется, корректность не проверяется
 *\param[in] ring - исходное кольцо (географические координаты)
 *\param[in] proj - требуемая проекция
 *\param[in] withHoles - фиктивный не используемый параметр, требуется для
 *                       универсальности интерфейса вызова
 *\return - спроецированное кольцо (на плоскости)
 */
Ring2D geoGeometryToProjectionForce( const GeoRing2D& ring
  , const std::shared_ptr<const AbstractProjection> proj
  , const bool withHoles = true );
//==============================================================================
/*!
 *\brief Проецирование полигона в заданной проекции
 *\details Возможен режим работы только с внешними границами полигона, без дырок.
 *         Полигон не исправляется, дырки проецируются без объединения.
 *\param[in] polygon - исходный полигон (географические координаты)
 *\param[in] proj - требуемая проекция
 *\param[in] withHoles - обрабатывать ли дырки?
 *                true - дырки попадают в итоговые полигоны
 *               false - дырки игнорируются
 *\return - спроецированный полигон (на плоскости)
 */
Polygon2D geoGeometryToProjectionForce( const GeoPolygon2D& polygon
  , const std::shared_ptr<const AbstractProjection> proj
  , const bool withHoles = true );
//==============================================================================
/*!
 *\brief Проецирование мультиполигона в заданной проекции
 *\details Возможен режим работы только с внешними границами полигона, без дырок.
 *         Полигоны не исправляются, дырки проецируются без объединения.
 *\param[in] polygons - исходный мультиполигон (географические координаты)
 *\param[in] proj - требуемая проекция
 *\param[in] withHoles - обрабатывать ли дырки?
 *                true - дырки попадают в итоговые полигоны
 *               false - дырки игнорируются
 *\return - спроецированный мультиполигон (на плоскости)
 */
MultiPolygon2D geoGeometryToProjectionForce( const GeoMultiPolygon2D& polygons
  , const std::shared_ptr<const AbstractProjection> proj
  , const bool withHoles = true );
//==============================================================================
/*!
 *\brief Восстановление точки из заданной проекции
 *\details Добавлено для унификации интерфейса
 *\param[in] point - точка на плоскости
 *\param[in] proj - текущая проекция
 *\param[in] withHoles - фиктивный не используемый параметр, требуется для
 *                       универсальности интерфейса вызова
 *\return - восстановленная точка (географические координаты)
 */
GeoPoint2D geoGeometryFromProjection(
           const Point2D& point,
           const std::shared_ptr<const AbstractProjection> proj,
           const bool withHoles = true);
//==============================================================================
/*!
*\brief Восстановление box из заданной проекции
*\param[in] box - исходная геометрия на плоскости
*\param[in] proj - текущая проекция
*\param[in] withHoles - фиктивный не используемый параметр, требуется для
*                       универсальности интерфейса вызова
*\return - восстановленный box (географические координаты)
*/
GeoBox2D geoGeometryFromProjection(
         const Box2D& box,
         const std::shared_ptr<const AbstractProjection> proj,
         const bool withHoles = true);
//==============================================================================
/*!
*\brief Восстановление линии из заданной проекции
*\param[in] line - исходная линия на плоскости
*\param[in] proj - текущая проекция
*\param[in] withHoles - фиктивный не используемый параметр, требуется для
*                       универсальности интерфейса вызова
*\return - восстановленная линия (географические координаты)
*/
GeoLinestring2D geoGeometryFromProjection(
                const Linestring2D& line,
                const std::shared_ptr<const AbstractProjection> proj,
                const bool withHoles = true);
//==============================================================================
/*!
*\brief Восстановление линии из заданной проекции
*\param[in] line - исходная линия на плоскости
*\param[in] proj - текущая проекция
*\param[in] withHoles - фиктивный не используемый параметр, требуется для
*                       универсальности интерфейса вызова
*\return - восстановленная линия (географические координаты)
*/
std::vector<GeoPoint2D> geoGeometryFromProjection(
                        const std::vector<Point2D>& line,
                        const std::shared_ptr<const AbstractProjection> proj,
                        const bool withHoles = true);
//==============================================================================
/*!
 *\brief Восстановление кольца из заданной проекции
 *\details После восстановления удаляются точки, находящиеся на одной прямой
 *         (используется deleteExcessVertices)
 *\param[in] ring      - исходное кольцо на плоскости
 *\param[in] proj      - текущая проекция
 *\param[in] withHoles - фиктивный не используемый параметр, требуется для
 *                       универсальности интерфейса вызова
 *\return - восстановленное кольцо (географические координаты)
 */
GeoRing2D geoGeometryFromProjection(
  const Ring2D& ring,
  const std::shared_ptr<const AbstractProjection> proj, 
  const bool withHoles = true);
//==============================================================================
/*!
 *\brief Восстановление полигона из заданной проекции
 *\details Возможен режим работы только с внешними границами полигона без дырок.
 *         После восстановления удаляются точки, находящиеся на одной прямой
 *         (используется deleteExcessVertices)
 *\param[in] polygon   - исходный полигон на плоскости
 *\param[in] proj      - текущая проекция
 *\param[in] withHoles - обрабатывать ли дырки?
 *                true - дырки попадают в итоговые полигоны
 *               false - дырки игнорируются
 *\return - восстановленный полигон (географические координаты)
 *\throws GeodesicFailure # DisConnectedPolygon
 */
GeoPolygon2D geoGeometryFromProjection(
  const Polygon2D& polygon,
  const std::shared_ptr<const AbstractProjection> proj, 
  const bool withHoles = true);
//==============================================================================
/*!
 *\brief Восстановление мультиполигона из заданной проекции
 *\details Возможен режим работы только с внешними границами полигона без дырок.
 *         После восстановления удаляются точки, находящиеся на одной прямой
 *         (используется deleteExcessVertices)
 *\param[in] polygons  - исходный мультиполигон на плоскости
 *\param[in] proj      - текущая проекция
 *\param[in] withHoles - обрабатывать ли дырки?
 *                true - дырки попадают в итоговые полигоны
 *               false - дырки игнорируются
 *\return - восстановленный мультиполигон (географические координаты)
 */
GeoMultiPolygon2D geoGeometryFromProjection(
  const MultiPolygon2D& polygons,
  const std::shared_ptr<const AbstractProjection> proj, 
  const bool withHoles = true);
//==============================================================================
/*!
 *\brief Восстановление множества точек из заданной проекции
 *\param[in] points    - исходные точки (на плоскости)
 *\param[in] proj      - текущая проекция
 *\param[in] withHoles - фиктивный не используемый параметр, требуется для
 *                       универсальности интерфейса вызова
 *\return - восстановленные точки (географические координаты)
 */
GeoMultiPoint2D geoGeometryFromProjection(
  const MultiPoint2D& points,
  const std::shared_ptr<const AbstractProjection> proj, 
  const bool withHoles = true);
//==============================================================================
} // ! Geodesic
//==============================================================================
#endif // ! GEODESIC_PROJECTION_TRANSITION_HPP
