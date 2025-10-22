#ifndef ABSTRACT_OPERATIONS_EXTRACT_POINTS_FROM_GEOMS_HPP
#define ABSTRACT_OPERATIONS_EXTRACT_POINTS_FROM_GEOMS_HPP
//==============================================================================
#include <PrimaryEntities/GeometryConverter.hpp>
//==============================================================================
namespace AbstractOperations
{
//==============================================================================
//==============================================================================
// Функции, достающие из различных геометрий составляющие их точки
//==============================================================================
/*
 * \brief Функция, возвращающая ограничивающие точки для одной точки
 * \param[in] point - точка
 * \return точка
 */
template <typename PointType>
PrimaryEntities::MultiPoint<PointType> 
getBorderPoints(const PointType& point) noexcept
{
  return { point };
}
//==============================================================================
/*
 * \brief Функция, возвращающая все точки для одной точки
 * \param[in] point - точка
 * \return точка
 */
template <typename PointType>
PrimaryEntities::MultiPoint<PointType>
getAllPoints(const PointType& point) noexcept
{
  return getBorderPoints( point );
}
//==============================================================================
/*
 * \brief Функция, возвращающая ограничивающие точки для набора точек
 * \param[in] points - точки
 * \return точки
 */
template <typename PointType>
PrimaryEntities::MultiPoint<PointType>
getBorderPoints(const PrimaryEntities::MultiPoint<PointType>& points)
noexcept
{
  return points;
}
//==============================================================================
/*
 * \brief Функция, возвращающая все точки для набора точек
 * \param[in] points - точки
 * \return точки
 */
template <typename PointType>
PrimaryEntities::MultiPoint<PointType>
getAllPoints(const PrimaryEntities::MultiPoint<PointType>& points)
noexcept
{
  return points;
}
//==============================================================================
/*!
 * \brief getBorderPoints -- функция, возвращающая ограничивающие box точки
 * (все четыре угла)
 * \param[in] box - заданный box
 * \return точки (четыре), формирующие границу этого box
 */
template <typename PointType>
PrimaryEntities::MultiPoint<PointType>
getBorderPoints( const PrimaryEntities::Box<PointType>& box ) noexcept
{
  geos::geom::Envelope geosBox = box.getEnvelope();

  geos::geom::CoordinateSequence sequence({
    geos::geom::CoordinateXY(geosBox.getMinX(), geosBox.getMinY()),
    geos::geom::CoordinateXY(geosBox.getMaxX(), geosBox.getMaxY()), 
    geos::geom::CoordinateXY(geosBox.getMaxX(), geosBox.getMinY()),
    geos::geom::CoordinateXY(geosBox.getMinX(), geosBox.getMaxY())});

  geos::geom::GeometryFactory::Ptr geometryFactory
    = geos::geom::GeometryFactory::create();
  std::unique_ptr<geos::geom::MultiPoint> multiPoint
    = geometryFactory->createMultiPoint(sequence);

  PrimaryEntities::MultiPoint<PointType> retVal(
    PrimaryEntities::fromGeosGeometry<PrimaryEntities::MultiPoint<PointType>>(
                                                            multiPoint.get()));

  return retVal;
}

//==============================================================================
/*!
 * \brief getAllPoints -- функция, возвращающая задающие box точки
 * (два противоположных угла)
 * \param[in] box - заданный box
 * \return точки (две), формирующие этот box
 */
template <typename PointType>
inline PrimaryEntities::MultiPoint<PointType>
getAllPoints(const PrimaryEntities::Box<PointType>& box) noexcept
{
  return { box.min_corner(), box.max_corner() };
}
//==============================================================================
/*!
 * \brief getBorderPoints -- функция, возвращающая содержащиеся в линии точки
 * \param[in] line - заданная линия
 * \return все точки, содержащиеся в линии
 */
template <typename PointType>
PrimaryEntities::MultiPoint<PointType>
getBorderPoints(
  const PrimaryEntities::Linestring<PointType>& line) noexcept
{
  PrimaryEntities::MultiPoint<PointType> retVal;

  retVal.insert(retVal.end(), line.begin(), line.end());

  return retVal;
}
//==============================================================================
/*!
 * \brief getAllPoints -- функция, возвращающая содержащиеся в линии точки 
 * \param[in] line - заданная линия
 * \return все точки, содержащиеся в линии
 */
template <typename PointType>
inline PrimaryEntities::MultiPoint<PointType>
getAllPoints(const PrimaryEntities::Linestring<PointType>& line) noexcept
{
  return getBorderPoints(line);
}
//==============================================================================
/*!
 * \brief getBorderPoints -- функция, возвращающая содержащиеся во множестве линий
 * точки
 * \param[in] line - заданная линия
 * \return все точки, содержащиеся в линии
 */
template <typename PointType>
PrimaryEntities::MultiPoint<PointType>
getBorderPoints(
  const PrimaryEntities::MultiLinestring<PointType> &multiLine) noexcept
{
  PrimaryEntities::MultiPoint<PointType> retVal;
  PrimaryEntities::MultiPoint<PointType> temp;

  for (const auto& line : multiLine)
  {
    temp = getBorderPoints(line);
    retVal.insert(retVal.end(), temp.begin(), temp.end());
  }

  return retVal;
}
//==============================================================================
/*!
 * \brief getAllPoints -- функция, возвращающая содержащиеся во множестве линий
 * точки
 * \param[in] line - заданная линия
 * \return все точки, содержащиеся в линии
 */
template <typename PointType>
inline PrimaryEntities::MultiPoint<PointType>
getAllPoints(
  const PrimaryEntities::MultiLinestring<PointType>& multiLine) noexcept
{
  return getBorderPoints(multiLine);
}
//==============================================================================
/*!
 * \brief getBorderPoints -- функция, возвращающая содержащиеся в массиве точки
 * \param[in] points - заданный массив точек
 * \return все точки, содержащиеся в массиве
 */
template <typename PointType>
PrimaryEntities::MultiPoint<PointType>
getBorderPoints(const std::vector<PointType>& points) noexcept
{
  PrimaryEntities::MultiPoint<PointType> retVal;

  retVal.insert(retVal.end(), points.begin(), points.end());

  return retVal;
}
//==============================================================================
/*!
 * \brief getAllPoints -- функция, возвращающая содержащиеся в массиве точки 
 * \param[in] points - заданный массив точек
 * \return все точки, содержащиеся в массиве
 */
template <typename PointType>
inline PrimaryEntities::MultiPoint<PointType>
getAllPoints(const std::vector<PointType>& points) noexcept
{
  return getBorderPoints(points);
}
//==============================================================================
/*!
 * \brief getBorderPoints -- функция, возвращающая все вершины кольца
 * \param[in] ring - заданное кольцо
 * \return все вершины кольца
 */
template <typename PointType>
PrimaryEntities::MultiPoint<PointType>
getBorderPoints(const PrimaryEntities::Ring<PointType>& ring) noexcept
{
  PrimaryEntities::MultiPoint<PointType> retVal;
  retVal.insert(retVal.end(), ring.begin(), ring.end());

  //! Если было замкнутым, то одна точка лишняя.
  if (ring.size() >= 2)
    if (retVal.front() == retVal.back())
      retVal.pop_back();

  return retVal;
}
//==============================================================================
/*!
 * \brief getAllPoints -- функция, возвращающая все точки кольца
 * \param[in] ring - заданное кольцо
 * \return все точки кольца
 */
template <typename PointType>
inline PrimaryEntities::MultiPoint<PointType>
getAllPoints(const PrimaryEntities::Ring<PointType>& ring) noexcept
{
  return getBorderPoints(ring);
}
//==============================================================================
/*!
 * \brief getBorderPoints -- функция,
 * возвращающая вершины из внешней границы многоугольника
 * \param[in] polygon - заданный многоугольник
 * \return точки внешней границы многоугольника
 */
template <typename PointType>
inline PrimaryEntities::MultiPoint<PointType>
getBorderPoints(
  const PrimaryEntities::Polygon<PointType>& polygon) noexcept
{
  return getAllPoints(polygon.outer());
}
//==============================================================================
/*!
 * \brief getAllPoints -- функция, возвращающая все вершины многоугольника
 * (включая дырки)
 * \param[in] polygon - заданный многоугольник
 * \return все вершины многоугольника
 */
template <typename PointType>
PrimaryEntities::MultiPoint<PointType>
getAllPoints(const PrimaryEntities::Polygon<PointType>& polygon) noexcept
{
  auto retVal(getBorderPoints(polygon));

  for (const auto& ring : polygon.inners())
  {
    auto ringPoints(getAllPoints(ring));
    retVal.insert(retVal.end(), ringPoints.begin(), ringPoints.end());
  }

  return retVal;
}
//==============================================================================
/*!
 * \brief getBorderPoints -- функция,
 * возвращающая точки внешних границ многоугольников
 * \details Все точки, содержащиеся во внешних границах многоугольников,
 * составляющих данное множество.
 * Взаимное расположение многоугольников не анализируется
 * \param[in] polygons - заданное множество многоугольников
 * \return точки внешних границ многоугольников
 */
template <typename PointType>
PrimaryEntities::MultiPoint<PointType>
getBorderPoints(
  const PrimaryEntities::MultiPolygon<PointType> &polygons) noexcept
{
  PrimaryEntities::MultiPoint<PointType> retVal;
  PrimaryEntities::MultiPoint<PointType> tmpPoint;

  for (const auto& polygon : polygons)
  {
    tmpPoint = getBorderPoints(polygon);
    retVal.insert(retVal.end(), tmpPoint.begin(), tmpPoint.end());
  }

  return retVal;
}
//==============================================================================
/*!
 * \brief getAllPoints -- функция, возвращающая все вершины
 * множества многоугольников
 * \param[in] polygons - заданное множество многоугольников
 * \return все точки множества многоугольников
 */
template <typename PointType>
PrimaryEntities::MultiPoint<PointType>
getAllPoints(
  const PrimaryEntities::MultiPolygon<PointType>& polygons) noexcept
{
  PrimaryEntities::MultiPoint<PointType> retVal;
  PrimaryEntities::MultiPoint<PointType> tmpPoint;

  for (const auto& polygon : polygons)
  {
    tmpPoint = getAllPoints(polygon);
    retVal.insert(retVal.end(), tmpPoint.begin(), tmpPoint.end());
  }

  return retVal;
}
//==============================================================================
//==============================================================================
} // ! AbstractOperations
//==============================================================================
#endif // ! ABSTRACT_OPERATIONS_EXTRACT_POINTS_FROM_GEOMS_HPP
