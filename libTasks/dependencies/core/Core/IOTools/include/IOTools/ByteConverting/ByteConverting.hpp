#ifndef IO_TOOLS_BYTE_CONVERTING_BYTE_CONVERTING_HPP
#define IO_TOOLS_BYTE_CONVERTING_BYTE_CONVERTING_HPP
//=============================================================================
//! MathTools
#include <MathTools/AngularUnits/Radian.hpp>
//! PrimaryEntities
#include <PrimaryEntities/PointConverter.hpp>
//! std
#ifdef __linux__
#include <byteswap.h>
#else
#include <stdlib.h>
#endif
//=============================================================================
namespace ByteConverting
{
//=============================================================================
/*!
 * \brief readBigEndianInt -- функция, считывающая целое число в порядке байтов
 * от "старшего" к "младшему" длиной в 4 байта со знаком.
 *
 * \param[in] fileBuf -- указатель на массив байтов,
 * из которого нужно осуществить чтение.
 *
 * \param[in] offset -- смещение относительно fileBuf (в байтах),
 * с которого нужно начинать чтение числа.
 *
 * \return искомое число.
 *
 * \details начиная с fileBuf + offset должны иметься 4 подряд идущих
 * доступных байта.
 */
inline int readBigEndianInt(const char* fileBuf, int offset = 0)
{
#ifdef __linux__
  return __builtin_bswap32(*((unsigned int*)(fileBuf + offset)));
#else
  return _byteswap_ulong(*((unsigned int*)(fileBuf + offset)));
#endif
}
//=============================================================================
/*!
 * \brief readBigEndianUnsignedInt -- функция,
 * считывающая целое число в порядке байтов
 * от "старшего" к "младшему" длиной в 4 байта без знака.
 *
 * \param[in] fileBuf -- указатель на массив байтов,
 * из которого нужно осуществить чтение.
 *
 * \param[in] offset -- смещение относительно fileBuf (в байтах),
 * с которого нужно начинать чтение числа.
 *
 * \return искомое число.
 *
 * \details начиная с fileBuf + offset должны иметься 4 подряд идущих
 * доступных байта.
 */
inline unsigned int
readBigEndianUnsignedInt(const char* fileBuf, int offset = 0)
{
#ifdef __linux__
  return __builtin_bswap32(*((unsigned int*)(fileBuf + offset)));
#else
  return _byteswap_ulong(*((unsigned int*)(fileBuf + offset)));
#endif
}
//=============================================================================
/*!
 * \brief readBigEndianShort -- функция,
 * считывающая целое число в порядке байтов
 * от "старшего" к "младшему" длиной в 2 байта со знаком.
 *
 * \param[in] fileBuf -- указатель на массив байтов,
 * из которого нужно осуществить чтение.
 *
 * \param[in] offset -- смещение относительно fileBuf (в байтах),
 * с которого нужно начинать чтение числа.
 *
 * \return искомое число.
 *
 * \details начиная с fileBuf + offset должны иметься 2 подряд идущих
 * доступных байта.
 */
inline short readBigEndianShort(const char* fileBuf, int offset = 0)
{
#ifdef __linux__
  return __builtin_bswap16(*((unsigned short*)(fileBuf + offset)));
#else
  return _byteswap_ushort(*((unsigned short*)(fileBuf + offset)));
#endif
}
//=============================================================================
/*!
 * \brief readBigEndianUnsignedShort -- функция,
 * считывающая целое число в порядке байтов
 * от "старшего" к "младшему" длиной в 2 байта без знака.
 *
 * \param[in] fileBuf -- указатель на массив байтов,
 * из которого нужно осуществить чтение.
 *
 * \param[in] offset -- смещение относительно fileBuf (в байтах),
 * с которого нужно начинать чтение числа.
 *
 * \return искомое число.
 *
 * \details начиная с fileBuf + offset должны иметься 2 подряд идущих
 * доступных байта.
 */
inline unsigned short
readBigEndianUnsignedShort(const char* fileBuf, int offset = 0)
{
#ifdef __linux__
  return __builtin_bswap16(*((unsigned short*)(fileBuf + offset)));
#else
  return _byteswap_ushort(*((unsigned short*)(fileBuf + offset)));
#endif
}
//=============================================================================
/*!
 * \brief writeBigEndianInt -- функция,
 * записывающая целое число в порядке байтов
 * от "старшего" к "младшему" длиной в 4 байта со знаком.
 *
 * \param[in] fileBuf -- указатель на массив байтов,
 * в который нужно осуществить запись.
 *
 * \param[in] startIndex -- смещение относительно fileBuf (в байтах),
 * с которого нужно начинать запись числа.
 *
 * \param[in] value -- число, которое требуется записать.
 *
 * \details начиная с fileBuf + startIndex должны иметься 4 подряд идущих
 * доступных байта.
 */
inline void writeBigEndianInt(char* fileBuf, int startIndex, int value)
{
#ifdef __linux__
  *((unsigned int*)(fileBuf + startIndex)) = __builtin_bswap32(value);
#else
  *((unsigned int*)(fileBuf + startIndex)) = _byteswap_ulong(value);
#endif
}
//=============================================================================
/*!
 * \brief readLittleEndian -- функция, считывающая целое число в порядке байтов
 * от "младшего" к "старшему" длиной.
 *
 * \param[in] fileBuf -- указатель на массив байтов,
 * из которого нужно осуществить чтение.
 *
 * \param[in] offset -- смещение относительно fileBuf (в байтах),
 * с которого нужно начинать чтение числа.
 *
 * \return искомое число.
 *
 * \details начиная с fileBuf + offset,
 * должны иметься столько подряд идущих байтов,
 * сколько занимает в памяти целочисленный тип-параметр шаблона.
 */
template <typename IntegerType>
inline IntegerType readLittleEndian(const char* fileBuf, int offset = 0)
{
  return *((IntegerType*)(fileBuf + offset));
}
//=============================================================================
/*!
 * \brief writeLittleEndian -- функция,
 * записывающая целое число в порядке байтов
 * от "младшего" к "старшему".
 *
 * \param[in] fileBuf -- указатель на массив байтов,
 * в который нужно осуществить запись.
 *
 * \param[in] value -- число, которое требуется записать.
 *
 * \param[in] offset -- смещение относительно fileBuf (в байтах),
 * с которого нужно начинать запись числа.
 *
 * \details начиная с fileBuf + startIndex
 * должно иметься столько доступных байт,
 * сколько занимает в памяти тип числа.
 */
template <typename IntegerType>
inline void writeLittleEndian(char* fileBuf, IntegerType value, int offset = 0)
{
  *((IntegerType*)(fileBuf + offset)) = value;
}
//=============================================================================
/*!
 * \brief readDouble -- функция, считывающая число с плавающей точкой
 * в стандарте IEEE длиной 8 байтов из массива байтов.
 *
 * \param[in] fileBuf -- указатель на массив байтов,
 * из которого нужно осуществить чтение.
 *
 * \param[in] startIndex -- смещение относительно fileBuf (в байтах),
 * с которого нужно начинать чтение числа.
 *
 * \return искомое число.
 *
 * \details начиная с fileBuf + startIndex должны иметься 8 подряд идущих
 * доступных байтов.
 */
inline double readDouble(const char* fileBuf, int startIndex)
{
  return *((double*)(fileBuf + startIndex));
}
//=============================================================================
/*!
 * \brief writeDouble -- функция, записывающая число с плавающей точкой
 * в стандарте IEEE длиной 8 байтов в массив байтов.
 *
 * \param[in] fileBuf -- указатель на массив байтов,
 * в который нужно осуществить запись.
 *
 * \param[in] startIndex -- смещение относительно fileBuf (в байтах),
 * с которого нужно начинать запись числа.
 *
 * \param[in] value -- число, которое требуется записать.
 *
 * \details начиная с fileBuf + startIndex должны иметься 8 подряд идущих
 * доступных байтов.
 */
inline void writeDouble(char* fileBuf, int startIndex, double value)
{
  *((double*)(fileBuf + startIndex)) = value;
}
//=============================================================================
/*!
 * \brief readDouble -- функция, создающая точку по двум координатам.
 *
 * \param[in] coordinate1 -- первая координата.
 *
 * \param[in] coordinate2 -- вторая координата.
 *
 * \return точка.
 */
template <typename Point_Type>
Point_Type createPointFromDouble(double coordinate1, double coordinate2)
{
  geos::geom::GeometryFactory::Ptr geometryFactory
    = geos::geom::GeometryFactory::create();

  std::unique_ptr<geos::geom::Point> geosPoint = geometryFactory->createPoint(
    geos::geom::CoordinateXY(coordinate1, coordinate2));

  return PrimaryEntities::fromGeosPoint<Point_Type>(geosPoint.get());
}
//=============================================================================
/*!
 * \brief readPoint -- функция, считывающая координаты точки
 * на декартовой плоскости из массива байтов
 * как два подряд идущих числа с плавающей точкой в стандартном формате IEEE.
 *
 * \param[in] fileBuf -- указатель на массив байтов,
 * из которого нужно осуществить чтение.
 *
 * \param[in] startIndex -- смещение относительно fileBuf (в байтах),
 * с которого нужно начинать чтение точки.
 *
 * \param[out] point -- точка, координаты которой требуется прочитать.
 *
 * \param[in] isNeedToConvertFromDegrees -- требуется ли
 * конвертация координат вершин из градусов в радианы после чтения.
 *
 * \details начиная с fileBuf + startIndex должны иметься 16 подряд идущих
 * доступных байтов;
 * первой по порядку читается координата x,
 * сразу после неё читается координата y.
 */
template <typename Point_Type>
void
readPoint(
  const char* fileBuf,
  int startIndex,
  Point_Type& point,
  bool isNeedToConvertToDegrees = false)
{
  //!< Сырое значение координаты x в точке.
  double actualX(ByteConverting::readDouble(fileBuf, startIndex));
  //!< Сырое значение координаты y в точке.
  double actualY(ByteConverting::readDouble(fileBuf, startIndex + 8));
  if (isNeedToConvertToDegrees)
  {
    actualX = Degree(actualX).toRadians().get();
    actualY = Degree(actualY).toRadians().get();
  }

  point = createPointFromDouble<Point_Type>(actualX, actualY);
}
//=============================================================================
/*!
 * \brief writePoint -- функция, записывающая координаты точки
 * на декартовой плоскости в массив байтов
 * как два подряд идущих числа с плавающей точкой в стандартном формате IEEE.
 *
 * \param[in] fileBuf -- указатель на массив байтов,
 * в который нужно осуществить запись.
 *
 * \param[in] startIndex -- смещение относительно fileBuf (в байтах),
 * с которого нужно начинать запись точки.
 *
 * \param[in] point -- точка, координаты которой требуется записать.
 *
 * \param[in] isNeedToConvertToDegrees -- требуется ли
 * предварительная конвертация координат вершин из радиан в градусы.
 *
 * \details начиная с fileBuf + startIndex должны иметься 16 подряд идущих
 * доступных байтов;
 * первой по порядку записывается координата x,
 * сразу после неё записывается координата y.
 */
template <typename Point_Type>
void
writePoint(
  char* fileBuf,
  int startIndex,
  const Point_Type& point,
  bool isNeedToConvertToDegrees = false)
{
  //!< Сырое значение координаты x в точке.
  double actualX(point.getCoordinates().x);
  //!< Сырое значение координаты y в точке.
  double actualY(point.getCoordinates().y);
  if (isNeedToConvertToDegrees)
  {
    actualX = Radian(actualX).toDegrees().get();
    actualY = Radian(actualY).toDegrees().get();
  }
  writeDouble(fileBuf, startIndex, actualX);
  writeDouble(fileBuf, startIndex + 8, actualY);
}
//=============================================================================
} //! ByteConverting
//=============================================================================
#endif //! IO_TOOLS_BYTE_CONVERTING_BYTE_CONVERTING_HPP