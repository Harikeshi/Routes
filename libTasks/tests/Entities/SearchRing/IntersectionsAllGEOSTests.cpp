#include <Task/Entities/SearchRing.hpp>
#include <gtest/gtest.h>

namespace Entities {
using SearchRing = Entities::SearchRing;

TEST(SearchRingIntersectionsGEOSTests, intersectionAll_Diagonal)
{
    SearchRing ring{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}};

    auto result = ring.intersectionsAllGEOS(Point2D{-2, 0}, Point2D{6, 4});

    EXPECT_EQ(result.size(), 2);

    EXPECT_NEAR(result[0].point.getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0].point.getY(), 1, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0].vertexIndex, 0);
    EXPECT_FALSE(result[0].isVertex);
    EXPECT_NEAR(result[1].point.getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[1].point.getY(), 3, ACCURACY_DOUBLE);
    EXPECT_EQ(result[1].vertexIndex, 2);
    EXPECT_FALSE(result[1].isVertex);
}

TEST(SearchRingIntersectionsGEOSTests, intersectionAll_SameLinesIntersect)
{
    SearchRing ring{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}};

    auto result = ring.intersectionsAllGEOS(Point2D{-1, 4}, Point2D{5, 4});

    EXPECT_EQ(result.size(), 2);
    EXPECT_NEAR(result[0].point.getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0].point.getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0].vertexIndex, 1);
    EXPECT_TRUE(result[0].isVertex);
    EXPECT_NEAR(result[1].point.getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[1].point.getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(result[1].vertexIndex, 2);
    EXPECT_TRUE(result[1].isVertex);
}

TEST(SearchRingIntersectionsGEOSTests, intersectionAll_ParallelLinesIntersect)
{
    SearchRing ring{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}};

    auto result = ring.intersectionsAllGEOS(Point2D{-1, 2}, Point2D{5, 2});

    EXPECT_EQ(result.size(), 2);
    EXPECT_NEAR(result[0].point.getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0].point.getY(), 2, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0].vertexIndex, 0);
    EXPECT_FALSE(result[0].isVertex);
    EXPECT_NEAR(result[1].point.getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[1].point.getY(), 2, ACCURACY_DOUBLE);
    EXPECT_EQ(result[1].vertexIndex, 2);
    EXPECT_FALSE(result[1].isVertex);
}

TEST(SearchRingIntersectionsGEOSTests, intersectionAll_Vertex)
{
    SearchRing ring{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}};

    auto result = ring.intersectionsAllGEOS(Point2D{-4, 0}, Point2D{4, 4});

    EXPECT_EQ(result.size(), 2);
    EXPECT_NEAR(result[0].point.getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0].point.getY(), 2, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0].vertexIndex, 0);
    EXPECT_FALSE(result[0].isVertex);
    EXPECT_NEAR(result[1].point.getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[1].point.getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(result[1].vertexIndex, 2);
    EXPECT_TRUE(result[1].isVertex);
}

TEST(SearchRingIntersectionsGEOSTests, intersectionAll_VertexDouble)
{
    SearchRing ring{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}};

    auto result = ring.intersectionsAllGEOS(Point2D{0, 0}, Point2D{4, 4});

    EXPECT_EQ(result.size(), 2);
    EXPECT_NEAR(result[0].point.getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0].point.getY(), 0, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0].vertexIndex, 0);
    EXPECT_TRUE(result[0].isVertex);
    EXPECT_NEAR(result[1].point.getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[1].point.getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(result[1].vertexIndex, 2);
    EXPECT_TRUE(result[1].isVertex);
}

TEST(SearchRingIntersectionsGEOSTests, intersectionAll_TouchVertex)
{
    SearchRing ring{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}};

    auto result = ring.intersectionsAllGEOS(Point2D{3, -1}, Point2D{5, 1});

    EXPECT_EQ(result.size(), 1);
    EXPECT_NEAR(result[0].point.getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0].point.getY(), 0, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0].vertexIndex, 3);
    EXPECT_TRUE(result[0].isVertex);
}

TEST(SearchRingIntersectionsGEOSTests, intersectionAll_Hight_First_Vertex)
{
    double current = 1e12;

    SearchRing ring{{current + 100, current}, {current + 100, 0}, {current, 0}, {current, current}, {current + 100, current}};

    auto result = ring.intersectionsAllGEOS(Point2D{0, 0}, Point2D{1e16, 1e16});

    EXPECT_EQ(result.size(), 1);
    EXPECT_NEAR(result[0].point.getX(), current, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0].point.getY(), current, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0].vertexIndex, 3);

    EXPECT_TRUE(result[0].isVertex);
}

TEST(SearchRingIntersectionsGEOSTests, intersectionAll_Hight_Second_Vertex)
{
    double current = 1e12;

    SearchRing ring{{current, current}, {current + 100, current}, {current + 100, 0}, {current, 0}, {current, current}};

    auto result = ring.intersectionsAllGEOS(Point2D{0, 0}, Point2D{1e16, 1e16});

    EXPECT_EQ(result.size(), 1);
    EXPECT_NEAR(result[0].point.getX(), current, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0].point.getY(), current, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0].vertexIndex, 0);

    EXPECT_TRUE(result[0].isVertex);
}
} // namespace Entities
