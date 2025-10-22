#pragma once

#include <Task/Outputs/Route.hpp>
#include <Task/Outputs/SchemeEfficiency.hpp>

#include <nlohmann/json.hpp>

namespace Abstractions {
/**
 * @brief Абстрактный класс поисковых схем.
 * 
*/
class Scheme
{
public:
    /**
     * @brief Виртуальный деструктор.
     * 
    */
    virtual ~Scheme() = default;
    //==============================================================================
    /**
     * @brief Метод расчета траектории.
     * 
     * @return Outputs::Route Путь рассчитанный в схеме.
    */
    virtual Outputs::Route calculate() = 0;
    //==============================================================================
    /**
     * @brief Метод расчета вероятности конкретной схемы.
     * 
     * @return Outputs::SchemeEfficiency Результат расчета вероятности конкретной схемы. 
    */
    virtual Outputs::SchemeEfficiency probability() = 0;
    //==============================================================================
};
} // namespace Abstractions
