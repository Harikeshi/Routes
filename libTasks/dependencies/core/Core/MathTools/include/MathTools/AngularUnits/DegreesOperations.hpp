#ifndef MATH_TOOLS_ANGULAR_UNITS_DEGREES_OPERATIONS_HPP
#define MATH_TOOLS_ANGULAR_UNITS_DEGREES_OPERATIONS_HPP
//==============================================================================
//! MathException
#include <MathTools/MathException.hpp>
//! BasicMath
#include <MathTools/BasicMath/BaseCompare.hpp>
//! AngularUnits 
#include <MathTools/AngularUnits/Degree.hpp>
//==============================================================================

//==============================================================================
//! Свободные функции: операторы сравнения градусов
//! Используются для сравнения градусов, в том числе в рамках стандартных 
//! функций  
//==============================================================================

//==============================================================================
/*!
* \brief Сравнение двух величин в градусах
* \param[in] leftDegree - величина в градусах, которую сравниваем
* \param[in] rightDegree - величина в градусах, с которой сравниваем
* \return Результат сравнения двух значений в градусах
*/
inline bool operator == (const Degree& leftDegree, const Degree& rightDegree)
{
  return (fabs(leftDegree.get() - rightDegree.get()) <= ACCURACY_DOUBLE);
}
//==============================================================================
/*!
* \brief Сравнение, что одна из величин больше другой
* \param[in] leftDegree - величина в градусах, которую сравниваем
* \param[in] rightDegree - величина в градусах, с которой сравниваем
* \return Результат сравнения двух значений в градусах
*/
inline bool operator > (const Degree& leftDegree, const Degree& rightDegree)
{
  return ((leftDegree.get() - rightDegree.get()) > ACCURACY_DOUBLE);
}
//==============================================================================
/*!
 * \brief Сравнение, что одна из величин не меньше другой
 * \param[in] leftDegree - величина в градусах, которую сравниваем
 * \param[in] rightDegree - величина в градусах, с которой сравниваем
 * \return Результат сравнения двух значений в градусах
*/
inline bool operator >= (const Degree& leftDegree, const Degree& rightDegree)
{
  return ((leftDegree > rightDegree) || (leftDegree == rightDegree));
}
//==============================================================================
/*!
* \brief Сравнение, что одна из величин меньше другой
* \param[in] leftDegree - величина в градусах, которую сравниваем
* \param[in] rightDegree - величина в градусах, с которой сравниваем
* \return Результат сравнения двух значений в градусах
*/
inline bool operator < (const Degree& leftDegree, const Degree& rightDegree)
{
  return ((rightDegree.get() - leftDegree.get()) > ACCURACY_DOUBLE);
}
//==============================================================================
/*!
 * \brief Сравнение, что одна из величин не превосходит (не меньше) другую
 * \param[in] leftDegree - величина в градусах, которую сравниваем
 * \param[in] rightDegree - величина в градусах, с которой сравниваем
 * \return Результат сравнения двух значений в градусах
*/
inline bool operator <= (const Degree& leftDegree, const Degree& rightDegree)
{
  return ((leftDegree < rightDegree) || (leftDegree == rightDegree));
}
//==============================================================================

//==============================================================================
//! Свободные функции: математические операции с числами
//==============================================================================

//==============================================================================
/*!
* \brief Сложение градусов с вещественным числом двойной точности 
* \param[in] degree - величина в градусах
* \param[in] value - вещественное число двойной точности, которое суммируется
* \return Результат сложения в градусах
*/
inline Degree operator + (const Degree& degree, double value)
{
  return Degree(degree.get() + value);
}
//==============================================================================
/*!
* \brief Вычитание из градусов вещественного числа двойной точности
* \param[in] degree - величина в градусах
* \param[in] value - вещественное число двойной точности, которое вычитается
* \return Результат вычитания в градусах
*/
inline Degree operator - (const Degree& degree, double value)
{
  return Degree(degree.get() - value);
}
//==============================================================================
/*!
* \brief Умножение градусов на вещественное число двойной точности
* \param[in] degree - величина в градусах
* \param[in] value - вещественное число двойной точности, на которое умножаем
* \return Результат умножения в градусах
*/
inline Degree operator * (const Degree& degree, double value)
{
  return Degree(degree.get() * value);
}
//==============================================================================
/*!
* \brief Деление градусов на вещественное число двойной точности
* \param[in] degree - величина в градусах
* \param[in] value - вещественное число двойной точности, на которое делим
* \return Результат деления в градусах
* \throws MathFailure # DegreeDivideByZero
*/
inline Degree operator / (const Degree& degree, double value)
{
  if (BasicMath::isNull(value))
    throw MathFailure(DegreeDivideByZero);
  return Degree(degree.get() / value);
}
//==============================================================================

#endif //! MATH_TOOLS_ANGULAR_UNITS_DEGREES_OPERATIONS_HPP