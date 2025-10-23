#include <AbstractOperations/LinesIntersectionTools.hpp>

#include <AbstractOperations/AbstractOperationsException.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeometryConverter.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace AbstractOperations
{
//==============================================================================
// Переопределяем опорные функции для классической двумерной точки.
// Данная реализация предназначена только для тестирования шаблонного 
// функционала!
//==============================================================================
template <>
class IntersectorHelper<Point2D>
{
public:
//==============================================================================
  static
  double distance( const Point2D& point1, const Point2D& point2 )
  {
    return sqrt( pow( point1.getX( ) - point2.getX( ), 2. )
      + pow( point1.getY( ) - point2.getY( ), 2. ) );
  }
//==============================================================================
  static
  std::pair<Radian, Radian> inclinations(
    const Point2D & first, const Point2D & second )
{
  // из пересекатора не должны прилетать такие точки, но вдруг
  if( first == second )
    throw AbstractOperationsFailure( LineThroughOnePoint );

  Radian angle ( std::fmod(
    std::atan2( second.getY() - first.getY(), second.getX() - first.getX() ),
    2. * M_PI ));
    if( angle.get( ) < 0. )
      angle += Radian( 2. * M_PI );

  return std::make_pair( angle, angle );
}
//==============================================================================
  static
  std::tuple<Radian, Radian, double> inclinationsAndDistance(
    const Point2D & first, const Point2D & second )
{
  // внутрення функция проверит несовпадение точек. Тут это можно опустить
  Radian angle = inclinations( first, second ).first;
  double dist = distance( first, second );
  return std::tuple<Radian, Radian, double>( angle, angle, dist );
}
//==============================================================================
  static
  Point2D pointByPortion( const Point2D& beg, const Point2D& end,
    const double portion )
{
  if( ( portion < 0 ) || ( portion > 1 ) )
    throw AbstractOperationsFailure(
      PointNotInInterval, "", "pointByPortion<Point2D>" );

  return Point2D(
    AbstractOperations::pointByPortion( beg.getX( ), end.getX( ), portion ),
    AbstractOperations::pointByPortion( beg.getY( ), end.getY( ), portion ) );
}
//==============================================================================
  static
  void findSingleIntersection( const Point2D& firstBeg, const Point2D& firstEnd,
    const Point2D& secondBeg, const Point2D& secondEnd,
    Point2D& crossP )
{
    Linestring2D first({firstBeg, firstEnd});
    Linestring2D second({secondBeg, secondEnd});
    auto intersectionRes
      = PrimaryEntities::toGeosGeometry(first)->intersection(
        PrimaryEntities::toGeosGeometry(second).get());

    if (intersectionRes->getGeometryTypeId() != geos::geom::GEOS_POINT)
      throw AbstractOperationsFailure(FakeIntersection,
                                      "", "findSingleIntersection");
    
    crossP = PrimaryEntities::fromGeosGeometry<Point2D>(intersectionRes.get());
}
//==============================================================================
};
//==============================================================================
} //! AbstractOperations
//==============================================================================
