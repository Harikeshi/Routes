#ifndef PRIMARY_ENTITIES_MULTIPOLYGON_HPP
#define PRIMARY_ENTITIES_MULTIPOLYGON_HPP
//==============================================================================
#include "Polygon.hpp"
//! Geos
#include <geos/geom/GeometryFactory.h>
//==============================================================================
namespace PrimaryEntities
{
//==============================================================================
/*!
 *\brief Мультиполигон.
 */
template <typename Point>
class MultiPolygon
  : public std::vector<Polygon<Point>, std::allocator<Polygon<Point>>>
{
//==============================================================================
  typedef std::vector<Polygon<Point>, std::allocator<Polygon<Point>>>
                                                                   vector_type;
//==============================================================================
public:
//==============================================================================
  /*!
   *\brief Конструктор класса
   */
  MultiPolygon() : vector_type()
  {}
//==============================================================================
  /*!
   *\brief Конструктор класса
   *\param[in] begin - указатель на первый полигон мультиполигона
   *\param[in] end   - указатель на последний полигон мультиполигона
   */
  template <typename Iterator>
  inline MultiPolygon(Iterator begin, Iterator end)
    : vector_type(begin, end)
  {}
//==============================================================================
  /*!
   *\brief Конструктор класса
   *\param[in] polygons - список инициализации
   */
  inline MultiPolygon(std::initializer_list<Polygon<Point>> polygons)
    : vector_type(polygons.begin(), polygons.end())
  {}
//==============================================================================
};
//==============================================================================
} // ! PrimaryEntities
//==============================================================================
#endif // ! PRIMARY_ENTITIES_MULTIPOLYGON_HPP