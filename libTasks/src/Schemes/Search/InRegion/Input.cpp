#include <Task/Schemes/Search/InRegion/Input.hpp>

#include <Task/Operations/JsonOperations.hpp>
#include <Task/Operations/ValidationOperations.hpp>

namespace Schemes {
namespace Search {
namespace InRegion {

Input::Input()
{
    // Внести значение search_velocity, max_velocity, detection_range, time
    // TODO: Проверка полей файла json, значения интервалов из файла config.json
    // auto config = Operations::read(path);

    // TODO: Добавить проверку региона

    //    addValidator("search_velocity", [](const nlohmann::json& j) {
    //        Operations::validateValueField(j["ships_parameters"], "search_velocity", 0, 99);
    //    });

    //    addValidator("max_velocity", [](const nlohmann::json& j) {
    //        Operations::validateValueField(j["ships_parameters"], "max_velocity", 0, 99);
    //    });

    //    addValidator("detection_range", [](const nlohmann::json& j) {
    //        Operations::validateValueField(j["ships_parameters"], "detection_range", 0, 999999);
    //    });

    addValidator("search_region", [](const nlohmann::json& j) { Operations::validateRegion(j, -999999, +999999); });

    addValidator("time", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "time", 0, 999999);
    });
}

void Input::initializeProperties(const nlohmann::json& json)
{
    // Район поиска
    Operations::setRegionFromJson(region, json["search_region"]);

    // initialPosition = Point2D{-4, 4};
    ship.fromJson(json["ships_parameters"]);

    searchTime = json["time"].get<double>();

    // TODO: Это делать надо в конкретном месте, а не менять универсальные входные данные
    //Operations::metricToNauticalInput<Input>(*this);
}
} // namespace InRegion
} // namespace Search
} // namespace Schemes
