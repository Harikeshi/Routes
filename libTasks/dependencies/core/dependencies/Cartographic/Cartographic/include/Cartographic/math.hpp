#ifndef CARTOGRAPHIC_MATH_HPP
#define CARTOGRAPHIC_MATH_HPP
//==============================================================================
//! std
#include <algorithm>

#define _USE_MATH_DEFINES
#if defined(WIN32) || defined(_WIN32)
#include <math.h> //!< для работы под Windows
#else
#include <cmath>  //!< для работы под Linux
#endif // WIN32
//==============================================================================
namespace Cartographic
{
//==============================================================================
extern double const Pi, HalfPi;
//==============================================================================
//! радиан/градус
extern double const Rho;
//==============================================================================
//! a почти не отличается от b
/* Не работает, если a или b равно 0 */
bool FuzzyEq(double a, double b);
//==============================================================================
} // !Cartographic
#endif //! CARTOGRAPHIC_MATH_HPP