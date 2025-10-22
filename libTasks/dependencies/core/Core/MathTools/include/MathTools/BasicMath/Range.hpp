#ifndef MATH_TOOLS_BASIC_MATH_RANGE_HPP
#define MATH_TOOLS_BASIC_MATH_RANGE_HPP
//==============================================================================
#define _USE_MATH_DEFINES
#include <cmath>
//==============================================================================
namespace BasicMath
{
  //==============================================================================
  /*!
  * \brief Функция для приведения значения к диапазону.
  * \details Значение, выходящее за пределы диапазона, обрезается по границе
  * \param[in] value - значение
  * \param[in] min - минимально допустимое значение
  * \param[in] max - максимально допустимое значение
  * \return Значение в пределах диапазона
  */
  template<class Number>
  Number toRange(const Number& value, const Number& min, const Number& max)
  {
    if (value < min)
      return min;
    else if (value > max)
      return max;
    return value;
  }
  //==============================================================================
  /*!
  * \brief Функция для проверки попадания значения в диапазон.
  * \param[in] value - значение
  * \param[in] min - минимально допустимое значение
  * \param[in] max - максимально допустимое значение
  * \return true  - значение в пределах диапазона
  *         false - значение за пределами диапазона
  */
  template<class Number>
  bool isInRange(const Number& value, const Number& min, const Number& max)
  {
    if ((min <= value) && (value <= max))
      return true;
    return false;
  }
  //==============================================================================
  /*!
  * \brief Приведение значения в диапазон [-halfPeriod, halfPeriod]
  * \datail У выходящих за пределы диапазона величин скручивается нужное число
  *         периодов.
  *         По умолчанию ориентирован на радианы и приводит в диапазон [-pi, pi]
  * \param[in] value - приводимое значение
  * \param[in] halfPeriod - ПОЛОВИНА периода
  * \return Приведённое в диапазон значение
  */
  double normalize( const double& value, const double& halfPeriod = M_PI );
  //==============================================================================
  /*!
  * \brief Приведение значения в диапазон [0, period]
  * \datail У выходящих за пределы диапазона величин скручивается нужное число
  *         периодов.
  *         По умолчанию ориентирован на радианы и приводит в диапазон [0, 2*pi]
  * \param[in] value - приводимое значение
  * \param[in] period - ПОЛНЫЙ период
  * \return Приведённое в диапазон значение
  */
  double normalizeWithShift( 
    const double& value, const double& period = 2 * M_PI );
  //==============================================================================
} //! BasicMath
  //==============================================================================
#endif //! MATH_TOOLS_BASIC_MATH_RANGE_HPP