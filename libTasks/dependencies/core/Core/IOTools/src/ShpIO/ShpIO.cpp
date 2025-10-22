#include <IOTools/ShpIO/ShpIO.hpp>

#include <IOTools/ByteConverting/ByteConverting.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryConverter.hpp>
//! std
#include <fstream>
#include <cstring>
//=============================================================================
constexpr int SHP_FILE_CODE(9994);
constexpr int SHP_FILE_VERSION(1000);
constexpr int SHP_POLYLINE(3);
constexpr int SHP_POLYGON(5);
constexpr int SHP_MULTIPOINT(8);
//=============================================================================
namespace HeaderShapeFile
{
//=============================================================================
inline int fileCode(const char* fileBuf, int startIndex) noexcept
{
  return ByteConverting::readBigEndianInt(fileBuf, startIndex);
}
//=============================================================================
inline int fileLength(const char* fileBuf, int startIndex) noexcept
{
  return ByteConverting::readBigEndianInt(fileBuf, startIndex);
}
//=============================================================================
inline int shapeType(const char* fileBuf, int startIndex) noexcept
{
  return ByteConverting::readLittleEndian<int>(fileBuf, startIndex);
}
//=============================================================================
} //! HeaderShapeFile
//=============================================================================
/*!
 * \brief fillHeaders -- функция, заполняющая основной заголовок .shp-файла
 * и заголовок первой записи .shp-файла.
 *
 * \details не рекомендуется использовать,
 * если планируется создание более чем одной записи в .shp-файле;
 * заголовок в этой функции формируется так, будто в файле,
 * которому он принадлежит, находится единственная запись о геометрии.
 *
 * \param[out] headers -- буфер, который в результате работы функции
 * будет иметь размер 108 байт
 * и содержать заголовки .shp-файла (основной и заголовок первой записи).
 *
 * \param[in] geometryType -- код геометрии в стандарте .shp.
 *
 * \param[in] boundingBox -- минимальный прямоугольник
 * содержащий в себе всю геометрию,
 * которая в дальнейшем должна быть записана в файл.
 *
 * \param[in] isNeedToConvertToDegrees -- требуется ли
 * предварительная конвертация координат вершин прямоугольника
 * из радиан в градусы.
 *
 * \param[in] contentLength -- размер записи
 * (в 16-битных word, без учёта заголовка записи).
 */
template <typename Point_Type>
void
fillHeaders(
  std::vector<char>& headers,
  int geometryType,
  const PrimaryEntities::Box<Point_Type>& boundingBox,
  int contentLength,
  bool isNeedToConvertToDegrees = false) noexcept
{
  /*!
   * Размер основного заголовка -- 100 байт,
   * размер заголовка записи (Main File Record Header) -- 8 байт.
   */
  headers.resize(108);
  //! Записываем идентификатор (код) файла.
  ByteConverting::writeBigEndianInt(headers.data(), 0, SHP_FILE_CODE);
  //! Записываем номер версии.
  ByteConverting::writeLittleEndian(headers.data(), SHP_FILE_VERSION, 28);
  //! Записываем тип геометрии.
  ByteConverting::writeLittleEndian(headers.data(), geometryType, 32);
  //! Записываем информацию о минимальном прямоугольнике, содержащем геометрию.
  ByteConverting::writePoint(
    headers.data(),
    36,
    boundingBox.min_corner(),
    isNeedToConvertToDegrees);
  ByteConverting::writePoint(
    headers.data(),
    52,
    boundingBox.max_corner(),
    isNeedToConvertToDegrees);
  /*!
   * Так как точки геометрии не имеют координат по оси z
   * и весовых коэффициентов,
   * эти поля инициализируем нулями, в соответствии со спецификацией.
   */
  for (int offset(68); offset < 100; offset += 8)
    ByteConverting::writeDouble(headers.data(), offset, 0.);

  //! Записываем номер первой записи.
  ByteConverting::writeBigEndianInt(headers.data(), 100, 1);
  //! Записываем размер первой записи (в 16-битных word, не включая заголовок).
  ByteConverting::writeBigEndianInt(headers.data(), 104, contentLength);
  //! Записываем размер файла (в 16-битных word).
  ByteConverting::writeBigEndianInt(headers.data(), 24, contentLength + 54);
}
//=============================================================================
/*!
 * \brief changeExtension -- функция, заменяющая в имени файла расширение.
 *
 * \param[in] originalName -- оригинальное имя файла.
 *
 * \param[in] extension -- желаемое расширение для имени файла.
 *
 * \return новое имя файла с желаемым расширением.
 *
 * \details Если в оригинальном имени файла нет расширения,
 * желаемое просто допишется в конец имени.
 */
std::string
changeExtension(
  const std::string& originalName,
  const std::string& extension) noexcept
{
  std::size_t positionOfLastDot(originalName.find_last_of("."));
  if (positionOfLastDot != std::string::npos)
    return originalName.substr(0, positionOfLastDot + 1) + extension;
  else
    return originalName + "." + extension;
}
//=============================================================================
/*!
 * \brief createIndexFile -- функция создающая индекс-файл для .shp-файла.
 *
 * \param[in] header -- массив байтов, первые 100 элементов которого
 * должны содержать заголовок оригинального .shp-файла в двоичном формате.
 *
 * \param[in] recordsLengths -- массив с элементами типа int, длина которого
 * равна количеству записей в оригинальном .shp-файле,
 * а i-ый элемент имеет значение размера i-ой записи,
 * измеряемого в 16-битных words
 *
 * \param[in] shapeFilePath -- путь к оригинальному .shp-файлу.
 *
 * \return true, если при создание файла прошло успешно;
 *         false -- иначе.
 */
bool
createIndexFile(
  const std::vector<char>& header,
  const std::vector<int>& recordsLengths,
  const std::string& shapeFilePath) noexcept
{
  std::string indexFileName(changeExtension(shapeFilePath, "shx"));

  try
  {
    std::fstream outputFileStream(
      indexFileName.c_str(),
      std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if (!outputFileStream.is_open())
      return false;

    outputFileStream.write(header.data(), 24);
    std::size_t countOfRecords(recordsLengths.size());
    char        fileLength[4];
    ByteConverting::writeBigEndianInt(
      fileLength,
      0,
      static_cast<int>(50 + 4 * countOfRecords));
    outputFileStream.write(fileLength, 4);
    outputFileStream.write(header.data() + 28, 72);

    int  currentOffset(50);
    char indexRecord[8];
    for (std::size_t i(0); i < countOfRecords; ++i)
    {
      ByteConverting::writeBigEndianInt(indexRecord, 0, currentOffset);
      ByteConverting::writeBigEndianInt(indexRecord, 4, recordsLengths[i]);
      outputFileStream.write(indexRecord, 8);
      currentOffset += recordsLengths[i] + 4;
    }
  }
  catch (...)
  {
    return false;
  }

  return true;
}
//=============================================================================
/*!
 * \brief createdBASEIVFile -- функция создающая .dbf-файл для .shp-файла.
 *
 * \param[in] count -- количество частей в геометрии
 * (например, для множества многоугольников,
 * состоящего из 4 многоугольников, следует указать 4).
 *
 * \param[in] shapeFilePath -- путь к оригинальному .shp-файлу.
 *
 * \param[in] geometryName -- имя, (в ASCII),
 * которое следует указать для геометрии.
 *
 * \return true, если при создание файла прошло успешно;
 *         false -- иначе.
 *
 * \details если длина указанного имени превышает 11 байт,
 * оно будет урезано до 11 байт.
 */
bool
createdBASEIVFile(
  const std::size_t count,
  const std::string& shapeFilePath,
  const std::string& geometryName = "Geometry") noexcept
{
  //!< Длина заголовка .dbf-файла (в байтах).
  constexpr unsigned short HEADER_LENGTH(65U);
  //!< Количество байт в записи .dbf-файла.
  constexpr unsigned short RECORD_LENGTH(12U);
  //!< ID языкового драйвера.
  constexpr unsigned char  LANGUAGE_DRIVER_ID(87U);
  //!< Максимальная длина имени поля (в байтах).
  constexpr unsigned int   FIELD_NAME_MAX_LENGTH(11U);
  //!< Длина поля (в байтах).
  constexpr unsigned char  FIELD_LENGTH(11U);
  //!< Имя .dbf-файла.
  std::string              dbfFileName(changeExtension(shapeFilePath, "dbf"));

  try
  {
    std::fstream outputFileStream(
      dbfFileName.c_str(),
      std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if (!outputFileStream.is_open())
      return false;

    //! Заголовок файла.

    outputFileStream.put(3); //! Информация о версии .dbf-файла.
    //! Дата последнего обновления формата.
    outputFileStream.put(0x5F); //! 1995-ый год.
    outputFileStream.put(0x07); //! Июль.
    outputFileStream.put(0x1A); //! 26-ое число.
    //!< Временный буфер.
    char buffer[17];
    //! Записываем число записей в файле.
    ByteConverting::writeLittleEndian(buffer, (unsigned int)count);
    outputFileStream.write(buffer, 4);
    //! Записываем длину заголовка файла.
    ByteConverting::writeLittleEndian(buffer, HEADER_LENGTH);
    outputFileStream.write(buffer, 2);
    //! Записываем длину каждой записи файла.
    ByteConverting::writeLittleEndian(buffer, RECORD_LENGTH);
    outputFileStream.write(buffer, 2);
    /*!
     * 17 нулевых байтов
     * (некоторые из них зарезервированы, некоторые должны быть нулями).
     */
    ByteConverting::writeLittleEndian(buffer, 0LL);
    ByteConverting::writeLittleEndian(buffer, 0LL, 8);
    ByteConverting::writeLittleEndian(buffer, char(0), 16);
    outputFileStream.write(buffer, 17);
    //! ID языкового драйвера.
    outputFileStream.put(LANGUAGE_DRIVER_ID);
    //! Два нулевых зарезервированных байта.
    outputFileStream.write(buffer, 2);

    //! Массив дескрипторов полей (в нём будет единственный элемент).

    //!< Имя поля.
    std::string fieldName(geometryName);
    if (fieldName.size() > FIELD_NAME_MAX_LENGTH)
      fieldName = fieldName.substr(0, FIELD_NAME_MAX_LENGTH);
    else if (fieldName.size() < FIELD_NAME_MAX_LENGTH)
      fieldName.resize(FIELD_NAME_MAX_LENGTH, 0);
    outputFileStream.write(fieldName.data(), FIELD_NAME_MAX_LENGTH);
    //! Тип поля (N - Name - имя).
    outputFileStream.put(0x4E);
    //! 4 нулевых зарезервированных байта.
    outputFileStream.write(buffer, 4);
    //! Длина поля.
    outputFileStream.put(FIELD_LENGTH);
    /*!
     * 15 нулевых байтов
     * (некоторые из них зарезервированы, некоторые должны быть равны нулю).
     */
    outputFileStream.write(buffer, 15);
    //! Байт окончания массива.
    outputFileStream.put(0x0D);

    //! Записи БД.

    /*!
     * К каждой части геометрии будем просто приписывать её порядковый номер
     * (так делалось и в Территории).
     */
    for (unsigned int i(0); i < count; ++i)
    {
      //! Байт, говорящий о том, что запись в порядке.
      outputFileStream.put(0x20);
      std::string numberAsStr(std::to_string(i));
      if (numberAsStr.size() < FIELD_LENGTH )
        numberAsStr =
          std::string(FIELD_LENGTH - numberAsStr.size(), 0x20)
          + numberAsStr;
      outputFileStream.write(numberAsStr.data(), FIELD_LENGTH);
    }
  }
  catch (...)
  {
    return false;
  }

  return true;
}
//=============================================================================
/*!
 * \brief createBoxForShp -- функция, формирующая минимальный прямоугольник
 *                           для геометрии
 * 
 * \details Если геометрия пуста, координаты точек прямоугольника будут
 *          заполнены максимально возможными значениями
 *
 * \param[in] geometry -- геометрия, для которой вычисляется прямоугольник.
 *
 * \return Минимальный прямоугольник.
 */
template <typename Point_Type, typename Entity>
PrimaryEntities::Box<Point_Type> createBoxForShp(Entity geometry)
{
  auto geosGeometry = PrimaryEntities::toGeosGeometry(geometry);
  const geos::geom::Envelope* geosEnvelope
                                         = geosGeometry->getEnvelopeInternal();

  PrimaryEntities::Box<Point_Type> box;
  if (geosEnvelope->isNull())
  {
    Point_Type nullBoxPoint(ByteConverting::createPointFromDouble<Point_Type>(
      std::numeric_limits<double>::max(),
      std::numeric_limits<double>::max()));
    box = PrimaryEntities::Box<Point_Type>(nullBoxPoint, nullBoxPoint);
  }
  else
    box = PrimaryEntities::fromGeosBox<Point_Type>(geosEnvelope);

  return box;
}
//=============================================================================
/*!
 * \brief exportToShapeFile -- функция, создающая .shp-файл,
 * содержащий множество точек.
 *
 * \param[in] path -- путь к файлу, который нужно создать/перезаписать.
 *
 * \param[in] multiPoint -- множество точек,
 * которое нужно записать в .shp-файл.
 *
 * \param[in] isNeedToConvertToDegrees -- требуется ли
 * предварительная конвертация координат вершин из радиан в градусы.
 *
 * \param[in] writeEmptyGeom - записывать ли пустое множество точек
 *                             (по умолчанию пишем).
 *
 * \details все старые данные, содержащиеся в файле,
 * будут уничтожены при вызове этой функции.
 *
 * \return true, если запись прошла успешно, иначе -- false.
 */
template <typename Point_Type>
bool
exportToShapeFile(
  const std::string& path,
  const PrimaryEntities::MultiPoint<Point_Type>& multiPoint,
  bool isNeedToConvertToDegrees = false,
  bool writeEmptyGeom = true) noexcept
{
  //! Если не хотим записывать файл для пустой геометрии, пропускаем
  if ((!writeEmptyGeom) && multiPoint.empty())
    return true;
  try
  {
    std::fstream outputFileStream(
      path.c_str(),
      std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if (!outputFileStream.is_open())
      return false;

    //!< Буфер для заголовков файла.
    std::vector<char> headers;
    //!< Минимальный прямоугольник, содержащий все точки из множества.
    PrimaryEntities::Box<Point_Type> boundingBox(
      createBoxForShp<Point_Type,
                      PrimaryEntities::MultiPoint<Point_Type>>(multiPoint));
    //!< Размер содержимого записи (в 16-битных word'ах).
    int recordContentLength(static_cast<int>(20 + 8 * multiPoint.size()));
    //! Заполняем заголовки.
    fillHeaders(
      headers,
      SHP_MULTIPOINT,
      boundingBox,
      recordContentLength,
      isNeedToConvertToDegrees);

    //!< Буфер содержимого записи.
    std::vector<char> content;
    content.resize(recordContentLength * 2);
    /*!
     * Копируем из заголовка тип геометрии и минимальный прямоугольник,
     * содержащий все точки из множества.
     */
    memcpy(content.data(), headers.data() + 32, 36);
    //! Записываем количество точек в множестве.
    ByteConverting::writeLittleEndian(
      content.data(),
      static_cast<int>(multiPoint.size()),
      36);
    //!< Текущее смещение относительно начала буфера содержимого записи.
    int currentOffset(40);
    //! Записываем точки в буфер.
    for (const auto& point : multiPoint)
    {
      ByteConverting::writePoint(
        content.data(),
        currentOffset,
        point,
        isNeedToConvertToDegrees);
      currentOffset += 16;
    }
    //! Копируем содержимое буферов в файловый поток.
    outputFileStream.write(headers.data(), headers.size());
    outputFileStream.write(content.data(), content.size());

    //! Создаём индекс-файл.
    if (!createIndexFile(
           headers,
           std::vector<int>{ recordContentLength },
           path))
      return false;
    //! Создаём .dbf-файл.
    if (!createdBASEIVFile(multiPoint.size(), path))
      return false;
  }
  catch (...)
  {
    return false;
  }
  return true;
}
//=============================================================================
bool
exportToShapeFile(
  const std::string& path,
  const GeoMultiPoint2D& multiPoint,
  bool writeEmptyGeom/* = true*/) noexcept
{
  return exportToShapeFile<GeoPoint2D>(
    path, multiPoint, false, writeEmptyGeom);
}
//=============================================================================
bool
exportToShapeFileInDegrees(
  const std::string& path,
  const GeoMultiPoint2D& multiPoint,
  bool writeEmptyGeom/* = true*/) noexcept
{
  return exportToShapeFile<GeoPoint2D>(
    path, multiPoint, true, writeEmptyGeom);
}
//=============================================================================
bool
exportToShapeFile(
  const std::string& path,
  const MultiPoint2D& multiPoint,
  bool writeEmptyGeom/* = true*/) noexcept
{
  return exportToShapeFile<Point2D>(
    path, multiPoint, false, writeEmptyGeom);
}
//=============================================================================
bool
exportToShapeFile(const std::string& path, const GeoPoint2D& point) noexcept
{
  return exportToShapeFile(path, GeoMultiPoint2D{ point });
}
//=============================================================================
bool
exportToShapeFileInDegrees(
  const std::string& path,
  const GeoPoint2D& point) noexcept
{
  return exportToShapeFileInDegrees(path, GeoMultiPoint2D{ point });
}
//=============================================================================
bool
exportToShapeFile(const std::string& path, const Point2D& point) noexcept
{
  return exportToShapeFile(path, MultiPoint2D{ point });
}
//=============================================================================
/*!
 * \brief exportToShapeFile -- функция, создающая .shp-файл,
 * содержащий ломаную линию.
 *
 * \param[in] path -- путь к файлу, который нужно создать/перезаписать.
 *
 * \param[in] linestring -- ломаная линия,
 * которую нужно записать в .shp-файл.
 *
 * \param[in] isNeedToConvertToDegrees -- требуется ли
 * предварительная конвертация координат вершин из радиан в градусы.
 *
 * \param[in] writeEmptyGeom - записывать ли пустую ломаную
 *                             (по умолчанию пишем).
 *
 * \details все старые данные, содержащиеся в файле,
 * будут уничтожены при вызове этой функции.
 *
 * \return true, если запись прошла успешно, иначе -- false.
 */
template <typename Point_Type>
bool
exportToShapeFile(
  const std::string& path,
  const PrimaryEntities::Linestring<Point_Type>& linestring,
  bool isNeedToConvertToDegrees = false,
  bool writeEmptyGeom = true) noexcept
{
  //! Если не хотим записывать файл для пустой геометрии, пропускаем
  if ((!writeEmptyGeom) && linestring.empty())
    return true;

  try
  {
    std::fstream outputFileStream(
      path.c_str(),
      std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if (!outputFileStream.is_open())
      return false;

    //!< Буфер для заголовков файла.
    std::vector<char> headers;
    //!< Минимальный прямоугольник, содержащий все точки из множества.
    PrimaryEntities::Box<Point_Type> boundingBox(
      createBoxForShp<Point_Type,
      PrimaryEntities::Linestring<Point_Type>>(linestring));
    //!< Размер содержимого записи (в 16-битных word'ах).
    int recordContentLength(static_cast<int>(24 + 8 * linestring.size()));
    //! Заполняем заголовки.
    fillHeaders(headers,
      SHP_POLYLINE,
      boundingBox,
      recordContentLength,
      isNeedToConvertToDegrees);

    //!< Буфер содержимого записи.
    std::vector<char> content;
    content.resize(recordContentLength * 2);
    /*!
     * Копируем из заголовка тип геометрии и минимальный прямоугольник,
     * содержащий ломаную.
     */
    memcpy(content.data(), headers.data() + 32, 36);
    //! Записываем количество частей в ломаной (в нашем случае -- только одна).
    ByteConverting::writeLittleEndian(content.data(), 1, 36);
    //! Записываем количество точек во всех частях ломаной.
    ByteConverting::writeLittleEndian(
      content.data(),
      static_cast<int>(linestring.size()),
      40);
    /*!
     * Записываем индекс первой точки первой части
     * относительно начала массива точек
     * (этот индекс равен нулю).
     */
    ByteConverting::writeLittleEndian(content.data(), 0, 44);
    //!< Текущее смещение относительно начала буфера содержимого записи.
    int currentOffset(48);
    //! Записываем вершины ломаной в буфер.
    for (const auto& point : linestring)
    {
      ByteConverting::writePoint(
        content.data(),
        currentOffset,
        point,
        isNeedToConvertToDegrees);
      currentOffset += 16;
    }
    //! Копируем содержимое буферов в файловый поток.
    outputFileStream.write(headers.data(), headers.size());
    outputFileStream.write(content.data(), content.size());

    //! Создаём индекс-файл.
    if (!createIndexFile(
           headers,
           std::vector<int>{ recordContentLength },
           path))
      return false;
    //! Создаём .dbf-файл.
    if (!createdBASEIVFile(1, path))
      return false;
  }
  catch (...)
  {
    return false;
  }
  return true;
}
//=============================================================================
bool
exportToShapeFile(
  const std::string& path,
  const GeoLinestring2D& linestring,
  bool writeEmptyGeom/* = true*/) noexcept
{
  return exportToShapeFile<GeoPoint2D>(
    path, linestring, false, writeEmptyGeom);
}
//=============================================================================
bool
exportToShapeFileInDegrees(
  const std::string& path,
  const GeoLinestring2D& linestring,
  bool writeEmptyGeom/* = true*/) noexcept
{
  return exportToShapeFile<GeoPoint2D>(
    path, linestring, true, writeEmptyGeom);
}
//=============================================================================
bool
exportToShapeFile(
  const std::string& path,
  const Linestring2D& linestring,
  bool writeEmptyGeom/* = true*/) noexcept
{
  return exportToShapeFile<Point2D>(
    path, linestring, false, writeEmptyGeom);
}
//=============================================================================
/*!
 * \brief exportToShapeFile -- функция, создающая .shp-файл,
 * содержащий прямоугольник, стороны которого параллельны координатным осям.
 *
 * \param[in] path -- путь к файлу, который нужно создать/перезаписать.
 *
 * \param[in] box -- прямоугольник,
 * стороны которого параллельны координатным осям,
 * который нужно записать в .shp-файл.
 *
 * \param[in] isNeedToConvertToDegrees -- требуется ли
 * предварительная конвертация координат вершин из радиан в градусы.
 *
 * \details все старые данные, содержащиеся в файле,
 * будут уничтожены при вызове этой функции.
 *
 * \return true, если запись прошла успешно, иначе -- false.
 */
template <typename Point_Type>
bool
exportToShapeFile(
  const std::string& path,
  const PrimaryEntities::Box<Point_Type>& box,
  bool isNeedToConvertToDegrees = false) noexcept
{
  /*!
   * Здесь мы просто конвертируем прямоугольник в многоугольник
   * и вызываем аналогичную функцию для многоугольника.
   */
  PrimaryEntities::Polygon<Point_Type> polygonFromBox = box.toPolygon();

  PrimaryEntities::MultiPolygon<Point_Type> multiPolygon{
      polygonFromBox
  };
  return exportToShapeFile<Point_Type>(
    path,
    multiPolygon,
    isNeedToConvertToDegrees);
}
//=============================================================================
bool exportToShapeFile(const std::string& path, const GeoBox2D& box) noexcept
{
  return exportToShapeFile<GeoPoint2D>(path, box);
}
//=============================================================================
bool
exportToShapeFileInDegrees(
  const std::string& path,
  const GeoBox2D& box) noexcept
{
  return exportToShapeFile<GeoPoint2D>(path, box, true);
}
//=============================================================================
bool exportToShapeFile(const std::string& path, const Box2D& box) noexcept
{
  return exportToShapeFile<Point2D>(path, box);
}
//=============================================================================
/*!
 * \brief exportToShapeFile -- функция, создающая .shp-файл,
 * содержащий множество многоугольников.
 *
 * \param[in] path -- путь к файлу, который нужно создать/перезаписать.
 *
 * \param[in] multiPolygon -- множество многоугольников,
 * которое нужно записать в .shp-файл.
 *
 * \param[in] isNeedToConvertToDegrees -- требуется ли
 * предварительная конвертация координат вершин из радиан в градусы.
 *
 * \param[in] writeEmptyGeom - записывать ли пустой полигон (по умолчанию
 *                             пишем).
 *
 * \details все старые данные, содержащиеся в файле,
 * будут уничтожены при вызове этой функции.
 *
 * \return true, если запись прошла успешно, иначе -- false.
 */
template <typename Point_Type>
bool
exportToShapeFile(
  const std::string& path,
  const PrimaryEntities::MultiPolygon<Point_Type>& multiPolygon,
  bool isNeedToConvertToDegrees = false,
  bool writeEmptyGeom = true
  ) noexcept
{
  //! Если не хотим записывать файл для пустой геометрии, пропускаем
  if ((!writeEmptyGeom) && multiPolygon.empty())
    return true;

  try
  {
    std::fstream outputFileStream(
      path.c_str(),
      std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if (!outputFileStream.is_open())
      return false;

    //!< Буфер для заголовков файла.
    std::vector<char> header;
    //! Размер основного заголовка -- 100 байт,
    header.resize(100);
    //! Записываем идентификатор файла.
    ByteConverting::writeBigEndianInt(header.data(), 0, SHP_FILE_CODE);
    //!< Размер файла (в 16-битных word'ах).
    std::size_t fileSize(50);
    for (const auto& polygon : multiPolygon)
    {
      fileSize += 28 + 2 * polygon.inners().size();
      fileSize += polygon.outer().size() * 8;
      for (const auto& innerRing : polygon.inners())
        fileSize += innerRing.size() * 8;
    }
    //! Записываем размер файла.
    ByteConverting::writeBigEndianInt(
      header.data(),
      24,
      static_cast<int>(fileSize));
    //! Записываем номер версии .shp.
    ByteConverting::writeLittleEndian(header.data(), SHP_FILE_VERSION, 28);
    //! Записываем тип геометрии.
    ByteConverting::writeLittleEndian(header.data(), SHP_POLYGON, 32);
    //!< Минимальный прямоугольник, содержащий все многоугольники из множества.
    PrimaryEntities::Box<Point_Type> boundingBox(
      createBoxForShp<Point_Type,
      PrimaryEntities::MultiPolygon<Point_Type>>(multiPolygon));

    ByteConverting::writePoint(
      header.data(),
      36,
      boundingBox.min_corner(),
      isNeedToConvertToDegrees);
    ByteConverting::writePoint(
      header.data(),
      52,
      boundingBox.max_corner(),
      isNeedToConvertToDegrees);
    /*!
     * Так как вершины многоугольников не имеют координат по оси z
     * и весовых коэффициентов,
     * эти поля инициализируем нулями, в соответствии со спецификацией.
     */
    for (int offset(68); offset < 100; offset += 8)
      ByteConverting::writeDouble(header.data(), offset, 0.);

    //! Копируем содержимое буфера в файловый поток.
    outputFileStream.write(header.data(), header.size());

    //!< Количество многоугольников в множетсве.
    std::size_t countOfPolygons(multiPolygon.size());
    //!< Размеры содержимого записей (в 16-битных word).
    std::vector<int> recordsLengths;
    recordsLengths.resize(countOfPolygons);
    /*!
     * Для каждого многоугольника создаём запись в буфере
     * и копируем её в файловый поток.
     */
    for (std::size_t polygonNumber(0);
         polygonNumber < countOfPolygons;
         ++polygonNumber)
    {
      //!< Буфер заголовка и содержимого записи.
      std::vector<char> content;

      //!< Количество вершин многоугольника.
      std::size_t countOfVertices(multiPolygon[polygonNumber].outer().size());
      for (const auto& innerRing : multiPolygon[polygonNumber].inners())
        countOfVertices += innerRing.size();
      /*!< 
       * Размер записи об одном многоугольнике
       * (в байтах, не включая заголовок).
       */
      int sizeOfContent(
        static_cast<int>(
          48
          + multiPolygon[polygonNumber].inners().size() * 4
          + countOfVertices * 16));
      recordsLengths[polygonNumber] = sizeOfContent / 2;

      //! Делаем размер буфера равным сумме размеров записи и её заголовка.
      content.resize(sizeOfContent + 8);
      //! Записываем номер записи.
      ByteConverting::writeBigEndianInt(
        content.data(),
        0,
        static_cast<int>(polygonNumber + 1));
      //! Записываем размер записи (в 16-битных word, не включая заголовок).
      ByteConverting::writeBigEndianInt(content.data(), 4, sizeOfContent / 2);
      //! Записываем код геометрии.
      ByteConverting::writeLittleEndian(content.data(), SHP_POLYGON, 8);
      /*!
       * Записываем информацию о минимальном прямоугольнике,
       * содержащем многоугольник.
       */
      PrimaryEntities::Box<Point_Type> boundingBox(
        createBoxForShp<Point_Type,
        PrimaryEntities::Polygon<Point_Type>>(multiPolygon[polygonNumber]));

      ByteConverting::writePoint(
        content.data(),
        12,
        boundingBox.min_corner(),
        isNeedToConvertToDegrees);
      ByteConverting::writePoint(
        content.data(),
        28,
        boundingBox.max_corner(),
        isNeedToConvertToDegrees);
      //! Записываем количество колец в многоугольнике.
      ByteConverting::writeLittleEndian(
        content.data(),
        static_cast<int>(1 + multiPolygon[polygonNumber].inners().size()),
        44);
      //! Записываем количество вершин многоугольника.
      ByteConverting::writeLittleEndian(
        content.data(),
        static_cast<int>(countOfVertices),
        48);

      /*!
       * Записываем индекс первой точки внешнего кольца многоугольника
       * относительно начала массива точек.
       */
      ByteConverting::writeLittleEndian(content.data(), 0, 52);
      //!< Текущее смещение относительно начала буфера содержимого записи.
      int currentOffset(56);
      //!< Текущий индекс точки относительно начала массива точек.
      std::size_t index(multiPolygon[polygonNumber].outer().size());
      /*!
       * Записываем индексы первых вершин каждого кольца
       * относительно начала массива вершин.
       */
      for (const auto& innerRing : multiPolygon[polygonNumber].inners())
      {
        ByteConverting::writeLittleEndian(
          content.data(),
          static_cast<int>(index),
          currentOffset);
        index += innerRing.size();
        currentOffset += 4;
      }

      //! Переходим непосредственно к записи вершин.
      for (const auto& point : multiPolygon[polygonNumber].outer())
      {
        ByteConverting::writePoint(
          content.data(),
          currentOffset,
          point,
          isNeedToConvertToDegrees);
        currentOffset += 16;
      }
      for (const auto& innerRing : multiPolygon[polygonNumber].inners())
        for (const auto& point : innerRing)
        {
          ByteConverting::writePoint(
            content.data(),
            currentOffset,
            point,
            isNeedToConvertToDegrees);
          currentOffset += 16;
        }
      //! Копируем содержимое буфера в файловый поток.
      outputFileStream.write(content.data(), content.size());
    }
    //! Создаём индекс-файл.
    if (!createIndexFile(header, recordsLengths, path))
      return false;
    //! Создаём .dbf-файл.
    if (!createdBASEIVFile(multiPolygon.size(), path))
      return false;
  }
  catch (...)
  {
    return false;
  }
  return true;
}
//=============================================================================
bool
exportToShapeFile(
  const std::string& path,
  const GeoMultiPolygon2D& multiPolygon,
  bool writeEmptyGeom/* = true*/) noexcept
{
  return exportToShapeFile<GeoPoint2D>(
    path, multiPolygon, false, writeEmptyGeom);
}
//=============================================================================
bool
exportToShapeFileInDegrees(
  const std::string& path,
  const GeoMultiPolygon2D& multiPolygon,
  bool writeEmptyGeom/* = true*/) noexcept
{
  return exportToShapeFile<GeoPoint2D>(
    path, multiPolygon, true, writeEmptyGeom);
}
//=============================================================================
bool
exportToShapeFile(
  const std::string& path,
  const MultiPolygon2D& multiPolygon,
  bool writeEmptyGeom/* = true*/) noexcept
{
  return exportToShapeFile<Point2D>(
    path, multiPolygon, false, writeEmptyGeom);
}
//=============================================================================
bool
exportToShapeFile(
  const std::string& path,
  const GeoPolygon2D& polygon,
  bool writeEmptyGeom/* = true*/) noexcept
{
  //! Предварительно нужно проверить, что полигон пустой, потому что после 
  //! конструирования мультиполигона, последний уже не будет пустым  
  if ((!writeEmptyGeom) && polygon.outer().empty())
    return true;
  //! Вызываем запись мультиполигона в файл
  return exportToShapeFile<GeoPoint2D>(path, GeoMultiPolygon2D{ polygon });
}
//==============================================================================
bool 
exportToShapeFileInDegrees(
  const std::string& path, 
  const GeoPolygon2D& polygon,
  bool writeEmptyGeom/* = true*/) noexcept
{
  //! Предварительно нужно проверить, что полигон пустой, потому что после 
  //! конструирования мультиполигона, последний уже не будет пустым  
  if ((!writeEmptyGeom) && polygon.outer().empty())
    return true;
  //! Вызываем запись мультиполигона в файл
  return exportToShapeFile<GeoPoint2D>(path, GeoMultiPolygon2D{ polygon },
                                       true);
}
//=============================================================================
bool
exportToShapeFile(
  const std::string& path,
  const Polygon2D& polygon,
  bool writeEmptyGeom/* = true*/) noexcept
{
  //! Предварительно нужно проверить, что полигон пустой, потому что после 
  //! конструирования мультиполигона, последний уже не будет пустым  
  if ((!writeEmptyGeom) && polygon.outer().empty())
    return true;
  //! Вызываем запись мультиполигона в файл
  return exportToShapeFile<Point2D>(path, MultiPolygon2D{ polygon });
}
//=============================================================================
/*!
 * \brief getSizeOfRecord -- функция, возвращающая размер записи,
 * указанный в её заголовке.
 *
 * \param[in] header -- указатель на начало заголовка записи;
 * память должна содержать заголовок в размере 8 байт.
 *
 * \return размер записи (в байтах) записи, к которой относится заголовок.
 */
inline int getSizeOfRecord(const char* header)
{
  return 2 * ByteConverting::readBigEndianInt(header, 4);
}
//=============================================================================
/*!
 * ПРИМЕЧАНИЕ.
 * Во всех объявленных ниже функциях считывается первая запись .shp-файла
 * и первая "геометрия" этой записи (в случае множеств с приставкой "multi-" --
 * все "геометрии" этой записи).
 */
//=============================================================================
/*!
 * \brief importMultiPointFromShapeFile -- функция, считывающая множество точек
 * из .shp-файла, содержащего множество точек.
 *
 * \param[in] path -- путь к файлу, из которого нужно считывать.
 *
 * \param[in] isNeedToConvertFromDegrees -- требуется ли
 * конвертировать значения координат вершин из градусов в радианы после чтения.
 *
 * \return искомое множество точек, если считывание прошло успешно,
 * иначе -- объект, созданный конструктором по умолчанию.
 *
 * \details считывается только первая запись файла (первое множество точек).
 */
template <typename Point_Type>
PrimaryEntities::MultiPoint<Point_Type>
importMultiPointFromShapeFile(
  const std::string& path,
  bool isNeedToConvertFromDegrees = false) noexcept
{
  PrimaryEntities::MultiPoint<Point_Type> multiPoint;

  //!< Буфер для заголовка файла.
  std::vector<char> fileBuf;
  try
  {
    std::fstream inputFileStream(
      path.c_str(),
      std::ios_base::in | std::ios_base::binary);
    if (!inputFileStream.is_open())
      return multiPoint;

    inputFileStream.seekg(0, std::ios_base::end);
    //!< Размер файла (в байтах).
    long fileSize(static_cast<long>(inputFileStream.tellg()));
    inputFileStream.seekg(0, std::ios_base::beg);
    if (fileSize < 100)
      return multiPoint;

    fileBuf.resize(100);
    //! Считываем заголовок файла.
    inputFileStream.read(fileBuf.data(), 100);

    //! Теперь можно прочитать необходимую информацию из заголовка.
    if (HeaderShapeFile::fileCode(fileBuf.data(), 0) != SHP_FILE_CODE)
      return multiPoint;
    if (HeaderShapeFile::shapeType(fileBuf.data(), 32) != SHP_MULTIPOINT)
      return multiPoint;

    /*!
     * Если перед нами действительно файл, содержащий множества точек,
     * считываем информацию о первой записи (первое множество точек).
     * Если быть точнее, считываем заголовок.
     */
    inputFileStream.read(fileBuf.data(), 8);
    //!< Размер записи о первом множетсве точек.
    int sizeOfFirstMultiPointRecord(getSizeOfRecord(fileBuf.data()));
    /*!
     * Перераспределяем память,
     * чтобы её хватило ровно под одну запись о множестве точек
     * (без заголовка).
     */
    fileBuf.resize(sizeOfFirstMultiPointRecord);
    //! Считываем запись о первом множестве точек.
    inputFileStream.read(fileBuf.data(), sizeOfFirstMultiPointRecord);
  }
  catch (...)
  {
    return multiPoint;
  }
  //!< Текущее смещение относительно начала записи (в байтах).
  int currentOffset(0);
  //! Если геометрия -- не множество точек, досрочно выходим из функции.
  if (HeaderShapeFile::shapeType(fileBuf.data(), currentOffset)
      != SHP_MULTIPOINT)
    return multiPoint;

  /*!
   * Пропускаем информацию о минимальном прямоугольнике,
   * содержащем все точки множества.
   */
  currentOffset += 36;
  //!< Количество точек в множестве.
  int countOfPoints(
    ByteConverting::readLittleEndian<int>(fileBuf.data(), currentOffset));
  currentOffset += 4;
  //! Читаем точки в множество.
  multiPoint.resize(countOfPoints);
  for (int pointNumber(0); pointNumber < countOfPoints; ++pointNumber)
  {
    ByteConverting::readPoint(
      fileBuf.data(),
      currentOffset,
      multiPoint[pointNumber],
      isNeedToConvertFromDegrees);
    currentOffset += 16;
  }

  return multiPoint;
}
//=============================================================================
GeoMultiPoint2D
importGeoMultiPoint2DFromShapeFile(const std::string& path) noexcept
{
  return importMultiPointFromShapeFile<GeoPoint2D>(path);
}
//=============================================================================
GeoMultiPoint2D
importGeoMultiPoint2DFromShapeFileInDegrees(const std::string& path) noexcept
{
  return importMultiPointFromShapeFile<GeoPoint2D>(path, true);
}
//=============================================================================
MultiPoint2D importMultiPoint2DFromShapeFile(const std::string& path) noexcept
{
  return importMultiPointFromShapeFile<Point2D>(path);
}
//=============================================================================
/*!
 * \brief importLinestringFromShapeFile -- функция, считывающая ломаную линию
 * из .shp-файла, содержащего ломаную линию.
 *
 * \param[in] path -- путь к файлу, из которого нужно считывать.
 *
 * \param[in] isNeedToConvertFromDegrees -- требуется ли
 * конвертировать значения координат вершин из градусов в радианы после чтения.
 *
 * \return искомая ломаная линия, если считывание прошло успешно,
 * иначе -- объект, созданный конструктором по умолчанию.
 */
template <typename Point_Type>
PrimaryEntities::Linestring<Point_Type>
importLinestringFromShapeFile(
  const std::string& path,
  bool isNeedToConvertFromDegrees = false) noexcept
{
  PrimaryEntities::Linestring<Point_Type> linestring;
  //!< Буфер для заголовка файла.
  std::vector<char> fileBuf;
  try
  {
    std::fstream inputFileStream(
      path.c_str(),
      std::ios_base::in | std::ios_base::binary);
    if (!inputFileStream.is_open())
      return linestring;

    inputFileStream.seekg(0, std::ios_base::end);
    //!< Размер файла (в байтах).
    long fileSize(static_cast<long>(inputFileStream.tellg()));
    inputFileStream.seekg(0, std::ios_base::beg);
    if (fileSize < 100)
      return linestring;


    fileBuf.resize(100);
    //! Считываем заголовок файла.
    inputFileStream.read(fileBuf.data(), 100);

    //! Теперь можно прочитать необходимую информацию из заголовка.
    if (HeaderShapeFile::fileCode(fileBuf.data(), 0) != SHP_FILE_CODE)
      return linestring;
    if (HeaderShapeFile::shapeType(fileBuf.data(), 32) != SHP_POLYLINE)
      return linestring;

    /*!
     * Если перед нами действительно файл, содержащий ломаные линии,
     * считываем информацию о первой записи (первой ломаной линии).
     * Если быть точнее, считываем заголовок.
     */
    inputFileStream.read(fileBuf.data(), 8);
    //!< Размер записи о первой ломаной.
    int sizeOfFirstPolyLineRecord(getSizeOfRecord(fileBuf.data()));
    /*!
     * Перераспределяем память,
     * чтобы её хватило ровно под одну запись о ломаной (без заголовка).
     */
    fileBuf.resize(sizeOfFirstPolyLineRecord);
    //! Считываем запись о первой ломаной линии.
    inputFileStream.read(fileBuf.data(), sizeOfFirstPolyLineRecord);
  }
  catch (...)
  {
    return linestring;
  }
  //!< Текущее смещение относительно начала записи (в байтах).
  int currentOffset(0);
  //! Если геометрия -- не ломаная, досрочно выходим из функции.
  if (HeaderShapeFile::shapeType(fileBuf.data(), currentOffset)
      != SHP_POLYLINE)
    return linestring;

  /*!
   * Пропускаем информацию о минимальном прямоугольнике для ломаной
   * и переходим к к информации о количестве частей.
   */
  currentOffset += 36;
  //!< Количество частей ломаной.
  int countOfParts(
    ByteConverting::readLittleEndian<int>(fileBuf.data(), currentOffset));
  currentOffset += 4;
  //!< Количество точек в первой части ломаной.
  int countOfPoints(
    ByteConverting::readLittleEndian<int>(fileBuf.data(), currentOffset));
  currentOffset += (countOfParts + 1) * 4;
  //! Читаем точки в ломаную.
  linestring.resize(countOfPoints);
  for (int pointNumber(0); pointNumber < countOfPoints; ++pointNumber)
  {
    ByteConverting::readPoint(
      fileBuf.data(),
      currentOffset,
      linestring[pointNumber],
      isNeedToConvertFromDegrees);
    currentOffset += 16;
  }

  return linestring;
}
//=============================================================================
GeoLinestring2D
importGeoLinestring2DFromShapeFile(const std::string& path) noexcept
{
  return importLinestringFromShapeFile<GeoPoint2D>(path);
}
//=============================================================================
GeoLinestring2D
importGeoLinestring2DFromShapeFileInDegrees(const std::string& path) noexcept
{
  return importLinestringFromShapeFile<GeoPoint2D>(path, true);
}
//=============================================================================
Linestring2D importLinestring2DFromShapeFile(const std::string& path) noexcept
{
  return importLinestringFromShapeFile<Point2D>(path);
}
//=============================================================================
/*!
 * \brief readSinglePolygonFromShapeFile -- функция, считывающая многоугольник
 * из файлового буфера .shp-файла.
 *
 * \param[in] shapeFileBuffer -- указатель на буфер,
 * из которого должно производиться чтение.
 *
 * \param[in|out] byteNumber -- смещение в буфере; при вызове функции
 * должно соответствовать началу записи о многоугольнике
 * (согласно формату .shp).
 *
 * \param[in] isNeedToConvertFromDegrees -- требуется ли
 * конвертировать значения координат вершин из градусов в радианы после чтения.
 *
 * \param[out] polygon -- многоугольник, содержимое которого заменяется
 * считанное содержимое буфера в соответствии с протоколом .shp.
 */
template <typename Point_Type>
void 
readSinglePolygonFromShapeFile(
  const char* shapeFileBuffer,
  long& byteNumber,
  PrimaryEntities::Polygon<Point_Type>& polygon,
  bool isNeedToConvertFromDegrees = false) noexcept
{
  //! Очищаем многоугольник.
  polygon.clear();

  //!< Исходное значение смещения относительно начала буфера.
  long initialByteNumber(byteNumber);

  //! Пропускаем информацию о номере записи и переходим к длине записи.
  byteNumber += 4;
  //!< Информацию о длине записи.
  long lengthOfContent(
    2 * ByteConverting::readBigEndianInt(shapeFileBuffer, byteNumber));
  
  //! Переходим к типу геометрии.
  byteNumber += 4;
  //!< Тип геометрии.
  long shapeTypes(HeaderShapeFile::shapeType(shapeFileBuffer, byteNumber));
  //! Если геометрия -- не многоугольник, досрочно выходим из функции.
  if (shapeTypes != 5)
  {
    /*! 
     * Увеличиваем сдвиг до значения,
     * соответствующему следующему байту после конца записи.
     */
    byteNumber = initialByteNumber + 8 + lengthOfContent;
    return;
  }

  /*! 
   * Пропускаем информацию о минимальном прямоугольнике для многоугольника
   * и переходим к к информации о количестве колец.
   */
  byteNumber += 36;
  //!< Количество колец многоугольника.
  int countOfRings(
    ByteConverting::readLittleEndian<int>(shapeFileBuffer, byteNumber));
  //! Корректируем количество внутренних колец для многоугольника.
  polygon.inners().resize(countOfRings - 1);

  //! Переходим к информации о количестве вершин многоугольника.
  byteNumber += 4;
  /*!< 
   * Суммарное количество вершин многоугольника
   * (считаются также замыкающие кольца вершины, равные первым вершинам колец).
   */
  int countOfPoints(
    ByteConverting::readLittleEndian<int>(shapeFileBuffer, byteNumber));

  //! Переходим к информации о расположении колец в наборе данных.
  byteNumber += 4;
  /*!< 
   * Смещение,
   * соответствующее началу массива индексов
   * начальных точек колец многоугольника.
   */
  long startOfRings(byteNumber);
  /*!<
   * Смещение, соответствующее началу массива вершин многоугольника.
   */
  long startOfPoints(byteNumber + 4 * countOfRings);
  //! Перемещаемся в начало массива вершин.
  byteNumber = startOfPoints;

  //!< Расположение начальной точки текущего кольца.
  long ringOffset(byteNumber);
  /*!<
   * Расположение начальной точки следующего кольца
   * или расположение первого байта данных, не принадлежащих текущему кольцу,
   * если оно является последним для многоугольника.
   */
  long nextRingOffset(
    (countOfRings > 1) ?
    startOfPoints + 20 * ByteConverting::readLittleEndian<int>(
                           shapeFileBuffer,
                           startOfRings + 4)
    :
    startOfPoints + 20 * countOfPoints);
  //!< Количество вершин в текущем кольце.
  int countOfVertices((nextRingOffset - ringOffset) / 20);
  polygon.outer().resize(countOfVertices);
  //! Заполняем внешнее кольцо.
  for (int numOfVertex(0); numOfVertex < countOfVertices; ++numOfVertex)
  {
    //! Считываем вершину.
    ByteConverting::readPoint(
      shapeFileBuffer,
      byteNumber,
      polygon.outer()[numOfVertex],
      isNeedToConvertFromDegrees);
    byteNumber += 16;
  }
  //! Заполняем внутренние кольца.
  for (int numOfRing(1); numOfRing < countOfRings; ++numOfRing)
  {
    //! Бывшее "следующее" кольцо -- теперь уже не следующее, а нынешнее.
    ringOffset = nextRingOffset;
    //! Вычисляем расположение начала нового "следующего" конца.
    nextRingOffset =
      (((numOfRing + 1) < countOfRings) ?
        startOfPoints + 20 * ByteConverting::readLittleEndian<int>(
          shapeFileBuffer,
          startOfRings + 4 * (numOfRing + 1)) :
        startOfPoints + 20 * countOfPoints);
    //! Фиксируем количество вершин в текущем кольце.
    countOfVertices = (nextRingOffset - ringOffset) / 20;
    //! Задаём кольцу соответствующую вместимость.
    polygon.inners()[numOfRing - 1].resize(countOfVertices);
    //! Заполняем внутреннее кольцо многоугольника.
    for (int numOfVertex(0); numOfVertex < countOfVertices; ++numOfVertex)
    {
      ByteConverting::readPoint(
        shapeFileBuffer,
        byteNumber,
        polygon.inners()[numOfRing - 1][numOfVertex],
        isNeedToConvertFromDegrees);
      byteNumber += 16;
    }
  }
  //! Наконец, задаём правильное расположение начала следующей записи.
  byteNumber = initialByteNumber + 8 + lengthOfContent;
}
//=============================================================================
/*!
 * \brief importPolygonFromShapeFile -- функция, считывающая многоугольник
 * из .shp-файла, содержащего многоугольник.
 *
 * \param[in] path -- путь к файлу, из которого нужно считывать.
 *
 * \param[in] isNeedToConvertFromDegrees -- требуется ли
 * конвертировать значения координат вершин из градусов в радианы после чтения.
 *
 * \return искомый многоугольник, если считывание прошло успешно,
 * иначе -- объект, созданный конструктором по умолчанию.
 */
template <typename Point_Type>
PrimaryEntities::Polygon<Point_Type>
importPolygonFromShapeFile(
  const std::string& path,
  bool isNeedToConvertFromDegrees = false) noexcept
{
  PrimaryEntities::Polygon<Point_Type> polygon;
  //!< Буфер для заголовка файла.
  std::vector<char> fileBuf;
  try
  {
    std::fstream inputFileStream(
      path.c_str(),
      std::ios_base::in | std::ios_base::binary);
    if (!inputFileStream.is_open())
      return polygon;

    inputFileStream.seekg(0, std::ios_base::end);
    //!< Размер файла (в байтах).
    long fileSize(static_cast<long>(inputFileStream.tellg()));
    inputFileStream.seekg(0, std::ios_base::beg);
    if (fileSize < 100)
      return polygon;


    fileBuf.resize(100);
    //! Считываем заголовок файла.
    inputFileStream.read(fileBuf.data(), 100);

    //! Теперь можно прочитать необходимую информацию из заголовка.
    if (HeaderShapeFile::fileCode(fileBuf.data(), 0) != SHP_FILE_CODE)
      return polygon;
    if (HeaderShapeFile::shapeType(fileBuf.data(), 32) != 5)
      return polygon;

    /*!
     * Если перед нами действительно файл, содержащий многоугольники,
     * считываем информацию о первой записи (первом многоугольнике).
     * Если быть точнее, считываем заголовок.
     */
    inputFileStream.read(fileBuf.data(), 8);
    //!< Размер записи о первом многоугольнике.
    int sizeOfFirstPolygonRecord(getSizeOfRecord(fileBuf.data()));
    /*!
     * Перераспределяем память,
     * чтобы её хватило ровно под одну запись о многоугольнике.
     */
    fileBuf.resize(sizeOfFirstPolygonRecord + 8);
    //! Заголовок также требуется скопировать в буфер заново.
    inputFileStream.seekg(-8, std::ios_base::cur);
    //! Считываем запись о первом многоугольнике.
    inputFileStream.read(fileBuf.data(), sizeOfFirstPolygonRecord + 8);
  }
  catch (...)
  {
    return polygon;
  }
  //!< Сдвиг относительно начала файлового буффера.
  long byteNumberInPolygonRecord(0);
  //! Записываем первый многоугольник.
  readSinglePolygonFromShapeFile(
    fileBuf.data(),
    byteNumberInPolygonRecord,
    polygon,
    isNeedToConvertFromDegrees);

  return polygon;
}
//=============================================================================
GeoPolygon2D importGeoPolygon2DFromShapeFile(const std::string& path) noexcept
{
  return importPolygonFromShapeFile<GeoPoint2D>(path);
}
//=============================================================================
GeoPolygon2D
importGeoPolygon2DFromShapeFileInDegrees(const std::string& path) noexcept
{
  return importPolygonFromShapeFile<GeoPoint2D>(path, true);
}
//=============================================================================
Polygon2D importPolygon2DFromShapeFile(const std::string& path) noexcept
{
  return importPolygonFromShapeFile<Point2D>(path);
}
//=============================================================================
/*!
 * \brief importBoxFromShapeFile -- функция, считывающая
 * прямоугольник, стороны которого параллельны координатным осям из .shp-файла,
 * содержащего такой прямоугольник.
 *
 * \param[in] path -- путь к файлу, из которого нужно считывать.
 *
 * \param[in] isNeedToConvertFromDegrees -- требуется ли
 * конвертировать значения координат вершин из градусов в радианы после чтения.
 *
 * \return искомый прямоугольник, если считывание прошло успешно,
 * иначе -- объект, созданный конструктором по умолчанию.
 */
template <typename Point_Type>
PrimaryEntities::Box<Point_Type>
importBoxFromShapeFile(
  const std::string& path,
  bool isNeedToConvertFromDegrees = false) noexcept
{
  //! Считываем многоугольник из файла.
  PrimaryEntities::Box<Point_Type> box;
  PrimaryEntities::Polygon<Point_Type> polygon(
    importPolygonFromShapeFile<Point_Type>(path));
  /*!
   * Далее просто проверяем,
   * является ли считанный многоугольник прямоугольником.
   */
  if ((polygon.outer().size() != 5) || (polygon.inners().size() != 0))
    return box;
  if (   (polygon.outer()[0].getCoordinates().x
        != polygon.outer()[4].getCoordinates().x)
      || (polygon.outer()[0].getCoordinates().y
        != polygon.outer()[4].getCoordinates().y))
    return box;

  double minCornerX(0.);
  double minCornerY(0.);
  double maxCornerX(0.);
  double maxCornerY(0.);
  double temporaryCoordinateX(0.);
  double temporaryCoordinateY(0.);
  for (int i(0); i < 4; ++i)
  {
    minCornerX = polygon.outer()[i].getCoordinates().x;
    minCornerY = polygon.outer()[i].getCoordinates().y;
    maxCornerX = polygon.outer()[(i + 2) % 4].getCoordinates().x;
    maxCornerY = polygon.outer()[(i + 2) % 4].getCoordinates().y;
    temporaryCoordinateX = polygon.outer()[i + 1].getCoordinates().x;
    temporaryCoordinateY = polygon.outer()[i + 1].getCoordinates().y;
    if ((minCornerX < maxCornerX)
        && (minCornerY < maxCornerY)
        && (minCornerX == temporaryCoordinateX)
        && (maxCornerY == temporaryCoordinateY))
    {
      temporaryCoordinateX = polygon.outer()[(i + 3) % 4].getCoordinates().x;
      temporaryCoordinateY = polygon.outer()[(i + 3) % 4].getCoordinates().y;
      if ((minCornerY == temporaryCoordinateY)
          && (maxCornerX == temporaryCoordinateX))
      {
        if (isNeedToConvertFromDegrees)
        {
          minCornerX = Degree(minCornerX).toRadians().get();
          minCornerY = Degree(minCornerY).toRadians().get();
          maxCornerX = Degree(maxCornerX).toRadians().get();
          maxCornerY = Degree(maxCornerY).toRadians().get();
        }

        geos::geom::Envelope geosEnvelope(minCornerX, maxCornerX,
                                          minCornerY, maxCornerY);

        box = PrimaryEntities::fromGeosBox<Point_Type>(&geosEnvelope);
        break;
      }
    }
  }
  return box;
}
//=============================================================================
GeoBox2D importGeoBox2DFromShapeFile(const std::string& path) noexcept
{
  return importBoxFromShapeFile<GeoPoint2D>(path);
}
//=============================================================================
GeoBox2D importGeoBox2DFromShapeFileInDegrees(const std::string& path) noexcept
{
  return importBoxFromShapeFile<GeoPoint2D>(path, true);
}
//=============================================================================
Box2D importBox2DFromShapeFile(const std::string& path) noexcept
{
  return importBoxFromShapeFile<Point2D>(path);
}
//=============================================================================
/*!
 * \brief importMultiPolygonFromShapeFile -- функция, считывающая
 * множество многоугольников из .shp-файла,
 * содержащего множество многоугольников.
 *
 * \param[in] path -- путь к файлу, из которого нужно считывать.
 *
 * \param[in] isNeedToConvertFromDegrees -- требуется ли
 * конвертировать значения координат вершин из градусов в радианы после чтения.
 *
 * \return искомое множество многоугольников, если считывание прошло успешно,
 * иначе -- объект, созданный конструктором по умолчанию.
 */
template <typename Point_Type>
PrimaryEntities::MultiPolygon<Point_Type>
importMultiPolygonFromShapeFile(
  const std::string& path,
  bool isNeedToConvertFromDegrees = false) noexcept
{
  PrimaryEntities::MultiPolygon<Point_Type> multiPolygon;

  try
  {
    std::fstream inputFileStream(
      path.c_str(),
      std::ios_base::in | std::ios_base::binary);
    if (!inputFileStream.is_open())
      return multiPolygon;

    inputFileStream.seekg(0, std::ios_base::end);
    //!< Размер файла (в байтах).
    long fileSize(static_cast<long>(inputFileStream.tellg()));
    inputFileStream.seekg(0, std::ios_base::beg);
    if (fileSize < 100)
      return multiPolygon;

    //!< Буфер для всего файла.
    std::vector<char> fileBuf;
    fileBuf.resize(fileSize);

    /*!
     * Считываем весь файл в буфер
     * (всё равно, вероятнее всего, нам придётся читать его целиком).
     */
    inputFileStream.read(fileBuf.data(), fileSize);

  if (HeaderShapeFile::fileCode(fileBuf.data(), 0) != SHP_FILE_CODE)
    return multiPolygon;
  if (HeaderShapeFile::shapeType(fileBuf.data(), 32) != 5)
    return multiPolygon;

  //!< Текущий сдвиг относительно начала файла (в байтах).
  long byteNumber(100);
  //! Каждый цикл соответствует одному многоугольнику.
  while (byteNumber < fileSize)
  {
    PrimaryEntities::Polygon<Point_Type> polygon;
    readSinglePolygonFromShapeFile(
      fileBuf.data(),
      byteNumber,
      polygon,
      isNeedToConvertFromDegrees);
    multiPolygon.emplace_back(polygon);
  }
  }
  catch (...)
  {
    return PrimaryEntities::MultiPolygon<Point_Type>();
  }
  return multiPolygon;
}
//=============================================================================
GeoMultiPolygon2D
importGeoMultiPolygon2DFromShapeFile(const std::string& path) noexcept
{
  return importMultiPolygonFromShapeFile<GeoPoint2D>(path);
}
//=============================================================================
GeoMultiPolygon2D
importGeoMultiPolygon2DFromShapeFileInDegrees(const std::string& path) noexcept
{
  return importMultiPolygonFromShapeFile<GeoPoint2D>(path, true);
}
//=============================================================================
MultiPolygon2D
importMultiPolygon2DFromShapeFile(const std::string& path) noexcept
{
  return importMultiPolygonFromShapeFile<Point2D>(path);
}
//=============================================================================