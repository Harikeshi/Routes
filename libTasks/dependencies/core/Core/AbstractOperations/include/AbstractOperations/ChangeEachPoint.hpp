#ifndef ABSTRACT_OPERATIONS_CHANGE_EACH_POINT_HPP
#define ABSTRACT_OPERATIONS_CHANGE_EACH_POINT_HPP
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
//      Преобразование, не меняющее тип точки
//==============================================================================
/*!
 *\brief Применение некоторого изменения к каждой точке геометрии
 *\details Применяемое изменение не меняет тип точки. 
 *\param[in|out] geometry - изменяемая геометрия
 *\param[in] converter    - функция изменения одной точки. Имеет профиль 
 *                          void ( Point& )
 */
template <typename Geometry, class Converter>
void changeEachPoint( Geometry& geometry, Converter converter )
{
  std::for_each(geometry.begin(), geometry.end(), converter);
}
//==============================================================================
/*!
 *\brief Применение некоторого преобразования к каждой точке из multiLinestring
 *\details Применяемое изменение не меняет тип точки.
 *\param[in|out] geometry - изменяемая мультилиния
 *\param[in] converter    - функция преобразования одной точки. Имеет профиль
 *                          void ( Point& )
 */
template <typename PointType, class Converter>
void changeEachPoint(
  PrimaryEntities::MultiLinestring<PointType>& geometry,
  Converter converter)
{
  for (auto& line : geometry)
    changeEachPoint(line, converter);
}
//==============================================================================
/*!
 *\brief Применение некоторого преобразования к каждой точке полигона
 *\details Применяемое изменение не меняет тип точки.
 *\param[in|out] geometry - изменяемый полигон
 *\param[in] converter    - функция преобразования одной точки. Имеет профиль
 *                          void ( Point& )
 */
template <typename PointType, class Converter>
void changeEachPoint(
  PrimaryEntities::Polygon<PointType>& geometry,
  Converter converter)
{
  changeEachPoint(geometry.outer(), converter);

  for (auto& ring : geometry.inners())
    changeEachPoint(ring, converter);
}
//==============================================================================
/*!
 *\brief Применение некоторого преобразования к каждой точке мультиполигона
 *\details Применяемое изменение не меняет тип точки.
 *\param[in|out] geometry - изменяемый полигон
 *\param[in] converter    - функция преобразования одной точки. Имеет профиль
 *                          void ( Point& )
 */
template <typename PointType, class Converter>
void changeEachPoint(
  PrimaryEntities::MultiPolygon<PointType>& geometry,
  Converter converter)
{
  for (auto& poly : geometry)
    changeEachPoint(poly, converter);
}
//==============================================================================
/*!
 *\brief Применение некоторого изменения к углам box
 *\details Применяемое изменение не меняет тип точки. 
 *\param[in|out] geometry - изменяемый box
 *\param[in] converter    - функция изменения одной точки. Имеет профиль
 *                          void ( Point& )
 */
template <typename Point, class Converter>
void changeEachPoint( PrimaryEntities::Box<Point>& box, Converter converter )
{
  converter( box.min_corner( ) );
  converter( box.max_corner( ) );
}
//==============================================================================
/*!
*\brief Применение некоторого изменения к каждой точке вектора
*\details Применяемое изменение не меняет тип точки.
*\param[in|out] vect  - изменяемый вектор
*\param[in] converter - функция изменения одной точки. Имеет профиль
*                       void ( Point& )
*/
template <typename Point, class Converter>
void changeEachPoint(
  std::vector<Point>& vect, Converter converter )
{
  for ( auto& point : vect )
    converter( point );
}
//==============================================================================
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_CHANGE_EACH_POINT_HPP