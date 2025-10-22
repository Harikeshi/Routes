#include <MathTools/AngularUnits/Degree.hpp>
//! MathTools
#include "ConvertFunctions.hpp"
#include <MathTools/AngularUnits/Radian.hpp>
#include <MathTools/BasicMath/Compare.hpp>
#include <MathTools/BasicMath/Range.hpp>
//==============================================================================
Degree::Degree(const double degrees /*= 0.*/)
: mValue(degrees)
{
}
//==============================================================================
Degree::Degree(const Radian& radians)
: mValue(fromRadiansToDegrees(radians.get()))
{
}
//==============================================================================
Radian Degree::toRadians() const
{
  return Radian(fromDegreesToRadians(mValue));
}
//==============================================================================
void Degree::fromRadians(const Radian& radians)
{
  mValue = fromRadiansToDegrees(radians.get());
}
//==============================================================================
std::string 
Degree::toDegreesString(const GeoCoordType geoCordType, 
                        const unsigned int digitDepth /*= 2*/,
                        const std::string& decimalDelimiter /*= "."*/)
const
{
  return fromDegreesToDegreeString(mValue, geoCordType, digitDepth, 
                                   decimalDelimiter);
}
//==============================================================================
bool Degree::fromDegreesString(const std::string& degreeString, 
                               bool needCheck /*= false*/)
{
  return fromDegreesStringToDegrees(degreeString, mValue, needCheck);
}
//==============================================================================
Degree Degree::normalize( ) const
{
  return Degree( BasicMath::normalize( mValue, WIDE_ANGLE ) );
}
//==============================================================================
Degree Degree::normalizeWithShift( ) const
{
  return Degree( BasicMath::normalizeWithShift( mValue, PERIGON ) );
}
//==============================================================================
Degree& Degree::operator+=(const Degree& degree) noexcept
{
  mValue += degree.get();
  return *this;
}
//==============================================================================
Degree& Degree::operator-=(const Degree& degree) noexcept
{
  mValue -= degree.get();
  return *this;
}
//==============================================================================
Degree& Degree::operator*=(const Degree& degree) noexcept
{
  mValue *= degree.get();
  return *this;
}
//==============================================================================
Degree& Degree::operator /= (const Degree& degree)
{
  if (!BasicMath::isNull(degree))
  {
    mValue /= degree.get();
    return *this;
  }
  else
    throw MathFailure(MathEnumFailure::DegreeDivideByZero);
}
//==============================================================================
Degree Degree::operator / (const Degree& degree) const
{
  if (!BasicMath::isNull(degree))
    return Degree(mValue / degree.get());
  else
    throw MathFailure(MathEnumFailure::DegreeDivideByZero);
}
//==============================================================================