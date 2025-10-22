#ifndef PRIMARY_ENTITIES_INNER_CONVERTER_HPP
#define PRIMARY_ENTITIES_INNER_CONVERTER_HPP
//==============================================================================
#include <PrimaryEntities/AtEntitiesType.hpp>
#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
#include <PrimaryEntities/PointConverter.hpp>
//==============================================================================
namespace PrimaryEntities
{
namespace Inner
{
//==============================================================================
/*!
 *\brief Преобразование из линии geos
 *\param[out] linestring   - линия
 *\param[in]  geosGeometry - линия geos
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template<typename PointType>
void fromGeosGeometry(PrimaryEntities::Linestring<PointType>& linestring,
                      const geos::geom::Geometry* geosGeometry)
{
  linestring.clear();

  // Проверим, что линия есть
  if (geosGeometry == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                 ": Empty pointer to linestring",
                                 "fromGeosGeometry");

  // Любые другие геометрии, кроме линии, записаны быть не могут
  if (geosGeometry->getGeometryTypeId() != geos::geom::GEOS_LINESTRING)
    throw PrimaryEntitiesFailure(IncorrectGeometryType,
                                 ": Geometry is not a linestring",
                                 "fromGeosGeometry");

  const geos::geom::LineString* geosLinestring
    = static_cast<const geos::geom::LineString*>(geosGeometry);

  for (size_t i = 0; i < geosLinestring->getNumPoints(); ++i)
  {
    std::unique_ptr<geos::geom::Point> geosPoint
      = geosLinestring->getPointN(i);
    linestring.push_back(fromGeosPoint<PointType>(geosPoint.get()));
  }
}
//==============================================================================
/*!
 *\brief Преобразование из мультилинии geos
 *\param[out] multiLinestring - мультилиния
 *\param[in]  geosGeometry    - мультилиния geos
 * \throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template<typename PointType>
void fromGeosGeometry(
  PrimaryEntities::MultiLinestring<PointType>& multiLinestring,
  const geos::geom::Geometry* geosGeometry)
{
  multiLinestring.clear();

  // Проверим, что мультилиния есть
  if (geosGeometry == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                 ": Empty pointer to multiLinestring",
                                 "fromGeosGeometry");

  // линию можем записать, как мультилинию
  if (geosGeometry->getGeometryTypeId() == geos::geom::GEOS_LINESTRING)
  {
    const geos::geom::LineString* geosLinestring
      = static_cast<const geos::geom::LineString*>(geosGeometry);
    PrimaryEntities::Linestring<PointType> linestring;
    fromGeosGeometry(linestring, geosLinestring);
    multiLinestring.push_back(linestring);
    return;
  }

  // Любые другие геометрии, кроме мультилинии, записаны быть не могут
  if (geosGeometry->getGeometryTypeId() != geos::geom::GEOS_MULTILINESTRING)
    throw PrimaryEntitiesFailure(IncorrectGeometryType,
                                 ": Geometry is not a multilinestring",
                                 "fromGeosGeometry");

  const geos::geom::MultiLineString* geosMultiLinestring
    = static_cast<const geos::geom::MultiLineString*>(geosGeometry);

  for (size_t i = 0; i < geosMultiLinestring->getNumGeometries(); ++i)
  {
    const geos::geom::LineString* geosLinestring
      = geosMultiLinestring->getGeometryN(i);
    PrimaryEntities::Linestring<PointType> linestring;
    fromGeosGeometry(linestring, geosLinestring);
    multiLinestring.push_back(linestring);
  }
}
//==============================================================================
/*!
 *\brief Преобразование из набора точек geos
 *\param[out] multiPoint   - набор точек
 *\param[in]  geosGeometry - набор точек geos
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template<typename PointType>
void fromGeosGeometry(
  PrimaryEntities::MultiPoint<PointType>& multiPoint,
  const geos::geom::Geometry* geosGeometry)
{
  multiPoint.clear();

  // Проверим, что набор точек есть
  if (geosGeometry == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                 ": Empty pointer to multiPoint",
                                 "fromGeosGeometry");

  // точку можем записать, как набор точек
  if (geosGeometry->getGeometryTypeId() == geos::geom::GEOS_POINT)
  {
    const geos::geom::Point* geosPoint
      = static_cast<const geos::geom::Point*>(geosGeometry);

    multiPoint.push_back(fromGeosPoint<PointType>(geosPoint));
    return;
  }

  // Любые другие геометрии, кроме набора точек, записаны быть не могут
  if (geosGeometry->getGeometryTypeId() != geos::geom::GEOS_MULTIPOINT)
    throw PrimaryEntitiesFailure(IncorrectGeometryType,
                                 ": Geometry is not a multipoint",
                                 "fromGeosGeometry");

  const geos::geom::MultiPoint* geosMultiPoint
    = static_cast<const geos::geom::MultiPoint*>(geosGeometry);

  for (size_t i = 0; i < geosMultiPoint->getNumPoints(); ++i)
  {
    const geos::geom::Point* geosPoint = geosMultiPoint->getGeometryN(i);
    multiPoint.push_back(fromGeosPoint<PointType>(geosPoint));
  }
}
//==============================================================================
/*!
 *\brief Преобразование из кольца geos
 *\details По факту работаем с полигоном geos вместо кольца, потому
 *         преобразование из полигона в наше кольцо также реализовано.
 *\param[out] ring         - кольцо
 *\param[in]  geosGeometry - кольцо geos
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template<typename PointType>
void fromGeosGeometry(PrimaryEntities::Ring<PointType>& ring,
                      const geos::geom::Geometry* geosGeometry)
{
  ring.clear();

  // Проверим, что кольцо есть
  if (geosGeometry == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                 ": Empty pointer to ring",
                                 "fromGeosGeometry");

  const geos::geom::LinearRing* geosRing;
  if (geosGeometry->getGeometryTypeId() == geos::geom::GEOS_POLYGON)
  {
    // Берём в качестве кольца внешнее кольцо полигона
    geosRing = static_cast<const geos::geom::Polygon*>(
      geosGeometry)->getExteriorRing();
  }
  else if (geosGeometry->getGeometryTypeId() == geos::geom::GEOS_LINEARRING)
  {
    geosRing = static_cast<const geos::geom::LinearRing*>(geosGeometry);
  }
  else
  {
    throw PrimaryEntitiesFailure(IncorrectGeometryType,
                                 ": Geometry is not a ring or polygon",
                                 "fromGeosGeometry");
  }

  for (size_t i = 0; i < geosRing->getNumPoints(); ++i)
  {
    std::unique_ptr<geos::geom::Point> geosPoint = geosRing->getPointN(i);
    ring.push_back(fromGeosPoint<PointType>(geosPoint.get()));
  }
}
//==============================================================================
/*!
 *\brief Преобразование из полигона geos
 *\param[out] polygon      - полигон
 *\param[in]  geosGeometry - полигон geos
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template<typename PointType>
void fromGeosGeometry(PrimaryEntities::Polygon<PointType>& polygon,
                      const geos::geom::Geometry* geosGeometry)
{
  polygon.clear();

  // Проверим, что полигон есть
  if (geosGeometry == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                 ": Empty pointer to polygon",
                                 "fromGeosGeometry");

  if (geosGeometry->getGeometryTypeId() != geos::geom::GEOS_POLYGON)
    throw PrimaryEntitiesFailure(IncorrectGeometryType,
                                 ": Geometry is not a polygon",
                                 "fromGeosGeometry");

  const geos::geom::Polygon* geosPolygon
    = static_cast<const geos::geom::Polygon*>(geosGeometry);

  PrimaryEntities::Ring<PointType> ring;
  fromGeosGeometry(ring, geosPolygon->getExteriorRing());
  polygon.outer() = ring;

  for (size_t i = 0; i < geosPolygon->getNumInteriorRing(); ++i)
  {
    fromGeosGeometry(ring, geosPolygon->getInteriorRingN(i));
    polygon.inners().push_back(ring);
  }
}
//==============================================================================
/*!
 *\brief Преобразование из мультиполигона geos
 *\param[out] multiPolygon - мультиполигон
 *\param[in]  geosGeometry - мультиполигон geos
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template<typename PointType>
void fromGeosGeometry(
  PrimaryEntities::MultiPolygon<PointType>& multiPolygon,
  const geos::geom::Geometry* geosGeometry)
{
  multiPolygon.clear();

  // Проверим, что мультиполигон есть
  if (geosGeometry == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                 ": Empty pointer to multiPolygon",
                                 "fromGeosGeometry");

  // Полигон можем записать, как мультиполигон
  if (geosGeometry->getGeometryTypeId() == geos::geom::GEOS_POLYGON)
  {
    const geos::geom::Polygon* geosPolygon
      = static_cast<const geos::geom::Polygon*>(geosGeometry);

    if (!geosPolygon->isEmpty())
    {
      PrimaryEntities::Polygon<PointType> polygon;
      fromGeosGeometry(polygon, geosPolygon);
      multiPolygon.push_back(polygon);
    }

    return;
  }

  // Любые другие геометрии, кроме мультиполигона, записаны быть не могут
  if (geosGeometry->getGeometryTypeId() != geos::geom::GEOS_MULTIPOLYGON)
    throw PrimaryEntitiesFailure(IncorrectGeometryType,
                                 ": Geometry is not a multipolygon",
                                 "fromGeosGeometry");

  const geos::geom::MultiPolygon* geosMultiPolygon
    = static_cast<const geos::geom::MultiPolygon*>(geosGeometry);

  for (size_t i = 0; i < geosMultiPolygon->getNumGeometries(); ++i)
  {
    PrimaryEntities::Polygon<PointType> polygon;
    fromGeosGeometry(polygon, geosMultiPolygon->getGeometryN(i));
    multiPolygon.push_back(polygon);
  }
}//==============================================================================
/*!
 *\brief Преобразование из коробки geos
 *\param[out] box     - коробка
 *\param[in]  geosBox - коробка geos
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template<typename PointType>
void fromGeosBox(PrimaryEntities::Box<PointType>& box,
                 const geos::geom::Envelope* geosBox)
{
  // Проверим, что коробка есть
  if (geosBox == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
      ": Empty pointer to box", "fromGeosBox");

  if (geosBox->isNull())
    throw PrimaryEntitiesFailure(IncorrectGeometryType,
      ": EmptyBox", "fromGeosBox");

  geos::geom::GeometryFactory::Ptr geometryFactory
    = geos::geom::GeometryFactory::create();

  std::unique_ptr<geos::geom::Point> geosPointFirst(
    geometryFactory->createPoint(
      geos::geom::CoordinateXY(geosBox->getMinX(), geosBox->getMinY())));
  std::unique_ptr<geos::geom::Point> geosPointSecond(
    geometryFactory->createPoint(
      geos::geom::CoordinateXY(geosBox->getMaxX(), geosBox->getMaxY())));

  box = PrimaryEntities::Box<PointType>(
    fromGeosPoint<PointType>(geosPointFirst.get()),
    fromGeosPoint<PointType>(geosPointSecond.get()));

}
//==============================================================================
/*!
 *\brief Преобразование из коробки geos
 *\details Коробка должна быть представлена geos полигоном или точкой.
 *         Проверка, что полигон действительно содержит коробку, не
 *         выполняется. Если полигон содержит произвольную геометрию, вернётся
 *         минимальная коробка, содержащая эту геометрию.
 *\param[out] box          - коробка
 *\param[in]  geosGeometry - коробка geos
 *\throws PrimaryEntitiesFailure # EmptyGeometryPtr || IncorrectGeometryType
 */
template<typename PointType>
void fromGeosGeometry(PrimaryEntities::Box<PointType>& box,
                      const geos::geom::Geometry* geosGeometry)
{
  // Проверим, что коробка есть
  if (geosGeometry == nullptr)
    throw PrimaryEntitiesFailure(EmptyGeometryPtr,
                                 ": Empty pointer to box",
                                 "fromGeosGeometry");

  if ((geosGeometry->getGeometryTypeId() != geos::geom::GEOS_POLYGON)
    && (geosGeometry->getGeometryTypeId() != geos::geom::GEOS_POINT))
    throw PrimaryEntitiesFailure(
      IncorrectGeometryType,
      ": Only polygon or point can be converted to box",
      "fromGeosGeometry");

  const geos::geom::Envelope* geosBox = geosGeometry->getEnvelopeInternal();
  fromGeosBox<PointType>(box, geosBox);
}
//==============================================================================
} // ! Inner
} // ! PrimaryEntities
//==============================================================================
#endif //! PRIMARY_ENTITIES_INNER_CONVERTER_HPP
