#include <Task/Schemes/Search/ByCall/StraightTack.hpp>
#include <gtest/gtest.h>

using namespace Schemes::Search::ByCall;

TEST(StraightTack, NoThrow)
{
    Input input;
    input.initialPosition = Point2D(-75, 150);
    input.ship.detectionRange = 5;
    input.ship.maxVelocity = 18;
    input.ship.searchVelocity = 12;

    input.target.contactPoint = Point2D(10, 10);
    input.target.contactRms = 2;
    input.target.midVelocity = 15;
    input.target.minVelocity = 10;
    input.target.maxVelocity = 30;
    input.target.timeSinceContact = 0.5;

    input.maxSearchTime = 30;

    StraightTack straight{input};

    EXPECT_NO_THROW(straight.calculate());

}
