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
};
} // namespace Models
