#pragma once

#include <nlohmann/json.hpp>

namespace Abstractions {
/**
 * @brief Абстрактный класс входных данных.
 * 
*/
class Input
{
public:
    /**
     * @brief Виртуальный деструктор.
     *
    */
    virtual ~Input() = default;
    //==============================================================================
    /**
     * @brief Метод десериализации из объекта json.
     *
     * @param json Объект json.
    */
    void fromJson(const nlohmann::json& json);
    //==============================================================================
protected:
    /**
     * @brief Метод инициализации полей структуры.
     *
     * @param json Объект json входящих данных.
    */
    virtual void initializeProperties(const nlohmann::json& json) = 0;
    //==============================================================================
    /**
     * @brief Метод валидации полей.
     *
     * @param object Объект json, который используется для десериализации.
    */
    void validate(const nlohmann::json& object) const;
    //==============================================================================
    /**
     * @brief Метод добавления функции валидации.
     *
     * @param key Имя поля json объекта;
     * @param validator Функция валидации.
    */
    void addValidator(const std::string& key, const std::function<void(const nlohmann::json&)>& validator);
    //==============================================================================
    /**
     * @brief Список методов, используемых для валидации.
     *
    */
    std::vector<std::function<void(const nlohmann::json&)>> validators;
};
} // namespace Abstractions
