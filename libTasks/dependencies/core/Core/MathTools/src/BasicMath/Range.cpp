#include <MathTools/BasicMath/Range.hpp>
//! std
#include <algorithm>
//==============================================================================
namespace BasicMath
{
//==============================================================================
double normalize( const double& value, const double& halfPeriod /*= M_PI*/ )
{
  if( value >= halfPeriod )
    return value 
      - ( floor( value / ( 2 * halfPeriod ) + .5 ) ) * 2 * halfPeriod ;
  if( value < -halfPeriod )
    return value 
      - ( ceil( value / ( 2 * halfPeriod ) - .5 ) ) * 2 * halfPeriod;
  return value;
}
//==============================================================================
double normalizeWithShift( 
  const double& value, const double& period /*= 2*M_PI*/ )
{
  double retVal = normalize( value, period / 2. );

  if( retVal < 0 )
    retVal += period;

  return retVal;
}
//==============================================================================
} //! BasicMath
//==============================================================================
