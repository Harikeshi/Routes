#include <Task/Schemes/Search/ByCall/StraightTack.hpp>

#include <Task/Operations/Geometry/Vector2D.hpp>
#include <Task/Schemes/Search/ByCall/Utilities.hpp>
#include <Task/Schemes/Search/Config.hpp>

namespace Schemes {
namespace Search {
namespace ByCall {

using Vector2D = Operations::Vector2D<Point2D>;

StraightTack::StraightTack(const Input& input)
    : input{input}
{
}

Outputs::Route StraightTack::calculate()
{
    if (input.target.timeSinceContact > MAX_DELAY_TIME)
    {
        throw std::invalid_argument("Слишком большое время запаздывания");
    }
    if (input.ship.maxVelocity < input.ship.searchVelocity)
    {
        throw std::invalid_argument("Максимальная скорость меньше поисковой");
    }

    std::vector<Point2D> positions(3);
    std::vector<double> velocities{input.ship.maxVelocity, input.ship.searchVelocity};

    /// Угол наклона прямой между БЭК и ПЛ
    const Radian beta = Vector2D(input.initialPosition, input.target.contactPoint).getCoreCourse();
    /// Расстояние между БЭК и ПЛ с учётом ошибки местоположения, времени запаздывания и радиуса поиска
    const double distance = Vector2D(input.initialPosition, input.target.contactPoint).length() - (input.target.contactRms + input.target.midVelocity * input.target.timeSinceContact) - input.ship.detectionRange;
    /// Время попадания ПЛ в радиус действия ГАС БЭКа при условии, что ПЛ двигался навстречу БЭК
    double tMeet = distance / (input.ship.maxVelocity + input.target.midVelocity);
    /// Оставшееся время поиска
    double tSearch = input.maxSearchTime - tMeet;

    positions[0] = input.initialPosition;                                                   /// Первая точка
    positions[1] = Vector2D(input.initialPosition, tMeet * input.ship.maxVelocity, beta).e; /// Точка БЭК на момент времени tMeet
    positions[2] = Vector2D(positions[1], tSearch * input.ship.searchVelocity, beta).e;     /// Последняя точка БЭК в рамках этого задания

    return Outputs::Route{positions, velocities};
}
} // namespace ByCall
} // namespace Search
} // namespace Schemes
