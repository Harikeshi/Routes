#include <Task/Entities/SearchRegion.hpp>

#include <gtest/gtest.h>

namespace Entities {

class SearchRegionIntersectionGEOSTest : public ::testing::Test
{
protected:
    SearchRegion region1;
    SearchRegion region2;
    SearchRegion region3;
    void SetUp() override
    {
        region1 = SearchRegion{{SearchRing{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}}},
                               {0, 0},
                               {0, 4}};

        region2 = SearchRegion{{SearchRing{{0, 0}, {10, 0}, {10, 10}, {0, 10}, {0, 0}},
                                SearchRing{{1, 1}, {4, 1}, {4, 4}, {1, 4}, {1, 1}},
                                SearchRing{{6, 1}, {9, 1}, {9, 4}, {6, 4}, {6, 1}},
                                SearchRing{{3, 6}, {7, 6}, {7, 9}, {3, 9}, {3, 6}}},
                               {0, 0},
                               {10, 0}};
    }
    void TearDown() override
    {
        // Код для очистки после каждого теста
    }
};

TEST_F(SearchRegionIntersectionGEOSTest, intersectionAll_Diagonal)
{
    Geometry::Line line(Point2D{-2, 0}, Point2D{6, 4});

    // Результат пересечения, является вектор std::vector<RingIntersection>
    // по вектору пересечений берется RingIntersection
    // в котором берется Intersection
    auto result = region1.intersectionsAllGEOS(Point2D{-2, 0}, Point2D{6, 4});
    ASSERT_EQ(result.size(), 1);
    ASSERT_NEAR(result[0][0].point.getX(), 0, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][0].point.getY(), 1, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][0].vertexIndex, 0);
    ASSERT_FALSE(result[0][0].isVertex);
    ASSERT_NEAR(result[0][1].point.getX(), 4, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][1].point.getY(), 3, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][1].vertexIndex, 2);
}

TEST_F(SearchRegionIntersectionGEOSTest, intersectionAll_SameLinesIntersect)
{
    auto result = region1.intersectionsAllGEOS(Point2D{-1, 4}, Point2D{5, 4});

    ASSERT_EQ(result.size(), 1);
    ASSERT_NEAR(result[0][0].point.getX(), 0, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][0].point.getY(), 4, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][0].vertexIndex, 1);
    ASSERT_TRUE(result[0][0].isVertex);
    ASSERT_NEAR(result[0][1].point.getX(), 4, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][1].point.getY(), 4, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][1].vertexIndex, 2);
    ASSERT_TRUE(result[0][1].isVertex);
}

TEST_F(SearchRegionIntersectionGEOSTest, intersectionAll_ParallelLinesIntersect)
{
    Geometry::Line line(Point2D{-1, 2}, Point2D{5, 2});

    auto result = region1.intersectionsAllGEOS(Point2D{-1, 2}, Point2D{5, 2});

    ASSERT_EQ(result.size(), 1);
    ASSERT_NEAR(result[0][0].point.getX(), 0, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][0].point.getY(), 2, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][0].vertexIndex, 0);
    ASSERT_FALSE(result[0][0].isVertex);
    ASSERT_NEAR(result[0][1].point.getX(), 4, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][1].point.getY(), 2, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][1].vertexIndex, 2);
    ASSERT_FALSE(result[0][1].isVertex);
}

TEST_F(SearchRegionIntersectionGEOSTest, intersectionAll_Vertex)
{
    auto result = region1.intersectionsAllGEOS(Point2D{-4, 0}, Point2D{4, 4});

    ASSERT_EQ(result.size(), 1);
    ASSERT_NEAR(result[0][0].point.getX(), 0, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][0].point.getY(), 2, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][0].vertexIndex, 0);
    ASSERT_FALSE(result[0][0].isVertex);
    ASSERT_NEAR(result[0][1].point.getX(), 4, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][1].point.getY(), 4, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][1].vertexIndex, 2);
    ASSERT_TRUE(result[0][1].isVertex);
}

TEST_F(SearchRegionIntersectionGEOSTest, intersectionAll_VertexDouble)
{
    auto result = region1.intersectionsAllGEOS(Point2D{0, 0}, Point2D{4, 4});

    ASSERT_EQ(result.size(), 1);
    ASSERT_NEAR(result[0][0].point.getX(), 0, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][0].point.getY(), 0, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][0].vertexIndex, 0);
    ASSERT_TRUE(result[0][0].isVertex);
    ASSERT_NEAR(result[0][1].point.getX(), 4, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][1].point.getY(), 4, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][1].vertexIndex, 2);
    ASSERT_TRUE(result[0][1].isVertex);
}

TEST_F(SearchRegionIntersectionGEOSTest, intersectionAll_TouchVertex)
{
    auto result = region1.intersectionsAllGEOS(Point2D{3, -1}, Point2D{5, 1});

    ASSERT_EQ(result.size(), 1);
    ASSERT_NEAR(result[0][0].point.getX(), 4, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][0].point.getY(), 0, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][0].vertexIndex, 3);
    ASSERT_TRUE(result[0][0].isVertex);
}

/* Отчет против часовой стрелки с внутренними полигонами против часовой стрелки */
TEST_F(SearchRegionIntersectionGEOSTest, intersectionAll_NoTouchInners)
{
    auto result = region2.intersectionsAllGEOS(Point2D{-2, 5}, Point2D{12, 5});

    // Номер вершины(vertexIndex) так и считается по индексу первой точки стороны
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].ringIndex, 0);

    ASSERT_NEAR(result[0][0].point.getX(), 0, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][0].point.getY(), 5, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][0].vertexIndex, 0);
    ASSERT_FALSE(result[0][0].isVertex);
    ASSERT_NEAR(result[0][1].point.getX(), 10, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][1].point.getY(), 5, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][1].vertexIndex, 2);
    ASSERT_FALSE(result[0][1].isVertex);
}

TEST_F(SearchRegionIntersectionGEOSTest, intersectionAll_TouchInners)
{
    /// @note нескорректированный
    auto result = region2.intersectionsAllGEOS(Point2D{-2, 2}, Point2D{12, 2});

    ASSERT_EQ(result.size(), 3);

    // Outer[0]
    ASSERT_EQ(result[0].ringIndex, 0);
    ASSERT_NEAR(result[0][0].point.getX(), 0, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][0].point.getY(), 2, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][0].vertexIndex, 0);
    ASSERT_FALSE(result[0][0].isVertex);
    ASSERT_NEAR(result[0][1].point.getX(), 10, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][1].point.getY(), 2, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][1].vertexIndex, 2);
    ASSERT_FALSE(result[0][1].isVertex);

    // Inner[1]
    ASSERT_EQ(result[1].ringIndex, 1);
    ASSERT_NEAR(result[1][0].point.getX(), 1, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[1][0].point.getY(), 2, ACCURACY_DOUBLE);
    ASSERT_EQ(result[1][0].vertexIndex, 3);
    ASSERT_FALSE(result[1][0].isVertex);
    ASSERT_NEAR(result[1][1].point.getX(), 4, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[1][1].point.getY(), 2, ACCURACY_DOUBLE);
    ASSERT_EQ(result[1][1].vertexIndex, 1);
    ASSERT_FALSE(result[1][1].isVertex);

    // Inner[2]
    ASSERT_EQ(result[2].ringIndex, 2);
    ASSERT_NEAR(result[2][0].point.getX(), 6, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[2][0].point.getY(), 2, ACCURACY_DOUBLE);
    ASSERT_EQ(result[2][0].vertexIndex, 3);
    ASSERT_FALSE(result[2][0].isVertex);
    ASSERT_NEAR(result[2][1].point.getX(), 9, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[2][1].point.getY(), 2, ACCURACY_DOUBLE);
    ASSERT_EQ(result[2][1].vertexIndex, 1);
    ASSERT_FALSE(result[2][1].isVertex);
}

TEST_F(SearchRegionIntersectionGEOSTest, intersectionAll_TouchInnersVertex)
{
    auto result = region2.intersectionsAllGEOS(Point2D{-3, 0}, Point2D{10, 13});

    ASSERT_EQ(result.size(), 3);

    // Outer[0]
    ASSERT_EQ(result[0].ringIndex, 0);
    ASSERT_NEAR(result[0][0].point.getX(), 0, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][0].point.getY(), 3, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][0].vertexIndex, 0);
    ASSERT_FALSE(result[0][0].isVertex);
    ASSERT_NEAR(result[0][1].point.getX(), 7, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][1].point.getY(), 10, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][1].vertexIndex, 1);
    ASSERT_FALSE(result[0][1].isVertex);

    // Inner[1]
    ASSERT_EQ(result[1].ringIndex, 1);
    ASSERT_NEAR(result[1][0].point.getX(), 1, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[1][0].point.getY(), 4, ACCURACY_DOUBLE);
    ASSERT_EQ(result[1][0].vertexIndex, 3);
    ASSERT_TRUE(result[1][0].isVertex);

    // Inner[3]
    ASSERT_EQ(result[2].ringIndex, 3);
    ASSERT_NEAR(result[2][0].point.getX(), 3, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[2][0].point.getY(), 6, ACCURACY_DOUBLE);
    ASSERT_EQ(result[2][0].vertexIndex, 0);
    ASSERT_TRUE(result[2][0].isVertex);
    ASSERT_NEAR(result[2][1].point.getX(), 6, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[2][1].point.getY(), 9, ACCURACY_DOUBLE);
    ASSERT_EQ(result[2][1].vertexIndex, 2);
    ASSERT_FALSE(result[2][1].isVertex);
}

TEST_F(SearchRegionIntersectionGEOSTest, intersectionAll_TouchInnersSameLinesIntersect)
{
    auto result = region2.intersectionsAllGEOS(Point2D{-2, 1}, Point2D{12, 1});

    ASSERT_EQ(result.size(), 3);

    // Outer[0]
    ASSERT_EQ(result[0].ringIndex, 0);
    ASSERT_NEAR(result[0][0].point.getX(), 0, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][0].point.getY(), 1, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][0].vertexIndex, 0);
    ASSERT_FALSE(result[0][0].isVertex);
    ASSERT_NEAR(result[0][1].point.getX(), 10, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][1].point.getY(), 1, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][1].vertexIndex, 2);
    ASSERT_FALSE(result[0][1].isVertex);

    // Inner[1]
    ASSERT_EQ(result[1].ringIndex, 1);
    ASSERT_NEAR(result[1][0].point.getX(), 1, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[1][0].point.getY(), 1, ACCURACY_DOUBLE);
    ASSERT_EQ(result[1][0].vertexIndex, 0);
    ASSERT_TRUE(result[1][0].isVertex);
    ASSERT_NEAR(result[1][1].point.getX(), 4, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[1][1].point.getY(), 1, ACCURACY_DOUBLE);
    ASSERT_EQ(result[1][1].vertexIndex, 1);
    ASSERT_TRUE(result[1][1].isVertex);

    // Inner[2]
    ASSERT_EQ(result[2].ringIndex, 2);
    ASSERT_NEAR(result[2][0].point.getX(), 6, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[2][0].point.getY(), 1, ACCURACY_DOUBLE);
    ASSERT_EQ(result[2][0].vertexIndex, 0);
    ASSERT_TRUE(result[2][0].isVertex);
    ASSERT_NEAR(result[2][1].point.getX(), 9, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[2][1].point.getY(), 1, ACCURACY_DOUBLE);
    ASSERT_EQ(result[2][1].vertexIndex, 1);
    ASSERT_TRUE(result[2][1].isVertex);
}

TEST_F(SearchRegionIntersectionGEOSTest, intersectionsAllGEOS_TouchVertexTriangleAndSameLinesIntersect)
{
    SearchRegion region{
        {SearchRing{{0, 0}, {0, 5}, {4, 5}, {2, 4}, {4, 4}, {4, 0}, {0, 0}}},
        {0, 0},
        {0, 5}};

    auto result = region.intersectionsAllGEOS(Point2D{4, -1}, Point2D{4, 5});

    ASSERT_EQ(result.size(), 1);

    // Outer[0]
    ASSERT_EQ(result[0].size(), 3);

    ASSERT_NEAR(result[0][0].point.getX(), 4, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][0].point.getY(), 0, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][0].vertexIndex, 5);
    ASSERT_TRUE(result[0][0].isVertex);
    ASSERT_NEAR(result[0][1].point.getX(), 4, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][1].point.getY(), 4, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][1].vertexIndex, 4);
    ASSERT_TRUE(result[0][1].isVertex);
    ASSERT_NEAR(result[0][2].point.getX(), 4, ACCURACY_DOUBLE);
    ASSERT_NEAR(result[0][2].point.getY(), 5, ACCURACY_DOUBLE);
    ASSERT_EQ(result[0][2].vertexIndex, 2);
    ASSERT_TRUE(result[0][2].isVertex);
}
} // namespace Entities
