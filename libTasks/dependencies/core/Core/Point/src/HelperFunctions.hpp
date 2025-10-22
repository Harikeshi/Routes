#ifndef POINT_HELPER_FUNCTIONS_HPP
#define POINT_HELPER_FUNCTIONS_HPP
//==============================================================================
#include <Point/Constants.hpp>
//! MathTools
#include <MathTools/AngularUnits/Radian.hpp>
//! std
#include<map>
//==============================================================================

//==============================================================================
//! Константы
//==============================================================================

//==============================================================================
//! Допустимые диапазоны долгот
const std::map<LongitudeRange, std::pair<Radian, Radian>> rangeByLongitude =
      { { REGULAR, std::make_pair(Radian(-M_PI), Radian(M_PI)) },
        { NORMAL, std::make_pair(Radian(0.), Radian(M_PI * 2)) } };
//==============================================================================
//! Допустимый диапазон широт
const std::pair<Radian, Radian> rangeByLatitude
      = std::make_pair(Radian(-M_PI / 2), Radian(M_PI / 2));
//==============================================================================

//==============================================================================
//! Функции
//==============================================================================
/*!
 * \brief Проверка, что широта находится в диапазоне допустимых значений: 
 *        [-PI / 2; PI / 2] = [-90; 90]
 * \param[in] latitude - значение широты в радианах
 * \return Результат проверки  
*/
inline bool isCorrectLatitude(const Radian& latitude)
{
  return (   (latitude >= rangeByLatitude.first)
          && (latitude <= rangeByLatitude.second));
}
//==============================================================================
/*!
 * \brief Проверка, что долгота находится в диапазоне допустимых значений
 * \param[in] latitude - значение долготы в радианах
 * \param[in] range - диапазон значений:
 *                    REGULAR - [-PI; PI) = [-180; 180)
 *                    NORMAL  - [0; 2 * PI) = [0; 360)
 * \return Результат проверки
*/
inline bool isCorrectLongitude(const Radian& longitude, LongitudeRange range)
{
  return (   (longitude >= rangeByLongitude.at(range).first)
          && (longitude < rangeByLongitude.at(range).second));
}
//==============================================================================
#endif //! POINT_HELPER_FUNCTIONS_HPP