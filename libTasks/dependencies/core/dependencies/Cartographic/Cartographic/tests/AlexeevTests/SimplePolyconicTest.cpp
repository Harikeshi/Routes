#include <gtest/gtest.h>
// std
#include <memory>
// Cartographic
#include <Cartographic/Alexeev/simplepolyconic.hpp>
#include <Cartographic/database.hpp>
//==============================================================================
using namespace Cartographic;
//==============================================================================
typedef Alexeev::SimplePolyconic TestProjection;
//==============================================================================
class SimplePolyconicFoo
{
public:
  SimplePolyconicFoo() :
    baseParameters {std::shared_ptr<const IEllipsoid>(
                      new Ellipsoid::Krasovsky), 0, 0},
    projSPoly(std::shared_ptr<const TestProjection>(
                new TestProjection(baseParameters)))
  {
    std::cout << "CONSTRUCTED" << std::endl;
  }

  ~SimplePolyconicFoo()
  {
    std::cout << "DESTRUCTED" << std::endl;
  }

  TestProjection::BaseParameters const baseParameters;
  std::shared_ptr<const TestProjection> projSPoly;

  Degrees b;
  Degrees l;

  Meters x;
  Meters y;

  Units d1;
  Units d2;
};
//==============================================================================
class TestSimplePolyconicFoo : public ::testing::Test
{
protected:
  void SetUp()
  {
    foo = new SimplePolyconicFoo;

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

  SimplePolyconicFoo *foo;
};
//==============================================================================
TEST_F(TestSimplePolyconicFoo, ForwardTest)
{
  foo->projSPoly->Forward(foo->b, foo->l, foo->x, foo->y);

  EXPECT_EQ  (30.,           foo->b);
  EXPECT_EQ  (45.,           foo->l);
  EXPECT_NEAR(4161813.51642, foo->x, 0.00001);
  EXPECT_NEAR(4231215.58901, foo->y, 0.00001);
}
//==============================================================================
TEST_F(TestSimplePolyconicFoo, ForwardWithDelTest)
{
  foo->projSPoly->Forward(foo->b, foo->l, foo->x, foo->y, &foo->d1, &foo->d2);

  EXPECT_EQ  (30.,           foo->b);
  EXPECT_EQ  (45.,           foo->l);
  EXPECT_NEAR(4161813.51642, foo->x, 0.00001);
  EXPECT_NEAR(4231215.58901, foo->y, 0.00001);
  EXPECT_EQ  (25.045,        foo->d1);
  EXPECT_EQ  (27.142,        foo->d2);
}
//==============================================================================
TEST_F(TestSimplePolyconicFoo, GetProjectionEllipsoidTest)
{
  auto testSp =
      std::shared_ptr<const IEllipsoid>(new Ellipsoid::Krasovsky);
  auto ellipsoid = GetProjectionEllipsoid<TestProjection>(foo->baseParameters);

  EXPECT_EQ  (testSp->A(),  ellipsoid->A());
  EXPECT_EQ  (testSp->E2(), ellipsoid->E2());
  EXPECT_EQ  (6378245,      ellipsoid->A());
  EXPECT_NEAR(0.00669342,   ellipsoid->E2(), 0.00000001);
}
