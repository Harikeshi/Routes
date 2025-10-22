#include <gtest/gtest.h>
// Cartographic
#include <Cartographic/Alexeev/alexeev.hpp>
#include <Cartographic/database.hpp>
#include <Cartographic/latitudelongitude.hpp>
//==============================================================================
using namespace Cartographic;
//==============================================================================
typedef LatitudeLongitude TestProjection;
//==============================================================================
class LatLonFoo
{
public:
  LatLonFoo() :
    baseParametersLatLon {},
    projLatLon(new TestProjection(baseParametersLatLon))
  {
    std::cout << "CONSTRUCTED" << std::endl;
  }

  ~LatLonFoo()
  {
    delete projLatLon;
    std::cout << "DESTRUCTED" << std::endl;
  }

  TestProjection::BaseParameters const baseParametersLatLon;
  TestProjection *projLatLon;

  Degrees b;
  Degrees l;

  Degrees x;
  Degrees y;

  Units d1;
  Units d2;
};
//==============================================================================
class TestingLatLonFoo : public ::testing::Test
{
protected:
  void SetUp()
  {
    foo = new LatLonFoo;

    foo->b  = 0.70711;
    foo->l  = 0.86603;
    foo->x  = 0.52;
    foo->y  = 0.11;
    foo->d1 = 2.;
    foo->d2 = 0.2;
  }

  void TearDown()
  {
    delete foo;
  }

  LatLonFoo *foo;
};
//==============================================================================
TEST_F(TestingLatLonFoo, ForwardTest1)
{
  foo->projLatLon->Forward(foo->x, foo->y, foo->b, foo->l);

  EXPECT_EQ(0.52,   foo->b );
  EXPECT_EQ(0.11,   foo->l );
  EXPECT_EQ(foo->b, foo->x );
  EXPECT_EQ(foo->l, foo->y );
  EXPECT_EQ(2.0,    foo->d1);
  EXPECT_EQ(0.2,    foo->d2);
}
//==============================================================================
TEST_F(TestingLatLonFoo, ForwardTest2)
{
  foo->projLatLon->Forward(foo->b, foo->l, foo->x, foo->y);

  EXPECT_EQ(0.70711, foo->b );
  EXPECT_EQ(0.86603, foo->l );
  EXPECT_EQ(foo->b,  foo->x );
  EXPECT_EQ(foo->l,  foo->y );
  EXPECT_EQ(2.0,     foo->d1);
  EXPECT_EQ(0.2,     foo->d2);
}
//==============================================================================
TEST_F(TestingLatLonFoo, ForwardWithDelTest)
{
  foo->projLatLon->Forward(foo->b, foo->l, foo->x, foo->y, &foo->d1, &foo->d2);

  EXPECT_EQ(0.70711, foo->b );
  EXPECT_EQ(0.86603, foo->l );
  EXPECT_EQ(foo->b,  foo->x );
  EXPECT_EQ(foo->l,  foo->y );
  EXPECT_EQ(2.0,     foo->d1);
  EXPECT_EQ(0.2,     foo->d2);
}
//==============================================================================
TEST_F(TestingLatLonFoo, InverseTest1)
{
  foo->projLatLon->Inverse(foo->x, foo->y, foo->b, foo->l, &foo->d1, &foo->d2);

  EXPECT_EQ(0.52,   foo->b );
  EXPECT_EQ(0.11,   foo->l );
  EXPECT_EQ(foo->b, foo->x );
  EXPECT_EQ(foo->l, foo->y );
  EXPECT_EQ(2.0,    foo->d1);
  EXPECT_EQ(0.2,    foo->d2);
}
//==============================================================================
TEST_F(TestingLatLonFoo, InverseTest2)
{
  foo->projLatLon->Forward(foo->b, foo->l, foo->x, foo->y, &foo->d1, &foo->d2);

  EXPECT_EQ(0.70711, foo->b );
  EXPECT_EQ(0.86603, foo->l );
  EXPECT_EQ(foo->b,  foo->x );
  EXPECT_EQ(foo->l,  foo->y );
  EXPECT_EQ(2.0,     foo->d1);
  EXPECT_EQ(0.2,     foo->d2);
}
//==============================================================================
TEST_F(TestingLatLonFoo, InverseWithDelTest)
{
  foo->projLatLon->Forward(foo->x, foo->y, foo->b, foo->l, &foo->d1, &foo->d2);

  EXPECT_EQ(0.52,   foo->b );
  EXPECT_EQ(0.11,   foo->l );
  EXPECT_EQ(foo->b, foo->x );
  EXPECT_EQ(foo->l, foo->y );
  EXPECT_EQ(2.0,    foo->d1);
  EXPECT_EQ(0.2,    foo->d2);
}
//==============================================================================
TEST_F(TestingLatLonFoo, GetProjectionEllipsoidTest)
{
  TestProjection::BaseParameters const bP =
      GetProjectionEllipsoid<TestProjection>(foo->baseParametersLatLon);

  ASSERT_EQ(LatitudeLongitude::BaseParameters(), bP);
}
