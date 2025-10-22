#include <Task/Entities/Target.hpp>

#include <Task/Operations/JsonOperations.hpp>
#include <Task/Operations/ValidationOperations.hpp>

namespace Entities {

Target::Target()
{
    addValidator("detection_point", [](const nlohmann::json& j) {
        Operations::validatePoint2D(j, "detection_point", -999999, +999999);
    });

    addValidator("root-mean-square_error", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "root-mean-square_error", 0, 99999);
    });

    // TODO: добавить std::pair<T, T>
    addValidator("courses", [](const nlohmann::json& j) {
        Operations::validateArrayField(j, "courses");
    });

    addValidator("obsolescence_time", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "obsolescence_time", 0, 999999);
    });
    addValidator("avoidance_distance", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "avoidance_distance", 0, 999999);
    });

    // velocities
    addValidator("current", [](const nlohmann::json& j) {
        Operations::validateValueField(j["velocities"], "current", 0., 99.99);
    });
    addValidator("max", [](const nlohmann::json& j) {
        Operations::validateValueField(j["velocities"], "max", 0., 99.99);
    });
    addValidator("min_noise-reduced", [](const nlohmann::json& j) {
        Operations::validateValueField(j["velocities"], "min_noise-reduced", 0., 99.99);
    });
    addValidator("max_noise-reduced", [](const nlohmann::json& j) {
        Operations::validateValueField(j["velocities"], "max_noise-reduced", 0., 99.99);
    });
}

void Target::initializeProperties(const nlohmann::json& json)
{
    validate(json);

    contactPoint = Operations::getPointFromJson(json["detection_point"]);
    contactRms = json["root-mean-square_error"].get<double>();
    courseSector = {Degree{json["courses"][0].get<double>()}.toRadians(), Degree{json["courses"][1].get<double>()}.toRadians()};
    timeSinceContact = json["obsolescence_time"].get<double>();
    avoidanceDistance = json["avoidance_distance"].get<double>();

    contactVelocity = json["velocities"]["current"].get<double>();
    maxVelocity = json["velocities"]["max"].get<double>();
    minVelocity = json["velocities"]["min_noise-reduced"].get<double>();
    midVelocity = json["velocities"]["max_noise-reduced"].get<double>();
}
} // namespace Entities
