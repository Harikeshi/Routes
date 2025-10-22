#include <gtest/gtest.h>
// std
#include <memory>
// Cartographic
#include <Cartographic/Alexeev/gausskrueger.hpp>
#include <Cartographic/database.hpp>
//==============================================================================
using namespace Cartographic;
//==============================================================================
typedef Alexeev::GaussKrueger TestProjection;
typedef Alexeev::GaussKruegerForZone TestProjectionForZone;
//==============================================================================
class GaussKruegerFoo
{
public:
  GaussKruegerFoo() :
    zoneNo(8),
    baseParametersGK {},
    baseParametersGKforZone {zoneNo},
    projGK(new TestProjection(baseParametersGK)),
    projGKforZone(new TestProjectionForZone(baseParametersGKforZone))
  {
    std::cout << "CONSTRUCTED" << std::endl;
  }

  ~GaussKruegerFoo()
  {
    delete projGK;
    delete projGKforZone;
    std::cout << "DESTRUCTED" << std::endl;
  }

private:
  Units zoneNo;

public:
  TestProjection::BaseParameters        const baseParametersGK;
  TestProjectionForZone::BaseParameters const baseParametersGKforZone;
  TestProjection        *projGK;
  TestProjectionForZone *projGKforZone;

  Degrees b;
  Degrees l;

  Meters x;
  Meters y;

  Units d1;
  Units d2;
};
//==============================================================================
class TestGaussKruegerFoo : public ::testing::Test
{
protected:
  void SetUp()
  {
    foo = new GaussKruegerFoo;

    foo->b  = 30.;
    foo->l  = 45.;
    foo->x  = 0;
    foo->y  = 0;
    foo->d1 = 25.045;
    foo->d2 = 27.142;
  }

  void TearDown()
  {
    delete foo;
  }

  GaussKruegerFoo *foo;
};
//==============================================================================
TEST_F(TestGaussKruegerFoo, GaussKruegerForwardTest)
{
  foo->projGK->Forward(foo->b, foo->l, foo->x, foo->y);

  EXPECT_EQ  (30.,           foo->b);
  EXPECT_EQ  (45.,           foo->l);
  EXPECT_NEAR(3320172.40672, foo->x, 0.00001);
  EXPECT_EQ  (8500000.,      foo->y);
}
//==============================================================================
TEST_F(TestGaussKruegerFoo, GaussKruegerForwardWithDelTest)
{
  foo->projGK->Forward(foo->b, foo->l, foo->x, foo->y, &foo->d1, &foo->d2);

  EXPECT_EQ  (30.,           foo->b);
  EXPECT_EQ  (45.,           foo->l);
  EXPECT_NEAR(3320172.40672, foo->x, 0.00001);
  EXPECT_EQ  (8500000.,      foo->y);
  EXPECT_EQ  (25.045,        foo->d1);
  EXPECT_EQ  (27.142,        foo->d2);
}
//==============================================================================
TEST_F(TestGaussKruegerFoo, GaussKruegerInverseTest)
{
  foo->projGK->Inverse(foo->x, foo->y, foo->b, foo->l);

  EXPECT_EQ  ( 0.,      foo->x);
  EXPECT_EQ  ( 0.,      foo->y);
  EXPECT_EQ  ( 0.,      foo->b);
  EXPECT_NEAR(-7.48688, foo->l, 0.00001);
}
//==============================================================================
TEST_F(TestGaussKruegerFoo, GaussKruegerInverseWithDelTest)
{
  foo->projGK->Inverse(foo->x, foo->y, foo->b, foo->l, &foo->d1, &foo->d2);

  EXPECT_EQ  ( 0.,      foo->x);
  EXPECT_EQ  ( 0.,      foo->y);
  EXPECT_EQ  ( 0.,      foo->b);
  EXPECT_NEAR(-7.48688, foo->l, 0.00001);
  EXPECT_EQ  ( 25.045,  foo->d1);
  EXPECT_EQ  ( 27.142,  foo->d2);
}
//==============================================================================
TEST_F(TestGaussKruegerFoo, GaussKruegerForZoneTest)
{
  EXPECT_EQ  (8, foo->baseParametersGKforZone.zoneNo);
}
//==============================================================================
TEST_F(TestGaussKruegerFoo, GaussKruegerForZoneForwardTest)
{
  foo->projGKforZone->Forward(foo->b, foo->l, foo->x, foo->y);

  EXPECT_EQ  (30.,           foo->b);
  EXPECT_EQ  (45.,           foo->l);
  EXPECT_NEAR(3320172.40672, foo->x, 0.00001);
  EXPECT_EQ  (8500000.,      foo->y);
}
//==============================================================================
TEST_F(TestGaussKruegerFoo, GaussKruegerForZoneForwardWithDelTest)
{
  foo->projGKforZone->Forward(foo->b, foo->l, foo->x, foo->y,
                              &foo->d1, &foo->d2);

  EXPECT_EQ  (30.,           foo->b);
  EXPECT_EQ  (45.,           foo->l);
  EXPECT_NEAR(3320172.40672, foo->x, 0.00001);
  EXPECT_EQ  (8500000.,      foo->y);
  EXPECT_EQ  (25.045,        foo->d1);
  EXPECT_EQ  (27.142,        foo->d2);
}
//==============================================================================
TEST_F(TestGaussKruegerFoo, GaussKruegerForZoneInverseTest)
{
  foo->projGKforZone->Inverse(foo->x, foo->y, foo->b, foo->l);

  EXPECT_EQ  ( 0.,       foo->x);
  EXPECT_EQ  ( 0.,       foo->y);
  EXPECT_EQ  ( 0.,       foo->b);
  EXPECT_NEAR(-18.71811, foo->l, 0.00001);
}
//==============================================================================
TEST_F(TestGaussKruegerFoo, GaussKruegerForZoneInverseWithDelTest)
{
  foo->projGKforZone->Inverse(foo->x, foo->y, foo->b, foo->l,
                              &foo->d1, &foo->d2);

  EXPECT_EQ  ( 0.,       foo->x);
  EXPECT_EQ  ( 0.,       foo->y);
  EXPECT_EQ  ( 0.,       foo->b);
  EXPECT_NEAR(-18.71811, foo->l, 0.00001);
  EXPECT_EQ  ( 25.045,   foo->d1);
  EXPECT_EQ  ( 27.142,   foo->d2);
}
//==============================================================================
TEST_F(TestGaussKruegerFoo, GetProjectionEllipsoidTest1)
{
  auto ellipsoid =
      std::shared_ptr<const IEllipsoid>(
        GetProjectionEllipsoid<TestProjection>(foo->baseParametersGK));

  EXPECT_DOUBLE_EQ(6378245.0,  ellipsoid->A());
  EXPECT_NEAR     (0.00669342, ellipsoid->E2(), 0.00000001);
}
//==============================================================================
TEST_F(TestGaussKruegerFoo, GetProjectionEllipsoidTest2)
{
  auto ellipsoid =
      std::shared_ptr<const IEllipsoid>(
      GetProjectionEllipsoid<TestProjectionForZone>(
        foo->baseParametersGKforZone));

  EXPECT_DOUBLE_EQ(6378245.0,  ellipsoid->A());
  EXPECT_NEAR     (0.00669342, ellipsoid->E2(), 0.00000001);
}
//==============================================================================
TEST_F(TestGaussKruegerFoo, GetProjectionEllipsoidTest3)
{
  TestProjectionForZone::BaseParameters const bP {34};

  auto ellipsoid =
      std::shared_ptr<const IEllipsoid>(
      GetProjectionEllipsoid<TestProjectionForZone>(bP));

  EXPECT_DOUBLE_EQ(6378245.0,  ellipsoid->A());
  EXPECT_NEAR     (0.00669342, ellipsoid->E2(), 0.00000001);
}
