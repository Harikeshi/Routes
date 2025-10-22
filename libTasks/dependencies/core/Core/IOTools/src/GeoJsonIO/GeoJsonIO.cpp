#include <IOTools/GeoJsonIO/GeoJsonIO.hpp>

#include <IOTools/ByteConverting/ByteConverting.hpp>
//! nlohmann::json
#include <geos/vend/json.hpp>
//! std
#include <fstream>
#include <iomanip>
//==============================================================================
const std::string GEOMETRY_TAG("geometry");
const std::string TYPE_TAG("type");
const std::string PROPERTIES_TAG("properties");
const std::string HEIGHT_TAG("height");
const std::string COORDINATES_TAG("coordinates");
const std::string FEATURES_TAG("features");
//==============================================================================
/*!
 * \brief Чтение содержимого файла
 * \param[in] path - путь к файлу
 * \return текстовое содержимое файла при успешном открытии файла
 */
std::string readAllFromFile(const std::string& path)
{
  //!< Поток для чтения файла
  std::ifstream fileStream(path);
  if (!fileStream.is_open())
    return std::string {};
  std::stringstream stream;
  //! Читаем все содержимое
  stream << fileStream.rdbuf();
  return stream.str();
}
//==============================================================================
/*!
 * \brief Функция, считывающая точку из объекта JSON
 *
 * \param[in] vertex - JSON-объект c данными о точке (массив);
 *
 * \param[in] isNeedToConvertToRadians - необходимо ли переводить
 *                                       в радианы;
 * \return точка.
 */
template <typename Point_Type>
Point_Type getVertex(const nlohmann::json& vertex,
    bool isNeedToConvertToRadians)
{
  //!< Первая координата вершины
  double firstCoord = vertex[0];
  //!< Вторая координата вершины
  double secondCoord = vertex[1];

  if (isNeedToConvertToRadians)
  {
    firstCoord = Degree(firstCoord).toRadians().get();
    secondCoord = Degree(secondCoord).toRadians().get();
  }

  return ByteConverting::createPointFromDouble<Point_Type>(firstCoord,
                                                           secondCoord);
}
//==============================================================================
/*!
 * \brief Функция, считывающая полигон из JSON-объекта
 *
 * \details Функция только читает полигон. Она не проверяет корректность тэгов
 *          и не ищет среди них нужный.
 * \param[in] rings - JSON-объект (массив колец),
 *                    содержащий информацию о кольцах полигона;
 *
 * \param[in] isNeedToConvertToRadians - необходимо ли переводить
 *                                       в радианы;
 * \return полигон, если считывание прошло успешно;
 *         иначе -- объект по умолчанию.
 */
template <typename Point_Type>
PrimaryEntities::Polygon<Point_Type>
readPolygon(
  const nlohmann::json& rings,
  bool isNeedToConvertToRadians) noexcept
{
  PrimaryEntities::Polygon<Point_Type> result;
  try
  {
    //!< Количество колец многоугольника.
    std::size_t countOfRings(rings.size());

    //! Если кольца есть, добавляем, как минимум, внешнее кольцо
    if (countOfRings != 0)
    {
      //!< Объект, содержащий информацию о кольце многоугольника.
      nlohmann::json ring(rings[0]);
      //!< Количество вершин в кольце многоугольника.
      std::size_t countOfVertices(ring.size());
      result.outer().reserve(countOfVertices);

      //! Добавляем вершины в кольцо
      for (std::size_t vertexIndex(0);
      vertexIndex < countOfVertices;
        ++vertexIndex)
      {
        //!< Объект, содержащий информацию о вершине кольца
        nlohmann::json vertex(ring[vertexIndex]);
        result.outer().emplace_back(
          getVertex<Point_Type>(vertex, isNeedToConvertToRadians));
      }
      /*!
      * Если количество колец больше единицы,
      * добавляем также все внутренние кольца
      */
      if (countOfRings > 1)
      {
        result.inners().reserve(countOfRings - 1);
        //! Добавляем внутренние кольца в многоугольник
        for (std::size_t ringIndex(1);
        ringIndex < countOfRings;
          ++ringIndex)
        {
          ring = rings[ringIndex];
          countOfVertices = ring.size();
          //!< Очередное внутреннее кольцо многоугольника
          PrimaryEntities::Ring<Point_Type> innerRing;
          innerRing.resize(countOfVertices);

          //! Добавляем вершины во внутреннее кольцо
          for (std::size_t vertexIndex(0);
          vertexIndex < countOfVertices;
            ++vertexIndex)
          {
            //!< Объект, содержащий информацию о вершине кольца
            nlohmann::json vertex(ring[vertexIndex]);
            innerRing[vertexIndex]
              = getVertex<Point_Type>(vertex, isNeedToConvertToRadians);
          }

          result.inners().emplace_back(innerRing);
        }
      }
    }
  }
  catch (...)
  {
    return PrimaryEntities::Polygon<Point_Type>();
  }
  return result;
}
//==============================================================================
/*!
 * \brief Функция, считывающая полигон из JSON-объекта
 *
 * \param[in] json - JSON-объект c данными о полигоне;
 *
 * \param[in] isNeedToConvertToRadians - необходимо ли переводить
 *                                       в радианы;
 * \return полигон, если считывание прошло успешно;
 *         иначе -- объект по умолчанию.
 */
template <typename Point_Type>
PrimaryEntities::Polygon<Point_Type>
importPolygonFromJson(
  const nlohmann::json& json,
  bool isNeedToConvertToRadians) noexcept
{
  PrimaryEntities::Polygon<Point_Type> result;

  //!< Количество тэгов
  std::size_t featureTagsCount(json.size());
  if (featureTagsCount < 2)
    return result;

  //! Если не найден тэг типа, возвращаем объект по умолчанию
  if (!json.contains(TYPE_TAG))
    return result;

  //! Если тип - не "Feature", возвращаем объект по умолчанию
  if (json[TYPE_TAG] != "Feature")
    return result;

  //! Если не найден тэг геометрии, возвращаем объект по умолчанию
  if (!json.contains(GEOMETRY_TAG))
    return result;

  //!< Геометрия
  nlohmann::json geometry = json[GEOMETRY_TAG];

  //! Если в геометрии не найден тэг типа, возвращаем объект по умолчанию
  if (!geometry.contains(TYPE_TAG))
    return result;

  /*!
   * Если в геометрии значение типа -- не многоугольник,
   * возвращаем объект по умолчанию
   */
  if (geometry[TYPE_TAG] != "Polygon")
    return result;

  //! Если в геометрии не найден тэг координат, возвращаем объект по умолчанию
  if (!geometry.contains(COORDINATES_TAG))
    return result;

  return readPolygon<Point_Type>(geometry[COORDINATES_TAG],
      isNeedToConvertToRadians);
}
//==============================================================================
/*!
 * \brief Функция, проверяющая данные на соответствие формату .geojson
 *
 * \param[in] json - JSON-объект c данными о полигоне;
 *
 * \return true - если данные соответствуют формату;
 *         false - иначе.
 */
bool isGeojsonData(const nlohmann::json& json)
{
  //! Если не найден тэг типа, то это не .geojson
  if (!json.contains(TYPE_TAG))
    return false;
  //! Если значение типа не "FeatureCollection", то это не .geojson
  if (json[TYPE_TAG] != "FeatureCollection")
    return false;
  //! Если не найден тэг признаков, то это не .geojson
  if (!json.contains(FEATURES_TAG))
    return false;

  return true;
}
//==============================================================================
/*!
 * \brief Функция, считывающая полигон из .geojson-файла.
 *
 * \param[in] path -- путь к файлу, из которого нужно считывать.
 *
 * \param[in] isNeedToConvertToRadians - необходимо ли переводить
 *                                       в радианы;
 * \return полигон, если считывание прошло успешно;
 *         иначе -- объект по умолчанию.
 */
template <typename Point_Type>
PrimaryEntities::Polygon<Point_Type>
importPolygonFromGeoJson(
  const std::string& path, bool isNeedToConvertToRadians) noexcept
{
  //!< Искомый полигон
  PrimaryEntities::Polygon<Point_Type> result;

  //!< Текстовое содержимое файла
  std::string fileData = readAllFromFile(path);

  if (fileData.empty())
      return result;

  //!< Объект, содержащий  данные из .geojson-файла
  auto json = nlohmann::json::parse(fileData);

  try
  {
    result = importPolygonFromJson<Point_Type>(
      json, isNeedToConvertToRadians);
  }
  catch (...)
  {
    return PrimaryEntities::Polygon<Point_Type>();
  }
  return result;
}
//==============================================================================
GeoPolygon2D importGeoPolygonFromGeoJson(const std::string& path) noexcept
{
  return importPolygonFromGeoJson<GeoPoint2D>(path, true);
}
//==============================================================================
Polygon2D importPolygonFromGeoJson(const std::string& path) noexcept
{
  return importPolygonFromGeoJson<Point2D>(path, false);
}
//==============================================================================
/*!
 * \brief Функция, считывающая рамку из .geojson-файла.
 *
 * \param[in] path -- путь к файлу, из которого нужно считывать.
 *
 * \param[in] isNeedToConvertToRadians - необходимо ли переводить
 *                                       в радианы;
 * \return рамка, если считывание прошло успешно;
 *         иначе -- объект по умолчанию.
 */
template <typename Point_Type>
PrimaryEntities::Box<Point_Type>
importBoxFromGeoJson(
  const std::string& path, bool isNeedToConvertToRadians) noexcept
{
  //!< Полигон, прочитанный из .geojson-файла
  PrimaryEntities::Polygon<Point_Type> polygon
    = importPolygonFromGeoJson<Point_Type>(path, isNeedToConvertToRadians);

  //! Если не удалось прочитать полигон, возвращаем объект по умолчанию
  if (polygon.outer().empty())
    return PrimaryEntities::Box<Point_Type>();

  //!< Минимальный угол рамки
  Point_Type minCorner = polygon.outer()[0];
  //!< Максимальный угол рамки
  Point_Type maxCorner = polygon.outer()[2];
  //!< Искомая рамка
  PrimaryEntities::Box<Point_Type> box(minCorner, maxCorner);
  return box;
}
//==============================================================================
GeoBox2D importGeoBoxFromGeoJson(const std::string& path) noexcept
{
  return importBoxFromGeoJson<GeoPoint2D>(path, true);
}
//==============================================================================
Box2D importBoxFromGeoJson(const std::string& path) noexcept
{
  return importBoxFromGeoJson<Point2D>(path, false);
}
//==============================================================================
/*!
 * \brief Функция, определяющая тип читаемой геометрии.
 *
 * \details Функция проверяет, является ли читаемая геометрия мультиполигоном.
 *
 * \param[in] json - JSON-объект c данными.
 *
 * \return true - геометрия является мультиполигоном;
 *         false - геометрия не является мультиполигоном.
 */
bool isReadingMultiPolygon(const nlohmann::json& json)
{
  //! Если не найден тэг геометрии - тип геометрии не определить
  if (!json.contains(GEOMETRY_TAG))
    return false;

  //!< Геометрия
  nlohmann::json geometry(json[GEOMETRY_TAG]);

  return geometry[TYPE_TAG] == "MultiPolygon";
}
//==============================================================================
/*!
 * \brief Функция, считывающая мультиполигон из JSON-объекта
 *
 * \param[in] json - JSON-объект c данными о мультиполигоне;
 *
 * \param[in] isNeedToConvertToRadians - необходимо ли переводить
 *                                       в радианы;
 * \return мультиполигон, если считывание прошло успешно;
 *         иначе -- объект по умолчанию.
 */
template <typename Point_Type>
PrimaryEntities::MultiPolygon<Point_Type>
importMultiPolygonFromJson(
  const nlohmann::json& json,
  bool isNeedToConvertToRadians) noexcept
{
  PrimaryEntities::MultiPolygon<Point_Type> result;

  //!< Количество тэгов
  std::size_t featureTagsCount(json.size());
  if (featureTagsCount < 2)
    return result;

  //! Если не найден тэг типа, возвращаем объект по умолчанию
  if (!json.contains(TYPE_TAG))
    return result;

  //! Если тип - не "Feature", возвращаем объект по умолчанию
  if (json[TYPE_TAG] != "Feature")
    return result;

  //! Если не найден тэг геометрии, возвращаем объект по умолчанию
  if (!json.contains(GEOMETRY_TAG))
    return result;

  //!< Геометрия
  nlohmann::json geometry(json[GEOMETRY_TAG]);

  //! Если в геометрии не найден тэг типа, возвращаем объект по умолчанию
  if (!geometry.contains(TYPE_TAG))
    return result;

  /*!
   * Если в геометрии значение типа -- не мультиполигон,
   * возвращаем объект по умолчанию
   */
  if (geometry[TYPE_TAG] != "MultiPolygon")
    return result;

  //! Если в геометрии не найден тэг координат, возвращаем объект по умолчанию
  if (!geometry.contains(COORDINATES_TAG))
    return result;

  //!< Объект, содержащий информацию о полигонах мультиполигона
  nlohmann::json polygons(geometry[COORDINATES_TAG]);
  //!< Количество полигонов
  std::size_t countOfPolygons(polygons.size());

  //! Если полигоны есть, добавляем их
  for (std::size_t polygonsIndex(0);
  polygonsIndex < countOfPolygons;
    ++polygonsIndex)
  {
    result.push_back(readPolygon<Point_Type>(
      polygons[polygonsIndex], isNeedToConvertToRadians));
  }

  return result;
}
//==============================================================================
/*!
 * \brief Функция, считывающая множество многоугольников
 *  из .geojson-файла.
 *
 * \param[in] path -- путь к файлу, из которого нужно считывать.
 *
 * \param[in] isNeedToConvertToRadians - необходимо ли переводить
 *                                       в радианы;
 * \return искомое множество многоугольников, если считывание
 *         прошло успешно;
 *         иначе -- объект по умолчанию.
 */
template <typename Point_Type>
PrimaryEntities::MultiPolygon<Point_Type>
importMultiPolygonFromGeoJson(
  const std::string& path, bool isNeedToConvertToRadians) noexcept
{
  //!< Искомое множество многоугольников.
  PrimaryEntities::MultiPolygon<Point_Type> result;

  //!< Текстовое содержимое файла
  std::string fileData = readAllFromFile(path);

  if (fileData.empty())
      return result;

  //!< Объект, содержащий  данные из .geojson-файла
  auto json = nlohmann::json::parse(fileData);

  try
  {
    //! Проверяем, похож ли этот файл на .geojson.
    if (!isGeojsonData(json))
      return result;

    //!< Объект, содержащий данные о множестве геометрий.
    nlohmann::json features(json[FEATURES_TAG]);
    //!< Количество геометрий.
    std::size_t countOfFeatures(features.size());

    if (countOfFeatures == 0)
      return result;

    if (isReadingMultiPolygon(features[0]))
    {
      //!< Свойство.
      nlohmann::json feature(features[0]);
      result = importMultiPolygonFromJson<Point_Type>(
        feature, isNeedToConvertToRadians);
    }
    else
    {
      result.reserve(countOfFeatures);

      //! Добавляем в множество многоугольников все найденные
      //! многоугольники.
      for (std::size_t i(0); i < countOfFeatures; ++i)
      {
        //!< Свойство.
        nlohmann::json feature(features[i]);

        //!< Многоугольник, читаемый из геометрии
        PrimaryEntities::Polygon<Point_Type> polygon
          = importPolygonFromJson<Point_Type>(
            feature, isNeedToConvertToRadians);
        result.emplace_back(polygon);
      }
    }
  }
  //! Если что-то пошло не так, возвращаем объект по умолчанию.
  catch (...)
  {
    return PrimaryEntities::MultiPolygon<Point_Type>();
  }
  return result;
}
//==============================================================================
GeoMultiPolygon2D importGeoMultiPolygonFromGeoJson(
  const std::string& path) noexcept
{
  return importMultiPolygonFromGeoJson<GeoPoint2D>(path, true);
}
//==============================================================================
MultiPolygon2D importMultiPolygonFromGeoJson(const std::string& path) noexcept
{
  return importMultiPolygonFromGeoJson<Point2D>(path, false);
}
//==============================================================================
/*!
 * \brief Функция, считывающая множество точек из .geojson-файла.
 *
 * \param[in] path -- путь к файлу, из которого нужно считывать.
 *
 * \param[in] isNeedToConvertToRadians - необходимо ли переводить
 *                                       в радианы;
 * \return искомое множество точек, если считывание прошло успешно;
 *         иначе -- объект по умолчанию.
 */
template <typename Point_Type>
PrimaryEntities::MultiPoint<Point_Type>
importMultiPointFromGeoJson(
  const std::string& path, bool isNeedToConvertToRadians) noexcept
{
  //!< Искомое множество точек
  PrimaryEntities::MultiPoint<Point_Type> result;

  //!< Текстовое содержимое файла
  std::string fileData = readAllFromFile(path);

  if (fileData.empty())
      return result;

  //!< Объект, содержащий  данные из .geojson-файла
  auto json = nlohmann::json::parse(fileData);

  try
  {
    //! Проверяем, похож ли этот файл на .geojson.
    if (!isGeojsonData(json))
      return result;

    //!< Объект, содержащий данные о множестве геометрий.
    nlohmann::json features(json[FEATURES_TAG]);
    //!< Количество геометрий.
    std::size_t countOfFeatures(features.size());
    result.resize(countOfFeatures);

    //! Добавляем в множество точек все найденные точки.
    for (std::size_t pointIndex(0); pointIndex < countOfFeatures; ++pointIndex)
    {
      //!< Свойство
      nlohmann::json feature(features[pointIndex]);
      //!< Количество тэгов, содержащихся в свойстве.
      std::size_t featureTagsCount(feature.size());

      if (featureTagsCount < 2)
        continue;

      //! Если не найден тэг типа, переходим к следующему свойству.
      if (!feature.contains(TYPE_TAG))
        continue;

      //! Если тип - не "Feature", переходим к следующему свойству
      if (feature[TYPE_TAG] != "Feature")
        continue;

      //! Если не найден тэг геометрии, переходим к следующему свойству
      if (!feature.contains(GEOMETRY_TAG))
        continue;

      //!< Геометрия.
      nlohmann::json geometry(feature[GEOMETRY_TAG]);

      //! Если в геометрии не найден тэг типа, экстренно завершаем работу
      if (!geometry.contains(TYPE_TAG))
        return PrimaryEntities::MultiPoint<Point_Type>();

      //! Если в геометрии значение типа -- не точка, экстренно завершаем работу
      if (geometry[TYPE_TAG] != "Point")
        return PrimaryEntities::MultiPoint<Point_Type>();

      //! Если в геометрии не найден тэг координат, экстренно завершаем работу.
      if (!geometry.contains(COORDINATES_TAG))
        return PrimaryEntities::MultiPoint<Point_Type>();

      //!< Объект, содержащий информацию о координатах точки
      nlohmann::json vertex(geometry[COORDINATES_TAG]);
      result[pointIndex]
        = getVertex<Point_Type>(vertex, isNeedToConvertToRadians);
    }
  }
  //! Если что-то пошло не так, возвращаем объект по умолчанию
  catch (...)
  {
    return PrimaryEntities::MultiPoint<Point_Type>();
  }
  return result;
}
//==============================================================================
GeoMultiPoint2D importGeoMultiPointFromGeoJson(
  const std::string& path) noexcept
{
  return importMultiPointFromGeoJson<GeoPoint2D>(path, true);
}
//==============================================================================
MultiPoint2D importMultiPointFromGeoJson(const std::string& path) noexcept
{
  return importMultiPointFromGeoJson<Point2D>(path, false);
}
//==============================================================================
/*!
* \brief Функция, считывающая ломаную линию из JSON-объекта
*
* \param[in] json - JSON-объект c данными о ломаной линии;
*
* \param[in] isNeedToConvertToRadians - необходимо ли переводить
*                                       в радианы;
* \return ломаная линия, если считывание прошло успешно;
*         иначе -- объект по умолчанию.
*/
template <typename Point_Type>
PrimaryEntities::Linestring<Point_Type>
importLinestringFromJson(
  const nlohmann::json& json,
  bool isNeedToConvertToRadians) noexcept
{
  PrimaryEntities::Linestring<Point_Type> result;

  //!< Количество тэгов, содержащихся в свойстве.
  std::size_t featureTagsCount(json.size());

  if (featureTagsCount < 2)
    return result;

  //! Если не найден тэг типа, завершаем работу.
  if (!json.contains(TYPE_TAG))
    return result;

  //! Если тип - не "Feature", завершаем работу.
  if (json[TYPE_TAG] != "Feature")
    return result;

  //! Если не найден тэг геометрии, завершаем работу.
  if (!json.contains(GEOMETRY_TAG))
    return result;

  //!< Геометрия.
  nlohmann::json geometry(json[GEOMETRY_TAG]);

  //! Если в геометрии не найден тэг типа, завершаем работу.
  if (!geometry.contains(TYPE_TAG))
    return result;

  //! Если в геометрии значение типа -- не ломаная линия, завершаем работу.
  if (geometry[TYPE_TAG] != "LineString")
    return result;

  //! Если в геометрии не найден тэг координат, завершаем работу.
  if (!geometry.contains(COORDINATES_TAG))
    return result;

  //!< Объект с коодинатами ломаной линии
  nlohmann::json linestring(geometry[COORDINATES_TAG]);
  //!< Количество точек ломаной.
  std::size_t countOfPoints(linestring.size());
  result.reserve(countOfPoints);

  //! Добавляем вершины в ломаную линию
  for (std::size_t pointIndex(0);
  pointIndex < countOfPoints;
    ++pointIndex)
  {
    //!< Объект, содержащий информацию о точке ломаной
    nlohmann::json point(linestring[pointIndex]);
    result.emplace_back(
      getVertex<Point_Type>(point, isNeedToConvertToRadians));
  }

  return result;
}
//==============================================================================
/*!
* \brief Функция, считывающая ломаную линию из .geojson-файла.
*
* \param[in] path -- путь к файлу, из которого нужно считывать.
*
* \param[in] isNeedToConvertToRadians - необходимо ли переводить
*                                       в радианы;
* \return искомое множество точек, если считывание прошло успешно;
*         иначе -- объект по умолчанию.
*/
template <typename Point_Type>
PrimaryEntities::Linestring<Point_Type>
importLinestringFromGeoJson(
  const std::string& path, bool isNeedToConvertToRadians) noexcept
{
  //!< Искомая ломаная линия
  PrimaryEntities::Linestring<Point_Type> result;

  //!< Текстовое содержимое файла
  std::string fileData = readAllFromFile(path);

  if (fileData.empty())
      return result;

  //!< Объект, содержащий  данные из .geojson-файла
  auto json = nlohmann::json::parse(fileData);

  try
  {
    //! Проверяем, похож ли этот файл на .geojson.
    if (!isGeojsonData(json))
      return result;

    //!< JSON-массив, содержащий данные о множестве геометрий.
    nlohmann::json features(json[FEATURES_TAG]);

    //! JSON-массив состоит только из одного элемента
    result = importLinestringFromJson<Point_Type>(
      features[0], isNeedToConvertToRadians);
  }
  //! Если что-то пошло не так, возвращаем объект по умолчанию
  catch (...)
  {
    return PrimaryEntities::Linestring<Point_Type>();
  }

  return result;
}
//==============================================================================
/*!
* \brief Функция, определяющая тип читаемой геометрии.
*
* \details Функция проверяет, является ли читаемая геометрия мультилинией.
*
* \param[in] json - JSON-объект c данными.
*
* \return true - геометрия является мультилинией;
*         false - геометрия не является мультилинией.
*/
bool isReadingMultiLineString(const nlohmann::json& json)
{
  //! Если не найден тэг геометрии - тип геометрии не определить
  if (!json.contains(GEOMETRY_TAG))
    return false;

  //!< Геометрия
  nlohmann::json geometry(json[GEOMETRY_TAG]);

  return (geometry[TYPE_TAG] == "MultiLineString");
}
//==============================================================================
/*!
* \brief Функция, считывающая мультилинию из JSON-объекта
*
* \param[in] json - JSON-объект c данными о мультилинии;
*
* \param[in] isNeedToConvertToRadians - необходимо ли переводить
*                                       в радианы;
* \return мультилиния, если считывание прошло успешно;
*         иначе -- объект по умолчанию.
*/
template <typename Point_Type>
PrimaryEntities::MultiLinestring<Point_Type>
importMultiLinestringFromJson(
  const nlohmann::json& json,
  bool isNeedToConvertToRadians) noexcept
{
  PrimaryEntities::MultiLinestring<Point_Type> result;

  //!< Количество тэгов.
  std::size_t featureTagsCount(json.size());

  if (featureTagsCount < 2)
    return result;

  //! Если не найден тэг типа, завершаем работу.
  if (!json.contains(TYPE_TAG))
    return result;

  //! Если тип - не "Feature", завершаем работу.
  if (json[TYPE_TAG] != "Feature")
    return result;

  //! Если не найден тэг геометрии, завершаем работу.
  if (!json.contains(GEOMETRY_TAG))
    return result;

  //!< Геометрия.
  nlohmann::json geometry(json[GEOMETRY_TAG]);

  //! Если в геометрии не найден тэг типа, завершаем работу.
  if (!geometry.contains(TYPE_TAG))
    return result;

  //! Если в геометрии значение типа -- не мультилиния, завершаем работу.
  if (geometry[TYPE_TAG] != "MultiLineString")
    return result;

  //! Если в геометрии не найден тэг координат, завершаем работу.
  if (!geometry.contains(COORDINATES_TAG))
    return result;

  //!< Объект, содержащий информацию о мультилинии
  nlohmann::json multilinestring(geometry[COORDINATES_TAG]);
  //!< Количество ломаных линий
  std::size_t countOfLines(multilinestring.size());

  //! Если ломаные есть, добавляем их
  for (std::size_t lineIndex(0);
  lineIndex < countOfLines;
    ++lineIndex)
  {
    //!< Объект ломаной линии
    nlohmann::json linestring(multilinestring[lineIndex]);
    //!< Число точек в ломаной линии
    std::size_t countOfPoints(linestring.size());

    PrimaryEntities::Linestring<Point_Type> curLine;

    //! Добавляем вершины ломаной
    for (std::size_t pointIndex(0);
    pointIndex < countOfPoints;
      ++pointIndex)
    {
      //!< Объект, содержащий информацию о точке ломаной
      nlohmann::json point(linestring[pointIndex]);
      curLine.emplace_back(
        getVertex<Point_Type>(point, isNeedToConvertToRadians));
    }
    result.push_back(curLine);
  }

  return result;
}
//==============================================================================
/*!
* \brief Функция, считывающая набор ломаных линий из .geojson-файла.
*
* \param[in] path -- путь к файлу, из которого нужно считывать.
*
* \param[in] isNeedToConvertToRadians - необходимо ли переводить
*                                       в радианы;
* \return искомый набор ломаных линий, если считывание прошло успешно;
*         иначе -- объект по умолчанию.
*/
template <typename Point_Type>
PrimaryEntities::MultiLinestring<Point_Type>
importMultiLinestringFromGeoJson(
  const std::string& path, bool isNeedToConvertToRadians) noexcept
{
  //!< Искомый набор ломаных линий
  PrimaryEntities::MultiLinestring<Point_Type> result;

  //!< Текстовое содержимое файла
  std::string fileData = readAllFromFile(path);

  if (fileData.empty())
      return result;

  //!< Объект, содержащий  данные из .geojson-файла
  auto json = nlohmann::json::parse(fileData);

  try
  {
    //! Проверяем, похож ли этот файл на .geojson.
    if (!isGeojsonData(json))
      return result;

    //!< Объект, содержащий данные о множестве геометрий.
    nlohmann::json features(json[FEATURES_TAG]);

    //!< Количество геометрий.
    std::size_t countOfFeatures(features.size());

    if (!countOfFeatures)
      return result;

    if (isReadingMultiLineString(features[0]))
    {
      result = importMultiLinestringFromJson<Point_Type>(
        features[0], isNeedToConvertToRadians);
    }
    else
    {
      result.reserve(countOfFeatures);
      for (std::size_t i(0); i < countOfFeatures; ++i)
      {
        result.push_back(importLinestringFromJson<Point_Type>(
          features[i], isNeedToConvertToRadians));
      }
    }
  }
  //! Если что-то пошло не так, возвращаем объект по умолчанию
  catch (...)
  {
    return PrimaryEntities::MultiLinestring<Point_Type>();
  }

  return result;
}
//==============================================================================
GeoLinestring2D importGeoLinestringFromGeoJson(
  const std::string& path) noexcept
{
  return importLinestringFromGeoJson<GeoPoint2D>(path, true);
}
//==============================================================================
Linestring2D importLinestringFromGeoJson(const std::string& path) noexcept
{
  return importLinestringFromGeoJson<Point2D>(path, false);
}
//==============================================================================
GeoMultiLinestring2D importGeoMultiLinestringFromGeoJson(
  const std::string& path) noexcept
{
  return importMultiLinestringFromGeoJson<GeoPoint2D>(path, true);
}
//==============================================================================
MultiLinestring2D importMultiLinestringFromGeoJson(
  const std::string& path) noexcept
{
  return importMultiLinestringFromGeoJson<Point2D>(path, false);
}
//==============================================================================
/*!
 * \brief Функция, считывающая множество многоугольников с высотами
 *        из .geojson-файла.
 *
 * \param[in] path -- путь к файлу, из которого нужно считывать.
 *
 * \param[in] isNeedToConvertToRadians - необходимо ли переводить
 *                                       в радианы;
 * \return искомое множество многоугольников с высотами, если
 *         считывание прошло успешно;
 *         иначе -- объект по умолчанию.
 */
template <typename Point_Type>
std::vector<std::pair<PrimaryEntities::Polygon<Point_Type>, double>>
importPolygonsWithHeightFromGeoJson(
  const std::string& path, bool isNeedToConvertToRadians) noexcept
{
  //!< Искомое множество многоугольников с высотами
  std::vector<std::pair<PrimaryEntities::Polygon<Point_Type>, double>> result;

  //!< Текстовое содержимое файла
  std::string fileData = readAllFromFile(path);

  if (fileData.empty())
    return result;

  //!< Объект, содержащий  данные из .geojson-файла
  auto json = nlohmann::json::parse(fileData);

  try
  {
    //! Проверяем, похож ли этот файл на .geojson.
    if (!isGeojsonData(json))
      return result;

    //!< Объект, содержащий данные о множестве геометрий.
    nlohmann::json features(json[FEATURES_TAG]);

    //!< Количество геометрий.
    std::size_t countOfFeatures(features.size());
    result.reserve(countOfFeatures);

    //! Добавляем в результат все найденные многоугольники с высотами
    for(std::size_t i(0); i < countOfFeatures; ++i)
    {
      //!< Свойство.
      nlohmann::json feature(features[i]);
      //!< Количество тэгов, содержащихся в свойстве.
      std::size_t featureTagsCount(feature.size());

      //! Число тэгов должно быть не менее 3 (type + properties + geometry)
      if (featureTagsCount < 3)
        continue;

      //! Если не найден тэг типа, переходим к следующему свойству
      if (!feature.contains(TYPE_TAG))
        continue;

      //! Если тип -- не "Feature", переходим к следующему свойству.
      if (feature[TYPE_TAG] != "Feature")
        continue;

      //! Если не найден тэг свойств, переходим к следующему свойству
      if (!feature.contains(PROPERTIES_TAG))
        continue;

      //! Свойства
      nlohmann::json properties(feature[PROPERTIES_TAG]);
      //!< Число свойств
      std::size_t propertiesChildCount(properties.size());

      //! Проверяем, что имеется хотя бы одно свойство
      if (propertiesChildCount == 0)
        continue;

      //! Если не найден тэг высоты переходим к следующему свойству
      if (!properties.contains(HEIGHT_TAG))
        continue;

      //! Получаем значение высоты
      double height(properties[HEIGHT_TAG]);

      //! Если не найден тэг геометрии, переходим к следующему свойству.
      if (!feature.contains(GEOMETRY_TAG))
        continue;

      //!< Геометрия.
      nlohmann::json geometry(feature[GEOMETRY_TAG]);

      //! Если в геометрии не найден тэг типа, экстренно завершаем работу.
      if (!geometry.contains(TYPE_TAG))
        return std::vector<std::pair<
          PrimaryEntities::Polygon<Point_Type>, double>>();

      /*!
       * Если в геометрии значение типа -- не многоугольник,
       * экстренно завершаем работу.
       */
      if (geometry[TYPE_TAG] != "Polygon")
        return std::vector<std::pair<
          PrimaryEntities::Polygon<Point_Type>, double>>();

      //! Если в геометрии не найден тэг координат, экстренно завершаем работу.
      if (!geometry.contains(COORDINATES_TAG))
        return std::vector<std::pair<
          PrimaryEntities::Polygon<Point_Type>, double>>();

      //!< Многоугольник, читаемый из геометрии.
      PrimaryEntities::Polygon<Point_Type> polygon;
      //!< Объект, содержащий информацию о кольцах многоугольника.
      nlohmann::json rings(geometry[COORDINATES_TAG]);
      //!< Количество колец многоугольника.
      std::size_t countOfRings(rings.size());

      //! Если кольца есть, добавляем, как минимум, внешнее кольцо.
      if (countOfRings != 0)
      {
        //!< Объект, содержащий информацию о кольце многоугольника.
        nlohmann::json ring(rings[0]);
        //!< Количество вершин в кольце многоугольника.
        std::size_t countOfVertices(ring.size());
        polygon.outer().reserve(countOfVertices);

        //! Добавляем вершины в кольцо.
        for (std::size_t vertexIndex(0);
             vertexIndex < countOfVertices;
             ++vertexIndex)
        {
          //!< Объект, содержащий информацию о вершине кольца.
          nlohmann::json vertex(ring[vertexIndex]);
          polygon.outer().emplace_back(
            getVertex<Point_Type>(vertex, isNeedToConvertToRadians));
        }

        /*!
         * Если количество колец больше единицы,
         * добавляем также все внутренние кольца.
         */
        if (countOfRings > 1)
        {
          polygon.inners().reserve(countOfRings - 1);

          //! Добавляем внутренние кольца в многоугольник.
          for (std::size_t ringIndex(1);
               ringIndex < countOfRings;
               ++ringIndex)
          {
            ring = rings[ringIndex];
            countOfVertices = ring.size();

            //!< Очередное внутреннее кольцо многоугольника.
            PrimaryEntities::Ring<Point_Type> innerRing;
            innerRing.resize(countOfVertices);

            //! Добаляем вершины во внутреннее кольцо.
            for (std::size_t vertexIndex(0);
                 vertexIndex < countOfVertices;
                 ++vertexIndex)
            {
              //!< Объект, содержащий информацию о вершине кольца.
              nlohmann::json vertex(ring[vertexIndex]);
              innerRing[vertexIndex]
                = getVertex<Point_Type>(vertex, isNeedToConvertToRadians);
            }

            polygon.inners().emplace_back(innerRing);
          }
        }
      }
      result.push_back({polygon, height});
    }
  }

  //! Если что-то пошло не так, возвращаем объект по умолчанию.
  catch (...)
  {
    return std::vector<std::pair<
      PrimaryEntities::Polygon<Point_Type>, double>>();
  }
  return result;
}
//==============================================================================
std::vector<std::pair<GeoPolygon2D, double>>
importGeoPolygonsWithHeightFromGeoJson(
  const std::string& path) noexcept
{
  return importPolygonsWithHeightFromGeoJson<GeoPoint2D>(path, true);
}
//==============================================================================
std::vector<std::pair<Polygon2D, double>>
importPolygonsWithHeightFromGeoJson(
  const std::string& path) noexcept
{
  return importPolygonsWithHeightFromGeoJson<Point2D>(path, false);
}
//==============================================================================
/*!
* \brief Запись информации о системе координат в JSON-объект
*
* \param[in/out] json - JSON-объект для записи
*
* \param[in] isProjection - true - если записывается геометрия на
*                           плоскости, иначе false.
*/
void exportCoordinateSystem(
  nlohmann::json& json,
  bool isProjection) noexcept
{
  //!< Объект с информацией о системе координат
  nlohmann::json crs;
  crs[TYPE_TAG] = std::string("name");
  //!< Система координат
  std::string coordinateSystem;
  if (isProjection)
    coordinateSystem = "3857"; // Pseudo-Mercator
  else
    coordinateSystem = "4326"; // WGS84
  //!< Объект свойств для системы коодинат
  nlohmann::json properties;
  properties["name"] = std::string("urn:ogc:def:crs:EPSG::" +
                                   coordinateSystem);
  crs[PROPERTIES_TAG] = std::move(properties);
  json["crs"] = std::move(crs);
}
//==============================================================================
/*!
* \brief Формирование JSON-объекта для точки
*
* \param[in] point - точка,
*
* \param[in] isNeedToConvertToDegrees - необходимо ли переводить
*                                       в градусы.
* \return JSON-объект для точки (массив с координатами X и Y)
*/
template <typename Point_Type>
nlohmann::json toJsonArray(
  const Point_Type& point,
  bool isNeedToConvertToDegrees) noexcept
{
  //! Получение координат точки (для географической системы
  //! координат в градусах)
  //!< Значение первой координаты точки.
  double firstCoord(point.getCoordinates().x);
  //!< Значение второй координаты точки.
  double secondCoord(point.getCoordinates().y);

  if (isNeedToConvertToDegrees)
  {
    firstCoord = Radian(firstCoord).toDegrees().get();
    secondCoord = Radian(secondCoord).toDegrees().get();
  }

  //!< JSON-массив для записи двух координат точки
  nlohmann::json jsonArray = nlohmann::json::array();

  //!< Первая координата точки
  jsonArray.push_back(firstCoord);

  //!< Вторая координата точки
  jsonArray.push_back(secondCoord);

  return jsonArray;
}
//==============================================================================
/*!
 * \brief Запись множества точек в .geojson-файл.
 *
 * \details Координаты точек в файл пишутся без кавычек.
 *
 * \param[in] path - путь к файлу, в который нужно записывать,
 *
 * \param[in] multiPoint - множество точек,
 *
 * \param[in] writeEmptyGeom - записывать ли пустое множество точек,
 *
 * \param[in] isNeedToConvertToDegrees - необходимо ли переводить
 *                                       в градусы.
 * \return true, если запись прошла успешно;
 *         false - иначе.
 */
template <typename Point_Type>
bool exportToGeoJson(
  const std::string& path,
  const PrimaryEntities::MultiPoint<Point_Type>& multiPoint,
  bool writeEmptyGeom,
  bool isNeedToConvertToDegrees) noexcept
{
  //! Если не хотим записывать файл для пустой геометрии, пропускаем
  if ((!writeEmptyGeom) && multiPoint.empty())
    return true;

  //!< Файловый поток для сериализации данных в .geojson.
  std::fstream output(path, std::ios_base::out | std::ios_base::trunc);
  if (!output.is_open())
    return false;

  try
  {
    //!< JSON-объект для последующей записи в файл
    nlohmann::json json;
    json[TYPE_TAG] = std::string("FeatureCollection");
    
    //! Записываем информацию о системе координат
    exportCoordinateSystem(json, !isNeedToConvertToDegrees);

    //!< JSON-массив свойств
    nlohmann::json features = nlohmann::json::array();

    //! Проходим по точкам
    for (const auto& point : multiPoint)
    {
      //!< JSON-объект свойств
      nlohmann::json feature;
      feature[TYPE_TAG] = std::string("Feature");
      //!< JSON-объект геометрии
      nlohmann::json geometry;
      geometry[TYPE_TAG] = std::string("Point");
      geometry[COORDINATES_TAG] = toJsonArray(point, isNeedToConvertToDegrees);

      feature[GEOMETRY_TAG] = std::move(geometry);

      features.push_back(feature);
    }

    json[FEATURES_TAG] = std::move(features);
    //! Сериализация JSON в файловый поток
    output << json;
  }
  catch (...)
  {
    return false;
  }
  return true;
}
//==============================================================================
bool exportToGeoJson(
  const std::string& path,
  const GeoMultiPoint2D& multiPoint,
  bool writeEmptyGeom/* = true*/) noexcept
{
  if (!exportToGeoJson(path, multiPoint, writeEmptyGeom, true))
    return false;

  return true;
}
//==============================================================================
bool exportToGeoJson(
  const std::string& path,
  const MultiPoint2D& multiPoint,
  bool writeEmptyGeom/* = true*/) noexcept
{
  if (!exportToGeoJson(path, multiPoint, writeEmptyGeom, false))
    return false;

  return true;
}
//==============================================================================
/*!
* \brief Запись ломаной линии в JSON-объект
*
* \param[in/out] json - JSON-объект для записи данных
*
* \param[in] linestring - ломаная линия,
*
* \param[in] isNeedToConvertToDegrees - необходимо ли переводить
*                                       в градусы.
*/
template <typename Point_Type>
bool exportToGeoJson(
  nlohmann::json& json,
  const PrimaryEntities::Linestring<Point_Type>& linestring,
  bool isNeedToConvertToDegrees) noexcept
{
  try
  {
    json[TYPE_TAG] = std::string("Feature");
    //!< JSON-объект геометрии
    nlohmann::json geometry;
    geometry[TYPE_TAG] = std::string("LineString");
    //!< JSON-массив координат
    nlohmann::json coordinates = nlohmann::json::array();

    //! Проходим по точкам
    for (const auto& point : linestring)
    {
      //! Запись точки
      coordinates.push_back(toJsonArray(point, isNeedToConvertToDegrees));
    }

    geometry[COORDINATES_TAG] = std::move(coordinates);

    json[GEOMETRY_TAG] = std::move(geometry);
  }
  catch (...)
  {
    return false;
  }
  return true;
}
//==============================================================================
/*!
 * \brief Запись ломаной линии в .geojson-файл.
 *
 * \details Координаты точек в файл пишутся без кавычек.
 *
 * \param[in] path - путь к файлу, в который нужно записывать,
 *
 * \param[in] linestring - ломаная линия,
 *
 * \param[in] writeEmptyGeom - записывать ли пустую ломаную,
 *
 * \param[in] isNeedToConvertToDegrees - необходимо ли переводить
 *                                       в градусы.
 * \return true, если запись прошла успешно;
 *         false - иначе.
 */
template <typename Point_Type>
bool exportToGeoJson(
  const std::string& path,
  const PrimaryEntities::Linestring<Point_Type>& linestring,
  bool writeEmptyGeom,
  bool isNeedToConvertToDegrees) noexcept
{
  //! Если не хотим записывать файл для пустой геометрии, пропускаем
  if ((!writeEmptyGeom) && linestring.empty())
    return true;

  //!< Файловый поток для сериализации данных в .geojson.
  std::fstream output(path, std::ios_base::out | std::ios_base::trunc);
  if (!output.is_open())
    return false;

  try
  {
    //!< JSON-объект для последующей сериализации в файл
    nlohmann::json json;
    json[TYPE_TAG] = std::string("FeatureCollection");

    //! Записываем информацию о системе координат
    exportCoordinateSystem(json, !isNeedToConvertToDegrees);

    //!< JSON-объект для признака
    nlohmann::json feature;

    //! Записи ломаной линии в JSON-объект
    exportToGeoJson<Point_Type>(feature, linestring, isNeedToConvertToDegrees);

    //!< Записываем в массив свотств единственный объект со свойством
    json[FEATURES_TAG] = nlohmann::json::array();
    json[FEATURES_TAG].push_back(feature);
    //! Сериализация JSON-объекта в файловый поток
    output << json;
  }
  catch (...)
  {
    return false;
  }
  return true;
}
//==============================================================================
/*!
 * \brief Запись набора ломаных линий в .geojson-файл.
 *
 * \details Координаты точек в файл пишутся без кавычек.
 *
 * \param[in] path - путь к файлу, в который нужно записывать,
 *
 * \param[in] linestring - набор ломаных линий,
 *
 * \param[in] writeEmptyGeom - записывать ли пустое ломаных,
 *
 * \param[in] isNeedToConvertToDegrees - необходимо ли переводить
 *                                       в градусы.
 * \return true, если запись прошла успешно;
 *         false - иначе.
 */
template <typename Point_Type>
bool exportToGeoJson(
  const std::string& path,
  const PrimaryEntities::MultiLinestring<Point_Type>& linestring,
  bool writeEmptyGeom,
  bool isNeedToConvertToDegrees) noexcept
{
  //! Если не хотим записывать файл для пустой геометрии, пропускаем
  if ((!writeEmptyGeom) && linestring.empty())
    return true;

  //!< Файловый поток для сериализации данных в .geojson.
  std::fstream output(path, std::ios_base::out | std::ios_base::trunc);
  if (!output.is_open())
    return false;

  try
  {
    //!< JSON-объект для последующей сериализации в файл
    nlohmann::json json;
    json[TYPE_TAG] = std::string("FeatureCollection");

    //! Записываем информацию о системе координат
    exportCoordinateSystem(json, !isNeedToConvertToDegrees);

    //!< JSON-массив для записи признаков
    nlohmann::json features = nlohmann::json::array();
    for (const auto& line : linestring)
    {
      //!< JSON-объект для свойства
      nlohmann::json feature;
      exportToGeoJson<Point_Type>(feature, line, isNeedToConvertToDegrees);
      features.push_back(feature);
    }

    json[FEATURES_TAG] = std::move(features);
    //! Сериализация JSON-объекта в файловый поток
    output << json;
  }
  catch (...)
  {
    return false;
  }
  return true;
}
//==============================================================================
bool exportToGeoJson(
  const std::string& path,
  const GeoLinestring2D& linestring,
  bool writeEmptyGeom/* = true*/) noexcept
{
  return exportToGeoJson(path, linestring, writeEmptyGeom, true);
}
//==============================================================================
bool exportToGeoJson(
  const std::string& path,
  const Linestring2D& linestring,
  bool writeEmptyGeom/* = true*/) noexcept
{
  return exportToGeoJson(path, linestring, writeEmptyGeom, false);
}
//==============================================================================
bool exportToGeoJson(
  const std::string& path,
  const GeoMultiLinestring2D& linestring,
  bool writeEmptyGeom/* = true*/) noexcept
{
  return exportToGeoJson(path, linestring, writeEmptyGeom, true);
}
//==============================================================================
bool exportToGeoJson(
  const std::string& path,
  const MultiLinestring2D& linestring,
  bool writeEmptyGeom/* = true*/) noexcept
{
  return exportToGeoJson(path, linestring, writeEmptyGeom, false);
}
//==============================================================================
/*!
 * \brief Формирование JSON-объекта для кольца
 *
 * \param[in] ring - кольцо,
 *
 * \param[in] isNeedToConvertToDegrees - необходимо ли переводить
 *                                       в градусы.
 * \return JSON-объект для кольца (массив точек)
 */
template <typename Point_Type>
nlohmann::json toJsonArray(
  const PrimaryEntities::Ring<Point_Type>& ring,
  bool isNeedToConvertToDegrees) noexcept
{
  //!< JSON-массив для записии координат точек
  nlohmann::json points = nlohmann::json::array();

  if (ring.empty())
    return points;

  //! Проход по вершинам
  for (const auto& point : ring)
  {
    points.push_back(toJsonArray(point, isNeedToConvertToDegrees));
  }

  return points;
}
//==============================================================================
/*!
 * \brief Запись многоугольника в JSON-объект
 *
 * \details Координаты точек в файл пишутся без кавычек.
 *
 * \param[in/out] json - JSON-объект для записи данных
 *
 * \param[in] polygon - многоугольник,
 *
 * \param[in] isNeedToConvertToDegrees - необходимо ли переводить
 *                                       в градусы.
 * \return true, если запись прошла успешно;
 *         false - иначе.
 */
template <typename Point_Type>
bool exportToGeoJson(
  nlohmann::json& json,
  const PrimaryEntities::Polygon<Point_Type>& polygon,
  bool isNeedToConvertToDegrees) noexcept
{
  try
  {
    //! Если в геометрии меньше 4 точек, она некорректна
    if ((!polygon.outer().empty()) && (polygon.outer().size() < 4))
      return false;

    json[TYPE_TAG] = std::string("Feature");

    //! Записываем информацию о системе координат
    exportCoordinateSystem(json, !isNeedToConvertToDegrees);

    //!< JSON-объект для геометрии
    nlohmann::json geometry;
    geometry[TYPE_TAG] = std::string("Polygon");
    //!< JSON-массив для координат
    nlohmann::json coordinates = nlohmann::json::array();

    //! Внешнее кольцо
    coordinates.push_back(toJsonArray(polygon.outer(),
        isNeedToConvertToDegrees));

    //! Проход по внутренним кольцам
    for (const auto& ring : polygon.inners())
    {
      //! В кольце должно быть не меньше 4 вершин
      if (ring.size() < 4)
        continue;

      //! Внутреннее кольцо
      coordinates.push_back(toJsonArray(ring,
          isNeedToConvertToDegrees));
    }

    geometry[COORDINATES_TAG] = std::move(coordinates);
    json[GEOMETRY_TAG] = std::move(geometry);
  }
  catch (...)
  {
    return false;
  }
  return true;
}
//==============================================================================
bool exportToGeoJson(
  const std::string& path,
  const GeoPolygon2D& polygon,
  bool writeEmptyGeom/* = true*/) noexcept
{
  //! Если не хотим записывать файл для пустой геометрии, пропускаем
  if ((!writeEmptyGeom) && polygon.outer().empty())
    return true;

  //!< Файловый поток для сериализации данных в .geojson.
  std::fstream output(path, std::ios_base::out | std::ios_base::trunc);
  if (!output.is_open())
    return false;

  //!< JSON-объект для последующей сериализации в файловый поток
  nlohmann::json json;
  //! Во внешнем контуре должно быть не меньше 4 вершин
  if (!exportToGeoJson(json, polygon, true))
      return false;
  //! Сериализация JSON-объекта в файловый поток
  output << json;

  return true;
}
//==============================================================================
bool exportToGeoJson(
  const std::string& path,
  const Polygon2D& polygon,
  bool writeEmptyGeom/* = true*/) noexcept
{
  //! Если не хотим записывать файл для пустой геометрии, пропускаем
  if ((!writeEmptyGeom) && polygon.outer().empty())
    return true;

  //!< Файловый поток для сериализации данных в .geojson.
  std::fstream output(path, std::ios_base::out | std::ios_base::trunc);
  if (!output.is_open())
    return false;

  //!< Объект для записи в файл
  nlohmann::json result;
  exportToGeoJson(result, polygon, false);

  //! Сериализация объекта в файловый поток
  output << result;

  return true;
}
//==============================================================================
bool exportToGeoJson(const std::string& path, const GeoBox2D& box) noexcept
{
  //! Конвертируем рамку в полигон
  GeoPolygon2D polygon = box.toPolygon();

  //! Записываем полигон в json
  return exportToGeoJson(path, polygon);
}
//==============================================================================
bool exportToGeoJson(const std::string& path, const Box2D& box) noexcept
{
  //! Конвертируем рамку в полигон
  Polygon2D polygon = box.toPolygon();

  //! Записываем полигон в json
  return exportToGeoJson(path, polygon);
}
//==============================================================================
/*!
 * \brief Запись множества многоугольников в .geojson-файл.
 *
 * \details Координаты точек в файл пишутся без кавычек.
 *
 * \param[in] path - путь к файлу, в который нужно записывать,
 *
 * \param[in] multiPolygon - множество многоугольников,
 *
 * \param[in] writeEmptyGeom - записывать ли пустое
 *                             множество многоугольников,
 * \param[in] isNeedToConvertToDegrees - необходимо ли переводить
 *                                       в градусы.
 * \return true, если запись прошла успешно;
 *         false - иначе.
 */
template <typename Point_Type>
bool exportToGeoJson(
  const std::string& path,
  const PrimaryEntities::MultiPolygon<Point_Type>& multiPolygon,
  bool writeEmptyGeom,
  bool isNeedToConvertToDegrees) noexcept
{
  //! Если не хотим записывать файл для пустой геометрии, пропускаем
  if ((!writeEmptyGeom) && multiPolygon.empty())
    return true;

  //!< Файловый поток для сериализации данных в .geojson.
  std::fstream output(path, std::ios_base::out | std::ios_base::trunc);
  if (!output.is_open())
    return false;

  //!< JSON-объект для последующей записи в файловый поток
  nlohmann::json json;
  json[TYPE_TAG] = std::string("FeatureCollection");

  //! Записываем информацию о системе координат
  exportCoordinateSystem(json, !isNeedToConvertToDegrees);
  //!< JSON-массив для признаков
  nlohmann::json features = nlohmann::json::array();

  //! Проходим по полигонам
  for (const auto& polygon : multiPolygon)
  {
      //!< JSON-объект для признака
      nlohmann::json feature;
      //! Во внешнем контуре должно быть не меньше 4 вершин
      if (!exportToGeoJson(feature, polygon, isNeedToConvertToDegrees))
        continue;
      features.push_back(feature);
  }

  json[FEATURES_TAG] = std::move(features);

  //! Сериализация JSON-объекта в файловый поток
  output << json;

  return true;
}
//==============================================================================
bool exportToGeoJson(
  const std::string& path,
  const GeoMultiPolygon2D& multiPolygon,
  bool writeEmptyGeom/* = true*/) noexcept
{
  if (!exportToGeoJson(path, multiPolygon, writeEmptyGeom, true))
    return false;
  
  return true;
}
//==============================================================================
bool exportToGeoJson(
  const std::string& path,
  const MultiPolygon2D& multiPolygon,
  bool writeEmptyGeom/* = true*/) noexcept
{
  if (!exportToGeoJson(path, multiPolygon, writeEmptyGeom, false))
    return false;

  return true;
}
//==============================================================================
/*!
 * \brief Запись множества многоугольников с высотами в .geojson-файл.
 *
 * \details Координаты точек в файл пишутся без кавычек.
 *
 * \param[in] path - путь к файлу, в который нужно записывать,
 *
 * \param[in] polygonsWithHeights - множество многоугольников с высотами,
 *
 * \param[in] writeEmptyGeom - записывать ли пустое
 *                             множество многоугольников с высотами,
 * \param[in] isNeedToConvertToDegrees - необходимо ли переводить
 *                                       в градусы.
 * \return true, если запись прошла успешно;
 *         false - иначе.
 */
template <typename Point_Type>
bool exportToGeoJson(
  const std::string& path,
  const std::vector<std::pair<
    PrimaryEntities::Polygon<Point_Type>, double>>& polygonsWithHeights,
  bool writeEmptyGeom,
  bool isNeedToConvertToDegrees) noexcept
{
  //! Нет данных - всё хорошо, но файл не записываем
  if ((!writeEmptyGeom) && polygonsWithHeights.empty())
    return true;

  //!< Файловый поток для сериализации данных в .geojson.
  std::fstream output(path, std::ios_base::out | std::ios_base::trunc);
  if (!output.is_open())
    return false;

  try
  {
    //!< JSON-объект для последующей сериализации в файловый поток
    nlohmann::json json;
    json[TYPE_TAG] = std::string("FeatureCollection");

    //! Записываем информацию о системе координат
    exportCoordinateSystem(json, !isNeedToConvertToDegrees);

    //!< JSON-массив для признаков
    nlohmann::json features = nlohmann::json::array();

    //! Проходим по полигонам
    for(const auto& polygonWithHeight: polygonsWithHeights)
    {
      //!< Полигон
      PrimaryEntities::Polygon<Point_Type> polygon
        = polygonWithHeight.first;

      //! Во внешнем контуре должно быть не меньше 4 вершин
      if (polygon.outer().size() < 4)
        continue;

      //!< Высота полигона
      double height = polygonWithHeight.second;

      //! Округляем значение высоты до 1 знака после запятой
      height = roundf(height * 10) / 10;

      //!< JSON-объект для свойства
      nlohmann::json feature;
      feature[TYPE_TAG] = std::string("Feature");
      feature[PROPERTIES_TAG] = nlohmann::json::object();
      feature[PROPERTIES_TAG][HEIGHT_TAG] = height;

      //!< JSON-объект для геометрии
      nlohmann::json geometry;
      geometry[TYPE_TAG] = std::string("Polygon");

      //!< JSON-массив для координат
      nlohmann::json coordinates = nlohmann::json::array();

      //! Помещаем координаты вершин внешнего кольца в массив координат
      coordinates.push_back(toJsonArray(polygon.outer(),
          isNeedToConvertToDegrees));

      //! Проход по внутренним кольцам
      for (const auto& ring : polygon.inners())
      {
        //! В кольце должно быть не меньше 4 вершин
        if (ring.size() < 4)
          continue;
        //! Помещаем координаты вершин внутреннего кольца в массив координат
        coordinates.push_back(toJsonArray(ring, isNeedToConvertToDegrees));
      }

      geometry[COORDINATES_TAG] = std::move(coordinates);
      feature[GEOMETRY_TAG] = std::move(geometry);

      features.push_back(feature);
    }

    json[FEATURES_TAG] = std::move(features);

    //! Сериализация JSON-объекта в файловый поток
    output << json;
  }

  catch(...)
  {
    return false;
  }
  return true;
}
//==============================================================================
bool
  exportToGeoJson(
    const std::string& path,
    const std::vector<std::pair<GeoPolygon2D, double>>& polygonsWithHeights,
    bool writeEmptyGeom/* = true*/)
  noexcept
{
  if (!exportToGeoJson(path, polygonsWithHeights, writeEmptyGeom, true))
    return false;

  return true;
}
//==============================================================================
bool
exportToGeoJson(
  const std::string& path,
  const std::vector<std::pair<Polygon2D, double>>& polygonsWithHeights,
  bool writeEmptyGeom/* = true*/)
  noexcept
{
  if (!exportToGeoJson(path, polygonsWithHeights, writeEmptyGeom, false))
    return false;

  return true;
}
//==============================================================================