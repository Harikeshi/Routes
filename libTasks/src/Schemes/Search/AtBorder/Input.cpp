#include <Task/Schemes/Search/AtBorder/Input.hpp>

#include <Task/Operations/JsonOperations.hpp>
#include <Task/Operations/ValidationOperations.hpp>

namespace Schemes {
namespace Search {
namespace AtBorder {

Input::Input()
{
    //    addValidator("shipsNmb", [](const nlohmann::json& j) {
    //        Operations::validateValueField(j["ships_parameters"], "count", 0, 999999);
    //    });

    addValidator("maxTargetVel", [](const nlohmann::json& j) {
        Operations::validateValueField(j["form_target"]["velocities"], "max", 0, 999999);
    });

    addValidator("setBorder", [](const nlohmann::json& j) {
        //std::cout << j.dump(4) << std::endl;
        Operations::validateArrayField(j, "border_line");
    });
}

void Input::initializeProperties(const nlohmann::json& json)
{
    //    shipsNmb = json["ships_parameters"]["count"]; //"count": 99,

    ship.fromJson(json["ships_parameters"]);

    maxTargetVel = json["form_target"]["velocities"]["max"].get<double>();

    //setBorder = json["border_line"].get<std::vector<Point2D>>();
    Operations::setVectorPointsFromJson(setBorder, json["border_line"]);

    //Operations::metricToNauticalInput<Input>(*this);
}
} // namespace AtBorder
} // namespace Search
} // namespace Schemes
