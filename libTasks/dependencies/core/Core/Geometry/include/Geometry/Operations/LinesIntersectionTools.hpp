#ifndef GEOMETRY_OPERATIONS_LINEINTERSECTIONTOOLS_HPP
#define GEOMETRY_OPERATIONS_LINEINTERSECTIONTOOLS_HPP
//==============================================================================
#include <Geometry/Operations/AtPoints2D.hpp>
#include <Geometry/Objects/Line.hpp>
//! AbstractOperations (для упрощения подключения в месте использования)
#include <AbstractOperations/LinesIntersectionTools.hpp>
//! std
#include <memory>
//==============================================================================
namespace AbstractOperations
{
//==============================================================================
using Geometry::distance;
//==============================================================================
template <>
class IntersectorHelper<Point2D>
{
public:
//==============================================================================
 /*!
  *\brief расстояние между точками
  *\param[in] first - первая точка
  *\param[in] second - вторая точка
  *\return расстояние между двумя точками [м]
  */
  static
  double distance( const Point2D& first, const Point2D& second )
  {
    return Geometry::distance( first, second );
  }
//==============================================================================
 /*!
  *\brief Углы наклона отрезка, соединяющего точки
  *\details На плоскости угол наклона отрезка постоянен. Будут возвращены два
  *         одинаковых значения
  *\param[in] first - первая точка
  *\param[in] second - вторая точка
  *\return пара углов наклона (в полуинтервале [0, 2π) рад)
  *\throws AbstractOperationsFailure : LineThroughOnePoint
  */
  static
  std::pair<Radian, Radian> inclinations(
    const Point2D& first, const Point2D& second )
  {
    // из пересекатора не должны прилетать такие точки, но вдруг
    if( first == second )
      throw AbstractOperationsFailure( LineThroughOnePoint, "", "Geometry/inclinations" );

    Radian angle( std::fmod(
      std::atan2(
        second.getY( ) - first.getY( ),
        second.getX( ) - first.getX( ) ),
      2. * M_PI ) );
    if( angle.get( ) < 0. )
      angle += Radian( 2. * M_PI );

    return std::make_pair( angle, angle );
  }
//==============================================================================
 /*!
  *\brief Углы наклона отрезка, а также его длина
  *\details На плоскости угол наклона отрезка постоянен. Будут возвращены два
  *         одинаковых значения в полуинтервале [0, 2π) рад
  *\param[in] first - первая точка
  *\param[in] second - вторая точка
  *\return - кортеж, содержащий пару углов наклона [радианы] и длину [м]
  */
  static
  std::tuple<Radian, Radian, double> inclinationsAndDistance(
    const Point2D& first, const Point2D& second )
  {
    // внутренняя функция проверит несовпадение точек. Тут это можно опустить
    Radian angle = inclinations( first, second ).first;
    double dist = distance( first, second );
    return std::tuple<Radian, Radian, double>( angle, angle, dist );
  }
//==============================================================================
 /*!
  *\brief Найти точку, делящую интервал в заданном соотношении
  *\details Найти на AB точку C такую, что portion = |AC| / |AB|
  *\param[in] beg - начало интервала
  *\param[in] end - конец интервала
  *\param[in] portion - заданное соотношение, должен принадлежать [0, 1]
  *\return найденная точка
  *\throws AbstractOperationsFailure : PointNotInInterval
  */
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
 /*!
  *\brief Найти простейшее пересечение
  *\details Заранее известно, что отрезки пересекаются и ровно в одной внутренней точке.
  *         Попадание найденного пересечения на отрезки не проверяется
  *\param[in] firstBeg - начало первого отрезка
  *\param[in] firstEnd - конец первого отрезка
  *\param[in] secondBeg - начало второго отрезка
  *\param[in] secondEnd - конец второго отрезка
  *\param[out] crossP - найденное пересечение (координаты)
  *\throws AbstractOperationsFailure : FakeIntersection
  */
  static
  void findSingleIntersection( const Point2D& firstBeg, const Point2D& firstEnd,
    const Point2D& secondBeg, const Point2D& secondEnd,
    Point2D& crossP )
  {
    using namespace Geometry;
    Line first( firstBeg, firstEnd );
    Line second( secondBeg, secondEnd );
    try
    {
      crossP = first.crossPoint( second );
    }
    catch( ... )
    {
      throw AbstractOperationsFailure( 
        FakeIntersection, "", "Geometry/findSingleIntersection" );
    }
  }
//==============================================================================
};
//==============================================================================
} // ! AbstractOperations
//==============================================================================
#endif // ! GEOMETRY_OPERATIONS_LINEINTERSECTIONTOOLS_HPP