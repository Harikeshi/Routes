#pragma once

#include <Task/Schemes/Search/Config.hpp>

/*! Core */
#include <MathTools/AngularUnits/Radian.hpp>

/*! std */
#include <random>

namespace Schemes {
namespace Search {
namespace InRegion {

/*
inline double tackLength(const double& tackMax, const double& detectionRange, const double& weight)
{
    if (tackMax <= detectionRange)
    {
        return tackMax;
    }
    else
    {
        return detectionRange + (tackMax - detectionRange) * weight;
    }
}
*/

inline double tackLength(const double& tackMax, const double& weight, const double& tackDistLeft)
{
    if (tackMax <= tackDistLeft)
    {
        return tackMax;
    }
    else
    {
        return tackDistLeft + (tackMax - tackDistLeft) * weight;
    }
}

/*!
 * \brief calculateTackDistance
 * \details Расчёт длины галса
 * \param tackMax
 * \param detectionRange
 * \param rand01
 * \return
 */
//inline double tackLength(const double tackMax, const double detectionRange, const double rand01)
//{
//    if (tackMax <= detectionRange)
//    {
//        return tackMax;
//    }
//    else
//    {
//        return detectionRange + (tackMax - detectionRange) * rand01;
//    }
//}

/*! @todo
 * \brief generateConstArray
 * \param size
 * \return
 */
std::vector<double> generateConstArray(size_t size);

/*!
 * \brief генерация случайного значения угла в диапазоне
 * \param u01 объект класса однородного распределения вещественных чисел на интервале [0, 1)
 * \return объект класса Radian со значением в диапазоне [TURN_ANGLE_MIN_DEG, TURN_ANGLE_MAX_DEG)
 */
//Radian generateTurnAngle(std::uniform_real_distribution<>& u01);

/*!  \brief генерация случайного значения длины галса
 * \details если tackMax > detectionRange, то возвращается случайное число, равномерно распределенное в диапазоне от [detectionRange, tackMax) иначе - возвращается если верхний предел меньше дальности обнаружения, то возвращается значение
 * \param tackMax наибольшая возможная длина галса
 * \param detectionRange дальность обнаружения
 * \param u01 объект класса распределения вероятностей с кусочно-линейной ф. плотности вещественных чисел на интервале [0, 1)
 * \return double
 */
//double generateTackDistance(const double tackMax, const double detectionRange, std::uniform_real_distribution<>& u01);

/*!  \brief генерация случайного значения длины галса
 * \details если tackMax > detectionRange, то возвращается случайное число, равномерно распределенное в диапазоне от [detectionRange, tackMax) иначе - возвращается если верхний предел меньше дальности обнаружения, то возвращается значение
 * \param tackMax наибольшая возможная длина галса
 * \param detectionRange дальность обнаружения
 * \param p01 объект класса однородного распределения вещественных чисел на интервале [0, 1)
 * \return double
 */
//double generateTackDistance(const double tackMax, const double visionDist, std::piecewise_linear_distribution<>& p01);

} // namespace InRegion
} // namespace Search
} // namespace Schemes
