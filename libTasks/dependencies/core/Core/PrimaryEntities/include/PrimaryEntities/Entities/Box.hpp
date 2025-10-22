#ifndef PRIMARY_ENTITIES_BOX_HPP
#define PRIMARY_ENTITIES_BOX_HPP
//==============================================================================
#include "Polygon.hpp"
#include <PrimaryEntities/PointConverter.hpp>
//! Geos
#include <geos/geom/Envelope.h>
//==============================================================================
namespace PrimaryEntities
{
//==============================================================================
/*!
 *\brief Коробка.
 *\details Основана на geos::geom::Envelope.
 */
template <typename Point>
class Box
{
//==============================================================================
public:
//==============================================================================
  /*!
   *\brief Конструктор класса
   *\param[in] min - координаты минимального угла box
   *\param[in] max - координаты максимального угла box
   */
  Box(const Point& min = Point(), const Point& max = Point())
    : mMin(min), mMax(max)
  {}
//==============================================================================
//==============================================================================
  /*!
   *\brief Получение минимального угла box по константной ссылке
   *\return Минимальный угол
   */
  inline Point const& min_corner() const
  {
    return mMin;
  }
//==============================================================================
  /*!
   *\brief Получение максимального угла box по константной ссылке
   *\return Максимальный угол
   */
  inline Point const& max_corner() const
  {
    return mMax;
  }
//==============================================================================
  /*!
   *\brief Получение минимального угла box по ссылке
   *\return Минимальный угол
   */
  inline Point& min_corner()
  {
    return mMin;
  }
//==============================================================================
  /*!
   *\brief Получение максимального угла box по ссылке
   *\return Максимальный угол
   */
  inline Point& max_corner()
  {
    return mMax;
  }
//==============================================================================
//==============================================================================
  /*!
   *\brief Увеличение данного box на точку
   *\details Если точка находится за пределами box, он растягивается, пока не
   *         накроет ее
   *\param[in] p - точка, которая должна оказаться накрытой box
   */
  inline void expandToPoint(const Point& p)
  {
    std::unique_ptr<geos::geom::CoordinateSequence> minPoint
      = toGeosGeometry(mMin)->getCoordinates();
    std::unique_ptr<geos::geom::CoordinateSequence> maxPoint
      = toGeosGeometry(mMax)->getCoordinates();
    std::unique_ptr<geos::geom::CoordinateSequence> curPoint
      = toGeosGeometry(p)->getCoordinates();

    for (size_t i = 0; i < minPoint->getDimension(); ++i)
    {
      if (curPoint->getOrdinate(0, i) < minPoint->getOrdinate(0, i))
        minPoint->setOrdinate(0, i, curPoint->getOrdinate(0, i));

      if (curPoint->getOrdinate(0, i) > maxPoint->getOrdinate(0, i))
        maxPoint->setOrdinate(0, i, curPoint->getOrdinate(0, i));
    }

    geos::geom::GeometryFactory::Ptr geometryFactory
      = geos::geom::GeometryFactory::create();

    mMin = fromGeosPoint<Point>(
      geometryFactory->createPoint(std::move(minPoint)).get());
    mMax = fromGeosPoint<Point>(
      geometryFactory->createPoint(std::move(maxPoint)).get());
  }
//==============================================================================
  /*!
   *\brief Получение копии данного box, увеличенного на точку
   *\details Если точка находится за пределами box, его копия растягивается,
   *         пока не накроет ее. Текущий объект не изменяется
   *\param[in] p - точка, которая должна оказаться накрытой box
   *\return увеличенный (при необходимости) box
   */
  inline Box<Point> getBoxExpandedToPoint(const Point& p) const
  {
    Box<Point> ret = *this;
    ret.expandToPoint(p);
    return ret;
  }
//==============================================================================
//==============================================================================
  /*!
   *\brief Увеличение данного box на другой box
   *\details Увеличиваем текущий box так, чтобы в него целиком влез другой
   *\param[in] incrementBox - прибавляемый box
   */
  inline void expandToBox(const Box<Point>& incrementBox)
  {
    expandToPoint(incrementBox.min_corner());
    expandToPoint(incrementBox.max_corner());
  }
//==============================================================================
  /*!
   *\brief Получение копии box, увеличенного на другой box
   *\param[in] incrementBox - прибавляемая коробка
   *\return увеличенный (при необходимости) box
   */
  inline Box<Point> getBoxExpandedToBox(const Box<Point>& incrementBox) const
  {
    Box<Point> retVal = *this;
    retVal.expandToBox(incrementBox);
    return retVal;
  }
//==============================================================================
//==============================================================================
  /*!
   *\brief Проверка наличия пересечения двух box
   *\details Проверка осуществляется покомпонентно.
   *         Поддерживаются все типы точек.
   *\param[in] other - сравниваемый box
   *\return - есть ли у текущего и сравниваемого box хотя бы одна общая точка
   */
  inline bool hasIntersectionWith(const Box<Point>& other) const
  {
    std::unique_ptr<geos::geom::CoordinateSequence> minPoint
      = toGeosGeometry(mMin)->getCoordinates();
    std::unique_ptr<geos::geom::CoordinateSequence> maxPoint
      = toGeosGeometry(mMax)->getCoordinates();

    std::unique_ptr<geos::geom::CoordinateSequence> minOtherPoint
      = toGeosGeometry(other.min_corner())->getCoordinates();
    std::unique_ptr<geos::geom::CoordinateSequence> maxOtherPoint
      = toGeosGeometry(other.max_corner())->getCoordinates();

    for (size_t i = 0; i < minPoint->getDimension(); ++i)
    {
      if (!((minPoint->getOrdinate(0, i) <= maxOtherPoint->getOrdinate(0, i))
        && (maxPoint->getOrdinate(0, i) >= minOtherPoint->getOrdinate(0, i))))
        return false;
    }

    return true;
  }
//==============================================================================
//==============================================================================
  /*!
   *\brief Преобразование в полигон
   *\details Результирующий полигон состоит из 5 точек,
   *         направление обхода - по часовой стрелке (в данной СК),
   *         первая точка - минимальный угол исходного box.
   *         Разрешены только двумерные точки
   *\return Результирующий полигон
   */
  inline Polygon<Point> toPolygon() const
  {
    geos::geom::CoordinateXY minCoord = mMin.getCoordinates();
    geos::geom::CoordinateXY maxCoord = mMax.getCoordinates();

    Polygon<Point> polygon;
    polygon.outer().push_back(mMin);

    geos::geom::GeometryFactory::Ptr geometryFactory
      = geos::geom::GeometryFactory::create();

    Point point(fromGeosPoint<Point>(geometryFactory->createPoint(
      geos::geom::CoordinateXY(minCoord.x, maxCoord.y)).get()));
    polygon.outer().push_back(point);

    polygon.outer().push_back(mMax);

    point = fromGeosPoint<Point>(geometryFactory->createPoint(
      geos::geom::CoordinateXY(maxCoord.x, minCoord.y)).get());
    polygon.outer().push_back(point);

    polygon.outer().push_back(mMin);

    return polygon;
  }
//==============================================================================
  /*!
   *\brief Получение геометрии geos
   *\return geos::geom::Envelope
   */
  inline geos::geom::Envelope getEnvelope() const
  {
    return geos::geom::Envelope(mMin.getCoordinates(), mMax.getCoordinates());
  }
//==============================================================================
private:
//==============================================================================
  Point mMin;
  Point mMax;
//==============================================================================
};
//==============================================================================
} //! PrimaryEntities
//==============================================================================
#endif //! PRIMARY_ENTITIES_BOX_HPP
