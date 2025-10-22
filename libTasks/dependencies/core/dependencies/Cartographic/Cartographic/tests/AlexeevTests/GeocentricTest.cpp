#include <gtest/gtest.h>
// Cartographic
#include <Cartographic/Alexeev/geocentric.hpp>
#include <Cartographic/database.hpp>
//==============================================================================
using namespace Cartographic;
//==============================================================================
typedef Alexeev::Geocentric TestProjection;
//==============================================================================
class GeocentricFoo
{
public:
  GeocentricFoo() :
    projGeocentric(new TestProjection()),
    ellipsoidKras(),
    ellipsoidWGS(),
    ellipsoidUnkn(6478000, 0.0033528),
    sphere(6371000)
  {
    std::cout << "CONSTRUCTED" << std::endl;
  }

  ~GeocentricFoo()
  {
    delete projGeocentric;
    std::cout << "DESTRUCTED" << std::endl;
  }

  TestProjection *projGeocentric;

  Ellipsoid::Krasovsky const ellipsoidKras; // Эллипсоид Красовского
  Ellipsoid::WGS_84    const ellipsoidWGS;  // Эллипсоид WGS84
  Ellipsoid::Unknown   const ellipsoidUnkn; // Неизвестный эллипсоид
  Sphere               const sphere;        // Сфера

  Degrees b;
  Degrees l;

  Meters h;

  Meters x;
  Meters y;
  Meters z;

  Units d1;
  Units d2;
  Units d3;
};
//==============================================================================
class TestGeocentricFoo : public ::testing::Test
{
protected:
  void SetUp()
  {
    foo = new GeocentricFoo;

    foo->b  = 30.;
    foo->l  = 45.;
    foo->h  = 150.;
    foo->x  = 3609220.;
    foo->y  = 3508340.;
    foo->z  = 3055500.;
    foo->d1 = 25.045;
    foo->d2 = 27.142;
    foo->d3 = 18.203;
  }

  void TearDown()
  {
    delete foo;
  }

  GeocentricFoo *foo;
};
//==============================================================================
TEST_F(TestGeocentricFoo, ForwardForKrasovskyTest)
{
  foo->projGeocentric->Forward(foo->ellipsoidKras,
                               foo->b, foo->l, foo->h, foo->x, foo->y, foo->z);

  EXPECT_EQ       (30.,           foo->b);
  EXPECT_EQ       (45.,           foo->l);
  EXPECT_EQ       (150.,          foo->h);
  EXPECT_DOUBLE_EQ(foo->x,        foo->y);
  EXPECT_NEAR     (3909225.33620, foo->x, 0.0001);
  EXPECT_NEAR     (3909225.33620, foo->y, 0.0001);
  EXPECT_NEAR     (3170505.09729, foo->z, 0.0001);
}
//==============================================================================
TEST_F(TestGeocentricFoo, ForwardForKrasovskyWithDelTest)
{
  foo->projGeocentric->Forward(foo->ellipsoidKras,
                               foo->b, foo->l, foo->h, foo->x, foo->y, foo->z,
                               &foo->d1, &foo->d2, &foo->d3);

  EXPECT_EQ       (30.,           foo->b);
  EXPECT_EQ       (45.,           foo->l);
  EXPECT_EQ       (150.,          foo->h);
  EXPECT_DOUBLE_EQ(foo->x,        foo->y);
  EXPECT_NEAR     (3909225.33620, foo->x, 0.0001);
  EXPECT_NEAR     (3909225.33620, foo->y, 0.0001);
  EXPECT_NEAR     (3170505.09729, foo->z, 0.0001);
  EXPECT_EQ       (25.045,        foo->d1);
  EXPECT_EQ       (27.142,        foo->d2);
  EXPECT_EQ       (18.203,        foo->d3);
}
//==============================================================================
TEST_F(TestGeocentricFoo, ForwardForWGSTest)
{
  foo->projGeocentric->Forward(foo->ellipsoidWGS,
                               foo->b, foo->l, foo->h, foo->x, foo->y, foo->z);

  EXPECT_EQ       (30.,           foo->b);
  EXPECT_EQ       (45.,           foo->l);
  EXPECT_EQ       (150.,          foo->h);
  EXPECT_DOUBLE_EQ(foo->x,        foo->y);
  EXPECT_NEAR     (3909159.61364, foo->x, 0.0001);
  EXPECT_NEAR     (3909159.61364, foo->y, 0.0001);
  EXPECT_NEAR     (3170448.73538, foo->z, 0.0001);
}
//==============================================================================
TEST_F(TestGeocentricFoo, ForwardForWGSWithDelTest)
{
  foo->projGeocentric->Forward(foo->ellipsoidWGS,
                               foo->b, foo->l, foo->h, foo->x, foo->y, foo->z,
                               &foo->d1, &foo->d2, &foo->d3);

  EXPECT_EQ       (30.,           foo->b);
  EXPECT_EQ       (45.,           foo->l);
  EXPECT_EQ       (150.,          foo->h);
  EXPECT_DOUBLE_EQ(foo->x,        foo->y);
  EXPECT_NEAR     (3909159.61364, foo->x, 0.0001);
  EXPECT_NEAR     (3909159.61364, foo->y, 0.0001);
  EXPECT_NEAR     (3170448.73538, foo->z, 0.0001);
  EXPECT_EQ       (25.045,        foo->d1);
  EXPECT_EQ       (27.142,        foo->d2);
  EXPECT_EQ       (18.203,        foo->d3);
}
//==============================================================================
TEST_F(TestGeocentricFoo, ForwardForUnknTest)
{
  foo->projGeocentric->Forward(foo->ellipsoidUnkn,
                               foo->b, foo->l, foo->h, foo->x, foo->y, foo->z);

  EXPECT_EQ       (30.,           foo->b);
  EXPECT_EQ       (45.,           foo->l);
  EXPECT_EQ       (150.,          foo->h);
  EXPECT_DOUBLE_EQ(foo->x,        foo->y);
  EXPECT_NEAR     (3970364.18891, foo->x, 0.0001);
  EXPECT_NEAR     (3970364.18891, foo->y, 0.0001);
  EXPECT_NEAR     (3220087.59030, foo->z, 0.0001);
}
//==============================================================================
TEST_F(TestGeocentricFoo, ForwardForUnknWithDelTest)
{
  foo->projGeocentric->Forward(foo->ellipsoidUnkn,
                               foo->b, foo->l, foo->h, foo->x, foo->y, foo->z,
                               &foo->d1, &foo->d2, &foo->d3);

  EXPECT_EQ       (30.,           foo->b);
  EXPECT_EQ       (45.,           foo->l);
  EXPECT_EQ       (150.,          foo->h);
  EXPECT_DOUBLE_EQ(foo->x,        foo->y);
  EXPECT_NEAR     (3970364.18891, foo->x, 0.0001);
  EXPECT_NEAR     (3970364.18891, foo->y, 0.0001);
  EXPECT_NEAR     (3220087.59030, foo->z, 0.0001);
  EXPECT_EQ       (25.045,        foo->d1);
  EXPECT_EQ       (27.142,        foo->d2);
  EXPECT_EQ       (18.203,        foo->d3);
}
//==============================================================================
TEST_F(TestGeocentricFoo, ForwardForSphereTest)
{
  foo->projGeocentric->Forward(foo->sphere,
                               foo->b, foo->l, foo->h, foo->x, foo->y, foo->z);

  EXPECT_EQ       (30.,           foo->b);
  EXPECT_EQ       (45.,           foo->l);
  EXPECT_EQ       (150.,          foo->h);
  EXPECT_DOUBLE_EQ(foo->x,        foo->y);
  EXPECT_NEAR     (3901516.64368, foo->x, 0.0001);
  EXPECT_NEAR     (3901516.64368, foo->y, 0.0001);
  EXPECT_NEAR     (3185574.99999, foo->z, 0.0001);
}
//==============================================================================
TEST_F(TestGeocentricFoo, ForwardForSphereWithDelTest)
{
  foo->projGeocentric->Forward(foo->sphere,
                               foo->b, foo->l, foo->h, foo->x, foo->y, foo->z,
                               &foo->d1, &foo->d2, &foo->d3);

  EXPECT_EQ       (30.,           foo->b);
  EXPECT_EQ       (45.,           foo->l);
  EXPECT_EQ       (150.,          foo->h);
  EXPECT_DOUBLE_EQ(foo->x,        foo->y);
  EXPECT_NEAR     (3901516.64368, foo->x, 0.0001);
  EXPECT_NEAR     (3901516.64368, foo->y, 0.0001);
  EXPECT_NEAR     (3185574.99999, foo->z, 0.0001);
  EXPECT_EQ       (25.045,        foo->d1);
  EXPECT_EQ       (27.142,        foo->d2);
  EXPECT_EQ       (18.203,        foo->d3);
}
//==============================================================================
TEST_F(TestGeocentricFoo, InverseForKrasovskyTest)
{
  foo->projGeocentric->Inverse(foo->ellipsoidKras,
                               foo->x, foo->y, foo->z, foo->b, foo->l, foo->h);

  EXPECT_NEAR( 31.4448,  foo->b, 0.0001);
  EXPECT_NEAR( 44.188,   foo->l, 0.001);
  EXPECT_NEAR(-484255.3, foo->h, 0.1);
  EXPECT_EQ  ( 3609220., foo->x);
  EXPECT_EQ  ( 3508340., foo->y);
  EXPECT_EQ  ( 3055500., foo->z);
}
//==============================================================================
TEST_F(TestGeocentricFoo, InverseForKrasovskyWithDelTest)
{
  foo->projGeocentric->Inverse(foo->ellipsoidKras,
                               foo->x, foo->y, foo->z, foo->b, foo->l, foo->h,
                               &foo->d1, &foo->d2, &foo->d3);

  EXPECT_NEAR( 31.4448,  foo->b, 0.0001);
  EXPECT_NEAR( 44.188,   foo->l, 0.001);
  EXPECT_NEAR(-484255.3, foo->h, 0.1);
  EXPECT_EQ  ( 3609220., foo->x);
  EXPECT_EQ  ( 3508340., foo->y);
  EXPECT_EQ  ( 3055500., foo->z);
  EXPECT_EQ  (25.045,    foo->d1);
  EXPECT_EQ  (27.142,    foo->d2);
  EXPECT_EQ  (18.203,    foo->d3);
}
//==============================================================================
TEST_F(TestGeocentricFoo, InverseForWGSTest)
{
  foo->projGeocentric->Inverse(foo->ellipsoidWGS,
                               foo->x, foo->y, foo->z, foo->b, foo->l, foo->h);

  EXPECT_NEAR( 31.4448,  foo->b, 0.0001);
  EXPECT_NEAR( 44.188,   foo->l, 0.001);
  EXPECT_NEAR(-484146.5, foo->h, 0.1);
  EXPECT_EQ  ( 3609220., foo->x);
  EXPECT_EQ  ( 3508340., foo->y);
  EXPECT_EQ  ( 3055500., foo->z);
}
//==============================================================================
TEST_F(TestGeocentricFoo, InverseForWGSWithDelTest)
{
  foo->projGeocentric->Inverse(foo->ellipsoidWGS,
                               foo->x, foo->y, foo->z, foo->b, foo->l, foo->h,
                               &foo->d1, &foo->d2, &foo->d3);

  EXPECT_NEAR( 31.4448,  foo->b, 0.0001);
  EXPECT_NEAR( 44.188,   foo->l, 0.001);
  EXPECT_NEAR(-484146.5, foo->h, 0.1);
  EXPECT_EQ  ( 3609220., foo->x);
  EXPECT_EQ  ( 3508340., foo->y);
  EXPECT_EQ  ( 3055500., foo->z);
  EXPECT_EQ  (25.045,    foo->d1);
  EXPECT_EQ  (27.142,    foo->d2);
  EXPECT_EQ  (18.203,    foo->d3);
}
//==============================================================================
TEST_F(TestGeocentricFoo, InverseForUnknTest)
{
  foo->projGeocentric->Inverse(foo->ellipsoidUnkn,
                               foo->x, foo->y, foo->z, foo->b, foo->l, foo->h);

  EXPECT_NEAR( 31.4477,  foo->b, 0.0001);
  EXPECT_NEAR( 44.188,   foo->l, 0.001);
  EXPECT_NEAR(-583918.5, foo->h, 0.1);
  EXPECT_EQ  ( 3609220., foo->x);
  EXPECT_EQ  ( 3508340., foo->y);
  EXPECT_EQ  ( 3055500., foo->z);
}
//==============================================================================
TEST_F(TestGeocentricFoo, InverseForUnknWithDelTest)
{
  foo->projGeocentric->Inverse(foo->ellipsoidUnkn,
                               foo->x, foo->y, foo->z, foo->b, foo->l, foo->h,
                               &foo->d1, &foo->d2, &foo->d3);

  EXPECT_NEAR( 31.4477,  foo->b, 0.0001);
  EXPECT_NEAR( 44.188,   foo->l, 0.001);
  EXPECT_NEAR(-583918.5, foo->h, 0.1);
  EXPECT_EQ  ( 3609220., foo->x);
  EXPECT_EQ  ( 3508340., foo->y);
  EXPECT_EQ  ( 3055500., foo->z);
  EXPECT_EQ  (25.045,    foo->d1);
  EXPECT_EQ  (27.142,    foo->d2);
  EXPECT_EQ  (18.203,    foo->d3);
}
//==============================================================================
TEST_F(TestGeocentricFoo, InverseForSphereTest)
{
  foo->projGeocentric->Inverse(foo->sphere,
                               foo->x, foo->y, foo->z, foo->b, foo->l, foo->h);

  EXPECT_NEAR( 31.2597,  foo->b, 0.0001);
  EXPECT_NEAR( 44.188,   foo->l, 0.001);
  EXPECT_NEAR(-482791.5, foo->h, 0.1);
  EXPECT_EQ  ( 3609220., foo->x);
  EXPECT_EQ  ( 3508340., foo->y);
  EXPECT_EQ  ( 3055500., foo->z);
}
//==============================================================================
TEST_F(TestGeocentricFoo, InverseForSphereWithDelTest)
{
  foo->projGeocentric->Inverse(foo->sphere,
                               foo->x, foo->y, foo->z, foo->b, foo->l, foo->h,
                               &foo->d1, &foo->d2, &foo->d3);

  EXPECT_NEAR( 31.2597,  foo->b, 0.0001);
  EXPECT_NEAR( 44.188,   foo->l, 0.001);
  EXPECT_NEAR(-482791.5, foo->h, 0.1);
  EXPECT_EQ  ( 3609220., foo->x);
  EXPECT_EQ  ( 3508340., foo->y);
  EXPECT_EQ  ( 3055500., foo->z);
  EXPECT_EQ  (25.045,    foo->d1);
  EXPECT_EQ  (27.142,    foo->d2);
  EXPECT_EQ  (18.203,    foo->d3);
}
