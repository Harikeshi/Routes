#include <gtest/gtest.h>
// std
#include <memory>
// Cartographic
#include <Cartographic/Alexeev/azimuthalequidistant.hpp>
#include <Cartographic/database.hpp>
//==============================================================================
using namespace Cartographic;
//==============================================================================
typedef Alexeev::AzimuthalEquidistant TestProjection;
//==============================================================================
class AzimuthalEquidistantFoo
{
public:
  AzimuthalEquidistantFoo() :
    baseParameters {std::shared_ptr<const Sphere>(new Sphere(6371000)), 0, 0},
    projAzEq(std::shared_ptr<TestProjection>(
               new TestProjection(baseParameters)))
  {
    std::cout << "CONSTRUCTED" << std::endl;
  }

  ~AzimuthalEquidistantFoo()
  {
    std::cout << "DESTRUCTED" << std::endl;
  }

  TestProjection::BaseParameters const baseParameters;
  std::shared_ptr<TestProjection> projAzEq;

  Degrees b;
  Degrees l;

  Meters x;
  Meters y;

  Units d1;
  Units d2;
};
//==============================================================================
class TestAzimuthalEquidistantFoo : public ::testing::Test
{
protected:
  void SetUp()
  {
    foo = new AzimuthalEquidistantFoo;

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

  AzimuthalEquidistantFoo *foo;
};
//==============================================================================
TEST_F(TestAzimuthalEquidistantFoo, ForwardTest)
{
  foo->projAzEq->Forward(foo->b, foo->l, foo->x, foo->y);

  EXPECT_EQ  ( 30.,       foo->b);
  EXPECT_EQ  ( 45.,       foo->l);
  EXPECT_NEAR( 3003318.20195, foo->x, 0.00001);
  EXPECT_NEAR(-3003318.20195, foo->y, 0.00001);
}
//==============================================================================
TEST_F(TestAzimuthalEquidistantFoo, ForwardWithDelTest)
{
  foo->projAzEq->Forward(foo->b, foo->l, foo->x, foo->y, &foo->d1, &foo->d2);

  EXPECT_EQ  ( 30.,           foo->b);
  EXPECT_EQ  ( 45.,           foo->l);
  EXPECT_NEAR( 3003318.20195, foo->x, 0.00001);
  EXPECT_NEAR(-3003318.20195, foo->y, 0.00001);
}
//==============================================================================
TEST_F(TestAzimuthalEquidistantFoo, GetProjectionEllipsoidTest)
{
  auto testSp = std::shared_ptr<const IEllipsoid>(new Sphere(6371000));
  auto ellipsoid = GetProjectionEllipsoid<TestProjection>(foo->baseParameters);

  EXPECT_EQ(testSp->A(),  ellipsoid->A());
  EXPECT_EQ(testSp->E2(), ellipsoid->E2());
  EXPECT_EQ(6371000,      ellipsoid->A());
  EXPECT_EQ(0,            ellipsoid->E2());
}
