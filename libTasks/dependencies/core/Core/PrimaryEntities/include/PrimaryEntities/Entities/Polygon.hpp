#ifndef PRIMARY_ENTITIES_POLYGON_HPP
#define PRIMARY_ENTITIES_POLYGON_HPP
//==============================================================================
#include <PrimaryEntities/Entities/Ring.hpp>
//! Geos
#include <geos/geom/GeometryFactory.h>
//! std
//==============================================================================
namespace PrimaryEntities
{
//==============================================================================
/*!
 *\brief Полигон.
 */
template <typename Point>
class Polygon
{
//==============================================================================
public:
//==============================================================================
  /*!
   *\brief Конструктор класса
   */
  Polygon() : mOuterRing(), mInnerRings()
  {}
//==============================================================================
  /*!
   *\brief Конструктор класса
   *\param[in] linestring - список инициализации
   *                        (набор колец, первое кольцо - внешнее,
   *                         все остальные - внутренние)
   */
  inline Polygon(std::initializer_list<Ring<Point>> rings)
    : mOuterRing(rings.size() > 0 ? *rings.begin() : Ring<Point>())
    , mInnerRings(rings.size() > 0 ? rings.begin() + 1
                                   : rings.begin(), rings.end())
  {}
//==============================================================================
  /*!
   *\brief Получение внешнего кольца по константной ссылке
   *\return Внешнее кольцо
   */
  inline Ring<Point> const& outer() const
  {
    return mOuterRing;
  }
//==============================================================================
  /*!
   *\brief Получение внутренних колец по константной ссылке
   *\return Внутренние кольца
   */
  inline std::vector<Ring<Point>>
  const& inners() const
  {
    return mInnerRings;
  }
//==============================================================================
  /*!
   *\brief Получение внешнего кольца по ссылке
   *\return Внешнее кольцо
   */
  inline Ring<Point>& outer()
  {
    return mOuterRing;
  }
//==============================================================================
  /*!
   *\brief Получение внутренних колец по ссылке
   *\return Внутренние кольца
   */
  inline std::vector<Ring<Point>>& inners()
  {
    return mInnerRings;
  }
//==============================================================================
  /*!
   *\brief Очищение данных полигона
   */
  inline void clear()
  {
    mOuterRing.clear();
    mInnerRings.clear();
  }
  private:
//==============================================================================
  Ring<Point> mOuterRing;
  std::vector<Ring<Point>> mInnerRings;
//==============================================================================
};
//==============================================================================
} // ! PrimaryEntities
//==============================================================================
#endif // ! PRIMARY_ENTITIES_POLYGON_HPP
