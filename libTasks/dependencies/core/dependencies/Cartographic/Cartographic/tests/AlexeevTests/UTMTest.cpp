#include <gtest/gtest.h>
// std
#include <memory>
// Cartographic
#include <Cartographic/Alexeev/utm.hpp>
#include <Cartographic/database.hpp>
//==============================================================================
using namespace Cartographic;
//==============================================================================
typedef Alexeev::UTM TestProjection;
//==============================================================================
class UTMFoo
{
public:
  UTMFoo() :
    baseParameters {},
    projUTM(new TestProjection(baseParameters))
  {
    std::cout << "CONSTRUCTED" << std::endl;
  }

  ~UTMFoo()
  {
    delete projUTM;
    std::cout << "DESTRUCTED" << std::endl;
  }

public:
  TestProjection::BaseParameters const baseParameters;
  TestProjection *projUTM;

  Degrees b;
  Degrees l;

  Meters x;
  Meters y;

  Units d1;
  Units d2;
};
//==============================================================================
class TestUTMFoo : public ::testing::Test
{
protected:
  void SetUp()
  {
    foo = new UTMFoo;

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

  UTMFoo *foo;
};
//==============================================================================
TEST_F(TestUTMFoo, ForwardTest)
{
  foo->projUTM->Forward(foo->b, foo->l, foo->x, foo->y);

  EXPECT_EQ  (30.,           foo->b);
  EXPECT_EQ  (45.,           foo->l);
  EXPECT_NEAR(3318785.35258, foo->x, 0.00001);
  EXPECT_EQ  (38500000.,     foo->y);
}
//==============================================================================
TEST_F(TestUTMFoo, ForwardWithDelTest)
{
  foo->projUTM->Forward(foo->b, foo->l, foo->x, foo->y, &foo->d1, &foo->d2);

  EXPECT_EQ  (30.,           foo->b);
  EXPECT_EQ  (45.,           foo->l);
  EXPECT_NEAR(3318785.35258, foo->x, 0.00001);
  EXPECT_EQ  (38500000.,     foo->y);
  EXPECT_EQ  (25.045,        foo->d1);
  EXPECT_EQ  (27.142,        foo->d2);
}
//==============================================================================
TEST_F(TestUTMFoo, InverseTest)
{
  foo->projUTM->Inverse(foo->x, foo->y, foo->b, foo->l);

  EXPECT_EQ  ( 0.,        foo->x);
  EXPECT_EQ  ( 0.,        foo->y);
  EXPECT_EQ  ( 0.,        foo->b);
  EXPECT_NEAR(-187.48874, foo->l, 0.00001);
}
//==============================================================================
TEST_F(TestUTMFoo, InverseWithDelTest)
{
  foo->projUTM->Inverse(foo->x, foo->y, foo->b, foo->l, &foo->d1, &foo->d2);

  EXPECT_EQ  ( 0.,        foo->x);
  EXPECT_EQ  ( 0.,        foo->y);
  EXPECT_EQ  ( 0.,        foo->b);
  EXPECT_NEAR(-187.48874, foo->l, 0.00001);
  EXPECT_EQ  ( 25.045,    foo->d1);
  EXPECT_EQ  ( 27.142,    foo->d2);
}
//==============================================================================
TEST_F(TestUTMFoo, GetProjectionEllipsoidTest1)
{
  auto ellipsoid = std::shared_ptr<const IEllipsoid>(
      GetProjectionEllipsoid<TestProjection>(foo->baseParameters));

  EXPECT_DOUBLE_EQ(6378137.0,  ellipsoid->A());
  EXPECT_NEAR     (0.00669437, ellipsoid->E2(), 0.00000001);
}
//==============================================================================
TEST_F(TestUTMFoo, GetProjectionEllipsoidTest2)
{
  auto test = std::shared_ptr<const IEllipsoid>(new Ellipsoid::WGS_84);
  auto ellipsoid = std::shared_ptr<const IEllipsoid>(
      GetProjectionEllipsoid<TestProjection>(foo->baseParameters));

  EXPECT_EQ(test->A(),  ellipsoid->A());
  EXPECT_EQ(test->E2(), ellipsoid->E2());
}
