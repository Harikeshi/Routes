#pragma once

#include <Task/Abstractions/Scheme.hpp>
#include <Task/Schemes/Search/ByCall/Input.hpp>

namespace Schemes {
namespace Search {
namespace ByCall {
/**
 * @brief Класс схемы Гребенка.
 * 
 */
class StraightTack : public Abstractions::Scheme
{
    const Input& input; //!< Входные данные схемы.

public:
    /**
 * @brief Конструктор.
 * 
 * @param input Входящие данные.
 */
    explicit StraightTack(const Input& input);

    /**
 * @brief Метод расчета траектории схемы Гребенка.
 * 
 * @return Outputs::Route Путь рассчитанный в схеме.
 */
    Outputs::Route calculate() override;

    /**
 * @brief Метод расчета вероятности схемы Гребенка.
 * 
 * @return Outputs::SchemeEfficiency Результат расчета вероятности схемы. 
 */
    Outputs::SchemeEfficiency probability() override
    {
        std::cout << "probability StraightTack" << std::endl;
        return Outputs::SchemeEfficiency{};
    }

    /**
 * @brief Деструктор.
 * 
 */
    ~StraightTack() = default;
};

} // namespace ByCall
} // namespace Search
} // namespace Schemes
