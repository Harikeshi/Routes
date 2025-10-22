#include <Cartographic/Alexeev/mercator.hpp>
#include <Cartographic/iellipsoid.hpp>
#include <Cartographic/math.hpp>
//==============================================================================
namespace Cartographic
{
//==============================================================================
Alexeev::Mercator::Mercator(BaseParameters const &base_parameters)
: mP2(sqrt(base_parameters.ellipsoid->E2())),
  mP3(base_parameters.l0)
{
  Units const n(base_parameters.ellipsoid->A()
    / sqrt(1. - base_parameters.ellipsoid->E2()
    * pow(sin(base_parameters.b0 * Rho), 2)));
  mP1 = n * cos(base_parameters.b0 * Rho); // alpha
  mP4 = n * log(tan(45.5 * Rho) / pow(tan
    ((HalfPi + asin(mP2 * sin(Rho))) / 2.), mP2)); // n * ln u
}
//==============================================================================
void Alexeev::Mercator::Forward(Degrees b, Degrees l,
                                Meters &x, Meters &y,
                                Units *, Units *) const
{
  /* Так должно быть:
  x = p1_ * log(tan((90. + b) * Rho / 2.) / pow
    (tan((HalfPi + asin(p2_ * sin(b * Rho))) / 2.), p2_)); */
  x = mP4 * b;
  y = mP1 * (l - mP3) * Rho;
}
//==============================================================================
template <>
IEllipsoid const *GetProjectionEllipsoid<Alexeev::Mercator>(
  Alexeev::Mercator::BaseParameters const &base_parameters)
{
  return base_parameters.ellipsoid.get();
}
//==============================================================================
} // !Cartographic
