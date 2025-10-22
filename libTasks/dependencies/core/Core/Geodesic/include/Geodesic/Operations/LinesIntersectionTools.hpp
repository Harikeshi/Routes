#ifndef GEODESIC_OPERATIONS_LINEINTERSECTIONTOOLS_HPP
#define GEODESIC_OPERATIONS_LINEINTERSECTIONTOOLS_HPP
//==============================================================================
#include <Geodesic/Operations/AtPoints2D.hpp>
#include <Geodesic/GeoLineTools/OrthodromyTools.hpp>
//! AbstractOperations
#include <AbstractOperations/LinesIntersectionTools.hpp>
#include <AbstractOperations/AbstractOperationsException.hpp>
//! Point
#include <Point/GeoPoint2D.hpp>
//! std
#include <memory>
//==============================================================================
namespace AbstractOperations
{
//==============================================================================
using Geodesic::Ellipsoid;
using Geodesic::EL_WGS_84;
using Geodesic::OrthodromyTools;
//==============================================================================
//==============================================================================
// Переопределяем опорные функции для классической двумерной точки.
//==============================================================================
template <>
class IntersectorHelper<GeoPoint2D>
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
  double distance( const GeoPoint2D& first, const GeoPoint2D& second )
  {
    return Geodesic::distance( first, second );
  }
//==============================================================================
 /*!
  *\brief Углы наклона отрезка, соединяющего точки
  *\param[in] first - первая точка
  *\param[in] second - вторая точка
  *\param[in] ellipsoid - эллипсоид, на котором следует решать задачу
  *\return пара углов наклона (в полуинтервале [0, 2π) рад)
  *\throws AbstractOperationsFailure : LineThroughOnePoint
  */
  static
  std::pair<Radian, Radian> inclinations(
    const GeoPoint2D& first, const GeoPoint2D& second,
    const Ellipsoid& ellipsoid = EL_WGS_84 )
  {
    if( first == second )
      throw AbstractOperationsFailure( LineThroughOnePoint, "", "Geodesic/inclinations" );

    OrthodromyTools orthodromyTools( ellipsoid );
    auto orthodromy = orthodromyTools.createOrthodromy( first, second );
    return std::make_pair(
      orthodromy.getAzimuthAtStart( ), orthodromy.getAzimuthAtFinish( ) );
  }
//==============================================================================
 /*!
  *\brief Углы наклона отрезка, а также его длина
  *\param[in] first - первая точка
  *\param[in] second - вторая точка
  *\param[in] ellipsoid - эллипсоид, на котором следует решать задачу
  *\return - кортеж, содержащий пару углов наклона [радианы] и длину [м]
  *\throws AbstractOperationsFailure : LineThroughOnePoint
  */
  static
  std::tuple<Radian, Radian, double> inclinationsAndDistance(
    const GeoPoint2D& first, const GeoPoint2D& second,
    const Ellipsoid& ellipsoid = EL_WGS_84 )
  {
    if( first == second )
      throw AbstractOperationsFailure( LineThroughOnePoint, "", "Geodesic/inclinationsAndDistance" );

    OrthodromyTools orthodromyTools( ellipsoid );
    auto orthodromy = orthodromyTools.createOrthodromy( first, second );
    return std::make_tuple( orthodromy.getAzimuthAtStart( ),
      orthodromy.getAzimuthAtFinish( ), orthodromy.getLength( ) );
  }
//==============================================================================
 /*!
  *\brief Найти точку на эллипсоиде, делящую интервал в заданном соотношении
  *\details Найти на AB точку C такую, что portion = |AC| / |AB|
  *\param[in] beg - начало интервала
  *\param[in] end - конец интервала
  *\param[in] portion - заданное соотношение, принадлежит [0, 1]
  *\param[in] ellipsoid - эллипсоид, на котором следует решать задачу
  *\return найденная точка
  *\throws AbstractOperationsFailure : PointNotInInterval
  */
  static
  GeoPoint2D pointByPortion( const GeoPoint2D& beg, const GeoPoint2D& end,
    const double portion,
    const Ellipsoid& ellipsoid = Ellipsoid( EL_WGS_84 ) )
  {
    if( beg == end )
      return beg;

    if( ( portion < 0 ) || ( portion > 1 ) )
      throw AbstractOperationsFailure(
        PointNotInInterval, "", "pointByPortion<GeoPoint2D>" );

    OrthodromyTools orth( ellipsoid );
    auto tot = orth.createOrthodromy( beg, end );
    return tot.pointPosition( portion * tot.getLength( ) );
  }
};
//==============================================================================
} // ! AbstractOperations
//==============================================================================
#endif // ! GEODESIC_OPERATIONS_LINEINTERSECTIONTOOLS_HPP