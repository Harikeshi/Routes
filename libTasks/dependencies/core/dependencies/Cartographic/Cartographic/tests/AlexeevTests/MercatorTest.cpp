#include <gtest/gtest.h>
// std
#include <memory>
// Cartographic
#include <Cartographic/Alexeev/mercator.hpp>
#include <Cartographic/database.hpp>
//==============================================================================
using namespace Cartographic;
//==============================================================================
typedef Alexeev::Mercator TestProjection;
//==============================================================================
class MercatorFoo
{
public:
  MercatorFoo() :
    baseParameters {std::shared_ptr<const IEllipsoid>(
                      new Ellipsoid::Krasovsky), 0, 0},
    projMerc(std::shared_ptr<TestProjection>(
               new TestProjection(baseParameters)))
  {
    std::cout << "CONSTRUCTED" << std::endl;
  }

  ~MercatorFoo()
  {
    std::cout << "DESTRUCTED" << std::endl;
  }

  TestProjection::BaseParameters const baseParameters;
  std::shared_ptr<TestProjection> projMerc;

  Degrees b;
  Degrees l;

  Meters x;
  Meters y;

  Units d1;
  Units d2;
};
//==============================================================================
class TestMercatorFoo : public ::testing::Test
{
protected:
  void SetUp()
  {
    foo = new MercatorFoo;

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

  MercatorFoo *foo;
};
//==============================================================================
TEST_F(TestMercatorFoo, ForwardTest)
{
  foo->projMerc->Forward(foo->b, foo->l, foo->x, foo->y);

  EXPECT_EQ  (30.,           foo->b);
  EXPECT_EQ  (45.,           foo->l);
  EXPECT_NEAR(3317458.33011, foo->x, 0.00001);
  EXPECT_NEAR(5009461.90869, foo->y, 0.00001);
}
//==============================================================================
TEST_F(TestMercatorFoo, ForwardWithDelTest)
{
  foo->projMerc->Forward(foo->b, foo->l, foo->x, foo->y, &foo->d1, &foo->d2);

  EXPECT_EQ  (30.,           foo->b);
  EXPECT_EQ  (45.,           foo->l);
  EXPECT_NEAR(3317458.33011, foo->x, 0.00001);
  EXPECT_NEAR(5009461.90869, foo->y, 0.00001);
  EXPECT_EQ  (25.045,        foo->d1);
  EXPECT_EQ  (27.142,        foo->d2);
}
//==============================================================================
TEST_F(TestMercatorFoo, GetProjectionEllipsoidTest)
{
  auto testSp = std::shared_ptr<const IEllipsoid>(new Ellipsoid::Krasovsky);
  auto ellipsoid = GetProjectionEllipsoid<TestProjection>(foo->baseParameters);

  EXPECT_EQ  (testSp->A(),  ellipsoid->A());
  EXPECT_EQ  (testSp->E2(), ellipsoid->E2());
  EXPECT_EQ  (6378245,      ellipsoid->A());
  EXPECT_NEAR(0.00669342,   ellipsoid->E2(), 0.00000001);
}
