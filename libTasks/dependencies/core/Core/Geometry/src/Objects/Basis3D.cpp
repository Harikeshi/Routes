#include <Geometry/Objects/Basis3D.hpp>

#include <Geometry/GeometryException.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
Basis3D::Basis3D(const Vector3D& i, const Vector3D& j, const Vector3D& k, 
                 const Point3D& zero) 
: mO(zero)
, mI(i)
, mJ(j)
, mK(k)
, mTMi(Vector3D())
, mTMj(Vector3D())
, mTMk(Vector3D())
, mZero(Point3D())
{
  mI.setAnchor(zero);
  mJ.setAnchor(zero);
  mK.setAnchor(zero);

  //! Вычисление матрицы трансформации в базис, если вектора образуют базис
  calcParams();
}
//==============================================================================
void Basis3D::calcParams()
{
  //! Вычисляем определитель (детерминант) матрицы, полученной из координат 
  //! векторов, задающих базис, которые образуют столбцы матрицы перехода. 
  double minorA11(mJ.getOY() * mK.getOZ() - mJ.getOZ() * mK.getOY());
  double minorA21(mI.getOY() * mK.getOZ() - mI.getOZ() * mK.getOY());
  double minorA31(mI.getOY() * mJ.getOZ() - mI.getOZ() * mJ.getOY());

  double determinant(  mI.getOX() * minorA11
                     - mJ.getOX() * minorA21
                     + mK.getOX() * minorA31);

  //! Вектора составляют базис, если определитель матрицы не равен 0
  if (fabs(determinant) < std::numeric_limits<double>::epsilon())
    throw GeometryFailure(GeometryEnumFailure::NotABasis);

  //! Вычисляем обрратную матрицу перехода и построчно записываем её в форме 
  //! векторов
  mTMi = Vector3D( minorA11, 
                  -(mJ.getOX() * mK.getOZ() - mJ.getOZ() * mK.getOX()),
                  -(mJ.getOY() * mK.getOX() - mJ.getOX() * mK.getOY()));
  mTMj = Vector3D(-minorA21,
                   mI.getOX() * mK.getOZ() - mK.getOX() * mI.getOZ(),
                  -(mI.getOX() * mK.getOY() - mI.getOY() * mK.getOX()));
  mTMk = Vector3D( minorA31,
                  -(mI.getOX() * mJ.getOZ() - mJ.getOX() * mI.getOZ()),
                   mI.getOX() * mJ.getOY() - mI.getOY() * mJ.getOX());
  mTMi /= determinant;
  mTMj /= determinant;
  mTMk /= determinant;

  //! Вычисление координат глобального нуля в новом базисе
  Vector3D vZero(mO); //!< вектор на начало координат в локальном базисе
  mZero = Point3D((mTMi * -1.) & vZero, 
                  (mTMj * -1.) & vZero, 
                  (mTMk * -1.) & vZero);
}
//==============================================================================
Point3D Basis3D::fromGlobal(const Point3D& point) const noexcept
{
  Vector3D vPoint(point);
  return Point3D((mTMi & vPoint) + mZero.getX(),
                 (mTMj & vPoint) + mZero.getY(), 
                 (mTMk & vPoint) + mZero.getZ());
}
//==============================================================================
Geometry::Vector3D Basis3D::fromGlobal(const Vector3D& vector) const noexcept
{
  return Vector3D(mTMi & vector, mTMj & vector, mTMk & vector,
                  fromGlobal(vector.getAnchor()));
}
//==============================================================================
Point3D Basis3D::toGlobal(const Point3D& point) const noexcept
{
  //! Сдвиг относительно точки начала координат локального базиса 
  Point3D pShift(point.getX() - mZero.getX(), 
                 point.getY() - mZero.getY(),
                 point.getZ() - mZero.getZ());

  return Point3D(mI.getOX() * pShift.getX() + mJ.getOX() * pShift.getY() 
                                            + mK.getOX() * pShift.getZ(),
                 mI.getOY() * pShift.getX() + mJ.getOY() * pShift.getY() 
                                            + mK.getOY() * pShift.getZ(),
                 mI.getOZ() * pShift.getX() + mJ.getOZ() * pShift.getY() 
                                            + mK.getOZ() * pShift.getZ());
}
//==============================================================================
Geometry::Vector3D Basis3D::toGlobal(const Vector3D& vector) const noexcept
{
  return Vector3D(mI.getOX() * vector.getOX() + mJ.getOX() * vector.getOY()
    + mK.getOX() * vector.getOZ(),
    mI.getOY() * vector.getOX() + mJ.getOY() * vector.getOY()
    + mK.getOY() * vector.getOZ(),
    mI.getOZ() * vector.getOX() + mJ.getOZ() * vector.getOY()
    + mK.getOZ() * vector.getOZ());
}
//==============================================================================
} //! Geometry
//==============================================================================
