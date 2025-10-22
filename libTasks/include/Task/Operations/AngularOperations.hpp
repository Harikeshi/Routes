#pragma once

/*! Core */
#include <MathTools/AngularUnits/Radian.hpp>

/*! std */
#include <cmath>

namespace Operations {
//==============================================================================
/*!
 * \brief Преобразование градусов в радианы
 * \param[in] degrees - значение угла в градусах
 * \return угол в радианах
*/
//==============================================================================
static inline double toRad(double degrees)
{
    return M_PI * degrees / 180;
}
//==============================================================================
/*!
 * \brief Преобразование радиан в градусы
 * \param[in] radians - значение угла в радианах
 * \return угол в градусах
*/
//==============================================================================
static inline double toDeg(double radians)
{
    return 180 * radians / M_PI;
}

//==============================================================================
/*!
 * \brief Приведение значения угла к диапазону [-полупериод , полупериод] = [-Pi, Pi]
 * \param[in] angle - значение угла в радианах
 * \param[in] hPeriod - полупериод = Pi
 * \return угол в радианах
*/
/*
static inline double normalizeAngle(double angle, double hPeriod=M_PI)
{
    if(angle >= hPeriod)
    {
        return angle-(floor(angle/(2*hPeriod)+0.5f))*2*hPeriod;
    }
    if(angle < -hPeriod)
    {
        return angle-(ceil(angle/(2*hPeriod)-0.5f))*2*hPeriod;
    }
    return angle;
}
*/

//==============================================================================
/*!
 * \brief Приведение значения угла к диапазону [-Pi, Pi]
 * \param[in] angle - значение угла в радианах
 * \return угол в радианах
*/
static inline double normalizeAngle(double angle)
{
    if (std::abs(angle) > M_PI)
    {
        angle = std::fmod(angle, 2 * M_PI); // angle %= 2Pi
        if (angle > M_PI)
        {
            angle -= (2 * M_PI);
        }
        else if (angle < -M_PI)
        {
            angle += (2 * M_PI);
        }
    }
    return angle;
}
} // namespace Operations
