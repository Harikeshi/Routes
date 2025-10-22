#pragma once

#include <Task/Abstractions/Input.hpp>

/*! Core */
#include <Point/Point2D.hpp>

namespace Schemes {
namespace Restore {

using point = Point2D;
using ty = uint32_t;

struct Input : public Abstractions::Input
{
    //::Restore::inputParameters parameters;
    point Tpk;               //        точка потери контакта
    point Tbek_0;            //     исходная точка БНК № 0 , в момент потери контакта
    point Tbek_exit;         //  конечная точка БНК, для выхода из района маневрирования по схеме
    double Dpgas;            //     miles дальность  подкильной ГАС (по сути - "радиус"  диаграммы направленности )
    double Dbgas;            //     miles дальность
    double Kt_deg;           //    курс цели в градусах, в момент потери контакта (он же будет оставаться предполагамым курсом цели во время вторичного поиска)
    double Kbek_deg;         //  курс БНК в  t=0 - момент потери контакта.
    double Ut;               //        Vtc,   knots - скорость искомой цели
    double Ukp;              //       Vkp ,  knots - скорость БНК поисковая
    double Ukph;             //      Vkph,  knots - скорость БНК полного хода.
    double Ukp_r;            //     Vkp_raschetnaja ,  knots - скорость БНК поискова расчетная на последнем участке СТБМ
    float P_tab_calm = 0.8;  // табличная величина Pконтакта в случае до 3х баллов
    float P_tab_storm = 0.7; //табличная величина Pконтакта в случае свыше 3х баллов
    //

    Input(const Input& input);

    Input();

    Input& operator=(const Input& input);

    void initializeProperties(const nlohmann::json& json) override;
};

//Input &Input::operator=(Input& input);

} // namespace Restore
} // namespace Schemes
