#include <Geodesic/GeoLineTools/Helper/Loxodrome.hpp>
//! GeographicLib
#include <GeographicLib/Rhumb.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
GeoPoint2D Loxodrome::
pointPosition(double distance) const noexcept
{
  //! Координаты точки в градусах
  double lat(0.);
  double lon(0.);

  //! Вычисление позиции  точки 
  mLine->Position(distance, lat, lon);
  
  //! Формирование точки
  GeoPoint2D result = GeoPoint2D(Degree(lat), Degree(lon));
  result.normalize(REGULAR);

  return result;
}
//==============================================================================
Loxodrome::Loxodrome()
: Inner::GeodesicLine()
, mLine(nullptr)
{
}
//==============================================================================
} //! Geodesic
//==============================================================================
