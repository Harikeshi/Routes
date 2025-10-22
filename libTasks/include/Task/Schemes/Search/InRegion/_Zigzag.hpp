#pragma once

#include <Task/Abstractions/Scheme.hpp>
#include <Task/Schemes/Search/InRegion/Input.hpp>

namespace Schemes {
namespace Search {
namespace InRegion {

/**
 * @brief Класс схемы Зигзаг.
 *
 */
class _Zigzag : public Abstractions::Scheme
{
    const Input& input; //!< Входные данные схемы.

public:
    /**
   * @brief Конструктор.
   *
   * @param input Входящие данные.
   */
    explicit _Zigzag(const Input& input);

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
