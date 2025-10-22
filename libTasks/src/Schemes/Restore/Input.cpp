#include <Task/Schemes/Restore/Input.hpp>

#include <Task/Operations/JsonOperations.hpp>
#include <Task/Operations/ValidationOperations.hpp>

namespace Schemes {
namespace Restore {

Input::Input()
{
    addValidator("Tpk", [](const nlohmann::json& j) {
        Operations::validatePoint2D(j, "Tpk", 0, 999999);
    });

    addValidator("Tbek_0", [](const nlohmann::json& j) {
        Operations::validatePoint2D(j, "Tbek_0", 0, 999999);
    });

    addValidator("Tbek_exit", [](const nlohmann::json& j) {
        Operations::validatePoint2D(j, "Tbek_exit", 0, 999999);
    });

    addValidator("Dpgas", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "Dpgas", 0, 999999);
    });

    addValidator("Dbgas", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "Dbgas", 0, 999999);
    });

    addValidator("Kt_deg", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "Kt_deg", -180, +180);
    });

    addValidator("Kbek_deg", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "Kbek_deg", -360, +360);
    });

    addValidator("Ut", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "Ut", 0, 150);
    });

    addValidator("Ukp", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "Ukp", 0, 150);
    });

    addValidator("Ukph", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "Ukph", 0, 150);
    });

    addValidator("Ukp_r", [](const nlohmann::json& j) {
        Operations::validateValueField(j, "Ukp_r", 0, 999999);
    });
}

Input::Input(const Input& input)
    : Tpk(input.Tpk),
      Tbek_0(input.Tbek_0),
      Tbek_exit(input.Tbek_exit),
      Dpgas(input.Dpgas),
      Dbgas(input.Dbgas),
      Kt_deg(input.Kt_deg),
      Kbek_deg(input.Kbek_deg),
      Ut(input.Ut),
      Ukp(input.Ukp),
      Ukph(input.Ukph),
      Ukp_r(input.Ukp_r)
{
}

Input& Input::operator=(const Input& input)
{
    if (this == &input)
        return *this;

    this->Tpk = input.Tpk;             //        точка потери контакта
    this->Tbek_0 = input.Tbek_0;       //     исходная точка БНК № 0, в момент потери контакта
    this->Tbek_exit = input.Tbek_exit; //  конечная точка БНК, для выхода из района маневрирования по схеме
    this->Dpgas = input.Dpgas;         //     miles дальность  подкильной ГАС (по сути - "радиус"  диаграммы направленности )
    this->Dbgas = input.Dbgas;         //     miles дальность
    this->Kt_deg = input.Kt_deg;       //    курс цели в градусах, в момент потери контакта (он же будет оставаться предполагамым курсом цели во время вторичного поиска)
    this->Kbek_deg = input.Kbek_deg;   //  курс БНК в  t=0 - момент потери контакта.
    this->Ut = input.Ut;               //        Vtc,   knots - скорость искомой цели
    this->Ukp = input.Ukp;             //       Vkp ,  knots - скорость БНК поисковая
    this->Ukph = input.Ukph;           //      Vkph,  knots - скорость БНК полного хода.
    this->Ukp_r = input.Ukp_r;         //     Vkp_raschetnaja ,  knots - скорость БНК поискова расчетная на последнем участке СТБМ

    return *this;
}

void Input::initializeProperties(const nlohmann::json& json)
{
    // Точки
    Tpk = Operations::getPointFromJson(json["Tpk"]);
    Tbek_0 = Operations::getPointFromJson(json["Tbek_0"]);
    Tbek_exit = Operations::getPointFromJson(json["Tbek_exit"]);

    // Данные
    Dpgas = json["Dpgas"].get<double>();
    Dbgas = json["Dbgas"].get<double>();
    Kt_deg = json["Kt_deg"].get<double>();
    Kbek_deg = json["Kbek_deg"].get<double>();
    Ut = json["Ut"].get<double>();
    Ukp = json["Ukp"].get<double>();
    Ukph = json["Ukph"].get<double>();
    Ukp_r = json["Ukp_r"].get<double>();
}
} // namespace Restore
} // namespace Schemes
