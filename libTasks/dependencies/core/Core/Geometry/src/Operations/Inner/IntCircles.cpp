#include "IntCircles.hpp"
//==============================================================================
namespace Geometry
{
//==============================================================================
void IntCircle::setRadius(unsigned int value) noexcept
{
  mIntRadius = value;
  mRadius = static_cast<double>(value);
}
//==============================================================================
void IntCircle::setRadius(double value) noexcept
{
  long intValue(static_cast<long>(value));
  mIntRadius = (intValue > 0) ? static_cast<unsigned int>(intValue) : 0U;
  mRadius = static_cast<double>(mIntRadius);
}
//==============================================================================
} //! Geometry
//==============================================================================