#include <Geodesic/GeoLineTools/Helper/Orthodromy.hpp>

#include <Geodesic/GeoLineTools/OrthodromyTools.hpp>
#include <Geodesic/GeodesicException.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
/*!
* \brief Тестирование поиска точки на ортодромии
*/
TEST( OrthodromyTest, PointPosition )
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;


  //! Ортодромия в районе экватора
  Orthodromy orthodromy = orthodromyTools.createOrthodromy(
    GeoPoint2D( ),
    GeoPoint2D( Degree( 0 ), Degree( 10 ) ) );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 10 ) ),
    orthodromy.pointPosition( orthodromy.getLength( ) ) );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( -10 ) ),
    orthodromy.pointPosition( -orthodromy.getLength( ) ) );

  //! Создание ортодромии как точки на пересечении экватора и гринвича
  ASSERT_THROW(orthodromyTools.createOrthodromy(GeoPoint2D(), GeoPoint2D()),
               GeodesicFailure);
  try
  {
    orthodromy = orthodromyTools.createOrthodromy(
      GeoPoint2D( ),
      GeoPoint2D( ) );
  }
  catch( const GeodesicFailure& exception )
  {
    ASSERT_EQ( exception.getLib( ), geodesicLibTitle );
    ASSERT_EQ(
      exception.getCode( ),
      GeodesicEnumFailure::OrthodromyThroughOnePoint );
  }

  //! Ортодромия через полюс
  orthodromy = orthodromyTools.createOrthodromy(
    GeoPoint2D( Degree( 85 ), Degree( 0 ) ),
    GeoPoint2D( Degree( 90 ), Degree( 0 ) ) );
  ASSERT_EQ( GeoPoint2D( Degree( 85 ), Degree( -180 ) ),
    orthodromy.pointPosition( 2 * orthodromy.getLength( ) ) );

  //! Ортодромия через 180 меридиан
  orthodromy = orthodromyTools.createOrthodromy(
    GeoPoint2D( Degree( 0 ), Degree( 170 ) ),
    GeoPoint2D( Degree( 0 ), Degree( 175 ) ) );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( -175 ) ),
    orthodromy.pointPosition( 3 * orthodromy.getLength( ) ) );

  orthodromy = orthodromyTools.createOrthodromy(
    GeoPoint2D( Degree( 0 ), Degree( 185 ) ),
    GeoPoint2D( Degree( 0 ), Degree( 190 ) ) );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 175 ) ),
    orthodromy.pointPosition( -2 * orthodromy.getLength( ) ) );

}
//==============================================================================
/*!
* \brief Тестирование разбиения ортодромии на равные части
*/
TEST( OrthodromyTest, Divide )
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Разбиение ортодромии, проходяще вдоль экватора.
  //! Изначально задана длина равномерного разбиения
  Orthodromy orthodromy = orthodromyTools.createOrthodromy(
    GeoPoint2D( ),
    GeoPoint2D( Degree( 0 ), Degree( 10 ) ) );
  auto points = orthodromy.divide( orthodromy.getLength( ) / 10 );

  ASSERT_EQ( 11, points.size( ) );
  for( int i = 0; i < points.size( ); i++ )
    ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( i ) ), points[i] );

  //! Разбиение ортодромии, проходяще вдоль меридиана через полюс.
  //! Изначально задана длина неравномерного разбиения - выравнивание
  orthodromy = orthodromyTools.createOrthodromy(
    GeoPoint2D( Degree( 85 ), Degree( 0 ) ),
    GeoPoint2D( Degree( 85 ), Degree( 180 ) ) );
  points = orthodromy.divide( 600000. );

  ASSERT_EQ( 3, points.size( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 85 ), Degree( 0 ) ), points[0] );
  ASSERT_EQ( GeoPoint2D( Degree( 90 ), Degree( -180 ) ), points[1] );
  ASSERT_EQ( GeoPoint2D( Degree( 85 ), Degree( -180 ) ), points[2] );

  //! Разбиение ортодромии, проходяще вдоль экватора.
  //! Разбивать не надо. Длина сегмента больше длины ортодромии
  orthodromy = orthodromyTools.createOrthodromy(
    GeoPoint2D( ),
    GeoPoint2D( Degree( 10 ), Degree( 0 ) ) );

  points = orthodromy.divide( 1200000. );
  ASSERT_EQ( 2, points.size( ) );
  ASSERT_EQ( GeoPoint2D( ), points[0] );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), points[1] );
}
//==============================================================================
/*!
* \brief Тестирование разбиения ортодромии на равные части с дозаписью и
*        параметрами
*/
TEST( OrthodromyTest, DivideWithParams )
{
  using namespace BasicMath;

  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  std::vector<GeoPoint2D> points;
  double lengthSegment;

  //! Разбиение ортодромии, проходяще вдоль экватора.
  //! Изначально задана длина равномерного разбиения.
  //! Граничные точки не записываем
  Orthodromy orthodromy = orthodromyTools.createOrthodromy(
    GeoPoint2D( ),
    GeoPoint2D( Degree( 0 ), Degree( 10 ) ) );
  lengthSegment = orthodromy.divide( points, orthodromy.getLength( ) / 10,
    false, false );

  ASSERT_TRUE( isEqual( orthodromy.getLength( ) / 10, lengthSegment ) );
  ASSERT_EQ( 9, points.size( ) );
  for( int i = 0; i < points.size( ); i++ )
    ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( i + 1 ) ), points[i] );

  //! Разбиение ортодромии, проходяще вдоль меридиана через полюс.
  //! Изначально задана длина неравномерного разбиения - выравнивание
  //! Результат дозаписываем к предыдущему
  orthodromy = orthodromyTools.createOrthodromy(
    GeoPoint2D( Degree( 85 ), Degree( 0 ) ),
    GeoPoint2D( Degree( 85 ), Degree( 180 ) ) );
  lengthSegment = orthodromy.divide( points, 600000. );

  ASSERT_TRUE( isEqual( orthodromy.getLength( ) / 2, lengthSegment ) );
  ASSERT_EQ( 12, points.size( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 85 ), Degree( 0 ) ), points[9] );
  ASSERT_EQ( GeoPoint2D( Degree( 90 ), Degree( -180 ) ), points[10] );
  ASSERT_EQ( GeoPoint2D( Degree( 85 ), Degree( -180 ) ), points[11] );

  //! Разбиение ортодромии, проходяще вдоль экватора.
  //! Разбивать не надо. Длина сегмента больше длины ортодромии
  //! Вызываем дважды и последовательно дозаписываем сначала точку завершения, 
  //! а потом точку начала
  points.clear( );

  orthodromy = orthodromyTools.createOrthodromy(
    GeoPoint2D( ),
    GeoPoint2D( Degree( 10 ), Degree( 0 ) ) );

  lengthSegment = orthodromy.divide( points, 1200000., false );

  ASSERT_TRUE( isEqual( orthodromy.getLength( ), lengthSegment ) );

  lengthSegment = orthodromy.divide( points, 1200000., true, false );

  ASSERT_TRUE( isEqual( orthodromy.getLength( ), lengthSegment ) );
  ASSERT_EQ( 2, points.size( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), points[0] );
  ASSERT_EQ( GeoPoint2D( ), points[1] );
}
//==============================================================================
} //! Tests
} //! Geodesic