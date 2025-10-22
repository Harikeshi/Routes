#include <MathTools/BasicMath/AngleTypes.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace BasicMath
{
namespace Tests
{
//==============================================================================
// определить тип угла
TEST( BorderLinesIntersectionTest_, checkAngleTest )
{
  Radian angle( ACCURACY_RADIAN / 10 );
  ASSERT_EQ( checkAndCorrectAngle( angle ), ZeroAngle );
  ASSERT_EQ( angle, Radian( ) );

  angle = Radian( M_PI / 2 );
  ASSERT_EQ( checkAndCorrectAngle( angle ), ArbitraryAngle );
  angle = Radian( -M_PI / 2 );
  ASSERT_EQ( checkAndCorrectAngle( angle ), ArbitraryAngle );

  angle = Radian( M_PI - ACCURACY_RADIAN / 10 );
  ASSERT_EQ( checkAndCorrectAngle( angle ), StraightAngle );
  ASSERT_NEAR( angle.get( ), M_PI, ACCURACY_DOUBLE );
  angle = Radian( -M_PI + ACCURACY_RADIAN / 10 );
  ASSERT_EQ( checkAndCorrectAngle( angle ), StraightAngle );
  ASSERT_NEAR( angle.get( ), M_PI, ACCURACY_DOUBLE );
}
//==============================================================================
} //! Tests
} //! BasicMath
//==============================================================================