#include <Geodesic/Projection/Orthographic/Orthographic.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
//! Основные тесты данного класса проверяются в OrthographicInnerTest.cpp
//! В данном файле проверяется, что нигде не нарушена работа с pimpl 
//! (внутренней реализацией). 
//==============================================================================
/*!
 * \brief Тестирование перевода геометрий из проекции и в проекцию
*/
TEST( OrthographicProjectionTest, ToFromProjection )
{
  using namespace BasicMath;

  //! Контрольные точки
  Point2D pBase2D( 20000., 70000. );
  Point3D pBase3D( 20000., 70000., 6377719. );

  //!< Контрольный многоугольник.
  Polygon2D polyBase2D;

  polyBase2D.outer( ) = {
    Point2D( 20000., 70000. ),
    Point2D( 30000., 70000. ),
    Point2D( 30000., 60000. ),
    Point2D( 20000., 60000. )
  };
  polyBase2D.outer( ).push_back( polyBase2D.outer( )[0] );
  polyBase2D.inners( ).resize( 1 );
  polyBase2D.inners( )[0] = {
    Point2D( 22500., 67500. ),
    Point2D( 22500., 62500. ),
    Point2D( 27500., 62500. ),
    Point2D( 27500., 67500. )
  };
  polyBase2D.inners( )[0].push_back( polyBase2D.inners( )[0][0] );

  //! Вспомогательные точки
  Point2D pResult2D;
  Point3D pResult3D;

  //!< Вспомогательное множество многоугольников.
  MultiPolygon2D polyResult2D;

  //! Создание проекции через 3D
  OrthographicProjection orthoProjection(
    std::make_shared<Ellipsoid>( EL_WGS_84 ),
    GeoPoint3D( Radian( 0. ), Radian( M_PI_4 ) ) );

  //! Проверка перевода 3D точки из проекции и обратно
  pResult3D = orthoProjection.toProjection(
    orthoProjection.fromProjection( pBase3D ) );
  ASSERT_TRUE( isEqual( pBase3D.getX( ), pResult3D.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( pBase3D.getY( ), pResult3D.getY( ), 1. ) );
  ASSERT_TRUE( isEqual( pBase3D.getZ( ), pResult3D.getZ( ), 1. ) );

  //! Проверка перевода 2D точки из проекции и обратно
  pResult2D = orthoProjection.toProjection(
    orthoProjection.fromProjection( pBase2D ) );
  ASSERT_TRUE( isEqual( pBase2D.getX( ), pResult2D.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( pBase2D.getY( ), pResult2D.getY( ), 1. ) );

  //! Проверка перевода 2D-множества многоугольников из проекции и обратно
  polyResult2D = orthoProjection.toProjection<GeoPoint2D, Point2D>(
    orthoProjection.fromProjection<Point2D, GeoPoint2D>(
      MultiPolygon2D{ polyBase2D } ) );
  ASSERT_EQ( 1, polyResult2D.size( ) );
  ASSERT_EQ( polyBase2D.outer( ).size( ), polyResult2D[0].outer( ).size( ) );
  ASSERT_EQ( polyBase2D.inners( ).size( ), polyResult2D[0].inners( ).size( ) );
  ASSERT_EQ( polyBase2D.inners( )[0].size( ), polyResult2D[0].inners( )[0].size( ) );
  for( std::size_t i( 0 ); i < polyBase2D.outer( ).size( ); ++i )
  {
    ASSERT_TRUE(
      isEqual(
        polyBase2D.outer( )[i].getX( ),
        polyResult2D[0].outer( )[i].getX( ),
        1. ) );
    ASSERT_TRUE(
      isEqual(
        polyBase2D.outer( )[i].getY( ),
        polyResult2D[0].outer( )[i].getY( ),
        1. ) );
  }
  for( std::size_t i( 0 ); i < polyBase2D.inners( )[0].size( ); ++i )
  {
    ASSERT_TRUE(
      isEqual(
        polyBase2D.inners( )[0][i].getX( ),
        polyResult2D[0].inners( )[0][i].getX( ),
        1. ) );
    ASSERT_TRUE(
      isEqual(
        polyBase2D.inners( )[0][i].getY( ),
        polyResult2D[0].inners( )[0][i].getY( ),
        1. ) );
  }

  //! Создание проекции через 2D
  OrthographicProjection orthoProjection2( GeoPoint2D( Radian( 0. ),
    Radian( M_PI_4 ) ) );

  //! Проверка перевода 3D точки из проекции и обратно
  pResult3D = orthoProjection2.toProjection(
    orthoProjection2.fromProjection( pBase3D ) );
  ASSERT_TRUE( isEqual( pBase3D.getX( ), pResult3D.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( pBase3D.getY( ), pResult3D.getY( ), 1. ) );
  ASSERT_TRUE( isEqual( pBase3D.getZ( ), pResult3D.getZ( ), 1. ) );

  //! Проверка перевода 2D точки из проекции и обратно
  pResult2D = orthoProjection2.toProjection(
    orthoProjection2.fromProjection( pBase2D ) );
  ASSERT_TRUE( isEqual( pBase2D.getX( ), pResult2D.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( pBase2D.getY( ), pResult2D.getY( ), 1. ) );

  //! Проверка перевода 2D-множества многоугольников из проекции и обратно
  polyResult2D = orthoProjection2.toProjection<GeoPoint2D, Point2D>(
    orthoProjection2.fromProjection<Point2D, GeoPoint2D>(
      MultiPolygon2D{ polyBase2D } ) );
  ASSERT_EQ( 1, polyResult2D.size( ) );
  ASSERT_EQ( polyBase2D.outer( ).size( ), polyResult2D[0].outer( ).size( ) );
  ASSERT_EQ( polyBase2D.inners( ).size( ), polyResult2D[0].inners( ).size( ) );
  ASSERT_EQ( polyBase2D.inners( )[0].size( ), polyResult2D[0].inners( )[0].size( ) );
  for( std::size_t i( 0 ); i < polyBase2D.outer( ).size( ); ++i )
  {
    ASSERT_TRUE(
      isEqual(
        polyBase2D.outer( )[i].getX( ),
        polyResult2D[0].outer( )[i].getX( ),
        1. ) );
    ASSERT_TRUE(
      isEqual(
        polyBase2D.outer( )[i].getY( ),
        polyResult2D[0].outer( )[i].getY( ),
        1. ) );
  }
  for( std::size_t i( 0 ); i < polyBase2D.inners( )[0].size( ); ++i )
  {
    ASSERT_TRUE(
      isEqual(
        polyBase2D.inners( )[0][i].getX( ),
        polyResult2D[0].inners( )[0][i].getX( ),
        1. ) );
    ASSERT_TRUE(
      isEqual(
        polyBase2D.inners( )[0][i].getY( ),
        polyResult2D[0].inners( )[0][i].getY( ),
        1. ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование задание и получение эллипсоида
*/
TEST( OrthographicProjectionTest, GetSetEllipsoid )
{
  //! Начальная проекция на эллипсоиде WGS-84
  OrthographicProjection orthoProjection;

  //! Эллипсоид Красовского
  std::shared_ptr<Ellipsoid> ellKras = std::make_shared<Ellipsoid>(
    EL_KRASOVSKY );

  //! Задание нового эллипсоида
  orthoProjection.setEllipsoid( ellKras );

  //! Проверка, что в проекции нужный эллипсоид
  ASSERT_EQ( ellKras.get( ), orthoProjection.getEllipsoid( ).get( ) );
}
//==============================================================================
/*!
* \brief Тестирование задание и получение центра проекции
*/
TEST( OrthographicProjectionTest, GetSetCenterProjection )
{
  //! Начальная проекция на эллипсоиде WGS-84 с центорм в (0, 0) и азимутом 0
  OrthographicProjection orthoProjection;

  //! Задание новой точки проекции
  orthoProjection.setProjectionPoint( GeoPoint3D( Radian( M_PI_4 ), Radian( M_PI ) ),
    Radian( M_PI_2 ) );

  //! Проверка
  ASSERT_EQ( GeoPoint3D( Radian( M_PI_4 ), Radian( M_PI ) ),
    orthoProjection.getCenterProjection( ) );
  ASSERT_EQ( GeoPoint2D( Radian( M_PI_4 ), Radian( M_PI ) ),
    orthoProjection.getCenterProjection2D( ) );
  ASSERT_EQ( Radian( M_PI_2 ), orthoProjection.getAzimuth( ) );
}
//==============================================================================
} //! Tests
} //! Geodesic
//==============================================================================
