#pragma once

#include <Task/Abstractions/Output.hpp>

/*! Core */
#include <Point/Point2D.hpp>

namespace Outputs {
/**
 * @brief Структура выходных данных Путь. 
 *
*/
struct Route : public Abstractions::Output
{
    std::vector<Point2D> points;    //!< Рассчитанный массив точек маршрута.
    std::vector<double> velocities; //!< Рассчитанный массив скоростей в точках маршрута.

    // TODO: Добавить конструктор при необходимости.
    Route(const std::vector<Point2D>& points, const std::vector<double>& velocities)
        : points{points}, velocities{velocities}
    {
    }
    /**
     * @brief Конструктор по-умолчанию.
     *
    */
    explicit Route() = default;

    /**
     * @brief Метод сериализации в объект json.
     *
     * @return nlohmann::json Объект json.
    */
    nlohmann::json toJson() const override;

    void validate();

    /*!
     * Метод вычисления длительности пути.
     * @return double
     */
    double duration() const;
    /*!
     * Метод проверки времени.
     * @param setTime
     * @param timeAccuracy
     * @return
     */
    bool checkTime(double setTime, double timeAccuracy) const;
    /*!
     * Метод вычисления длины пути.
     * @return double
     */
    double length() const;

    //TODO: operator <<
};
} // namespace Outputs
