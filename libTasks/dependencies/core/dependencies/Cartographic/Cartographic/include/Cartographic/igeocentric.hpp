#ifndef CARTOGRAPHIC_I_GEOCENTRIC_HPP
#define CARTOGRAPHIC_I_GEOCENTRIC_HPP
//==============================================================================
#include <Cartographic/units.hpp>
//==============================================================================
namespace Cartographic
{
//==============================================================================
class IEllipsoid;
//==============================================================================
class IForwardGeocentric
{
public:
  virtual ~IForwardGeocentric() {}

  virtual void Forward(IEllipsoid const &ellipsoid,
                       Degrees b, Degrees l,
                       Meters h, Meters &x, Meters &y, Meters &z,
                       Units *d1 = 0, Units *d2 = 0, Units *d3 = 0) const = 0;
};
//==============================================================================
class IInverseGeocentric
{
public:
  virtual ~IInverseGeocentric() {}

  virtual void Inverse(IEllipsoid const &ellipsoid,
                       Meters x, Meters y, Meters z,
                       Degrees &b, Degrees &l, Meters &h,
                       Units *d1 = 0, Units *d2 = 0, Units *d3 = 0) const = 0;
};
//==============================================================================
class IGeocentric: public IForwardGeocentric,
    public IInverseGeocentric {};
//==============================================================================
} // !Cartographic
//==============================================================================
#endif //! CARTOGRAPHIC_I_PROJECTION_HPP
