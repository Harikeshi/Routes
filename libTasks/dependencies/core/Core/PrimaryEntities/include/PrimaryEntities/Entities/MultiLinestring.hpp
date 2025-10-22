#ifndef PRIMARY_ENTITIES_MULTILINESTRING_HPP
#define PRIMARY_ENTITIES_MULTILINESTRING_HPP
//==============================================================================
#include "Linestring.hpp"
//! Geos
#include <geos/geom/GeometryFactory.h>
//==============================================================================
namespace PrimaryEntities
{
//==============================================================================
/*!
 *\brief Мультилиния.
 */
template <typename Point>
class MultiLinestring
  : public std::vector<Linestring<Point>, std::allocator<Linestring<Point>>>
{
//==============================================================================
  typedef std::vector<Linestring<Point>, std::allocator<Linestring<Point>>>
                                                                   vector_type;
//==============================================================================
public:
//==============================================================================
  /*!
   *\brief Конструктор класса
   */
  MultiLinestring()
    : vector_type()
  {}
//==============================================================================
  /*!
   *\brief Конструктор класса
   *\param[in] begin - указатель на начало линии
   *\param[in] end   - указатель на конец линии
   */
  template <typename Iterator>
  inline MultiLinestring(Iterator begin, Iterator end)
    : vector_type(begin, end)
  {}
//==============================================================================
  /*!
   *\brief Конструктор класса
   *\param[in] multiLinestring - список инициализации
   */
  inline MultiLinestring(
    std::initializer_list<Linestring<Point>> multiLinestring)
    : vector_type(multiLinestring.begin(), multiLinestring.end())
  {}
//==============================================================================
};
//==============================================================================
} // ! PrimaryEntities
//==============================================================================
#endif // ! PRIMARY_ENTITIES_MULTILINESTRING_HPP