#include <Task/Schemes/Search/AtBorder/LinearPatrolling.hpp>

#include <Task/Schemes/Search/Config.hpp>

namespace Schemes {
namespace Search {
namespace AtBorder {

LinearPatrolling::LinearPatrolling(const Input& input)
    : input{input}
{
}

Outputs::SchemeEfficiency LinearPatrolling::probability()
{
    std::cout << "probability LinearPatrolling" << std::endl;
    return Outputs::SchemeEfficiency{};
}

Outputs::Route LinearPatrolling::calculate()
{
    calculateParameters(); /// Расчёт параметров патрулирования

    const BorderedLine setBorder{input.setBorder[0], input.setBorder[1]};
    const double borderLength = setBorder.getLength();
    const double observedLength = parameters.patrollingLength + 2 * parameters.lengthDifference;

    /// Сравниваем длины рубежа и обозреваемого участка
    if (observedLength < MIN_BORDER_CLOSING_PROBABILITY * borderLength)
    {
        throw std::runtime_error("Невозможно контролировать рубеж с требуемой вероятностью. Нужно уменьшить длину рубежа");
    }
    else if (observedLength - borderLength > ACCURACY_DOUBLE * borderLength)
    {
        /// Уменьшаем длины галсов и/или обзоров(охватов), что бы "умещаться" в рубеж
        correctPatrollingParameters(observedLength - borderLength);
    }
    BorderedLine positions = positionsAtBorder(setBorder, parameters.lengthDifference);
    return Outputs::Route{{positions.getStart(), positions.getFinish()}, {input.ship.searchVelocity}};
}

void LinearPatrolling::calculateParameters()
{
    parameters = patrollingParameters(input.ship.searchVelocity, input.maxTargetVel, input.ship.detectionRange, input.ship.turningRadius, "linear");
}

void LinearPatrolling::correctPatrollingParameters(double difference)
{
    /// @todo реализовать по намеченному алгоритму
    parameters.patrollingLength -= difference;
}
} // namespace AtBorder
} // namespace Search
} // namespace Schemes
