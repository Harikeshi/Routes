#pragma once

#include <nlohmann/json.hpp>

namespace Abstractions {
/**
 * @brief Абстрактный класс выходных данных.
 *
*/
class Output
{
public:
    /**
     * @brief Виртуальный деструктор.
     * 
    */
    virtual ~Output() = default;
    //==============================================================================
    /**
     * @brief Виртуальный метод сериализации в json.
     * 
     * @return nlohmann::json Объект json.
    */
    virtual nlohmann::json toJson() const = 0;
    //==============================================================================
};
} // namespace Abstractions
