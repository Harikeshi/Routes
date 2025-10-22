#include <Geometry/Operations/AtVectors.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
/*!
* \brief Тестирование operator & для векторов
*        Скалярное произведение векторов
*/
TEST( VectorOperationsTest, ScalarProduct )
{
  using namespace BasicMath;

  Vector3D vector1;
  Vector3D vector2( Point3D( -2.4, 4.3, -3.7 ), Point3D( 1.1, -2., 4.5 ) );

  ASSERT_TRUE( isEqual( 0., scalarProduct( vector1, vector2 ) ) );

  vector1 = Vector3D( Point3D( -1., 2.5, -4. ), Point3D( 3., -17.8, 9.1 ) );
  ASSERT_TRUE( isEqual( 4. * 3.5 + 20.3 * 6.3 + 13.1 * 8.2,
    scalarProduct( vector1, vector2 ) ) );
}
//==============================================================================
/*!
* \brief Тестирование operator * для векторов
*        Векторное произведение векторов
*/
TEST( VectorOperationsTest, CrossProduct )
{
  using namespace BasicMath;

  Vector3D vector1;
  Vector3D vector2( Point3D( -2.4, 4.3, -3.7 ), Point3D( 1.1, -2., 4.5 ) );
  Vector3D vector3 = crossProduct( vector1, vector2 );
  ASSERT_TRUE( isEqual( 0., vector3.getOX( ) ) );
  ASSERT_TRUE( isEqual( 0., vector3.getOY( ) ) );
  ASSERT_TRUE( isEqual( 0., vector3.getOZ( ) ) );
  ASSERT_EQ( Point3D( ), vector3.getAnchor( ) );

  vector1 = Vector3D( Point3D( -1., 2.5, -4. ), Point3D( 3., -17.8, 9.1 ) );
  vector3 = crossProduct( vector1, vector2 );
  ASSERT_TRUE( isEqual( -20.3 * 8.2 + 13.1 * 6.3, vector3.getOX( ) ) );
  ASSERT_TRUE( isEqual( 13.1 * 3.5 - 4. * 8.2, vector3.getOY( ) ) );
  ASSERT_TRUE( isEqual( 4 * -6.3 + 20.3 * 3.5, vector3.getOZ( ) ) );
  ASSERT_EQ( Point3D( -1., 2.5, -4. ), vector3.getAnchor( ) );

  vector3 = crossProduct( vector2, vector1 );
  ASSERT_TRUE( isEqual( -6.3 * 13.1 + 8.2 * 20.3, vector3.getOX( ) ) );
  ASSERT_TRUE( isEqual( 8.2 * 4. - 3.5 * 13.1, vector3.getOY( ) ) );
  ASSERT_TRUE( isEqual( 3.5 * -20.3 + 6.3 * 4., vector3.getOZ( ) ) );
  ASSERT_EQ( Point3D( -2.4, 4.3, -3.7 ), vector3.getAnchor( ) );
}
//==============================================================================
/*!
* \brief Тестирование cosAngle
*        Проверка нахождения косинуса угла
*/
TEST( VectorOperationsTest, CosAngle )
{
  using namespace BasicMath;

  Vector3D vector1 = Vector3D( Point3D( 0., 1., 1. ) );
  Vector3D vector2 = Vector3D( Point3D( 2., 2., 0. ) );
  ASSERT_TRUE( isEqual( 0.5, cosAngle( vector1, vector2 ) ) );

  vector1 = Vector3D( Point3D( -1., 1., 0. ) );
  vector2 = Vector3D( Point3D( -2., -2., 0. ) );
  ASSERT_TRUE( isEqual( 0., cosAngle( vector1, vector2 ) ) );

  vector1 = Vector3D( Point3D( 0., 0., 0. ) );
  vector2 = Vector3D( Point3D( 2., 2., 0. ) );
  ASSERT_TRUE( isEqual( 1., cosAngle( vector1, vector2 ) ) );
}
//==============================================================================
} //! Tests
} //! Geometry
//==============================================================================