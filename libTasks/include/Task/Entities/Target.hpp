#pragma once

#include <Task/Abstractions/Input.hpp>

/*! Core */
#include <MathTools/AngularUnits/Radian.hpp>
#include <Point/Point2D.hpp>

namespace Entities {
/**
 * @brief Структура Цель.
 * 
 */
struct Target : public Abstractions::Input
{
    Point2D contactPoint;                   //!< Точка контакта с целью(обнаружения цели) , ["x", "y"],
    double contactRms;                      //!< Среднеквадратичная ошибка (RMS) определения точки контакта, [0-99999]
    double contactVelocity;                 //!< Скорость цели в момент контакта, [0-99.99, м/c]
    double timeSinceContact;                //!< Время прошедшее с момента контакта, [0-999999, c]
    std::pair<Radian, Radian> courseSector; //!< Сектор курсов движения цели, [0-360, 0-360], градусы. Для правой системы координат first будет соответствовать правому лучу сектора, second - левому
    double maxVelocity;                     //!< Скорость полного хода цели, [0-99.99, м/c]
    double minVelocity;                     //!< Минимальная малошумная скорость цели, [0-99.99, м/c]
    double midVelocity;                     //!< Максимальная малошумная скорость цели, [0-99.99,  м/c]
    double avoidanceDistance;               //!< Дистанция уклонения, [0-999999, м]

    /**
 * @brief Конструктор по-умолчанию
 * 
 */
    explicit Target();

protected:
    /**
 * @brief Метод инициализации полей структуры.
 * 
 * @param json Объект json входных данных(json["input"]["form_target"])
 */
    void initializeProperties(const nlohmann::json& json) override;
};
} // namespace Entities
