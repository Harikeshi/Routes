#include "Inner/Angle.hpp"
//! std
#define _USE_MATH_DEFINES
#include <cmath>
//==============================================================================
namespace ReachableAreaCalculator
{
//==============================================================================
Angle::Angle(double value)
{
  mValue = fmod(value, 2 * M_PI);
  if (mValue < 0.)
    mValue += 2 * M_PI;
}
//==============================================================================
Angle::Angle(const Angle& angle)
  : mValue(angle.value())
{ }
//==============================================================================
double Angle::value() const
{
  return mValue;
}
//==============================================================================
double Angle::deltaAngle(const Angle& angle) const
{
  double rotateAngle = angle.mValue - mValue;

  while (rotateAngle <= -M_PI)
    rotateAngle += double(2 * M_PI);

  while (rotateAngle > M_PI)
    rotateAngle -= double(2 * M_PI);

  return rotateAngle;
}
//==============================================================================
double Angle::deltaAngleCounterclockwise(const Angle& angle) const
{
    if (angle.mValue >= mValue)
        return angle.mValue - mValue;

    return double(angle.mValue + 2 * M_PI - mValue);
}
//==============================================================================
} //! ReachableAreaCalculator
