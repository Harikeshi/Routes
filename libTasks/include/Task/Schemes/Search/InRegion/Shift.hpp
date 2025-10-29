#pragma once

#include <Task/Abstractions/Scheme.hpp>
#include <Task/Schemes/Search/InRegion/Input.hpp>

namespace Schemes {
namespace Search {
namespace InRegion {

struct ShiftParameters
{
    /// левая граница длины галса (минимальная длина при отсутствии ограничений)
    double tackDistLeft;

    /// параметр наклона линейной функции распределения длины галса
    double distributionCoefficient;
    ShiftParameters()
    {
        tackDistLeft = TACK_DIST_LEFT;
        distributionCoefficient = 2;
    }
    ShiftParameters(double tackDistLeft, double distributionCoefficient)
        : tackDistLeft(tackDistLeft), distributionCoefficient(distributionCoefficient)
    {
    }
};

/**
 * @brief Класс схемы Сдвиг.
 * 
 */
class Shift : public Abstractions::Scheme
{
    const Input& input; //!< Входные данные схемы.
    ShiftParameters parameters;

public:
    /**
 * @brief Конструктор.
 * 
 * @param input Входящие данные.
 */
    explicit Shift(const Input& input);

    /*!
     * \brief Конструктор Shift
     * \param input структура входных данных
     * \param parameters параметры алгоритма
     */
    Shift(const Input& input, const ShiftParameters& parameters);

    /**
 * @brief Метод расчета траектории схемы Сдвиг.
 * 
 * @return Outputs::Route Путь рассчитанный в схеме.
 */
    Outputs::Route calculate() override;

    /**
 * @brief Метод расчета вероятности схемы Сдвиг.
 * 
 * @return Outputs::SchemeEfficiency Результат расчета вероятности схемы. 
 */
    Outputs::SchemeEfficiency probability() override;

    ~Shift() = default;
};
} // namespace InRegion
} // namespace Search
} // namespace Schemes
