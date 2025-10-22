#ifndef PRIMARY_ENTITIES_MULTIPOINT_HPP
#define PRIMARY_ENTITIES_MULTIPOINT_HPP
//==============================================================================
//! Geos
#include <geos/geom/GeometryFactory.h>
//==============================================================================
namespace PrimaryEntities
{
//==============================================================================
/*!
 *\brief Набор точек.
 */
template <typename Point>
class MultiPoint : public std::vector<Point, std::allocator<Point>>
{
//==============================================================================
  typedef std::vector<Point, std::allocator<Point>> vector_type;
//==============================================================================
public:
//==============================================================================
  /*!
   *\brief Конструктор класса
   */
  MultiPoint() : vector_type()
  {}
//==============================================================================
  /*!
   *\brief Конструктор класса
   *\param[in] begin - указатель на начало набора точек
   *\param[in] end   - указатель на конец набора точек
   */
  template <typename Iterator>
  inline MultiPoint(Iterator begin, Iterator end)
    : vector_type(begin, end)
  {}
//==============================================================================
    /*!
   *\brief Конструктор класса
   *\param[in] multiPoint - список инициализации
   */
  inline MultiPoint(std::initializer_list<Point> multiPoint)
    : vector_type(multiPoint.begin(), multiPoint.end())
  {}
//==============================================================================
};
//==============================================================================
} // ! PrimaryEntities
//==============================================================================
#endif // ! PRIMARY_ENTITIES_MULTIPOINT_HPP