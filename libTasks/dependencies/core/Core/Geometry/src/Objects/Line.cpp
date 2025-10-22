#include <Geometry/Objects/Line.hpp>

#include <Geometry/Operations/AtPoints2D.hpp>
#include <Geometry/GeometryException.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//==============================================================================
using BasicMath::isNull;
using BasicMath::isEqual;
//==============================================================================
namespace Geometry
{
//==============================================================================
Line::Line( const Point2D & firstPoint, const Point2D & secondPoint )
{
  // вместо длинных и отвлекающих имен вводим понятные обозначения
  double x1 = firstPoint.getX( );
  double y1 = firstPoint.getY( );
  double x2 = secondPoint.getX( );
  double y2 = secondPoint.getY( );

  mA = y2 - y1;
  mB = -x2 + x1;
  if ( isNull(mA) && isNull(mB) )
    throw GeometryFailure( GeometryEnumFailure::LineThroughOnePoint );
  mC = - mB * y1 - mA * x1;
  if ( isNull(mB) )
    mIsForward = ( y2 > y1 );
  else
    mIsForward = ( x2 > x1 );
}
//==============================================================================
Line::Line( const double a, const double b, const double c )
: mA (a)
, mB (b)
, mC (c)
, mIsForward (true)
{
  if( isNull( a ) && isNull( b ) )
    throw GeometryFailure( GeometryEnumFailure::LineWithZeroParams ); 
}
//==============================================================================
Line::Line( const double k, const double b )
: mA (k)
, mB (-1.)
, mC (b)
, mIsForward (true)
{}
//==============================================================================
Line::Line(const Point2D& firstPoint, Radian slope)
{
  //прямая параллельна оси ординат
  if (isNull(fabs(slope.get()) - M_PI / 2))
  {
    mA = 1;
    mB = 0;
    mC = -firstPoint.getX();
    mIsForward = (slope > 0);
  }
  else //произвольная прямая
  {
    mA = std::tan(slope.get());
    mB = -1;
    mC = firstPoint.getY() - mA * firstPoint.getX();
    mIsForward = (fabs(slope.get()) < M_PI / 2);
  }
}
//==============================================================================
Radian Line::getSlope( ) const
{
  Radian retVal;

  if( ! isNull(mB) )
    retVal = Radian( atan( -mA / mB ) );
  else
    retVal = Radian( M_PI / 2 );

  if( !mIsForward )
    retVal += Radian( M_PI );

  return retVal.normalize( );
}
//==============================================================================
Radian Line::getReversedSlope() const
{
  return (getSlope() + Radian(M_PI)).normalize();
}
//==============================================================================
Radian Line::getAngleTo( const Line& anotherLine ) const
{
  Radian retVal = this->getSlope( ) - anotherLine.getSlope( );

  return retVal.normalize( );
}
//==============================================================================
std::tuple <double, double, double> Line::getAllCoeffs( ) const
{
  return std::make_tuple( mA, mB, mC );
}
//==============================================================================
std::pair <double, double> Line::getBaseCoeffs( ) const
{
  if( ! isNull(mB) )
    return std::make_pair( -mA / mB, -mC / mB );
  else
    throw GeometryFailure
      ( GeometryEnumFailure::BaseCoefsForVerticalLine );
}
//==============================================================================
bool Line::isForward( ) const
{
  return mIsForward;
}
//==============================================================================
bool Line::isVertical( ) const
{
  return isNull(mB);
}
//==============================================================================
Line Line::perpendicularLine( const Point2D& point ) const
{
  Line retVal;
  retVal.mIsForward = mIsForward;

  retVal.mA = mB;
  retVal.mB = -mA;
  retVal.mC = -retVal.mA * point.getX( ) - retVal.mB * point.getY( );
  return retVal;
}
//==============================================================================
bool Line::hasPoint( const Point2D& point ) const
{
  return isNull( mA * point.getX( ) + mB * point.getY( ) + mC );
}
//==============================================================================
bool Line::hasPointNear(
  const Point2D& point, const double eps/* = ACCURACY_DOUBLE*/) const
{
  // опускаем перпендикуляр из точки на линию
  Line perpLine = perpendicularLine(point);

  // находим точку пересечения
  Point2D crossP = crossPoint(perpLine);

  // расстояние от заданной точки до точки пересечения должно быть
  // меньше заданной погрешности
  return (distance(point, crossP) < eps);
}
//==============================================================================
Line Line::reversedLine() const
{
  Line retVal(mA, mB, mC);
  retVal.mIsForward = !mIsForward;
  return retVal;
}
//==============================================================================
Line Line::parallelLine( const Point2D& point ) const
{
  // если точка лежит на прямой, нужно просто вернуть эту прямую
  if ( hasPoint( point ) )
    return *this;

  Line retVal;
  retVal.mIsForward = mIsForward;

  retVal.mA = mA;
  retVal.mB = mB;
  retVal.mC = -retVal.mA * point.getX( ) - retVal.mB * point.getY( );

  return retVal;
}
//==============================================================================
Line Line::parallelLine(double distance, bool isLeftSide) const
{
  int sgn = (isForward() == isLeftSide) ? 1 : (-1);

  Line retVal;

  // Линия параллельна оси y
  if (isVertical())
    retVal = Line(1., 0., mC / mA + sgn * distance);
  else
  {
    auto coefs = getBaseCoeffs();
    retVal = Line(
      coefs.first,
      coefs.second + sgn * distance / std::cos(std::atan(coefs.first)));
  }

  retVal.mIsForward = mIsForward;

  return retVal;
}
//==============================================================================
Line Line::parallelTranslation(const Point2D& point) const
{
  // если система координат осталась неизменной, ничего переносить не нужно
  if (point == Point2D(0., 0.))
    return *this;

  Line retVal;
  retVal.mIsForward = mIsForward;

  if (!isNull(mB))
  {
    retVal.mA = -mA / mB;
    retVal.mB = -1;
    retVal.mC = -mC / mB + retVal.mA * point.getX() - point.getY();
  }
  else
  {
    // случай вертикальной прямой
    retVal.mA = 1;
    retVal.mB = 0;
    retVal.mC = mC / mA + point.getX();
  }

  return retVal;
}
//==============================================================================
bool Line::isParallel( const Line& anotherLine ) const
{
  // достаем коэффициенты второй прямой
  double a2, b2, c2;
  std::tie( a2, b2, c2 ) = anotherLine.getAllCoeffs( );

  return isNull( mA * b2 - a2 * mB );
}
//==============================================================================
bool Line::isSameTo( const Line& anotherLine ) const
{
  // если заданные прямые не параллельны, то совпасть они точно не смогут
  if( ! isParallel( anotherLine ) )
    return false;

  // достаем коэффициенты второй прямой
  double a2, b2, c2;
  std::tie( a2, b2, c2 ) = anotherLine.getAllCoeffs( );

  // ищем пробную точку ( параллельность + общая точка = совпадение )
  if( isNull( mB ) )
  { // случай вертикальных прямых
    // выражаем x
    return isEqual( -mC / mA, -c2 / a2 );
  }
  else
  { // случай произвольных прямых
    // выражаем y для x = 1
    return isEqual( - mA / mB - mC / mB, - a2 / b2 - c2 / b2);
  }

  // недостижимый выход для слишком нервных компиляторов
  return false;
}
//==============================================================================
Point2D Line::crossPoint( const Line& anotherLine ) const
{
  Point2D retVal;
  // если пересечения не нашлось, выясняем причину и бросаем исключение
  if( ! crossPoint( anotherLine, retVal ) )
  {
    if( isSameTo( anotherLine ) )
      throw GeometryFailure( GeometryEnumFailure::SameLinesIntersect );
    else
      throw GeometryFailure( GeometryEnumFailure::ParallelLinesIntersect );
  }

  return retVal;
}
//==============================================================================
bool Line::crossPoint( const Line& anotherLine, Point2D& crossPoint ) const
{
  // подготавливаем точку для некорректной задачи
  crossPoint.set( 0, 0 );

  // достаем коэффициенты второй прямой
  double a2, b2, c2;
  std::tie( a2, b2, c2 ) = anotherLine.getAllCoeffs( );

  bool hasIntersection = false;
  const double detA ( mA * b2 - a2 * mB );
  if (fabs(detA) > ACCURACY_DOUBLE)
  {
    hasIntersection = true;
    const double x ( ( - mC * b2 + c2 * mB ) / detA );
    const double y ( ( - mA * c2 + a2 * mC ) / detA );
    crossPoint.set(x,y);
  }
  return hasIntersection;
}
//==============================================================================
Point2D Line::findPoint(
  const Point2D& point, double dist, bool changePoint /*= false*/,
  bool alongLine/* = true*/) const
{
  bool findingForward = alongLine ? mIsForward : !mIsForward;
  Point2D firstPoint = point;
  if (!hasPoint(point))
  {
    if (changePoint)
      firstPoint.set(point.getX(), -mA / mB * point.getX() - mC / mB);
    else
      throw GeometryFailure(GeometryEnumFailure::IncorrectPoint);
  }

  // Случай параллельности y
  if (isVertical())
  {
    if (findingForward)
      return Point2D(firstPoint.getX(), firstPoint.getY() + dist);

    return Point2D(firstPoint.getX(), firstPoint.getY() - dist);
  }

  // k и b - коэфициенты уравнения прямой
  double k, b;
  std::tie(k, b) = getBaseCoeffs();

  // Откладываем точку на заданное расстояние от заданной точки
  double offset = sqrt(dist * dist / (1 + k * k));
  double x 
    = (findingForward ? offset : -offset) + firstPoint.getX();

  return Point2D(x, k * x + b);
}
//==============================================================================
double Line::distanceFromPoint(const Point2D& point) const
{
  Line line = parallelTranslation(point);

  // Коэффициенты прямой
  double a, b, c;
  std::tie(a, b, c) = line.getAllCoeffs();

  return fabs(c) / sqrt(a * a + b * b);
}
//==============================================================================
Point2D Line::findNearestLinePoint(const Point2D& point) const
{
  Line line = parallelTranslation(point);

  // Коэффициенты прямой
  double a, b, c;
  std::tie(a, b, c) = line.getAllCoeffs();

  // Вычисляем точку и сразу возвращаем её в исходную систему координат
  return Point2D(-(a * c) / (a * a + b * b) + point.getX(),
    -(b * c) / (a * a + b * b) + point.getY());
}
//==============================================================================
} //! Geometry
//==============================================================================
