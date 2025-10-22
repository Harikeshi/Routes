#pragma once

#include <Task/Abstractions/Scheme.hpp>
#include <Task/Entities/SearchRegion.hpp>
#include <Task/Operations/SegmentOperations.hpp>
#include <Task/Schemes/Search/InRegion/Input.hpp>
#include <cmath>

namespace Schemes {
namespace Search {
namespace InRegion {

/**
 * @brief Класс схемы Гребёнка.
 *
 */
class StraightTackOnce : public Abstractions::Scheme
{
    const Input& input; //!< Входные данные схемы.
public:
    /**
 * @brief Конструктор.
 *
 * @param input Входящие данные.
 */
    explicit StraightTackOnce(const Input& input);

    /**
 * @brief Метод расчета вероятности схемы Зигзаг.
 *
 * @return Outputs::SchemeEfficiency Результат расчета вероятности схемы.
 */
    Outputs::SchemeEfficiency probability() override;

    /**
 * @brief Метод расчета траектории схемы Зигзаг.
 *
 * @return Outputs::Route Путь рассчитанный в схеме.
 */
    Outputs::Route calculate() override;
};
} // namespace InRegion
} // namespace Search
} // namespace Schemes
