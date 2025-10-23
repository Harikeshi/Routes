#include <Task/Entities/SearchRing.hpp>

#include <gtest/gtest.h>

namespace Entities {
using SearchRing = Entities::SearchRing;

TEST(SearchRingIntersectionsTests, RingIntersection_defaultCtor)
{
    RingIntersection rInter;
    EXPECT_TRUE(rInter.empty());
    EXPECT_THROW(Intersection inter = rInter[0], std::out_of_range);
}

TEST(SearchRingIntersectionsTests, intersectionAll_Diagonal)
{
    SearchRing ring{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}};
    Geometry::Line line{Point2D{-2, 0}, Point2D{6, 4}};

    auto result = ring.intersectionsAllCore(line);

    //EXPECT_EQ(result.size(), 2);
    EXPECT_NEAR(result[0].point.getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0].point.getY(), 1, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0].vertexIndex, 0);
    EXPECT_FALSE(result[0].isVertex);
    EXPECT_NEAR(result[1].point.getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[1].point.getY(), 3, ACCURACY_DOUBLE);
    EXPECT_EQ(result[1].vertexIndex, 2);
    EXPECT_FALSE(result[1].isVertex);
}

TEST(SearchRingIntersectionsTests, intersectionAll_SameLinesIntersect)
{
    SearchRing ring{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}};

    Geometry::Line line(Point2D{-1, 4}, Point2D{5, 4});

    auto result = ring.intersectionsAllCore(line);

    //EXPECT_EQ(result.size(), 2);
    EXPECT_NEAR(result[0].point.getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0].point.getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0].vertexIndex, 1);
    EXPECT_TRUE(result[0].isVertex);
    EXPECT_NEAR(result[1].point.getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[1].point.getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(result[1].vertexIndex, 2);
    EXPECT_TRUE(result[1].isVertex);
}

TEST(SearchRingIntersectionsTests, intersectionAll_ParallelLinesIntersect)
{
    SearchRing ring{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}};

    Geometry::Line line(Point2D{-1, 2}, Point2D{5, 2});

    auto result = ring.intersectionsAllCore(line);

    //EXPECT_EQ(result.size(), 2);
    EXPECT_NEAR(result[0].point.getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0].point.getY(), 2, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0].vertexIndex, 0);
    EXPECT_FALSE(result[0].isVertex);
    EXPECT_NEAR(result[1].point.getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[1].point.getY(), 2, ACCURACY_DOUBLE);
    EXPECT_EQ(result[1].vertexIndex, 2);
    EXPECT_FALSE(result[1].isVertex);
}

TEST(SearchRingIntersectionsTests, intersectionAll_Vertex)
{
    SearchRing ring{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}};

    Geometry::Line line(Point2D{-4, 0}, Point2D{4, 4});

    auto result = ring.intersectionsAllCore(line);

    //EXPECT_EQ(result.size(), 2);
    EXPECT_NEAR(result[0].point.getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0].point.getY(), 2, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0].vertexIndex, 0);
    EXPECT_FALSE(result[0].isVertex);
    EXPECT_NEAR(result[1].point.getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[1].point.getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(result[1].vertexIndex, 2);
    EXPECT_TRUE(result[1].isVertex);
}

TEST(SearchRingIntersectionsTests, intersectionAll_VertexDouble)
{
    SearchRing ring{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}};

    Geometry::Line line(Point2D{0, 0}, Point2D{4, 4});

    auto result = ring.intersectionsAllCore(line);

    //EXPECT_EQ(result.size(), 2);
    EXPECT_NEAR(result[0].point.getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0].point.getY(), 0, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0].vertexIndex, 0);
    EXPECT_TRUE(result[0].isVertex);
    EXPECT_NEAR(result[1].point.getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[1].point.getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(result[1].vertexIndex, 2);
    EXPECT_TRUE(result[1].isVertex);
}

TEST(SearchRingIntersectionsTests, intersectionAll_TouchVertex)
{
    SearchRing ring{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}};

    Geometry::Line line(Point2D{3, -1}, Point2D{5, 1});

    auto result = ring.intersectionsAllCore(line);

    //EXPECT_EQ(result.size(), 1);
    EXPECT_NEAR(result[0].point.getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0].point.getY(), 0, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0].vertexIndex, 3);
    EXPECT_TRUE(result[0].isVertex);
}
} // namespace Entities
