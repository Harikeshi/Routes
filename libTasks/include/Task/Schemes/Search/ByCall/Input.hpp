#pragma once

#include <Task/Abstractions/Input.hpp>
#include <Task/Entities/Ship.hpp>
#include <Task/Entities/Target.hpp>

namespace Schemes {
namespace Search {
namespace ByCall {
/**
 * @brief Структура входных данных схемы Спираль в секторе.
 * 
 */
struct Input : public Abstractions::Input
{
    // для задач А1, А2
    //    size_t shipsNmb;
    Point2D initialPosition; //!< Позиция корабля в начальный момент времени
    Entities::Ship ship;     //!< Данные корабля
    Entities::Target target; //!< Данные цели
    double maxSearchTime;    //!< Максимальное время на выполнение задания

    /**
 * @brief Конструктор.
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

} // namespace ByCall
} // namespace Search
} // namespace Schemes
