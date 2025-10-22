#include <Cartographic/Alexeev/arcofmeridian.hpp>
#include <Cartographic/Alexeev/utm.hpp>
#include <Cartographic/database.hpp>
#include <Cartographic/math.hpp>
//! std
#include <algorithm>
//==============================================================================
namespace Cartographic
{
//==============================================================================
Alexeev::UTM::UTM(BaseParameters const &)
{}
//==============================================================================
void Alexeev::UTM::Forward(Degrees b, Degrees l,
                             Meters &x, Meters &y,
                             Units *, Units *) const
{
  b *= Rho;
  l += 180.;
  Units const zone(ceil(l / 6.));
  l -= 6. * zone - 3.;
  l *= Rho;
  Ellipsoid::WGS_84 const earth;
  Units const sb(sin(b)), cb(cos(b)),
    n(earth.a / sqrt(1. - earth.e2 * sb * sb)),
    h(earth.e2 * cb * cb / (1. - earth.e2)),
    t(sb * sb / cb / cb), s(l * l * cb * cb);
  x = (ArcOfMeridian::Length(earth, b) + ((61. - (330. * t - 270.) * h
    + (t - 58.) * t) * s * s / 360. + (5. - t + (9. + 4. * h) * h) * s / 12.
    + 1) * n * l * l * sb * cb / 2.) * .9996;
  y = ((((t - 18.) * t + 5. - (((t - 179.) * t + 479.) * t - 61.) * s / 42.
    - ((64. * h + 58.) * t - (13. * h + 14.)) * h) * s / 20. + h - t + 1.)
    * s / 6. + 1.) * n * l * cb * .9996 + (zone + .5) * 1.e6;
}
//==============================================================================
void Alexeev::UTM::Inverse(Meters x, Meters y,
                             Degrees &b, Degrees &l,
                             Units *, Units *) const
{
  Units const zone(floor(y / 1.e6));
  y -= 1.e6 * (zone + .5);
  x /= .9996;
  y /= .9996;
  Ellipsoid::WGS_84 const earth;
  b = ArcOfMeridian::Angle(earth, x);
  Units const sb(sin(b)), cb(cos(b)), t(sb * sb / cb / cb),
    h(earth.e2 * cb * cb / (1. - earth.e2)),
    n(earth.a / sqrt(1. - earth.e2 * sb * sb)), d(y * y / n / n);
  l = (y / n / cb * (1. - (1. + h + 2. * t - d / 20.
    * (5. + 28. * t + 24. * t * t + 6. * h + 8. * h * t)) * d / 6.))
    / Rho + 6. * zone - 183.;
  b = (b - d * sb / cb / 2. * (1. + h + ((61. + 45. * t * (t + 2.))
    * d / 30. - 5. - 3. * t - 6. * h + 6. * h * t) * d / 12.)) / Rho;
}
//==============================================================================
template <>
IEllipsoid const *GetProjectionEllipsoid<Alexeev::UTM>(
  Alexeev::UTM::BaseParameters const &)
{
  return new Ellipsoid::WGS_84;
}
//==============================================================================
} // !Cartographic
