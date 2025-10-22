#ifndef MATH_TOOLS_BASIC_MATH_COMPARE_HPP
#define MATH_TOOLS_BASIC_MATH_COMPARE_HPP
//==============================================================================
//! Constants
#include <MathTools/Constants.hpp>
//! AngularUnits
#include <MathTools/AngularUnits/Degree.hpp>
#include <MathTools/AngularUnits/Radian.hpp>
//==============================================================================
namespace BasicMath
{

//==============================================================================
//! Сравнение значений с требуемой точностью
//==============================================================================

//==============================================================================
/*!
 * \brief Сравнение значений градусов с требуемой точностью.
 * \param[in] value1 - первое значение
 * \param[in] value2 - второе значение
 * \param[in] accuracy - требуемая точность сравнения
 * \return true  - значения совпадают
 *         false - значения различны
*/
inline bool isEqual(const Degree& value1, const Degree& value2,
                    const double accuracy = ACCURACY_DEGREE)
{
  return (fabs(value1.get() - value2.get()) < accuracy);
}
//==============================================================================
/*!
 * \brief Сравнение значений радиан с требуемой точностью.
 * \param[in] value1 - первое значение
 * \param[in] value2 - второе значение
 * \param[in] accuracy - требуемая точность сравнения
 * \return true  - значения совпадают
 *         false - значения различны
*/
inline bool isEqual(const Radian& value1, const Radian& value2,
                    const double accuracy = ACCURACY_RADIAN)
{
  return (fabs(value1.get() - value2.get()) < accuracy);
}
//==============================================================================

//==============================================================================
//! Сравнение значения на равенсто нулю
//==============================================================================

//==============================================================================
/*!
 * \brief Сравнение значения градусов на равенство нулю.
 * \param[in] value - сравниваемое значение
 * \param[in] accuracy - требуемая точность сравнения
 * \return true  - равно нулю
 *         false - не равно нулю
*/
inline bool isNull(const Degree& value, const double accuracy = ACCURACY_DEGREE)
{
  return isEqual(value, Degree(), accuracy);
}
//==============================================================================
/*!
 * \brief Сравнение значения радиан на равенство нулю.
 * \param[in] value - сравниваемое значение
 * \param[in] accuracy - требуемая точность сравнения
 * \return true  - равно нулю
 *         false - не равно нулю
*/
inline bool isNull(const Radian& value, const double accuracy = ACCURACY_RADIAN)
{
  return isEqual(value, Radian(), accuracy);
}
//==============================================================================
} //! BasicMath
//==============================================================================
#include <MathTools/BasicMath/BaseCompare.hpp>
//==============================================================================
#endif //! MATH_TOOLS_BASIC_MATH_COMPARE_HPP