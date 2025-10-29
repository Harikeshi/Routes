#pragma once

namespace Models {
struct Parameters
{
    // perimeter
    /// минимальное траверзное расстояние
    double traversaMin;

    /// минимальная длина галса
    double tackDistMin;

    // algorithms
    /// левая граница длины галса (минимальная длина при отсутствии ограничений)
    double tackDistLeft;

    /// минимальный угол поворота
    double turnAngleDegMin;

    /// максимальный угол поворота
    double turnAngleDegMax;

    /// параметр наклона линейной функции распределения длины галса
    double distributionCoefficient;

    Parameters()
    {
        traversaMin = 500;
        tackDistMin = 1000;
        tackDistLeft = 3000;
        distributionCoefficient = 2;
        turnAngleDegMin = 30;
        turnAngleDegMax = 80;
    }

    // Schemes::Search::InRegion::ShiftParameters getShiftParameters() const
    // {
    //     return Schemes::Search::InRegion::ShiftParameters{tackDistLeft, distributionCoefficient};
    // }

    // Schemes::Search::InRegion::ZigzagParameters getZigzagParameters() const
    // {
    //     Schemes::Search::InRegion::ZigzagParameters params;
    //     params.tackDistLeft = tackDistLeft;
    //     params.turnAngleDegMin = turnAngleDegMin;
    //     params.turnAngleDegMax = turnAngleDegMax;
    //
    //     return params;
    // }
    //
    // Entities::RegionParameters getRegParameters() const
    // {
    //     return Entities::RegionParameters{traversaMin, tackDistMin};
    // }
};
} // namespace Models
