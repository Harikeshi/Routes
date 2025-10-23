#include <Task/Entities/SearchRegion.hpp>

#include <gtest/gtest.h>

namespace Entities {

TEST(SearchRegionTests, intersectionAll_Diagonal)
{
    SearchRegion region;
    Polygon2D polygon({PrimaryEntities::Ring<Point2D>{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}}});
    region.setFromPolygon(polygon);
    Geometry::Line line(Point2D{-2, 0}, Point2D{6, 4});

    auto result = region.intersectionsAllGEOS(Point2D{-2, 0}, Point2D{6, 4});

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].size(), 2);
    EXPECT_NEAR(result[0][0].getPoint().getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0][0].getPoint().getY(), 1, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0][0].getIndex(), 0);
    EXPECT_FALSE(result[0][0].isVertex);
    EXPECT_NEAR(result[0][1].getPoint().getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0][1].getPoint().getY(), 3, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0][1].getIndex(), 2);
    EXPECT_FALSE(result[0][1].isVertex);
}

TEST(SearchRegionTests, intersectionAll_SameLinesIntersect)
{
    SearchRegion region;
    Polygon2D polygon({PrimaryEntities::Ring<Point2D>{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}}});
    region.setFromPolygon(polygon);
    Geometry::Line line(Point2D{-1, 4}, Point2D{5, 4});

    auto result = region.intersectionsAllGEOS(Point2D{-1, 4}, Point2D{5, 4});

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].size(), 2);
    EXPECT_NEAR(result[0][0].getPoint().getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0][0].getPoint().getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0][0].vertexIndex, 1);
    EXPECT_TRUE(result[0][0].isVertex);
    EXPECT_NEAR(result[0][1].getPoint().getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0][1].getPoint().getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0][1].vertexIndex, 2);
    EXPECT_TRUE(result[0][1].isVertex);
}

TEST(SearchRegionTests, intersectionAll_ParallelLinesIntersect)
{
    SearchRegion region;
    Polygon2D polygon({PrimaryEntities::Ring<Point2D>{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}}});
    region.setFromPolygon(polygon);
    Geometry::Line line(Point2D{-1, 2}, Point2D{5, 2});

    auto result = region.intersectionsAllGEOS(Point2D{-1, 2}, Point2D{5, 2});

    EXPECT_EQ(result.size(), 1);
    EXPECT_NEAR(result[0][0].getPoint().getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0][0].getPoint().getY(), 2, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0][0].getIndex(), 0);
    EXPECT_FALSE(result[0][0].isVertex);
    EXPECT_NEAR(result[0][1].getPoint().getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0][1].getPoint().getY(), 2, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0][1].getIndex(), 2);
    EXPECT_FALSE(result[0][1].isVertex);
}

TEST(SearchRegionTests, intersectionAll_Vertex)
{
    SearchRegion region;
    Polygon2D polygon({PrimaryEntities::Ring<Point2D>{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}}});
    region.setFromPolygon(polygon);
    Geometry::Line line(Point2D{-4, 0}, Point2D{4, 4});

    auto result = region.intersectionsAllGEOS(Point2D{-4, 0}, Point2D{4, 4});

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].size(), 2);

    EXPECT_NEAR(result[0][0].getPoint().getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0][0].getPoint().getY(), 2, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0][0].getIndex(), 0);
    EXPECT_FALSE(result[0][0].isVertex);

    EXPECT_NEAR(result[0][1].getPoint().getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0][1].getPoint().getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0][1].getIndex(), 2);
    EXPECT_TRUE(result[0][1].isVertex);
}

TEST(SearchRegionTests, intersectionAll_VertexDouble)
{
    SearchRegion region;
    Polygon2D polygon({PrimaryEntities::Ring<Point2D>{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}}});
    region.setFromPolygon(polygon);
    Geometry::Line line(Point2D{0, 0}, Point2D{4, 4});

    auto result = region.intersectionsAllGEOS(Point2D{0, 0}, Point2D{4, 4});

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].size(), 2);

    EXPECT_NEAR(result[0][0].getPoint().getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0][0].getPoint().getY(), 0, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0][0].getIndex(), 0);
    EXPECT_TRUE(result[0][0].isVertex);

    EXPECT_NEAR(result[0][1].getPoint().getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0][1].getPoint().getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0][1].getIndex(), 2);
    EXPECT_TRUE(result[0][1].isVertex);
}

TEST(SearchRegionTests, intersectionAll_TouchVertex)
{
    SearchRegion region;
    Polygon2D polygon({PrimaryEntities::Ring<Point2D>{{0, 0}, {0, 4}, {4, 4}, {4, 0}, {0, 0}}});
    region.setFromPolygon(polygon);
    Geometry::Line line(Point2D{3, -1}, Point2D{5, 1});

    auto result = region.intersectionsAllGEOS(Point2D{3, -1}, Point2D{5, 1});

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].size(), 1);

    EXPECT_NEAR(result[0][0].getPoint().getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0][0].getPoint().getY(), 0, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0][0].getIndex(), 3);
    EXPECT_TRUE(result[0][0].isVertex);
}

TEST(SearchRegionTests, intersectionAll_NoTouchInners)
{
    SearchRegion region;
    Polygon2D polygon(PrimaryEntities::Polygon<Point2D>{{{0, 0}, {0, 10}, {10, 10}, {10, 0}, {0, 0}},
                                                        {{1, 1}, {4, 1}, {4, 4}, {1, 4}, {1, 1}},
                                                        {{6, 1}, {9, 1}, {9, 4}, {6, 4}, {6, 1}},
                                                        {{3, 6}, {7, 6}, {7, 9}, {3, 9}, {3, 6}}});
    region.setFromPolygon(polygon);
    Geometry::Line line(Point2D{-2, 5}, Point2D{12, 5});

    auto result = region.intersectionsAllGEOS(Point2D{-2, 5}, Point2D{12, 5});

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].size(), 2);

    EXPECT_NEAR(result[0][0].getPoint().getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0][0].getPoint().getY(), 5, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0][0].getIndex(), 0);
    EXPECT_FALSE(result[0][0].isVertex);

    EXPECT_NEAR(result[0][1].getPoint().getX(), 10, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0][1].getPoint().getY(), 5, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0][1].getIndex(), 2);
    EXPECT_FALSE(result[0][1].isVertex);
}

TEST(SearchRegionTests, intersectionAll_TouchInners)
{
    SearchRegion region;
    /// @note нескорректированный полигон
    Polygon2D polygon(PrimaryEntities::Polygon<Point2D>{{{0, 0}, {10, 0}, {10, 10}, {0, 10}, {0, 0}},
                                                        {{1, 1}, {4, 1}, {4, 4}, {1, 4}, {1, 1}},
                                                        {{6, 1}, {9, 1}, {9, 4}, {6, 4}, {6, 1}},
                                                        {{3, 6}, {7, 6}, {7, 9}, {3, 9}, {3, 6}}});
    region.setFromPolygon(polygon);
    std::vector<Point2D> standart{{0, 2}, {1, 2}, {4, 2}, {6, 2}, {9, 2}, {10, 2}};
    Geometry::Line line(Point2D{-2, 2}, Point2D{12, 2});

    auto result = region.intersectionsAllGEOS(Point2D{-2, 2}, Point2D{12, 2});

    for (const auto& intersections : result)
    {
        for (const auto& intersection : intersections.getIntersections())
        {
            ASSERT_TRUE(find(standart.begin(), standart.end(), intersection.getPoint()) != standart.end());
        }
    }

    EXPECT_EQ(result[0].size() + result[1].size() + result[2].size(), standart.size());
}

TEST(SearchRegionTests, intersectionAll_TouchInnersVertex)
{
    SearchRegion region;
    Polygon2D polygon(PrimaryEntities::Polygon<Point2D>{{{0, 0}, {10, 0}, {10, 10}, {0, 10}, {0, 0}},
                                                        {{1, 1}, {4, 1}, {4, 4}, {1, 4}, {1, 1}},
                                                        {{6, 1}, {9, 1}, {9, 4}, {6, 4}, {6, 1}},
                                                        {{3, 6}, {7, 6}, {7, 9}, {3, 9}, {3, 6}}});
    region.setFromPolygon(polygon);
    std::vector<Point2D> standart{{0, 3}, {1, 4}, {3, 6}, {6, 9}, {7, 10}};
    Geometry::Line line(Point2D{-3, 0}, Point2D{10, 13});

    auto result = region.intersectionsAllGEOS(Point2D{-3, 0}, Point2D{10, 13});

    for (const auto& intersections : result)
    {
        for (const auto& intersection : intersections.getIntersections())
            ASSERT_TRUE(find(standart.begin(), standart.end(), intersection.getPoint()) != standart.end());
    }

    EXPECT_EQ(result[0].size() + result[1].size() + result[2].size(), standart.size());
}

TEST(SearchRegionTests, intersectionAll_TouchInnersSameLinesIntersect)
{
    SearchRegion region;
    Polygon2D polygon(PrimaryEntities::Polygon<Point2D>{{{0, 0}, {10, 0}, {10, 10}, {0, 10}, {0, 0}},
                                                        {{1, 1}, {4, 1}, {4, 4}, {1, 4}, {1, 1}},
                                                        {{6, 1}, {9, 1}, {9, 4}, {6, 4}, {6, 1}},
                                                        {{3, 6}, {7, 6}, {7, 9}, {3, 9}, {3, 6}}});
    region.setFromPolygon(polygon);
    std::vector<Point2D> standart{{0, 1}, {1, 1}, {4, 1}, {6, 1}, {9, 1}, {10, 1}};
    Geometry::Line line(Point2D{-2, 1}, Point2D{12, 1});

    auto result = region.intersectionsAllGEOS({-2, 1}, {12, 1});

    for (const auto& intersections : result)
    {
        for (const auto& intersection : intersections.getIntersections())
            ASSERT_TRUE(find(standart.begin(), standart.end(), intersection.getPoint()) != standart.end());
    }

    EXPECT_EQ(result[0].size() + result[1].size() + result[2].size(), standart.size());
}

TEST(SearchRegionTests, intersectionsAll_TouchVertexTriangleAndSameLinesIntersect)
{
    SearchRegion region;
    Polygon2D polygon({PrimaryEntities::Ring<Point2D>{{0, 0}, {0, 5}, {4, 5}, {2, 4}, {4, 4}, {4, 0}, {0, 0}}});
    region.setFromPolygon(polygon);
    Geometry::Line line(Point2D{4, -1}, Point2D{4, 5});

    auto result = region.intersectionsAllGEOS(Point2D{4, -1}, Point2D{4, 5});

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].size(), 3);

    EXPECT_NEAR(result[0][0].getPoint().getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0][0].getPoint().getY(), 0, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0][0].getIndex(), 5);
    EXPECT_TRUE(result[0][0].isVertex);

    EXPECT_NEAR(result[0][1].getPoint().getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0][1].getPoint().getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0][1].getIndex(), 4);
    EXPECT_TRUE(result[0][1].isVertex);

    EXPECT_NEAR(result[0][2].getPoint().getX(), 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(result[0][2].getPoint().getY(), 5, ACCURACY_DOUBLE);
    EXPECT_EQ(result[0][2].getIndex(), 2);
    EXPECT_TRUE(result[0][2].isVertex);
}

} // namespace Entities
