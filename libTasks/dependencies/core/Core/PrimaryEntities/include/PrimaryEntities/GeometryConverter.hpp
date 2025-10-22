#ifndef PRIMARY_ENTITIES_GEOMETRY_CONVERTER_HPP
#define PRIMARY_ENTITIES_GEOMETRY_CONVERTER_HPP
//==============================================================================
#include <PrimaryEntities/AtEntitiesType.hpp>
#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
#include <PrimaryEntities/PointConverter.hpp>
#include <PrimaryEntities/Inner/InnerConverter.hpp>
//==============================================================================
namespace PrimaryEntities
{
//==============================================================================
/*!
 *\brief Преобразование из коробки geos
 *\param[in] box - коробка geos
 *\return коробка PrimaryEntities.
 * *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template <typename PointType>
PrimaryEntities::Box<PointType> fromGeosBox(
  const geos::geom::Envelope* geosBox)
{
  PrimaryEntities::Box<PointType> box;
  Inner::fromGeosBox(box, geosBox);
  return box;
}
//==============================================================================
/*!
 *\brief Преобразование из геометрии geos
 *\param[in] geosGeometry - геометрия geos
 *\return геометрия PrimaryEntities.
 * *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template <typename GeometryType>
GeometryType fromGeosGeometry(const geos::geom::Geometry* geosGeometry)
{
  GeometryType geometry;
  Inner::fromGeosGeometry(geometry, geosGeometry);
  return geometry;
}
//==============================================================================
/*!
 *\brief Преобразование из точки geos
 *\param[in] geosGeometry - геометрия geos
 *\return 2D точка в декартовых координатах.
 * *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template<>
Point2D fromGeosGeometry(const geos::geom::Geometry* geosGeometry);
//==============================================================================
/*!
 *\brief Преобразование из точки geos
 *\param[in] geosGeometry - геометрия geos
 *\return 3D точка в декартовых координатах.
 * *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template<>
Point3D fromGeosGeometry(const geos::geom::Geometry* geosGeometry);
//==============================================================================
/*!
 *\brief Преобразование из точки geos
 *\param[in] geosGeometry - геометрия geos
 *\return 2D точка в геодезических координатах.
 * *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template<>
GeoPoint2D fromGeosGeometry(const geos::geom::Geometry* geosGeometry);
//==============================================================================
/*!
 *\brief Преобразование из точки geos
 *\param[in] geosGeometry - геометрия geos
 *\return 3D точка в геодезических координатах.
 * *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template<>
GeoPoint3D fromGeosGeometry(const geos::geom::Geometry* geosGeometry);
//==============================================================================
//==============================================================================
/*!
 *\brief Получение линии geos
 *\param[in] linestring - линия
 *\return Указатель на geos Linestring
 * \throws PrimaryEntitiesFailure # IncorrectLinestring || EmptyGeometryPtr
 */
template <typename PointType>
std::unique_ptr<geos::geom::LineString> toGeosGeometry(
  const PrimaryEntities::Linestring<PointType>& linestring)
{
  // Линия в geos не может содержать только одну точку
  if (linestring.size() == 1)
    throw PrimaryEntitiesFailure(IncorrectLinestring,
                                 ": Too few points", "toGeosGeometry");

  geos::geom::GeometryFactory::Ptr geometryFactory
    = geos::geom::GeometryFactory::create();
  geos::geom::CoordinateSequence sequence;

  for (size_t i = 0; i < linestring.size(); ++i)
    sequence.add(linestring.at(i).getCoordinates());

  std::unique_ptr<geos::geom::LineString> geosLinestring(
    geometryFactory->createLineString(sequence));

  // Проверим, что линия действительно есть
  if (geosLinestring == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                 ": Linestring was not converted",
                                 "toGeosGeometry");

  return geosLinestring;
}
//==============================================================================
/*!
 *\brief Получение мультилинии geos
 *\param[in] linestrings - мультилиния
 *\return Указатель на geos MultiLineString
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr
 */
template <typename PointType>
std::unique_ptr<geos::geom::MultiLineString> toGeosGeometry(
  const PrimaryEntities::MultiLinestring<PointType>& linestrings)
{
  geos::geom::GeometryFactory::Ptr geometryFactory
    = geos::geom::GeometryFactory::create();
  std::vector<std::unique_ptr<geos::geom::LineString>> linestringVector;

  for (size_t i = 0; i < linestrings.size(); ++i)
    linestringVector.push_back(toGeosGeometry(linestrings.at(i)));

  std::unique_ptr<geos::geom::MultiLineString> geosMultiLinestring(
    geometryFactory->createMultiLineString(std::move(linestringVector)));

  // Проверим, что мультилиния действительно есть
  if (geosMultiLinestring == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                 ": MultiLinestring was not converted",
                                 "toGeosGeometry");

  return geosMultiLinestring;
}
//==============================================================================
/*!
 *\brief Получение набора точек geos
 *\param[in] multiPoint - набор точек
 *\return Указатель на geos MultiPoint
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr
 */
template <typename PointType>
std::unique_ptr<geos::geom::MultiPoint> toGeosGeometry(
  const PrimaryEntities::MultiPoint<PointType>& multiPoint)
{
  geos::geom::GeometryFactory::Ptr geometryFactory
    = geos::geom::GeometryFactory::create();
  geos::geom::CoordinateSequence sequence;

  for (size_t i = 0; i < multiPoint.size(); ++i)
    sequence.add(multiPoint.at(i).getCoordinates());

  std::unique_ptr<geos::geom::MultiPoint> geosMultiPoint(
    geometryFactory->createMultiPoint(sequence));

  // Проверим, что набор точек действительно есть
  if (geosMultiPoint == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                 ": MultiPoint was not converted",
                                 "toGeosGeometry");

  return geosMultiPoint;
}
//==============================================================================
/*!
 *\brief Получение кольца как geos полигона
 *\details Кольцо должно быть замкнуто. Кольцо конвертируется в geos полигон,
 *         поскольку хотим работать с площадной геометрией, а кольцо geos
 *         площадным не является.
 *\param[in] ring - кольцо
 *\return Указатель на geos Polygon
 * \throws PrimaryEntitiesFailure # IncorrectRing || EmptyGeometryPtr
 */
template <typename PointType>
std::unique_ptr<geos::geom::Polygon> toGeosGeometry(
  const PrimaryEntities::Ring<PointType>& ring)
{
  // Кольцо в geos всегда должно быть замкнутым
  if (!ring.empty() && !(ring.at(0) == ring.at(ring.size() - 1)))
    throw PrimaryEntitiesFailure(IncorrectRing,
                                 ": Ring must be closed", "toGeosGeometry");

  // В кольце не может быть меньше четырёх вершин
  if (!ring.empty() && (ring.size() < 4))
    throw PrimaryEntitiesFailure(IncorrectRing,
                                 ": Too few points in ring", "toGeosGeometry");

  geos::geom::GeometryFactory::Ptr geometryFactory
    = geos::geom::GeometryFactory::create();
  geos::geom::CoordinateSequence sequence;

  for (size_t i = 0; i < ring.size(); ++i)
    sequence.add(ring.at(i).getCoordinates());

  std::unique_ptr<geos::geom::Polygon> geosPolygon(
    geometryFactory->createPolygon(std::move(sequence)));

  // Проверим, что полигон действительно есть
  if (geosPolygon == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                 ": Ring was not converted",
                                 "toGeosGeometry");

  return geosPolygon;
}
//==============================================================================
/*!
 *\brief Преобразование в кольцо geos
 *\details Кольцо должно быть замкнуто.
 *\param[in] ring - кольцо
 *\return Указатель на geos LinearRing
 * \throws PrimaryEntitiesFailure # IncorrectRing || EmptyGeometryPtr
 */
template <typename PointType>
std::unique_ptr<geos::geom::LinearRing> toGeosRing(
  const PrimaryEntities::Ring<PointType>& ring)
{
  // Кольцо в geos всегда должно быть замкнутым
  if (!ring.empty() && !(ring.at(0) == ring.at(ring.size() - 1)))
    throw PrimaryEntitiesFailure(IncorrectRing,
                                 ": Ring must be closed", "toGeosRing");

  // В кольце не может быть меньше четырёх вершин
  if (!ring.empty() && (ring.size() < 4))
    throw PrimaryEntitiesFailure(IncorrectRing,
                                 ": Too few points in ring", "toGeosRing");

  geos::geom::GeometryFactory::Ptr geometryFactory
    = geos::geom::GeometryFactory::create();
  geos::geom::CoordinateSequence sequence;

  for (size_t i = 0; i < ring.size(); ++i)
    sequence.add(ring.at(i).getCoordinates());

  std::unique_ptr<geos::geom::LinearRing> geosLinearRing(
    geometryFactory->createLinearRing(sequence));

  // Проверим, что кольцо действительно есть
  if (geosLinearRing == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                 ": Ring was not converted",
                                 "toGeosGeometry");

  return geosLinearRing;
}
//==============================================================================
/*!
 *\brief Получение полигона geos
 *\param[in] polygon - полигон
 *\return Указатель на geos Polygon
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr
 */
template <typename PointType>
std::unique_ptr<geos::geom::Polygon> toGeosGeometry(
  const PrimaryEntities::Polygon<PointType>& polygon)
{
  geos::geom::GeometryFactory::Ptr geometryFactory
    = geos::geom::GeometryFactory::create();

  std::unique_ptr<geos::geom::LinearRing> outerRing
    = toGeosRing(polygon.outer());

  std::vector<std::unique_ptr<geos::geom::LinearRing>> innerRings;
  for (size_t i = 0; i < polygon.inners().size(); ++i)
    innerRings.push_back(toGeosRing(polygon.inners().at(i)));

  std::unique_ptr<geos::geom::Polygon> geosPolygon(
    geometryFactory->createPolygon(std::move(outerRing),
                                   std::move(innerRings)));

  // Проверим, что полигон действительно есть
  if (geosPolygon == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                 ": Polygon was not converted",
                                 "toGeosGeometry");

  return geosPolygon;
}
//==============================================================================
/*!
 *\brief Получение мультиполигона geos
 *\param[in] polygons - набор полигонов
 *\return Указатель на geos MultiPolygon
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr
 */
template <typename PointType>
std::unique_ptr<geos::geom::MultiPolygon> toGeosGeometry(
  const PrimaryEntities::MultiPolygon<PointType>& polygons)
{
  geos::geom::GeometryFactory::Ptr geometryFactory
    = geos::geom::GeometryFactory::create();
  std::vector<std::unique_ptr<geos::geom::Polygon>> polygonVector;

  for (size_t i = 0; i < polygons.size(); ++i)
    polygonVector.push_back(toGeosGeometry(polygons.at(i)));

  std::unique_ptr<geos::geom::MultiPolygon> multiPolygon
    = geometryFactory->createMultiPolygon(std::move(polygonVector));

  // Проверим, что мультиполигон действительно есть
  if (multiPolygon == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                 ": MultiPolygon was not converted",
                                 "toGeosGeometry");

  return multiPolygon;
}
//==============================================================================
/*!
 *\brief Получение коробки, как geos полигона
 *\details У коробки geos нет функций, аналогичных другим геометриям.
 *         Конвертация в полигон производится для возможности использования
 *         коробки в шаблоннных операциях над геометриями
 *\param[in] box - коробка
 *\return Указатель на geos Polygon
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr
 */
template <typename PointType>
std::unique_ptr<geos::geom::Polygon> toGeosGeometry(
  const PrimaryEntities::Box<PointType>& box)
{
  geos::geom::CoordinateXY min = box.min_corner().getCoordinates();
  geos::geom::CoordinateXY max = box.max_corner().getCoordinates();

  geos::geom::CoordinateSequence sequence({
    min, geos::geom::CoordinateXY(min.x, max.y), max,
    geos::geom::CoordinateXY(max.x, min.y), min });

  geos::geom::GeometryFactory::Ptr geometryFactory
    = geos::geom::GeometryFactory::create();

  std::unique_ptr<geos::geom::Polygon> geosPolygon
    = geometryFactory->createPolygon(std::move(sequence));

  // Проверим, что мультиполигон действительно есть
  if (geosPolygon == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                 ": Box was not converted",
                                 "toGeosGeometry");

  return geosPolygon;
}
//==============================================================================
//==============================================================================
/*!
 *\brief Преобразование из коллекции geos в мультиполигон geos
 *\details Все геометрии коллекции, не являющиеся полигонами, игнорируются.
 *\param[in] collection - коллекция geos
 *\return мультиполигон geos.
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr
 */
std::unique_ptr<geos::geom::MultiPolygon> choosePolygonsFromCollection(
  const geos::geom::GeometryCollection* collection);
//==============================================================================
} // ! PrimaryEntities
//==============================================================================
#endif //! PRIMARY_ENTITIES_GEOMETRY_CONVERTER_HPP
