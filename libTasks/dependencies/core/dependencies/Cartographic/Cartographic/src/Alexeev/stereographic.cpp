#include <Cartographic/Alexeev/stereographic.hpp>
#include <Cartographic/ellipsoid.hpp>
#include <Cartographic/math.hpp>
//==============================================================================
namespace Cartographic
{
//==============================================================================
Alexeev::Stereographic::Stereographic(BaseParameters const &base_parameters)
: mP1(2. * base_parameters.sphere->A() * pow(
    cos((90. - base_parameters.b0) * Rho / 2.), 2)
  ),
  mP2(base_parameters.l0) {}
//==============================================================================
void Alexeev::Stereographic::Forward(Degrees b, Degrees l,
                             Meters &x, Meters &y,
                             Units *, Units *) const
{
  Units const rho(mP1 * tan((90. - b) * Rho / 2.)), dl((mP2 - l) * Rho);
  x = rho * cos(dl);
  y = rho * sin(dl);
}
//==============================================================================
template <>
IEllipsoid const *GetProjectionEllipsoid<Alexeev::Stereographic>(
  Alexeev::Stereographic::BaseParameters const &base_parameters)
{
  return base_parameters.sphere.get();
}
//==============================================================================
} // !Cartographic
