#include <ReachableArea/ReachableAreaInterface.hpp>

#include <ReachableAreaCalculator.hpp>
#include <ReachableArea/ReachableAreaException.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/CoveredBy.hpp>
#include <AbstractOperations/Algorithms/Intersection.hpp>
#include <AbstractOperations/Algorithms/Simplify.hpp>
#include <AbstractOperations/Algorithms/Within.hpp>
//! Geodesic
#include <Geodesic/Projection/Orthographic/Orthographic.hpp> 
#include <Geodesic/GeoLineTools/OrthodromyTools.hpp>
//! std
#include <thread>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace ReachableArea
{
namespace Tests
{
//==============================================================================
TEST( ReachableAreaInterfaceTest, CalculateReachableArea )
{
  //! Подгтовка исходных данных
  //! Исходная точка 
  GeoPoint2D point( GeoPoint2D( Degree( 35. ), Degree( 1. ) ) );
  //! Дальность
  double range( 500000. );
  //! Препятствия
  GeoMultiPolygon2D restrictions 
    = { { { GeoPoint2D( Degree( 33. ), Degree( -2. ) ),
            GeoPoint2D( Degree( 37. ), Degree( -2. ) ),
            GeoPoint2D( Degree( 35. ), Degree( 0. ) ),
            GeoPoint2D( Degree( 33. ), Degree( -2. ) ) } } };
  //! Район
  GeoPolygon2D area = { { GeoPoint2D( Degree( 34. ), Degree( 2. ) ),
                          GeoPoint2D( Degree( 34. ), Degree( -1. ) ),
                          GeoPoint2D( Degree( 36. ), Degree( -1. ) ),
                          GeoPoint2D( Degree( 36. ), Degree( 2. ) ),
                          GeoPoint2D( Degree( 34. ), Degree( 2. ) ) } };

  GeoPolygon2D divArea;
  Geodesic::OrthodromyTools ortodromyTools;
  ortodromyTools.divide( area, divArea, 10000. );
  //! Количество одновременных потоков
  int threadsCount( std::thread::hardware_concurrency( ) );
  if( threadsCount > 8 )
    threadsCount = 8;
  else if( threadsCount > 2 )
    threadsCount -= 2;
  else
    threadsCount = -1;

  //! Формируем контрольное значение
  //! Собираем ортографическую проекцию с центром в заданной точке
  Geodesic::OrthographicProjection projection( point );
  //! Перепроецируем исходные данные
  Point2D pointProj( projection.toProjection( point ) );
  MultiPolygon2D restrictionsProj( projection.toProjection<GeoPoint2D, Point2D>(
                                   restrictions ) );
  MultiPolygon2D areaProj( projection.toProjection<GeoPoint2D, Point2D>(
                                                            { { divArea } } ) );
  //! Собираем окружение для задачи ГЗДЦ
  ReachableAreaCalculator::Environment environment;
  for( const Polygon2D& polygon : restrictionsProj )
    environment.addPolygon( polygon );
  //! Калькулятор для ГЗДЦ
  ReachableAreaCalculator::ReachableArea calculator;
  //! Инициализация результата в формате калькулятора ГЗДЦ
  ReachableAreaCalculator::ReachableAreaResult reachableAreaResult;
  //! Вычисляем ГЗДЦ
  calculator.calculate( environment, pointProj, range, reachableAreaResult, 360,
                        threadsCount );
  //! Обрезаем по району
  MultiPolygon2D reachableArea;
  AbstractOperations::intersection( reachableAreaResult.area, areaProj,
                                    reachableArea );
  //! Находим область, в которой находится исходная точка
  MultiPolygon2D withinArea;
  for( auto polygon : reachableArea )
  {
    if( AbstractOperations::coveredBy( pointProj, polygon ) )
    {
      withinArea.emplace_back( polygon );
      break;
    }
  }
  //! Проверяем и корректируем контрольный вариант
  if( !AbstractOperations::correct( withinArea ) )
  {
    MultiPolygon2D preSimpleArea( withinArea );
    withinArea.clear( );
    AbstractOperations::simplify( preSimpleArea, withinArea, 1 );
  }

  Polygon2D reachableProjControl = withinArea.front( );

  //! Перепроецируем
  GeoPolygon2D reachableControl
    = projection.fromProjection<Point2D, GeoPoint2D>( withinArea ).front( );
  GeoMultiPolygon2D geoReachableArea
    = projection.fromProjection<Point2D, GeoPoint2D>(
                                                { reachableAreaResult.area } );

  //! Обрезаем ГЗДЦ по локсодромии в геокоординатах
  AbstractOperations::intersection( geoReachableArea, area, geoReachableArea );

  //! Находим область, в которой находится исходная точка в гео
  GeoPolygon2D reachableLoxControl;
  for( auto polygon : geoReachableArea )
  {
    if(AbstractOperations::coveredBy( point, polygon ) )
    {
      reachableLoxControl = polygon;
      break;
    }
  }

  //! Проверяем и корректируем результат
  if( !AbstractOperations::correct( reachableControl ) )
  { //!< если correct не помог - упрощаем с точностью около метра в радианах
    GeoPolygon2D preSimpleControl( reachableControl );
    reachableControl.clear( );
    AbstractOperations::simplify( preSimpleControl, reachableControl,
      ACCURACY_RADIAN );
  }

  if( !AbstractOperations::correct( reachableLoxControl ) )
  { //!< если correct не помог - упрощаем с точностью около метра в радианах
    GeoPolygon2D preSimpleControl( reachableLoxControl );
    reachableLoxControl.clear( );
    AbstractOperations::simplify( preSimpleControl, reachableLoxControl,
      ACCURACY_RADIAN );
  }

  //! Рассчитываем контрольные результаты
  Polygon2D reachableProjCalc
    = calcReachableArea( pointProj, range, restrictionsProj, 
                         areaProj.front( ) );
  GeoPolygon2D reachableCalc = calcReachableArea( point, range, restrictions,
                                                  area, true );

  GeoPolygon2D reachableLoxCalc = calcReachableArea( point, range, restrictions,
                                                     area, false );

  //! Проверяем корректность

  //! Плоскость 
  ASSERT_EQ( reachableProjControl.outer( ).size( ),
             reachableProjCalc.outer( ).size( ) );
  ASSERT_EQ( reachableProjControl.inners( ).size( ),
             reachableProjCalc.inners( ).size( ) );
  for( std::size_t i = 0; i < reachableProjControl.outer( ).size( ); ++i )
    ASSERT_EQ( reachableProjControl.outer( ).at( i ),
               reachableProjCalc.outer( ).at( i ) );
  for( std::size_t i = 0; i < reachableProjControl.inners( ).size( ); ++i )
  {
    ASSERT_EQ( reachableProjControl.inners( ).at( i ).size( ),
               reachableProjCalc.inners( ).at( i ).size( ) );
    for( std::size_t j = 0; j < reachableProjControl.inners( ).at( i ).size( ); ++j )
      ASSERT_EQ( reachableProjControl.inners( ).at( i ).at( j ),
                 reachableProjCalc.inners( ).at( i ).at( j ) );
  }

  //! Geo
  ASSERT_EQ( reachableControl.outer( ).size( ), 
             reachableCalc.outer( ).size( ) );
  ASSERT_EQ( reachableControl.inners( ).size( ), 
             reachableCalc.inners( ).size( ) );
  for( std::size_t i = 0; i < reachableControl.outer( ).size( ); ++i )
    ASSERT_EQ( reachableControl.outer( ).at( i ), 
               reachableCalc.outer( ).at( i ) );
  for( std::size_t i = 0; i < reachableControl.inners( ).size( ); ++i )
  {
    ASSERT_EQ( reachableControl.inners( ).at( i ).size( ),
               reachableCalc.inners( ).at( i ).size( ) );
    for( std::size_t j = 0; j < reachableControl.inners( ).at( i ).size( ); 
         ++j )
      ASSERT_EQ( reachableControl.inners( ).at( i ).at( j ),
                 reachableCalc.inners( ).at( i ).at( j ) );
  }

  //! GeoLox
  ASSERT_EQ( reachableLoxControl.outer( ).size( ),
             reachableLoxCalc.outer( ).size( ) );
  ASSERT_EQ( reachableLoxControl.inners( ).size( ),
             reachableLoxCalc.inners( ).size( ) );
  for( std::size_t i = 0; i < reachableLoxControl.outer( ).size( ); ++i )
    ASSERT_EQ( reachableLoxControl.outer( ).at( i ),
               reachableLoxCalc.outer( ).at( i ) );
  for( std::size_t i = 0; i < reachableLoxControl.inners( ).size( ); ++i )
  {
    ASSERT_EQ( reachableLoxControl.inners( ).at( i ).size( ),
               reachableLoxCalc.inners( ).at( i ).size( ) );
    for( std::size_t j = 0; j < reachableLoxControl.inners( ).at( i ).size( ); 
         ++j )
      ASSERT_EQ( reachableLoxControl.inners( ).at( i ).at( j ),
                 reachableLoxCalc.inners( ).at( i ).at( j ) );
  }

  //! Проверка исключений

  //! Некорректный район операции
  GeoPolygon2D incorrectArea 
    = { { GeoPoint2D( Degree( 34. ), Degree( 2. ) ),
          GeoPoint2D( Degree( 34. ), Degree( -1. ) ),
          GeoPoint2D( Degree( 36. ), Degree( -1. ) ),
          GeoPoint2D( Degree( 36. ), Degree( 2. ) ) } };
  ASSERT_THROW(calcReachableArea(point, range, restrictions, incorrectArea),
               ReachableAreaFailure);
  try
  {
    reachableCalc = calcReachableArea( point, range, restrictions, 
                                       incorrectArea );
  }
  catch( const ReachableAreaFailure& failure )
  {
    ASSERT_EQ( IncorrectGeometry, failure.getCode( ) );
    ASSERT_EQ( reachableAreaLibTitle, failure.getLib( ) );
  }

  //! Некорректная дальность
  ASSERT_THROW(calcReachableArea(point, -1, restrictions, area),
               ReachableAreaFailure);
  try
  {
    reachableCalc = calcReachableArea( point, -1, restrictions, area );
  }
  catch( const ReachableAreaFailure& failure )
  {
    ASSERT_EQ( NotPositiveRange, failure.getCode( ) );
    ASSERT_EQ( reachableAreaLibTitle, failure.getLib( ) );
  }

  //! Некорректное препятствие
  GeoMultiPolygon2D incorrectRest 
    = { { { GeoPoint2D( Degree( 33. ), Degree( -2. ) ),
            GeoPoint2D( Degree( 37. ), Degree( -2. ) ),
            GeoPoint2D( Degree( 35. ), Degree( 0. ) )  } } };
  ASSERT_THROW(calcReachableArea(point, range, incorrectRest, area),
               ReachableAreaFailure);
  try
  {
    reachableCalc = calcReachableArea( point, range, incorrectRest, area );
  }
  catch( const ReachableAreaFailure& failure )
  {
    ASSERT_EQ( IncorrectGeometry, failure.getCode( ) );
    ASSERT_EQ( reachableAreaLibTitle, failure.getLib( ) );
  }

  //! Точка в препятствии
  ASSERT_THROW(calcReachableArea(GeoPoint2D(Degree(35.), Degree(-1.)),
                                 range, restrictions, area),
               ReachableAreaFailure);
  try
  {
    reachableCalc = calcReachableArea( GeoPoint2D( Degree( 35. ), 
                                                   Degree( -1. ) ),
                                       range, restrictions, area );
  }
  catch( const ReachableAreaFailure& failure )
  {
    ASSERT_EQ( PointInRestriction, failure.getCode( ) );
    ASSERT_EQ( reachableAreaLibTitle, failure.getLib( ) );
  }

  //! Точка на границе препятствия
  ASSERT_THROW(calcReachableArea(GeoPoint2D(Degree(35.), Degree(0.)),
                                 range, restrictions, area),
               ReachableAreaFailure);
  try
  {
    GeoPolygon2D reachableCalc = calcReachableArea( GeoPoint2D( Degree( 35. ),
                                                                Degree( 0. ) ),
                                                    range, restrictions, area );
  }
  catch( const ReachableAreaFailure& failure )
  {
    ASSERT_EQ( PointInRestriction, failure.getCode( ) );
    ASSERT_EQ( reachableAreaLibTitle, failure.getLib( ) );
  }

  //! Пустой район - не исключение
  area.clear( );
  ASSERT_NO_THROW( calcReachableArea( point, range, restrictions, area ) );
}
//==============================================================================
} //! Tests
} //! ReachableArea
//==============================================================================
