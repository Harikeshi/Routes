#include <Geometry/Operations/TransitionBases.hpp>
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
class TransitionBasesPrivate : public TransitionBases
{
  //==============================================================================
public:
  //==============================================================================
  TransitionBasesPrivate( const Basis3D& initBasis,
    const Basis3D& goalBasis )
    : TransitionBases( initBasis, goalBasis )
  {
  }
  //==============================================================================
protected:
  //==============================================================================
  friend class TransitionBasesTest;
  FRIEND_TEST( TransitionBasesTest, CalculateParams );
  //==============================================================================
};
//==============================================================================
class TransitionBasesTest : public ::testing::Test
{
  //==============================================================================
protected:
  //==============================================================================
  void SetUp( )
  {
    Basis3D initBasis;

    Vector3D i( Point3D( 2., 1., 3. ) );
    Vector3D j( Point3D( 3., -2., 1. ) );
    Vector3D k( Point3D( 1., -3., 4. ) );
    Basis3D goalBasis( i, j, k, Point3D( 1., 2., 3. ) );

    transitionPrivate = std::make_shared<TransitionBasesPrivate>( initBasis,
      goalBasis );
  }
  //==============================================================================
  void TearDown( )
  {
  }
  //==============================================================================
  std::shared_ptr<TransitionBasesPrivate> transitionPrivate;
};
//==============================================================================
/*!
* \brief Тестирование конструктора класса
*/
TEST_F( TransitionBasesTest, Constructor )
{
  Basis3D initBasis;

  Vector3D i( Point3D( 2., 1., 3. ) );
  Vector3D j( Point3D( 3., -2., 1. ) );
  Vector3D k( Point3D( 1., -3., 4. ) );
  Basis3D goalBasis( i, j, k, Point3D( 1., 2., 3. ) );

  TransitionBases transition( initBasis, goalBasis );

  ASSERT_EQ( initBasis, transition.getInitBasis( ) );
  ASSERT_EQ( goalBasis, transition.getGoalBasis( ) );
}
//==============================================================================
/*!
* \brief Тестирование перевода координат точки
*/
TEST_F( TransitionBasesTest, TransitionPoint )
{
  Basis3D initBasis;

  Vector3D i( Point3D( 2., 1., 3. ) );
  Vector3D j( Point3D( 3., -2., 1. ) );
  Vector3D k( Point3D( 1., -3., 4. ) );
  Basis3D goalBasis( i, j, k, Point3D( 1., 2., 3. ) );

  TransitionBases transition( initBasis, goalBasis );

  Point3D initPoint( 7., 0., 7. );
  Point3D goalPoint = transition.forward( initPoint );
  ASSERT_EQ( Point3D( 36. / 42., 60. / 42., 0 ), goalPoint );
  ASSERT_EQ( initPoint, transition.inverse( goalPoint ) );

  i = Vector3D( Point3D( 5., 3., 1. ) );
  j = Vector3D( Point3D( -2., -1., 4. ) );
  k = Vector3D( Point3D( 7., 1., 2 ) );
  initBasis = Basis3D( i, j, k, Point3D( 2., 3., 1. ) );

  transition.setBases( initBasis, goalBasis );

  goalPoint = transition.forward( initPoint );
  ASSERT_EQ( Point3D( 62267. / 2982., 58717. / 2982., -47215. / 2982. ),
    goalPoint );
  ASSERT_EQ( initPoint, transition.inverse( goalPoint ) );
}
//==============================================================================
/*!
* \brief Тестирование перевода координат вектора
*/
TEST_F( TransitionBasesTest, TransitionVector )
{
  Basis3D initBasis;

  Vector3D i( Point3D( 2., 1., 3. ) );
  Vector3D j( Point3D( 3., -2., 1. ) );
  Vector3D k( Point3D( 1., -3., 4. ) );
  Basis3D goalBasis( i, j, k, Point3D( 1., 2., 3. ) );

  TransitionBases transition( initBasis, goalBasis );

  Vector3D initVector( 7., 0., 7. );
  Vector3D goalVector = transition.forward( initVector );
  ASSERT_EQ( Vector3D( 2., 1., 0 ), goalVector );
  ASSERT_EQ( Point3D( -48. / 42., 18. / 42., 0. ), goalVector.getAnchor( ) );
  ASSERT_EQ( initVector, transition.inverse( goalVector ) );
  ASSERT_EQ( Point3D( ), transition.inverse( goalVector ).getAnchor( ) );

  i = Vector3D( Point3D( 5., 3., 1. ) );
  j = Vector3D( Point3D( -2., -1., 4. ) );
  k = Vector3D( Point3D( 7., 1., 2 ) );
  initBasis = Basis3D( i, j, k, Point3D( 2., 3., 1. ) );

  transition.setBases( initBasis, goalBasis );

  goalVector = transition.forward( initVector );
  ASSERT_EQ( Vector3D( 62125., 57155., -45227 ) / 2982., goalVector );
  ASSERT_EQ( Point3D( 142. / 2982., 1562. / 2982., -1988. / 2982. ),
    goalVector.getAnchor( ) );
  ASSERT_EQ( initVector, transition.inverse( goalVector ) );
  ASSERT_EQ( Point3D( ), transition.inverse( goalVector ).getAnchor( ) );
}
//==============================================================================
/*!
* \brief Тестирование вычисления матриц перехода
*/
TEST_F( TransitionBasesTest, CalculateParams )
{
  ASSERT_EQ( Vector3D( 5. / 42., 11. / 42., 7. / 42. ),
    transitionPrivate->mFTMi );
  ASSERT_EQ( Vector3D( 13. / 42., -5. / 42., -7. / 42. ),
    transitionPrivate->mFTMj );
  ASSERT_EQ( Vector3D( -7. / 42., -7. / 42., 7. / 42. ),
    transitionPrivate->mFTMk );
  ASSERT_EQ( Vector3D( 2., 3., 1. ), transitionPrivate->mITMi );
  ASSERT_EQ( Vector3D( 1., -2., -3. ), transitionPrivate->mITMj );
  ASSERT_EQ( Vector3D( 3., 1., 4. ), transitionPrivate->mITMk );

  Vector3D i( Point3D( 5., 3., 1. ) );
  Vector3D j( Point3D( -2., -1., 4. ) );
  Vector3D k( Point3D( 7., 1., 2. ) );
  Basis3D initBasis( i, j, k, Point3D( 2., 3., 1. ) );

  i = Vector3D( Point3D( 2., 1., 3. ) );
  j = Vector3D( Point3D( 3., -2., 1. ) );
  k = Vector3D( Point3D( 1., -3., 4. ) );
  Basis3D goalBasis( i, j, k, Point3D( 1., 2., 3. ) );

  transitionPrivate->setBases( initBasis, goalBasis );

  ASSERT_EQ( Vector3D( 4615, 497., 4260. ) / 2982., transitionPrivate->mFTMi );
  ASSERT_EQ( Vector3D( 3053., -3479., 5112. ) / 2982., transitionPrivate->mFTMj );
  ASSERT_EQ( Vector3D( -3479., 3479., -2982. ) / 2982., transitionPrivate->mFTMk );

  ASSERT_EQ( Vector3D( 1470., -3234., -3444. ) / 2982., transitionPrivate->mITMi );
  ASSERT_EQ( Vector3D( 1722., -210., 2100. ) / 2982., transitionPrivate->mITMj );
  ASSERT_EQ( Vector3D( 294., 3528., 3486. ) / 2982., transitionPrivate->mITMk );
}
//==============================================================================
} //! Tests
} //! Geometry
//==============================================================================