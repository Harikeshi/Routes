#include <MathTools/BasicMath/Range.hpp>
//! BasicMath
#include <MathTools/BasicMath/Compare.hpp>
//! Base constants
#include <MathTools/Constants.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace BasicMath
{
namespace Tests
{
//==============================================================================
  /*!
  * \brief Тестирование toRange
  *        Приведение значения к требуемому диапазону
  */
  TEST( RangeTest, NumberToRange )
{
  //! Если min < max
  ASSERT_EQ( 3, toRange( 3, 0, 5 ) );
  ASSERT_EQ( 0, toRange( -1, 0, 5 ) );
  ASSERT_EQ( 5., toRange( 7., 0., 5. ) );

  //! Если min > max
  ASSERT_EQ( 5, toRange( 3, 5, 0 ) );
  ASSERT_EQ( 5, toRange( -1, 5, 0 ) );
  ASSERT_EQ( 0., toRange( 7., 5., 0. ) );
}
//==============================================================================
/*!
* \brief Тестирование isInRange
*        Проверка попадания значения в диапазон
*/
TEST( RangeTest, NumberInRange )
{
  ASSERT_TRUE( isInRange( 3, 0, 5 ) );
  ASSERT_FALSE( isInRange( -1, 0, 5 ) );
  ASSERT_FALSE( isInRange( 7., 0., 5. ) );
}
//==============================================================================
/*!
* \brief Тестирование normalize()
*/
TEST( RangeTest, Normalize )
{
  using namespace BasicMath;

  // значение, лежащее в допустимом диапазоне, не меняется
  ASSERT_TRUE( isEqual( M_PI / 4, normalize( M_PI / 4 ) ) );
  ASSERT_TRUE( isEqual( -M_PI / 4, normalize( -M_PI / 4 ) ) );

  // скручиваются лишние периоды
  ASSERT_TRUE( isEqual( M_PI / 4, normalize( 2 * M_PI + M_PI / 4 ) ) );

  // добавляются недостающие периоды
  ASSERT_TRUE( isEqual( M_PI / 4, normalize( -4 * M_PI + M_PI / 4 ) ) );

  // округление в сторону от 0 работает при необходимости
  ASSERT_TRUE( isEqual( -M_PI / 2, normalize( 3 * M_PI / 2 ) ) );
  ASSERT_TRUE( isEqual( M_PI / 2, normalize( -3 * M_PI / 2 ) ) );

  // период можно задавать свой
  ASSERT_TRUE( isEqual( 45, normalize( 6 * WIDE_ANGLE + 45, WIDE_ANGLE ) ) );
}
//==============================================================================
/*!
* \brief Тестирование normalizeWithShift()
*/
TEST( RangeTest, NormalizeWithShift )
{
  using namespace BasicMath;

  // значение, лежащее в допустимом диапазоне, не меняется
  ASSERT_TRUE( isEqual( M_PI / 4, normalizeWithShift( M_PI / 4 ) ) );
  // отрицательные значения становятся недопустимыми
  ASSERT_TRUE( isEqual( 7 * M_PI / 4, normalizeWithShift( -M_PI / 4 ) ) );

  // скручиваются лишние периоды
  ASSERT_TRUE( isEqual( M_PI / 4, normalizeWithShift( 2 * M_PI + M_PI / 4 ) ) );

  // добавляются недостающие периоды
  ASSERT_TRUE( isEqual( M_PI / 4, normalizeWithShift( -4 * M_PI + M_PI / 4 ) ) );

  // период можно задавать свой
  ASSERT_TRUE( isEqual( 45, normalizeWithShift(
    6 * WIDE_ANGLE + 45, WIDE_ANGLE ) ) );
}
//==============================================================================
} //! Tests
} //! BasicMath
//==============================================================================