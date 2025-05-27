#pragma once

#include <QJsonObject>

namespace Scene {
namespace Objects {
namespace Models {
class Input
{
public:
    //virtual ~Input() = default;

    void fromJson(const QJsonObject& json)
    {
        validate(json);

        initializeProperties(json);
    }

protected:
    // TODO: Использовать JsonValue
    virtual void initializeProperties(const QJsonObject& json) = 0;

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
} // namespace Objects
} // namespace Scene