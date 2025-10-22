#include <HelperFunctions.hpp>

#include <Point/Constants.hpp>
//! MathTools
#include <MathTools/AngularUnits/Degree.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Test
{
//==============================================================================
/*!
 * \brief Тестирование isCorrectLatitude
 *        (проверка попадания широты в диапазон допустимых значений)
 */
TEST( HelperFunctionsTest, IsCorrectLatitude )
{
  ASSERT_TRUE( isCorrectLatitude( Radian( 0. ) ) );
  ASSERT_TRUE( isCorrectLatitude( Radian( M_PI / 4 ) ) );
  ASSERT_TRUE( isCorrectLatitude( Radian( M_PI / 2 ) ) );
  ASSERT_TRUE( isCorrectLatitude( Radian( -M_PI / 2 ) ) );
  ASSERT_FALSE( isCorrectLatitude( Radian( M_PI ) ) );
  ASSERT_FALSE( isCorrectLatitude( Radian( -M_PI ) ) );
  ASSERT_TRUE( isCorrectLatitude( Radian( Degree( 89.9999999999 ) ) ) );
  ASSERT_FALSE( isCorrectLatitude( Radian( Degree( 90.0000000001 ) ) ) );
  ASSERT_FALSE( isCorrectLatitude( Radian( Degree( 1024. ) ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование isCorrectLongitude
 *        (проверка попадания долготы в диапазон допустимых значений)
 */
TEST( HelperFunctionsTest, IsCorrectLongitude )
{
  ASSERT_TRUE( isCorrectLongitude( Radian( 0. ), REGULAR ) );
  ASSERT_TRUE( isCorrectLongitude( Radian( 0. ), NORMAL ) );
  ASSERT_TRUE( isCorrectLongitude( Radian( M_PI / 2 ), REGULAR ) );
  ASSERT_TRUE( isCorrectLongitude( Radian( M_PI / 2 ), NORMAL ) );
  ASSERT_TRUE( isCorrectLongitude( Radian( -M_PI / 2 ), REGULAR ) );
  ASSERT_FALSE( isCorrectLongitude( Radian( -M_PI / 2 ), NORMAL ) );
  ASSERT_FALSE( isCorrectLongitude( Radian( M_PI ), REGULAR ) );
  ASSERT_TRUE( isCorrectLongitude( Radian( M_PI ), NORMAL ) );
  ASSERT_TRUE( isCorrectLongitude( Radian( -M_PI ), REGULAR ) );
  ASSERT_FALSE( isCorrectLongitude( Radian( -M_PI ), NORMAL ) );
  ASSERT_FALSE( isCorrectLongitude( Radian( M_PI * 2 ), REGULAR ) );
  ASSERT_FALSE( isCorrectLongitude( Radian( M_PI * 2 ), NORMAL ) );
  ASSERT_FALSE( isCorrectLongitude( Radian( -M_PI * 2 ), REGULAR ) );
  ASSERT_FALSE( isCorrectLongitude( Radian( -M_PI * 2 ), NORMAL ) );
  ASSERT_FALSE( isCorrectLongitude( Radian( M_PI * 4 ), REGULAR ) );
  ASSERT_FALSE( isCorrectLongitude( Radian( M_PI * 4 ), NORMAL ) );
  ASSERT_FALSE( isCorrectLongitude( Radian( -M_PI * 4 ), REGULAR ) );
  ASSERT_FALSE( isCorrectLongitude( Radian( -M_PI * 4 ), NORMAL ) );
  ASSERT_TRUE( isCorrectLongitude(
                                Radian( Degree( 359.9999999999 ) ), NORMAL ) );
  ASSERT_FALSE( isCorrectLongitude(
                                Radian( Degree( 360.0000000001 ) ), NORMAL ) );
}
//==============================================================================
} //! Tests
//==============================================================================

