#ifndef MATH_TOOLS_BASIC_MATH_BASE_COMPARE_HPP
#define MATH_TOOLS_BASIC_MATH_BASE_COMPARE_HPP
//==============================================================================
//! Constants
#include <MathTools/Constants.hpp>
//! std
#include <typeindex>
//==============================================================================
namespace BasicMath
{

//==============================================================================
//! Сравнение значений (стандартных типов) с требуемой точностью
//==============================================================================

//==============================================================================
/*!
 * \brief Сравнение вещественных чисел с требуемой точностью.
 * \param[in] value1 - первое значение
 * \param[in] value2 - второе значение
 * \param[in] accuracy - требуемая точность сравнения
 * \return true  - значения совпадают
 *         false - значения различны
*/
inline bool isEqual(const float value1, const float value2, 
                    const float accuracy = ACCURACY_FLOAT)
{
  return (fabs(value1 - value2) < accuracy);
}
//==============================================================================
/*!
 * \brief Сравнение вещественных чисел с требуемой точностью.
 * \param[in] value1 - первое значение
 * \param[in] value2 - второе значение
 * \param[in] accuracy - требуемая точность сравнения
 * \return true  - значения совпадают
 *         false - значения различны
*/
inline bool isEqual(const double value1, const double value2,
                    const double accuracy = ACCURACY_DOUBLE)
{
  return (fabs(value1 - value2) < accuracy);
}
//==============================================================================

//==============================================================================
//! Сравнение значения на равенсто нулю
//==============================================================================

//==============================================================================
/*!
 * \brief Сравнение вещественного числа на равенство нулю.
 * \param[in] value - сравниваемое значение
 * \param[in] accuracy - требуемая точность сравнения
 * \return true  - равно нулю
 *         false - не равно нулю
*/
inline bool isNull(const float value, const float accuracy = ACCURACY_FLOAT)
{
  return isEqual(value, 0.f, accuracy);
}
//==============================================================================
/*!
 * \brief Сравнение вещественного числа на равенство нулю.
 * \param[in] value - сравниваемое значение
 * \param[in] accuracy - требуемая точность сравнения
 * \return true  - равно нулю
 *         false - не равно нулю
*/
inline bool isNull(const double value, const double accuracy = ACCURACY_DOUBLE)
{
  return isEqual(value, 0., accuracy);
}
//==============================================================================
} //! BasicMath
//==============================================================================
#endif //! MATH_TOOLS_BASIC_MATH_BASE_COMPARE_HPP