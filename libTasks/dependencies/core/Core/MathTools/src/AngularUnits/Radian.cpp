#include <MathTools/AngularUnits/Radian.hpp>
//! MathTools
#include "ConvertFunctions.hpp"
#include <MathTools/AngularUnits/Degree.hpp>
#include <MathTools/BasicMath/Compare.hpp>
#include <MathTools/BasicMath/Range.hpp>
#include <MathTools/MathException.hpp>
//==============================================================================
Radian::Radian(const double radians /*= 0.*/)
: mValue(radians)
{
}
//==============================================================================
Radian::Radian(const Degree& degrees)
: mValue(fromDegreesToRadians(degrees.get()))
{
}
//==============================================================================
Degree Radian::toDegrees() const
{
  return Degree(fromRadiansToDegrees(mValue));
}
//==============================================================================
void Radian::fromDegrees(const Degree& deg)
{
  mValue = fromDegreesToRadians(deg.get());
}
//==============================================================================
std::string 
Radian::toDegreesString(const GeoCoordType geoCordType, 
                        const unsigned int digitDepth /*= 2*/, 
                        const std::string& decimalDelimiter /*= "."*/) 
const
{
  return fromRadiansToDegreeString(mValue, geoCordType, digitDepth,
                                   decimalDelimiter);
}
//==============================================================================
bool Radian::fromDegreesString(const std::string& degreeString, 
                               bool needCheck /*= false*/)
{
  return fromDegreesStringToRadians(degreeString, mValue, needCheck);
}
//==============================================================================
Radian Radian::normalize() const
{
  return Radian ( BasicMath::normalize ( mValue ) );
}
//==============================================================================
Radian Radian::normalizeWithShift( ) const
{
  return Radian( BasicMath::normalizeWithShift( mValue ) );
}
//==============================================================================
Radian& Radian::operator+=(const Radian& radian) noexcept
{
  mValue += radian.get();
  return *this;
}
//==============================================================================
Radian& Radian::operator-=(const Radian& radian) noexcept
{
  mValue -= radian.get();
  return *this;
}
//==============================================================================
Radian& Radian::operator*=(const Radian& radian) noexcept
{
  mValue *= radian.get();
  return *this;
}
//==============================================================================
Radian& Radian::operator /= ( const Radian& radian )
{
  if (!BasicMath::isNull(radian))
  {
    mValue /= radian.get();
    return *this;
  }
  else
    throw MathFailure(MathEnumFailure::RadianDivideByZero);
}
//==============================================================================
Radian Radian::operator / ( const Radian& radian ) const
{
  if( !BasicMath::isNull( radian ) )
    return Radian( mValue / radian.get( ) );
  else
    throw MathFailure(MathEnumFailure::RadianDivideByZero);
}
//==============================================================================
