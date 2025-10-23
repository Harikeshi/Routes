#include <Task/Entities/SearchRegion.hpp>
#include <Task/Schemes/Search/Config.hpp>

#include <gtest/gtest.h>

namespace Entities {

TEST(SearchRegionNearestTest, nearestIntersectionAhead1)
{
    SearchRegion region{{SearchRing{{0, 0}, {0, 9}, {9, 9}, {9, 0}, {0, 0}},
                         SearchRing{{2, 2}, {2, 7}, {7, 7}, {7, 2}, {2, 2}}},
                        {0, 0},
                        {0, 9}};

    // -> {0,4} -> {1,4} -> {2,4} -> {4,4} -> {7,4} -> {9,4} ->

    // -> {0,4}
    auto inter = region.nearestIntersectionAhead({0, 4}, Radian(0)); // TODO: берется угол относительно оси X

    ASSERT_FALSE(inter.empty());
    EXPECT_NEAR(inter[0].getPoint().getX(), 2, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter[0].getPoint().getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.ringIndex, 1);
    EXPECT_EQ(inter[0].vertexIndex, 3);

    // ->{1, 4}
    inter = region.nearestIntersectionAhead({1, 4}, Radian(0));

    ASSERT_FALSE(inter.empty());
    EXPECT_NEAR(inter[0].point.getX(), 2, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter[0].point.getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.ringIndex, 1);
    EXPECT_EQ(inter[0].vertexIndex, 3);

    // -> {2, 4}
    inter = region.nearestIntersectionAhead({2, 4}, Radian(0));

    ASSERT_FALSE(inter.empty());
    EXPECT_NEAR(inter[0].point.getX(), 7, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter[0].point.getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.ringIndex, 1);
    EXPECT_EQ(inter[0].vertexIndex, 1);

    // -> {4, 4}
    inter = region.nearestIntersectionAhead({4, 4}, Radian(0));

    ASSERT_FALSE(inter.empty());
    EXPECT_NEAR(inter[0].point.getX(), 7, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter[0].point.getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.ringIndex, 1);
    EXPECT_EQ(inter[0].vertexIndex, 1);

    // -> {7, 4}
    inter = region.nearestIntersectionAhead({7, 4}, Radian(0));

    ASSERT_FALSE(inter.empty());
    EXPECT_NEAR(inter[0].point.getX(), 9, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter[0].point.getY(), 4, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.ringIndex, 0);
    EXPECT_EQ(inter[0].vertexIndex, 2);

    // -> {9, 4}
    inter = region.nearestIntersectionAhead({9, 4}, Radian(0));

    ASSERT_TRUE(inter.empty());

    // -> {0, 7}
    inter = region.nearestIntersectionAhead({0, 7}, Radian(0));

    ASSERT_FALSE(inter.empty());
    EXPECT_NEAR(inter[0].point.getX(), 2, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter[0].point.getY(), 7, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.ringIndex, 1);
    EXPECT_EQ(inter[0].vertexIndex, 3);
}

TEST(SearchRegionNearestTest, nearestIntersectionAhead2)
{
    PrimaryEntities::Polygon<Point2D> polygon = {PrimaryEntities::Ring<Point2D>({Point2D{1, 1}, Point2D{1, 6}, Point2D{6, 6}, Point2D{6, 1}, Point2D{1, 1}})};
    SearchRegion region(polygon, Point2D(1, 2), Point2D(6, 2));

    auto inter = region.nearestIntersectionAhead(Point2D(3, 2), Radian(M_PI / 2));

    ASSERT_TRUE(!inter.empty());
    EXPECT_NEAR(inter[0].getPoint().getX(), 3, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter[0].getPoint().getY(), 6, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.ringIndex, 0);
    EXPECT_EQ(inter[0].getIndex(), 2);

    /// @note фэйлится тест
    inter = region.nearestIntersectionAhead(Point2D(3.5, 1.5), Radian(0));
    ASSERT_TRUE(!inter.empty());
    EXPECT_NEAR(inter[0].getPoint().getX(), 6, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter[0].getPoint().getY(), 1.5, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.ringIndex, 0);
    EXPECT_EQ(inter[0].getIndex(), 4);
}

TEST(SearchRegionNearestTest, nearestIntersectionAhead3)
{
    PrimaryEntities::Polygon<Point2D> polygon = {PrimaryEntities::Ring<Point2D>({Point2D{0, 0}, Point2D{2, 2}, Point2D{4, 2}, Point2D{4, 0}, Point2D{2, -2}, Point2D{0, -2}, Point2D{0, 0}})};
    SearchRegion region(polygon, {0, 0}, {0, 2});
    auto inter = region.nearestIntersectionAhead(Point2D(1, -1), Radian(-M_PI / 2), IntersectionMethod::Geos);

    ASSERT_TRUE(!inter.empty());

    EXPECT_NEAR(inter[0].getPoint().getX(), 1, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter[0].getPoint().getY(), -2, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.ringIndex, 0);
    EXPECT_EQ(inter[0].getIndex(), 4);
}

TEST(SearchRegionNearestTest, nearestIntersectionAhead4)
{
    SearchRegion region{{SearchRing{{1, 1}, {1, 6}, {6, 6}, {6, 1}, {1, 1}},
                         SearchRing{{3, 2}, {5, 2}, {5, 5}, {3, 5}, {3, 2}}},
           {1, 1},
           {1, 6}};

    auto inter = region.nearestIntersectionAhead(Point2D(2, 3), Radian(0));

    ASSERT_TRUE(!inter.empty());

    EXPECT_NEAR(inter[0].getPoint().getX(), 3, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter[0].getPoint().getY(), 3, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.ringIndex, 1);
    EXPECT_EQ(inter[0].getIndex(), 3);

    inter = region.nearestIntersectionAhead(Point2D(3.5, 1.5), Radian(M_PI / 2));

    ASSERT_TRUE(!inter.empty());

    EXPECT_NEAR(inter[0].getPoint().getX(), 3.5, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter[0].getPoint().getY(), 2, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.ringIndex, 1);
    EXPECT_EQ(inter[0].getIndex(), 0);
}

TEST(SearchRegionNearestTest, nearestIntersectionAheadBringToZero)
{
    // 1e-14
    // <-
    SearchRegion region = SearchRegion{{SearchRing{{0, 0}, {1, 1}, {2, 0}, {1, -1}, {0, 0}}},
                                       {0, 0},
                                       {1, 1}};

    auto inter = region.nearestIntersectionAhead(Point2D(1, 0), Radian(M_PI));

    ASSERT_FALSE(inter.empty());

    EXPECT_NEAR(inter[0].point.getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter[0].point.getY(), 0, ACCURACY_DOUBLE);
    EXPECT_TRUE(inter[0].isVertex);
    EXPECT_EQ(inter.ringIndex, 0);
    EXPECT_EQ(inter[0].vertexIndex, 0);

    // ->
    region = SearchRegion{{SearchRing{{0, 0}, {-1, -1}, {-2, 0}, {-1, 1}, {0, 0}}},
                          {0, 0},
                          {-1, -1}};

    inter = region.nearestIntersectionAhead(Point2D(-1, 0), Radian());

    ASSERT_FALSE(inter.empty());

    EXPECT_NEAR(inter[0].point.getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter[0].point.getY(), 0, ACCURACY_DOUBLE);
    EXPECT_TRUE(inter[0].isVertex);
    EXPECT_EQ(inter.ringIndex, 0);
    EXPECT_EQ(inter[0].vertexIndex, 0);

    // v
    region = SearchRegion{{SearchRing{{0, 0}, {-1, 1}, {0, 2}, {1, 1}, {0, 0}}},
                          {0, 0},
                          {1, 1}};

    inter = region.nearestIntersectionAhead(Point2D(0, 1), Radian(-M_PI / 2));

    ASSERT_FALSE(inter.empty());

    EXPECT_NEAR(inter[0].point.getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter[0].point.getY(), 0, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.ringIndex, 0);
    EXPECT_TRUE(inter[0].isVertex);
    EXPECT_EQ(inter[0].vertexIndex, 0);

    // ^
    region = SearchRegion{{SearchRing{{0, 0}, {1, -1}, {0, -2}, {-1, -1}, {0, 0}}},
                          {0, 0},
                          {1, -1}};

    inter = region.nearestIntersectionAhead(Point2D(0, -1), Radian(M_PI / 2));

    ASSERT_FALSE(inter.empty());

    EXPECT_NEAR(inter[0].point.getX(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter[0].point.getY(), 0, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.ringIndex, 0);
    EXPECT_TRUE(inter[0].isVertex);
    EXPECT_EQ(inter[0].vertexIndex, 0);
}



TEST(SearchRegionNearestTest, nearestIntersectionAhead_largeValues)
{
    Polygon2D metricPolygon = {{Point2D(0, 0), Point2D(0, 40000), Point2D(40000, 40000), Point2D(40000, 8000), Point2D(50000, 40000), Point2D(50000, 0)}, {Point2D(14000, 16000), Point2D(14000, 24000), Point2D(26000, 24000), Point2D(26000, 16000)}};

    SearchRegion metricRegion(metricPolygon, Point2D(0, 12000), Point2D(50000, 12000));
	Radian course(-2.0344439357957027);
	Radian turnAngle(0.98006219812081574);

	Radian nextCourseLeft = (course - turnAngle).normalize();
	Radian nextCourseRight = (course + turnAngle).normalize();

	Point2D coordinates(11673.037201230225, 17836.518600615116);

    RingIntersection interLeft  = metricRegion.nearestIntersectionAhead(coordinates, nextCourseLeft);
    RingIntersection interRight = metricRegion.nearestIntersectionAhead(coordinates, nextCourseRight);
    ASSERT_FALSE(interLeft.empty());
    EXPECT_FALSE(interRight.empty());
}
} // namespace Entities
