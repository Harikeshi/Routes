#ifndef ABSTRACT_OPERATIONS_CONVERT_EACH_POINT_HPP
#define ABSTRACT_OPERATIONS_CONVERT_EACH_POINT_HPP
//==============================================================================
#include <AbstractOperations/AbstractOperationsException.hpp>
//! PrimaryEntities
#include <PrimaryEntities/Entities/Box.hpp>
#include <PrimaryEntities/Entities/MultiLinestring.hpp>
#include <PrimaryEntities/Entities/MultiPolygon.hpp>
#include <PrimaryEntities/Entities/Polygon.hpp>
//==============================================================================
namespace AbstractOperations 
{
//==============================================================================
//        Преобразование, меняющее тип точки
//==============================================================================
/*!
*\brief Применение некоторого преобразования к каждой точке геометрии, подобной
*       вектору (вектор, линия, кольцо, мультиточка)
*\details Подразумевается, что преобразование переводит точку в другой тип.
*         Для поточечной операции без смены типа следует использовать 
 *         changeEachPoint
*\param[in] source    - исходный мультиполигон
*\param[out] target   - мультиполигон, полученный в результате преобразования
*\param[in] converter - функция преобразования одной точки. Имеет профиль
*                       TargetPoint ( const SourcePoint& )
*/
template <typename SourceGeometry, typename TargetGeometry, class Converter>
void convertEachPoint(
  const SourceGeometry& source,
  TargetGeometry& target,
  Converter converter )
{
  target.clear( );
  target.reserve( source.size( ) );
  for( const auto& point : source )
    target.emplace_back( converter( point ) );
}
//==============================================================================
/*!
 *\brief Применение некоторого преобразования к каждой точке из multiLinestring
 *\details Подразумевается, что преобразование переводит точку в другой тип.
 *         Для поточечной операции без смены типа следует использовать 
 *         changeEachPoint
 *\param[in] source    - исходная мультилиния
 *\param[out] target   - мультилиния, полученная в результате преобразования
 *\param[in] converter - функция преобразования одной точки. Имеет профиль 
 *                       TargetPoint ( const SourcePoint& )
 */
template <typename SourcePoint, typename TargetPoint, class Converter>
void convertEachPoint(
  const PrimaryEntities::MultiLinestring<SourcePoint> &source,
   PrimaryEntities::MultiLinestring<TargetPoint> &target,
  Converter converter )
{
  target.clear( );
  target.reserve( source.size( ) );
    PrimaryEntities::Linestring<TargetPoint> tmpLine;
    for( const auto& line : source )
  {
    convertEachPoint( line, tmpLine, converter );
    target.emplace_back( tmpLine );
  }
}
//==============================================================================
/*!
 *\brief Применение некоторого преобразования к каждой точке полигона
 *\details Подразумевается, что преобразование переводит точку в другой тип.
 *         Для поточечной операции без смены типа следует использовать 
 *         changeEachPoint
 *\param[in] source    - исходный полигон
 *\param[out] target   - полигон, полученный в результате преобразования
 *\param[in] converter - функция преобразования одной точки. Имеет профиль 
 *                       TargetPoint ( const SourcePoint& )
 */
template <typename SourcePoint, typename TargetPoint, class Converter>
void convertEachPoint(
  const PrimaryEntities::Polygon<SourcePoint>& source,
  PrimaryEntities::Polygon<TargetPoint>& target,
  Converter converter )
{
  target.clear( );
  convertEachPoint( source.outer( ), target.outer( ), converter );

  PrimaryEntities::Ring<TargetPoint> tmpRing;
  target.inners( ).reserve( source.inners( ).size( ) );
  for( const auto& ring : source.inners( ) )
  {
    tmpRing.clear( );
    convertEachPoint( ring, tmpRing, converter );
    target.inners().emplace_back( tmpRing );
  }
}
//==============================================================================
/*!
 *\brief Применение некоторого преобразования к каждой точке мультиполигона
 *\details Подразумевается, что преобразование переводит точку в другой тип.
 *         Для поточечной операции без смены типа следует использовать 
 *         changeEachPoint
 *\param[in] source    - исходный мультиполигон
 *\param[out] target   - мультиполигон, полученный в результате преобразования
 *\param[in] converter - функция преобразования одной точки. Имеет профиль 
 *                       TargetPoint ( const SourcePoint& )
 */
template <typename SourcePoint, typename TargetPoint, class Converter>
void convertEachPoint(
  const PrimaryEntities::MultiPolygon<SourcePoint>& source,
  PrimaryEntities::MultiPolygon<TargetPoint>& target,
  Converter converter )
{
  target.clear( );
  target.reserve( source.size( ) );
  PrimaryEntities::Polygon<TargetPoint> tmpPoly;
  for( const auto& poly : source )
  {
    convertEachPoint( poly, tmpPoly, converter );
    target.emplace_back( tmpPoly );
  }
}
//==============================================================================
/*!
 *\brief Применение некоторого преобразования к углам box
 *\details Подразумевается, что преобразование переводит точку в другой тип.
 *         Для поточечной операции без смены типа следует использовать 
 *         changeEachPoint
 *\param[in] source    - исходный box
 *\param[out] target   - box, полученный в результате преобразования
 *\param[in] converter - функция преобразования одной точки. Имеет профиль 
 *                       TargetPoint ( const SourcePoint& )
 */
template <typename SourcePoint, typename TargetPoint, class Converter>
void convertEachPoint(
  const PrimaryEntities::Box<SourcePoint>& source,
  PrimaryEntities::Box<TargetPoint>& target,
  Converter converter )
{
  target.min_corner( ) = converter( source.min_corner( ) );
  target.max_corner( ) = converter( source.max_corner( ) );
}
//==============================================================================
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_CONVERT_EACH_POINT_HPP