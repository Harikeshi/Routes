#include <Point/Point2D.hpp>
#include <Task/Operations/Geometry/SegmentOperations.hpp>
#include <gtest/gtest.h>

using namespace Operations;

TEST(SegmentOperations, pointToSegmentDistance_projectionInside)
{
    Point2D A(0, 0);
    Point2D B(3, 0);
    Point2D P1(2, 1);
    EXPECT_EQ(pointToSegmentDistance(P1, A, B), 1.);
    Point2D P2(0, -2);
    EXPECT_EQ(pointToSegmentDistance(P2, A, B), 2);
}

TEST(SegmentOperations, pointToSegmentDistance_projectionOutside)
{
    Point2D A(0, 0);
    Point2D B(-3, 3);
    Point2D P1(2, 0);
    EXPECT_EQ(pointToSegmentDistance(P1, A, B), 2);
    Point2D P2(-3, 5);
    EXPECT_EQ(pointToSegmentDistance(P2, A, B), 2);
}

TEST(SegmentOperations, pointToSegmentDistance_belongs)
{
    Point2D A(0, 0);
    Point2D B(-3, 0);
    Point2D P1(-1, 0);
    EXPECT_EQ(pointToSegmentDistance(P1, A, B), 0);
}
