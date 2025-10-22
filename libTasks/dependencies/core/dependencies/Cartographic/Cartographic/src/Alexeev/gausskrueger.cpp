#include <Cartographic/Alexeev/arcofmeridian.hpp>
#include <Cartographic/Alexeev/gausskrueger.hpp>
#include <Cartographic/database.hpp>
#include <Cartographic/math.hpp>
//==============================================================================
namespace Cartographic
{
namespace Alexeev
{
//==============================================================================
void GaussKruegerForwardInZone(Degrees b, Degrees l, Meters &x, Meters &y)
{
  b *= Rho;
  l *= Rho;

  Ellipsoid::Krasovsky const earth;

  Units const sb(sin(b));
  Units const cb(cos(b));
  Units const n (earth.a / sqrt(1. - earth.e2 * sb * sb));
  Units const h (earth.e2 * cb * cb / (1. - earth.e2));
  Units const t (sb * sb / cb / cb);
  Units const s (l * l * cb * cb);

  x = ArcOfMeridian::Length(earth, b) + (
    (61. - (330. * t - 270.) * h + (t - 58.) * t) * s * s / 360. +
    (5. - t + (9. + 4. * h) * h) * s / 12. + 1
  ) * n * l * l * sb * cb / 2.;

  y = ((
    ((t - 18.) * t + 5. - (((t - 179.) * t + 479.) * t - 61.) * s / 42. -
    ((64. * h + 58.) * t - (13. * h + 14.)) * h) * s / 20. + h - t + 1.
  ) * s / 6. + 1.) * n * l * cb;
}
//==============================================================================
void GaussKruegerInverseInZone(Meters x, Meters y, Degrees &b, Degrees &l)
{
  Ellipsoid::Krasovsky const earth;

  b = ArcOfMeridian::Angle(earth, x);

  Units const sb(sin(b));
  Units const cb(cos(b));
  Units const n (earth.a / sqrt(1. - earth.e2 * sb * sb));
  Units const h (earth.e2 * cb * cb / (1. - earth.e2));
  Units const t (sb * sb / cb / cb);
  Units const d (y * y / n / n);

  b = (b - d * sb / cb / 2. * (1. + h + (
    (61. + 45. * t * (t + 2.)) * d / 30. - 5. - 3. * t - 6. * h + 6. * h * t
  ) * d / 12.)) / Rho;

  l = (y / n / cb * (1. - (1. + h + 2. * t - d / 20. * (
    5. + 28. * t + 24. * t * t + 6. * h + 8. * h * t
  )) * d / 6.)) / Rho;
}
//==============================================================================
GaussKrueger::GaussKrueger(BaseParameters const &) {}
//==============================================================================
void GaussKrueger::Forward(Degrees b, Degrees l,
                           Meters &x, Meters &y, Units *, Units *) const
{
  if (l < 0.)
    l += 360.;

  Units const zone(ceil(l / 6.));

  l -= 6. * zone - 3.;

  GaussKruegerForwardInZone(b, l, x, y);

  y += (zone + .5) * 1.e6;
}
//==============================================================================
void GaussKrueger::Inverse(Meters x, Meters y,
                           Degrees &b, Degrees &l,
                           Units *, Units *) const
{
  Units const zone(floor(y / 1.e6));

  y -= 1.e6 * (zone + .5);

  GaussKruegerInverseInZone(x, y, b, l);

  l += 6. * zone - 3.;
}
//==============================================================================
GaussKruegerForZone::GaussKruegerForZone(BaseParameters const &baseParameters)
: mZoneNo(baseParameters.zoneNo) {}
//==============================================================================
void GaussKruegerForZone::Forward(Degrees b, Degrees l,
                                  Meters &x, Meters &y,
                                  Units *, Units *) const
{
  if (l < 0.)
    l += 360.;

  l -= 6. * mZoneNo - 3.;

  GaussKruegerForwardInZone(b, l, x, y);

  y += (mZoneNo + .5) * 1.e6;
}
//==============================================================================
void GaussKruegerForZone::Inverse(Meters x, Meters y,
                                  Degrees &b, Degrees &l,
                                  Units *, Units *) const
{
  y -= 1.e6 * (mZoneNo + .5);

  GaussKruegerInverseInZone(x, y, b, l);

  l += 6. * mZoneNo - 3.;
}
//==============================================================================
} // !Alexeev
//==============================================================================
template <>
IEllipsoid const *GetProjectionEllipsoid<Alexeev::GaussKrueger>(
  Alexeev::GaussKrueger::BaseParameters const &)
{
  return new Ellipsoid::Krasovsky;
}
//==============================================================================
template <>
IEllipsoid const *GetProjectionEllipsoid<Alexeev::GaussKruegerForZone>(
  Alexeev::GaussKruegerForZone::BaseParameters const &)
{
  return new Ellipsoid::Krasovsky;
}
//==============================================================================
} // !Cartographic
