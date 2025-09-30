#pragma once

#include <QJsonObject>
#include <nlohmann/json.hpp>

namespace Models {
class Input
{
public:
    virtual ~Input() = default;

    /*!
     * Из входных данных.
     * @param json
     */
    void fromJson(const QJsonObject& json)
    {
        //        validate(json);

        initializeProperties(json);
    }

    /*!
     * Первичный ключ.
     * @return
     */
    virtual size_t getId() const = 0;

    /*!
     * Для базы данных(Отображение отличается от входного json).
     * @return
     */
    virtual QJsonObject toJson() const = 0;

    virtual nlohmann::json toNJson() const = 0;

protected:
    // TODO: Использовать JsonValue
    /*!
     * Инициализация из входного json.
     * @param json
     */
    virtual void
    initializeProperties(const QJsonObject& json) = 0;

    void validate(const QJsonObject& object) const
    {
        for (const auto& validator : validators)
            validator(object);
    }

    std::vector<std::function<void(const QJsonObject&)>> validators;

    void addValidator(const std::string& key, const std::function<void(const QJsonObject&)>& validator)
    {
        validators.emplace_back([key, validator](const QJsonObject& j) {
            validator(j);
        });
    }
};
} // namespace Models
