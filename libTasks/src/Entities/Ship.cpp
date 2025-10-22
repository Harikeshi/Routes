#include <Task/Entities/Ship.hpp>

#include <Task/Operations/ValidationOperations.hpp>

namespace Entities {

Ship::Ship()
{
    addValidator("detection_range", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "detection_range", 0, 999999);
    });

    addValidator("max_velocity", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "max_velocity", 0, 99);
    });

    addValidator("search_velocity", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "search_velocity", 0, 99);
    });

    addValidator("turning_radius", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "turning_radius", 0, 999);
    });

    addValidator("min_lenght_section", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "min_lenght_section", 0, 999999);
    });
}

void Ship::initializeProperties(const nlohmann::json& json)
{
    //    validate(json);
    //std::cout << json.dump(4) << std::endl;

    detectionRange = json["detection_range"].get<double>();
    maxVelocity = json["max_velocity"].get<double>();
    searchVelocity = json["search_velocity"].get<double>();
    turningRadius = json["turning_radius"].get<double>();
    minLengthSection = json["min_lenght_section"].get<double>();
}
} // namespace Entities
