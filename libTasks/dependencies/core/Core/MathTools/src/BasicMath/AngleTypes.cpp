#include <MathTools/BasicMath/AngleTypes.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//==============================================================================
namespace BasicMath
{
//==============================================================================
AngleType checkAndCorrectAngle( Radian& angle )
{
  if( BasicMath::isNull( angle ) )
  {
    angle = Radian( 0 );
    return ZeroAngle;
  }

  Radian pi( M_PI );
  if( BasicMath::isEqual( angle, pi ) || BasicMath::isEqual( angle, -pi ) )
  {
    angle = pi;
    return StraightAngle;
  }

  return ArbitraryAngle;
}
//==============================================================================
} //! BasicMath
//==============================================================================
