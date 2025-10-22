#ifndef PRIMARY_ENTITIES_CIRCLE_LITE_HPP
#define PRIMARY_ENTITIES_CIRCLE_LITE_HPP
//==============================================================================
//! Point
#include <Point/Point2D.hpp>
#include <Point/GeoPoint2D.hpp>
//! BasicMath
#include <MathTools/BasicMath/Compare.hpp>
//==============================================================================
namespace PrimaryEntities
{
//==============================================================================
/*!
 *\brief Легкая структура для хранения окружности
 */
template <class Point>
struct CircleLite
{
  Point  center; //! Центр окружности
  double radius; //! Радиус окружности
//==============================================================================
  /*!
   *\brief Конструктор с параметрами
   *\param[in] cent - центр окружности (точка типа Point)
   *\param[in] r - радиус окружности [м]
   */
  CircleLite( const Point& cent = Point(), const double r = 1 )
    : center( cent )
    , radius( r )
  {}
//==============================================================================
};
//==============================================================================
/*!
 *\brief Оператор сравнения
 */
template <class PointOne, class PointOther>
bool operator== ( 
  const CircleLite<PointOne>& one, const CircleLite<PointOther>& other )
{
  return ( ( one.center == other.center )
    && BasicMath::isEqual( one.radius, other.radius ) );
}
//==============================================================================
//! Окружность на плоскости
typedef CircleLite<Point2D> CircleLite2D;

//! Окружность на эллипсоиде
typedef CircleLite<GeoPoint2D> GeoCircleLite2D;
//==============================================================================
} //! PrimaryEntities
//==============================================================================
#endif //! PRIMARY_ENTITIES_CIRCLE_LITE_HPP
