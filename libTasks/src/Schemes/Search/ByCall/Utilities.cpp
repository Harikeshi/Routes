#include <Task/Schemes/Search/ByCall/Utilities.hpp>

#include <Task/Operations/Vector2D.hpp>

namespace Schemes {
namespace Search {
namespace ByCall {

using Vector2D = Operations::Vector2D<Point2D>;

Point2D positionInSector(const Point2D& lCurr, const Point2D& mCurr, const Radian& alpha, const double& vFinder, const double& vL)
{
    Point2D position;
    double vClosing, tClosing;
    Radian beta;
    //    lCurr = Vector2D(input.l0, vL * input.t0 + input.delta, alpha).e;
    beta = alpha - Vector2D(lCurr, mCurr).getCoreCourse();
    if (vFinder <= vL && std::abs(beta.get()) >= asin(vFinder / vL))
    {
        throw std::runtime_error("Схема не может быть построена, курсовой угол больше критического");
    }
    vClosing = calculateClosingSpeed(vFinder, vL, beta.normalize());
    tClosing = Vector2D(lCurr, mCurr).length() / vClosing;
    position = Vector2D(lCurr, vL * tClosing, alpha).e;
    return position;
}

std::vector<Point2D> positionsInSector(const Input& input, const Radian& alphaFirst, const Radian& alphaSecond, bool isStandart)
{
    std::vector<Point2D> positions;
    Point2D lCurr, pFirst, pSecond, pThird;
    double t1 = 0., t2 = 0.;
    Radian alphaThird = ((alphaFirst + alphaSecond) / 2).normalize();
    positions.push_back(input.initialPosition);
    try
    {
        if (isStandart)
        {
            lCurr = Vector2D(input.target.contactPoint, input.target.contactVelocity * input.target.timeSinceContact + input.target.contactRms, alphaFirst).e;
            pFirst = positionInSector(lCurr, input.initialPosition, alphaFirst, input.ship.maxVelocity, input.target.contactVelocity);
            t1 = Vector2D(input.initialPosition, pFirst).length() / input.ship.maxVelocity;
            lCurr = Vector2D(input.target.contactPoint, input.target.contactVelocity * (input.target.timeSinceContact + t1) + input.target.contactRms, alphaSecond).e;
            pSecond = positionInSector(lCurr, pFirst, alphaSecond, input.ship.searchVelocity, input.target.contactVelocity);
            t2 = Vector2D(pFirst, pSecond).length() / input.ship.searchVelocity;
            lCurr = Vector2D(input.target.contactPoint, input.target.contactVelocity * (input.target.timeSinceContact + t1 + t2) + input.target.contactRms, alphaThird).e;
            pThird = positionInSector(lCurr, pSecond, alphaThird, input.ship.searchVelocity, input.target.contactVelocity);
        }
        else
        {
            lCurr = Vector2D(input.target.contactPoint, input.target.maxVelocity * input.target.timeSinceContact + input.target.contactRms, alphaFirst).e;
            pFirst = positionInSector(lCurr, input.initialPosition, alphaFirst, input.ship.maxVelocity, input.target.maxVelocity);
            t1 = Vector2D(input.initialPosition, pFirst).length() / input.ship.maxVelocity;
            lCurr = Vector2D(input.target.contactPoint, input.target.midVelocity * (input.target.timeSinceContact + t1) + input.target.contactRms, alphaSecond).e;
            pSecond = positionInSector(lCurr, pFirst, alphaSecond, input.ship.searchVelocity, input.target.midVelocity);
            t2 = Vector2D(pFirst, pSecond).length() / input.ship.searchVelocity;
            lCurr = Vector2D(input.target.contactPoint, input.target.minVelocity * (input.target.timeSinceContact + t1 + t2) + input.target.contactRms, alphaThird).e;
            pThird = positionInSector(lCurr, pSecond, alphaThird, input.ship.searchVelocity, input.target.minVelocity);
        }
    }
    catch (const std::runtime_error& e)
    {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
    positions.push_back(pFirst);
    positions.push_back(pSecond);
    positions.push_back(pThird);
    return positions;
}

} // namespace ByCall
} // namespace Search
} // namespace Schemes
