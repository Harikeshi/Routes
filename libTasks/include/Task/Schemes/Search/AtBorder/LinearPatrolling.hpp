#pragma once

#include <Task/Abstractions/Scheme.hpp>
#include <Task/Schemes/Search/AtBorder/Input.hpp>
#include <Task/Schemes/Search/AtBorder/Utilities.hpp>

namespace Schemes {
namespace Search {
namespace AtBorder {
/**
 * @brief Класс схемы Линейное патрулирование. 
 * 
 */
class LinearPatrolling : public Abstractions::Scheme
{
    const Input& input; //!< Входные данные схемы.

    PatrollingParameters parameters; // TODO: Откуда и зачем?? Ошибка проектирования

public:
    /**
 * @brief Конструктор.
 * 
 * @param input Входящие данные.
 */
    explicit LinearPatrolling(const Input& input);

    /**
 * @brief Метод расчета вероятности схемы Линейного патрулирования.
 *
 * @return Outputs::SchemeEfficiency Результат расчета вероятности схемы.
 */
    Outputs::SchemeEfficiency probability() override;

    /**
     * @note не учитывает возможность нескольких рубежей
     *
    * \brief calculateLinearPatrolling получение точек БЭК на рубеже при патрулировании способом "Линейное патрулирование"
    * @return Outputs::Route Путь рассчитанный в схеме.
    */
    Outputs::Route calculate() override;

    /**
 * @brief Деструктор.
 *
 */
    ~LinearPatrolling() = default;

protected:
    /*!
     * \brief lensLinearPatrolling Расчёт параметров линейного патрулирования для группы БЭК
     * \details Расчёт длин галсов и охвата(обзора) для каждого БЭК
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
