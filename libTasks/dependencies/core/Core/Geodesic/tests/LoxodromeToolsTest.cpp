#include <Geodesic/GeoLineTools/LoxodromeTools.hpp>

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
constexpr double ACCURACY_DISTANCE( .01 ); //!< точность расчёта расстояний
//==============================================================================
/*!
* \brief Тестирование конструктора
*/
TEST( LoxodromeToolsTest, Constructor )
{
  using namespace BasicMath;

  //! WGS-84
  Ellipsoid ellipsoid( EL_WGS_84 );    //!< создание эллипсоида 
  LoxodromeTools loxodromeTools;  //!< создание инструментов для локсодромии

  //! СК-42
  ellipsoid = Ellipsoid( EL_KRASOVSKY );
  loxodromeTools = LoxodromeTools( ellipsoid );

  ASSERT_TRUE( isEqual( ellipsoid.getMajorRadius( ),
    loxodromeTools.getMajorRadius( ) ) );
  ASSERT_TRUE( isEqual( ellipsoid.getFlatenning( ),
    loxodromeTools.getFlatenning( ) ) );

  //! ПЗ-90
  ellipsoid = Ellipsoid( EL_GENERAL_GLOBE );
  loxodromeTools = LoxodromeTools( Ellipsoid( EL_GENERAL_GLOBE ) );

  ASSERT_TRUE( isEqual( ellipsoid.getMajorRadius( ),
    loxodromeTools.getMajorRadius( ) ) );
  ASSERT_TRUE( isEqual( ellipsoid.getFlatenning( ),
    loxodromeTools.getFlatenning( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование создания локсодромии по двум точкам
*/
TEST( LoxodromeToolsTest, CreateLoxodromeByPoints )
{
  using namespace BasicMath;

  //! Создаем инструменты для эллипсоида WGS-84
  LoxodromeTools loxodromeTools;

  //! Проверка основных параметров на различных случаях

  //! Локсодромия: линия в 10° на экваторе
  Loxodrome loxodrome = loxodromeTools.createLoxodrome(
    GeoPoint2D( ),
    GeoPoint2D( Degree( 0 ), Degree( 10 ) ) );
  ASSERT_EQ( GeoPoint2D( ), loxodrome.getStartPoint( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 10 ) ), loxodrome.getFinishPoint( ) );
  ASSERT_TRUE( isEqual( 1113194.90793273, loxodrome.getLength( ),
    ACCURACY_DISTANCE ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtStart( ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtFinish( ) );

  //! Локсодромия: линия в 10° на экваторе, пересекающая 180 меридиан
  loxodrome = loxodromeTools.createLoxodrome(
    GeoPoint2D( Degree( 0 ), Degree( 175 ) ),
    GeoPoint2D( Degree( 0 ), Degree( 185 ) ) );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 175 ) ), loxodrome.getStartPoint( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( -175 ) ), loxodrome.getFinishPoint( ) );
  ASSERT_TRUE( isEqual( 1113194.90793273, loxodrome.getLength( ),
    ACCURACY_DISTANCE ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtStart( ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtFinish( ) );

  //! Локсодромия: линия в 10° на широте 45°
  loxodrome = loxodromeTools.createLoxodrome(
    GeoPoint2D( Degree( 45 ), Degree( 0 ) ),
    GeoPoint2D( Degree( 45 ), Degree( 10 ) ) );
  ASSERT_EQ( GeoPoint2D( Degree( 45 ), Degree( 0 ) ), loxodrome.getStartPoint( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 45 ), Degree( 10 ) ), loxodrome.getFinishPoint( ) );
  ASSERT_TRUE( isEqual( 788468.350939781, loxodrome.getLength( ),
    ACCURACY_DISTANCE ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtStart( ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtFinish( ) );

  //! Локсодромия: линия в 10° на полюсе (на широте 90°)
  loxodrome = loxodromeTools.createLoxodrome(
    GeoPoint2D( Degree( 90 ), Degree( 0 ) ),
    GeoPoint2D( Degree( 90 ), Degree( 10 ) ) );
  ASSERT_EQ( GeoPoint2D( Degree( 90 ), Degree( 0 ) ), loxodrome.getStartPoint( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 90 ), Degree( 10 ) ), loxodrome.getFinishPoint( ) );
  ASSERT_TRUE( isEqual( 0., loxodrome.getLength( ), ACCURACY_DISTANCE ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtStart( ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtFinish( ) );

  //! Локсодромия: линия в 10° на меридиане
  loxodrome = loxodromeTools.createLoxodrome(
    GeoPoint2D( Degree( 0 ), Degree( 0 ) ),
    GeoPoint2D( Degree( 10 ), Degree( 0 ) ) );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 0 ) ), loxodrome.getStartPoint( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), loxodrome.getFinishPoint( ) );
  ASSERT_TRUE( isEqual( 1105854.83323437, loxodrome.getLength( ),
    ACCURACY_DISTANCE ) );
  ASSERT_EQ( Radian( 0 ), loxodrome.getAzimuthAtStart( ) );
  ASSERT_EQ( Radian( 0. ), loxodrome.getAzimuthAtFinish( ) );

  //! Локсодромия: линия через полюс в 10° на меридиане, которая превращается
  //!              в участок параллели, соединяющий заданные точки
  loxodrome = loxodromeTools.createLoxodrome(
    GeoPoint2D( Degree( 85 ), Degree( 0 ) ),
    GeoPoint2D( Degree( 85 ), Degree( 180 ) ) );
  ASSERT_EQ( GeoPoint2D( Degree( 85 ), Degree( 0 ) ), loxodrome.getStartPoint( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 85 ), Degree( -180 ) ), loxodrome.getFinishPoint( ) );
  ASSERT_TRUE( isEqual( 1752214.06391237, loxodrome.getLength( ),
    ACCURACY_DISTANCE ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtStart( ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtFinish( ) );

  //! Локсодромия: простая, но длинная
  loxodrome = loxodromeTools.createLoxodrome(
    GeoPoint2D( Degree( -75 ), Degree( -165 ) ),
    GeoPoint2D( Degree( 85 ), Degree( 125 ) ) );
  ASSERT_EQ( GeoPoint2D( Degree( -75 ), Degree( -165 ) ), loxodrome.getStartPoint( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 85 ), Degree( 125 ) ), loxodrome.getFinishPoint( ) );
  ASSERT_TRUE( isEqual( 18264452.0117405, loxodrome.getLength( ),
    ACCURACY_DISTANCE ) );
  ASSERT_EQ( Radian( -0.233109942474453 ), loxodrome.getAzimuthAtStart( ) );
  ASSERT_EQ( Radian( -0.233109942474453 ), loxodrome.getAzimuthAtFinish( ) );

  //! Локсодромия: точка на пересечении экватора и гринвича.
  ASSERT_THROW(loxodromeTools.createLoxodrome(GeoPoint2D(), GeoPoint2D()),
               GeodesicFailure);
  try
  {
    loxodrome = loxodromeTools.createLoxodrome( GeoPoint2D( ), GeoPoint2D( ) );
  }
  catch( const GeodesicFailure& exception )
  {
    ASSERT_EQ(
      GeodesicEnumFailure::LoxodromeThroughOnePoint,
      exception.getCode( ) );
    ASSERT_EQ( geodesicLibTitle, exception.getLib( ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование создания локсодромии по точке, азимуту и протяженности
*/
TEST( LoxodromeToolsTest, CreateLoxodromeByParams )
{
  using namespace BasicMath;

  //! Создаем инструменты для эллипсоида WGS-84
  LoxodromeTools loxodromeTools;

  //! Проверка основных параметров на различных случаях

  //! Локсодромия: луч от точки на пересечении экватора и гринвича на север
  Loxodrome loxodrome = loxodromeTools.createLoxodrome( GeoPoint2D( ),
    Radian( 0. ), 0. );
  ASSERT_EQ( GeoPoint2D( ), loxodrome.getStartPoint( ) );
  ASSERT_EQ( GeoPoint2D( ), loxodrome.getFinishPoint( ) );
  ASSERT_TRUE( isEqual( 0., loxodrome.getLength( ), ACCURACY_DISTANCE ) );
  ASSERT_EQ( Radian( 0. ), loxodrome.getAzimuthAtStart( ) );
  ASSERT_EQ( Radian( 0. ), loxodrome.getAzimuthAtFinish( ) );

  //! Локсодромия: линия в 10° на экваторе
  loxodrome = loxodromeTools.createLoxodrome( GeoPoint2D( ), Radian( M_PI_2 ),
    1113194.90793273 );
  ASSERT_EQ( GeoPoint2D( ), loxodrome.getStartPoint( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 10 ) ), loxodrome.getFinishPoint( ) );
  ASSERT_TRUE( isEqual( 1113194.90793273, loxodrome.getLength( ),
    ACCURACY_DISTANCE ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtStart( ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtFinish( ) );

  //! Локсодромия: линия в 10° на экваторе, пересекающая 180 меридиан
  loxodrome = loxodromeTools.createLoxodrome( GeoPoint2D( Degree( 0 ), Degree( 175 ) ),
    Radian( M_PI_2 ), 1113194.90793273 );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 175 ) ), loxodrome.getStartPoint( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( -175 ) ), loxodrome.getFinishPoint( ) );
  ASSERT_TRUE( isEqual( 1113194.90793273, loxodrome.getLength( ),
    ACCURACY_DISTANCE ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtStart( ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtFinish( ) );

  //! Локсодромия: линия в 10° на широте 45°
  loxodrome = loxodromeTools.createLoxodrome( GeoPoint2D( Degree( 45 ), Degree( 0 ) ),
    Radian( M_PI_2 ), 788468.350939781 );
  ASSERT_EQ( GeoPoint2D( Degree( 45 ), Degree( 0 ) ), loxodrome.getStartPoint( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 45 ), Degree( 10 ) ), loxodrome.getFinishPoint( ) );
  ASSERT_TRUE( isEqual( 788468.350939781, loxodrome.getLength( ),
    ACCURACY_DISTANCE ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtStart( ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtFinish( ) );

  //! Локсодромия: луч вдоль полюса (на широте 90°) - не корректно.
  ASSERT_THROW(loxodromeTools.createLoxodrome(
                 GeoPoint2D(Degree(90), Degree(0)),
                 Radian(M_PI_2), 0.),
               GeodesicFailure);
  try
  {
    loxodrome = loxodromeTools.createLoxodrome(
      GeoPoint2D( Degree( 90 ), Degree( 0 ) ),
      Radian( M_PI_2 ), 0. );
  }
  catch( const GeodesicFailure& exception )
  {
    ASSERT_EQ( geodesicLibTitle, exception.getLib( ) );
    ASSERT_EQ( GeodesicEnumFailure::LoxodromeThroughPole, exception.getCode( ) );
  }

  //! Локсодромия: попытка пересечь полюс.
  ASSERT_THROW(loxodromeTools.createLoxodrome(
                 GeoPoint2D(Degree(85), Degree(0)),
                 Radian(0), 15000000.),
               GeodesicFailure);
  try
  {
    loxodrome = loxodromeTools.createLoxodrome(
      GeoPoint2D( Degree( 85 ), Degree( 0 ) ),
      Radian( 0 ), 15000000. );
  }
  catch( const GeodesicFailure& exception )
  {
    ASSERT_EQ( geodesicLibTitle, exception.getLib( ) );
    ASSERT_EQ( GeodesicEnumFailure::LoxodromeThroughPole, exception.getCode( ) );
  }

  //! Локсодромия: линия в 10° на меридиане
  loxodrome = loxodromeTools.createLoxodrome( GeoPoint2D( ),
    Radian( 0. ), 1105854.83323437 );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 0 ) ), loxodrome.getStartPoint( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), loxodrome.getFinishPoint( ) );
  ASSERT_TRUE( isEqual( 1105854.83323437, loxodrome.getLength( ),
    ACCURACY_DISTANCE ) );
  ASSERT_EQ( Radian( 0. ), loxodrome.getAzimuthAtStart( ) );
  ASSERT_EQ( Radian( 0. ), loxodrome.getAzimuthAtFinish( ) );

  //! Локсодромия: по параллели (взамен прохода через полюс)
  loxodrome = loxodromeTools.createLoxodrome( GeoPoint2D( Degree( 85 ), Degree( 0 ) ),
    Radian( M_PI_2 ),
    1752214.06391238 );
  ASSERT_EQ( GeoPoint2D( Degree( 85 ), Degree( 0 ) ), loxodrome.getStartPoint( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 85 ), Degree( -180 ) ), loxodrome.getFinishPoint( ) );
  ASSERT_TRUE( isEqual( 1752214.06391238, loxodrome.getLength( ),
    ACCURACY_DISTANCE ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtStart( ) );
  ASSERT_EQ( Radian( M_PI_2 ), loxodrome.getAzimuthAtFinish( ) );

  //! Локсодромия: простая, но длинная
  loxodrome = loxodromeTools.createLoxodrome(
    GeoPoint2D( Degree( -75 ), Degree( -165 ) ),
    Radian( -0.233109942474453 ), 18264452.0117405 );
  ASSERT_EQ( GeoPoint2D( Degree( -75 ), Degree( -165 ) ), loxodrome.getStartPoint( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 85 ), Degree( 125 ) ), loxodrome.getFinishPoint( ) );
  ASSERT_TRUE( isEqual( 18264452.0117405, loxodrome.getLength( ),
    ACCURACY_DISTANCE ) );
  ASSERT_EQ( Radian( -0.233109942474453 ), loxodrome.getAzimuthAtStart( ) );
  ASSERT_EQ( Radian( -0.233109942474453 ), loxodrome.getAzimuthAtFinish( ) );
}
//==============================================================================
/*!
* \brief Тестирование поиска на локсодромии точки, отстоящей от заданной
*/
TEST( LoxodromeToolsTest, PointPosition )
{
  //! Создаем инструменты для эллипсоида WGS-84
  LoxodromeTools loxodromeTools;

  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ),
    loxodromeTools.pointPosition( GeoPoint2D( ), Radian( 0. ),
      1105854.83323437 ) );
  ASSERT_EQ( GeoPoint2D( Degree( -10 ), Degree( 0 ) ),
    loxodromeTools.pointPosition( GeoPoint2D( ), Radian( 0. ),
      -1105854.83323437 ) );

  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( -175 ) ),
    loxodromeTools.pointPosition( GeoPoint2D( Degree( 0 ), Degree( 175 ) ),
      Radian( M_PI_2 ), 1113194.90793273 ) );

  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 175 ) ),
    loxodromeTools.pointPosition( GeoPoint2D( Degree( 0 ), Degree( -175 ) ),
      Radian( -M_PI_2 ), 1113194.90793273 ) );

  ASSERT_EQ( GeoPoint2D( Degree( 80 ), Degree( 0 ) ),
    loxodromeTools.pointPosition( GeoPoint2D( Degree( 90 ), Degree( 0 ) ),
      Radian( M_PI ), 1116825.85737585 ) );

  ASSERT_TRUE( std::isnan( loxodromeTools.pointPosition(
    GeoPoint2D( Degree( 90 ), Degree( 0 ) ),
    Radian( M_PI_2 ), 1. )
    .getLongitude( ).get( ) ) );

  ASSERT_TRUE( std::isnan( loxodromeTools.pointPosition(
    GeoPoint2D( Degree( 85 ), Degree( 0 ) ),
    Radian( 0. ), 1105854.83323437 )
    .getLongitude( ).get( ) ) );

  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 10 ) ),
    loxodromeTools.pointPosition( GeoPoint2D( ),
      GeoPoint2D( Degree( 0 ), Degree( 20 ) ) ) );

  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 5 ) ),
    loxodromeTools.pointPosition( GeoPoint2D( ),
      GeoPoint2D( Degree( 0 ), Degree( 20 ) ),
      .25 ) );
}
//==============================================================================
/*!
* \brief Тестирование поиска расстояния между двумя точками по локсодромии
*/
TEST( LoxodromeToolsTest, Distance )
{
  using namespace BasicMath;

  //! Создаем инструменты для эллипсоида WGS-84
  LoxodromeTools loxodromeTools;

  ASSERT_TRUE( isEqual( 1105854.83323437,
    loxodromeTools.distance(
      GeoPoint2D( ),
      GeoPoint2D( Degree( 10 ), Degree( 0 ) ) ),
    ACCURACY_DISTANCE ) );

  ASSERT_TRUE( isEqual( 1105854.83323437,
    loxodromeTools.distance(
      GeoPoint2D( ),
      GeoPoint2D( Degree( -10 ), Degree( 0 ) ) ),
    ACCURACY_DISTANCE ) );

  ASSERT_TRUE( isEqual( 1113194.90793273,
    loxodromeTools.distance(
      GeoPoint2D( Degree( 0 ), Degree( 175 ) ),
      GeoPoint2D( Degree( 0 ), Degree( -175 ) ) ),
    ACCURACY_DISTANCE ) );

  ASSERT_TRUE( isEqual( 1113194.90793273,
    loxodromeTools.distance(
      GeoPoint2D( Degree( 0 ), Degree( -175 ) ),
      GeoPoint2D( Degree( 0 ), Degree( 175 ) ) ),
    ACCURACY_DISTANCE ) );

  ASSERT_TRUE( isEqual( 1116825.85737585,
    loxodromeTools.distance(
      GeoPoint2D( Degree( 90 ), Degree( 0 ) ),
      GeoPoint2D( Degree( 80 ), Degree( 0 ) ) ),
    ACCURACY_DISTANCE ) );

  ASSERT_TRUE( isEqual( 1752214.06391237,
    loxodromeTools.distance(
      GeoPoint2D( Degree( 85 ), Degree( -180 ) ),
      GeoPoint2D( Degree( 85 ), Degree( 0 ) ) ),
    ACCURACY_DISTANCE ) );
}
//==============================================================================
/*!
* \brief Тестирование поиска протяжённости между набором точек по локсодромиям
*/
TEST( LoxodromeToolsTest, LenghtByPoints )
{
  using namespace BasicMath;

  //! Создаем инструменты для эллипсоида WGS-84
  LoxodromeTools loxodromeTools;

  std::vector<GeoPoint2D> points;
  std::vector<double> lengths;

  points.emplace_back( GeoPoint2D( Degree( 10 ), Degree( 0 ) ) );

  //! Набор из одной точки
  ASSERT_TRUE( isEqual( 0., loxodromeTools.length( points, lengths ),
    ACCURACY_DISTANCE ) );
  ASSERT_EQ( 0, lengths.size( ) );

  //! Набор из трёх точек
  points.emplace_back( GeoPoint2D( Degree( 0 ), Degree( 0 ) ) );
  points.emplace_back( GeoPoint2D( Degree( -10 ), Degree( 0 ) ) );

  ASSERT_TRUE( isEqual( 2211709.66646874,
    loxodromeTools.length( points, lengths ),
    ACCURACY_DISTANCE ) );
  ASSERT_TRUE( isEqual( 1105854.83323437, lengths[0], ACCURACY_DISTANCE ) );
  ASSERT_TRUE( isEqual( 1105854.83323437, lengths[1], ACCURACY_DISTANCE ) );
}
//==============================================================================
/*!
* \brief Тестирование поиска протяжённости ломаной по локсодромиям
*/
TEST( LoxodromeToolsTest, LenghtByLineString )
{
  using namespace BasicMath;

  //! Создаем инструменты для эллипсоида WGS-84
  LoxodromeTools loxodromeTools;

  GeoLinestring2D line;
  line.emplace_back( GeoPoint2D( Degree( 10 ), Degree( 0 ) ) );

  //! Ломаная из одной точки
  std::vector<double> lenghts;
  ASSERT_TRUE( isEqual( 0., loxodromeTools.length( line, lenghts ),
    ACCURACY_DISTANCE ) );

  //! Ломаная из трёх точек
  line.emplace_back( GeoPoint2D( Degree( 0 ), Degree( 0 ) ) );
  line.emplace_back( GeoPoint2D( Degree( -10 ), Degree( 0 ) ) );

  ASSERT_TRUE( isEqual( 2211709.66646874, loxodromeTools.length( line, lenghts ),
    ACCURACY_DISTANCE ) );
}
//==============================================================================
/*!
* \brief Тестирование разбиения локсодромии из двух точек на равные части
*/
TEST( LoxodromeToolsTest, DivideByTwoPoints )
{
  //! Создаем инструменты для эллипсоида WGS-84
  LoxodromeTools loxodromeTools;

  //! Локсодромия по меридиану вдоль полюса.
  auto points = loxodromeTools.divide( GeoPoint2D( Degree( 85 ), Degree( 0 ) ),
    GeoPoint2D( Degree( 85 ), Degree( 180 ) ),
    600000. );

  ASSERT_EQ( 4, points.size( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 85 ), Degree( 0 ) ), points[0] );
  ASSERT_EQ( GeoPoint2D( Degree( 85 ), Degree( 60 ) ), points[1] );
  ASSERT_EQ( GeoPoint2D( Degree( 85 ), Degree( 120 ) ), points[2] );
  ASSERT_EQ( GeoPoint2D( Degree( 85 ), Degree( -180 ) ), points[3] );
}
//==============================================================================
/*!
* \brief Тестирование разбиения локсодромий заданных набором точек на равные
*        части с заменой исходного набора на результат
*/
TEST( LoxodromeToolsTest, DivideByVectorOfPoints )
{
  //! Создаем инструменты для эллипсоида WGS-84
  LoxodromeTools loxodromeTools;

  //! Локсодромии заданы набором из трёх точек: на пересечение гринвича и 
  //! экватора.
  std::vector<GeoPoint2D> points;
  points.emplace_back( GeoPoint2D( Degree( 10 ), Degree( 0 ) ) );
  points.emplace_back( GeoPoint2D( Degree( 0 ), Degree( 0 ) ) );
  points.emplace_back( GeoPoint2D( Degree( 0 ), Degree( 10 ) ) );

  loxodromeTools.divide( points, 600000. );

  ASSERT_EQ( 5, points.size( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), points[0] );
  ASSERT_EQ( GeoPoint2D( Degree( 5.00037949475415 ), Degree( 0 ) ), points[1] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 0 ) ), points[2] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 5 ) ), points[3] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 10 ) ), points[4] );
}
//==============================================================================
/*!
* \brief Тестирование разбиения локсодромий заданных набором точек на равные
*        части без замены исходного набора
*/
TEST( LoxodromeToolsTest, DivideByVectorOfPointsWithoutChange )
{
  //! Создаем инструменты для эллипсоида WGS-84
  LoxodromeTools loxodromeTools;

  //! Локсодромии заданы набором из трёх точек: на пересечение гринвича и 
  //! экватора.
  std::vector<GeoPoint2D> points;
  points.emplace_back( GeoPoint2D( Degree( 10 ), Degree( 0 ) ) );
  points.emplace_back( GeoPoint2D( Degree( 0 ), Degree( 0 ) ) );
  points.emplace_back( GeoPoint2D( Degree( 0 ), Degree( 10 ) ) );

  std::vector<GeoPoint2D> result;

  loxodromeTools.divide( points, result, 600000. );

  ASSERT_EQ( 3, points.size( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), points[0] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 0 ) ), points[1] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 10 ) ), points[2] );

  ASSERT_EQ( 5, result.size( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), result[0] );
  ASSERT_EQ( GeoPoint2D( Degree( 5.00037949475415 ), Degree( 0 ) ), result[1] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 0 ) ), result[2] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 5 ) ), result[3] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 10 ) ), result[4] );
}
//==============================================================================
/*!
* \brief Тестирование разбиения локсодромий заданных ломаной на равные части с
*        заменой исходной ломаной на результат
*/
TEST( LoxodromeToolsTest, DivideByLinestring )
{
  //! Создаем инструменты для эллипсоида WGS-84
  LoxodromeTools loxodromeTools;

  //! Локсодромии заданы ломаной из трёх точек: на пересечение гринвича и 
  //! экватора.
  GeoLinestring2D line;
  line.emplace_back( GeoPoint2D( Degree( 10 ), Degree( 0 ) ) );
  line.emplace_back( GeoPoint2D( Degree( 0 ), Degree( 0 ) ) );
  line.emplace_back( GeoPoint2D( Degree( 0 ), Degree( 10 ) ) );

  loxodromeTools.divide( line, 600000. );

  ASSERT_EQ( 5, line.size( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), line[0] );
  ASSERT_EQ( GeoPoint2D( Degree( 5.00037949475415 ), Degree( 0 ) ), line[1] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 0 ) ), line[2] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 5 ) ), line[3] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 10 ) ), line[4] );
}
//==============================================================================
/*!
* \brief Тестирование разбиения локсодромий заданных ломаной на равные части без
*        замены исходной ломаной
*/
TEST( LoxodromeToolsTest, DivideByLinestringWithoutChange )
{
  //! Создаем инструменты для эллипсоида WGS-84
  LoxodromeTools loxodromeTools;

  //! Локсодромии заданы ломаной из трёх точек: на пересечение гринвича и 
  //! экватора.
  GeoLinestring2D line;
  line.emplace_back( GeoPoint2D( Degree( 10 ), Degree( 0 ) ) );
  line.emplace_back( GeoPoint2D( Degree( 0 ), Degree( 0 ) ) );
  line.emplace_back( GeoPoint2D( Degree( 0 ), Degree( 10 ) ) );

  GeoLinestring2D result;

  loxodromeTools.divide( line, result, 600000. );

  ASSERT_EQ( 3, line.size( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), line[0] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 0 ) ), line[1] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 10 ) ), line[2] );

  ASSERT_EQ( 5, result.size( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), result[0] );
  ASSERT_EQ( GeoPoint2D( Degree( 5.00037949475415 ), Degree( 0 ) ), result[1] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 0 ) ), result[2] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 5 ) ), result[3] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 10 ) ), result[4] );
}
//==============================================================================
/*!
* \brief Тестирование разбиения локсодромий заданных кольцом на равные части с
*        заменой исходного кольца на результат
*/
TEST( LoxodromeToolsTest, DivideByRing )
{
  //! Создаем инструменты для эллипсоида WGS-84
  LoxodromeTools loxodromeTools;

  //! Локсодромии заданы кольцом из 5 точек: на пересечение гринвича и 
  //! экватора.
  GeoRing2D ring;
  ring.emplace_back( GeoPoint2D( Degree( 10 ), Degree( 0 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 10 ), Degree( 10 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 0 ), Degree( 10 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 0 ), Degree( 0 ) ) );
  ring.emplace_back( GeoPoint2D( ring.at( 0 ) ) );

  loxodromeTools.divide( ring, 600000. );

  ASSERT_EQ( 9, ring.size( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), ring[0] );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 5 ) ), ring[1] );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 10 ) ), ring[2] );
  ASSERT_EQ( GeoPoint2D( Degree( 5.00037949475415 ), Degree( 10 ) ), ring[3] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 10 ) ), ring[4] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 5 ) ), ring[5] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 0 ) ), ring[6] );
  ASSERT_EQ( GeoPoint2D( Degree( 5.00037949475415 ), Degree( 0 ) ), ring[7] );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), ring[8] );
}
//==============================================================================
/*!
* \brief Тестирование разбиения локсодромий заданных кольцом на равные части без
*        замены исходного кольца
*/
TEST( LoxodromeToolsTest, DivideByRingWithoutChange )
{
  //! Создаем инструменты для эллипсоида WGS-84
  LoxodromeTools loxodromeTools;

  //! Локсодромии заданы кольцом из 5 точек: на пересечение гринвича и 
  //! экватора.
  GeoRing2D ring;
  ring.emplace_back( GeoPoint2D( Degree( 10 ), Degree( 0 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 10 ), Degree( 10 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 0 ), Degree( 10 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 0 ), Degree( 0 ) ) );
  ring.emplace_back( GeoPoint2D( ring.at( 0 ) ) );

  GeoRing2D result;

  loxodromeTools.divide( ring, result, 600000. );

  ASSERT_EQ( 5, ring.size( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), ring[0] );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 10 ) ), ring[1] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 10 ) ), ring[2] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 0 ) ), ring[3] );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), ring[4] );

  ASSERT_EQ( 9, result.size( ) );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), result[0] );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 5 ) ), result[1] );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 10 ) ), result[2] );
  ASSERT_EQ( GeoPoint2D( Degree( 5.00037949475415 ), Degree( 10 ) ), result[3] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 10 ) ), result[4] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 5 ) ), result[5] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 0 ) ), result[6] );
  ASSERT_EQ( GeoPoint2D( Degree( 5.00037949475415 ), Degree( 0 ) ), result[7] );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ), result[8] );
}
//==============================================================================
/*!
* \brief Тестирование разбиения локсодромий заданных полигоном на равные части с
*        заменой исходного полигона на результат
*/
TEST( LoxodromeToolsTest, DivideByPolygon )
{
  //! Создаем инструменты для эллипсоида WGS-84
  LoxodromeTools loxodromeTools;

  //! Локсодромии заданы полигоном: внешнее и внутреннее кольцо из 5 точек: на 
  //! пересечение гринвича и экватора.
  GeoPolygon2D polygon;
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 10 ), Degree( 0 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 10 ), Degree( 10 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 0 ), Degree( 10 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 0 ), Degree( 0 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( polygon.outer( ).at( 0 ) ) );

  GeoRing2D ring;
  ring.emplace_back( GeoPoint2D( Degree( 8 ), Degree( 2 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 2 ), Degree( 2 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 2 ), Degree( 8 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 8 ), Degree( 8 ) ) );
  ring.emplace_back( GeoPoint2D( ring.at( 0 ) ) );
  polygon.inners( ).emplace_back( ring );

  loxodromeTools.divide( polygon, 600000. );

  ASSERT_EQ( 9, polygon.outer( ).size( ) );
  ASSERT_EQ( 9, polygon.inners( ).at( 0 ).size( ) );

  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ),
    polygon.outer( )[0] );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 5 ) ),
    polygon.outer( )[1] );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 10 ) ),
    polygon.outer( )[2] );
  ASSERT_EQ( GeoPoint2D( Degree( 5.00037949475415 ), Degree( 10 ) ),
    polygon.outer( )[3] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 10 ) ),
    polygon.outer( )[4] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 5 ) ),
    polygon.outer( )[5] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 0 ) ),
    polygon.outer( )[6] );
  ASSERT_EQ( GeoPoint2D( Degree( 5.00037949475415 ), Degree( 0 ) ),
    polygon.outer( )[7] );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ),
    polygon.outer( )[8] );

  ASSERT_EQ( GeoPoint2D( Degree( 8 ), Degree( 2 ) ),
    polygon.inners( ).at( 0 )[0] );
  ASSERT_EQ( GeoPoint2D( Degree( 5.00013683484431 ), Degree( 2 ) ),
    polygon.inners( ).at( 0 )[1] );
  ASSERT_EQ( GeoPoint2D( Degree( 2 ), Degree( 2 ) ),
    polygon.inners( ).at( 0 )[2] );
  ASSERT_EQ( GeoPoint2D( Degree( 2 ), Degree( 5 ) ),
    polygon.inners( ).at( 0 )[3] );
  ASSERT_EQ( GeoPoint2D( Degree( 2 ), Degree( 8 ) ),
    polygon.inners( ).at( 0 )[4] );
  ASSERT_EQ( GeoPoint2D( Degree( 5.00013683484431 ), Degree( 8 ) ),
    polygon.inners( ).at( 0 )[5] );
  ASSERT_EQ( GeoPoint2D( Degree( 8 ), Degree( 8 ) ),
    polygon.inners( ).at( 0 )[6] );
  ASSERT_EQ( GeoPoint2D( Degree( 8 ), Degree( 5 ) ),
    polygon.inners( ).at( 0 )[7] );
  ASSERT_EQ( GeoPoint2D( Degree( 8 ), Degree( 2 ) ),
    polygon.inners( ).at( 0 )[8] );
}
//==============================================================================
/*!
* \brief Тестирование разбиения локсодромий заданных полигоном на равные части
*        без замены исходного полигона
*/
TEST( LoxodromeToolsTest, DivideByPolygonWithoutChange )
{
  //! Создаем инструменты для эллипсоида WGS-84
  LoxodromeTools loxodromeTools;

  //! Локсодромии заданы полигоном: внешнее и внутреннее кольцо из 5 точек: на 
  //! пересечение гринвича и экватора.
  GeoPolygon2D polygon;
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 10 ), Degree( 0 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 10 ), Degree( 10 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 0 ), Degree( 10 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 0 ), Degree( 0 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( polygon.outer( ).at( 0 ) ) );

  GeoRing2D ring;
  ring.emplace_back( GeoPoint2D( Degree( 8 ), Degree( 2 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 2 ), Degree( 2 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 2 ), Degree( 8 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 8 ), Degree( 8 ) ) );
  ring.emplace_back( GeoPoint2D( ring.at( 0 ) ) );
  polygon.inners( ).emplace_back( ring );

  GeoPolygon2D result;

  loxodromeTools.divide( polygon, result, 600000. );

  ASSERT_EQ( 5, polygon.outer( ).size( ) );
  ASSERT_EQ( 5, polygon.inners( ).at( 0 ).size( ) );

  ASSERT_EQ( 9, result.outer( ).size( ) );
  ASSERT_EQ( 9, result.inners( ).at( 0 ).size( ) );

  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ),
    result.outer( )[0] );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 5 ) ),
    result.outer( )[1] );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 10 ) ),
    result.outer( )[2] );
  ASSERT_EQ( GeoPoint2D( Degree( 5.00037949475415 ), Degree( 10 ) ),
    result.outer( )[3] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 10 ) ),
    result.outer( )[4] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 5 ) ),
    result.outer( )[5] );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 0 ) ),
    result.outer( )[6] );
  ASSERT_EQ( GeoPoint2D( Degree( 5.00037949475415 ), Degree( 0 ) ),
    result.outer( )[7] );
  ASSERT_EQ( GeoPoint2D( Degree( 10 ), Degree( 0 ) ),
    result.outer( )[8] );

  ASSERT_EQ( GeoPoint2D( Degree( 8 ), Degree( 2 ) ),
    result.inners( ).at( 0 )[0] );
  ASSERT_EQ( GeoPoint2D( Degree( 5.00013683484431 ), Degree( 2 ) ),
    result.inners( ).at( 0 )[1] );
  ASSERT_EQ( GeoPoint2D( Degree( 2 ), Degree( 2 ) ),
    result.inners( ).at( 0 )[2] );
  ASSERT_EQ( GeoPoint2D( Degree( 2 ), Degree( 5 ) ),
    result.inners( ).at( 0 )[3] );
  ASSERT_EQ( GeoPoint2D( Degree( 2 ), Degree( 8 ) ),
    result.inners( ).at( 0 )[4] );
  ASSERT_EQ( GeoPoint2D( Degree( 5.00013683484431 ), Degree( 8 ) ),
    result.inners( ).at( 0 )[5] );
  ASSERT_EQ( GeoPoint2D( Degree( 8 ), Degree( 8 ) ),
    result.inners( ).at( 0 )[6] );
  ASSERT_EQ( GeoPoint2D( Degree( 8 ), Degree( 5 ) ),
    result.inners( ).at( 0 )[7] );
  ASSERT_EQ( GeoPoint2D( Degree( 8 ), Degree( 2 ) ),
    result.inners( ).at( 0 )[8] );
}
//==============================================================================
/*!
* \brief Тестирование разбиения локсодромий заданных мультиполигоном на равные
*        части с заменой исходного мультиполигона на результат
*/
TEST( LoxodromeToolsTest, DivideByMultiPolygon )
{
  //! Создаем инструменты для эллипсоида WGS-84
  LoxodromeTools loxodromeTools;

  //! Локсодромии заданы мультиполигоном: 
  GeoMultiPolygon2D multiPolygon;

  //! первый: внешнее и внутреннее кольцо из 5 точек на пересечение гринвича и 
  //!         экватора.
  GeoPolygon2D polygon;
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 10 ), Degree( 0 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 10 ), Degree( 10 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 0 ), Degree( 10 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 0 ), Degree( 0 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( polygon.outer( ).at( 0 ) ) );

  GeoRing2D ring;
  ring.emplace_back( GeoPoint2D( Degree( 8 ), Degree( 2 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 2 ), Degree( 2 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 2 ), Degree( 8 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 8 ), Degree( 8 ) ) );
  ring.emplace_back( GeoPoint2D( ring.at( 0 ) ) );

  polygon.inners( ).emplace_back( ring );
  multiPolygon.emplace_back( polygon );

  //! второй: внешнее и внутреннее кольцо из 4 точек в районе полюса.
  polygon.clear( );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 85 ), Degree( 0 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 85 ), Degree( -180 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 85 ), Degree( 10 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( polygon.outer( ).at( 0 ) ) );

  ring.clear( );
  ring.emplace_back( GeoPoint2D( Degree( 87 ), Degree( 2 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 90 ), Degree( 5 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 87 ), Degree( 8 ) ) );
  ring.emplace_back( GeoPoint2D( ring.at( 0 ) ) );

  polygon.inners( ).emplace_back( ring );
  multiPolygon.emplace_back( polygon );

  //! Разбиение
  loxodromeTools.divide( multiPolygon, 330000. );

  //! Проверка
  ASSERT_EQ( 17, multiPolygon.at( 0 ).outer( ).size( ) );
  ASSERT_EQ( 13, multiPolygon.at( 0 ).inners( ).at( 0 ).size( ) );
  ASSERT_EQ( 14, multiPolygon.at( 1 ).outer( ).size( ) );
  ASSERT_EQ( 6, multiPolygon.at( 1 ).inners( ).at( 0 ).size( ) );
}
//==============================================================================
/*!
* \brief Тестирование разбиения локсодромий заданных мультиполигоном на равные
*        части с заменой исходного мультиполигона на результат
*/
TEST( LoxodromeToolsTest, DivideByMultiPolygonWithoutChange )
{
  //! Создаем инструменты для эллипсоида WGS-84
  LoxodromeTools loxodromeTools;

  //! Локсодромии заданы мультиполигоном: 
  GeoMultiPolygon2D multiPolygon;

  //! первый: внешнее и внутреннее кольцо из 5 точек на пересечение гринвича и 
  //!         экватора.
  GeoPolygon2D polygon;
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 10 ), Degree( 0 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 10 ), Degree( 10 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 0 ), Degree( 10 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 0 ), Degree( 0 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( polygon.outer( ).at( 0 ) ) );

  GeoRing2D ring;
  ring.emplace_back( GeoPoint2D( Degree( 8 ), Degree( 2 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 2 ), Degree( 2 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 2 ), Degree( 8 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 8 ), Degree( 8 ) ) );
  ring.emplace_back( GeoPoint2D( ring.at( 0 ) ) );

  polygon.inners( ).emplace_back( ring );
  multiPolygon.emplace_back( polygon );

  //! второй: внешнее и внутреннее кольцо из 4 точек в районе полюса.
  polygon.clear( );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 85 ), Degree( 0 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 85 ), Degree( -180 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 85 ), Degree( 10 ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( polygon.outer( ).at( 0 ) ) );

  ring.clear( );
  ring.emplace_back( GeoPoint2D( Degree( 87 ), Degree( 2 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 90 ), Degree( 5 ) ) );
  ring.emplace_back( GeoPoint2D( Degree( 87 ), Degree( 8 ) ) );
  ring.emplace_back( GeoPoint2D( ring.at( 0 ) ) );

  polygon.inners( ).emplace_back( ring );
  multiPolygon.emplace_back( polygon );

  //! Разбиение
  GeoMultiPolygon2D result;
  loxodromeTools.divide( multiPolygon, result, 330000. );

  //! Проверка
  ASSERT_EQ( 17, result.at( 0 ).outer( ).size( ) );
  ASSERT_EQ( 13, result.at( 0 ).inners( ).at( 0 ).size( ) );
  ASSERT_EQ( 14, result.at( 1 ).outer( ).size( ) );
  ASSERT_EQ( 6, result.at( 1 ).inners( ).at( 0 ).size( ) );

  ASSERT_EQ( 5, multiPolygon.at( 0 ).outer( ).size( ) );
  ASSERT_EQ( 5, multiPolygon.at( 0 ).inners( ).at( 0 ).size( ) );
  ASSERT_EQ( 4, multiPolygon.at( 1 ).outer( ).size( ) );
  ASSERT_EQ( 4, multiPolygon.at( 1 ).inners( ).at( 0 ).size( ) );
}
//==============================================================================
} //! Tests
} //! Geodesic