#include <Task/Operations/Geometry/AngularOperations.hpp>
#include <Task/Schemes/Search/ByCall/Utilities.hpp>
#include <gtest/gtest.h>
#include <iostream>

using namespace Operations;
using namespace Schemes::Search::ByCall;

TEST(byCallUtils, calculateClosingSpeed)
{
    // Act & Assert
    EXPECT_NEAR(calculateClosingSpeed(0, 10, Radian(0)), 10, ACCURACY_DOUBLE);
    EXPECT_NEAR(calculateClosingSpeed(10, 0, Radian(M_PI)), 10, ACCURACY_DOUBLE);
    EXPECT_NEAR(calculateClosingSpeed(10., 10., Radian(M_PI / 3)), 10., ACCURACY_DOUBLE);
}

TEST(byCallUtils, calculateOrderAngle)
{
    EXPECT_THROW(calculateOrderAngle(0, 10, 10, 10), std::invalid_argument);
    EXPECT_THROW(calculateOrderAngle(10, 10, 10, 0), std::invalid_argument);
}

TEST(sectorSpiral, positionInSector)
{
    Point2D shipPosition{-75, 150};
    Radian alpha(Degree{90});
    double vTarget = 0;
    double vShip = 10;
    Point2D targetPosition{10, 12};

    EXPECT_NEAR(positionInSector(targetPosition, shipPosition, alpha, vShip, vTarget).getX(), 10., ACCURACY_DOUBLE);
    EXPECT_NEAR(positionInSector(targetPosition, shipPosition, alpha, vShip, vTarget).getY(), 12, ACCURACY_DOUBLE);
}

TEST(sectorSpiral, positionInSectorThrow)
{
    Point2D shipPosition{-75, 90};
    Radian alpha(Degree{90});
    double vTargetMax = 30;
    double vShipMax = 18;
    Point2D targetPosition{10, 12};

    EXPECT_THROW(positionInSector(targetPosition, shipPosition, alpha, vShipMax, vTargetMax), std::runtime_error);

    double vTarget = 20;
    EXPECT_NO_THROW(positionInSector(targetPosition, shipPosition, alpha, vShipMax, vTarget));
}

TEST(tasks, vThrow)
{
    //Input input;
    //initializeInput(input);

    //    std::vector<double> order_vels{};
    //    const std::vector<std::vector<Point2D>> points  = ByCall::calculateStraightTack(input, order_vels);

    //    EXPECT_TRUE(timecheck_straight_tack(input.tMax, input.vSearch, points, order_vels));
    //EXPECT_TRUE(1);

    /*Input input;
    initializeInput(input);
    Point2D start = Point2D(-75, 150);
    Radian alpha(toRad(input.alphaRightDeg));
    double vTarget = 10;
    double vShip = 10;

    auto a = positionInSector(input, start, alpha, vShip, vTarget);

    EXPECT_THROW(positionInSector(input, start, alpha, vShip, vTarget), std::runtime_error);*/
}
