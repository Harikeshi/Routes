#include <Geodesic/Operations/Conversion.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeodesicEntities.hpp>
#include <PrimaryEntities/GeometryEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic 
{
namespace Operations 
{
namespace Tests
{
//==============================================================================
/*!
* \brief Тестирование перевода GeoBox2D в строку для Postgis
*/
TEST( ConversionTest, BoxToPostgisString )
{
  //! Исходная рамка
  GeoBox2D box( GeoPoint2D( Degree( 0 ), Degree( 0 ) ),
    GeoPoint2D( Degree( 10 ), Degree( 10 ) ) );

  //! Переводим в строку
  std::string boxStr = boxToPostgisString( box, 4 );

  //! Проверяем
  std::string controlStr( "SRID=4326;POLYGON((0.0000 0.0000,0.1745 0.0000,"
    "0.1745 0.1745,0.0000 0.1745,0.0000 0.0000))" );

  ASSERT_EQ( controlStr, boxStr );
}
//==============================================================================
/*!
* \brief Тестирование перевода из строки в GeoBox2D
*/
TEST( ConversionTest, BoxFromStdString )
{
  //! Исходная строка
  std::string boxStr( "((10., 10.), (0., 0.))" );

  //! Получаем рамку
  GeoBox2D box = boxFromStdString( boxStr );

  //! Проверяем
  ASSERT_EQ( GeoPoint2D( Degree( 0. ), Degree( 0. ) ), box.min_corner( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 10. ), Degree( 10. ) ), box.max_corner( ) );
}
//==============================================================================
} //! Tests
} //! Operations
} //! Geodesic
//==============================================================================
