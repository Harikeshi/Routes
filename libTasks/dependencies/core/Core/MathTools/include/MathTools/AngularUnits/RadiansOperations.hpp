#ifndef MATH_TOOLS_ANGULAR_UNITS_RADIANS_OPERATIONS_HPP
#define MATH_TOOLS_ANGULAR_UNITS_RADIANS_OPERATIONS_HPP
//==============================================================================
//! MathException
#include <MathTools/MathException.hpp>
//! BasicMath
#include <MathTools/BasicMath/BaseCompare.hpp>
//! AngularUnits 
#include <MathTools/AngularUnits/Degree.hpp>
#include <MathTools/AngularUnits/Radian.hpp>
//! std
#include <sstream>
//==============================================================================

//==============================================================================
//! Свободные функции: операторы сравнения радиан
//! Используются для сравнения радиан, в том числе в рамках стандартных функций  
//==============================================================================

//==============================================================================
/*!
 * \brief Сравнение двух величин в радианах
 * \param[in] leftRadian - величина в радианах, которую сравниваем
 * \param[in] rightRadian - величина в радианах, с которой сравниваем
 * \return Результат сравнения двух значений радиан
*/
inline bool operator == (const Radian& leftRadian, const Radian& rightRadian)
{
  return (fabs(leftRadian.get() - rightRadian.get()) <= ACCURACY_DOUBLE);
}
//==============================================================================
/*!
 * \brief Сравнение, что одна из величин больше другой
 * \param[in] leftRadian - величина в радианах, которую сравниваем
 * \param[in] rightRadian - величина в радианах, с которой сравниваем
 * \return Результат сравнения двух значений радиан
*/
inline bool operator > (const Radian& leftRadian, const Radian& rightRadian)
{
  return ((leftRadian.get() - rightRadian.get()) > ACCURACY_DOUBLE);
}
//==============================================================================
/*!
 * \brief Сравнение, что одна из величин не меньше другой
 * \param[in] leftRadian - величина в радианах, которую сравниваем
 * \param[in] rightRadian - величина в радианах, с которой сравниваем
 * \return Результат сравнения двух значений радиан
*/
inline bool operator >= (const Radian& leftRadian, const Radian& rightRadian)
{
  return ((leftRadian > rightRadian) || (leftRadian == rightRadian));
}
//==============================================================================
/*!
 * \brief Сравнение, что одна из величин меньше другой
 * \param[in] leftRadian - величина в радианах, которую сравниваем
 * \param[in] rightRadian - величина в радианах, с которой сравниваем
 * \return Результат сравнения двух значений радиан
*/
inline bool operator < (const Radian& leftRadian, const Radian& rightRadian)
{
  return ((rightRadian.get() - leftRadian.get()) > ACCURACY_DOUBLE);
}
//==============================================================================
/*!
 * \brief Сравнение, что одна из величин не превосходит (не меньше) другую
 * \param[in] leftRadian - величина в радианах, которую сравниваем
 * \param[in] rightRadian - величина в радианах, с которой сравниваем
 * \return Результат сравнения двух значений радиан
*/
inline bool operator <= (const Radian& leftRadian, const Radian& rightRadian)
{
  return ((leftRadian < rightRadian) || (leftRadian == rightRadian));
}
//==============================================================================

//==============================================================================
//! Свободные функции: операторы сравнения радиан и целых чисел
//! Требуется для boost::geometry (union_, intersection, difference и т.п.)  
//==============================================================================

//==============================================================================
/*!
 * \brief Сравнение на равенство радиан и целого числа
 * \param[in] radian - величина в радианах, которую сравниваем
 * \param[in] value - целое число, с которым сравниваем
 * \return Результат сравнения
*/
inline bool operator == (const Radian& radian, const int value)
{
  return (radian.get() == value);
}
//==============================================================================
/*!
 * \brief Сравнение на неравенство радиан и целого числа
 * \param[in] radian - величина в радианах, которую сравниваем
 * \param[in] value - целое число, с которым сравниваем
 * \return Результат сравнения
*/
inline bool operator != (const Radian& radian, const int value)
{
  return (radian.get() != value);
}
//==============================================================================
/*!
 * \brief Сравнение: превосходит ли значение в радианах целого числа
 * \param[in] radian - величина в радианах, которую сравниваем
 * \param[in] value - целое число, с которым сравниваем
 * \return Результат сравнения
*/
inline bool operator > (const Radian& radian, const int value)
{
  return (radian.get() > value);
}
//==============================================================================  
/*!
 * \brief Сравнение: меньше ли значение в радианах целого числа
 * \param[in] radian - величина в радианах, которую сравниваем
 * \param[in] value - целое число, с которым сравниваем
 * \return Результат сравнения
*/
inline bool operator < (const Radian& radian, const int value)
{
  return (radian.get() < value);
}
//==============================================================================
/*!
 * \brief Сравнение: не уступает ли значение в радианах целому числу
 * \param[in] radian - величина в радианах, которую сравниваем
 * \param[in] value - целое число, с которым сравниваем
 * \return Результат сравнения
*/
inline bool operator >= (const Radian& radian, const int value)
{
  return (radian.get() >= value);
}
//==============================================================================  
/*!
 * \brief Сравнение: не уступает ли значение в радианах целому числу
 * \param[in] radian - величина в радианах, которую сравниваем
 * \param[in] value - целое число, с которым сравниваем
 * \return Результат сравнения
*/
inline bool operator <= (const Radian& radian, const int value)
{
  return (radian.get() <= value);
}
//==============================================================================

//==============================================================================
//! Свободные функции: операторы сравнения радиан и вещественных чисел
//! Требуется для boost::geometry (area и т.п.)  
//==============================================================================

//==============================================================================
/*!
 * \brief Сравнение: превосходит ли значение в радианах вещественного числа
 * \param[in] radian - величина в радианах, которую сравниваем
 * \param[in] value - вещественное число, с которым сравниваем
 * \return Результат сравнения
*/
inline bool operator > (const Radian& radian, const double value)
{
  return (radian.get() > value);
}
//==============================================================================

//==============================================================================
//! Свободные функции: операторы вывода в поток
//! Требуется для boost::geometry::read_wkt
//==============================================================================

//==============================================================================
/*!
 * \brief Перегруженный оператор извлечения из потока
 * \details Из потока берутся значения в ГРАДУСАХ.
 * \param[in] stream - поток ввода
 * \param[in] radian - величина в радианах, которую записываем
 * \return Поток ввода
*/
inline std::istream& operator>>(std::istream& stream, Radian& radian)
{
  double value;
  stream >> value;
  radian = Degree(value).toRadians();
  return stream;
}
//==============================================================================

//==============================================================================
//! Свободные функции: математические операции с числами
//==============================================================================

//==============================================================================
/*!
 * \brief Сложение радиан с вещественным числом двойной точности 
 * \param[in] radian - величина в радианах
 * \param[in] value - вещественное число двойной точности, которое суммируется
 * \return Результат сложения в радианах
 */
inline Radian operator + (const Radian& radian, double value)
{
  return Radian(radian.get() + value);
}
//==============================================================================
/*!
 * \brief Вычитание из радиан вещественного числа двойной точности
 * \param[in] radian - величина в радианах
 * \param[in] value - вещественное число двойной точности, которое вычитается
 * \return Результат вычитания в радианах
 */
inline Radian operator - (const Radian& radian, double value)
{
  return Radian(radian.get() - value);
}
//==============================================================================
/*!
 * \brief Умножение радиан на вещественное число двойной точности
 * \param[in] radian - величина в радианах
 * \param[in] value - вещественное число двойной точности, на которое умножаем
 * \return Результат умножения в радианах
 */
inline Radian operator * (const Radian& radian, double value)
{
  return Radian(radian.get() * value);
}
//==============================================================================
/*!
 * \brief Деление радиан на вещественное число двойной точности
 * \param[in] radian - величина в радианах
 * \param[in] value - вещественное число двойной точности, на которое делим
 * \return Результат деления в радианах
 * \throws MathFailure # RadianDivideByZero
*/
inline Radian operator / (const Radian& radian, double value)
{
  if (BasicMath::isNull(value))
    throw MathFailure(RadianDivideByZero);
  return Radian(radian.get() / value);
}
//==============================================================================
  /*!
   * \brief Абсолютное значение радиан
   * \param[in] radian - исходное значение [рад]
   * \return абсолютное значение [рад]
   */
  inline Radian fabs( const Radian& radian )
  {
    return Radian ( fabs ( radian.get() ) );
  }
//==============================================================================

  /*!
 * \brief bisector нахождение биссектрисы дирекционных углов
 * \note углы должны быть нормализованы.
 * В случае если beta > alpha получается биссектриса острого угла, иначе - выпуклого угла
 * \param alpha - первый угол
 * \param beta - второй угол
 * \return
 */
inline Radian bisector(const Radian& alpha, const Radian& beta)
{
    return alpha + (beta - alpha) / 2;
}

#endif //! MATH_TOOLS_ANGULAR_UNITS_RADIANS_OPERATIONS_HPP
