#ifndef PRIMARY_ENTITIES_POLYGON_HPP
#define PRIMARY_ENTITIES_POLYGON_HPP
//==============================================================================
#include <PrimaryEntities/Entities/Intersector.hpp>
#include <PrimaryEntities/Entities/Ring.hpp>
//! Geos
#include <geos/geom/GeometryFactory.h>
//! std
#include <optional>
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
//==============================================================================
  /*!
   * \brief intersectionsAll
   * \details Поиск всех точек пересечения с полигоном
   * \param line [in] линия пересечения ринга
   * \return [out] Вектор точек пересечения полигона
   */
  std::vector<RingIntersection> intersectionsAll(const Line& line) const
  {
      std::vector<RingIntersection> result;
      std::vector<Intersection> vectorOuters = mOuterRing.intersectionsAll(line).intersections;
      if (!vectorOuters.empty())
      {
          RingIntersection outerIntersections(vectorOuters, 0);
          result.push_back(outerIntersections);
      }
      if (!mInnerRings.empty())
      {
          for (size_t i = 0; i < mInnerRings.size(); i++)
          {
              std::vector<Intersection> vectorInners = mInnerRings[i].intersectionsAll(line).intersections;
              if (!vectorInners.empty())
              {
                  RingIntersection innerIntersections(vectorInners, i + 1);
                  result.push_back(innerIntersections);
              }
          }
      }
      return result;
  }
//==============================================================================
  /*!
   * \brief nearestIntersectionInDirection
   * \details Расчет ближайшего пересечения со стороной региона по заданному направлению
   * \param start [in] Точка относительно которой ищем пересечение
   * \param direction [in] Направление поика
   * \return [out] Точка пересечения в пространстве и на регионе
   */
  std::optional<RingIntersection> nearestIntersectionAhead(const Point2D& start, const Radian& direction, const double& TRAVERSA_MIN) const
  {
      const Line guideLine{start, direction};
      Intersection inter;
      int indexRing = 0;
      bool hasIntersection = false;
      double minDist = mOuterRing.halfPerimeter();
      std::vector<RingIntersection> intersections = intersectionsAll(guideLine);
      for (size_t i = 0; i < intersections.size(); ++i)
      {
          for (size_t j = 0; j < intersections[i].intersections.size(); ++j)
          {
              if (Geometry::distance(start, intersections[i].intersections[j].point) < mOuterRing.halfPerimeter() * ACCURACY_DOUBLE)
              {
                  continue;
              }
              BorderedLine tack(start, intersections[i].intersections[j].point);
              double tackLen = tack.getLength();
              if (tack.isForward() == guideLine.isForward() && tackLen < minDist)
              {
                  inter = intersections[i].intersections[j];
                  indexRing = intersections[i].ringIndex;
                  minDist = tackLen;
                  hasIntersection = true;
              }
          }
      }
      RingIntersection nearestIntersection(inter, indexRing);
      if (Geometry::distance(start, nearestIntersection.point) < TRAVERSA_MIN)
      {
          hasIntersection = false;
      }
      return hasIntersection ? std::optional(nearestIntersection) : std::nullopt;
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
