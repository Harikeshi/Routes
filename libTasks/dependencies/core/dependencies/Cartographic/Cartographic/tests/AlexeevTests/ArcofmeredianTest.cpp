#include <gtest/gtest.h>
// Cartographic
#include <Cartographic/Alexeev/arcofmeridian.hpp>
#include <Cartographic/database.hpp>
//==============================================================================
using namespace Cartographic;
//==============================================================================
class ArcofmeredianFoo
{
public:
  ArcofmeredianFoo() :
    ellipsoidKras(),
    ellipsoidWGS(),
    ellipsoidUnkn(6478000, 0.0033528),
    sphere(6371000)
  {
    std::cout << "CONSTRUCTED" << std::endl;
  }

  ~ArcofmeredianFoo()
  {
    std::cout << "DESTRUCTED" << std::endl;
  }

  // Эллипсоиды
  Ellipsoid::Krasovsky const ellipsoidKras; // Эллипсоид Красовского
  Ellipsoid::WGS_84    const ellipsoidWGS;  // Эллипсоид WGS84
  Ellipsoid::Unknown   const ellipsoidUnkn; // Неизвестный эллипсоид
  Sphere               const sphere;        // Сфера

  Meters  a;
  Meters  e2;
  Radians b;
};
//==============================================================================
class TestArcofmeredianFoo : public ::testing::Test
{
protected:
  void SetUp()
  {
    foo = new ArcofmeredianFoo;

    // ПЗ-90
    foo->a  = 6478000;
    foo->e2 = 0.0033528;
    foo->b  = 0.3433;
  }

  void TearDown()
  {
    delete foo;
  }

  ArcofmeredianFoo *foo;
};
//==============================================================================
TEST_F(TestArcofmeredianFoo, LengthTest)
{
  Meters r = Alexeev::ArcOfMeridian::Length(foo->a, foo->e2, foo->b);

  ASSERT_NEAR(2216868.9393072859, r, .0000000001);
}
//==============================================================================
TEST_F(TestArcofmeredianFoo, LengthWithDelTest)
{
  Units pa(0);
  Units pc(0);

  Meters r = Alexeev::ArcOfMeridian::Length(foo->a, foo->e2, foo->b, &pa, &pc);

  ASSERT_NEAR(2216868.9393072859, r,  .0000000001);
  ASSERT_NEAR(6472566.7221395392, pa, .0000000001);
  ASSERT_NEAR(-5163.216403217999, pc, .0000000001);
}
//==============================================================================
TEST_F(TestArcofmeredianFoo, LengthEllipsTest)
{
  Meters rKras = Alexeev::ArcOfMeridian::Length(foo->ellipsoidKras, foo->b);
  Meters rWGS  = Alexeev::ArcOfMeridian::Length(foo->ellipsoidWGS,  foo->b);
  Meters rUnkn = Alexeev::ArcOfMeridian::Length(foo->ellipsoidUnkn, foo->b);
  Meters rSphr = Alexeev::ArcOfMeridian::Length(foo->sphere,        foo->b);

  ASSERT_NEAR(2175833.6076033856, rKras, .0000000001);
  ASSERT_NEAR(2175794.7860157313, rWGS,  .0000000001);
  ASSERT_NEAR(2209861.4232021621, rUnkn, .0000000001);
  ASSERT_NEAR(2187164.2999999998, rSphr, .0000000001);
}
//==============================================================================
TEST_F(TestArcofmeredianFoo, LengthEllipsWithDelTest)
{
  Units pa(0);
  Units pc(0);
  Meters r(0);

  r = Alexeev::ArcOfMeridian::Length(foo->ellipsoidKras, foo->b, &pa, &pc);

  ASSERT_NEAR(2175833.6076033856, r,  .0000000001);
  ASSERT_NEAR(6367558.4968178943, pa, .0000000001);
  ASSERT_NEAR(-10149.22435419744, pc, .0000000001);

  pa = 0;
  pc = 0;

  r = Alexeev::ArcOfMeridian::Length(foo->ellipsoidWGS,  foo->b, &pa, &pc);

  ASSERT_NEAR(2175794.7860157313, r,  .0000000001);
  ASSERT_NEAR(6367449.1457662918, pa, .0000000001);
  ASSERT_NEAR(-10150.50572583669, pc, .0000000001);

  pa = 0;
  pc = 0;

  r = Alexeev::ArcOfMeridian::Length(foo->ellipsoidUnkn, foo->b, &pa, &pc);

  ASSERT_NEAR(2209861.4232021621, r,  .0000000001);
  ASSERT_NEAR(6467144.8396937288, pa, .0000000001);
  ASSERT_NEAR(-10309.40026469463, pc, .0000000001);

  pa = 0;
  pc = 0;

  r = Alexeev::ArcOfMeridian::Length(foo->sphere,        foo->b, &pa, &pc);

  ASSERT_NEAR(2187164.2999999998, r, .0000000001);
  ASSERT_EQ  (6371000.,           pa            );
  ASSERT_EQ  (0.,                 pc            );
}
//==============================================================================
TEST_F(TestArcofmeredianFoo, AngleTest)
{
  Radians x    (0);
  Radians rKras(0);
  Radians rWGS (0);
  Radians rUnkn(0);
  Radians rSphr(0);

  x = 1250;

  rKras = Alexeev::ArcOfMeridian::Angle(foo->ellipsoidKras, x);
  rWGS  = Alexeev::ArcOfMeridian::Angle(foo->ellipsoidWGS,  x);
  rUnkn = Alexeev::ArcOfMeridian::Angle(foo->ellipsoidUnkn, x);
  rSphr = Alexeev::ArcOfMeridian::Angle(foo->sphere       , x);

  ASSERT_NEAR(0.00019729928163694091, rKras, 0.00000000000000000001);
  ASSERT_NEAR(0.00019730281283764705, rWGS,  0.00000000000000000001);
  ASSERT_NEAR(0.00019426124480349417, rUnkn, 0.00000000000000000001);
  ASSERT_NEAR(0.00019620153822005965, rSphr, 0.00000000000000000001);
}
