#include <Geometry/Operations/TransitionBases.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
TransitionBases::TransitionBases(const Basis3D& initBasis, 
                                 const Basis3D& goalBasis)
: mInitBasis(initBasis)
, mGoalBasis(goalBasis)
, mFTMi(Vector3D())
, mFTMj(Vector3D())
, mFTMk(Vector3D())
, mITMi(Vector3D())
, mITMj(Vector3D())
, mITMk(Vector3D())
{
  calcParams();
}
//==============================================================================
Point3D TransitionBases::forward(const Point3D& point)
{
  //! вектор по проекциям точки сдвига
  Vector3D vShift(point.getX() - mInitBasis.mZero.getX(),
                  point.getY() - mInitBasis.mZero.getY(), 
                  point.getZ() - mInitBasis.mZero.getZ());

  return Point3D((mFTMi & vShift) + mGoalBasis.mZero.getX(),
                 (mFTMj & vShift) + mGoalBasis.mZero.getY(),
                 (mFTMk & vShift) + mGoalBasis.mZero.getZ());
}
//==============================================================================
Vector3D TransitionBases::forward(const Vector3D& vector)
{
  return Vector3D(mFTMi & vector, mFTMj & vector, mFTMk & vector,
                  forward(vector.getAnchor()));
}
//==============================================================================
Point3D TransitionBases::inverse(const Point3D& point)
{
  //! вектор по проекциям точки сдвига
  Vector3D vShift(point.getX() - mGoalBasis.mZero.getX(),
                  point.getY() - mGoalBasis.mZero.getY(),
                  point.getZ() - mGoalBasis.mZero.getZ());

  return Point3D((mITMi & vShift) + mInitBasis.mZero.getX(),
                 (mITMj & vShift) + mInitBasis.mZero.getY(),
                 (mITMk & vShift) + mInitBasis.mZero.getZ());
}
//==============================================================================
Geometry::Vector3D TransitionBases::inverse(const Vector3D& vector)
{
  return Vector3D(mITMi & vector, mITMj & vector, mITMk & vector,
                  inverse(vector.getAnchor()));
}
//==============================================================================
void TransitionBases::setBases(const Basis3D& initBasis, 
                               const Basis3D& goalBasis)
{
  mInitBasis = initBasis;
  mGoalBasis = goalBasis;
  mFTMi = Vector3D();
  mFTMj = Vector3D();
  mFTMk = Vector3D();
  mITMi = Vector3D();
  mITMj = Vector3D();
  mITMk = Vector3D();
  calcParams();
}
//==============================================================================
void TransitionBases::calcParams()
{
  //! Формируем матрицу перехода от исходного к целевому базису
  mFTMi = Vector3D(mGoalBasis.mTMi & mInitBasis.mI,
                   mGoalBasis.mTMi & mInitBasis.mJ,
                   mGoalBasis.mTMi & mInitBasis.mK);
  mFTMj = Vector3D(mGoalBasis.mTMj & mInitBasis.mI,
                   mGoalBasis.mTMj & mInitBasis.mJ,
                   mGoalBasis.mTMj & mInitBasis.mK);
  mFTMk = Vector3D(mGoalBasis.mTMk & mInitBasis.mI,
                   mGoalBasis.mTMk & mInitBasis.mJ,
                   mGoalBasis.mTMk & mInitBasis.mK);

  //! Формируем матрицу перехода от целевого к исходному базису
  mITMi = Vector3D(mInitBasis.mTMi & mGoalBasis.mI,
                   mInitBasis.mTMi & mGoalBasis.mJ,
                   mInitBasis.mTMi & mGoalBasis.mK);
  mITMj = Vector3D(mInitBasis.mTMj & mGoalBasis.mI,
                   mInitBasis.mTMj & mGoalBasis.mJ,
                   mInitBasis.mTMj & mGoalBasis.mK);
  mITMk = Vector3D(mInitBasis.mTMk & mGoalBasis.mI,
                   mInitBasis.mTMk & mGoalBasis.mJ,
                   mInitBasis.mTMk & mGoalBasis.mK);
}
//==============================================================================
} //! Geometry
//==============================================================================
