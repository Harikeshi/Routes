#include <Geometry/Objects/Line.hpp>

#include <Geometry/GeometryException.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
using BasicMath::isEqual;
using BasicMath::isNull;
//==============================================================================
//==============================================================================
class LineTest : public ::testing::Test
{
protected:

  void SetUp( )
  {
    pA.set( 2., 1. );
    pB.set( 2., 4. );
    pC.set( 3., 4. );
    pD.set( 3., 1. );
    pE.set( 6., 5. );
  }
//==============================================================================
  void TearDown( )
  {
  }
//==============================================================================
  
  Point2D pA; // (2, 1)
  Point2D pB; // (2, 4)
  Point2D pC; // (3, 4)
  Point2D pD; // (3, 1)
  Point2D pE; // (6, 5)
};
//==============================================================================
//==============================================================================
bool isEqual( const Point2D& firstPoint, const Point2D& secondPoint )
{
  return isEqual( firstPoint.getX( ), secondPoint.getX( ) )
    && isEqual( firstPoint.getY( ), secondPoint.getY( ) );
}
//==============================================================================
bool isEqual( const Line& firstLine, const Line& secondLine ) 
{
  return firstLine.isSameTo(secondLine);
}
//==============================================================================
//==============================================================================
// вместе проверяем конструктор, getters и определение базовых свойств прямой
TEST_F( LineTest, BaseFunctions )
{
  double a, b, c, k;

  // некорректная прямая
  ASSERT_THROW( Line invalidLine( pA, pA ), GeometryFailure );
  try
  {
    Line invalidLine( pA, pA );
  }
  catch( const GeometryFailure& exception )
  {
    ASSERT_EQ( GeometryEnumFailure::LineThroughOnePoint, exception.getCode( ) );
    ASSERT_EQ( geomLibTitle, exception.getLib( ) );
  }
  
  // прямая, параллельная оси Oy 
  Line verticalLine( pA, pB ); // x = 2
  ASSERT_TRUE( verticalLine.isVertical( ) ); // параллельна Oy
  ASSERT_TRUE( verticalLine.isForward( ) ); // направлена вперёд
  // x - 2 = 0
  ASSERT_TRUE( isEqual(verticalLine, Line(1, 0, -2)));
  // для вертикальной прямой упрощенное уравнение получить не выйдет
  ASSERT_THROW( verticalLine.getBaseCoeffs( ), GeometryFailure );
  try
  {
    verticalLine.getBaseCoeffs( );
  }
  catch( const GeometryFailure& exception )
  {
    ASSERT_EQ( GeometryEnumFailure::BaseCoefsForVerticalLine, 
      exception.getCode( ) );
    ASSERT_EQ( geomLibTitle, exception.getLib( ) );
  }

  // обычная прямая
  Line line( pC, pA ); 
  ASSERT_FALSE( line.isVertical( ) ); // не параллельна Oy
  ASSERT_FALSE( line.isForward( ) ); // направлена назад
  // 3x - y - 5 = 0
  ASSERT_TRUE( isEqual(line, Line(3, -1, -5)));
  std::tie( k, b ) = line.getBaseCoeffs( ); // y = 3x - 5
  ASSERT_TRUE( isEqual( k, 3. ) );
  ASSERT_TRUE( isEqual( b, -5. ) );

  // задание прямой с помощью пары коэффициентов
  Line kbLine( 2., 3. );
  std::tie( k, b ) = kbLine.getBaseCoeffs( ); // y = 2x + 3
  ASSERT_TRUE( isEqual( k, 2. ) );
  ASSERT_TRUE( isEqual( b, 3. ) );

  // задание прямой с помощью тройки коэффициентов
  Line abcLine( 1., 2., 3. );
  std::tie( a, b, c ) = abcLine.getAllCoeffs( ); // x + 2y + 3 = 0 
  ASSERT_TRUE( isEqual( a, 1. ) );
  ASSERT_TRUE( isEqual( b, 2. ) );
  ASSERT_TRUE( isEqual( c, 3. ) );

  // задание некорректной прямой с помощью тройки коэффициентов
  ASSERT_THROW( Line invalidLine( 0., 0., 0. ), GeometryFailure );
  try
  {
    Line invalidLine( 0., 0., 0. );
  }
  catch( const GeometryFailure& exception )
  {
    ASSERT_EQ( GeometryEnumFailure::LineWithZeroParams, exception.getCode( ) );
    ASSERT_EQ( geomLibTitle, exception.getLib( ) );
  }
}
//==============================================================================
//==============================================================================
TEST_F(LineTest, lineByPointAndSlopeTest)
{
  Line line1(Point2D(0, 1), Radian(M_PI / 4));
  double a, b, c;
  std::tie(a, b, c) = line1.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 1.));
  ASSERT_TRUE(isEqual(b, -1.));
  ASSERT_TRUE(isEqual(c, 1.));
  ASSERT_TRUE(line1.isForward());

  Line line2(Point2D(1, 2), Radian(- 3 * M_PI / 4));
  std::tie(a, b, c) = line2.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 1.));
  ASSERT_TRUE(isEqual(b, -1.));
  ASSERT_TRUE(isEqual(c, 1.));
  ASSERT_FALSE(line2.isForward());

  Line line3(Point2D(0, 1), Radian(- M_PI / 4));
  std::tie(a, b, c) = line3.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, -1.));
  ASSERT_TRUE(isEqual(b, -1.));
  ASSERT_TRUE(isEqual(c, 1.));
  ASSERT_TRUE(line3.isForward());

  Line line4(Point2D(1, 0), Radian(3 * M_PI / 4));
  std::tie(a, b, c) = line4.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, -1.));
  ASSERT_TRUE(isEqual(b, -1.));
  ASSERT_TRUE(isEqual(c, 1.));
  ASSERT_FALSE(line4.isForward());

  Line line5(Point2D(0, 0), Radian(M_PI / 2));
  std::tie(a, b, c) = line5.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 1.));
  ASSERT_TRUE(isEqual(b, 0.));
  ASSERT_TRUE(isEqual(c, 0.));
  ASSERT_TRUE(line5.isForward());

  Line line6(Point2D(0, 0), Radian(- M_PI / 2));
  std::tie(a, b, c) = line6.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 1.));
  ASSERT_TRUE(isEqual(b, 0.));
  ASSERT_TRUE(isEqual(c, 0.));
  ASSERT_FALSE(line6.isForward());

  Line line7(Point2D(0, 0), Radian(0));
  std::tie(a, b, c) = line7.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 0.));
  ASSERT_TRUE(isEqual(b, -1.));
  ASSERT_TRUE(isEqual(c, 0.));
  ASSERT_TRUE(line7.isForward());

  Line line8(Point2D(0, 0), Radian(M_PI));
  std::tie(a, b, c) = line8.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 0.));
  ASSERT_TRUE(isEqual(b, -1.));
  ASSERT_TRUE(isEqual(c, 0.));
  ASSERT_FALSE(line8.isForward());

  Line line9(Point2D(0, 0), Radian(- M_PI));
  std::tie(a, b, c) = line9.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 0.));
  ASSERT_TRUE(isEqual(b, -1.));
  ASSERT_TRUE(isEqual(c, 0.));
  ASSERT_FALSE(line9.isForward());
}
//==============================================================================
//==============================================================================
TEST_F( LineTest, slopeTest )
{
  // вертикальная прямая
  Line verticalLine( pA, pB ); // x = 2
  ASSERT_TRUE( isEqual( verticalLine.getSlope( ), Radian( M_PI / 2. ) ) );

  // горизонтальная прямая
  Line horizontalLine( pA, pD ); // y = 1
  ASSERT_TRUE( isEqual( horizontalLine.getSlope( ), Radian( 0. ) ) );

  // обычная прямая
  Line line( pA, pC ); // 3x - y -5 = 0 
  ASSERT_TRUE( isEqual( line.getSlope( ), Radian( atan( 3 ) ) ) );
  // прямая, направленная назад
  Line inverseLine( pC, pA ); // 3x - y -5 = 0 
  ASSERT_TRUE( isEqual( inverseLine.getSlope( ), Radian( atan( 3 ) - M_PI ) ) );
}
//==============================================================================
//==============================================================================
TEST_F(LineTest, reversedSlopeTest)
{
  // вертикальная прямая
  Line verticalLine(pA, pB); // x = 2
  ASSERT_TRUE(isEqual(verticalLine.getReversedSlope(), Radian(-M_PI / 2.)));

  // горизонтальная прямая
  Line horizontalLine(pA, pD); // y = 1
  ASSERT_TRUE(isEqual(horizontalLine.getReversedSlope(), Radian(-M_PI)));

  // обычная прямая
  Line line(pA, pC); // 3x - y - 5 = 0 
  ASSERT_TRUE(isEqual(line.getReversedSlope(), Radian(atan(3) - M_PI)));
  // прямая, направленная назад
  Line inverseLine(pC, pA); // 3x - y - 5 = 0 
  ASSERT_TRUE(isEqual(inverseLine.getReversedSlope(), Radian(atan(3))));
}
//==============================================================================
//==============================================================================
TEST_F( LineTest, getAngleToTest )
{
  Line line1( pA, pE ); // y = x - 1
  Line verticalLine( pA, pB ); // x = 2
  Line horizontalLine( pA, pD ); // y = 1

  // поворот по часовой
  ASSERT_TRUE( isEqual( line1.getAngleTo( horizontalLine ), 
                        Radian( M_PI / 4. ) ) );

  // поворот против часовой
  ASSERT_TRUE( isEqual( line1.getAngleTo( verticalLine ), 
                        Radian( - M_PI / 4. ) ) );
}
//==============================================================================
//==============================================================================
TEST_F( LineTest, hasPointTest )
{
  Line line( pA, pE ); // y = x - 1
  ASSERT_TRUE( line.hasPoint( Point2D( 10, 9 ) ) );
  ASSERT_FALSE( line.hasPoint( Point2D( 9, 10 ) ) );
}
//==============================================================================
//==============================================================================
TEST_F(LineTest, hasPointNearTest)
{
  Line line(pA, pE); // y = x - 1
  ASSERT_TRUE(line.hasPointNear(Point2D(10, 9)));
  ASSERT_TRUE(line.hasPointNear(Point2D(10, 8.99), 0.05));
  ASSERT_FALSE(line.hasPointNear(Point2D(10, 8.9), 0.05));
}
//==============================================================================
//==============================================================================
TEST_F( LineTest, isParallelTest )
{
  Line line1( pA, pE ); // y = x - 1

  // обычная параллельная прямая
  Line line2( Point2D( 12, 1 ), Point2D( 16, 5 ) ); // y = x - 11
  ASSERT_TRUE( line1.isParallel( line2 ) );

  // параллельная (совпадающая) прямая, направленная в противоположную сторону
  Line line3( pE, pA );
  ASSERT_TRUE( line1.isParallel( line3 ) );

  // не параллельная прямая
  Line line4( pA, pC ); // 3x - y -5 = 0 
  ASSERT_FALSE( line1.isParallel( line4 ) );

  // параллельные горизонтальные прямые (обрабатываются в той же ветке, но пусть будут)
  Line horizontalLine1( pA, Point2D( 5, 1 ) );
  Line horizontalLine2( pB, Point2D( 5, 4 ) );
  ASSERT_TRUE( horizontalLine1.isParallel( horizontalLine2 ) );

  // параллельные вертикальные прямые (обрабатываются в той же ветке, но пусть будут)
  Line verticalLine1( pA, Point2D( 2, 10 ) );
  Line verticalLine2( pC, Point2D( 3, 8 ) );
  ASSERT_TRUE( verticalLine1.isParallel( verticalLine2 ) );
}
//==============================================================================
//==============================================================================
TEST_F( LineTest, isSameTest )
{
  Line line1( pA, pE ); // y = x - 1

  // параллельная, но не совпадающая прямая
  Line line2( Point2D( 12, 1 ), Point2D( 16, 5 ) ); // y = x - 11
  ASSERT_FALSE( line1.isSameTo( line2 ) );

  // параллельная (совпадающая) прямая, направленная в противоположную сторону
  Line line3( pE, pA );
  ASSERT_TRUE( line1.isSameTo( line3 ) );

  // произвольная пересекающаяся прямая
  Line line4( pA, Point2D( 3, 0) ); // y = -x + 3
  ASSERT_FALSE( line1.isSameTo( line4 ) );

  // совпадающие вертикальные прямые
  Line verticalLine1( 1, 0, -1 );
  Line verticalLine2( 5, 0, -5 );
  ASSERT_TRUE( verticalLine1.isSameTo( verticalLine2 ) );
  Line verticalLine3( 5, 0, -6 );
  ASSERT_FALSE( verticalLine1.isSameTo( verticalLine3 ) );

  // совпадающие горизонтальные прямые
  Line horizontalLine1( 0, 1, -1 );
  Line horizontalLine2( 0, 5, -5 );
  ASSERT_TRUE( horizontalLine1.isSameTo( horizontalLine2 ) );
  Line horizontalLine3( 0, 5, -6 );
  ASSERT_FALSE( horizontalLine1.isSameTo( horizontalLine3 ) );

  // совпадающие наклонные прямые
  Line casLine1( 2, -1, -3 );
  Line casLine2( 20, -10, -30 );
  ASSERT_TRUE( casLine1.isSameTo( casLine2 ) );
  Line casLine3( 20, -10, -300 );
  ASSERT_FALSE( casLine1.isSameTo( casLine3 ) );
  Line casLine4( 20, -100, -30 );
  ASSERT_FALSE( casLine1.isSameTo( casLine4 ) );
}
//==============================================================================
//==============================================================================
TEST_F( LineTest, crossPointTest )
{
  Point2D crossPoint;

  Line line1( pA, pE ); // y = x - 1
  Line line2( pA, pC ); // 3x - y -5 = 0 
  Line line3( pE, pA ); // y = x - 1
  Line line4( Point2D( 12, 1 ), Point2D( 16, 5 ) ); // y = x - 11

  Line horizontalLine1( pA, Point2D( 5, 1 ) ); // y = 1
  Line horizontalLine2( pB, Point2D( 5, 4 ) ); // y = 4

  // две произвольные прямые
  ASSERT_TRUE( line1.crossPoint( line2, crossPoint ) );
  ASSERT_TRUE( isEqual( crossPoint, pA ) );

  // две совпадающие прямые
  ASSERT_FALSE( line1.crossPoint( line3, crossPoint ) );
  ASSERT_TRUE( isNull( crossPoint.getX( ) ) );
  ASSERT_TRUE( isNull( crossPoint.getY( ) ) );
  ASSERT_THROW( line1.crossPoint(line3), GeometryFailure );
  try
  {
    line1.crossPoint(line3);
  }
  catch( const GeometryFailure& exception )
  {
    ASSERT_EQ( GeometryEnumFailure::SameLinesIntersect, exception.getCode( ) );
    ASSERT_EQ( geomLibTitle, exception.getLib( ) );
  }
  Line line11( 5, -5, -5 ); // 5y = 5x - 5
  ASSERT_THROW( line1.crossPoint( line11 ), GeometryFailure );
  try
  {
    line1.crossPoint( line11 );
  }
  catch( const GeometryFailure& exception )
  {
    ASSERT_EQ( GeometryEnumFailure::SameLinesIntersect, exception.getCode( ) );
    ASSERT_EQ( geomLibTitle, exception.getLib( ) );
  }

  // две параллельные прямые
  ASSERT_FALSE( line1.crossPoint( line4, crossPoint ) );
  ASSERT_TRUE( isNull( crossPoint.getX( ) ) );
  ASSERT_TRUE( isNull( crossPoint.getY( ) ) );
  ASSERT_THROW( line1.crossPoint( line4 ), GeometryFailure );
  try
  {
    line1.crossPoint( line4 );
  }
  catch( const GeometryFailure& exception )
  {
    ASSERT_EQ( GeometryEnumFailure::ParallelLinesIntersect, exception.getCode( ) );
    ASSERT_EQ( geomLibTitle, exception.getLib( ) );
  }


  // если первая прямая горизонтальная (отдельная ветка решения)
  // с параллельной прямой пересечений нет
  ASSERT_FALSE( horizontalLine1.crossPoint( horizontalLine2, crossPoint ) );
  ASSERT_TRUE( isNull( crossPoint.getX( ) ) );
  ASSERT_TRUE( isNull( crossPoint.getY( ) ) );
  ASSERT_THROW( horizontalLine1.crossPoint( horizontalLine2 ), GeometryFailure );
  try
  {
    horizontalLine1.crossPoint( horizontalLine2 );
  }
  catch( const GeometryFailure& exception )
  {
    ASSERT_EQ( GeometryEnumFailure::ParallelLinesIntersect, exception.getCode( ) );
    ASSERT_EQ( geomLibTitle, exception.getLib( ) );
  }

  // с обычной прямой пересечение есть
  ASSERT_TRUE( horizontalLine1.crossPoint( line1, crossPoint ) );
  ASSERT_TRUE( isEqual( crossPoint, pA ) );
}
//==============================================================================
//==============================================================================
TEST_F(LineTest, reversedLineTest)
{
  // вертикальная прямая
  Line verticalLine(pA, pB); // x = 2
  ASSERT_TRUE(isEqual(verticalLine.reversedLine().getSlope(),
                      verticalLine.getReversedSlope()));

  // горизонтальная прямая
  Line horizontalLine(pA, pD); // y = 1
  ASSERT_TRUE(isEqual(verticalLine.reversedLine().getSlope(),
                      verticalLine.getReversedSlope()));

  // обычная прямая
  Line line(pA, pC); // 3x - y - 5 = 0 
  ASSERT_TRUE(isEqual(verticalLine.reversedLine().getSlope(),
                      verticalLine.getReversedSlope()));
  // прямая, направленная назад
  Line inverseLine(pC, pA); // 3x - y - 5 = 0 
  ASSERT_TRUE(isEqual(verticalLine.reversedLine().getSlope(),
                      verticalLine.getReversedSlope()));

  // прямая с отрицательным углом наклона
  line = Line(pB, pD); // -3x - y + 10 = 0 
  ASSERT_TRUE(isEqual(verticalLine.reversedLine().getSlope(),
                      verticalLine.getReversedSlope()));
  // прямая с отрицательным углом наклона, направленная назад
  inverseLine = Line(pD, pB); // -3x - y + 10 = 0 
  ASSERT_TRUE(isEqual(verticalLine.reversedLine().getSlope(),
                      verticalLine.getReversedSlope()));
}
//==============================================================================
//==============================================================================
TEST_F( LineTest, parallelLineTest )
{
  // Построение параллельной линии через заданную точку

  Line line1( pA, pE ); // y = x - 1

  // особый случай: точка лежит на прямой
  Line retLine = line1.parallelLine( Point2D( 15, 14 ) );
  ASSERT_TRUE( isEqual( line1, retLine ) );

  // исходная прямая не вертикальна
  Line line2( Point2D( 12, 1 ), Point2D( 16, 5 ) ); // y = x - 11
  retLine = line1.parallelLine( Point2D (16, 5) );
  ASSERT_TRUE( isEqual( retLine, line2 ) );

  // исходная  прямая вертикальна (отдельная ветка решения)
  Line verticalLine1( pA, Point2D( 2, 10 ) );
  Line verticalLine2( pC, Point2D( 3, 8 ) );
  retLine = verticalLine1.parallelLine( Point2D( 3, 8 ) );
  ASSERT_TRUE( isEqual( retLine, verticalLine2 ) );

  // Построение параллельной линии на заданном расстоянии от исходной
  
  // вертикальная прямая
  Line verticalLine(pA, pB); // x = 2
  Line parallelLine(verticalLine.parallelLine(1., true)); // x = 1;
  ASSERT_TRUE(isEqual(parallelLine, Line(Point2D(1., 0.), Point2D(1., 1.))));

  verticalLine = Line(2, 0, -4);
  parallelLine = Line(verticalLine.parallelLine(1., true)); // x = 1;
  ASSERT_TRUE(isEqual(parallelLine, Line(Point2D(1., 0.), Point2D(1., 1.))));
  
  parallelLine = Line(verticalLine.parallelLine(1., false)); // x = 3;
  ASSERT_TRUE(isEqual(parallelLine, Line(Point2D(3., 0.), Point2D(3., 1.))));

  // вертикальная прямая, направленная назад
  verticalLine = Line(pB, pA);
  parallelLine = Line(verticalLine.parallelLine(1., true)); // x = 3;
  ASSERT_TRUE(isEqual(parallelLine, Line(Point2D(3., 1.), Point2D(3., 0.))));

  parallelLine = Line(verticalLine.parallelLine(1., false)); // x = 1;
  ASSERT_TRUE(isEqual(parallelLine, Line(Point2D(1., 1.), Point2D(1., 0.))));

  // горизонтальная прямая
  Line horizontalLine(pA, pD); // y = 1
  parallelLine = Line(horizontalLine.parallelLine(1., true)); // y = 2;
  ASSERT_TRUE(isEqual(parallelLine, Line(0., 2.)));

  parallelLine = Line(horizontalLine.parallelLine(1., false)); // y = 0;
  ASSERT_TRUE(isEqual(parallelLine, Line(0., 0.)));

  // горизонтальная прямая, направленная назад
  horizontalLine = Line(pD, pA); // y = 1
  parallelLine = Line(horizontalLine.parallelLine(1., true)); // y = 0;
  ASSERT_TRUE(isEqual(parallelLine, Line(0., 0.).reversedLine()));

  parallelLine = Line(horizontalLine.parallelLine(1., false)); // y = 2;
  ASSERT_TRUE(isEqual(parallelLine, Line(0., 2.).reversedLine()));

  // обычная прямая
  Line line(pA, pC); // y = 3x - 5
  parallelLine = Line(line.parallelLine(sin(atan(3.)), true)); // y = 3x - 2
  ASSERT_TRUE(isEqual(parallelLine, Line(3., -2.)));

  parallelLine = Line(line.parallelLine(sin(atan(3.)), false)); // y = 3x - 8
  ASSERT_TRUE(isEqual(parallelLine, Line(3., -8.)));

  // прямая, направленная назад
  Line inverseLine(pC, pA); // y = 3x - 5
  parallelLine = Line(inverseLine.parallelLine(sin(atan(3.)),
                      true)); // y = 3x - 8
  ASSERT_TRUE(isEqual(parallelLine, Line(3., -8.).reversedLine()));

  parallelLine = Line(inverseLine.parallelLine(sin(atan(3.)),
                      false)); // y = 3x - 2
  ASSERT_TRUE(isEqual(parallelLine, Line(3., -2.).reversedLine()));

  // прямая с отрицательным углом наклона
  line = Line(pB, pD); // y = -3x + 10
  parallelLine = Line(line.parallelLine(sin(atan(3.)), true)); // y = -3x + 13
  ASSERT_TRUE(isEqual(parallelLine, Line(-3., 13.)));

  parallelLine = Line(line.parallelLine(sin(atan(3.)), false)); // y = -3x + 7
  ASSERT_TRUE(isEqual(parallelLine, Line(-3., 7.)));

  // прямая с отрицательным углом наклона, направленная назад
  inverseLine = Line(pD, pB); // y = -3x + 10
  parallelLine = Line(inverseLine.parallelLine(sin(atan(3.)),
                      true)); // y = -3x + 7
  ASSERT_TRUE(isEqual(parallelLine, Line(-3., 7.).reversedLine()));

  parallelLine = Line(inverseLine.parallelLine(sin(atan(3.)),
                      false)); // y = -3x + 13
  ASSERT_TRUE(isEqual(parallelLine, Line(-3., 13.).reversedLine()));
}
//==============================================================================
//==============================================================================
TEST_F(LineTest, parallelTranslationTest)
{
  // Линия с положительным угловым коэффициентом
  Line lineFirst(Point2D(0., 6.), Point2D(1., 4.));
  // Линия с отрицательным угловым коэффициентом
  Line lineSecond(Point2D(3., -1.), Point2D(5., 0.));
  // Горизонтальная линия
  Line horizontalLine(Point2D(0., -4.), Point2D(1., -4.));
  // Вертикальная линия
  Line verticalLine(Point2D(7., 0.), Point2D(7., 1.));

  // Направим их в обратную сторону
  Line lineFirstReverse(Point2D(1., 4.), Point2D(0., 6.));
  Line lineSecondReverse(Point2D(5., 0.), Point2D(3., -1.));
  Line horizontalLineReverse(Point2D(1., -4.), Point2D(0., -4.));
  Line verticalLineReverse(Point2D(7., 1.), Point2D(7., 0.));

  Point2D newCoordCenter(0., 0.);

  // Ничего не должно измениться, так как система координат не менялась
  Line resultLine = lineFirst.parallelTranslation(newCoordCenter);

  ASSERT_TRUE(resultLine.isSameTo(lineFirst));

  // Зададим другой центр системы координат
  newCoordCenter = Point2D(4., 1.);

  resultLine = lineFirst.parallelTranslation(newCoordCenter);

  double a, b, c; // коэффициенты полученной прямой
  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, -2));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, -3));

  resultLine = lineSecond.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 0.5));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, -1.5));

  resultLine = horizontalLine.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 0));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, -5));

  resultLine = verticalLine.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 1));
  ASSERT_TRUE(isEqual(b, 0));
  ASSERT_TRUE(isEqual(c, -3));

  resultLine = lineFirstReverse.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, -2));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, -3));

  resultLine = lineSecondReverse.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 0.5));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, -1.5));

  resultLine = horizontalLineReverse.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 0));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, -5));

  resultLine = verticalLineReverse.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 1));
  ASSERT_TRUE(isEqual(b, 0));
  ASSERT_TRUE(isEqual(c, -3));

  // Зададим другой центр системы координат
  newCoordCenter = Point2D(-1., 1.);

  resultLine = lineFirst.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, -2));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, 7));

  resultLine = lineSecond.parallelTranslation(newCoordCenter);
  
  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 0.5));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, -4));

  resultLine = horizontalLine.parallelTranslation(newCoordCenter);
  
  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 0));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, -5));
  
  resultLine = verticalLine.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 1));
  ASSERT_TRUE(isEqual(b, 0));
  ASSERT_TRUE(isEqual(c, -8));

  // Зададим другой центр системы координат
  newCoordCenter = Point2D(-1., -2.);

  resultLine = lineFirst.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, -2));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, 10));

  resultLine = lineSecond.parallelTranslation(newCoordCenter);
  
  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 0.5));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, -1));
  
  resultLine = horizontalLine.parallelTranslation(newCoordCenter);
  
  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 0));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, -2));
  
  resultLine = verticalLine.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 1));
  ASSERT_TRUE(isEqual(b, 0));
  ASSERT_TRUE(isEqual(c, -8));

  // Зададим другой центр системы координат
  newCoordCenter = Point2D(3., -1.);

  resultLine = lineFirst.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, -2));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, 1));

  resultLine = lineSecond.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 0.5));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, 0));

  resultLine = horizontalLine.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 0));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, -3));

  resultLine = verticalLine.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 1));
  ASSERT_TRUE(isEqual(b, 0));
  ASSERT_TRUE(isEqual(c, -4));

  // Проверим, что, если увеличить коэффициенты, ничего не изменится
  std::tie(a, b, c) = lineFirst.getAllCoeffs();
  lineFirst = Line(a * 2., b * 2., c * 2.);

  resultLine = lineFirst.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, -2));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, 1));

  std::tie(a, b, c) = lineSecond.getAllCoeffs();
  lineSecond = Line(a * 2., b * 2., c * 2.);

  resultLine = lineSecond.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 0.5));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, 0));

  std::tie(a, b, c) = horizontalLine.getAllCoeffs();
  horizontalLine = Line(a * 2., b * 2., c * 2.);

  resultLine = horizontalLine.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 0));
  ASSERT_TRUE(isEqual(b, -1));
  ASSERT_TRUE(isEqual(c, -3));

  std::tie(a, b, c) = verticalLine.getAllCoeffs();
  verticalLine = Line(a * 2., b * 2., c * 2.);

  resultLine = verticalLine.parallelTranslation(newCoordCenter);

  std::tie(a, b, c) = resultLine.getAllCoeffs();
  ASSERT_TRUE(isEqual(a, 1));
  ASSERT_TRUE(isEqual(b, 0));
  ASSERT_TRUE(isEqual(c, -4));
}
//==============================================================================
//==============================================================================
TEST_F( LineTest, perpendicularLineTest )
{
  Line line( pA, pE ); // y = x - 1

  // исходная прямая не вертикальна
  Line retLine = line.perpendicularLine( Point2D( 5, 5 ) );
  ASSERT_TRUE( isEqual(retLine, Line(-1, -1, 10)) );

  // исходная прямая вертикальна (отдельная ветка решения)
  Line verticalLine( pA, Point2D( 2, 10 ) ); // x = 2
  retLine = verticalLine.perpendicularLine( Point2D( 5, 1 ) );
  Line horizontalLine( pA, Point2D( 5, 1 ) ); // y = 1
  ASSERT_TRUE( isEqual( retLine, horizontalLine ) );

  // исходная прямая горизонтальна (отдельная ветка решения)
  retLine = horizontalLine.perpendicularLine( Point2D( 2, 5 ) );
  ASSERT_TRUE( isEqual( retLine, verticalLine ) );
}
//==============================================================================
//==============================================================================
TEST_F(LineTest, findPointTest)
{
  Line line1(Point2D(0, 1), Radian(M_PI / 4));
  double dist = sqrt(2);
  ASSERT_EQ(line1.findPoint(Point2D(0, 1), dist), Point2D(1., 2.));

  Line line2(Point2D(1, 2), Radian(-3 * M_PI / 4));
  ASSERT_EQ(line2.findPoint(Point2D(1, 2), dist), Point2D(0., 1.));

  Line line3(Point2D(0, 1), Radian(-M_PI / 4));
  ASSERT_EQ(line3.findPoint(Point2D(0, 1), dist), Point2D(1., 0.));

  Line line4(Point2D(1, 0), Radian(3 * M_PI / 4));
  ASSERT_EQ(line4.findPoint(Point2D(1, 0), dist), Point2D(0., 1.));

  Line line5(Point2D(0, 0), Radian(M_PI / 2));
  dist = 1;
  ASSERT_EQ(line5.findPoint(Point2D(0, 0), dist), Point2D(0., 1.));

  Line line6(Point2D(0, 0), Radian(-M_PI / 2));
  ASSERT_EQ(line6.findPoint(Point2D(0, 0), dist), Point2D(0., -1.));

  Line line7(Point2D(0, 0), Radian(0));
  ASSERT_EQ(line7.findPoint(Point2D(0, 0), dist), Point2D(1., 0.));

  Line line8(Point2D(0, 0), Radian(M_PI));
  ASSERT_EQ(line8.findPoint(Point2D(0, 0), dist), Point2D(-1., 0.));

  Line line9(Point2D(0, 0), Radian (-M_PI));
  ASSERT_EQ(line9.findPoint(Point2D(0, 0), dist), Point2D(-1., 0.));

  //! Проверить исправление некорректной точки и исключение
  ASSERT_EQ(line9.findPoint(Point2D(0, 0.1), dist, true), Point2D(-1., 0.));

  ASSERT_THROW(line9.findPoint(Point2D(0, 0.1), dist), GeometryFailure);
  try
  {
    ASSERT_TRUE(line9.findPoint(Point2D(0, 0.1), dist) == Point2D(-1., 0.));
  }
  catch (const GeometryFailure& exception)
  {
    ASSERT_EQ(GeometryEnumFailure::IncorrectPoint, exception.getCode());
    ASSERT_EQ(geomLibTitle, exception.getLib());
  }
}
//==============================================================================
//==============================================================================
TEST_F(LineTest, distanceFromPointTest)
{
  // Расстояние от центра системы координат
  Point2D point(0., 0.);
  Line line(Point2D(4., 0.), Point2D(0., 4.));
  Line lineParallelX(Point2D(4., 4.), Point2D(0., 4.));
  Line lineParallelY(Point2D(4., 0.), Point2D(4., 4.));
  double dist = 2. * sqrt(2.);

  ASSERT_NEAR(
    line.distanceFromPoint(point), dist, ACCURACY_DOUBLE);
  ASSERT_EQ(lineParallelX.distanceFromPoint(point), 4.);
  ASSERT_EQ(lineParallelY.distanceFromPoint(point), 4.);

  // Те же уравнения, но в смещённой системе координат
  point = Point2D(1., 3.);
  line = Line(Point2D(5., 3.), Point2D(1., 7.));
  lineParallelX = Line(Point2D(5., 7.), Point2D(1., 7.));
  lineParallelY = Line(Point2D(5., 3.), Point2D(5., 7.));

  ASSERT_NEAR(
    line.distanceFromPoint(point), dist, ACCURACY_DOUBLE);
  ASSERT_EQ(lineParallelX.distanceFromPoint(point), 4.);
  ASSERT_EQ(lineParallelY.distanceFromPoint(point), 4.);

  point = Point2D(4., -2.);
  // Линия, проходящая через центр системы координат
  line = Line(Point2D(-1., -2.), Point2D(0., 0.));
  dist = 2 * sqrt(5);

  ASSERT_NEAR(
    line.distanceFromPoint(point), dist, ACCURACY_DOUBLE);

  // Координатные оси
  lineParallelX = Line(Point2D(1., 0.), Point2D(0., 0.));
  lineParallelY = Line(Point2D(0., 0.), Point2D(0., 1.));

  ASSERT_EQ(lineParallelX.distanceFromPoint(point), 2.);
  ASSERT_EQ(lineParallelY.distanceFromPoint(point), 4.);

  // Линии, параллельные координатным осям
  lineParallelX = Line(Point2D(0., -4.), Point2D(1., -4.));
  lineParallelY = Line(Point2D(8., 0.), Point2D(8., -1.));

  ASSERT_EQ(lineParallelX.distanceFromPoint(point), 2.);
  ASSERT_EQ(lineParallelY.distanceFromPoint(point), 4.);

  // Линия, проходящая через центр окружности
  line = Line(Point2D(4., -2.), Point2D(5., -1.));

  ASSERT_EQ(line.distanceFromPoint(point), 0.);
}
//==============================================================================
//==============================================================================
TEST_F(LineTest, findNearestLinePointTest)
{
  Point2D point(4., -2.);
  
  Line line(Point2D(-1., -2.), Point2D(0., 0.));
  Line lineParallelX = Line(Point2D(1., 2.), Point2D(0., 2.));
  Line lineParallelY = Line(Point2D(0., 0.), Point2D(0., 1.));

  ASSERT_EQ(line.findNearestLinePoint(point), Point2D(0., 0.));
  ASSERT_EQ(
    lineParallelX.findNearestLinePoint(point), Point2D(4., 2.));
  ASSERT_EQ(
    lineParallelY.findNearestLinePoint(point), Point2D(0., -2.));

  lineParallelX = Line(Point2D(0., 1.), Point2D(1., 1.));
  lineParallelY = Line(Point2D(1., 0.), Point2D(1., -1.));

  ASSERT_EQ(
    lineParallelX.findNearestLinePoint(point), Point2D(4., 1.));
  ASSERT_EQ(
    lineParallelY.findNearestLinePoint(point), Point2D(1., -2.));

  line = Line(Point2D(1., -2.), Point2D(4., 1.));
  Line line2(Point2D(4., 1.), Point2D(7., -2.));
  lineParallelX = Line(Point2D(0., -4.), Point2D(1., -4.));
  lineParallelY = Line(Point2D(6., 0.), Point2D(6., -1.));

  ASSERT_EQ(line.findNearestLinePoint(point), Point2D(2.5, -0.5));
  ASSERT_EQ(line2.findNearestLinePoint(point), Point2D(5.5, -0.5));
  ASSERT_EQ(
    lineParallelX.findNearestLinePoint(point), Point2D(4., -4.));
  ASSERT_EQ(
    lineParallelY.findNearestLinePoint(point), Point2D(6., -2.));

  line = Line(Point2D(4., -2.), Point2D(5., 1.));

  ASSERT_EQ(line.findNearestLinePoint(point), Point2D(4., -2.));
}
//==============================================================================
//==============================================================================
} //! Tests
} //! Geometry
