#include <Geometry/Objects/Circle.hpp>

#include <Geometry/GeometryException.hpp>
#include <Geometry/Operations/AtPoints2D.hpp>
//! MathTools
#include <MathTools/AngularUnits/Radian.hpp>
#include <MathTools/BasicMath/Arithmetic.hpp>
//==============================================================================
namespace Geometry
{
using namespace PrimaryEntities;
//==============================================================================
Circle::
Circle( const Point2D & center, const double radius )
{
  if( radius < 0 )
    throw GeometryFailure( GeometryEnumFailure::NegativeRadius );

  mCenter = center;
  mRadius = radius;
}
//==============================================================================
Circle::Circle( const Point2D & pA, const Point2D & pB, const Point2D & pC )
{
  buildNearTriangle( pA, pB, pC );
}
//==============================================================================
void Circle::
setRadius( const double radius )
{
  if( radius < 0 )
    throw GeometryFailure( GeometryEnumFailure::NegativeRadius );

  mRadius = radius;
}
//==============================================================================
void Circle::
setParams( const Point2D & center, const double radius )
{
  if( radius < 0 )
    throw GeometryFailure( GeometryEnumFailure::NegativeRadius );

  mCenter = center;
  mRadius = radius;
}
//==============================================================================
void Circle::setCircle( const CircleLite2D & circle )
{
  if( circle.radius < 0 )
    throw GeometryFailure( GeometryEnumFailure::NegativeRadius );

  mCenter = circle.center;
  mRadius = circle.radius;
}
//==============================================================================
void Circle::
buildNearTriangle( const Point2D & a, const Point2D & b, const Point2D & c )
{
  using BasicMath::sqr;

  double aX( a.getX( ) );
  double aY( a.getY( ) );
  double bX( b.getX( ) );
  double bY( b.getY( ) );
  double cX( c.getX( ) );
  double cY( c.getY( ) );
  double aLength( distance( b, c ) );
  double bLength( distance( a, c ) );

  double divisor( 2. * ( aX * ( bY - cY ) + bX * ( cY - aY ) + cX * ( aY - bY ) ) );

  //! Если вершины треугольника лежат на одной прямой...
  if( BasicMath::isNull( divisor ) )
  {
    //! Если вершины не совпадают, круг не определён.
    if( ( !BasicMath::isNull( aLength ) ) || ( !BasicMath::isNull( bLength ) ) )
      throw GeometryFailure(
        GeometryEnumFailure::CircumscribedCircleForBadTriangle );
    //! Иначе возвращаем окружность, вырожденную в точку.
    else
    {
      mCenter = a;
      mRadius = 0;
      return;
    }
  }

  double circleCenterX(
    ( ( aX * aX + aY * aY ) * ( bY - cY )
      + ( bX * bX + bY * bY ) * ( cY - aY )
      + ( cX * cX + cY * cY ) * ( aY - bY ) )
    / divisor );
  double circleCenterY(
    ( ( aX * aX + aY * aY ) * ( cX - bX )
      + ( bX * bX + bY * bY ) * ( aX - cX )
      + ( cX * cX + cY * cY ) * ( bX - aX ) )
    / divisor );

  mCenter.set( circleCenterX, circleCenterY );
  mRadius = sqrt( sqr( aX - circleCenterX ) + sqr( aY - circleCenterY ) );
}
//==============================================================================
bool Circle::
isInsideCircle( const Point2D & point, const double eps )
{
  using BasicMath::sqr;

  double dist = sqrt( sqr( point.getX( ) - mCenter.getX( ) )
    + sqr( point.getY( ) - mCenter.getY( ) ) );
  return dist <= mRadius + eps;
}
//==============================================================================
bool Circle::isOnCircle( const Point2D & point, const double eps )
{
  using BasicMath::sqr;

  double dist = sqrt( sqr( point.getX( ) - mCenter.getX( ) )
    + sqr( point.getY( ) - mCenter.getY( ) ) );
  return fabs ( dist - mRadius ) <= eps;
}
//==============================================================================
Point2D Circle::
tangentPoint( const Point2D & point, const bool toLeftSide )
{
  int sgn = ( toLeftSide ) ? 1 : -1;

  //расстояние от центра окружности до точки, через которую проводим касательную
  double dx = mCenter.getX() - point.getX();
  double dy = mCenter.getY() - point.getY();
  double distToCenter = sqrt( dx*dx + dy*dy );

  //если искомая точка находится внутри окружности - генерируем исключение
  if( distToCenter < mRadius )
    throw GeometryFailure( GeometryEnumFailure::TangentThroughInsidePoint );

  // ищем угол наклона отрезка от заданной точки к центру окружности
  // от направления направо, против часовой, в диапазоне [-pi:pi]
  Radian alpha;
  int dySgn = dy > 0 ? 1 : -1;
  if( dx > 0 )
    alpha = Radian ( atan( dy / dx ) );
  else
  {
    if( dx < 0 )
      alpha = Radian ( atan( dy / dx ) + M_PI * dySgn );
    else
      alpha = Radian ( M_PI / 2. * dySgn );
  }

  // ищем, на сколько касательная отклоняется от линии симметрии
  // всегда будет между 0 и pi/2
  Radian beta = Radian ( asin( mRadius / distToCenter ) );

  // угол наклона радиуса, идущего к нужной точке касания
  // возможно, с дополнительным периодом
  Radian gamma = alpha + Radian ( ( beta + M_PI / 2. ) * sgn );

  return Point2D ( mCenter.getX() + mRadius * cos( gamma.get() ),
    mCenter.getY() + mRadius * sin( gamma.get() ) );
}
//==============================================================================
Line Circle::tangentLine( const Point2D & point, const bool toLeftSide )
{
  // ищем точку касания окружности и искомой прямой
  // если была задана точка внутри окружности, генерируется соответствующее исключение
  auto tangentP = tangentPoint( point, toLeftSide );

  if( tangentP == point )
  { // задана точка на окружности
    // возвращаем прямую, перпендикулярную радиусу к точке касания
    Line radius( mCenter, tangentP );
    return radius.perpendicularLine( tangentP );
  }
  else
  { // задана точка вне окружности
    // возвращаем прямую через точку касания и заданную точку
    return Line( tangentP, point );
  }
}
//==============================================================================
Line Circle::tangentLine(Radian lineSlope, const bool toLeftSide)
{
  // Нормализация угла наклона
  lineSlope = lineSlope.normalize();

  int sgn = toLeftSide ? 1 : -1;

  // Точка касания
  Point2D tangentPoint(mCenter.getX() - sgn * mRadius * sin(lineSlope.get()),
                       mCenter.getY() + sgn * mRadius * cos(lineSlope.get()));

  // Если прямая должна быть вертикальной
  if (fabs(lineSlope.get()) == (M_PI / 2.))
  {
    Line line(1., 0., -tangentPoint.getX());

    // Корректируем направление линии
    if (lineSlope < 0)
      line = line.reversedLine();

    return line;
  }

  double k = tan(lineSlope.get());
  Line line(k, tangentPoint.getY() - k * tangentPoint.getX());

  // Корректируем направление линии
  if (fabs(lineSlope.get()) > M_PI / 2.)
    line = line.reversedLine();

  return line;
}
//==============================================================================
} //! Geometry
//==============================================================================
