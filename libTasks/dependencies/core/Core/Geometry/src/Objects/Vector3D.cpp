#include <Geometry/Objects/Vector3D.hpp>
// Geometry
#include <Geometry/GeometryException.hpp>
// std 
#include <limits>
//==============================================================================
namespace Geometry
{
//==============================================================================
void Vector3D::normalize()
{
  if (BasicMath::isNull(length()))
    throw GeometryFailure(GeometryEnumFailure::NullVectorIncorrectOperation);

  *this /= length();
}
//==============================================================================
Vector3D Vector3D::operator /= (double number)
{
  if (fabs(number) > std::numeric_limits<double>::epsilon())
  {
    mOX /= number;
    mOY /= number;
    mOZ /= number;
    return *this;
  }
  throw GeometryFailure(GeometryEnumFailure::DividingByZero);
}
//==============================================================================
Vector3D Vector3D::operator / (double number) const
{
  if (fabs(number) > std::numeric_limits<double>::epsilon())
    return Vector3D(mOX / number, mOY / number, mOZ / number, mAnchor);
  throw GeometryFailure(GeometryEnumFailure::DividingByZero);
}
//==============================================================================
Vector3D Vector3D::operator*=(double number) noexcept
{
  mOX *= number;
  mOY *= number;
  mOZ *= number;
  return *this;
}
//==============================================================================
double Vector3D::cosAtVector(const Vector3D& vector) const
{
  //! Считаем длину векторов
  double lenV1 = length();
  double lenV2 = vector.length();
  //! Если обе длины больше 0 высчитываем косинус угла по формуле
  if ((lenV1 > 0.) && (lenV2 > 0.))
    return *this & vector / lenV1 / lenV2;
  //! Если какой либо из векторов имеет нулевую длину, считаем что угол равен
  //! 0 градусов и возвращаем соответствующий косинус
  return 1.;
}
//==============================================================================
bool Vector3D::isParallel(const Vector3D& vector) const
{
  /*!
   * Ненулевые векторы параллельны, если абсолютное значение отношения
   * их скалярного произведения к произведению их длин
   * равно 1.
   */
  if ((BasicMath::isNull(length())) || (BasicMath::isNull(vector.length())))
    throw GeometryFailure(GeometryEnumFailure::NullVectorIncorrectOperation);

  if (fabs(fabs(unit() & vector.unit()) - 1.) 
      <  std::numeric_limits<double>::epsilon())
    return true;
  return false;
}
//==============================================================================
void Vector3D::rotateCorkscrew(const Vector3D& axis, const Radian& angle)
{
  //! Выполняем поворот вектора без изменения точки привязки
  rotateCorkscrewWithoutFulcrum(axis, angle);

  if (axis.mAnchor != mAnchor)
  { //! Если точки привязки вращаемого вектора и оси вращения не совпадают

    //! Собираем вектор из точек привязки
    //! от привязки оси вращения до привязки вращаемого вектора
    Vector3D vFulcrum(axis.mAnchor, mAnchor);

    //! Вращаем полученный вектор вокруг заданной оси
    vFulcrum.rotateCorkscrewWithoutFulcrum(axis, angle);

    //! Новая точка привязки текущего вектора после поворота лежит на конце
    //! полученного после поворота вектора
    mAnchor = vFulcrum.destination();
  }
}
//==============================================================================
void Vector3D::rotateCorkscrewWithoutFulcrum(const Vector3D& axis, 
                                             const Radian& angle)
{
  //! Если вектор параллелен оси вращения, то ничего не делаем
  if (isParallel(axis))
    return;

  Vector3D vE = axis.unit();    //!< единичный вектор, задающий ось вращения
  double len(length());         //!< длина вращаемого (текущего) вектора 
  double lenProj((*this) & vE); //!< длина проекции вектора на ось вращения
  
  //! Плоскостью вращения - плоскость _|_-ная оси вращения  
  //! Длина проекции вращаемого вектора на плоскость вращения
  double lenRotary(sqrt(len * len - lenProj * lenProj));
    
  //! Готовим вектора для поворота 

  //! При вращении вектора его компонента по оси вращения остается неизменной,
  //! а оставшаяся часть сохраняет длину, но изменяет направление
  
  //! Компонента вращаемого вектора по оси вращения
  Vector3D vAxis = vE * lenProj;    
  
  //! Компонента вращаемого вектора в плоскости вращения
  Vector3D vRotary = *this - vAxis; 

  //! Готовим угол поворота
  
  Radian normA(angle.normalize());  //!< нормализованный угол поворота
  
  //! Абсолютное значение нормализованного угла поворота
  double absA(fabs(normA.get())); 
  
  int signA = normA.get() > 0 ? 1 : -1; //!< знак нормализованного угла поворота

  //! Поворачиваем на 90 градусов, столько раз, сколько позволяет угол поворота
  while (absA >= M_PI_2)
  {
    absA -= M_PI_2;
    //! Строим перпендикуляр в плоскости вращения
    vRotary = vE * vRotary * signA; 
  }

  //! Доворот на 45 градусов, если позволяет оставшийся угол поворота
  if (absA > M_PI_4)
  {
    absA -= M_PI_4;
    //! Строим биссектрису угла между вектором и перпендикуляром к нему
    vRotary = ((vE * vRotary * signA) + vRotary).unit() * lenRotary;
  }

  //! Доворот на оставшийся угол поворота на основе прямоугольного треугольника
  vRotary = (vRotary + vE * vRotary * tan(absA * signA)).unit() * lenRotary;

  //! Формируем результат: 
  //! сумма компонентов вращаемого вектора = vRotary + vAxis
  //! Но точка привязки текущего вектора не должна меняться
  mOX = vRotary.mOX + vAxis.mOX;
  mOY = vRotary.mOY + vAxis.mOY;
  mOZ = vRotary.mOZ + vAxis.mOZ;
}
//==============================================================================
} //! Geometry
//==============================================================================
