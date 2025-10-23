#include <Task/Entities/SearchRegion.hpp>

#include <gtest/gtest.h>

namespace Entities {
using SearchRing = Entities::SearchRing;
class SearchRegionConstructorTests : public testing::Test
{
protected:
    SearchRing outer_ccw;
    SearchRing outer_cw;

    SearchRing inner_cw1;
    SearchRing inner_cw2;

    SearchRing inner_ccw1;
    SearchRing inner_ccw2;

    void SetUp() override
    {
        outer_ccw = SearchRing{{0, 0}, {10, 0}, {10, 10}, {0, 10}, {0, 0}};

        inner_cw1 = SearchRing{{1, 1}, {1, 4}, {4, 4}, {4, 1}, {1, 1}};
        inner_cw2 = SearchRing{{5, 5}, {5, 9}, {9, 9}, {9, 5}, {5, 5}};
    }

    void TearDown() override
    {
        // Код для очистки после каждого теста
    }
};

TEST_F(SearchRegionConstructorTests, Outer_CW_Inners_CW)
{
    SearchRegion region{{outer_ccw, inner_cw1}, {0, 0}, {0, 1}};

    EXPECT_TRUE(region.hasCorrectOrientation());

    Polygon2D polygon{outer_ccw, inner_cw1};

    region = SearchRegion{polygon, {0, 0}, {0, 1}};

    EXPECT_TRUE(region.hasCorrectOrientation());
}

TEST_F(SearchRegionConstructorTests, Outer_CW_Inners_CCW)
{
    SearchRegion region{{outer_ccw, inner_cw1}, {0, 0}, {0, 1}};

    EXPECT_TRUE(region.hasCorrectOrientation());

    region = SearchRegion{Polygon2D{outer_ccw, inner_cw1}, {0, 0}, {0, 1}};

    EXPECT_TRUE(region.hasCorrectOrientation());
}

TEST_F(SearchRegionConstructorTests, Outer_CCW_Inners_CW)
{
    SearchRegion region{{outer_ccw, inner_cw1}, {0, 0}, {0, 1}};

    EXPECT_TRUE(region.hasCorrectOrientation());

    region = SearchRegion{Polygon2D{outer_ccw, inner_cw1}, {0, 0}, {0, 1}};

    EXPECT_TRUE(region.hasCorrectOrientation());
}

TEST_F(SearchRegionConstructorTests, Outer_CCW_Inners_CCW)
{
    SearchRegion region{{outer_ccw, inner_cw1}, {0, 0}, {0, 1}};

    EXPECT_TRUE(region.hasCorrectOrientation());

    region = SearchRegion{Polygon2D{outer_ccw, inner_cw1}, {0, 0}, {0, 1}};

    EXPECT_TRUE(region.hasCorrectOrientation());
}

TEST_F(SearchRegionConstructorTests, Outer_CW_Inners_CCW_CW)
{
    SearchRegion region{{outer_ccw, inner_cw1, inner_cw1}, {0, 0}, {0, 1}};

    EXPECT_TRUE(region.hasCorrectOrientation());

    region = SearchRegion{Polygon2D{outer_ccw, inner_cw1, inner_cw1}, {0, 0}, {0, 1}};

    EXPECT_TRUE(region.hasCorrectOrientation());
}

TEST_F(SearchRegionConstructorTests, Outer_CCW_Inners_CCW_CW)
{ //outer_ccw, inner_cw1, inner_ccw1,inner_ccw2
    SearchRegion region{{outer_ccw, inner_cw1, inner_cw1}, {0, 0}, {0, 1}};

    EXPECT_TRUE(region.hasCorrectOrientation());

    region = SearchRegion{Polygon2D{outer_ccw, inner_cw1, inner_cw1}, {0, 0}, {0, 1}};

    EXPECT_TRUE(region.hasCorrectOrientation());
}

// old
TEST_F(SearchRegionConstructorTests, Default)
{
    SearchRegion region{};

    EXPECT_EQ(region.outer().size(), 0);
    EXPECT_EQ(region.inners().size(), 0);
    EXPECT_EQ(region.getEntrance(), 0);
    EXPECT_EQ(region.getExit(), 0);
}

TEST_F(SearchRegionConstructorTests, fromPolygonPlusPoints01)
{
    PrimaryEntities::Polygon<Point2D> polygon({PrimaryEntities::Ring<Point2D>{{0, 0}, {4, 0}, {4, 4}, {2, 4}, {4, 5}, {0, 5}, {0, 0}},
                                               PrimaryEntities::Ring<Point2D>{{1, 1}, {1, 2}, {2, 2}, {2, 1}, {1, 1}}});

    SearchRegion region(polygon, {0, 0}, {4, 5});

    EXPECT_EQ(region.getEntrance(), 0);
    EXPECT_EQ(region.getExit(), 2);
}

TEST_F(SearchRegionConstructorTests, fromPolygonPlusPoints02)
{
    PrimaryEntities::Polygon<Point2D> polygon({PrimaryEntities::Ring<Point2D>{{0, 0}, {0, 5}, {4, 5}, {2, 4}, {4, 4}, {4, 0}, {0, 0}},
                                               PrimaryEntities::Ring<Point2D>{{1, 1}, {1, 2}, {2, 2}, {2, 1}, {1, 1}}});
    SearchRegion region(polygon, {0, 0}, {4, 5});

    EXPECT_EQ(region.getEntrance(), 0);
    EXPECT_EQ(region.getExit(), 2);
}

TEST_F(SearchRegionConstructorTests, fromPolygonPlusPoints03)
{
    PrimaryEntities::Polygon<Point2D> polygon({PrimaryEntities::Ring<Point2D>{{0, 0}, {0, 5}, {4, 5}, {2, 4}, {4, 4}, {4, 0}, {0, 0}}});
    SearchRegion region(polygon, {0, 0}, {4, 5});

    EXPECT_EQ(region.getEntrance(), 0);
    EXPECT_EQ(region.getExit(), 2);
}

TEST_F(SearchRegionConstructorTests, fromPolygonPlusPoints04)
{
    PrimaryEntities::Polygon<Point2D> polygon({PrimaryEntities::Ring<Point2D>{{0, 0}, {0, 5}, {4, 5}, {2, 4}, {4, 4}, {4, 0}, {0, 0}}});
    SearchRegion region(polygon, {1, 0}, {1, 5});

    EXPECT_EQ(region.getEntrance(), 7);
    EXPECT_EQ(region.getExit(), 2);
}

TEST_F(SearchRegionConstructorTests, SearchRingInitializerList01)
{
    SearchRegion region({SearchRing{{0, 0}, {4, 0}, {4, 4}, {2, 4}, {4, 5}, {0, 5}, {0, 0}},
                         SearchRing{{1, 1}, {1, 2}, {2, 2}, {2, 1}, {1, 1}}},
                        {0, 0},
                        {4, 5});

    EXPECT_EQ(region.getEntrance(), 0);
    EXPECT_EQ(region.getExit(), 2);
}

TEST_F(SearchRegionConstructorTests, SearchRingInitializerList02)
{
    SearchRegion region;

    EXPECT_THROW(region = SearchRegion({SearchRing{{0, 0}, {4, 0}, {4, 4}, {2, 4}, {4, 5}, {0, 5}, {0, 0}},
                                        SearchRing{{1, 1}, {1, 2}, {2, 2}, {2, 1}, {1, 1}}},
                                       {0, 0},
                                       {0, 0}),
                 std::invalid_argument);
}

TEST_F(SearchRegionConstructorTests, SearchRingInitializerList03)
{
    // Точка входа и выхода на ребре
    SearchRegion region({SearchRing{{0, 0}, {4, 0}, {4, 4}, {2, 4}, {4, 5}, {0, 5}, {0, 0}},
                         SearchRing{{1, 1}, {1, 2}, {2, 2}, {2, 1}, {1, 1}}},
                        {0, 1},
                        {0, 2});

    EXPECT_EQ(region.getEntrance(), 1);
    EXPECT_EQ(region.getExit(), 2);
}

TEST_F(SearchRegionConstructorTests, SearchRingInitializerList04)
{
    SearchRegion region({SearchRing{{0, 0}, {0, 5}, {4, 5}, {2, 4}, {4, 4}, {4, 0}, {0, 0}},
                         SearchRing{{1, 1}, {1, 2}, {2, 2}, {2, 1}, {1, 1}}},
                        {0, 0},
                        {4, 5});

    EXPECT_EQ(region.getEntrance(), 0);
    EXPECT_EQ(region.getExit(), 2);
}

TEST_F(SearchRegionConstructorTests, SearchRingInitializerList05)
{
    SearchRegion region({SearchRing{{0, 0}, {0, 5}, {4, 5}, {2, 4}, {4, 4}, {4, 0}, {0, 0}}}, {1, 0}, {1, 5});

    EXPECT_EQ(region.getEntrance(), 7);
    EXPECT_EQ(region.getExit(), 2);
}

//TEST_F(SearchRegionConstructorTests, SearchRingInitializerListEntranceMissingPoint)
//{
//    EXPECT_THROW(SearchRegion region({SearchRing{{0, 0}, {0, 5}, {4, 5}, {2, 4}, {4, 4}, {4, 0}, {0, 0}},
//                                              SearchRing{{1, 1}, {1, 2}, {2, 2}, {2, 1}, {1, 1}}},
//                                             {3, 3},
//                                             {4, 5}),
//                 std::invalid_argument);
//}

TEST_F(SearchRegionConstructorTests, SearchRingInitializerListExitMissingPoint)
{
    EXPECT_NO_THROW(SearchRegion region({SearchRing{{0, 0}, {0, 5}, {4, 5}, {2, 4}, {4, 4}, {4, 0}, {0, 0}},
                                         SearchRing{{1, 1}, {1, 2}, {2, 2}, {2, 1}, {1, 1}}},
                                        {0, 0},
                                        {3, 3}));
}
} // namespace Entities
