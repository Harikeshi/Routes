#pragma once

#include <Task/Abstractions/Input.hpp>
#include <Task/Entities/Ship.hpp>

/*! Core */
#include <Point/Point2D.hpp>

namespace Schemes {
namespace Search {
namespace AtBorder {
/**
 * @brief Структура входных данных схемы Линейного патрулирования.
 * 
 */
struct Input : public Abstractions::Input
{
    /// @note Для Линейного и перекрёстного патрулирований рубеж всегда задаётся 2мя точками?
    //Point2D initialPosition; // Точка базирования(начала)

    //size_t shipsNmb;       // число кораблей в группы
    Entities::Ship ship;

    double maxTargetVel; // Скорость ПЛ
    //double searchTime;         //!< Время отведенное на поиск
    std::vector<Point2D> setBorder; // Назначенный рубеж поиска для группы.

    /**
    * @brief Конструктор.
    *
    */
    explicit Input();

protected:
    /**
 * @brief Метод инициализации полей структуры.
 * 
 * @param json Объект json входящих данных.
 */
    void initializeProperties(const nlohmann::json& json) override;
};

} // namespace AtBorder
} // namespace Search
} // namespace Schemes
