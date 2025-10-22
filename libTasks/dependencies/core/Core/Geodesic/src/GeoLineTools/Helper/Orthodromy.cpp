#include <Geodesic/GeoLineTools/Helper/Orthodromy.hpp>
//! GeographicLib
#include <GeographicLib/GeodesicLine.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
GeoPoint2D Orthodromy::
pointPosition(double distance) const noexcept
{
  double lat(0.); //!< Широта искомой точки (в градусах).
  double lon(0.); //!< Долгота искомой точки (в градусах).

  mLine->Position(distance, lat, lon);

  //! Формирование точки
  GeoPoint2D result = GeoPoint2D(Degree(lat), Degree(lon));
  result.normalize(REGULAR);

  return result;
}
//==============================================================================
GeoPoint2D Orthodromy::
pointPosition(double distance, Radian& newAzimuth) const noexcept
{
  double lat(0.); //!< Широта искомой точки (в градусах).
  double lon(0.); //!< Долгота искомой точки (в градусах).
  //!< Азимут направления ортодромии в искомой точке (в градусах).
  double azimuth(0.);

  mLine->Position(distance, lat, lon, azimuth);

  //! Формирование точки
  GeoPoint2D result = GeoPoint2D(Degree(lat), Degree(lon));
  result.normalize(REGULAR);

  //! Устанавливаем азимут направления ортодромии в искомой точке.
  newAzimuth = Degree(azimuth).toRadians();

  return result;
}
//==============================================================================
Orthodromy::Orthodromy()
: Inner::GeodesicLine()
, mLine(nullptr)
{
}
//==============================================================================
} //! Geodesic
//==============================================================================
