#include <Geometry/Objects/Sector.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
/*!
* \brief Тестирование конструкторов класса Sector
*/
TEST( SectorTest, Constructors )
{
  // по умолчанию создаем (0, 0)
  ASSERT_EQ( std::make_pair( Radian( 0. ), Radian( 0. ) ),
    Sector( ).get( ) );
  // можем создавать сектор из радианных углов
  ASSERT_EQ( std::make_pair( Radian( M_PI / 4 ), Radian( M_PI / 3 ) ),
    Sector( Radian( M_PI / 4 ), Radian( M_PI / 3 ) ).get( ) );
  // можем создавать сектор из градусных углов
  ASSERT_EQ( std::make_pair( Radian( M_PI / 4 ), Radian( M_PI / 3 ) ),
    Sector( Degree( 45 ), Degree( 60 ) ).get( ) );
  // можем скручивать лишние периоды
  ASSERT_EQ( std::make_pair( Radian( M_PI / 4 ), Radian( M_PI / 3 ) ),
    Sector( Radian( 2 * M_PI + M_PI / 4 ), Radian( M_PI / 3 - 6 * M_PI ) ).get( ) );
  // скручивание периодов не мешает задать полный сектор
  ASSERT_EQ( std::make_pair( Radian( 0 ), Radian( 2 * M_PI ) ),
    Sector( Radian( 0 ), Radian( 2 * M_PI ) ).get( ) );
}
//==============================================================================
/*!
* \brief Тестирование getters класса Sector
*/
TEST( SectorTest, Getters )
{
  Sector sect( Radian( M_PI / 4 ), Radian( M_PI / 3 ) );
  // можно получить пару значений в радианах
  ASSERT_EQ( std::make_pair( Radian( M_PI / 4 ), Radian( M_PI / 3 ) ),
    sect.get( ) );
  // можно получить пару значений в градусах
  ASSERT_EQ( std::make_pair( Degree( 45 ), Degree( 60 ) ),
    sect.getDegree( ) );
  // можно раздельно получить начало и конец сектора
  ASSERT_EQ( Radian( M_PI / 4 ), sect.getBegAngle( ) );
  ASSERT_EQ( Radian( M_PI / 3 ), sect.getEndAngle( ) );
}
//==============================================================================
/*!
* \brief Тестирование setters класса Sector
*/
TEST( SectorTest, Setters )
{
  Sector sect;
  ASSERT_EQ( std::make_pair( Radian( 0 ), Radian( 0 ) ), sect.get( ) );

  // можно задать значения в радианах
  sect.set( Radian( M_PI / 4 ), Radian( M_PI / 3 ) );
  ASSERT_EQ( std::make_pair( Radian( M_PI / 4 ), Radian( M_PI / 3 ) ),
    sect.get( ) );

  // можно задать значения в градусах
  sect.set( Degree( 60 ), Degree( 45 ) );
  ASSERT_EQ( std::make_pair( Radian( M_PI / 3 ), Radian( M_PI / 4 ) ),
    sect.get( ) );
}
//==============================================================================
} // ! Tests
} // ! Geometry