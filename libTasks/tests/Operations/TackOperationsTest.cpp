#include <Geometry/Objects/BorderedLine.hpp>
#include <Point/Point2D.hpp>
#include <Task/Operations/Random.hpp>
#include <Task/Operations/TackOperations.hpp>
#include <cmath>
#include <gtest/gtest.h>

namespace Operations {

TEST(TackOperations, chordSegment)
{
    Point2D projection{0.5, 0};
    Point2D base{2, 0};
    double radius = 1;
    double distToLine = 0.5;
    double res = std::sqrt(0.75) - 0.5;
    EXPECT_EQ(chordSegment(projection, base, radius, distToLine), res);
}

TEST(TackOperations, obsevationLength_inCircle)
{
    Geometry::BorderedLine tack{{1, 0}, {4, 0}};
    double radius = 1.;
    Point2D point1{1.5, 0.5};
    double res = std::sqrt(0.75) + 0.5;
    EXPECT_EQ(observationLength(tack, point1, radius), res);

    Point2D point2{4, -0.5};
    EXPECT_EQ(observationLength(tack, point2, radius), std::sqrt(0.75));

    Point2D point3{1, 1};
    EXPECT_EQ(observationLength(tack, point3, radius), 0);
}

TEST(TackOperations, obsevationLength_betweenCircles)
{
    Geometry::BorderedLine tack{{1, 0}, {4, 0}};
    double radius = 1.;
    Point2D point1{2.4, 0.5};
    double res = 2 * std::sqrt(0.75);
    EXPECT_EQ(observationLength(tack, point1, radius), res);

    Point2D point2{3, -1};
    EXPECT_EQ(observationLength(tack, point2, radius), 0);
}

TEST(TackOperations, observationLength_45deg)
{
    Geometry::BorderedLine tack{{1000, 1000}, {3000, 3000}};
    double radius = 1000;
    Point2D point1{1500, 2500};
    double res = 1000 * std::sqrt(2);
    EXPECT_EQ(observationLength(tack, point1, radius), res);

    Point2D pntOut{1500, 3000};
    EXPECT_ANY_THROW(observationLength(tack, pntOut, radius));
}

} // namespace Operations
