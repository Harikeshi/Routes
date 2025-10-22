#ifndef CARTOGRAPHIC_ALEXEEV_GEOCENTRIC_HPP
#define CARTOGRAPHIC_ALEXEEV_GEOCENTRIC_HPP
//==============================================================================
#include <Cartographic/igeocentric.hpp>
//==============================================================================
namespace Cartographic
{
namespace Alexeev
{
//==============================================================================
class Geocentric: public IGeocentric
{
public:
  void Forward(IEllipsoid const &ellipsoid,
               Degrees b, Degrees l, Meters h, Meters &x, Meters &y, Meters &z,
               Units *d1 = 0, Units *d2 = 0, Units *d3 = 0) const;

  void Inverse(IEllipsoid const &ellipsoid,
               Meters x, Meters y, Meters z, Degrees &b, Degrees &l, Meters &h,
               Units *d1 = 0, Units *d2 = 0, Units *d3 = 0) const;
};
//==============================================================================
} // !Alexeev
} // !Cartographic
//==============================================================================
#endif //! CARTOGRAPHIC_ALEXEEV_GEOCENTRIC_HPP
