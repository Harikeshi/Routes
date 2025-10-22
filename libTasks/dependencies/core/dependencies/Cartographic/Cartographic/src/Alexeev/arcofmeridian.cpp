#include <Cartographic/Alexeev/arcofmeridian.hpp>
#include <Cartographic/iellipsoid.hpp>
#include <Cartographic/math.hpp>
//==============================================================================
namespace Cartographic
{
namespace Alexeev
{
namespace ArcOfMeridian
{
//==============================================================================
Meters Length(Meters a_, Units e2_, Radians b, Units *pa, Units *pc)
{
  double const
    av[4] = { 0.67291259765625, 0.68359375, 0.703125, 0.75 },
    sv[4][4] = {
      { -.538330078125,    -.5126953125,     -.46875,    -.375 },
      {  .13458251953125,   .1025390625,      .05859375,  .0   },
      { -.025634765625,    -.01139322911667,  .0,         .0   },
      {  .002403259277344,  .0,               .0,         .0   } };
  double a(0.), c(0.);
  for(int i(0); i < 4; ++i)
  {
    a = (a + av[i]) * e2_;
    double s(0.);
    for(int j(0); j < 4; ++j)
      s = (s + sv[i][j]) * e2_;
    c += s * sin(2. * (i + 1) * b) * (1. - e2_) * a_;
  }
  a = (1. + a) * (1. - e2_) * a_;
  if(pa)
    *pa = a;
  if(pc)
    *pc = c;
  return a * b + c;
}
//==============================================================================
Meters Length(IEllipsoid const &ellipsoid, Radians b, Units *pa, Units *pc)
{
  return Length(ellipsoid.A(), ellipsoid.E2(), b, pa, pc);
}
//==============================================================================
Radians Angle(IEllipsoid const &ellipsoid, Meters x)
{
  double b(0.), b1, a, c;
  do
  {
    b1 = b;
    Length(ellipsoid, b, &a, &c);
    b = (x - c) / a;
  }
  while(!FuzzyEq(b, b1));
  return b;
}
//==============================================================================
} // !ArcOfMeridian
} // !Alexeev
} // !Cartographic
