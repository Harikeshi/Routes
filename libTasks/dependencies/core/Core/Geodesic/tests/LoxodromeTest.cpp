#include <Geodesic/GeoLineTools/Helper/Loxodrome.hpp>

#include <Geodesic/GeoLineTools/LoxodromeTools.hpp>
#include <Geodesic/GeodesicException.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование поиска точки на локсодромии
*/
TEST( LoxodromeTest, pointPosition )
{
  //! Создание инструмента
  LoxodromeTools loxodromeTools = LoxodromeTools( Ellipsoid( EL_WGS_84 ) );


  //! Локсодромия на экваторе
  Loxodrome loxodrome = loxodromeTools.createLoxodrome(
    GeoPoint2D( ),
    GeoPoint2D( Degree( 0 ), Degree( 10 ) ) );
  ASSERT_EQ(
    GeoPoint2D( Degree( 0 ), Degree( 10 ) ),
    loxodrome.pointPosition( loxodrome.getLength( ) ) );
  ASSERT_EQ(
    GeoPoint2D( Degree( 0 ), Degree( -10 ) ),
    loxodrome.pointPosition( -loxodrome.getLength( ) ) );

  //! Создание локсодромии как точки
  ASSERT_THROW(loxodromeTools.createLoxodrome(GeoPoint2D(), GeoPoint2D()),
               GeodesicFailure);
  try
  {
    loxodrome = loxodromeTools.createLoxodrome( GeoPoint2D( ), GeoPoint2D( ) );
  }
  catch( const GeodesicFailure& exception )
  {
    ASSERT_EQ( exception.getLib( ), geodesicLibTitle );
    ASSERT_EQ(
      exception.getCode( ),
      GeodesicEnumFailure::LoxodromeThroughOnePoint );
  }

  //! Локсодромия на полюсе не работает на поиск точки!
  loxodrome = loxodromeTools.createLoxodrome(
    GeoPoint2D( Degree( 90 ), Degree( 0 ) ),
    GeoPoint2D( Degree( 90 ), Degree( 10 ) ) );
  ASSERT_TRUE(
    std::isnan( loxodrome.pointPosition( 10000. ).getLongitude( ).get( ) ) );

  //! Локсодромия через полюс
  loxodrome = loxodromeTools.createLoxodrome(
    GeoPoint2D( Degree( 85 ), Degree( 0 ) ),
    GeoPoint2D( Degree( 90 ), Degree( 0 ) ) );
  ASSERT_TRUE(
    std::isnan( loxodrome.pointPosition( 2 * loxodrome.getLength( ) )
      .getLongitude( ).get( ) ) );

  //! Некорректная локсодромия.
  ASSERT_THROW(loxodromeTools.createLoxodrome(
                 GeoPoint2D(Degree(85), Degree(0)),
                 Radian(0), 15000000.),
               GeodesicFailure);
  try
  {
    loxodrome = loxodromeTools.createLoxodrome(
      GeoPoint2D( Degree( 85 ), Degree( 0 ) ),
      Radian( 0 ),
      15000000. );
  }
  catch( const GeodesicFailure& exception )
  {
    ASSERT_EQ( geodesicLibTitle, exception.getLib( ) );
    ASSERT_EQ( GeodesicEnumFailure::LoxodromeThroughPole, exception.getCode( ) );
  }

  //! Локсодромия через 180 меридиан
  loxodrome = loxodromeTools.createLoxodrome(
    GeoPoint2D( Degree( 0 ), Degree( 170 ) ),
    GeoPoint2D( Degree( 0 ), Degree( 175 ) ) );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( -175 ) ),
    loxodrome.pointPosition( 3 * loxodrome.getLength( ) ) );

  loxodrome = loxodromeTools.createLoxodrome(
    GeoPoint2D( Degree( 0 ), Degree( 185 ) ),
    GeoPoint2D( Degree( 0 ), Degree( 190 ) ) );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 175 ) ),
    loxodrome.pointPosition( -2 * loxodrome.getLength( ) ) );
}
//==============================================================================
/*!
* \brief Тестирование разбиения локсодромии на равные части
*/
TEST( LoxodromeTest, Divide )
{
  //! Создание инструмента
  LoxodromeTools loxodromeTools = LoxodromeTools( Ellipsoid( EL_WGS_84 ) );

  //! Разбиение локсодромии, проходящей вдоль экватора.
  //! Изначально задана длина равномерного разбиения
  Loxodrome loxodrome = loxodromeTools.createLoxodrome(
    GeoPoint2D( ),
    GeoPoint2D( Degree( 0 ), Degree( 10 ) ) );
  auto points = loxodrome.divide( loxodrome.getLength( ) / 10 );

  ASSERT_EQ( 11, points.size( ) );
  for( int i = 0; i < points.size( ); i++ )
    ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( i * 1 ) ), points[i] );

  //! Разбиение локсодромии, проходящей вдоль экватора.
  //! Разбивать не надо. Длина сегмента больше длины локосодромии
  loxodrome = loxodromeTools.createLoxodrome( GeoPoint2D( ),
    GeoPoint2D( Degree( 10 ), Degree( 0 ) ) );

  points = loxodrome.divide( 1200000. );
  ASSERT_EQ( 2, points.size( ) );
  ASSERT_EQ( GeoPoint2D( ), points[0] );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), points[1] );
}
//==============================================================================
/*!
* \brief Тестирование разбиения локсодромии на равные части с дозаписью и
*        параметрами
*/
TEST( LoxodromeTest, DivideWithParams )
{
  using namespace BasicMath;

  //! Создание инструмента
  LoxodromeTools loxodromeTools = LoxodromeTools( Ellipsoid( EL_WGS_84 ) );

  std::vector<GeoPoint2D> points;
  double lengthSegment;

  //! Разбиение локсодромии, проходящей вдоль экватора.
  //! Изначально задана длина равномерного разбиения
  //! Граничные точки не записываем
  Loxodrome loxodrome = loxodromeTools.createLoxodrome(
    GeoPoint2D( ),
    GeoPoint2D( Degree( 0 ), Degree( 10 ) ) );
  lengthSegment = loxodrome.divide( points, loxodrome.getLength( ) / 10,
    false, false );

  ASSERT_TRUE( isEqual( loxodrome.getLength( ) / 10, lengthSegment ) );
  ASSERT_EQ( 9, points.size( ) );
  for( int i = 0; i < points.size( ); i++ )
    ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( i + 1 ) ), points[i] );

  //! Разбиение локсодромии, проходящей вдоль экватора.
  //! Разбивать не надо. Длина сегмента больше длины локосодромии
  //! Вызываем дважды и последовательно дозаписываем сначала точку завершения, 
  //! а потом точку начала
  points.clear( );

  loxodrome = loxodromeTools.createLoxodrome( GeoPoint2D( ),
    GeoPoint2D( Degree( 10 ), Degree( 0 ) ) );

  lengthSegment = loxodrome.divide( points, 1200000., false );

  ASSERT_TRUE( isEqual( loxodrome.getLength( ), lengthSegment ) );

  lengthSegment = loxodrome.divide( points, 1200000., true, false );

  ASSERT_TRUE( isEqual( loxodrome.getLength( ), lengthSegment ) );
  ASSERT_EQ( 2, points.size( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), points[0] );
  ASSERT_EQ( GeoPoint2D( ), points[1] );
}
//==============================================================================
} //! Tests
} //! Geodesic
