#include <Point/Point3D.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование Point3D
 *        (конструктор на оcнове трёх значений в double)
 */
TEST( Point3DTest, ConstructorPoint3D )
{
  using namespace BasicMath;

  Point3D point1;
  ASSERT_TRUE( isEqual( 0., point1.getX( ) ) );
  ASSERT_TRUE( isEqual( 0., point1.getY( ) ) );
  ASSERT_TRUE( isEqual( 0., point1.getZ( ) ) );
  Point3D point2( 0.1, -10., 5. );
  ASSERT_TRUE( isEqual( 0.1, point2.getX( ) ) );
  ASSERT_TRUE( isEqual( -10., point2.getY( ) ) );
  ASSERT_TRUE( isEqual( 5., point2.getZ( ) ) );
  Point3D point3( -10., -0.1, -3. );
  ASSERT_TRUE( isEqual( -10., point3.getX( ) ) );
  ASSERT_TRUE( isEqual( -0.1, point3.getY( ) ) );
  ASSERT_TRUE( isEqual( -3., point3.getZ( ) ) );
  Point3D point4( -0.5, 0.5, -0.5 );
  ASSERT_TRUE( isEqual( -0.5, point4.getX( ) ) );
  ASSERT_TRUE( isEqual( 0.5, point4.getY( ) ) );
  ASSERT_TRUE( isEqual( -0.5, point4.getZ( ) ) );
}
//==============================================================================
TEST( Point3DTest, ConstructorCopied )
{
  using namespace BasicMath;

  Point3D point;
  Point3D point1( point );
  ASSERT_TRUE( isEqual( 0., point1.getX( ) ) );
  ASSERT_TRUE( isEqual( 0., point1.getY( ) ) );
  ASSERT_TRUE( isEqual( 0., point1.getZ( ) ) );
  point.set( 0.1, -10., 5. );
  Point3D point2( point );
  ASSERT_TRUE( isEqual( 0.1, point2.getX( ) ) );
  ASSERT_TRUE( isEqual( -10., point2.getY( ) ) );
  ASSERT_TRUE( isEqual( 5., point2.getZ( ) ) );
  point.set( -10., -0.1, -3. );
  Point3D point3( point );
  ASSERT_TRUE( isEqual( -10., point3.getX( ) ) );
  ASSERT_TRUE( isEqual( -0.1, point3.getY( ) ) );
  ASSERT_TRUE( isEqual( -3., point3.getZ( ) ) );
  point.set( -0.5, 0.5, -0.5 );
  Point3D point4( point );
  ASSERT_TRUE( isEqual( -0.5, point4.getX( ) ) );
  ASSERT_TRUE( isEqual( 0.5, point4.getY( ) ) );
  ASSERT_TRUE( isEqual( -0.5, point4.getZ( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование set и get
 *        (присвоение коордиант на основе double и получение)
 */
TEST( Point3DTest, Point3DGetSet )
{
  using namespace BasicMath;

  Point3D point;
  std::tuple<double, double, double> resPoint = point.get();
  ASSERT_TRUE( isEqual( 0., std::get<0>(resPoint) ) );
  ASSERT_TRUE( isEqual( 0., std::get<1>(resPoint) ) );
  ASSERT_TRUE( isEqual( 0., std::get<2>(resPoint) ) );
  point.set( 0.1, -10., 5. );
  resPoint = point.get();
  ASSERT_TRUE( isEqual( 0.1, std::get<0>(resPoint) ) );
  ASSERT_TRUE( isEqual( -10., std::get<1>(resPoint) ) );
  ASSERT_TRUE( isEqual( 5., std::get<2>(resPoint) ) );
  point.set( -10., -0.1, -3. );
  resPoint = point.get();
  ASSERT_TRUE( isEqual( -10., std::get<0>(resPoint) ) );
  ASSERT_TRUE( isEqual( -0.1, std::get<1>(resPoint) ) );
  ASSERT_TRUE( isEqual( -3., std::get<2>(resPoint)) );
  point.set( -0.5, 0.5, -0.5 );
  resPoint = point.get();
  ASSERT_TRUE( isEqual( -0.5, std::get<0>(resPoint) ) );
  ASSERT_TRUE( isEqual( 0.5, std::get<1>(resPoint)) );
  ASSERT_TRUE( isEqual( -0.5, std::get<2>(resPoint) ) );
}
//==============================================================================
/*!
 * \brief Тестирование getX, getY, getZ
 *        (получение коордиант по определённым осям)
 */
TEST( Point3DTest, Point3DGetXYZ )
{
  using namespace BasicMath;

  Point3D point;
  ASSERT_TRUE( isEqual( 0., point.getX( ) ) );
  ASSERT_TRUE( isEqual( 0., point.getY( ) ) );
  ASSERT_TRUE( isEqual( 0., point.getZ( ) ) );
  point.set( 0.1, -10., 5. );
  ASSERT_TRUE( isEqual( 0.1, point.getX( ) ) );
  ASSERT_TRUE( isEqual( -10., point.getY( ) ) );
  ASSERT_TRUE( isEqual( 5., point.getZ( ) ) );
  point.set( -10., -0.1, -3. );
  ASSERT_TRUE( isEqual( -10., point.getX( ) ) );
  ASSERT_TRUE( isEqual( -0.1, point.getY( ) ) );
  ASSERT_TRUE( isEqual( -3., point.getZ( ) ) );
  point.set( -0.5, 0.5, -0.5 );
  ASSERT_TRUE( isEqual( -0.5, point.getX( ) ) );
  ASSERT_TRUE( isEqual( 0.5, point.getY( ) ) );
  ASSERT_TRUE( isEqual( -0.5, point.getZ( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование operator ==
 *        Покоординатное сравнение двух точек на равенство
 */
TEST( Point3DTest, Point3DEqual )
{
  Point3D point1;
  Point3D point2( 0.1, -10., 5. );
  ASSERT_FALSE( point1 == point2 );

  point2.set( 0.0000000000000001, 0.0000000000000001, 0.0000000000000001 );
  ASSERT_TRUE( point1 == point2 );
}
//==============================================================================
/*!
 * \brief Тестирование operator !=
 *        Покоординатное сравнение двух точек на неравенство
 */
TEST( Point3DTest, Point3DNotEqual )
{
  Point3D point1;
  Point3D point2( 0.1, -10., 5. );
  ASSERT_TRUE( point1 != point2 );

  point2.set( 0.0000000000000001, 0.0000000000000001, 0.0000000000000001 );
  ASSERT_FALSE( point1 != point2 );
}
//==============================================================================
} //! Tests
//==============================================================================

