#pragma once

/*! Core */
#include <AbstractOperations/ChangeEachPoint.hpp>

namespace Operations {

// TODO: Глобальные переменные, сомнительно.
const double knotToMps = 0.514444;
const double mileToMeter = 1852;
const double hourToSecond = 3600;

/*!
 * \brief milesToMeters перевод координат из метров в мили
 * \param pnt координатная точка в метрах
 */
inline void metersToMiles(Point2D& pnt)
{
    pnt.set(pnt.getX() / mileToMeter, pnt.getY() / mileToMeter);
}

/*!
 * \brief milesToMeters перевод координат из миль в метры
 * \param pnt координатная точка в милях
 */
inline void milesToMeters(Point2D& pnt)
{
    pnt.set(pnt.getX() * mileToMeter, pnt.getY() * mileToMeter);
}

/*!
 * \brief metricToNauticalInput перевод полей входной структуры поиска в районе в навигационные систему единиц (миля, час, узел)
 * \param входная структура поиска в районе в метрической системе единиц (метр, секунда, м/с)
 */
template<typename T>
static void metricToNauticalInput(T& input)
{
    input.region.metersToMiles();
    input.ship.maxVelocity /= knotToMps;
    input.ship.searchVelocity /= knotToMps;
    input.ship.detectionRange /= mileToMeter;
    input.ship.turningRadius /= mileToMeter;
    input.ship.minLengthSection /= mileToMeter;
    input.searchTime /= hourToSecond;
}

/*!
 * \brief nauticalToMetricOutput перевод векторов координат и скоростей в метрическую систему единиц (метры, м/с)
 * \param points вектор координатных точек в милях
 * \param velocities вектор скоростей в узлах
 */
static void nauticalToMetricOutput(std::vector<Point2D>& points, std::vector<double>& velocities)
{
    AbstractOperations::changeEachPoint(points, milesToMeters);
    std::transform(velocities.begin(), velocities.end(), velocities.begin(), [](double x) { return x * knotToMps; });
}
} // namespace Operations
