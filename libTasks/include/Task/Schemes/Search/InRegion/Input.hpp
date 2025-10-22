#pragma once

#include <Task/Abstractions/Input.hpp>
#include <Task/Entities/SearchRegion.hpp>
#include <Task/Entities/Ship.hpp>

namespace Schemes {
namespace Search {
namespace InRegion {
/**
 * @brief Структура входных данных схемы Зигзаг.
 * 
 */
struct Input : public Abstractions::Input
{
    Entities::SearchRegion region; //!< Район поиска
    Entities::Ship ship;           //!< Данные корабля
    Point2D initialPosition;       //!< Начальная позиция.
    double avoidanceDistance;      //!< Дистанция уклонения, т.е. расстояние, на котором цель может увидеть БЭК
    double searchTime;             //!< Время отведенное на поиск.

public:
    /**
 * @brief Конструктор.
 * 
 */
    explicit Input();
    //explicit Input(const Point2D& startPoint, const Point2D& finishPoint): firstPointInBl(startPoint), secondPointInBl(finishPoint), midLine(firstPointInBl, secondPointInBl) {};

protected:
    /**
 * @brief Метод инициализации полей структуры.
 * 
 * @param json Объект json входящих данных.
 */
    void initializeProperties(const nlohmann::json& json) override;
};
} // namespace InRegion
} // namespace Search
} // namespace Schemes
