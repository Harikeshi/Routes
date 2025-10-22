#pragma once

#include <Task/Abstractions/Scheme.hpp>
#include <Task/Schemes/Search/ByCall/Input.hpp>

namespace Schemes {
namespace Search {
namespace ByCall {
/**
 * @brief Класс схемы Спираль в секторе.
 * 
 */
class SectorSpiral : public Abstractions::Scheme
{
    const Input& input; //!< Входные данные схемы.

public:
    /**
 * @brief Конструктор.
 * 
 * @param input Входящие данные.
 */
    explicit SectorSpiral(const Input& input);

    /**
 * @brief Метод расчета траектории схемы Спираль в секторе.
 * 
 * @return Outputs::Route Путь рассчитанный в схеме.
 */
    Outputs::Route calculate() override;

    /**
 * @brief Метод расчета вероятности схемы Спираль в секторе.
 * 
 * @return Outputs::SchemeEfficiency Результат расчета вероятности схемы. 
 */
    Outputs::SchemeEfficiency probability() override;

    /**
 * @brief Деструктор.
 * 
 */
    ~SectorSpiral() = default;
};
} // namespace ByCall
} // namespace Search
} // namespace Schemes
