#pragma once

#include <Task/Abstractions/Input.hpp>

namespace Entities {

/**
 * @brief Структура Корабль.
 * 
 */
struct Ship : public Abstractions::Input
{
    double detectionRange;   //!< Дальность действия ГАС, [0-999999, м]
    double maxVelocity;      //!< Скорость полного хода, [0-99, м/с]
    double searchVelocity;   //!< Поисковая скорость, [0-99, м/с]
    double turningRadius;    //!< Радиус циркуляции группы судов, [0-999, м]
    double minLengthSection; //!< Минимальная длина участка, [0-999999, м]

public:
    /**
 * @brief Конструктор по-умолчанию.
 *
 */
    explicit Ship();

protected:
    /**
 * @brief Метод инициализации полей структуры.
 * 
 * @param json Объект json входных данных(json["input"]["ships_parameters"])
 */
    void initializeProperties(const nlohmann::json& json) override;
};
} // namespace Entities
