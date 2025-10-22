#include <Projection/Orthographic/OrthographicInner.hpp>

#include <Geodesic/Helper/Ellipsoid.hpp>
#include <Geodesic/GeodesicException.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! Geometry
#include <Geometry/Objects/Basis3D.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
class OrthographicInnerPrivate : public OrthographicInner
{
  //==============================================================================
  friend class OrthographicInnerTest;
  //==============================================================================
  FRIEND_TEST( OrthographicInnerTest, Constructor );
  FRIEND_TEST( OrthographicInnerTest, GetSetCenter );
  //==============================================================================
public:
  //==============================================================================
  OrthographicInnerPrivate(
    const std::shared_ptr<Ellipsoid>& ellipsoid,
    const GeoPoint3D& center = GeoPoint3D( Radian( 0. ), Radian( 0. ), 0.f ),
    const Radian& azimuth = Radian( 0. ) )
    : OrthographicInner( ellipsoid, center, azimuth )
  {
  }
  //==============================================================================
};
//==============================================================================
class OrthographicInnerTest : public ::testing::Test
{
  //==============================================================================
protected:
  //==============================================================================
  void SetUp( )
  {
  }
  //==============================================================================
  void TearDown( )
  {
  }
  //==============================================================================
  std::shared_ptr<OrthographicInnerPrivate> orthoPrivate;
  //==============================================================================
};
//==============================================================================
/*!
 * \brief Тестирование конструктора
*/
TEST_F( OrthographicInnerTest, Constructor )
{
  using namespace BasicMath;

  //! Вспомогательные переменные
  std::shared_ptr<Ellipsoid> ellipsoid = std::make_shared<Ellipsoid>( EL_WGS_84 );
  double aaDEVbb( ellipsoid->getMajorRadius( ) * ellipsoid->getMajorRadius( )
    / ellipsoid->getMinorRadius( ) / ellipsoid->getMinorRadius( ) );
  double invSqrt2( 1. / sqrt( 2. ) );

  //! Проекции с центром на экваторе

  //! Создание проекции с центром в точке (0°, 45°)
  orthoPrivate = std::make_shared<OrthographicInnerPrivate>(
    ellipsoid, GeoPoint3D( Radian( 0. ), Radian( M_PI_4 ) ) );
  //! Проверка базиса
  ASSERT_EQ( Geometry::Vector3D( -invSqrt2, invSqrt2, 0. ),
    orthoPrivate->mBasis->getI( ) );
  ASSERT_EQ( Geometry::Vector3D( 0., 0., 1. ),
    orthoPrivate->mBasis->getJ( ) );
  ASSERT_EQ( Geometry::Vector3D( invSqrt2, invSqrt2, 0. ),
    orthoPrivate->mBasis->getK( ) );
  //! Проверка коэффициентов
  ASSERT_TRUE( isEqual( 1., orthoPrivate->mTgYX ) );
  ASSERT_TRUE( isEqual( 2., orthoPrivate->mCosYX ) );

  //! Создание проекции с центром в точке (0°, 60°)
  orthoPrivate = std::make_shared<OrthographicInnerPrivate>(
    ellipsoid, GeoPoint3D( Radian( 0. ), Radian( M_PI / 3 ) ) );
  //! Проверка базиса
  ASSERT_EQ( Geometry::Vector3D( -sqrt( 3. ) / 2., 1. / 2., 0. ),
    orthoPrivate->mBasis->getI( ) );
  ASSERT_EQ( Geometry::Vector3D( 0., 0., 1. ),
    orthoPrivate->mBasis->getJ( ) );
  ASSERT_EQ( Geometry::Vector3D( 1. / 2., sqrt( 3. ) / 2., 0. ),
    orthoPrivate->mBasis->getK( ) );
  //! Проверка коэффициентов
  ASSERT_TRUE( isEqual( sqrt( 3. ), orthoPrivate->mTgYX ) );
  ASSERT_TRUE( isEqual( 4., orthoPrivate->mCosYX ) );

  //! Проекции с центром на полюсе

  //! Создание проекции с центром в точке (-90°, 0°)
  orthoPrivate = std::make_shared<OrthographicInnerPrivate>(
    ellipsoid, GeoPoint3D( Radian( -M_PI_2 ), Radian( 0. ) ) );
  //! Проверка базиса
  ASSERT_EQ( Geometry::Vector3D( 1., 0., 0. ), orthoPrivate->mBasis->getI( ) );
  ASSERT_EQ( Geometry::Vector3D( 0., -1., 0. ), orthoPrivate->mBasis->getJ( ) );
  ASSERT_EQ( Geometry::Vector3D( 0., 0., -1. ), orthoPrivate->mBasis->getK( ) );
  //! Проверка коэффициентов
  ASSERT_TRUE( isEqual( 0., orthoPrivate->mTgZKxz ) );
  ASSERT_TRUE( isEqual( 0., orthoPrivate->mTgZKxy ) );
  ASSERT_TRUE( isEqual( aaDEVbb, orthoPrivate->mTgZK ) );

  //! Создание проекции с центром в точке (90°, 0°)
  orthoPrivate = std::make_shared<OrthographicInnerPrivate>(
    ellipsoid, GeoPoint3D( Radian( M_PI_2 ), Radian( 0. ) ) );
  //! Проверка базиса
  ASSERT_EQ( Geometry::Vector3D( 1., 0., 0. ), orthoPrivate->mBasis->getI( ) );
  ASSERT_EQ( Geometry::Vector3D( 0., 1., 0. ), orthoPrivate->mBasis->getJ( ) );
  ASSERT_EQ( Geometry::Vector3D( 0., 0., 1. ), orthoPrivate->mBasis->getK( ) );
  //! Проверка коэффициентов
  ASSERT_TRUE( isEqual( 0., orthoPrivate->mTgZKxz ) );
  ASSERT_TRUE( isEqual( 0., orthoPrivate->mTgZKxy ) );
  ASSERT_TRUE( isEqual( aaDEVbb, orthoPrivate->mTgZK ) );

  //! Дополнительные проверки

  //! Создание проекции с центром в точке (45°, 45°)
  orthoPrivate = std::make_shared<OrthographicInnerPrivate>(
    ellipsoid, GeoPoint3D( Radian( M_PI_4 ), Radian( M_PI_4 ) ) );

  //! Проверка базиса
  ASSERT_EQ( Geometry::Vector3D( -0.70710678118654746, 0.70710678118654746, 0. ),
    orthoPrivate->mBasis->getI( ) );
  ASSERT_EQ( Geometry::Vector3D( -0.49831797407719369, -0.49831797407719369,
    0.70947754962592213 ),
    orthoPrivate->mBasis->getJ( ) );
  ASSERT_EQ( Geometry::Vector3D( 0.50167638644010482, 0.50167638644010482,
    0.70472803731425171 ),
    orthoPrivate->mBasis->getK( ) );
  //! Проверка коэффициентов
  ASSERT_TRUE( isEqual( 0.71187232503479592, orthoPrivate->mTgZKxz ) );
  ASSERT_TRUE( isEqual( 0.71187232503479592, orthoPrivate->mTgZKxy ) );
  ASSERT_TRUE( isEqual( 2.02026391104316883, orthoPrivate->mTgZK ) );

  //! Создание проекции с центром в точке (0°, 45°) и азимутом 45°
  orthoPrivate = std::make_shared<OrthographicInnerPrivate>(
    ellipsoid, GeoPoint3D( Radian( 0 ), Radian( M_PI_4 ) ), Radian( M_PI_4 ) );
  //! Проверка базиса
  ASSERT_EQ( Geometry::Vector3D( -1. / 2., 1. / 2., -invSqrt2 ),
    orthoPrivate->mBasis->getI( ) );
  ASSERT_EQ( Geometry::Vector3D( -1. / 2., 1. / 2., invSqrt2 ),
    orthoPrivate->mBasis->getJ( ) );
  ASSERT_EQ( Geometry::Vector3D( invSqrt2, invSqrt2, 0. ),
    orthoPrivate->mBasis->getK( ) );
  //! Проверка коэффициентов
  ASSERT_TRUE( isEqual( 1., orthoPrivate->mTgYX ) );
  ASSERT_TRUE( isEqual( 2., orthoPrivate->mCosYX ) );
}
//==============================================================================
TEST_F( OrthographicInnerTest, GetSetEllipsoid )
{
  std::shared_ptr<Ellipsoid> ellWGS = std::make_shared<Ellipsoid>( EL_WGS_84 );
  std::shared_ptr<Ellipsoid> ellKras = std::make_shared<Ellipsoid>( EL_KRASOVSKY );

  //! Проекция на эллипсоиде WGS-84 
  orthoPrivate = std::make_shared<OrthographicInnerPrivate>( ellWGS );
  ASSERT_EQ( ellWGS.get( ), orthoPrivate->getEllipsoid( ).get( ) );

  //! Переход на эллипсоид Красовского
  orthoPrivate->setEllipsoid( ellKras );
  ASSERT_EQ( ellKras.get( ), orthoPrivate->getEllipsoid( ).get( ) );
}
//==============================================================================
TEST_F( OrthographicInnerTest, GetSetCenter )
{
  using namespace BasicMath;

  //! Создание проекции с центром в точке (0°, 60°)
  orthoPrivate = std::make_shared<OrthographicInnerPrivate>(
    std::make_shared<Ellipsoid>( EL_WGS_84 ),
    GeoPoint3D( Radian( 0. ), Radian( M_PI / 3 ) ) );
  //! Проверка базиса
  ASSERT_EQ( Geometry::Vector3D( -sqrt( 3. ) / 2., 1. / 2., 0. ),
    orthoPrivate->mBasis->getI( ) );
  ASSERT_EQ( Geometry::Vector3D( 0., 0., 1. ),
    orthoPrivate->mBasis->getJ( ) );
  ASSERT_EQ( Geometry::Vector3D( 1. / 2., sqrt( 3. ) / 2., 0. ),
    orthoPrivate->mBasis->getK( ) );
  //! Проверка коэффициентов
  ASSERT_TRUE( isEqual( sqrt( 3. ), orthoPrivate->mTgYX ) );
  ASSERT_TRUE( isEqual( 4., orthoPrivate->mCosYX ) );
  //! Проверка центра и азимута
  ASSERT_EQ( GeoPoint3D( Radian( 0. ), Radian( M_PI / 3 ) ),
    orthoPrivate->getCenterProjection( ) );
  ASSERT_EQ( Radian( 0. ), orthoPrivate->getAzimuth( ) );

  //! Смена центра и азимута
  orthoPrivate->setProjectionPoint( GeoPoint3D( Radian( 0 ), Radian( M_PI_4 ) ),
    Radian( M_PI_4 ) );
  //! Проверка базиса
  ASSERT_EQ( Geometry::Vector3D( -1. / 2., 1. / 2., -1. / sqrt( 2. ) ),
    orthoPrivate->mBasis->getI( ) );
  ASSERT_EQ( Geometry::Vector3D( -1. / 2., 1. / 2., 1. / sqrt( 2. ) ),
    orthoPrivate->mBasis->getJ( ) );
  ASSERT_EQ( Geometry::Vector3D( 1. / sqrt( 2. ), 1. / sqrt( 2. ), 0. ),
    orthoPrivate->mBasis->getK( ) );
  //! Проверка коэффициентов
  ASSERT_TRUE( isEqual( 1., orthoPrivate->mTgYX ) );
  ASSERT_TRUE( isEqual( 2., orthoPrivate->mCosYX ) );
  //! Проверка центра и азимута
  ASSERT_EQ( GeoPoint3D( Radian( 0. ), Radian( M_PI_4 ) ),
    orthoPrivate->getCenterProjection( ) );
  ASSERT_EQ( Radian( M_PI_4 ), orthoPrivate->getAzimuth( ) );
}
//==============================================================================
TEST_F( OrthographicInnerTest, ToFromProjection )
{
  using namespace BasicMath;

  //! Эллипсоид WGS
  std::shared_ptr<Ellipsoid> ellipsoid = std::make_shared<Ellipsoid>( EL_WGS_84 );

  //! Вспомогательные переменные
  double dx( 20000. );
  double dy( 70000. );
  double ellA( ellipsoid->getMajorRadius( ) );
  double ellB( ellipsoid->getMinorRadius( ) );

  //! Контрольные точки
  Point3D pBase( dx, dy );
  Point3D pFail( ellipsoid->getMajorRadius( ), dy );
  Point3D pVBorder( sqrt( ellA * ellA - ellA * ellA / ellB / ellB * dy * dy ), dy );
  Point3D pSBorder( ellA / sqrt( 1. + ellA * ellA / ellB / ellB ) * sqrt( 2. ), 0. );
  Point3D pStrange( -.85 * ellA, dy );
  Point3D pSkew( 3830000., 3830000. );

  //! Центры проекции
  GeoPoint3D gpNorthPole( Radian( M_PI_2 ), Radian( 0. ) );
  GeoPoint3D gpVerticalPi4( Radian( 0. ), Radian( M_PI_4 ) );
  GeoPoint3D gpVerticalPi2( Radian( 0. ), Radian( M_PI_2 ) );
  GeoPoint3D gpVerticalInvPi2( Radian( 0. ), Radian( -M_PI_2 ) );
  GeoPoint3D gpSkewPi4( Radian( M_PI_4 ), Radian( M_PI_4 ) );

  //! Вспомогательные точки
  Point3D pResult;
  GeoPoint3D gpVisible;
  GeoPoint3D gpInvisible;

  //! Проекция на эллипсоиде WGS-84 
  orthoPrivate = std::make_shared<OrthographicInnerPrivate>( ellipsoid );

  //! Точка проекции на полюсе
  orthoPrivate->setProjectionPoint( gpNorthPole );
  //! Проверка, что центр проекции попал в (0, 0)
  pResult = orthoPrivate->toProjection( gpNorthPole );
  ASSERT_TRUE( isEqual( 0., pResult.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( 0., pResult.getY( ), 1. ) );
  //! Проверка перевода точки из проекции и обратно
  ASSERT_TRUE( orthoPrivate->fromProjection( pBase, gpVisible, gpInvisible ) );
  pResult = orthoPrivate->toProjection( gpVisible );
  ASSERT_TRUE( isEqual( pBase.getX( ), pResult.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( pBase.getY( ), pResult.getY( ), 1. ) );

  //! 1. Вертикальна произвольная проекция
  orthoPrivate->setProjectionPoint( gpVerticalPi4 );
  //! Проверка, что центр проекции попал в (0, 0)
  pResult = orthoPrivate->toProjection( gpVerticalPi4 );
  ASSERT_TRUE( isEqual( 0., pResult.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( 0., pResult.getY( ), 1. ) );
  //! Проверка перевода точки из проекции и обратно
  //! 1.1 Точка (20000., 70000.)
  ASSERT_TRUE( orthoPrivate->fromProjection( pBase, gpVisible, gpInvisible,
    false ) );
  ASSERT_TRUE( gpVisible.getLongitude( ) > 0 );
  ASSERT_TRUE( gpInvisible.getLongitude( ) < 0 );
  pResult = orthoPrivate->toProjection( gpVisible );
  ASSERT_TRUE( isEqual( pBase.getX( ), pResult.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( pBase.getY( ), pResult.getY( ), 1. ) );
  //! 1.2 Точка за пределами проекции эллипсоида
  ASSERT_FALSE(orthoPrivate->fromProjection(pFail, gpVisible,
                                            gpInvisible, false));
  //! 1.3 Точка на границе
  ASSERT_TRUE( orthoPrivate->fromProjection( pVBorder, gpVisible, gpInvisible,
    false ) );
  ASSERT_EQ( gpVisible, gpInvisible );
  pResult = orthoPrivate->toProjection( gpVisible );
  ASSERT_TRUE( isEqual( pVBorder.getX( ), pResult.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( pVBorder.getY( ), pResult.getY( ), 1. ) );
  //! 1.4 Точка с противоположным выбором корней (b<0 && fabs(b)>sd)
  ASSERT_TRUE( orthoPrivate->fromProjection( pStrange, gpVisible, gpInvisible,
    false ) );
  ASSERT_TRUE( fabs( gpVisible.getLongitude( ).get( ) )
    < fabs( gpInvisible.getLongitude( ).get( ) ) );
  pResult = orthoPrivate->toProjection( gpVisible );
  ASSERT_TRUE( isEqual( pStrange.getX( ), pResult.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( pStrange.getY( ), pResult.getY( ), 1. ) );

  //! 2. Вертикальная проекция в восточном полушарии, но х0 = 0 
  orthoPrivate->setProjectionPoint( gpVerticalPi2 );
  //! Проверка, что центр проекции попал в (0, 0)
  pResult = orthoPrivate->toProjection( gpVerticalPi2 );
  ASSERT_TRUE( isEqual( 0., pResult.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( 0., pResult.getY( ), 1. ) );
  //! Проверка перевода точки из проекции и обратно
  //! 2.1 Точка (20000., 70000.)
  ASSERT_TRUE( orthoPrivate->fromProjection( pBase, gpVisible, gpInvisible,
    false ) );
  ASSERT_TRUE( gpVisible.getLongitude( ) > 0 );
  ASSERT_TRUE( gpInvisible.getLongitude( ) < 0 );
  pResult = orthoPrivate->toProjection( gpVisible );
  ASSERT_TRUE( isEqual( pBase.getX( ), pResult.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( pBase.getY( ), pResult.getY( ), 1. ) );
  //! 2.2 Точка за пределами эллипсоида
  ASSERT_FALSE(orthoPrivate->fromProjection(pFail, gpVisible,
                                            gpInvisible, false));
  //! 2.3 Точка на границе
  ASSERT_TRUE( orthoPrivate->fromProjection( pVBorder, gpVisible, gpInvisible,
    false ) );
  ASSERT_EQ( gpVisible, gpInvisible );
  pResult = orthoPrivate->toProjection( gpVisible );
  ASSERT_TRUE( isEqual( pVBorder.getX( ), pResult.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( pVBorder.getY( ), pResult.getY( ), 1. ) );

  //! 2'. Вертикальная проекция в западном полушарии, но х0 = 0 
  orthoPrivate->setProjectionPoint( gpVerticalInvPi2 );
  //! Проверка, что центр проекции попал в (0, 0)
  pResult = orthoPrivate->toProjection( gpVerticalInvPi2 );
  ASSERT_TRUE( isEqual( 0., pResult.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( 0., pResult.getY( ), 1. ) );
  //! Проверка перевода точки из проекции и обратно
  //! 2'.1 Точка (20000., 70000.)
  ASSERT_TRUE( orthoPrivate->fromProjection( pBase, gpVisible, gpInvisible,
    false ) );
  ASSERT_TRUE( gpVisible.getLongitude( ) < 0 );
  ASSERT_TRUE( gpInvisible.getLongitude( ) > 0 );
  pResult = orthoPrivate->toProjection( gpVisible );
  ASSERT_TRUE( isEqual( pBase.getX( ), pResult.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( pBase.getY( ), pResult.getY( ), 1. ) );

  //! 3. Косая проекция
  orthoPrivate->setProjectionPoint( gpSkewPi4 );
  //! Проверка, что центр проекции попал в (0, 0)
  pResult = orthoPrivate->toProjection( gpSkewPi4 );
  ASSERT_TRUE( isEqual( 0., pResult.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( 0., pResult.getY( ), 1. ) );
  //! Проверка перевода точки из проекции и обратно
  //! 3.1 Точка (20000., 70000.)
  ASSERT_TRUE( orthoPrivate->fromProjection( pBase, gpVisible, gpInvisible,
    false ) );
  ASSERT_TRUE( gpVisible.getLongitude( ) > 0 );
  ASSERT_TRUE( gpInvisible.getLongitude( ) < 0 );
  pResult = orthoPrivate->toProjection( gpVisible );
  ASSERT_TRUE( isEqual( pBase.getX( ), pResult.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( pBase.getY( ), pResult.getY( ), 1. ) );
  //! 3.2 Точка за пределами эллипсоида
  ASSERT_FALSE(orthoPrivate->fromProjection(pFail, gpVisible,
                                            gpInvisible, false));
  //! 3.3 Точка на границе
  ASSERT_TRUE( orthoPrivate->fromProjection( pSBorder, gpVisible, gpInvisible,
    false ) );
  pResult = orthoPrivate->toProjection( gpVisible );
  Point3D pResultInv = orthoPrivate->toProjection( gpInvisible );
  ASSERT_TRUE( isEqual( pResult.getX( ), pResultInv.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( pResult.getY( ), pResultInv.getY( ), 1. ) );
  pResult = orthoPrivate->toProjection( gpVisible );
  ASSERT_TRUE( isEqual( pSBorder.getX( ), pResult.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( pSBorder.getY( ), pResult.getY( ), 1. ) );
  //! 3.4 Точка с противоположным выбором корней (b<0 && fabs(b)>sd)
  ASSERT_TRUE( orthoPrivate->fromProjection( pSkew, gpVisible, gpInvisible,
    false ) );
  pResult = orthoPrivate->toProjection( gpVisible );
  ASSERT_TRUE( isEqual( pSkew.getX( ), pResult.getX( ), 1. ) );
  ASSERT_TRUE( isEqual( pSkew.getY( ), pResult.getY( ), 1. ) );
}
//==============================================================================
} //! Tests
} //! Geodesic
//==============================================================================
