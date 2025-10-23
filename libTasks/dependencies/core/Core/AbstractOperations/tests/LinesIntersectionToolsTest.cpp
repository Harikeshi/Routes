#include <AbstractOperations/LinesIntersectionTools.hpp>

#include  "LinesIntersectionToolsTestHelper.hpp"
#include <AbstractOperations/AbstractOperationsException.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace AbstractOperations
{
namespace Tests
{
//==============================================================================
class BorderLinesIntersectionTest : public ::testing::Test
{
protected:
  void SetUp( )
  {
    base = std::make_pair<Point2D, Point2D>( Point2D( ), Point2D( 10, 0 ) );

    // отрезки не пересекаются
    others.insert( std::make_pair( WithoutIntersection,
      std::make_pair( Point2D( 1, 1 ), Point2D( 1, 5 ) ) ) );
    portions.insert( 
      std::make_pair( WithoutIntersection, std::vector<double>() ) );
    crossPoints.insert( std::make_pair( WithoutIntersection, Point2D( ) ) );

    // отрезки пересекаются в одной (внутренней для обоих) точке
    others.insert( std::make_pair( Intersection,
      std::make_pair( Point2D( 1, 1 ), Point2D( 1, -1 ) ) ) );
    portions.insert(
      std::make_pair( Intersection, std::vector<double>( { 0.1, 0.5 } ) ) );
    crossPoints.insert( std::make_pair( Intersection, Point2D( 1, 0 ) ) );

    // у отрезков общее начало, остальные точки различны
    others.insert( std::make_pair( CommonBegPoint,
      std::make_pair( Point2D(  ), Point2D(  1, 1 ) ) ) );
    portions.insert(
      std::make_pair( CommonBegPoint, std::vector<double> ( { 0, 0 } ) ) );
    crossPoints.insert( std::make_pair( CommonBegPoint, Point2D( ) ) );

    // у отрезков общий конец, остальные точки различны
    others.insert( std::make_pair( CommonEndPoint,
      std::make_pair( Point2D( 1, 1 ), Point2D( 10, 0 ) ) ) );
    portions.insert(
      std::make_pair( CommonEndPoint, std::vector<double>( { 1, 1 } ) ) );
    crossPoints.insert( std::make_pair( CommonEndPoint, Point2D( 10, 0 ) ) );

    // начало 1-го отрезка совпадает с концом 2-го, остальные точки различны
    others.insert( std::make_pair( FirstAfterSecondPoint,
      std::make_pair( Point2D( -1, -1 ), Point2D( ) ) ) );
    portions.insert(
      std::make_pair( FirstAfterSecondPoint, std::vector<double>( { 0, 1 } ) ) );
    crossPoints.insert( std::make_pair( FirstAfterSecondPoint, Point2D( ) ) );

    // конец 1-го отрезка совпадает с началом 2-го, остальные точки различны
    others.insert( std::make_pair( SecondAfterFirstPoint,
      std::make_pair( Point2D( 10, 0 ), Point2D( 1, 5 ) ) ) );
    portions.insert(
      std::make_pair( SecondAfterFirstPoint, std::vector<double>( { 1, 0 } ) ) );
    crossPoints.insert( std::make_pair( SecondAfterFirstPoint, Point2D( 10, 0) ) );

    // начало 1-го отрезка лежит внутри 2-го, остальные точки различны
    others.insert( std::make_pair( FirstStartsFromSecond,
      std::make_pair( Point2D( 0, 1 ), Point2D( 0, -3 ) ) ) );
    portions.insert(
      std::make_pair( FirstStartsFromSecond, std::vector<double>( { 0, 0.25 } ) ) );
    crossPoints.insert( std::make_pair( FirstStartsFromSecond, Point2D( ) ) );

    // конец 1-го отрезка лежит внутри 2-го, остальные точки различны
    others.insert( std::make_pair( FirstFinishesAtSecond,
      std::make_pair( Point2D( 10, 3 ), Point2D( 10, -1 ) ) ) );
    portions.insert(
      std::make_pair( FirstFinishesAtSecond, std::vector<double>( { 1, 0.75 } ) ) );
    crossPoints.insert( std::make_pair( FirstFinishesAtSecond, Point2D( 10, 0 ) ) );

    // начало 2-го отрезка лежит внутри 1-го, остальные точки различны
    others.insert( std::make_pair( SecondStartsFromFirst,
      std::make_pair( Point2D( 1, 0 ), Point2D( 1, 5 ) ) ) );
    portions.insert(
      std::make_pair( SecondStartsFromFirst, std::vector<double>( { 0.1, 0 } ) ) );
    crossPoints.insert( std::make_pair( SecondStartsFromFirst, Point2D( 1, 0) ) );

    // конец 2-го отрезка лежит внутри 1-го, остальные точки различны
    others.insert( std::make_pair( SecondFinishesAtFirst,
      std::make_pair( Point2D( 2, 1 ), Point2D( 2, 0 ) ) ) );
    portions.insert(
      std::make_pair( SecondFinishesAtFirst, 
        std::vector<double>( { 0.2, 1 } ) ) );
    crossPoints.insert( std::make_pair( SecondFinishesAtFirst, Point2D( 2, 0) ) );

    // в начале отрезков есть общий участок
    others.insert( std::make_pair( CommonBegOverlay,
      std::make_pair( Point2D( 1, 0 ), Point2D( -3, 0 )) ) );
    portions.insert(
      std::make_pair( CommonBegOverlay, 
        std::vector<double>( { 0, 0.1, 0.25, 0 } ) ) );
    crossPoints.insert( std::make_pair( CommonBegOverlay, Point2D( ) ) );

    // в конце отрезков есть общий участок
    others.insert( std::make_pair( CommonEndOverlay,
      std::make_pair( Point2D( 13, 0 ), Point2D( 9, 0 ) ) ) );
    portions.insert(
      std::make_pair( CommonEndOverlay, 
        std::vector<double>( { 0.9, 1, 1, 0.75 } ) ) );
    crossPoints.insert( std::make_pair( CommonEndOverlay, Point2D( 9, 0) ) );

    // в начале 1-го и конце 2-го есть общий участок
    others.insert( std::make_pair( FirstAfterSecondOverlay,
      std::make_pair( Point2D( -3, 0 ), Point2D( 2, 0 ) ) ) );
    portions.insert(
      std::make_pair( FirstAfterSecondOverlay, 
        std::vector<double>( { 0, 0.2, 0.6, 1 } ) ) );
    crossPoints.insert( std::make_pair( FirstAfterSecondOverlay, Point2D( ) ) );

    // в начале 2-го и конце 1-го есть общий участок
    others.insert( std::make_pair( SecondAfterFirstOverlay,
      std::make_pair( Point2D( 8, 0 ), Point2D( 13, 0 ) ) ) );
    portions.insert(
      std::make_pair( SecondAfterFirstOverlay, 
        std::vector<double>( { 0.8, 1, 0, 0.4 } ) ) );
    crossPoints.insert( std::make_pair( SecondAfterFirstOverlay, Point2D( 8, 0 ) ) );

    // 1-й отрезок целиком лежит внутри 2-го
    others.insert( std::make_pair( FirstInsideSecond,
      std::make_pair( Point2D( -1, 0 ), Point2D( 19, 0 ) ) ) );
    portions.insert(
      std::make_pair( FirstInsideSecond, 
        std::vector<double>( { 0, 1, 0.05, 0.55 } ) ) );
    crossPoints.insert( std::make_pair( FirstInsideSecond, Point2D( ) ) );

    // 2-й отрезок целиком лежит внутри 1-го
    others.insert( std::make_pair( SecondInsideFirst,
      std::make_pair( Point2D( 1, 0 ), Point2D( 7, 0 ) ) ) );
    portions.insert(
      std::make_pair( SecondInsideFirst, 
        std::vector<double>( { 0.1, 0.7, 0, 1 } ) ) );
    crossPoints.insert( std::make_pair( SecondInsideFirst, Point2D( 1, 0) ) );

    // 1-й отрезок лежит внутри 2-го и у них общее начало
    others.insert( std::make_pair( FrstInScndWithCommonBeg,
      std::make_pair( Point2D( ), Point2D( 16, 0 ) ) ) );
    portions.insert(
      std::make_pair( FrstInScndWithCommonBeg, 
        std::vector<double>( { 0, 1, 0, 0.625 } ) ) );
    crossPoints.insert( std::make_pair( FrstInScndWithCommonBeg, Point2D( ) ) );

    // 1-й отрезок лежит внутри 2-го и у них общий конец
    others.insert( std::make_pair( FrstInScndWithCommonEnd,
      std::make_pair( Point2D( -6, 0 ), Point2D( 10, 0 ) ) ) );
    portions.insert(
      std::make_pair( FrstInScndWithCommonEnd, 
        std::vector<double>( { 0, 1, 0.375, 1 } ) ) );
    crossPoints.insert( std::make_pair( FrstInScndWithCommonEnd, Point2D( ) ) );

    // 1-й отрезок лежит внутри 2-го и начало 1-го совпадает с концом 2-го
    others.insert( std::make_pair( FrstInScndAndFrstBegIsScndEnd,
      std::make_pair( Point2D( 16, 0 ), Point2D( ) ) ) );
    portions.insert(
      std::make_pair( FrstInScndAndFrstBegIsScndEnd, 
        std::vector<double>( { 0, 1, 1, 0.375 } ) ) );
    crossPoints.insert( std::make_pair( 
      FrstInScndAndFrstBegIsScndEnd, Point2D( ) ) );

    // 1-й отрезок лежит внутри 2-го и начало 2-го совпадает с концом 1-го
    others.insert( std::make_pair( FrstInScndAndFrstEndIsScndBeg,
      std::make_pair( Point2D( 10, 0 ), Point2D( -6, 0 ) ) ) );
    portions.insert(
      std::make_pair( FrstInScndAndFrstEndIsScndBeg, 
        std::vector<double>( { 0, 1, 0.625, 0 } ) ) );
    crossPoints.insert( std::make_pair( 
      FrstInScndAndFrstEndIsScndBeg, Point2D( ) ) );

    // 2-й отрезок лежит внутри 1-го и у них общее начало 
    others.insert( std::make_pair( ScndInFrstWithCommonBeg,
      std::make_pair( Point2D(  ), Point2D( 2, 0 ) ) ) );
    portions.insert(
      std::make_pair( ScndInFrstWithCommonBeg, 
        std::vector<double>( { 0, 0.2, 0, 1 } ) ) );
    crossPoints.insert( std::make_pair( ScndInFrstWithCommonBeg, Point2D( ) ) );

    // 2-й отрезок лежит внутри 1-го и у них общий конец 
    others.insert( std::make_pair( ScndInFrstWithCommonEnd,
      std::make_pair( Point2D( 6, 0 ), Point2D( 10, 0 ) ) ) );
    portions.insert(
      std::make_pair( ScndInFrstWithCommonEnd, 
        std::vector<double>( { 0.6, 1, 0, 1 } ) ) );
    crossPoints.insert( std::make_pair( ScndInFrstWithCommonEnd, Point2D( 6, 0) ) );

    // 2-й отрезок лежит внутри 1-го и начало 1-го совпадает с концом 2-го
    others.insert( std::make_pair( ScndInFrstAndFrstBegIsScndEnd,
      std::make_pair( Point2D( 4, 0 ), Point2D(  ) ) ) );
    portions.insert(
      std::make_pair( ScndInFrstAndFrstBegIsScndEnd, 
        std::vector<double>( { 0, 0.4, 1, 0 } ) ) );
    crossPoints.insert( std::make_pair( 
      ScndInFrstAndFrstBegIsScndEnd, Point2D( ) ) );

    // 2-й отрезок лежит внутри 1-го и начало 2-го совпадает с концом 1-го
    others.insert( std::make_pair( ScndInFrstAndFrstEndIsScndBeg,
      std::make_pair( Point2D( 10, 0 ), Point2D( 9, 0 ) ) ) );
    portions.insert(
      std::make_pair( ScndInFrstAndFrstEndIsScndBeg, 
        std::vector<double>( { 0.9, 1, 1, 0 } ) ) );
    crossPoints.insert( std::make_pair( 
      ScndInFrstAndFrstEndIsScndBeg, Point2D( 9, 0) ) );

    // отрезки полностью совпадают
    others.insert( std::make_pair( Coincide,
      std::make_pair( Point2D(  ), Point2D( 10, 0 ) ) ) );
    portions.insert(
      std::make_pair( Coincide, std::vector<double>( { 0, 1, 0, 1 } ) ) );
    crossPoints.insert( std::make_pair( Coincide, Point2D( ) ) );

    // отрезки совпадают с точностью до направления
    others.insert( std::make_pair( Contradirectional,
      std::make_pair( Point2D( 10, 0 ), Point2D(  ) ) ) );
    portions.insert(
      std::make_pair( Contradirectional, std::vector<double>( { 0, 1, 1, 0 } ) ) );
    crossPoints.insert( std::make_pair( Contradirectional, Point2D( ) ) );
  }
//==============================================================================
  void TearDown( )
  {

  }
//==============================================================================
  std::pair<Point2D, Point2D> base; // основной отрезок

  std::map<IntersectionType, std::pair<Point2D, Point2D> >others;
  std::map<IntersectionType, std::vector<double>> portions;
  std::map<IntersectionType, Point2D> crossPoints;
//==============================================================================
};
//==============================================================================
// для двумерных отрезков определить тип и положение пересечения
TEST_F( BorderLinesIntersectionTest, findIntersection2DTest )
{
  IntersectionType resVal;
  std::vector<double> resPortions;
  Point2D resCrossPoint;

  for( const auto& other : others )
  {
    resVal = findIntersection(
      base.first, base.second, other.second.first, other.second.second,
      resPortions, resCrossPoint );
    ASSERT_EQ( other.first, resVal );
    ASSERT_EQ( portions.at( other.first ), resPortions );
    ASSERT_EQ( crossPoints.at( other.first ), resCrossPoint );
  }

  // вариант не пересекающихся отрезков, лежащих на одной прямой 
  resVal = findIntersection(
    base.first, base.second, Point2D(15, 0), Point2D(20, 0),
    resPortions, resCrossPoint );
  ASSERT_EQ( WithoutIntersection, resVal );
  ASSERT_EQ( resPortions.size( ), 0 );
  ASSERT_EQ( resCrossPoint, Point2D( ) );
}
//==============================================================================
// определить точку по соотношению и наоборот
TEST( BorderLinesIntersectionTest_, portionsTest )
{
  double eps = ACCURACY_DOUBLE;

  ASSERT_NEAR( 0.3,
    portionByPoint ( Point2D( ), Point2D( 10, 20 ), Point2D( 3, 6 ) ), 
    eps );

  ASSERT_NEAR( 0.3,
    portionByPoint( Point2D( ), Point2D( 10, 20 ), Point2D( 3, 6 ) ),
    eps );

  ASSERT_NEAR( 0.3, generalizePortion( 0.2, 0.6, 0.25 ), eps );
}
//==============================================================================
// поиск пересечений с учетом времени (точное совпадение и допуск)
TEST_F( BorderLinesIntersectionTest, FindIntersectionWithTimeTest )
{
  std::vector<double> resPortions;
  WayPoint<Point2D> resCrossPointBeg;
  WayPoint<Point2D> resCrossPointEnd;

  auto other = others.at( WithoutIntersection );

  WayPoint<Point2D> baseSt( 0, base.first );
  WayPoint<Point2D> baseFin( 5, base.second );
  WayPoint<Point2D> otherSt( 0, other.first );
  WayPoint<Point2D> otherFin( 5, other.second );

  // если сами отрезки не пересекаются, то и учет времени не поможет
  ASSERT_FALSE( findIntersection(
    baseSt, baseFin, otherSt, otherFin, resPortions, resCrossPointBeg, resCrossPointEnd, 0 ) );
  ASSERT_EQ( 0, resPortions.size( ) );


  // сами отрезки пересекаются, время в пересечении не совпадает
  otherSt = WayPoint<Point2D> ( 4, Point2D( 2, 1 ) );
  otherFin = WayPoint<Point2D>( 5, Point2D( 2, -1 ) );
  ASSERT_FALSE( findIntersection(
    baseSt, baseFin, otherSt, otherFin, resPortions, resCrossPointBeg, resCrossPointEnd, 0 ) );
  ASSERT_EQ( 0, resPortions.size( ) );

  // сами отрезки пересекаются, время в пересечении совпадает точно
  otherSt = WayPoint<Point2D>( 0, Point2D( 2, 1 ) );
  otherFin = WayPoint<Point2D>( 2, Point2D( 2, -1 ) );
  ASSERT_TRUE( findIntersection(
    baseSt, baseFin, otherSt, otherFin, resPortions, resCrossPointBeg, resCrossPointEnd, 0 ) );
  ASSERT_EQ( std::vector<double>( { 0.2, 0.5 } ), resPortions );
  ASSERT_EQ( Point2D( 2, 0 ), resCrossPointBeg.coord );

  // сами отрезки пересекаются, время в пересечении почти совпадает
  otherSt = WayPoint<Point2D>( 0, Point2D( 2, 1 ) );
  otherFin = WayPoint<Point2D>( 2.1, Point2D( 2, -1 ) );
  ASSERT_TRUE( findIntersection(
    baseSt, baseFin, otherSt, otherFin, resPortions, resCrossPointBeg, resCrossPointEnd, 0.1 ) );
  ASSERT_EQ( std::vector<double>( { 0.2, 0.5 } ), resPortions );
  ASSERT_EQ( Point2D( 2, 0 ), resCrossPointBeg.coord );


  // у отрезков есть общий интервал, время не пересекается
  otherSt = WayPoint<Point2D>( 10, Point2D( 7, 0 ) );
  otherFin = WayPoint<Point2D>( 15, Point2D( 11, 0 ) );
  ASSERT_FALSE( findIntersection(
    baseSt, baseFin, otherSt, otherFin, resPortions, resCrossPointBeg, resCrossPointEnd, 0 ) );
  ASSERT_EQ( 0, resPortions.size( ) );

  // у отрезков есть общий интервал и в нем одна точка совпадает по времени
  // ( в точке (9,0) у обоих время будет - 4.5
  otherSt = WayPoint<Point2D>( 0.5, Point2D( 11, 0 ) );
  otherFin = WayPoint<Point2D>( 10.5, Point2D( 6, 0 ) );
  ASSERT_TRUE( findIntersection(
    baseSt, baseFin, otherSt, otherFin, resPortions, resCrossPointBeg, resCrossPointEnd, 0 ) );
  ASSERT_NEAR( 0.9, resPortions[0], ACCURACY_DOUBLE );
  ASSERT_NEAR( 0.4, resPortions[1], ACCURACY_DOUBLE );
  ASSERT_EQ( Point2D( 9, 0 ), resCrossPointBeg.coord );
  // аналогичный вариант для сонаправленного движения
  otherSt = WayPoint<Point2D>( 0.5, Point2D( 7, 0 ) );
  otherFin = WayPoint<Point2D>( 10.5, Point2D( 12, 0 ) );
  ASSERT_TRUE( findIntersection(
    baseSt, baseFin, otherSt, otherFin, resPortions, resCrossPointBeg, resCrossPointEnd, 0 ) );
  ASSERT_NEAR( 0.9, resPortions[0], ACCURACY_DOUBLE );
  ASSERT_NEAR( 0.4, resPortions[1], ACCURACY_DOUBLE );
  ASSERT_EQ( Point2D( 9, 0 ), resCrossPointBeg.coord );

  // у отрезков есть общий интервал и в нем совпадающий по времени интервал
  otherSt = WayPoint<Point2D>( 4, Point2D( 8, 0 ) );
  otherFin = WayPoint<Point2D>( 6.5, Point2D( 13, 0 ) );
  ASSERT_TRUE( findIntersection(
    baseSt, baseFin, otherSt, otherFin, resPortions, resCrossPointBeg, resCrossPointEnd, 0 ) );
  ASSERT_NEAR( 0.8, resPortions[0], ACCURACY_DOUBLE );
  ASSERT_NEAR( 1, resPortions[1], ACCURACY_DOUBLE );
  ASSERT_NEAR( 0, resPortions[2], ACCURACY_DOUBLE );
  ASSERT_NEAR( 0.4, resPortions[3], ACCURACY_DOUBLE );
  ASSERT_EQ( Point2D( 8, 0 ), resCrossPointBeg.coord );
}
//==============================================================================
// поиск пересечений траекторий как наборов отрезков со временем. Рассматриваются
// точные пересечения
TEST_F( BorderLinesIntersectionTest, TrajectoryIntersection )
{
  std::vector<WayPoint<Point2D>> base =
  {
    WayPoint<Point2D>( 4, Point2D( 4, 0 ) ),
    WayPoint<Point2D>( 14, Point2D( 14, 0 ) ),
    WayPoint<Point2D>( 24, Point2D( 24, 0 ) )
  };
  std::vector<WayPoint<Point2D>> other = 
  {
    WayPoint<Point2D>( 2, Point2D( 2, 0 ) ),
    WayPoint<Point2D>( 3, Point2D( 3, 0 ) ),
    WayPoint<Point2D>( 4, Point2D( 4, 0 ) ), // начало наложения
    WayPoint<Point2D>( 4.5, Point2D( 4.5, 0 ) ),
    WayPoint<Point2D>( 5, Point2D( 5, 0 ) ),
    WayPoint<Point2D>( 6.5, Point2D( 6, 0 ) ),
    WayPoint<Point2D>( 7, Point2D( 7, 0 ) ), // начало следующего наложения
    WayPoint<Point2D>( 8, Point2D( 8, 0 ) ),
    WayPoint<Point2D>( 9, Point2D( 9, -1 ) ), // тут будет пересечение - точка (9.5, 0)
    WayPoint<Point2D>( 10, Point2D( 10, 1 ) ),
    WayPoint<Point2D>( 14, Point2D( 14, 0 ) ), // общая точка на границе
    WayPoint<Point2D>( 15, Point2D( 17, 0 ) )
  };

  auto resCrossPoints = findIntersections( base, other, -1, 0 );

  ASSERT_EQ( 4, resCrossPoints.size( ) );
  ASSERT_NEAR( 4, resCrossPoints[0].getX(), ACCURACY_DOUBLE );
  ASSERT_NEAR( 7, resCrossPoints[1].getX( ), ACCURACY_DOUBLE );
  ASSERT_NEAR( 9.5, resCrossPoints[2].getX( ), ACCURACY_DOUBLE );
  ASSERT_NEAR( 14, resCrossPoints[3].getX( ), ACCURACY_DOUBLE );
}
//==============================================================================
TEST( IntersectionToolsTest, HorizontallyEqPartsTests )
{
  std::vector<double> portions;

  // не пересекающиеся отрезки
  ASSERT_FALSE( haveHorizontallyCloseParts( 0, 2, 3, 4, portions ) );
  ASSERT_EQ( portions.size( ), 0 );

  // пересекаются через четверть
  ASSERT_TRUE( haveHorizontallyCloseParts( 10, 14, 11.5, 9.5, portions ) );
  ASSERT_EQ( portions.size( ), 2 );
  ASSERT_EQ( portions[0], 0.25 );

  // пересекаются в начале
  ASSERT_TRUE( haveHorizontallyCloseParts( 0, 2, 0, 4, portions ) );
  ASSERT_EQ( portions.size( ), 2 );
  ASSERT_EQ( portions[0], 0 );

  // пересекаются в конце
  ASSERT_TRUE( haveHorizontallyCloseParts( 0, 2, 1, 2, portions ) );
  ASSERT_EQ( portions.size( ), 2 );
  ASSERT_EQ( portions[0], 1 );

  // полностью совпадают
  ASSERT_TRUE( haveHorizontallyCloseParts( 0, 2, 0, 2, portions ) );
  ASSERT_EQ( portions.size( ), 4 );
  ASSERT_EQ( portions[0], 0 );
  ASSERT_EQ( portions[1], 1 );
}
//==============================================================================
TEST( IntersectionToolsTest, HorizontallyClosePartsTests )
{
  std::vector<double> portions;

  // не пересекающиеся далекие отрезки
  ASSERT_FALSE( haveHorizontallyCloseParts( 0, 2, 3, 4, portions ) );
  ASSERT_EQ( portions.size( ), 0 );

  // пересекаются в середине, концы сильно разнесены
  ASSERT_TRUE( haveHorizontallyCloseParts( 0, 2, 2, 0, portions, 1 ) );
  ASSERT_EQ( portions.size( ), 4 );
  ASSERT_EQ( portions[0], 0.25 );
  ASSERT_EQ( portions[1], 0.75 );

  // пересекаются и имеют близкие начала
  ASSERT_TRUE( haveHorizontallyCloseParts( 4, 8, 5, 1, portions, 1 ) );
  ASSERT_EQ( portions.size( ), 4 );
  ASSERT_EQ( portions[0], 0 );
  ASSERT_EQ( portions[1], 0.25 );

  // пересекаются и имеют близкие концы
  ASSERT_TRUE( haveHorizontallyCloseParts( 4, 8, 11, 7, portions, 1 ) );
  ASSERT_EQ( portions.size( ), 4 );
  ASSERT_EQ( portions[0], 0.75 );
  ASSERT_EQ( portions[1], 1 );

  // близкие начала без пересечений
  ASSERT_TRUE( haveHorizontallyCloseParts( 4, 8, 4.5, 10.5, portions, 1 ) );
  ASSERT_EQ( portions.size( ), 4 );
  ASSERT_EQ( portions[0], 0 );
  ASSERT_EQ( portions[1], 0.25 );

  // близкие концы без пересечений
  ASSERT_TRUE( haveHorizontallyCloseParts( 4, 8, 1.5, 7.5, portions, 1 ) );
  ASSERT_EQ( portions.size( ), 4 );
  ASSERT_EQ( portions[0], 0.75 );
  ASSERT_EQ( portions[1], 1 );

  // близость вдоль всей длины
  ASSERT_TRUE( haveHorizontallyCloseParts( 0, 2, 0.1, 2.1, portions, 0.5 ) );
  ASSERT_EQ( portions.size( ), 4 );
  ASSERT_EQ( portions[0], 0 );
  ASSERT_EQ( portions[1], 1 );
}
//==============================================================================
TEST( IntersectionToolsTest, FindIntersectionWithHTest )
{
  // отрезки с интервальным совпадением координат и времени и близким по высоте участком
  WayPoint<Point2D> frstBeg;
  WayPoint<Point2D> frstEnd(10, Point2D(0, 10),0);
  WayPoint<Point2D> scndBeg( 3, Point2D( 0, 3 ), -20 );
  WayPoint<Point2D> scndEnd( 13, Point2D( 0, 13 ), 0 );

  std::vector<double> portions;
  WayPoint<Point2D> crossBeg;
  WayPoint<Point2D> crossEnd;

  ASSERT_EQ( IntervalIntersection,
    findIntersection( frstBeg, frstEnd, scndBeg, scndEnd, portions, crossBeg, crossEnd, 2, 10 ) );
  ASSERT_EQ( portions.size( ), 4 ); // интервальное пересечение
  ASSERT_EQ( crossBeg.coord, Point2D( 0, 8 ) );
  ASSERT_EQ( portions[0], 0.8 );
  ASSERT_EQ( portions[1], 1 );
  ASSERT_EQ( portions[2], 0.5 );
  ASSERT_NEAR( portions[3], 0.7, 1e-10 );

  // отрезки с интервальным совпадением координат и близких на нем времени и высоте
  scndBeg.t = 3.1;
  scndEnd.t = 13.1;

  ASSERT_EQ( IntervalIntersection,
    findIntersection( frstBeg, frstEnd, scndBeg, scndEnd, portions, crossBeg, crossEnd, 2, 10 ) );
  ASSERT_EQ( portions.size( ), 4 ); // интервальное пересечение
  ASSERT_EQ( crossBeg.coord, Point2D( 0, 8 ) );
  ASSERT_EQ( portions[0], 0.8 );
  ASSERT_EQ( portions[1], 1 );
  ASSERT_EQ( portions[2], 0.5 );
  ASSERT_NEAR( portions[3], 0.7, 1e-10 );

  // отрезки с интервальным совпадением координат с последующим сужением по 
  // времени и по высоте
  scndBeg.t = 3;
  scndEnd.t = 16 + 1./3;
  ASSERT_EQ( IntervalIntersection,
    findIntersection( frstBeg, frstEnd, scndBeg, scndEnd, portions, crossBeg, crossEnd, 2, 10 ) );
  ASSERT_EQ( portions.size( ), 4 ); // интервальное пересечение
  ASSERT_EQ( crossBeg.coord, Point2D( 0, 8 ) );
  ASSERT_NEAR( portions[0], 0.8, 1E-10 );
  ASSERT_NEAR( portions[1], 0.9, 1e-10 );
  ASSERT_NEAR( portions[2], 0.5, 1E-10 );
  ASSERT_NEAR( portions[3], 0.6, 1E-10 );
}
//==============================================================================
TEST( IntersectionToolsTest, BigEpsT )
{
  // проверяем, что при допуске по врмени, сильно превышающем шаг траектории,
  // все работает корректно и лишних дублей не возникает

  // допуск по времени из констант: 2 сек

  std::vector<WayPoint<Point2D>> frst = {
    WayPoint<Point2D>( 0, Point2D( ) ),
    WayPoint<Point2D>( 10, Point2D( 10, 0 ) ),
    WayPoint<Point2D>( 10.1, Point2D( 10.1, 0 ) ),
    WayPoint<Point2D>( 10.2, Point2D( 10.2, 0 ) ),
    WayPoint<Point2D>( 10.5, Point2D( 10.5, 0 ) ),
    WayPoint<Point2D>( 14, Point2D( 14, 0 ) ) };

  std::vector<WayPoint<Point2D>> scnd = {
    WayPoint<Point2D>( 5, Point2D( ) ),
    WayPoint<Point2D>( 11.5, Point2D( 10, 0 ) ),
    WayPoint<Point2D>( 11.6, Point2D( 10.15, 0 ) ),
    WayPoint<Point2D>( 11.7, Point2D( 10.2, 0 ) ),
    WayPoint<Point2D>( 11.7, Point2D( 10.25, 0 ) ),
    WayPoint<Point2D>( 12.0, Point2D( 10.5, 0 ) ),
    WayPoint<Point2D>( 14, Point2D( 14, 0 ) ) };

  // по плоским координатам - полное наложение
  // с учетом времени достаточно близки с 8.5714
  auto crossP = findIntersections( frst, scnd, -1, 2, 10 );
  ASSERT_EQ( crossP.size( ), 1 );
  ASSERT_NEAR( crossP[0].getX(), 8.5714, 1e-4 );
}
//==============================================================================
} //! Tests
} //! AbstractOperations
//==============================================================================
