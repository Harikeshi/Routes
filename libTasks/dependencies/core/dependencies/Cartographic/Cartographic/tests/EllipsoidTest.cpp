#include <gtest/gtest.h>
//! Cartographic
#include <Cartographic/database.hpp>
//==============================================================================
using namespace Cartographic;
//==============================================================================
constexpr double TEST_EPSILON(.0000001);
//==============================================================================
TEST(EllipsoidTest, UnknownEllTest)
{
  Ellipsoid::Unknown uEl(150, 23);

  ASSERT_EQ(150, uEl.A());
  ASSERT_EQ(-483, uEl.E2());
}
//==============================================================================
TEST(EllipsoidTest, SphereTest)
{
  Sphere sph(743);

  ASSERT_EQ(743, sph.A());
  ASSERT_EQ(0, sph.E2());
}
//==============================================================================
TEST(EllipsoidTest, GeneralGlobeTest)
{
  auto ellipsoid = Ellipsoid::GeneralGlobe();

  ASSERT_NEAR(6378136., ellipsoid.A(), TEST_EPSILON);
  double testE2((2. - 1. / 298.25784) / 298.25784);
  ASSERT_NEAR(testE2, ellipsoid.E2(), TEST_EPSILON);
}
//==============================================================================
TEST(EllipsoidTest, KrasovskyTest)
{
  auto ellipsoid = Ellipsoid::Krasovsky();

  ASSERT_NEAR(6378245., ellipsoid.A(), TEST_EPSILON);
  double testE2((2. - 1. / 298.3) / 298.3);
  ASSERT_NEAR(testE2, ellipsoid.E2(), TEST_EPSILON);
}
//==============================================================================
TEST(EllipsoidTest, WGS_84Test)
{
  auto ellipsoid = Ellipsoid::WGS_84();

  ASSERT_NEAR(6378137., ellipsoid.A(), TEST_EPSILON);
  double testE2((2. - 1. / 298.257223563) / 298.257223563);
  ASSERT_NEAR(testE2, ellipsoid.E2(), TEST_EPSILON);
}
//==============================================================================
