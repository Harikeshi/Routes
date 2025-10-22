#include <Cartographic/Alexeev/azimuthalequidistant.hpp>
#include <Cartographic/ellipsoid.hpp>
#include <Cartographic/math.hpp>
//==============================================================================
namespace Cartographic
{
//==============================================================================
Alexeev::AzimuthalEquidistant::AzimuthalEquidistant(
                                          BaseParameters const &base_parameters)
: mP1(base_parameters.sphere->A() * (
    FuzzyEq(base_parameters.b0, 90.)?
      Rho:
      cos(base_parameters.b0 * Rho) / (90. - base_parameters.b0)
  )),
  mP2(base_parameters.l0) {}
//==============================================================================
void Alexeev::AzimuthalEquidistant::Forward(Degrees b, Degrees l,
                                            Meters &x, Meters &y, 
                                            Units *, Units *) const
{
  Units const rho(mP1 * (90. - b)), dl((mP2 - l) * Rho);
  x = rho * cos(dl);
  y = rho * sin(dl);
}
//==============================================================================
template <>
IEllipsoid const *GetProjectionEllipsoid<Alexeev::AzimuthalEquidistant>(
  Alexeev::AzimuthalEquidistant::BaseParameters const &base_parameters)
{
  return base_parameters.sphere.get();
}
//==============================================================================
} // !Cartographic
