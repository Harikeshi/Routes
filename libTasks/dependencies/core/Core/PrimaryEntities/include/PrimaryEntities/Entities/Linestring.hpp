#ifndef PRIMARY_ENTITIES_LINESTRING_HPP
#define PRIMARY_ENTITIES_LINESTRING_HPP
//==============================================================================
#include <PrimaryEntities/PrimaryEntitiesExceptions.hpp>
//! Geos
#include <geos/geom/GeometryFactory.h>
//==============================================================================
namespace PrimaryEntities
{
//==============================================================================
/*!
 *\brief Линия.
 */
template <typename Point>
class Linestring : public std::vector<Point, std::allocator<Point>>
{
//==============================================================================
  typedef std::vector<Point, std::allocator<Point>> vector_type;
//==============================================================================
public:
//==============================================================================
  /*!
   *\brief Конструктор класса
   */
  Linestring()
    : vector_type()
  {}
//==============================================================================
  /*!
   *\brief Конструктор класса
   *\param[in] begin - указатель на начало линии
   *\param[in] end   - указатель на конец линии
   */
  template <typename Iterator>
  inline Linestring(Iterator begin, Iterator end)
    : vector_type(begin, end)
  {}
//==============================================================================
  /*!
   *\brief Конструктор класса
   *\param[in] linestring - список инициализации
   */
  inline Linestring(std::initializer_list<Point> linestring)
    : vector_type(linestring.begin(), linestring.end())
  {}
//==============================================================================
};
//==============================================================================
} // ! PrimaryEntities
//==============================================================================
#endif // ! PRIMARY_ENTITIES_LINESTRING_HPP