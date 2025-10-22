#ifndef ABSTRACT_OPERATIONS_NORMALIZE_HPP
#define ABSTRACT_OPERATIONS_NORMALIZE_HPP
//==============================================================================
#include <AbstractOperations/AbstractOperationsException.hpp>
#include <AbstractOperations/ExtractPointsFromGeoms.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeodesicEntities.hpp>
//==============================================================================
namespace AbstractOperations 
{
//==============================================================================
/*!
 * \brief Функция, осуществляющая нормализацию точки в географической системе 
 *        координат.
 * \param[in|out] point - точка, которую необходимо нормализовать.
 * \param[in] range -- диапазон значений:
 *                     REGULAR - от -180 (-PI) до 180 (PI)
 *                     NORMAL  - от 0 (0) до 360 (2 * PI)
 */
template <typename PointType>
void
normalize(PointType& point, LongitudeRange range = NORMAL) noexcept
{
  point.normalize(range);
}
//==============================================================================
/*!
 * \brief normalize -- функция,
 * осуществляющая нормализацию множества точек
 * в географической системе координат.
 *
 * \param[in|out] multiPoint -- множество точек,
 * которое необходимо нормализовать.
 *
 * \param[in] range -- диапазон значений:
 *                     REGULAR - от -180 (-PI) до 180 (PI)
 *                     NORMAL  - от 0 (0) до 360 (2 * PI)
 *
 * \warning Функцию можно использовать только для множества точек,
 * имеющих реализацию собственной нормализации в определённом методе
 * ::normalize(LongitudeRange range).
 */
template <typename PointType>
void
normalize(
  PrimaryEntities::MultiPoint<PointType>& multiPoint,
  LongitudeRange range = NORMAL) noexcept
{
  for (PointType& p : multiPoint)
    p.normalize(range);
}
//==============================================================================
/*!
 * \brief normalize -- функция,
 * осуществляющая нормализацию ломаной линии
 * в географической системе координат.
 *
 * \param[in|out] linestring -- ломаная линия,
 * которую необходимо нормализовать.
 *
 * \param[in] range -- диапазон значений:
 *                     REGULAR - от -180 (-PI) до 180 (PI)
 *                     NORMAL  - от 0 (0) до 360 (2 * PI)
 *
 * \warning Функцию можно использовать только для ломаных линий,
 * точки которых имеют реализацию собственной нормализации в определённом методе
 * ::normalize(LongitudeRange range).
 */
template <typename PointType>
void
normalize(
  PrimaryEntities::Linestring<PointType>& linestring,
  LongitudeRange range = NORMAL) noexcept
{
  for (PointType& p : linestring)
    p.normalize(range);
}
//==============================================================================
/*!
 * \brief normalize -- функция,
 * осуществляющая нормализацию множества ломаных линий
 * в географической системе координат.
 *
 * \param[in|out] multiLinestring -- множество ломаных линий,
 * которое необходимо нормализовать.
 *
 * \param[in] range -- диапазон значений:
 *                     REGULAR - от -180 (-PI) до 180 (PI)
 *                     NORMAL  - от 0 (0) до 360 (2 * PI)
 *
 * \warning Функцию можно использовать только для множества ломаных линий,
 * вершины которых имеют реализацию собственной нормализации
 * в определённом методе ::normalize(LongitudeRange range).
 */
template <typename PointType>
void
normalize(
  PrimaryEntities::MultiLinestring<PointType>& multiLinestring,
  LongitudeRange range = NORMAL) noexcept
{
  for (auto& linestring : multiLinestring)
    normalize(linestring, range);
}
//==============================================================================
/*!
 * \brief normalize -- функция,
 * осуществляющая нормализацию прямоугольника,
 * стороны которого параллельны координатным осям,
 * в географической системе координат.
 *
 * \param[in|out] box -- прямоугольник,
 * стороны которого параллельны координатным осям,
 * который необходимо нормализовать.
 *
 * \param[in] range -- диапазон значений:
 *                     REGULAR - от -180 (-PI) до 180 (PI)
 *                     NORMAL  - от 0 (0) до 360 (2 * PI)
 *
 * \warning Функцию можно использовать только для прямоугольника,
 * стороны которого параллельны координатным осям,
 * вершины которого являются точками,
 * имеющими реализацию собственной нормализации в определённом методе
 * ::normalize(LongitudeRange range).
 */
template <typename PointType>
void
normalize(
  PrimaryEntities::Box<PointType>& box,
  LongitudeRange range = NORMAL ) noexcept
{
  box.min_corner( ).normalize( range );
  box.max_corner( ).normalize( range );
}

//==============================================================================
/*!
 * \brief normalize -- функция,
 * осуществляющая нормализацию кольца
 * в географической системе координат.
 *
 * \param[in|out] ring -- кольцо,
 * которое необходимо нормализовать.
 *
 * \param[in] range -- диапазон значений:
 *                     REGULAR - от -180 (-PI) до 180 (PI)
 *                     NORMAL  - от 0 (0) до 360 (2 * PI)
 *
 * \warning Функцию можно использовать только для колец, вершины которых
 * имеют реализацию собственной нормализации в определённом методе
 * ::normalize(LongitudeRange range).
 */
template <typename PointType>
void
normalize(
  PrimaryEntities::Ring<PointType>& ring,
  LongitudeRange range = NORMAL) noexcept
{
  for (PointType& p : ring)
    p.normalize(range);
}
//==============================================================================
/*!
 * \brief normalize -- функция,
 * осуществляющая нормализацию многоугольника
 * в географической системе координат.
 *
 * \param[in|out] polygon -- многоугольник,
 * который необходимо нормализовать.
 *
 * \param[in] range -- диапазон значений:
 *                     REGULAR - от -180 (-PI) до 180 (PI)
 *                     NORMAL  - от 0 (0) до 360 (2 * PI)
 *
 * \warning Функцию можно использовать только для многоугольников,
 * вершины которых имеют реализацию собственной нормализации
 * в определённом методе ::normalize(LongitudeRange range).
 */
template <typename PointType>
void
normalize(
  PrimaryEntities::Polygon<PointType>& polygon,
  LongitudeRange range = NORMAL) noexcept
{
  normalize(polygon.outer(), range);
  for (auto& ring : polygon.inners())
    normalize(ring, range);
}
//==============================================================================
/*!
 * \brief normalize -- функция,
 * осуществляющая нормализацию множества многоугольников
 * в географической системе координат.
 *
 * \param[in|out] multiPolygon -- множество многоугольников,
 * которое необходимо нормализовать.
 *
 * \param[in] range -- диапазон значений:
 *                     REGULAR - от -180 (-PI) до 180 (PI)
 *                     NORMAL  - от 0 (0) до 360 (2 * PI)
 *
 * \warning Функцию можно использовать только для множества многоугольников,
 * вершины которых имеют реализацию собственной нормализации
 * в определённом методе ::normalize(LongitudeRange range).
 */
template <typename PointType>
void
normalize(
  PrimaryEntities::MultiPolygon<PointType>& multiPolygon,
  LongitudeRange range = NORMAL) noexcept
{
  for (auto& polygon : multiPolygon)
    normalize(polygon, range);
}
//==============================================================================
/*! 
 * \brief envelopeByOrder -- обёртка массива точек на поверхности Земли
 * в GeoBox с учетом возможного 180-го меридиана.
 * 
 * \param[in] points - непустой массив точек в географических координатах.
 * 
 * \return искомый прямоугольник,
 * стороны которого параллельны линиям широты и долготы.
 *
 * \details функция определяет расположение прямоугольника
 * путём постепенного расширения во время перебора точек:
 * если очередная не попавшая в прямоугольник точка находится ближе
 * к западной границе прямоугольника,
 * прямоугольник расширяется на запад,
 * если ближе к восточной границе, то на восток.
 * Таким образом, расположение коробки на земном эллипсоиде
 * зависит от порядка следования точек в массиве.
 *
 * \warning Можно использовать только для массива точек,
 * имеющих открытые методы Radian ::getLatitude() и Radian ::getLongitude(),
 * возвращающие широту и долготу точки в радианах соответственно.
 *
 * \throws GeodesicFailure # NullGeometryIncorrectOperation
 */
template <typename PointType>
GeoBox2D
envelopeByOrder(const PrimaryEntities::MultiPoint<PointType>& points)
{
  if( points.empty( ) )
    throw AbstractOperationsFailure( AbstractOperationsEnumFailure::NullGeometryIncorrectOperation );

  //! Инициализируем углы прямоугольника.

  //!< Минимальная широта прямоугольника.
  Radian minLatitude(points.front().getCoordinates().y);
  //!< Минимальная долгота прямоугольника.
  Radian minLongitude(points.front().getCoordinates().x);
  //!< Максимальная широта прямоугольника.
  Radian maxLatitude(minLatitude);
  //!< Максимальная долгота прямоугольника.
  Radian maxLongitude(minLongitude);
  //!< Значение итератора, соответствующее концу множества точек.
  for (auto point : points)
  {
    /*!<
     * Разность долготы новой точки и долготы юго-западного угла прямоугольника,
     * приведённая к диапазону [-π, π].
     */
    Radian dmin((Radian(point.getCoordinates().x) - minLongitude).normalize());
    /*!<
     * Разность долготы новой точки
     * и долготы северо-восточного угла прямоугольника,
     * приведённая к диапазону [-π, π].
     */
    Radian dmax((Radian(point.getCoordinates().x) - maxLongitude).normalize());

    //! Сдвиг юго-западного угла.
    if ((dmin < Radian(0.)) && (std::abs(dmin.get()) <= std::abs(dmax.get())))
      minLongitude = Radian(point.getCoordinates().x);
    //! Сдвиг северо-восточного угла.
    if ((dmax > Radian(0.)) && (std::abs(dmin.get()) >= std::abs(dmax.get())))
      maxLongitude = Radian(point.getCoordinates().x);
    //! Обновление широты.
    if (Radian(point.getCoordinates().y) < minLatitude)
      minLatitude = Radian(point.getCoordinates().y);
    if (Radian(point.getCoordinates().y) > maxLatitude)
      maxLatitude = Radian(point.getCoordinates().y);
  }

  return GeoBox2D(
    GeoPoint2D(minLatitude, minLongitude),
    GeoPoint2D(maxLatitude, maxLongitude));
}
//==============================================================================
/*!
 * \brief isNeedToBeNormalized -- функция, возвращающая "наивную" оценку того,
 * нуждается ли геометрия в географических координатах в нормализации.
 *
 * \param[in] box -- геометрия, для которой необходимо получить оценку.
 *
 * \return true, если геометрия с таким box нуждается в нормализации;
 *         false -- иначе.
 *
 * \warning Очевидно, что возвращаемое значение зависит не от самой геометрии,
 * а, скорее, от выбора прямоугольника, полностью содержащего её в себе.
 * Сам этот выбор неоднозначен.
 * Функцию можно использовать только для геометрий,
 * основанных на GeoPoint2D или GeoPoint3D.
 */
template <typename GeometryType>
bool isNeedToBeNormalized(const GeometryType& geometry) noexcept
{
  auto points(getAllPoints(geometry));
  if (points.empty())
    return false;
  else
  {
    GeoBox2D box(envelopeByOrder(points));
    return box.min_corner().getCoordinates().x 
           > box.max_corner().getCoordinates().x;
  }
}
//==============================================================================
/*!
 * \brief normalizeIfNeed -- функция,
 * осуществляющая приведение геометрии
 * в географической системе координат в диапазон [0, 2π] по долготе.
 *
 * \param[in|out] geometry -- геометрия,
 * которую необходимо нормализовать.
 *
 * \warning Функцию можно использовать только для геометрий,
 * основанных на точках,
 * имеющих реализацию собственной нормализации в определённом методе
 * ::normalize(LongitudeRange range).
 */
template <typename GeometryType>
void normalizeIfNeed(GeometryType& geometry) noexcept
{
  if (isNeedToBeNormalized(geometry))
    normalize(geometry, NORMAL);
}
//==============================================================================
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_NORMALIZE_HPP