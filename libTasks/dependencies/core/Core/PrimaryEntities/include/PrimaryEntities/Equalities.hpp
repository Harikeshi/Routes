#ifndef PRIMARY_ENTITIES_EQUALITIES_HPP
#define PRIMARY_ENTITIES_EQUALITIES_HPP
//==============================================================================
#include <PrimaryEntities/Entities/Box.hpp>
#include <PrimaryEntities/Entities/Linestring.hpp>
#include <PrimaryEntities/Entities/MultiLinestring.hpp>
#include <PrimaryEntities/Entities/MultiPoint.hpp>
#include <PrimaryEntities/Entities/MultiPolygon.hpp>
#include <PrimaryEntities/Entities/Polygon.hpp>
#include <PrimaryEntities/Entities/Ring.hpp>

#include <PrimaryEntities/PrimaryEntitiesExceptions.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//==============================================================================
/*!
 *\brief Сравнение двух box со стандартной погрешностью
 *\param[in] first - первый из сравниваемых box
 *\param[in] second - второй из сравниваемых box
 *\return Равны ли сравниваемые box
 */
template <typename Point>
inline bool operator == (
  const PrimaryEntities::Box<Point>& first, 
  const PrimaryEntities::Box<Point>& second )
{
  return ( ( first.min_corner() == second.min_corner() )
          && ( first.max_corner() == second.max_corner() ) );
}
//==============================================================================
/*!
 *\brief Сравнение двух box со стандартной погрешностью
 *\param[in] first - первый из сравниваемых box
 *\param[in] second - второй из сравниваемых box
 *\return Различаются ли сравниваемые box
 */
template <typename Point>
inline bool operator != (
  const PrimaryEntities::Box<Point>& first,
  const PrimaryEntities::Box<Point>& second )
{
  return !( first == second );
}
//==============================================================================
/*!
 *\brief Сравнение двух полигонов со стандартной погрешностью
 *\param[in] first - первый из сравниваемых полигонов
 *\param[in] second - второй из сравниваемых полигонов
 *\return Равны ли сравниваемые полигоны
 */
template <typename Point>
inline bool operator == (
  const PrimaryEntities::Polygon<Point>& first,
  const PrimaryEntities::Polygon<Point>& second )
{
  if( first.outer( ) != second.outer( ) )
    return false;

  size_t n = first.inners( ).size( );
  if( n != second.inners( ).size( ) )
    return false;

  for( size_t i = 0; i < n; i++ )
    if( first.inners( )[i] != second.inners( )[i] )
      return false;

  return true;
}
//==============================================================================
/*!
 *\brief Сравнение двух полигонов со стандартной погрешностью
 *\param[in] first - первый из сравниваемых полигонов
 *\param[in] second - второй из сравниваемых полигонов
 *\return Различаются ли сравниваемые полигоны
 */
template <typename Point>
inline bool operator != (
  const PrimaryEntities::Polygon<Point>& first,
  const PrimaryEntities::Polygon<Point>& second )
{
  return ! ( first == second );
}
//==============================================================================
/*!
 *\brief Сравнение двух мультиполигонов со стандартной погрешностью
 *\param[in] first - первый из сравниваемых мультиполигонов
 *\param[in] second - второй из сравниваемых мультиполигонов
 *\return Равны ли сравниваемые мультиполигоны
 */
template <typename Point>
inline bool operator == (
  const PrimaryEntities::MultiPolygon<Point> &first,
  const PrimaryEntities::MultiPolygon<Point>&second )
{
  size_t n = first.size( );
  if( n != second.size( ) )
    return false;

  for( size_t i = 0; i < n; i++ )
    if( first[i] != second[i] )
      return false;

  return true;
}
//==============================================================================
/*!
 *\brief Сравнение двух мультиполигонов со стандартной погрешностью
 *\param[in] first - первый из сравниваемых мультиполигонов
 *\param[in] second - второй из сравниваемых мультиполигонов
 *\return Различаются ли сравниваемые мультиполигоны
 */
template <typename Point>
inline bool operator != (
  const PrimaryEntities::MultiPolygon<Point>&first,
  const PrimaryEntities::MultiPolygon<Point>&second )
{
  return ! ( first == second );
}
//==============================================================================
/*!
 *\brief Сравнение двух колец с возможным циклическим сдвигом. 
 *\details Равными считать кольца у которых совпадают вершины и направление,
 * но отсчет может начинаться с разных вершин. 
 * Подразумевается, что начальная точка первого кольца
 * может встретиться во втором только один раз
 *\param[in] firstRing - первое из сравниваемых колец
 *\param[in] secondRing - второе из сравниваемых колец
 *\return Равны ли сравниваемые кольца
 * \throws PrimaryEntitiesFailure # IncorrectRing
 */
template <typename Point>
inline bool areEqualUpToRotate(
  const PrimaryEntities::Ring<Point> &firstRing,
  const PrimaryEntities::Ring<Point> &secondRing)
{
  if (!(firstRing.front() == firstRing.back()) ||
      !(secondRing.front() == secondRing.back()))
    throw PrimaryEntities::PrimaryEntitiesFailure(
          PrimaryEntities::IncorrectRing, ": Ring is not closed");

  if ((firstRing.size() < 4) || (secondRing.size() < 4))
    throw PrimaryEntities::PrimaryEntitiesFailure(
          PrimaryEntities::IncorrectRing, ": The ring is degenerate");
  
  size_t n = firstRing.size();
  if (n != secondRing.size())
    return false;
      
  if (firstRing == secondRing) 
    return true;

  size_t shift = 0;
  if (!(firstRing.front() == secondRing.front())) 
  {
    for (size_t i = 1; i < n - 1; ++i) 
    {
      if (secondRing[i] == firstRing[0])  
      {
        shift = i;
        break;
      }
    }
    if (shift == 0)
      return false;
    
    size_t border = (n - 1) - shift;
    for (size_t j = 0; j < n - shift; ++j)
    {
      if (!(firstRing[j] == secondRing[j + shift]))
        return false;
    }
    for (size_t j = 1; j < shift; ++j)
    {
      if (!(secondRing[j] == firstRing[j + border]))
        return false;                    
    }
    return true;
  }
  return false;
}
//==============================================================================
#endif //! PRIMARY_ENTITIES_EQUALITIES_HPP