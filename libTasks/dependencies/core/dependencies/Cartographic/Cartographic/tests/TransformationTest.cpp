#include <gtest/gtest.h>
// Cartographic
#include <Cartographic/Alexeev/alexeev.hpp>
#include <Cartographic/database.hpp>
#include <Cartographic/latitudelongitude.hpp>
#include <Cartographic/transformation.hpp>
//==============================================================================
using namespace Cartographic;
//==============================================================================
class TranformationFoo
{
public:
  TranformationFoo()
    : transformationSKnWGS(),
      transformationGKnWGS()
  {
    // Настройка преобразования координат из СК-42 в WGS84
    // Исходная проекция (Кросовского)
    transformationSKnWGS.SetSourceProjection<LatitudeLongitude>(
          new Ellipsoid::Krasovsky);
    transformationSKnWGS.AddDatumShift(
      new HelmertDatumShift<Datum::SK_42, Datum::PZ_90_11, Authority::GOST_32453_2017>);
    transformationSKnWGS.AddDatumShift(
      new HelmertDatumShift<Datum::PZ_90_11, Datum::WGS_84, Authority::GOST_32453_2017>);
    transformationSKnWGS.SetGeocentric<Alexeev::Geocentric>();
    // Проекция для перевода (WGS84)
    transformationSKnWGS.SetTargetProjection<LatitudeLongitude>(
          new Ellipsoid::WGS_84);

    // Настройка преобразования координат из Гаусса-Крюгера в WGS84
    // Исходная проекция (Гаусса-Крюгера)
    Alexeev::GaussKrueger::BaseParameters const baseParameters = {};
    transformationGKnWGS.SetSourceProjection<Alexeev::GaussKrueger>(
          baseParameters);
    transformationGKnWGS.AddDatumShift(
      new HelmertDatumShift<Datum::SK_42, Datum::PZ_90_11, Authority::GOST_32453_2017>);
    transformationGKnWGS.AddDatumShift(
      new HelmertDatumShift<Datum::PZ_90_11, Datum::WGS_84, Authority::GOST_32453_2017>);
    transformationGKnWGS.SetGeocentric<Alexeev::Geocentric>();
    // Проекция для перевода (WGS84)
    transformationGKnWGS.
      SetTargetProjection<LatitudeLongitude>(new Ellipsoid::WGS_84);

    std::cout << "CONSTRUCTED" << std::endl;
  }

  ~TranformationFoo()
  { std::cout << "DESTRUCTED"  << std::endl; }

  struct TestPoint
  {
    TestPoint()
      : x(0), y(0), h(0) {}

    Units x;
    Units y;
    Units h = 0;
  };

  void exchangeTransformationSK42()
  {
    // Инвертирование преобразования координат из WGS84 в СК-42
    // Исходная проекция (WGS84)
    transformationSKnWGS.SetSourceProjection<LatitudeLongitude>(
          new Ellipsoid::WGS_84);
    transformationSKnWGS.AddDatumShift(
      new HelmertDatumShift<Datum::WGS_84, Datum::PZ_90_11, Authority::GOST_32453_2017>);
    transformationSKnWGS.AddDatumShift(
      new HelmertDatumShift<Datum::PZ_90_11, Datum::SK_42, Authority::GOST_32453_2017>);
    transformationSKnWGS.SetGeocentric<Alexeev::Geocentric>();
    // Проекция для перевода (Кросовского)
    transformationSKnWGS.SetTargetProjection<LatitudeLongitude>(
          new Ellipsoid::Krasovsky);
  }

  void exchangeTransformationGK()
  {
    // Инвертирование преобразования координат из WGS84 в Гаусса-Крюгера
    // Исходная проекция (WGS84)
    transformationGKnWGS.
      SetSourceProjection<LatitudeLongitude>(new Ellipsoid::WGS_84);
    transformationGKnWGS.AddDatumShift(
      new HelmertDatumShift<Datum::SK_42, Datum::PZ_90_11, Authority::GOST_32453_2017>);
    transformationGKnWGS.AddDatumShift(
      new HelmertDatumShift<Datum::PZ_90_11, Datum::WGS_84, Authority::GOST_32453_2017>);
    transformationGKnWGS.SetGeocentric<Alexeev::Geocentric>();
    // Проекция для перевода (Гаусса-Крюгера )
    Alexeev::GaussKrueger::BaseParameters const baseParameters = {};
    transformationGKnWGS.SetTargetProjection<Alexeev::GaussKrueger>(
          baseParameters);
  }

  // Координаты
  TestPoint pKrasovsky; // Входная точка в СК-42
  TestPoint pGK;        // Входная точка в Г-К
  TestPoint rWGS1;      // Результат перевода из СК-42 в WGS84
  TestPoint rWGS2;      // Результат перевода из Г-К   в WGS84

  // Трансформаторы
  Transformation transformationSKnWGS;
  Transformation transformationGKnWGS;
};
//==============================================================================
class TestTranformationFoo : public ::testing::Test
{
protected:
  void SetUp()
  {
    foo = new TranformationFoo;

    foo->pKrasovsky.x =  -5.;
    foo->pKrasovsky.y = 115.;

    foo->pGK.x = 6609072.86;
    foo->pGK.y = 2612367.52;

    foo->rWGS1.x =  -5.0007902;
    foo->rWGS1.y = 115.0005716;
    foo->rWGS1.h = -23.6106047;
    foo->rWGS2.x =  59.5795407;
    foo->rWGS2.y =  10.9864215;
    foo->rWGS2.h =  37.8771221;
  }

  void TearDown()
  {
    delete foo;
  }

  TranformationFoo *foo;
};
//==============================================================================
TEST_F(TestTranformationFoo, KrasovskytoWGS84)
{
  // Трансформация (перевод в WGS84)
  foo->transformationSKnWGS.Transform(foo->pKrasovsky.x,
                                      foo->pKrasovsky.y,
                                      foo->pKrasovsky.h);

  EXPECT_NEAR(foo->rWGS1.x, foo->pKrasovsky.x, 0.0000001);
  EXPECT_NEAR(foo->rWGS1.y, foo->pKrasovsky.y, 0.0000001);
  EXPECT_NEAR(foo->rWGS1.h, foo->pKrasovsky.h, 0.0000001);
}
//==============================================================================
TEST_F(TestTranformationFoo, WGS84toKrasovsky)
{
  // Настройка преобразования координат из WGS84 в СК-42
  foo->exchangeTransformationSK42();

  // Трансформация (перевод в СК-42)
  foo->transformationSKnWGS.Transform(foo->rWGS1.x,
                                      foo->rWGS1.y,
                                      foo->rWGS1.h);

  EXPECT_NEAR(foo->pKrasovsky.x, foo->rWGS1.x, 0.001);
  EXPECT_NEAR(foo->pKrasovsky.y, foo->rWGS1.y, 0.001);
}
//==============================================================================
TEST_F(TestTranformationFoo, GKtoWGS84)
{
  // Трансформация (перевод в WGS84)
  foo->transformationGKnWGS.Transform(foo->pGK.x,
                                      foo->pGK.y,
                                      foo->pGK.h);

  EXPECT_NEAR(foo->rWGS2.x, foo->pGK.x, 0.0000001);
  EXPECT_NEAR(foo->rWGS2.y, foo->pGK.y, 0.0000001);
  EXPECT_NEAR(foo->rWGS2.h, foo->pGK.h, 0.0000001);
}
//==============================================================================
TEST_F(TestTranformationFoo, WGS84toGK)
{
  // Настройка преобразования координат из WGS84 в Гаусса-Крюгера
  foo->exchangeTransformationGK();

  // Трансформация (перевод в Гаусса-Крюгера )
  foo->transformationGKnWGS.Transform(foo->rWGS2.x,
                                    foo->rWGS2.y,
                                    foo->rWGS2.h);

  // 1000. - погрешность
  EXPECT_NEAR(foo->pGK.x, foo->rWGS2.x, 1000.);
  EXPECT_NEAR(foo->pGK.y, foo->rWGS2.y, 1000.);
  EXPECT_NEAR(foo->pGK.h, foo->rWGS2.h, 1000.);
}
