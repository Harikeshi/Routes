#include <gtest/gtest.h>
// Cartographic
#include <Cartographic/math.hpp>
//==============================================================================
using namespace Cartographic;
//==============================================================================
TEST(MathTest, FuzzyEqTest)
{
  double val1(125.);
  double val2(125.);

  ASSERT_TRUE(FuzzyEq(val1, val2));
  ASSERT_TRUE(FuzzyEq(val2, val1));

  val1 = 168.653 / 12.37;
  val2 = 13.6340339531;

  ASSERT_TRUE(FuzzyEq(val1, val2));
  ASSERT_TRUE(FuzzyEq(val2, val1));

  val1 = 0.000000000000000001;
  val2 = 0.;

  ASSERT_FALSE(FuzzyEq(val1, val2));
  ASSERT_FALSE(FuzzyEq(val2, val1));
}
