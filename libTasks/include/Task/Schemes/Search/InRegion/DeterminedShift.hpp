#ifndef DETERMINEDSHIFT_H
#define DETERMINEDSHIFT_H

#pragma once

#include <Task/Abstractions/Scheme.hpp>
#include <Task/Schemes/Search/InRegion/Input.hpp>

namespace Schemes {
namespace Search {
namespace InRegion {
/**
 * @brief Класс схемы Детерменированный Сдвиг.
 *
 */
class DeterminedShift : public Abstractions::Scheme
{
    const Input& input; //!< Входные данные схемы.

public:
    /**
 * @brief Конструктор.
 *
 * @param input Входящие данные.
 */
    explicit DeterminedShift(const Input& input);

    /**
 * @brief Метод расчета траектории схемы Детерменированный Сдвиг.
 *
 * @return Outputs::Route Путь расcчитанный в схеме.
 */
    Outputs::Route calculate() override;

    /**
 * @brief Метод расчета вероятности схемы Детерменированный Сдвиг.
 *
 * @return Outputs::SchemeEfficiency Результат расчета вероятности схемы.
 */
    Outputs::SchemeEfficiency probability() override;

    ~DeterminedShift() = default;
};
} // namespace InRegion
} // namespace Search
} // namespace Schemes

#endif // DETERMAINDSHIFT_HPP
