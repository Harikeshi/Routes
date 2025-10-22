#include <IOTools/ByteConverting/ByteConverting.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//=============================================================================
namespace ByteConverting
{
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование чтения знакового целого числа длиной 4 байта
 * в порядке следования байтов от "старшего" к "младшему" из массива байтов.
 */
TEST( ByteConvertingTest, ReadBigEndianInt )
{
#pragma warning( disable : 4309 ) //!< Игнорирование предупреждения 4309 о
  //!  возможной потери части значения, так 
  //!  как идёт приведение к знаковому, но 
  //! здесь в тестах мы знаем, что значения не
  //! теряем  
  std::vector<char> bytes{
    static_cast< char >( 0xED ),
    static_cast< char >( 0xAF ),
    static_cast< char >( 0xA0 ),
    static_cast< char >( 0x1F )
  };
#pragma warning( default : 4309 ) //!< Возвращение отслеживания предупреждения
  ASSERT_EQ( readBigEndianInt( bytes.data( ), 0 ), -307257313 );
}
//=============================================================================
/*!
 * \brief Тестирование чтения беззнакового целого числа длиной 4 байта
 * в порядке следования байтов от "старшего" к "младшему" из массива байтов.
 */
TEST( ByteConvertingTest, ReadBigEndianUnsignedInt )
{
#pragma warning( disable : 4309 ) //!< Игнорирование предупреждения 4309 о
  //!  возможной потери части значения, так 
  //!  как идёт приведение к знаковому, но 
  //! здесь в тестах мы знаем, что значения не
  //! теряем  
  std::vector<char> bytes{
    static_cast< char >( 0x91 ),
    static_cast< char >( 0xB8 ),
    static_cast< char >( 0x05 ),
    static_cast< char >( 0x1D )
  };
#pragma warning( default : 4309 ) //!< Возвращение отслеживания предупреждения
  ASSERT_EQ(
    readBigEndianUnsignedInt( bytes.data( ), 0 ),
    2444756253U );
}
//=============================================================================
/*!
 * \brief Тестирование чтения знакового целого числа длиной 2 байта
 * в порядке следования байтов от "старшего" к "младшему" из массива байтов.
 */
TEST( ByteConvertingTest, ReadBigEndianShort )
{
#pragma warning( disable : 4309 ) //!< Игнорирование предупреждения 4309 о
  //!  возможной потери части значения, так 
  //!  как идёт приведение к знаковому, но 
  //! здесь в тестах мы знаем, что значения не
  //! теряем  
  std::vector<char> bytes{
    static_cast< char >( 0x94 ),
    static_cast< char >( 0x73 )
  };
#pragma warning( default : 4309 ) //!< Возвращение отслеживания предупреждения
  ASSERT_EQ( readBigEndianShort( bytes.data( ), 0 ), short( -27533 ) );
}
//=============================================================================
/*!
 * \brief Тестирование чтения беззнакового целого числа длиной 2 байта
 * в порядке следования байтов от "старшего" к "младшему" из массива байтов.
 */
TEST( ByteConvertingTest, ReadBigEndianUnsignedShort )
{
#pragma warning( disable : 4309 ) //!< Игнорирование предупреждения 4309 о
  //!  возможной потери части значения, так 
  //!  как идёт приведение к знаковому, но 
  //! здесь в тестах мы знаем, что значения не
  //! теряем  
  std::vector<char> bytes{
    static_cast< char >( 0x3B ),
    static_cast< char >( 0x81 )
  };
#pragma warning( default : 4309 ) //!< Возвращение отслеживания предупреждения
  ASSERT_EQ(
    readBigEndianUnsignedShort( bytes.data( ), 0 ),
    ( unsigned short )15233U );
}
//=============================================================================
/*!
 * \brief Тестирование записи знакового целого числа длиной 4 байта
 * в порядке следования байтов от "старшего" к "младшему" в массив байтов.
 */
TEST( ByteConvertingTest, WriteBigEndianInt )
{
  std::vector<char> bytes;
  bytes.resize( 4 );
  writeBigEndianInt( bytes.data( ), 0, 499732479 );
#pragma warning( disable : 4309 ) //!< Игнорирование предупреждения 4309 о
  //!  возможной потери части значения, так 
  //!  как идёт приведение к знаковому, но 
  //! здесь в тестах мы знаем, что значения не
  //! теряем  
  ASSERT_EQ( bytes[0], static_cast< char >( 0x1D ) );
  ASSERT_EQ( bytes[1], static_cast< char >( 0xC9 ) );
  ASSERT_EQ( bytes[2], static_cast< char >( 0x4F ) );
  ASSERT_EQ( bytes[3], static_cast< char >( 0xFF ) );
#pragma warning( default : 4309 ) //!< Возвращение отслеживания предупреждения
}
//=============================================================================
/*!
 * \brief Тестирование чтения знакового целого числа длиной 4 байта
 * в порядке следования байтов от "младшего" к "старшему" из массива байтов.
 */
TEST( ByteConvertingTest, ReadLittleEndian )
{
#pragma warning( disable : 4309 ) //!< Игнорирование предупреждения 4309 о
  //!  возможной потери части значения, так 
  //!  как идёт приведение к знаковому, но 
  //! здесь в тестах мы знаем, что значения не
  //! теряем  
  std::vector<char> bytes{
    static_cast< char >( 0x53 ),
    static_cast< char >( 0x18 ),
    static_cast< char >( 0xDD ),
    static_cast< char >( 0xEF )
  };
#pragma warning( default : 4309 ) //!< Возвращение отслеживания предупреждения
  ASSERT_EQ( readLittleEndian<int>( bytes.data( ), 0 ), -270722989 );
}
//=============================================================================
/*!
 * \brief Тестирование записи знакового целого числа длиной 4 байта
 * в порядке следования байтов от "младшего" к "старшему" в массив байтов.
 */
TEST( ByteConvertingTest, WriteLittleEndian )
{
  std::vector<char> bytes;
  bytes.resize( 4 );
  writeLittleEndian( bytes.data( ), -327866745, 0 );
#pragma warning( disable : 4309 ) //!< Игнорирование предупреждения 4309 о
  //!  возможной потери части значения, так 
  //!  как идёт приведение к знаковому, но 
  //! здесь в тестах мы знаем, что значения не
  //! теряем  
  ASSERT_EQ( bytes[0], static_cast< char >( 0x87 ) );
  ASSERT_EQ( bytes[1], static_cast< char >( 0x26 ) );
  ASSERT_EQ( bytes[2], static_cast< char >( 0x75 ) );
  ASSERT_EQ( bytes[3], static_cast< char >( 0xEC ) );
#pragma warning( default : 4309 ) //!< Возвращение отслеживания предупреждения
}
//=============================================================================
/*!
 * \brief Тестирование чтения числа с плавающей точкой длиной 8 байтов
 * в стандартном формате IEEE из массива байтов.
 */
TEST( ByteConvertingTest, ReadDouble )
{
#pragma warning( disable : 4309 ) //!< Игнорирование предупреждения 4309 о
  //!  возможной потери части значения, так 
  //!  как идёт приведение к знаковому, но 
  //! здесь в тестах мы знаем, что значения не
  //! теряем  
  std::vector<char> bytes{
    static_cast< char >( 0x57 ),
    static_cast< char >( 0x1F ),
    static_cast< char >( 0xCC ),
    static_cast< char >( 0xD4 ),
    static_cast< char >( 0xB9 ),
    static_cast< char >( 0x84 ),
    static_cast< char >( 0xA5 ),
    static_cast< char >( 0x41 )
  };
#pragma warning( default : 4309 ) //!< Возвращение отслеживания предупреждения
  ASSERT_EQ( readDouble( bytes.data( ), 0 ), 180509930.398676599 );
}
//=============================================================================
/*!
 * \brief Тестирование записи числа с плавающей точкой длиной 8 байтов
 * в стандартном формате IEEE в массив байтов.
 */
TEST( ByteConvertingTest, WriteDouble )
{
  std::vector<char> bytes;
  bytes.resize( 8 );
  writeDouble( bytes.data( ), 0, -17280683.39579685 );
#pragma warning( disable : 4309 ) //!< Игнорирование предупреждения 4309 о
  //!  возможной потери части значения, так 
  //!  как идёт приведение к знаковому, но 
  //! здесь в тестах мы знаем, что значения не
  //! теряем  
  ASSERT_EQ( bytes[0], static_cast< char >( 0x14 ) );
  ASSERT_EQ( bytes[1], static_cast< char >( 0x2F ) );
  ASSERT_EQ( bytes[2], static_cast< char >( 0x55 ) );
  ASSERT_EQ( bytes[3], static_cast< char >( 0xB6 ) );
  ASSERT_EQ( bytes[4], static_cast< char >( 0xEA ) );
  ASSERT_EQ( bytes[5], static_cast< char >( 0x7A ) );
  ASSERT_EQ( bytes[6], static_cast< char >( 0x70 ) );
  ASSERT_EQ( bytes[7], static_cast< char >( 0xC1 ) );
#pragma warning( default : 4309 ) //!< Возвращение отслеживания предупреждения
}
//=============================================================================
/*!
 * \brief Тестирование записи координат точки
 * в виде двух подряд идущих чисел с плавающей точкой длиной 8 байтов
 * в стандартном формате IEEE в массив байтов.
 */
TEST( ByteConvertingTest, WritePoint )
{
  std::vector<char> bytes( 16 );
  writePoint(
    bytes.data( ),
    0,
    Point2D( -384564894.208322323, 253147459.262876298 ) );
#pragma warning( disable : 4309 ) //!< Игнорирование предупреждения 4309 о
  //!  возможной потери части значения, так 
  //!  как идёт приведение к знаковому, но 
  //! здесь в тестах мы знаем, что значения не
  //! теряем  
  ASSERT_EQ( bytes[0], static_cast< char >( 0x9D ) );
  ASSERT_EQ( bytes[1], static_cast< char >( 0x54 ) );
  ASSERT_EQ( bytes[2], static_cast< char >( 0x35 ) );
  ASSERT_EQ( bytes[3], static_cast< char >( 0x9E ) );
  ASSERT_EQ( bytes[4], static_cast< char >( 0xFE ) );
  ASSERT_EQ( bytes[5], static_cast< char >( 0xEB ) );
  ASSERT_EQ( bytes[6], static_cast< char >( 0xB6 ) );
  ASSERT_EQ( bytes[7], static_cast< char >( 0xC1 ) );
  ASSERT_EQ( bytes[8], static_cast< char >( 0xB9 ) );
  ASSERT_EQ( bytes[9], static_cast< char >( 0x97 ) );
  ASSERT_EQ( bytes[10], static_cast< char >( 0x86 ) );
  ASSERT_EQ( bytes[11], static_cast< char >( 0x86 ) );
  ASSERT_EQ( bytes[12], static_cast< char >( 0x72 ) );
  ASSERT_EQ( bytes[13], static_cast< char >( 0x2D ) );
  ASSERT_EQ( bytes[14], static_cast< char >( 0xAE ) );
  ASSERT_EQ( bytes[15], static_cast< char >( 0x41 ) );
#pragma warning( default : 4309 ) //!< Возвращение отслеживания предупреждения

  /*!
   * Проверка записи
   * с предварительной конвертацией координат из радиан в градусы.
   */
  writePoint(
    bytes.data( ),
    0,
    GeoPoint2D( Radian( 6.597 ), Radian( 7.878 ) ),
    true );

#pragma warning( disable : 4309 ) //!< Игнорирование предупреждения 4309 о
  //!  возможной потери части значения, так 
  //!  как идёт приведение к знаковому, но 
  //! здесь в тестах мы знаем, что значения не
  //! теряем  
  ASSERT_EQ( bytes[0], static_cast< char >( 0xE6 ) );
  ASSERT_EQ( bytes[1], static_cast< char >( 0x4C ) );
  ASSERT_EQ( bytes[2], static_cast< char >( 0xEA ) );
  ASSERT_EQ( bytes[3], static_cast< char >( 0xB6 ) );
  ASSERT_EQ( bytes[4], static_cast< char >( 0x04 ) );
  ASSERT_EQ( bytes[5], static_cast< char >( 0x36 ) );
  ASSERT_EQ( bytes[6], static_cast< char >( 0x7C ) );
  ASSERT_EQ( bytes[7], static_cast< char >( 0x40 ) );
  ASSERT_EQ( bytes[8], static_cast< char >( 0xAA ) );
  ASSERT_EQ( bytes[9], static_cast< char >( 0xFF ) );
  ASSERT_EQ( bytes[10], static_cast< char >( 0x6E ) );
  ASSERT_EQ( bytes[11], static_cast< char >( 0x22 ) );
  ASSERT_EQ( bytes[12], static_cast< char >( 0xAF ) );
  ASSERT_EQ( bytes[13], static_cast< char >( 0x9F ) );
  ASSERT_EQ( bytes[14], static_cast< char >( 0x77 ) );
  ASSERT_EQ( bytes[15], static_cast< char >( 0x40 ) );
#pragma warning( default : 4309 ) //!< Возвращение отслеживания предупреждения
}
//==============================================================================
/*!
 * \brief Тестирование чтения координат точки
 * в виде двух подряд идущих чисел с плавающей точкой длиной 8 байтов
 * в стандартном формате IEEE в массив байтов.
 */
TEST( ByteConvertingTest, ReadPoint )
{
  Point2D original( 426770.581594, -912750.4568 );
  std::vector<char> bytes( 16 );
  writePoint( bytes.data( ), 0, original );
  Point2D result;
  readPoint( bytes.data( ), 0, result );
  ASSERT_EQ( original, result );

  /*!
   * Проверка записи
   * с конвертацией координат из градусов в радианы.
   */
  GeoPoint2D geoOriginal( Radian( 0.709 ), Radian( 3.339 ) );
  writePoint( bytes.data( ), 0, geoOriginal, true );
  GeoPoint2D geoResult;
  readPoint( bytes.data( ), 0, geoResult, true );
  ASSERT_EQ( geoOriginal, geoResult );
}
//==============================================================================
} //! Tests
} //! ByteConverting
//==============================================================================
