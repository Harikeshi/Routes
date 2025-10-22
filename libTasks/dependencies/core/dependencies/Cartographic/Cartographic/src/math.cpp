#include "Cartographic/math.hpp"
//==============================================================================
namespace Cartographic
{
//==============================================================================
  double const Pi = M_PI;
  double const HalfPi(Pi / 2.);
  double const Rho(Pi / 180.);
//==============================================================================
  bool FuzzyEq(double a, double b)
  {
    return fabs(a - b) <= 1.e-12 * std::min(fabs(a), fabs(b));
  }
//==============================================================================
}//! Cartographic
