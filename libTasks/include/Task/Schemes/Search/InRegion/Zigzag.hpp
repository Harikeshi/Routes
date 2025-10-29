#pragma once

#include <Task/Abstractions/Scheme.hpp>
#include <Task/Schemes/Search/InRegion/Input.hpp>

namespace Schemes {
namespace Search {
namespace InRegion {

struct ZigzagParameters
{
    /// левая граница длины галса (минимальная длина при отсутствии ограничений)
    double tackDistLeft;
    /// минимальный угол поворота
    double turnAngleDegMin;
    /// максимальный угол поворота
    double turnAngleDegMax;
    ZigzagParameters()
    {
        tackDistLeft = TACK_DIST_LEFT;
        turnAngleDegMin = TURN_ANGLE_MIN_DEG;
        turnAngleDegMax = TURN_ANGLE_MAX_DEG;
    }
    ZigzagParameters(double tackDistLeft, double turnAngleDegMin, double turnAngleDegMax)
        : tackDistLeft(tackDistLeft), turnAngleDegMin(turnAngleDegMin), turnAngleDegMax(turnAngleDegMax)
    {
    }
};

/**
 * @brief Класс схемы Зигзаг.
 * 
 */
class Zigzag : public Abstractions::Scheme
{
    const Input& input; //!< Входные данные схемы.
    ZigzagParameters parameters;

public:
    /**
 * @brief Конструктор.
 * 
 * @param input Входящие данные.
 */
    explicit Zigzag(const Input& input);

    /*!
     * \brief Конструктор Zigzag
     * \param input структура входных данных
     * \param parameters параметры алгоритма
     */
    Zigzag(const Input& input, const ZigzagParameters& parameters);

    /**
 * @brief Метод расчета вероятности схемы Зигзаг.
 * 
 * @return Outputs::SchemeEfficiency Результат расчета вероятности схемы. 
 */
    Outputs::SchemeEfficiency probability() override;

    /**
 * @brief Метод расчета траектории схемы Зигзаг.
 *
 * @return Outputs::Route Путь рассчитанный в схеме.
 */
    Outputs::Route calculate() override;
};
} // namespace InRegion
} // namespace Search
} // namespace Schemes
