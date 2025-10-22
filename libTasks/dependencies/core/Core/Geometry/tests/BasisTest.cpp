#include <Geometry/Objects/Basis3D.hpp>

#include <Geometry/GeometryException.hpp>
//! std
#include <memory>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
class Basis3DPrivate : public Basis3D
{
  friend class Basis3DTest;
  FRIEND_TEST( Basis3DTest, CalculateParams );
};
//==============================================================================
class Basis3DTest : public ::testing::Test
{
  //==============================================================================
protected:
  //==============================================================================
  void SetUp( )
  {
    basisPrivate = std::make_shared<Basis3DPrivate>( );
  }
  //==============================================================================
  void TearDown( )
  {
  }
  //==============================================================================
  std::shared_ptr<Basis3DPrivate> basisPrivate;
  //==============================================================================
};
//==============================================================================
/*!
* \brief Тестирование конструктора по умолчанию
*/
TEST_F( Basis3DTest, DefaultConstructor )
{
  Basis3D basis;
  ASSERT_EQ( Vector3D( 1., 0., 0. ), basis.getI( ) );
  ASSERT_EQ( Point3D( ), basis.getI( ).getAnchor( ) );
  ASSERT_EQ( Vector3D( 0., 1., 0. ), basis.getJ( ) );
  ASSERT_EQ( Point3D( ), basis.getJ( ).getAnchor( ) );
  ASSERT_EQ( Vector3D( 0., 0., 1. ), basis.getK( ) );
  ASSERT_EQ( Point3D( ), basis.getK( ).getAnchor( ) );
  ASSERT_EQ( Point3D( ), basis.getZero( ) );
}
//==============================================================================
/*!
* \brief Тестирование конструктора
*/
TEST_F( Basis3DTest, Constructor )
{
  Vector3D i( Point3D( 1., 2., 3. ) );
  Vector3D j( Point3D( 1., 2., -3. ) );
  Vector3D k( Point3D( 1., -2., 3. ) );
  Point3D zero( 1., 1., 1. );
  Basis3D basis( i, j, k, zero );
  ASSERT_EQ( i, basis.getI( ) );
  ASSERT_EQ( zero, basis.getI( ).getAnchor( ) );
  ASSERT_EQ( j, basis.getJ( ) );
  ASSERT_EQ( zero, basis.getJ( ).getAnchor( ) );
  ASSERT_EQ( k, basis.getK( ) );
  ASSERT_EQ( zero, basis.getK( ).getAnchor( ) );
  ASSERT_EQ( zero, basis.getZero( ) );
}
//==============================================================================
/*!
* \brief Тестирование копирующего конструктора
*/
TEST_F( Basis3DTest, ConstructorCopied )
{
  Vector3D i( Point3D( 1., 2., 3. ) );
  Vector3D j( Point3D( 1., 2., -3. ) );
  Vector3D k( Point3D( 1., -2., 3. ) );
  Point3D zero( 1., 1., 1. );
  Basis3D basis1( i, j, k, zero );

  Basis3D basis2( basis1 );
  ASSERT_EQ( i, basis2.getI( ) );
  ASSERT_EQ( zero, basis2.getI( ).getAnchor( ) );
  ASSERT_EQ( j, basis2.getJ( ) );
  ASSERT_EQ( zero, basis2.getJ( ).getAnchor( ) );
  ASSERT_EQ( k, basis2.getK( ) );
  ASSERT_EQ( zero, basis2.getK( ).getAnchor( ) );
  ASSERT_EQ( zero, basis2.getZero( ) );
}
//==============================================================================
/*!
* \brief Тестирование fromGlobal/toGlobal для точки
*        Перевод коордиант точки из глобального базиса в локальный и обратно
*/
TEST_F( Basis3DTest, BasisPoint )
{
  //! Базис
  Vector3D i( Point3D( 2., 1., 3. ) );
  Vector3D j( Point3D( 3., -2., 1. ) );
  Vector3D k( Point3D( 1., -3., 4. ) );
  Basis3D basis( i, j, k, Point3D( 1., 2., 3. ) );

  Point3D globalPoint( 7., 0., 7. );
  Point3D localPoint = basis.fromGlobal( globalPoint );
  ASSERT_EQ( Point3D( 36. / 42., 60. / 42., 0 ), localPoint );
  ASSERT_EQ( globalPoint, basis.toGlobal( localPoint ) );

  basis = Basis3D( );
  ASSERT_EQ( globalPoint, basis.fromGlobal( globalPoint ) );
  ASSERT_EQ( globalPoint, basis.toGlobal( globalPoint ) );
}
//==============================================================================
/*!
* \brief Тестирование fromGlobal/toGlobal для вектора
*        Перевод коордиант вектора из глобального базиса в локальный и обратно
*/
TEST_F( Basis3DTest, BasisVector )
{
  //! Базис
  Vector3D i( Point3D( 2., 1., 3. ) );
  Vector3D j( Point3D( 3., -2., 1. ) );
  Vector3D k( Point3D( 1., -3., 4. ) );
  Basis3D basis( i, j, k, Point3D( 1., 2., 3. ) );

  Vector3D globalVector( 7., 0., 7. );
  Vector3D localVector = basis.fromGlobal( globalVector );
  ASSERT_EQ( Vector3D( 2., 1., 0 ), localVector );
  ASSERT_EQ( Point3D( -48. / 42., 18. / 42., 0. ), localVector.getAnchor( ) );
  ASSERT_EQ( globalVector, basis.toGlobal( localVector ) );
  ASSERT_EQ( Point3D( ), basis.toGlobal( localVector ).getAnchor( ) );

  basis = Basis3D( );
  ASSERT_EQ( globalVector, basis.fromGlobal( globalVector ) );
  ASSERT_EQ( globalVector, basis.toGlobal( globalVector ) );
}
//==============================================================================
/*!
* \brief Тестирование calcParams
*        Вычисление параметров базиса
*/
TEST_F( Basis3DTest, CalculateParams )
{
  Vector3D i( Point3D( 2., 1., 3. ) );
  Vector3D j( Point3D( 3., -2., 1. ) );
  Vector3D k( Point3D( 1., -3., 4. ) );
  ASSERT_NO_THROW( basisPrivate->set( i, j, k, Point3D( 1., 2., 3. ) ););
  ASSERT_EQ( Vector3D( -5. / -42., -11. / -42., -7. / -42. ), basisPrivate->mTMi );
  ASSERT_EQ( Vector3D( -13. / -42., 5. / -42., 7. / -42. ), basisPrivate->mTMj );
  ASSERT_EQ( Vector3D( 7. / -42., 7. / -42., -7. / -42. ), basisPrivate->mTMk );
  ASSERT_EQ( Point3D( 48. / -42., -18. / -42., 0. ), basisPrivate->mZero );

  //! Система линейно зависимых векторов.
  i = Vector3D( Point3D( 1., -2., 3. ) );
  j = Vector3D( Point3D( -1., 2., -3. ) );
  k = Vector3D( Point3D( 1., -2., 3. ) );
  ASSERT_THROW(basisPrivate->set(i, j, k, Point3D(1., 2., 3.)),
               GeometryFailure);
  try
  {
    basisPrivate->set( i, j, k, Point3D( 1., 2., 3. ) );
  }
  catch( const GeometryFailure& exception )
  {
    ASSERT_EQ( GeometryEnumFailure::NotABasis, exception.getCode( ) );
    ASSERT_EQ( geomLibTitle, exception.getLib( ) );
    ASSERT_EQ(
      "[Core::Geometry] <NotABasis>: "
      "Попытка задания системы линейно зависимых векторов в качестве базиса"
      , std::string( exception.what( ) ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование operator ==
*        Сравнение базисов
*/
TEST_F( Basis3DTest, BasisEqual )
{
  Vector3D i( Point3D( 2., 1., 3. ) );
  Vector3D j( Point3D( 3., -2., 1. ) );
  Vector3D k( Point3D( 1., -3., 4. ) );
  Basis3D basis1( i, j, k, Point3D( 1., 2., 3. ) );

  Basis3D basis2( basis1 );
  Basis3D basis3;

  ASSERT_TRUE( basis1 == basis2 );
  ASSERT_FALSE( basis1 == basis3 );
}
//==============================================================================
} //! Tests
} //! Geometry
//==============================================================================