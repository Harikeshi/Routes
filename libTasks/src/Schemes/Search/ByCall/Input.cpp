#include <Task/Schemes/Search/ByCall/Input.hpp>

#include <Task/Operations/ValidationOperations.hpp>

namespace Schemes {
namespace Search {
namespace ByCall {

Input::Input()
{
    addValidator("maxSearchTime", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "time", 0, 999999);
    });

    /*addValidator("initialPosition", [](const nlohmann::json& j) {
        Operations::validatePoint2D(j, "initialPosition", 0, 999999);
    });*/
}
void Input::initializeProperties(const nlohmann::json& json)
{
    initialPosition = Point2D(0, 140000);

    //    shipsNmb = json["ships_parameters"]["count"];
    ship.fromJson(json["ships_parameters"]);
    target.fromJson(json["form_target"]);

    maxSearchTime = json["time"].get<double>();
}
} // namespace ByCall
} // namespace Search
} // namespace Schemes
