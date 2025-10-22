#include <MathTools/BasicMath/SignOperations.hpp>
//! AngularUnits
#include <MathTools/AngularUnits/Degree.hpp>
#include <MathTools/AngularUnits/Radian.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование определения положительное значение или отрицательное, где
  *       0 относится к положительным
*/
TEST( SignOperationsTest, Signum )
{
  using namespace BasicMath;

  //! float
  ASSERT_EQ( -1, signum( -.5f ) );
  ASSERT_EQ( 1, signum( .5f ) );
  ASSERT_EQ( 1, signum( .0f ) );

  //! double
  ASSERT_EQ( -1, signum( -.5 ) );
  ASSERT_EQ( 1, signum( .5 ) );
  ASSERT_EQ( 1, signum( .0 ) );

  //! градусы
  ASSERT_EQ( -1, signum( Degree( -.5 ) ) );
  ASSERT_EQ( 1, signum( Degree( .5 ) ) );
  ASSERT_EQ( 1, signum( Degree( .0 ) ) );

  //! радианы
  ASSERT_EQ( -1, signum( Radian( -.5 ) ) );
  ASSERT_EQ( 1, signum( Radian( .5 ) ) );
  ASSERT_EQ( 1, signum( Radian( .0 ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование нахождения знака значения (математическая классика), где
  *       0 - это 0
*/
TEST( SignOperationsTest, Sign )
{
  using namespace BasicMath;

  //! float
  ASSERT_EQ( -1, sign( -.5f ) );
  ASSERT_EQ( 1, sign( .5f ) );
  ASSERT_EQ( 0, sign( .0f ) );

  //! double
  ASSERT_EQ( -1, sign( -.5 ) );
  ASSERT_EQ( 1, sign( .5 ) );
  ASSERT_EQ( 0, sign( .0 ) );

  //! градусы
  ASSERT_EQ( -1, sign( Degree( -.5 ) ) );
  ASSERT_EQ( 1, sign( Degree( .5 ) ) );
  ASSERT_EQ( 0, sign( Degree( .0 ) ) );

  //! радианы
  ASSERT_EQ( -1, sign( Radian( -.5 ) ) );
  ASSERT_EQ( 1, sign( Radian( .5 ) ) );
  ASSERT_EQ( 0, sign( Radian( .0 ) ) );
}
//==============================================================================
} //! Tests
//==============================================================================

