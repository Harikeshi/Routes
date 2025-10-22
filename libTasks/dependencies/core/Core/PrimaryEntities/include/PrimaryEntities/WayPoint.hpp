#ifndef PRIMARY_ENTITIES_WAY_POINT_HPP
#define PRIMARY_ENTITIES_WAY_POINT_HPP
//==============================================================================
//! Point
#include <Point/GeoPoint2D.hpp>
#include <Point/Point2D.hpp>
//! std
#include <memory>
//==============================================================================
namespace PrimaryEntities
{

/*!
*\brief Параметры полёта для определённого момента времени.
*\details Из всех телеметрируемых при моделировании параметров оставлены те,
*         что отвечают за пространственно-временную траекторию полета.
*/
template <class Point>
struct WayPoint
{
  double t; //!< Время [с]
  Point coord; //!< координаты 2D, без высоты
  double h; //!< высота относительно эллипсоида [м]
//==============================================================================
  WayPoint(
    const double time = 0, 
    const Point& coordinates = Point(), 
    const double height = 0 )
  {
    t = time;
    coord = coordinates;
    h = height;
  }
//==============================================================================
inline bool operator== ( const WayPoint& other ) const
{
  return (
    ( this->t == other.t ) &&
    ( this->coord == other.coord ) &&
    ( this->h == other.h ) );
}
//==============================================================================
};
//==============================================================================
//==============================================================================
// путевая точка в географических координатах
typedef WayPoint<GeoPoint2D> WayGeoPoint;
typedef std::shared_ptr<WayPoint<GeoPoint2D>> WayGeoPointPtr;
typedef std::shared_ptr<const WayPoint<GeoPoint2D>> WayGeoPointCPtr;
//==============================================================================
// путевая точка на плоскости
typedef WayPoint<Point2D> WayFlPoint;
typedef std::shared_ptr<WayPoint<Point2D>> WayFlPointPtr;
typedef std::shared_ptr<const WayPoint<Point2D>> WayFlPointCPtr;
//==============================================================================
} // ! PrimaryEntities

#endif // ! PRIMARY_ENTITIES_WAY_POINT_HPP