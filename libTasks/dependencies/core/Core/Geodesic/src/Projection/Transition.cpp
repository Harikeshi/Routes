#include <Geodesic/Projection/Transition.hpp>
//! MathTools
#include <MathTools/BasicMath/BaseCompare.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Union.hpp>
#include <AbstractOperations/Algorithms/Difference.hpp>
#include <AbstractOperations/Algorithms/Buffer.hpp>
#include <AbstractOperations/Algorithms/Correct.hpp>
#include <AbstractOperations/DeleteExcessVertices.hpp>
//==============================================================================
namespace Geodesic
{
using namespace AbstractOperations;
//==============================================================================
Point2D geoGeometryToProjection(
  const GeoPoint2D & point, 
  const std::shared_ptr<const AbstractProjection> proj,
  const bool withHoles )
{
  return proj->toProjection( point );
}
//==============================================================================
Box2D geoGeometryToProjection( 
  const GeoBox2D & box, 
  const std::shared_ptr<const AbstractProjection> proj,
  const bool withHoles )
{
  Box2D retVal;
  toProjection( box, retVal, proj );
  return retVal;
}
//==============================================================================
Linestring2D geoGeometryToProjection(
  const GeoLinestring2D & line, 
  const std::shared_ptr<const AbstractProjection> proj,
  const bool withHoles )
{
  Linestring2D retVal;

  toProjection( line, retVal, proj );

  return retVal;
}
//==============================================================================
std::vector<Point2D> geoGeometryToProjection(
  const std::vector<GeoPoint2D>& line, 
  const std::shared_ptr<const AbstractProjection> proj,
  const bool withHoles )
{
  std::vector<Point2D> retVal;

  toProjection( line, retVal, proj );

  return retVal;
}
//==============================================================================
Ring2D geoGeometryToProjection( const GeoRing2D & ring,
  const std::shared_ptr<const AbstractProjection> proj,
  const bool withHoles )
{
  Ring2D retVal;

  toProjection( ring, retVal, proj );
  correct( retVal );

  AbstractOperations::deleteExcessVertices(retVal);

  return retVal;
}
//==============================================================================
/*!
 *\brief Безопасное проецирование полигона в заданной проекции
 *\details Возможен режим работы только с внешними границами полигона, без дырок.
 *         Дополнительно выполняется объединение внутренних колец, с последующим
 *         вычитанием из внешнего контура.
 *         Результат приводится к мультиполигону.
 *\param[in] polygon   - исходный полигон (географические координаты)
 *\param[in] proj      - требуемая проекция
 *\param[in] withHoles - обрабатывать ли дырки?
 *                true - дырки попадают в итоговые полигоны
 *               false - дырки игнорируются
 *\return - спроецированный мультиполигон (на плоскости)
 */
MultiPolygon2D geoGeometryToProjectionSafe( const GeoPolygon2D & polygon,
  const std::shared_ptr<const AbstractProjection> proj,
  const bool withHoles )
{
  MultiPolygon2D retVal;
  MultiPolygon2D holes;

  try
  {
    if (withHoles)
    {
      holes.reserve(polygon.inners().size());

      for (auto hole : polygon.inners())
        holes.emplace_back(
          Polygon2D({ geoGeometryToProjection(hole, proj) }));
      AbstractOperations::correct(holes);
      unionAll(holes);
    }

    difference(geoGeometryToProjection(polygon.outer(), proj), holes, retVal);
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<GeodesicFailure>(
                                                "geoGeometryToProjectionSafe");
  }

  return retVal;
}
//==============================================================================
Polygon2D geoGeometryToProjection( const GeoPolygon2D & polygon,
  const std::shared_ptr<const AbstractProjection> proj,
  const bool withHoles /*= true*/ )
{
  MultiPolygon2D retVal;

  try
  {
    retVal = geoGeometryToProjectionSafe(polygon, proj, withHoles);
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<GeodesicFailure>("geoGeometryToProjection");
  }


  switch( retVal.size( ) )
  {
  case 0: // Полигон был пуст или вырожден. Или стал таким при проецировании
    return Polygon2D( );
  case 1: // При проецировании получился один корректный полигон
  {
    return retVal[0];
  }
  default: // При проецировании дырки разорвали полигон на несколько несвязных 
           // частей
    throw GeodesicFailure( DisConnectedPolygon );
  }
}
//==============================================================================
MultiPoint2D geoGeometryToProjection( const GeoMultiPoint2D & points,
  const std::shared_ptr<const AbstractProjection> proj,
  const bool withHoles /*= true*/ )
{
  MultiPoint2D retVal;

  toProjection( points, retVal, proj );

  return retVal;
}
//==============================================================================
MultiPolygon2D geoGeometryToProjection( const GeoMultiPolygon2D& polygons
  , const std::shared_ptr<const AbstractProjection> proj,
  const bool withHoles /*= true*/ )
{
  MultiPolygon2D retVal; 
  retVal.reserve( polygons.size( ) );

  Polygon2D tmpPolyg;

  try
  {
    for( auto polyg : polygons )
    {
      tmpPolyg = geoGeometryToProjection( polyg, proj, withHoles );
      retVal.emplace_back( tmpPolyg );
    }

    unionAll(retVal);
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<GeodesicFailure>("geoGeometryToProjection");
  }

  return retVal;
}
//==============================================================================
Ring2D geoGeometryToProjectionForce( const GeoRing2D& ring
  , const std::shared_ptr<const AbstractProjection> proj,
  const bool withHoles )
{
  Ring2D retVal;

  toProjection( ring, retVal, proj );

  return retVal;
}
//==============================================================================
Polygon2D geoGeometryToProjectionForce( const GeoPolygon2D& polygon
  , const std::shared_ptr<const AbstractProjection> proj,
  const bool withHoles )
{
  Polygon2D retVal;

  if( withHoles )
  {
    toProjection( polygon, retVal, proj );
  }
  else
  {
    toProjection( polygon.outer( ), retVal.outer( ), proj );
  }

  return retVal;
}
//==============================================================================
MultiPolygon2D geoGeometryToProjectionForce( const GeoMultiPolygon2D& polygons
  , const std::shared_ptr<const AbstractProjection> proj,
  const bool withHoles )
{
  MultiPolygon2D retVal;

  retVal.reserve( polygons.size( ) );

  for( const auto& polyg : polygons )
    retVal.emplace_back( 
      geoGeometryToProjectionForce( polyg, proj, withHoles ) );

  return retVal;
}
//==============================================================================
// Реализация для точки
GeoPoint2D geoGeometryFromProjection(
           const Point2D& point,
           const std::shared_ptr<const AbstractProjection> proj,
           const bool withHoles)
{
  return proj->fromProjection( point );
}
//==============================================================================
// Реализация для бокса
GeoBox2D geoGeometryFromProjection(
         const Box2D& box,
         const std::shared_ptr<const AbstractProjection> proj,
         const bool withHoles)
{
  GeoBox2D retVal;
  fromProjection( box, retVal, proj );
  return retVal;
}
//==============================================================================
// Реализация для линии, заданной как линия
GeoLinestring2D geoGeometryFromProjection(
                const Linestring2D& line,
                const std::shared_ptr<const AbstractProjection> proj,
                const bool withHoles)
{
  GeoLinestring2D retVal;
  fromProjection( line, retVal, proj );
  return retVal;
}
//==============================================================================
// Реализация для линии, заданной как вектор точек
std::vector<GeoPoint2D> geoGeometryFromProjection(
                        const std::vector<Point2D>& line,
                        const std::shared_ptr<const AbstractProjection> proj,
                        const bool withHoles)
{
  std::vector<GeoPoint2D> retVal;
  fromProjection( line, retVal, proj );
  return retVal;
}
//==============================================================================
// Реализация для кольца
GeoRing2D geoGeometryFromProjection(
  const Ring2D& ring,
  const std::shared_ptr<const AbstractProjection> proj,
  const bool withHoles /*= true*/)
{
  GeoRing2D retVal;

  fromProjection(ring, retVal, proj);

  AbstractOperations::deleteExcessVertices(retVal);

  return retVal;
}
//==============================================================================
/*!
 *\brief Безопасное восстановление полигона из заданной проекции
 *\details Возможен режим работы только с внешними границами полигона, без дырок.
 *         Дополнительно выполняется объединение внутренних колец, с последующим
 *         вычитанием из внешнего контура.
 *         Результат приводится к мультиполигону.
 *\param[in] polygon   - исходный полигон (на плоскости)
 *\param[in] proj      - текущая проекция
 *\param[in] withHoles - обрабатывать ли дырки?
 *                true - дырки попадают в итоговые полигоны
 *               false - дырки игнорируются
 *\return - восстановленный мультиполигон (географические координаты)
 */
GeoMultiPolygon2D geoGeometryFromProjectionSafe(
  const Polygon2D& polygon,
  const std::shared_ptr<const AbstractProjection> proj,
  const bool withHoles /*= true*/)
{
  GeoMultiPolygon2D retVal;
  GeoMultiPolygon2D holes;
  
  try
  {
    if (withHoles)
    {
      holes.reserve(polygon.inners().size());

      for (auto hole : polygon.inners())
        holes.emplace_back(
          GeoPolygon2D({ geoGeometryFromProjection(hole, proj) }));
      AbstractOperations::correct(holes);
      unionAll(holes);
    }

    difference(geoGeometryFromProjection(polygon.outer(), proj), holes, retVal);
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<GeodesicFailure>(
                                              "geoGeometryFromProjectionSafe");
  }

  return retVal;
}
//==============================================================================
// Реализация для полигона
GeoPolygon2D geoGeometryFromProjection(
  const Polygon2D& polygon,
  const std::shared_ptr<const AbstractProjection> proj,
  const bool withHoles /*= true*/)
{
  GeoMultiPolygon2D retVal;
  try
  {
    retVal = geoGeometryFromProjectionSafe(polygon, proj, withHoles);
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<GeodesicFailure>(
                                                  "geoGeometryFromProjection");
  }

  switch (retVal.size())
  {
  case 0: // Полигон был пуст или вырожден. Или стал таким при восстановлении
    return GeoPolygon2D();
  case 1: // При восстановлении получился один корректный полигон
  {
    return retVal[0];
  }
  default: // При восстановлении дырки разорвали полигон на несколько несвязных 
           // частей
    throw GeodesicFailure(DisConnectedPolygon);
  };
}
//==============================================================================
// Реализация для мультиполигона
GeoMultiPolygon2D geoGeometryFromProjection(
  const MultiPolygon2D& polygons,
  const std::shared_ptr<const AbstractProjection> proj,
  const bool withHoles /*= true*/)
{
  GeoMultiPolygon2D retVal;
  retVal.reserve(polygons.size());

  GeoPolygon2D tmpPolyg;

  try
  {
    for (auto polyg : polygons)
    {
      tmpPolyg = geoGeometryFromProjection(polyg, proj, withHoles);
      retVal.emplace_back(tmpPolyg);
    }

    unionAll(retVal);
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<GeodesicFailure>(
                                                  "geoGeometryFromProjection");
  }

  return retVal;
}
//==============================================================================
// Реализация для коллекции точек
GeoMultiPoint2D geoGeometryFromProjection(
  const MultiPoint2D& points,
  const std::shared_ptr<const AbstractProjection> proj,
  const bool withHoles /*= true*/)
{
  GeoMultiPoint2D retVal;

  fromProjection(points, retVal, proj);

  return retVal;
}
//==============================================================================
} // ! Geodesic