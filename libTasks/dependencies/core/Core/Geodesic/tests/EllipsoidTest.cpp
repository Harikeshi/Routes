#include <Geodesic/Helper/Ellipsoid.hpp>

#include <Geodesic/GeoLineTools/LoxodromeTools.hpp>
#include <Geodesic/GeoLineTools/OrthodromyTools.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! Cartographic
#include <Cartographic/database.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование конструктора по параметрам
*/
TEST( EllipsoidTest, ConstructorByParameters )
{
  using namespace BasicMath;

  //! Готовим эллипсиод
  Ellipsoid ellipsoid( 6378136., 1. / 298.25784 );
  const double minorRadius( 6378136. * ( 1. - 1. / 298.25784 ) );

  //! Сравниваем 
  ASSERT_TRUE( isEqual( 6378136., ellipsoid.getMajorRadius( ) ) );
  ASSERT_TRUE( isEqual( minorRadius, ellipsoid.getMinorRadius( ) ) );
  ASSERT_TRUE( isEqual( 1. / 298.25784, ellipsoid.getFlatenning( ) ) );
}
//==============================================================================
/*!
 *\brief Тестирование конструктора по типу для WGS 84
*/
TEST( EllipsoidTest, ConstructorByTypeWGS84 )
{
  using namespace BasicMath;

  //! Готовим эллипсоид типа WGS 84
  Ellipsoid ellipsoid( EL_WGS_84 );

  //! Вычисляем контрольные значения
  auto ellipsoidWGS84 = std::make_shared<Cartographic::Ellipsoid::WGS_84>( );
  const double flattening( 1. - sqrt( 1 - ellipsoidWGS84->E2( ) ) );
  const double minorRadius( ellipsoidWGS84->A( ) * ( 1. - flattening ) );

  //! Сравниваем
  ASSERT_TRUE( isEqual( ellipsoidWGS84->A( ), ellipsoid.getMajorRadius( ) ) );
  ASSERT_TRUE( isEqual( minorRadius, ellipsoid.getMinorRadius( ) ) );
  ASSERT_TRUE( isEqual( flattening, ellipsoid.getFlatenning( ) ) );
}
//==============================================================================
/*!
*\brief Тестирование конструктора по типу для СК-42
*/
TEST( EllipsoidTest, ConstructorByTypeSK42 )
{
  using namespace BasicMath;

  //! Готовим эллипсоид типа СК-42
  Ellipsoid ellipsoid( EL_KRASOVSKY );

  //! Вычисляем контрольные значения
  auto ellipsoidSK42 = std::make_shared<Cartographic::Ellipsoid::Krasovsky>( );
  const double flattening( 1. - sqrt( 1 - ellipsoidSK42->E2( ) ) );
  const double minorRadius( ellipsoidSK42->A( ) * ( 1. - flattening ) );

  //! Сравниваем
  ASSERT_TRUE( isEqual( ellipsoidSK42->A( ), ellipsoid.getMajorRadius( ) ) );
  ASSERT_TRUE( isEqual( minorRadius, ellipsoid.getMinorRadius( ) ) );
  ASSERT_TRUE( isEqual( flattening, ellipsoid.getFlatenning( ) ) );
}
//==============================================================================
/*!
*\brief Тестирование конструктора по типу для ПЗ-90
*/
TEST( EllipsoidTest, ConstructorByTypePZ90 )
{
  using namespace BasicMath;

  //! Готовим эллипсоид типа ПЗ-90
  Ellipsoid ellipsoid( EL_GENERAL_GLOBE );

  //! Вычисляем контрольные значения
  auto ellipsoidPZ90
    = std::make_shared<Cartographic::Ellipsoid::GeneralGlobe>( );
  const double flattening( 1. - sqrt( 1 - ellipsoidPZ90->E2( ) ) );
  const double minorRadius( ellipsoidPZ90->A( ) * ( 1. - flattening ) );

  //! Сравниваем
  ASSERT_TRUE( isEqual( ellipsoidPZ90->A( ), ellipsoid.getMajorRadius( ) ) );
  ASSERT_TRUE( isEqual( minorRadius, ellipsoid.getMinorRadius( ) ) );
  ASSERT_TRUE( isEqual( flattening, ellipsoid.getFlatenning( ) ) );
}
//==============================================================================
/*!
 *\brief Тестирование создания инструментов для ортодромии и локсодромии
*/
TEST( EllipsoidTest, CreateTools )
{
  using namespace BasicMath;

  Ellipsoid ellipsoid( EL_WGS_84 );
  auto loxodromeTools = ellipsoid.createLoxodromeTools( );
  auto orthodromyTools = ellipsoid.createOrthodromyTools( );

  ASSERT_TRUE( isEqual( ellipsoid.getMajorRadius( ),
    loxodromeTools->getMajorRadius( ) ) );
  ASSERT_TRUE( isEqual( ellipsoid.getFlatenning( ),
    loxodromeTools->getFlatenning( ) ) );
  ASSERT_TRUE( isEqual( ellipsoid.getMajorRadius( ),
    orthodromyTools->getMajorRadius( ) ) );
  ASSERT_TRUE( isEqual( ellipsoid.getFlatenning( ),
    orthodromyTools->getFlatenning( ) ) );
}
//==============================================================================
} //! Tests
} //! Geodesic