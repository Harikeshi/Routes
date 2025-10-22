#pragma once

#include <Task/Abstractions/Scheme.hpp>
#include <Task/Schemes/Search/InRegion/Input.hpp>

namespace Schemes {
namespace Search {
namespace InRegion {
/**
 * @brief Класс схемы Сдвиг.
 * 
 */
class Shift : public Abstractions::Scheme
{
    const Input& input; //!< Входные данные схемы.

public:
    /**
 * @brief Конструктор.
 * 
 * @param input Входящие данные.
 */
    explicit Shift(const Input& input);

    /**
 * @brief Метод расчета траектории схемы Сдвиг.
 * 
 * @return Outputs::Route Путь рассчитанный в схеме.
 */
    Outputs::Route calculate() override;

    /**
 * @brief Метод расчета вероятности схемы Сдвиг.
 * 
 * @return Outputs::SchemeEfficiency Результат расчета вероятности схемы. 
 */
    Outputs::SchemeEfficiency probability() override;

    ~Shift() = default;
};
} // namespace InRegion
} // namespace Search
} // namespace Schemes
