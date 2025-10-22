#pragma once

#include <Task/Abstractions/Scheme.hpp>
#include <Task/Schemes/Search/AtBorder/Input.hpp>

namespace Schemes {
namespace Search {
namespace AtBorder {
/**
 * @brief Класс схемы На стопе. 
 * 
 */
class OnStop : public Abstractions::Scheme
{
    const Input& input; //!< Входные данные схемы.

public:
    /**
 * @brief Конструктор.
 * 
 * @param input Входящие данные.
 */
    explicit OnStop(const Input& input)
        : input{input}
    {
    }

    /**
 * @brief Метод расчета траектории схемы На стопе.
 * 
 * @return Outputs::Route Путь рассчитанный в схеме.
 */
    Outputs::Route calculate() override
    {
        std::cout << "calculate OnStop" << std::endl;
        return Outputs::Route{};
    }

    /**
 * @brief Метод расчета вероятности схемы На стопе.
 * 
 * @return Outputs::SchemeEfficiency Результат расчета вероятности схемы. 
 */
    Outputs::SchemeEfficiency probability() override
    {
        std::cout << "probability OnStop" << std::endl;

        return Outputs::SchemeEfficiency{};
    }
    /**
 * @brief Деструктор.
 * 
 */
    ~OnStop() = default;
};
} // namespace AtBorder
} // namespace Search
} // namespace Schemes
