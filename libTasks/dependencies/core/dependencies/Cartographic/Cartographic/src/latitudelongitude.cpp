#include <Cartographic/latitudelongitude.hpp>
//==============================================================================
namespace Cartographic
{
//==============================================================================
LatitudeLongitude::LatitudeLongitude(BaseParameters const &) {}
//==============================================================================
void LatitudeLongitude::Forward(Degrees b, Degrees l, Degrees &x,
                                Degrees &y, Units *, Units *) const
{
  x = b;
  y = l;
}
//==============================================================================
void LatitudeLongitude::Inverse(Degrees x, Degrees y, Degrees &b,
                                Degrees &l, Units *, Units *) const
{
  b = x;
  l = y;
}
//==============================================================================
template <>
IEllipsoid const *GetProjectionEllipsoid<LatitudeLongitude>(
  LatitudeLongitude::BaseParameters const &base_parameters)
{
  return base_parameters;
}
//==============================================================================
} // !Cartographic
