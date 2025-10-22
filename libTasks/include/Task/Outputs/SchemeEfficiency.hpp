#pragma once

#include <Task/Abstractions/Output.hpp>

/*! Core */
#include <MathTools/AngularUnits/Radian.hpp>

namespace Outputs {
/**
 * @brief Структура входных данных Эффективность схемы.
*/
struct SchemeEfficiency : public Abstractions::Output
{
    size_t scheme; //!< Наименование схемы поиска.

    double efficiency;     //!< Значение эффективности, %.
    Radian approachCourse; //!< Курс входа в район.

    /**
     * @brief Конструктор.
     * @param name Название схемы;
     * @param efficiency Значение эффективности;
     * @param course Курс.
    */
    explicit SchemeEfficiency(size_t name = 0, double efficiency = 0, double course = 0);

    /**
     * @brief Метод сериализации в объект json.
     * @return nlohmann::json Объект json.
    */
    nlohmann::json toJson() const override;
};
} // namespace Outputs
