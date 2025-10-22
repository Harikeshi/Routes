#pragma once

#include <Task/Abstractions/Scheme.hpp>
#include <Task/Schemes/Search/AtBorder/Input.hpp>
#include <Task/Schemes/Search/AtBorder/Utilities.hpp>

using Geometry::BorderedLine;

namespace Schemes {
namespace Search {
namespace AtBorder {
/**
 * @brief Класс схемы Линейное патрулирование.
 *
 */
class CrossPatrolling : public Abstractions::Scheme
{
    const Input& input; //!< Входные данные схемы.

    PatrollingParameters parameters; // TODO: Откуда и зачем?? Ошибка проектирования
    double angleSine;

public:
    /**
 * @brief Конструктор.
 *
 * @param input Входящие данные.
 */
    explicit CrossPatrolling(const Input& input);

    /**
 * @brief Метод расчета вероятности схемы Линейного патрулирования.
 *
 * @return Outputs::SchemeEfficiency Результат расчета вероятности схемы.
 */
    Outputs::SchemeEfficiency probability() override;

    /**
     * @note не учитывает возможность нескольких рубежей
     *
    * \brief calculateCrossPatrolling получение точек БЭК на рубеже при патрулировании способом "Линейное патрулирование"
    * @return Outputs::Route Путь рассчитанный в схеме.
    */
    Outputs::Route calculate() override;

    /**
 * @brief Деструктор.
 *
 */
    ~CrossPatrolling() = default;

protected:
    /*!
     * \brief calculatePositions Получение точек патрулирования 1го БЭК на рубеже при патрулировании способом "Перекрестное патрулирование"
     * \param input[in]
     * \return 4 точки "восьмерки"
     * 1) слева от начала "отрезка патрулирования"
     * 2) справа от конца "отрезка патрулирования" и т. д.
     */
    std::vector<Point2D> calculatePositions();

    /*!
     * \brief lensCrossPatrolling Расчёт параметров линейного патрулирования для БЭК
     * \param input[in]
     */
    void calculateParameters();

    ///@note Возможно стоит переименовать?
    /*!
     * \brief regulateCruising Регулирование галсов
     * \details Уменьшение галсов и длин охвата(обзора), что бы не выходить за пределы рубежа
     * \param input Входящая структура задания
     * \param difference[in] Разница между максимальным охватом группы БЭК и длиной рубежа
     * \param average[in] Средняя длина галса (длина рубежа / количество БЭК)
     */
    void correctPatrollingParameters(double difference);
};

} // namespace AtBorder
} // namespace Search
} // namespace Schemes
