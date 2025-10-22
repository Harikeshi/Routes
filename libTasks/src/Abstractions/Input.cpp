#include <Task/Abstractions/Input.hpp>

namespace Abstractions {

void Input::fromJson(const nlohmann::json& json)
{
    validate(json);

    initializeProperties(json);
}

void Input::validate(const nlohmann::json& object) const
{
    for (const auto& validator : validators)
        validator(object);
}

void Input::addValidator(const std::string& key, const std::function<void(const nlohmann::json&)>& validator)
{
    validators.emplace_back([key, validator](const nlohmann::json& j) {
        validator(j);
    });
}
} // namespace Abstractions
