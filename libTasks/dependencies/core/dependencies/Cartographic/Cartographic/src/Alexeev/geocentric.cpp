#include <Cartographic/Alexeev/geocentric.hpp>
#include <Cartographic/iellipsoid.hpp>
#include <Cartographic/math.hpp>
//==============================================================================
namespace Cartographic
{
namespace Alexeev
{
//==============================================================================
void Geocentric::Forward(IEllipsoid const &ellipsoid,
                         Degrees b, Degrees l,
                         Meters h, Meters &x, Meters &y, Meters &z,
                         Units *, Units *, Units *) const
{
  b *= Rho;
  l *= Rho;
  double const sb(sin(b)), cb(cos(b)),
    n(ellipsoid.A() / sqrt(1. - ellipsoid.E2() * sb * sb)), hn(h + n);
  x = hn * cb * cos(l);
  y = hn * cb * sin(l);
  z = (hn - n * ellipsoid.E2()) * sb;
}
//==============================================================================
void Geocentric::Inverse(IEllipsoid const &ellipsoid,
                         Meters x, Meters y, Meters z,
                         Degrees &b, Degrees &l, Meters &h,
                         Units *, Units *, Units *) const
{
  double const d(sqrt(x * x + y * y));
  bool const equat(FuzzyEq(z + 1., 1.));
  if (FuzzyEq(d + 1., 1.))
  {
    // Точка на оси
    b = equat? 0.: z > 0.? 90.: -90.;
    double const sn(b / 90.); // sin(b)
    l = 0.;
    h = z * sn - ellipsoid.A() * sqrt(1. - ellipsoid.E2() * sn * sn);
    return;
  }
  else
    l = atan2(y, x) / Rho;
  if (equat)
  {
    // Точка в плоскости экватора
    b = 0.;
    h = d - ellipsoid.A();
    return;
  }
  double const r(sqrt(d * d + z * z));
  double s1, s2(0.), sb, cb;
  do
  {
    s1 = s2;
    b = s1 + asin(z / r);
    sb = sin(b);
    cb = cos(b);
    s2 = asin(ellipsoid.E2() * ellipsoid.A() * sb * cb / r
      / sqrt(1. - ellipsoid.E2() * sb * sb));
  }
  while (!FuzzyEq(s1, s2));
  h = d * cb + z * sb - ellipsoid.A() * sqrt(1. - ellipsoid.E2() * sb * sb);
  b /= Rho;
}
//==============================================================================
} // !Alexeev
} // !Cartographic
