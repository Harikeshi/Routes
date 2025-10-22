#include <MathTools/BasicMath/LengthOperations.hpp>
//! BasicMath
#include <MathTools/BasicMath/Compare.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование нахождения усреднённой длины сегмента равномерного
 *        разбиения общей длины
*/
TEST( LengthOperationsTest, AvgSegmentLength )
{
  using namespace BasicMath;

  double segmentLength;
  int numSegments;

  //! Разбивать надо
  ASSERT_TRUE( avgSegmentLength( 5., 1., segmentLength, numSegments ) );
  ASSERT_TRUE( isEqual( 1., segmentLength ) );
  ASSERT_EQ( 5, numSegments );

  ASSERT_TRUE( avgSegmentLength( 5., 3., segmentLength, numSegments ) );
  ASSERT_TRUE( isEqual( 2.5, segmentLength ) );
  ASSERT_EQ( 2, numSegments );

  //! Уже разбито
  ASSERT_FALSE( avgSegmentLength( 5., 5., segmentLength, numSegments ) );
  ASSERT_TRUE( isEqual( 5., segmentLength ) );
  ASSERT_EQ( 1, numSegments );

  //! Разбивать не надо
  ASSERT_FALSE( avgSegmentLength( -5., 1., segmentLength, numSegments ) );
  ASSERT_TRUE( isEqual( -5., segmentLength ) );
  ASSERT_EQ( 1, numSegments );

  ASSERT_FALSE( avgSegmentLength( -5., -1., segmentLength, numSegments ) );
  ASSERT_TRUE( isEqual( -5., segmentLength ) );
  ASSERT_EQ( 1, numSegments );

  ASSERT_FALSE( avgSegmentLength( 5., -1., segmentLength, numSegments ) );
  ASSERT_TRUE( isEqual( 5., segmentLength ) );
  ASSERT_EQ( 1, numSegments );

  ASSERT_FALSE( avgSegmentLength( 0., -1., segmentLength, numSegments ) );
  ASSERT_TRUE( isEqual( 0., segmentLength ) );
  ASSERT_EQ( 1, numSegments );

  ASSERT_FALSE( avgSegmentLength( 5., 0., segmentLength, numSegments ) );
  ASSERT_TRUE( isEqual( 5., segmentLength ) );
  ASSERT_EQ( 1, numSegments );
}
//==============================================================================
} //! Tests
//==============================================================================
