#include <Operations/Inner/IntCircles.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
class IntCirclePrivate : public IntCircle
{
  //==============================================================================
  FRIEND_TEST( IntCircleTest, Constructor );
  //==============================================================================
public:
  //==============================================================================
  inline
    IntCirclePrivate(
      const Point2D& center,
      unsigned int radius ) noexcept : IntCircle( center, radius ) {}
  //==============================================================================
};
//==============================================================================
class IntCircleTest : public ::testing::Test
{
  //==============================================================================
protected:
  //==============================================================================
  void SetUp( )
  {
  }
  //==============================================================================
  void TearDown( )
  {
  }
  //==============================================================================
};
//==============================================================================
/*!
 * \brief Тестирование конструктора.
 */
TEST_F( IntCircleTest, Constructor )
{
  Point2D          expectedCenter( 4., 4. );
  unsigned int     expectedRadius( 234U );
  IntCirclePrivate intCircle( expectedCenter, expectedRadius );
  ASSERT_EQ( expectedCenter, intCircle.mCenter );
  ASSERT_EQ( 234., intCircle.mRadius );
  ASSERT_EQ( expectedRadius, intCircle.mIntRadius );
}
//==============================================================================
/*!
 * \brief Тестирование геттеров.
 */
TEST_F( IntCircleTest, Getters )
{
  Point2D      expectedCenter( 12., 81. );
  unsigned int expectedRadius( 791U );
  IntCircle    intCircle( expectedCenter, expectedRadius );
  ASSERT_EQ( expectedCenter, intCircle.center( ) );
  ASSERT_EQ( 791., intCircle.radius( ) );
  ASSERT_EQ( expectedRadius, intCircle.intRadius( ) );
}
//==============================================================================
/*!
 * \brief Тестирование сеттеров.
 */
TEST_F( IntCircleTest, Setters )
{
  Point2D   expectedCenter( 56.7255, 86.7831 );
  IntCircle intCircle( Point2D( 0., 0. ), 0U );
  intCircle.setCenter( expectedCenter );
  ASSERT_EQ( expectedCenter, intCircle.center( ) );

  unsigned int expectedRadius( 660U );
  intCircle.setRadius( expectedRadius );
  ASSERT_EQ( expectedRadius, intCircle.intRadius( ) );
  ASSERT_EQ( 660., intCircle.radius( ) );

  double realExpectedRadius( 826.250557 );
  intCircle.setRadius( realExpectedRadius );
  ASSERT_EQ( 826U, intCircle.intRadius( ) );
  ASSERT_EQ( 826., intCircle.radius( ) );

  realExpectedRadius = -447.312134;
  intCircle.setRadius( realExpectedRadius );
  ASSERT_EQ( 0U, intCircle.intRadius( ) );
  ASSERT_EQ( 0., intCircle.radius( ) );
}
//==============================================================================
} //! Tests
} //! Geometry
//==============================================================================