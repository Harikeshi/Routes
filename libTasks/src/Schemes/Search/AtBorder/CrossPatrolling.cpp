#include <Task/Schemes/Search/AtBorder/CrossPatrolling.hpp>

#include <Task/Schemes/Search/Config.hpp>

namespace Schemes {
namespace Search {
namespace AtBorder {

CrossPatrolling::CrossPatrolling(const Input& input)
    : input{input}
{
}

Outputs::SchemeEfficiency CrossPatrolling::probability()
{
    std::cout << "probability CrossPatrolling" << std::endl;
    return Outputs::SchemeEfficiency{};
}

Outputs::Route CrossPatrolling::calculate()
{
    calculateParameters(); /// Расчёт параметров патрулирования

    const BorderedLine setBorder{input.setBorder[0], input.setBorder[1]};
    const double borderLength = setBorder.getLength();
    const double observedLength = parameters.patrollingLength + 2 * parameters.lengthDifference;

    /// Исключение, если скорость цели больше скорости наблюдателя
    if (angleSine > 1)
    {
        throw std::runtime_error("Невозможно построить схему при данном отношении скоростей наблюдателя и цели");
    }
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
    std::vector<Point2D> positions = calculatePositions();
    positions.push_back(positions[0]);
    return Outputs::Route{positions, std::vector<double>(4, input.ship.searchVelocity)};
}

std::vector<Point2D> CrossPatrolling::calculatePositions()
{
    BorderedLine crossProjection = positionsAtBorder(BorderedLine{input.setBorder[0], input.setBorder[1]}, parameters.lengthDifference);
    std::vector<Point2D> positions;
    positions.push_back(crossProjection.findPointAtPerpendLine(crossProjection.getStart(), parameters.patrollingLength / 2 * angleSine, true));
    positions.push_back(crossProjection.findPointAtPerpendLine(crossProjection.getFinish(), parameters.patrollingLength / 2 * angleSine, false));
    positions.push_back(crossProjection.findPointAtPerpendLine(crossProjection.getStart(), parameters.patrollingLength / 2 * angleSine, false));
    positions.push_back(crossProjection.findPointAtPerpendLine(crossProjection.getFinish(), parameters.patrollingLength / 2 * angleSine, true));
    return positions;
}

void CrossPatrolling::calculateParameters()
{
    parameters = patrollingParameters(input.ship.searchVelocity, input.maxTargetVel, input.ship.detectionRange, input.ship.turningRadius, "cross");
    angleSine = input.maxTargetVel / input.ship.searchVelocity;
}

void CrossPatrolling::correctPatrollingParameters(double difference)
{
    /// @todo реализовать по намеченному алгоритму
    parameters.patrollingLength -= difference;
}
} // namespace AtBorder
} // namespace Search
} // namespace Schemes
