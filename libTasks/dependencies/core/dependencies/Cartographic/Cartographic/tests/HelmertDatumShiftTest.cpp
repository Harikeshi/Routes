#include <gtest/gtest.h>
// std
#include <list>
// Cartographic
#include <Cartographic/helmertdatumshift.hpp>
#include <Cartographic/database.hpp>
//==============================================================================
using namespace Cartographic;
//==============================================================================
constexpr double EPSILON(.0000001);
//==============================================================================
class HDatumFoo
{
public:
  HDatumFoo() :
    tWGStoPZ(
      new HelmertDatumShift
      <
        Datum::WGS_84,
        Datum::PZ_90_11,
        Authority::GOST_32453_2017
      >),
    tSKtoPZ(
      new HelmertDatumShift
      <
        Datum::SK_42,
        Datum::PZ_90_11,
        Authority::GOST_32453_2017
      >),
    tPZtoWGS(
      new HelmertDatumShift
      <
        Datum::PZ_90_11,
        Datum::WGS_84,
        Authority::GOST_32453_2017
      >),
    tPZtoSK(
      new HelmertDatumShift
      <
        Datum::PZ_90_11,
        Datum::SK_42,
        Authority::GOST_32453_2017
      >)
  {
    std::cout << "CONSTRUCTED" << std::endl;
  }

  ~HDatumFoo()
  {
    delete tWGStoPZ;
    delete tSKtoPZ;
    delete tPZtoWGS;
    delete tPZtoSK;
    std::cout << "DESTRUCTED" << std::endl;
  }

  // Сдвиги
  IDatumShift *tWGStoPZ; // Из WGS84 в ПЗ-90.11
  IDatumShift *tSKtoPZ;  // Из СК-42 в ПЗ-90.11
  IDatumShift *tPZtoWGS; // Из ПЗ-90.11 в WGS84
  IDatumShift *tPZtoSK;  // Из ПЗ-90.11 в СК-42

  // Сдвинутые координаты
  Meters tx;
  Meters ty;
  Meters tz;

  // Доп. координаты
  Meters dx;
  Meters dy;
  Meters dz;

  // Исходные координаты
  Meters sx;
  Meters sy;
  Meters sz;
};
//==============================================================================
class TestHDatumFoo : public ::testing::Test
{
protected:
  void SetUp()
  {
    foo = new HDatumFoo;

    foo->sx = 42;
    foo->sy = 59;
    foo->sz = 10;

    foo->tx = 0;
    foo->ty = 0;
    foo->tz = 0;

    foo->dx = 0;
    foo->dy = 0;
    foo->dz = 0;
  }

  void TearDown()
  {
    delete foo;
  }

  HDatumFoo *foo;
};
//==============================================================================
TEST_F(TestHDatumFoo, IdentityTest)
{
  ASSERT_FALSE(foo->tWGStoPZ->IsIdentity());
  ASSERT_FALSE(foo->tSKtoPZ-> IsIdentity());
  ASSERT_FALSE(foo->tPZtoWGS->IsIdentity());
  ASSERT_FALSE(foo->tPZtoSK-> IsIdentity());
}
//==============================================================================
TEST_F(TestHDatumFoo, AuthorityTest)
{
  // ГОСТ
  EXPECT_EQ(Authority::GOST_32453_2017, foo->tWGStoPZ->Authority());
  EXPECT_EQ(Authority::GOST_32453_2017, foo->tSKtoPZ-> Authority());
  EXPECT_EQ(Authority::GOST_32453_2017, foo->tPZtoWGS->Authority());
  EXPECT_EQ(Authority::GOST_32453_2017, foo->tPZtoSK-> Authority());
}
//==============================================================================
TEST_F(TestHDatumFoo, SourceDatumTest)
{
  // Входной датум
  EXPECT_EQ(Datum::WGS_84,   foo->tWGStoPZ->SourceDatum());
  EXPECT_EQ(Datum::SK_42,    foo->tSKtoPZ-> SourceDatum());
  EXPECT_EQ(Datum::PZ_90_11, foo->tPZtoWGS->SourceDatum());
  EXPECT_EQ(Datum::PZ_90_11, foo->tPZtoSK-> SourceDatum());
}
//==============================================================================
TEST_F(TestHDatumFoo, TargetDatumTest)
{
  // Датум перевода
  EXPECT_EQ(Datum::PZ_90_11, foo->tWGStoPZ->TargetDatum());
  EXPECT_EQ(Datum::PZ_90_11, foo->tSKtoPZ-> TargetDatum());
  EXPECT_EQ(Datum::WGS_84,   foo->tPZtoWGS->TargetDatum());
  EXPECT_EQ(Datum::SK_42,    foo->tPZtoSK-> TargetDatum());
}
//==============================================================================
TEST_F(TestHDatumFoo, WGStoPZShiftTest)
{
  // Сдвиг из WGS84 в ПЗ-90.11
  foo->tWGStoPZ->Shift(foo->sx, foo->sy, foo->sz,
                       foo->tx, foo->ty, foo->tz);

  EXPECT_NEAR(42.,        foo->sx, EPSILON);
  EXPECT_NEAR(59.,        foo->sy, EPSILON);
  EXPECT_NEAR(10.,        foo->sz, EPSILON);
  EXPECT_NEAR(41.9869983, foo->tx, EPSILON);
  EXPECT_NEAR(59.1060003, foo->ty, EPSILON);
  EXPECT_NEAR(10.0220013, foo->tz, EPSILON);
}
//==============================================================================
TEST_F(TestHDatumFoo, WGStoPZShiftWithDelTest)
{
  // Сдвиг из WGS84 в ПЗ-90.11
  foo->tWGStoPZ->Shift( foo->sx,  foo->sy,  foo->sz,
                        foo->tx,  foo->ty,  foo->tz,
                       &foo->dx, &foo->dy, &foo->dz);

  EXPECT_NEAR(42.,        foo->sx, EPSILON);
  EXPECT_NEAR(59.,        foo->sy, EPSILON);
  EXPECT_NEAR(10.,        foo->sz, EPSILON);
  EXPECT_NEAR(41.9869983, foo->tx, EPSILON);
  EXPECT_NEAR(59.1060003, foo->ty, EPSILON);
  EXPECT_NEAR(10.0220013, foo->tz, EPSILON);
  EXPECT_NEAR(0.,         foo->dx, EPSILON);
  EXPECT_NEAR(0.,         foo->dy, EPSILON);
  EXPECT_NEAR(0.,         foo->dz, EPSILON);
}
//==============================================================================
TEST_F(TestHDatumFoo, SKtoPZShiftTest)
{
  // Сдвиг из СК-42 в ПЗ-90.11
  foo->tSKtoPZ->Shift(foo->sx, foo->sy, foo->sz,
                      foo->tx, foo->ty, foo->tz);

  EXPECT_NEAR(42.,          foo->sx, EPSILON);
  EXPECT_NEAR(59.,          foo->sy, EPSILON);
  EXPECT_NEAR(10.,          foo->sz, EPSILON);
  EXPECT_NEAR(65.5567800,   foo->tx, EPSILON);
  EXPECT_NEAR(-81.8438518,  foo->ty, EPSILON);
  EXPECT_NEAR(-69.7780722,  foo->tz, EPSILON);
}
//==============================================================================
TEST_F(TestHDatumFoo, SKtoPZShiftWithDelTest)
{
  // Сдвиг из СК-42 в ПЗ-90.11
  foo->tSKtoPZ->Shift( foo->sx,  foo->sy,  foo->sz,
                       foo->tx,  foo->ty,  foo->tz,
                      &foo->dx, &foo->dy, &foo->dz);

  EXPECT_NEAR(42.,          foo->sx, EPSILON);
  EXPECT_NEAR(59.,          foo->sy, EPSILON);
  EXPECT_NEAR(10.,          foo->sz, EPSILON);
  EXPECT_NEAR(65.5567800,   foo->tx, EPSILON);
  EXPECT_NEAR(-81.8438518,  foo->ty, EPSILON);
  EXPECT_NEAR(-69.7780722,  foo->tz, EPSILON);
  EXPECT_NEAR(0.,           foo->dx, EPSILON);
  EXPECT_NEAR(0.,           foo->dy, EPSILON);
  EXPECT_NEAR(0.,           foo->dz, EPSILON);
}
//==============================================================================
TEST_F(TestHDatumFoo, PZtoWGSShiftTest)
{
  // Сдвиг из ПЗ-90 в WGS84
  foo->tPZtoWGS->Shift(foo->sx, foo->sy, foo->sz,
                       foo->tx, foo->ty, foo->tz);

  EXPECT_NEAR(42.,        foo->sx, EPSILON);
  EXPECT_NEAR(59.,        foo->sy, EPSILON);
  EXPECT_NEAR(10.,        foo->sz, EPSILON);
  EXPECT_NEAR(42.0130017, foo->tx, EPSILON);
  EXPECT_NEAR(58.8939997, foo->ty, EPSILON);
  EXPECT_NEAR(9.9779987,  foo->tz, EPSILON);
}
//==============================================================================
TEST_F(TestHDatumFoo, PZtoWGSShiftWithDelTest)
{
  // Сдвиг из ПЗ-90 в WGS84
  foo->tPZtoWGS->Shift( foo->sx,  foo->sy,  foo->sz,
                        foo->tx,  foo->ty,  foo->tz,
                       &foo->dx, &foo->dy, &foo->dz);

  EXPECT_NEAR(42.,        foo->sx, EPSILON);
  EXPECT_NEAR(59.,        foo->sy, EPSILON);
  EXPECT_NEAR(10.,        foo->sz, EPSILON);
  EXPECT_NEAR(42.0130017, foo->tx, EPSILON);
  EXPECT_NEAR(58.8939997, foo->ty, EPSILON);
  EXPECT_NEAR(9.9779987,  foo->tz, EPSILON);
  EXPECT_NEAR(0.,         foo->dx, EPSILON);
  EXPECT_NEAR(0.,         foo->dy, EPSILON);
  EXPECT_NEAR(0.,         foo->dz, EPSILON);
}
//==============================================================================
TEST_F(TestHDatumFoo, PZtoSKShiftTest)
{
  // Сдвиг из ПЗ-90 в СК-42
  foo->tPZtoSK->Shift(foo->sx, foo->sy, foo->sz,
                      foo->tx, foo->ty, foo->tz);

  EXPECT_NEAR(42.,        foo->sx, EPSILON);
  EXPECT_NEAR(59.,        foo->sy, EPSILON);
  EXPECT_NEAR(10.,        foo->sz, EPSILON);
  EXPECT_NEAR(18.4432200, foo->tx, 0.01);
  EXPECT_NEAR(199.843852, foo->ty, 0.01);
  EXPECT_NEAR(89.7780721, foo->tz, 0.01);
}
//==============================================================================
TEST_F(TestHDatumFoo, PZtoSKShiftWithDelTest)
{
  // Сдвиг из ПЗ-90 в СК-42
  foo->tPZtoSK->Shift( foo->sx,  foo->sy,  foo->sz,
                       foo->tx,  foo->ty,  foo->tz,
                      &foo->dx, &foo->dy, &foo->dz);

  EXPECT_NEAR(42.,        foo->sx, EPSILON);
  EXPECT_NEAR(59.,        foo->sy, EPSILON);
  EXPECT_NEAR(10.,        foo->sz, EPSILON);
  EXPECT_NEAR(18.4432200, foo->tx, 0.01);
  EXPECT_NEAR(199.843852, foo->ty, 0.01);
  EXPECT_NEAR(89.7780721, foo->tz, 0.01);
  EXPECT_NEAR(0.,         foo->dx, EPSILON);
  EXPECT_NEAR(0.,         foo->dy, EPSILON);
  EXPECT_NEAR(0.,         foo->dz, EPSILON);
}
//==============================================================================
TEST_F(TestHDatumFoo, HelmetFunctionTest)
{
  Meters d_x(12);
  Meters d_y(9);
  Meters d_z(1);

  Radians w_x(0.25);
  Radians w_y(0.201);
  Radians w_z(0.097);

  Units m(320);

  HelmertFunction(d_x, d_y, d_z, w_x, w_y, w_z, m,
                  foo->sx, foo->sy, foo->sz,
                  foo->tx, foo->ty, foo->tz);

  EXPECT_EQ  (12,        d_x           );
  EXPECT_EQ  (9,         d_y           );
  EXPECT_EQ  (1,         d_z           );
  EXPECT_EQ  (0.25,      w_x           );
  EXPECT_EQ  (0.201,     w_y           );
  EXPECT_EQ  (0.097,     w_z           );
  EXPECT_EQ  (320,       m             );
  EXPECT_EQ  (42,        foo->sx       );
  EXPECT_EQ  (59,        foo->sy       );
  EXPECT_EQ  (10,        foo->sz       );
  EXPECT_NEAR(14685.873, foo->tx, 0.001);
  EXPECT_NEAR(18442.746, foo->ty, 0.001);
  EXPECT_NEAR(1186.132,  foo->tz, 0.001);
}
//==============================================================================
TEST_F(TestHDatumFoo, HelmetFunctionWithDelTest)
{
  Meters d_x(12);
  Meters d_y(9);
  Meters d_z(1);

  Radians w_x(0.25);
  Radians w_y(0.201);
  Radians w_z(0.097);

  Units m(320);

  HelmertFunction( d_x, d_y, d_z, w_x, w_y, w_z, m,
                   foo->sx, foo->sy, foo->sz,
                   foo->tx, foo->ty, foo->tz,
                  &foo->dx, &foo->dy, &foo->dz);

  EXPECT_EQ  (12,        d_x           );
  EXPECT_EQ  (9,         d_y           );
  EXPECT_EQ  (1,         d_z           );
  EXPECT_EQ  (0.25,      w_x           );
  EXPECT_EQ  (0.201,     w_y           );
  EXPECT_EQ  (0.097,     w_z           );
  EXPECT_EQ  (320,       m             );
  EXPECT_EQ  (42,        foo->sx       );
  EXPECT_EQ  (59,        foo->sy       );
  EXPECT_EQ  (10,        foo->sz       );
  EXPECT_NEAR(14685.873, foo->tx, 0.001);
  EXPECT_NEAR(18442.746, foo->ty, 0.001);
  EXPECT_NEAR(1186.132,  foo->tz, 0.001);
  EXPECT_EQ  (0,         foo->dx       );
  EXPECT_EQ  (0,         foo->dy       );
  EXPECT_EQ  (0,         foo->dz       );
}
