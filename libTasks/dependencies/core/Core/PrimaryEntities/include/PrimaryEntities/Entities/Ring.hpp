#ifndef PRIMARY_ENTITIES_RING_HPP
#define PRIMARY_ENTITIES_RING_HPP
//==============================================================================
#include <PrimaryEntities/PrimaryEntitiesExceptions.hpp>
//! Geos
#include <geos/geom/GeometryFactory.h>
//! std
//==============================================================================
namespace PrimaryEntities
{
//==============================================================================
/*!
 *\brief Кольцо.
 */
template <typename Point>
class Ring : public std::vector<Point, std::allocator<Point>>
{
//==============================================================================
  typedef std::vector<Point, std::allocator<Point>> vector_type;
//==============================================================================
public:
//==============================================================================
  /*!
   *\brief Конструктор класса
   */
  Ring() : vector_type()
  {}
//==============================================================================
  /*!
   *\brief Конструктор класса
   *\param[in] begin - указатель на начало кольца
   *\param[in] end   - указатель на конец кольца
   */
  template <typename Iterator>
  inline Ring(Iterator begin, Iterator end)
    : vector_type(begin, end)
  {}
//==============================================================================
  /*!
   *\brief Конструктор класса
   *\param[in] ring - список инициализации
   */
  inline Ring(const std::initializer_list<Point>& ring)
    : vector_type(ring.begin(), ring.end())
  {}
//==============================================================================
};
//==============================================================================
} // ! PrimaryEntities
//==============================================================================
#endif // ! PRIMARY_ENTITIES_RING_HPP
