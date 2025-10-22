#pragma once

#include <Task/Abstractions/Output.hpp>

namespace Outputs {
/**
 * @brief Структура входных данных Сообщение.
 *
*/
struct Message : public Abstractions::Output
{
    unsigned int code; //!< Код.
    std::string type;  //!< Тип.
    std::string text;  //!< Текст.

public:
    /**
     * @brief Конструктор.
     *
     * @param code Код сообщения;
     * @param type Тип сообщения;
     * @param text Подробный текст.
    */
    explicit Message(unsigned int code = 0, const std::string& type = "Info", const std::string& text = "<Success>: Расчет выполнен успешно.");

    /**
     * @brief Метод сериализации в объект json.
     *
     * @return nlohmann::json Объект json.
    */
    nlohmann::json toJson() const override;
};
} // namespace Outputs
