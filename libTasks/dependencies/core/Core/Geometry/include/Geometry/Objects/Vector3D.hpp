#ifndef GEOMETRY_OBJECTS_VECTOR3D_HPP
#define GEOMETRY_OBJECTS_VECTOR3D_HPP
//==============================================================================
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
#include <MathTools/AngularUnits/Radian.hpp>
//! Point
#include <Point/Point3D.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
/*!
 * \brief Вектор в трехмерном евклидовом пространстве
 *        Задается точкой приложения (началом) и тремя проекциями на оси
 *        координат.
*/
class Vector3D
{
//==============================================================================
public:
//==============================================================================
  /*!
   * \brief Конструктор класса по проекциям на оси и точке привязки
   * \param[in] ox - проекция на ось OX
   * \param[in] oy - проекция на ось OY
   * \param[in] oz - проекция на ось OZ
   * \param[in] anchor - точка приложения/привязки вектора
  */
  Vector3D(double ox = 0., double oy = 0., double oz = 0., 
           const Point3D& anchor = Point3D())
  : mOX(ox)
  , mOY(oy)
  , mOZ(oz)
  , mAnchor(anchor)
  {
  }
//==============================================================================
  /*!
   * \brief Конструктор класса по двум точкам
   * \param[in] startPoint - точка начала вектора
   * \param[in] endPoint   - точка конца вектора
  */
  Vector3D(const Point3D& startPoint, const Point3D& endPoint)
  : mOX(endPoint.getX() - startPoint.getX())
  , mOY(endPoint.getY() - startPoint.getY())
  , mOZ(endPoint.getZ() - startPoint.getZ())
  , mAnchor(startPoint)
  {
  }
//==============================================================================
  /*!
   * \brief Конструктор класса по одной точке
   * \details В качестве начала вектора берётся точка начала глобальных 
   *          координат (0, 0, 0)
   * \param[in] point - точка конца вектора
  */
  Vector3D(const Point3D& point)
  : mOX(point.getX())
  , mOY(point.getY())
  , mOZ(point.getZ())
  , mAnchor(Point3D())
  {
  }
//==============================================================================
  /*!
   * \brief Копирующий конструктор класса
   * \param[in] vector - вектор, копию которого надо создать
  */
  Vector3D(const Vector3D& vector)
  : mOX(vector.mOX)
  , mOY(vector.mOY)
  , mOZ(vector.mOZ)
  , mAnchor(vector.mAnchor)
  {
  }
//==============================================================================
  /*!
   * \brief Вычисление длины вектора
   * \return Длина вектора
  */ 
  inline double length() const
  {
    return sqrt(mOX * mOX + mOY * mOY + mOZ * mOZ);
  }
//==============================================================================
  /*!
   * \brief Приведение вектора к единичному
   * \details Деление вектора на его длину
   * \throws GeometryFailure # NullVectorIncorrectOperation
   */  
  void normalize();
//==============================================================================
  /*!
   * \brief Получение точки на конце вектора
   * \return Точка на конце вектора
  */
  inline Point3D destination() const
  {
    return Point3D(mAnchor.getX() + mOX, 
                   mAnchor.getY() + mOY, 
                   mAnchor.getZ() + mOZ);
  }
//==============================================================================
  /*!
   * \brief Получение единичного вектора
   * \return Единичный вектор, соответствующий текущему.
   * \throws GeometryFailure # DividingByZero
   */
  inline Vector3D unit() const
  {
    return (*this / length());
  }
//==============================================================================
  /*!
  * \brief Нахождение косинуса угла между векторами
  * \param[in] vector - вектор, с которым ищется косинус угла
  * \return Косинус угла.
  */
  double cosAtVector(const Vector3D& vector) const;
//==============================================================================
  /*!
   * \brief Проверка на параллельность векторов
   * \param[in] vector - вектор, с которым проверяется параллельность
   * \return true  - параллельны;
   *        false - непараллельны.
   * \throws GeometryFailure # NullVectorIncorrectOperation
   */
  bool isParallel(const Vector3D& vector) const;
//==============================================================================
  /*!
   * \brief Вращение вектора по правилу буравчика относительно заданной оси на
   *        заданный угол
   * \param[in] axis - ось, относительно которой вращать вектор
   * \param[in] angle - угол, на который требуется повернуть вектор [радиан]
   * \return Вектор, получившийся в результате вращения 
  */
  void rotateCorkscrew(const Vector3D& axis, const Radian& angle);
//==============================================================================
  /*!
   * \brief Вращение вектора по правилу буравчика относительно заданной оси на
   *        заданный угол без изменения точки приложения
   * \param[in] axis - ось, относительно которой вращать вектор
   * \param[in] angle - угол, на который требуется повернуть вектор [радиан]
   * \return Вектор, получившийся в результате вращения 
  */
  void rotateCorkscrewWithoutFulcrum(const Vector3D& axis, const Radian& angle);
//==============================================================================
  /*!
   * \brief Деление (умножение на число, обратное числу) вектора на число
   * \param[in] number - число, на которое требуется поделить вектор
   * \return результат деления вектора.
   * \throws GeometryFailure # DividingByZero
   */
  Vector3D operator /= (double number);
//==============================================================================
  /*!
   * \brief Умножение вектора на число
   * \param[in] number - число, на которое требуется умножить вектор
   * \return результат умножения вектора.
   */
  Vector3D operator *= (double number) noexcept;
//==============================================================================
  /*!
   * \brief Сложение векторов
   * \param[in] vector - вектор, который суммируется с имеющимся
   * \return Вектор, полученный в результате сложения
  */
  inline Vector3D operator + (const Vector3D& vector) const
  {
    return Vector3D(mOX + vector.mOX, mOY + vector.mOY, mOZ + vector.mOZ, 
                    mAnchor);
  }
//==============================================================================
  /*!
   * \brief Вычитание векторов
   * \param[in] vector - вектор, который вычитается из имеющегося
   * \return Вектор, полученный в результате вычитания
  */
  inline Vector3D operator - (const Vector3D& vector) const
  {
    return Vector3D(mOX - vector.mOX, mOY - vector.mOY, mOZ - vector.mOZ,
                    mAnchor);
  }
//==============================================================================
  /*! 
   * \brief Скалярное произведение двух векторов
   * \param[in] vector - вектор, с которым производится скалярное произведение
   * \return Скалярное произведение текущего вектора и заданного 
  */
  inline double operator & (const Vector3D& vector) const
  {
    return (mOX * vector.mOX + mOY * vector.mOY + mOZ * vector.mOZ);
  }
//==============================================================================
  /*!
   *\brief Векторное произведение двух векторов
   * \param[in] vector - вектор, с которым производится векторное произведение
   * \return Вектор, результирующий векторное произведение
  */
  inline Vector3D operator * (const Vector3D& vector) const
  {
    return Vector3D(mOY * vector.mOZ - mOZ * vector.mOY,
                    mOZ * vector.mOX - mOX * vector.mOZ,
                    mOX * vector.mOY - mOY * vector.mOX,
                    mAnchor);
  }
//==============================================================================
  /*!
   * \brief Умножение вектора на число
   * \param[in] number - число, на которое надо умножить вектор
   * \return Вектор, полученный в результате умножения
  */
  inline Vector3D operator * (double number) const
  {
    return Vector3D(mOX * number, mOY * number, mOZ * number, mAnchor);
  }
//==============================================================================
  /*!
   * \brief Деление вектора на число
   * \param[in] number - число, на которое требуется поделить вектор
   * \return Вектор, полученный в результате деления
   * \throws GeometryFailure # DividingByZero
   */
  Vector3D operator / (double number) const;
//==============================================================================
  /*!
   * \brief Сравнение двух векторов на равенство.
   * \details Вектора равны, если их проекции на оси равны
   * \param[in] vector - вектор, с которым сравниваем текущий
   * \return true  - равны
   *         false - не равны
  */
  inline bool operator == (const Vector3D& vector) const
  {
    if (   BasicMath::isEqual(mOX, vector.mOX)
        && BasicMath::isEqual(mOY, vector.mOY)
        && BasicMath::isEqual(mOZ, vector.mOZ))
      return true;
    return false;
  }
//==============================================================================
  /*!
   * \brief Получение проекции вектора на ось OX
   * \return Проекция вектора на ось OX
  */
  inline double getOX() const
  {
    return mOX;
  }
//==============================================================================
  /*!
   * \brief Получение проекции вектора на ось OY
   * \return Проекция вектора на ось OY
  */
  inline double getOY() const
  {
    return mOY;
  }
//==============================================================================
  /*!
   * \brief Получение проекции вектора на ось OZ
   * \return Проекция вектора на ось OZ
  */
  inline double getOZ() const
  {
    return mOZ;
  }
//==============================================================================
  /*!
   * \brief Получение точки приложения/привязки вектора
   * \return Точка приложения/привязки вектора
  */
  inline Point3D getAnchor() const
  {
    return mAnchor;
  }
//==============================================================================
  /*!
   * \brief Задание новой точки приложения/привязки вектора
   * \details Проекции на оси сохраняются.
   * \param[in] anchor - новая точка привязки вектора
  */
  inline void setAnchor(const Point3D& anchor)
  {
    mAnchor = anchor;
  }
//==============================================================================
protected:
//==============================================================================
  double  mOX;      //!< проекция на ось OX
  double  mOY;      //!< проекция на ось OY
  double  mOZ;      //!< проекция на ось OZ
  Point3D mAnchor;  //!< точка приложения/привязки вектора
//==============================================================================
};
//==============================================================================
} //! Geometry
//==============================================================================
#endif //! GEOMETRY_OBJECTS_VECTOR3D_HPP
