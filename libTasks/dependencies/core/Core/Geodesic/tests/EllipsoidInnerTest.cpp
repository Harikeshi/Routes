#include <Helper/EllipsoidInner.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование конструктора
*/
TEST(EllipsoidInnerTest, Constructor)
{ 
  using namespace BasicMath;

  //! Вычисляем контрольные значения
  const double a        (6378136.);
  const double f        (1. / 298.25784);
  const double b        (a * (1. - f));
  const double ee       (f * (2. - f));
  const double ree      (1. / ee);
  const double e        (sqrt(ee));
  const double aDEVe    (a / e);
  const double aa       (a * a);
  const double aaDEVbb  (aa / (b * b));
  const double aee      (a * ee);

  //! Собираем объект класса
  EllipsoidInner ellipsoid(a, f);

  //! Сравниваем 
  ASSERT_TRUE(isEqual(6378136.,       ellipsoid.a      ));
  ASSERT_TRUE(isEqual(1. / 298.25784, ellipsoid.f      ));
  ASSERT_TRUE(isEqual(b,              ellipsoid.b      ));
  ASSERT_TRUE(isEqual(ee,             ellipsoid.ee     ));
  ASSERT_TRUE(isEqual(ree,            ellipsoid.ree    ));
  ASSERT_TRUE(isEqual(e,              ellipsoid.e      ));
  ASSERT_TRUE(isEqual(aDEVe,          ellipsoid.aDEVe  ));
  ASSERT_TRUE(isEqual(aa,             ellipsoid.aa     ));
  ASSERT_TRUE(isEqual(aaDEVbb,        ellipsoid.aaDEVbb));
  ASSERT_TRUE(isEqual(aee,            ellipsoid.aee    ));
}
//==============================================================================
} //! Tests
} //! Geodesic
//==============================================================================
