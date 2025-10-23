#include <Task/Entities/Region.hpp>
#include <Task/Operations/RouteOperations.hpp>
#include <Task/Schemes/Search/Config.hpp>

#include <AbstractOperations/Algorithms/Correct.hpp>
#include <Geometry/Objects/BorderedLine.hpp>
#include <gtest/gtest.h>

using namespace Schemes::Search;
using namespace Operations;
using namespace Geometry;
using InRegion::Region;

class RegionTestFixture : public testing::Test
{
protected:
    Polygon2D polygonSquareMin;
    Polygon2D polygonSquareMid;
    Polygon2D polygonSquareBig;
    Polygon2D polygonNonconvex;
    Polygon2D polygonWithInners;

    void SetUp() override
    {
        polygonSquareMin = {{{0, 0}, {0, 4000}, {4000, 4000}, {4000, 0}, {0, 0}}};
        polygonSquareMid = {{{1000, 1000}, {6000, 1000}, {6000, 6000}, {1000, 6000}, {1000, 1000}}};
        polygonSquareBig = {{{0, 0}, {0, 9000}, {9000, 9000}, {9000, 0}}};
        polygonNonconvex = {{{0, 0}, {4000, 0}, {4000, 4000}, {2000, 4000}, {4000, 5000}, {0, 5000}, {0, 0}}};
        polygonWithInners = {{{0, 0}, {0, 10000}, {10000, 10000}, {10000, 0}, {0, 0}},
                             {{1000, 1000}, {4000, 1000}, {4000, 4000}, {1000, 4000}, {1000, 1000}},
                             {{6000, 1000}, {9000, 1000}, {9000, 4000}, {6000, 4000}, {6000, 1000}},
                             {{3000, 6000}, {7000, 6000}, {7000, 9000}, {3000, 9000}, {3000, 6000}}};
    }
};
/*
TEST(region, base)
{
    Region region;

    Polygon2D polygon = {{{1000, 2000}, {5000, 6000}, {9000, 2000}, {5000, -2000}}};

    region.setPolygon(polygon);
    region.setEntrance({1000, 2000});
    region.setExit({3000, 0});

    Region region2(region);
    Region region3 = region;

    EXPECT_EQ(region2.getEntrance(), region3.getEntrance());
    EXPECT_EQ(region2.getExit(), region3.getExit());
    EXPECT_EQ(region2.getPolygon().outer(), region3.getPolygon().outer());
    EXPECT_EQ(region2.getPolygon().inners(), region3.getPolygon().inners());
}

// initializeDoorPoint, setEntrance, setExit
namespace {

TEST_F(RegionTestFixture, initializeDoorPoint1)
{
    Region region;
    region.setPolygon(polygonSquareMin);
    region.setEntrance({0, 2000});
    region.setExit({2000, 0});
    EXPECT_EQ(region.getEntrance(), 1);
    EXPECT_EQ(region.getExit(), 5);
}

TEST_F(RegionTestFixture, initializeDoorPoint2)
{
    PrimaryEntities::Polygon<Point2D> polygon(polygonNonconvex);
    Region region(polygon, {1000, 0}, {1000, 5000});

    EXPECT_EQ(region.getEntrance(), 7);
    EXPECT_EQ(region.getExit(), 2);
}

TEST_F(RegionTestFixture, initializeDoorPoint_InverseOrder)
{
    Region region;
    region.setPolygon(polygonSquareMin);
    region.setEntrance({2000, 0});
    region.setExit({0, 2000});
    EXPECT_EQ(region.getEntrance(), 5);
    EXPECT_EQ(region.getExit(), 1);
}

TEST_F(RegionTestFixture, initializeDoorPoint_SameEdge)
{
    Region region;
    region.setPolygon(polygonSquareMin);
    region.setEntrance({0, 1000});
    region.setExit({0, 2000});
    EXPECT_EQ(region.getEntrance(), 1);
    EXPECT_EQ(region.getExit(), 2);
}

TEST_F(RegionTestFixture, initializeDoorPoint_SameEdgeInverseOrder)
{
    Region region;
    region.setPolygon(polygonSquareMin);
    region.setEntrance({0, 2000});
    region.setExit({0, 1000});
    EXPECT_EQ(region.getEntrance(), 2);
    EXPECT_EQ(region.getExit(), 1);
}

TEST_F(RegionTestFixture, initializeDoorPoint_SameEdgeValidity)
{
    Region region;
    region.setPolygon(polygonNonconvex);
    region.setEntrance({4, 0});
    region.setExit({4, 4});
    EXPECT_NO_THROW(region.validate());
}

TEST_F(RegionTestFixture, initializeDoorPoint_EdgesValidity)
{
    Region region;
    region.setPolygon(polygonNonconvex);
    region.setEntrance({1000, 0});
    region.setExit({1, 5000});
    EXPECT_NO_THROW(region.validate());
}

TEST_F(RegionTestFixture, initializeDoorPoint_VertexAndEdgeValidity)
{
    Region region;
    region.setPolygon(polygonNonconvex);
    region.setEntrance({2000, 0});
    region.setExit({2000, 4000});
    EXPECT_NO_THROW(region.validate());
}

TEST_F(RegionTestFixture, initializeDoorPoint_Vertices)
{
    PrimaryEntities::Polygon<Point2D> polygon(polygonNonconvex);
    Region region(polygon, {0, 0}, {4000, 5000});

    EXPECT_EQ(region.getEntrance(), 0);
    EXPECT_EQ(region.getExit(), 2);
}

TEST_F(RegionTestFixture, initializeDoorPoint_VerticesSame)
{
    PrimaryEntities::Polygon<Point2D> polygon(polygonNonconvex);
    Region region(polygon, {4000, 5000}, {4000, 5000});

    EXPECT_EQ(region.getEntrance(), 2);
    EXPECT_EQ(region.getExit(), 2);
}

TEST_F(RegionTestFixture, initializeDoorPoint_VerticesValidity1)
{
    Region region;
    region.setPolygon(polygonNonconvex);
    region.setEntrance({0, 0});
    region.setExit({4000, 4000});
    EXPECT_NO_THROW(region.validate());
}

TEST_F(RegionTestFixture, initializeDoorPoint_VerticesValidity2)
{
    Region region;
    region.setPolygon(polygonNonconvex);
    region.setEntrance({4000, 0});
    region.setExit({4000, 5000});
    EXPECT_NO_THROW(region.validate());
}

TEST_F(RegionTestFixture, initializeDoorPoint_invalidEntrance)
{
    Region region;
    region.setPolygon(polygonNonconvex);
    region.setEntrance({2000, 0});
    region.setExit({2000, 2000});
    EXPECT_THROW(region.validate(), std::runtime_error);
}

TEST_F(RegionTestFixture, initializeDoorPoint_invalidExit)
{
    Region region;
    region.setPolygon(polygonNonconvex);
    region.setEntrance({3000, 2000});
    region.setExit({3000, 4000});
    EXPECT_THROW(region.validate(), std::runtime_error);
}

TEST_F(RegionTestFixture, initializeDoorPoint_ReversePolygon)
{
    // после корректировки порядок точек обратится
    Region region(polygonNonconvex, {0, 0}, {4000, 5000});

    EXPECT_EQ(region.getEntrance(), 0);
    EXPECT_EQ(region.getExit(), 2);
}

} // namespace

TEST_F(RegionTestFixture, size)
{
    Region region;
    region.setPolygon(polygonSquareMin);
    EXPECT_EQ(region.size(), 8000);
}

// nearestIntersectionAhead
namespace {

TEST_F(RegionTestFixture, nearestIntersectionAhead)
{
    PrimaryEntities::Polygon<Point2D> polygon = polygonSquareMid;
    Region region(polygon, Point2D(1000, 2000), Point2D(6000, 2000));
    std::optional<InRegion::PolygonIntersection> inter;

    inter = region.nearestIntersectionAhead(Point2D(3000, 2000), Radian(M_PI / 2));
    ASSERT_TRUE(inter);
    EXPECT_NEAR(inter.value().point.getX(), 3000, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter.value().point.getY(), 6000, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.value().ringIndex, 0);
    EXPECT_EQ(inter.value().vertexIndex, 2);

    /// @note фэйлится тест
    inter = region.nearestIntersectionAhead(Point2D(3500, 1500), Radian(0));
    ASSERT_TRUE(inter);
    EXPECT_NEAR(inter.value().point.getX(), 6000, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter.value().point.getY(), 1500, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.value().ringIndex, 0);
    EXPECT_EQ(inter.value().vertexIndex, 4);

    polygon = {PrimaryEntities::Ring<Point2D>({Point2D{0, 0}, Point2D{2000, 2000}, Point2D{4000, 2000}, Point2D{4000, 0}, Point2D{2000, -2000}, Point2D{0, -2000}, Point2D{0, 0}})};
    region.setPolygon(polygon);
    inter = region.nearestIntersectionAhead(Point2D(1000, -1000), Radian(-M_PI / 2));
    ASSERT_TRUE(inter);
    EXPECT_NEAR(inter.value().point.getX(), 1000, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter.value().point.getY(), -2000, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.value().ringIndex, 0);
    EXPECT_EQ(inter.value().vertexIndex, 4);

    polygon.outer() = polygonSquareMid.outer();
    polygon.inners() = {PrimaryEntities::Ring<Point2D>({Point2D{3000, 2000}, Point2D{5000, 2000}, Point2D{5000, 5000}, Point2D{3000, 5000}, Point2D{3000, 2000}})};
    region.setPolygon(polygon);

    inter = region.nearestIntersectionAhead(Point2D(2000, 3000), Radian(0));
    ASSERT_TRUE(inter);
    EXPECT_NEAR(inter.value().point.getX(), 3000, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter.value().point.getY(), 3000, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.value().ringIndex, 1);
    EXPECT_EQ(inter.value().vertexIndex, 3);

    inter = region.nearestIntersectionAhead(Point2D(3500, 1500), Radian(M_PI / 2));
    ASSERT_TRUE(inter);
    EXPECT_NEAR(inter.value().point.getX(), 3500, ACCURACY_DOUBLE);
    EXPECT_NEAR(inter.value().point.getY(), 2000, ACCURACY_DOUBLE);
    EXPECT_EQ(inter.value().ringIndex, 1);
    EXPECT_EQ(inter.value().vertexIndex, 0);
}

TEST(region, nearestIntersectionAhead_largeValues)
{
    InRegion::Polygon metricPolygon = {{Point2D(0, 0), Point2D(0, 40000), Point2D(40000, 40000), Point2D(40000, 8000), Point2D(50000, 40000), Point2D(50000, 0)}, {Point2D(14000, 16000), Point2D(14000, 24000), Point2D(26000, 24000), Point2D(26000, 16000)}};
    Region metricRegion(metricPolygon, Point2D(0, 12000), Point2D(50000, 12000));
    Radian course(-2.0344439357957027);
    Radian turnAngle(0.98006219812081574);
    Radian nextCourseLeft = (course - turnAngle).normalize();
    Radian nextCourseRight = (course + turnAngle).normalize();
    Point2D coordinates(11673.037201230225, 17836.518600615116);
    std::optional<InRegion::PolygonIntersection> interLeft = metricRegion.nearestIntersectionAhead(coordinates, nextCourseLeft);
    std::optional<InRegion::PolygonIntersection> interRight = metricRegion.nearestIntersectionAhead(coordinates, nextCourseRight);
    ASSERT_TRUE(interLeft.has_value());
    EXPECT_TRUE(interRight.has_value());
}

} // namespace

TEST(region, calculateTackMax)
{
    PrimaryEntities::Polygon<Point2D> polygon;
    Region region;
    polygon = {PrimaryEntities::Ring<Point2D>({Point2D{0, 0}, Point2D{0, 9000}, Point2D{7000, 9000}, Point2D{2000, 7000}, Point2D{7000, 7000}, Point2D{7000, 0}, Point2D{0, 0}})};
    region.setPolygon(polygon);
    double polygonSize = region.size();
    InRegion::PolygonIntersection inter = {Point2D(3000, 0), 0, 5};
    EXPECT_NEAR(region.maxTackLength(Point2D(1000, 2000), inter), sqrt(2) * (2000 - TRAVERSA_MIN), ACCURACY_DOUBLE * polygonSize);

    inter = {Point2D(0, 3000), 0, 0};
    EXPECT_NEAR(region.maxTackLength(Point2D(2000, 1000), inter), sqrt(2) * (2000 - TRAVERSA_MIN), ACCURACY_DOUBLE * polygonSize);

    inter = {Point2D(0, 5000), 0, 0};
    EXPECT_NEAR(region.maxTackLength(Point2D(2000, 1000), inter), sqrt(5) * (2000 - TRAVERSA_MIN), ACCURACY_DOUBLE * polygonSize);

    inter = {Point2D(4000, 0), 0, 5};
    EXPECT_NEAR(region.maxTackLength(Point2D(4000, 2000), inter), (2000 - TRAVERSA_MIN), ACCURACY_DOUBLE * polygonSize);

    inter = {Point2D(6000, 0), 0, 5};
    EXPECT_NEAR(region.maxTackLength(Point2D(2000, 2000), inter), sqrt(5) * (2000 - TRAVERSA_MIN), ACCURACY_DOUBLE * polygonSize);

    inter = {Point2D(0, 0), 5, 0};
    EXPECT_THROW(region.maxTackLength(Point2D(1000, 1000), inter), std::runtime_error);
}

// nextCourse
namespace {

TEST_F(RegionTestFixture, nextCourse1)
{
    PrimaryEntities::Polygon<Point2D> polygon = polygonSquareMid;
    Region region(polygon, Point2D(2000, 1000), Point2D(2000, 6000));
    double polygonSize = region.size();

    EXPECT_NEAR(region.nextCourse(Point2D(3000, 2000), Radian(M_PI / 4), Radian(M_PI / 4), true)->first.get(), M_PI / 2, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(region.nextCourse(Point2D(2000, 3000), Radian(0), Radian(M_PI / 4), true)->first.get(), M_PI / 4, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(region.nextCourse(Point2D(3000, 5000), Radian(-M_PI / 2), Radian(M_PI / 4), true)->first.get(), -M_PI / 4, ACCURACY_DOUBLE * polygonSize);
}

TEST(region, nextCourse_AcuteAngle)
{
    PrimaryEntities::Polygon<Point2D> polygon = {PrimaryEntities::Ring<Point2D>({Point2D{1000, 1000}, Point2D{1000, 10000}, Point2D{3000, 10000}, Point2D{3000, 1000}, Point2D{1000, 1000}})};
    Region region(polygon, Point2D(2000, 1000), Point2D(2000, 10000));
    EXPECT_NEAR(region.nextCourse(Point2D(2000, 9000), Radian(M_PI / 4), Radian(M_PI / 4), true)->first.get(), -M_PI / 2, ACCURACY_DOUBLE * region.size());
}

TEST(region, nextCourse_ObtuseAngle)
{
    PrimaryEntities::Polygon<Point2D> polygon = {PrimaryEntities::Ring<Point2D>({Point2D{0, 0}, Point2D{0, 3000}, Point2D{2000, 3000}, Point2D{2000, 4000}, Point2D{4000, 4000}, Point2D{4000, 2000}, Point2D{3000, 2000}, Point2D{3000, 0}, Point2D{0, 0}})};
    Region region(polygon, Point2D(1000, 0), Point2D(1000, 3000));
    EXPECT_NEAR(region.nextCourse(Point2D(3000, 3000), Radian(M_PI / 4), Radian(M_PI / 4), true)->first.get(), -3 * M_PI / 4, ACCURACY_DOUBLE * region.size());
}

TEST(region, nextCourse_noValue)
{
    PrimaryEntities::Polygon<Point2D> polygon = {PrimaryEntities::Ring<Point2D>({Point2D{0, 0}, Point2D{0, 2000}, Point2D{2000, 2000}, Point2D{2000, 0}, Point2D{0, 0}})};
    Region region(polygon, Point2D(0, 0), Point2D(2000, 2000));
    auto result = region.nextCourse(Point2D(10000, 10000), Radian(M_PI / 4), Radian(M_PI / 4), true);
    EXPECT_TRUE(!result.has_value());
}

} // namespace

// longerTack
namespace {

TEST_F(RegionTestFixture, longerTack1)
{
    Region region(polygonSquareMin, Point2D(1000, 0), Point2D(1000, 4000));
    auto result = region.longerTack(Point2D{1000, 1000}, Radian{M_PI / 2}, Radian{M_PI / 2});
    EXPECT_DOUBLE_EQ(result->first.get(), 0.0);
    EXPECT_DOUBLE_EQ(result->second, 2500);
}

TEST_F(RegionTestFixture, longerTack2)
{
    Region region(polygonSquareMin, Point2D(1000, 0), Point2D(1000, 4000));
    auto result = region.longerTack(Point2D{0, 2000}, Radian{M_PI / 2}, Radian{M_PI / 2});
    EXPECT_DOUBLE_EQ(result->first.get(), 0.0);
    EXPECT_DOUBLE_EQ(result->second, 3500);
}

TEST_F(RegionTestFixture, longerTack3)
{
    Region region(polygonSquareMin, Point2D(1000, 0), Point2D(1000, 4000));
    auto result = region.longerTack(Point2D{10000, 10000}, Radian{M_PI / 2}, Radian{M_PI / 2});
    EXPECT_FALSE(result.has_value());
}

} // namespace

// entranceCourse
namespace {

TEST_F(RegionTestFixture, entranceCourse_OnBordered)
{
    Region region(polygonSquareMin, Point2D(1000, 0), Point2D(1000, 4000));
    EXPECT_DOUBLE_EQ(region.entranceCourse().get(), M_PI / 2);
}

TEST_F(RegionTestFixture, entranceCourse_Vertex)
{
    Region region(polygonSquareMin, Point2D(0, 0), Point2D(4000, 0));
    EXPECT_DOUBLE_EQ(region.entranceCourse().get(), M_PI / 4);
}

TEST(region, entranceCourse_NoThrow)
{
    PrimaryEntities::Polygon<Point2D> polygon = {PrimaryEntities::Ring<Point2D>({Point2D{0, 0}, Point2D{0, 10000}, Point2D{1000, 0}, Point2D{0, 0}})};
    Region region(polygon, Point2D(0, 4000), Point2D(0, 0));
    EXPECT_NO_THROW(region.entranceCourse());
}

} // namespace

// intersectionAll
namespace {

TEST_F(RegionTestFixture, intersectionAll_Diagonal)
{
    Region region;
    region.setPolygon(polygonSquareMin);
    double polygonSize = region.size();
    Geometry::Line line(Point2D{-2000, 0}, Point2D{6000, 4000});

    auto result = region.intersectionsAll(line, 0);

    EXPECT_EQ(result.size(), 2);
    EXPECT_NEAR(result[0].point.getX(), 0, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(result[0].point.getY(), 1000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_EQ(result[0].vertexIndex, 0);
    EXPECT_FALSE(result[0].isVertex);
    EXPECT_NEAR(result[1].point.getX(), 4000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(result[1].point.getY(), 3000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_EQ(result[1].vertexIndex, 2);
    EXPECT_FALSE(result[1].isVertex);
}

TEST_F(RegionTestFixture, intersectionAll_SameLinesIntersect)
{
    Region region;
    region.setPolygon(polygonSquareMin);
    double polygonSize = region.size();
    Geometry::Line line(Point2D{-1000, 4000}, Point2D{5000, 4000});

    auto result = region.intersectionsAll(line, 0);

    EXPECT_EQ(result.size(), 2);
    EXPECT_NEAR(result[0].point.getX(), 0, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(result[0].point.getY(), 4000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_EQ(result[0].vertexIndex, 1);
    EXPECT_TRUE(result[0].isVertex);
    EXPECT_NEAR(result[1].point.getX(), 4000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(result[1].point.getY(), 4000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_EQ(result[1].vertexIndex, 2);
    EXPECT_TRUE(result[1].isVertex);
}

TEST_F(RegionTestFixture, intersectionAll_ParallelLinesIntersect)
{
    Region region;
    region.setPolygon(polygonSquareMin);
    double polygonSize = region.size();
    Geometry::Line line(Point2D{-1000, 2000}, Point2D{5000, 2000});

    auto result = region.intersectionsAll(line, 0);

    EXPECT_EQ(result.size(), 2);
    EXPECT_NEAR(result[0].point.getX(), 0, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(result[0].point.getY(), 2000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_EQ(result[0].vertexIndex, 0);
    EXPECT_FALSE(result[0].isVertex);
    EXPECT_NEAR(result[1].point.getX(), 4000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(result[1].point.getY(), 2000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_EQ(result[1].vertexIndex, 2);
    EXPECT_FALSE(result[1].isVertex);
}

TEST_F(RegionTestFixture, intersectionAll_Vertex)
{
    Region region;
    region.setPolygon(polygonSquareMin);
    double polygonSize = region.size();
    Geometry::Line line(Point2D{-4000, 0}, Point2D{4000, 4000});

    auto result = region.intersectionsAll(line, 0);

    EXPECT_EQ(result.size(), 2);
    EXPECT_NEAR(result[0].point.getX(), 0, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(result[0].point.getY(), 2000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_EQ(result[0].vertexIndex, 0);
    EXPECT_FALSE(result[0].isVertex);
    EXPECT_NEAR(result[1].point.getX(), 4000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(result[1].point.getY(), 4000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_EQ(result[1].vertexIndex, 2);
    EXPECT_TRUE(result[1].isVertex);
}

TEST_F(RegionTestFixture, intersectionAll_VertexDouble)
{
    Region region;
    region.setPolygon(polygonSquareMin);
    double polygonSize = region.size();
    Geometry::Line line(Point2D{0, 0}, Point2D{4000, 4000});

    auto result = region.intersectionsAll(line, 0);

    EXPECT_EQ(result.size(), 2);
    EXPECT_NEAR(result[0].point.getX(), 0, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(result[0].point.getY(), 0, ACCURACY_DOUBLE * polygonSize);
    EXPECT_EQ(result[0].vertexIndex, 0);
    EXPECT_TRUE(result[0].isVertex);
    EXPECT_NEAR(result[1].point.getX(), 4000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(result[1].point.getY(), 4000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_EQ(result[1].vertexIndex, 2);
    EXPECT_TRUE(result[1].isVertex);
}

TEST_F(RegionTestFixture, intersectionAll_TouchVertex)
{
    Region region;
    region.setPolygon(polygonSquareMin);
    double polygonSize = region.size();
    Geometry::Line line(Point2D{3000, -1000}, Point2D{5000, 1000});

    auto result = region.intersectionsAll(line, 0);

    EXPECT_EQ(result.size(), 1);
    EXPECT_NEAR(result[0].point.getX(), 4000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(result[0].point.getY(), 0, ACCURACY_DOUBLE * polygonSize);
    EXPECT_EQ(result[0].vertexIndex, 3);
    EXPECT_TRUE(result[0].isVertex);
}

TEST_F(RegionTestFixture, intersectionAll_NoTouchInners)
{
    Region region;
    region.setPolygon(polygonWithInners);
    double polygonSize = region.size();
    Geometry::Line line(Point2D{-2000, 5000}, Point2D{12000, 5000});

    auto result = region.intersectionsAll(line);

    EXPECT_EQ(result.size(), 2);
    EXPECT_NEAR(result[0].point.getX(), 0, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(result[0].point.getY(), 5000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_EQ(result[0].vertexIndex, 0);
    EXPECT_FALSE(result[0].isVertex);
    EXPECT_NEAR(result[1].point.getX(), 10000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(result[1].point.getY(), 5000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_EQ(result[1].vertexIndex, 2);
    EXPECT_FALSE(result[1].isVertex);
}

TEST_F(RegionTestFixture, intersectionAll_TouchInners)
{
    Region region;
    region.setPolygon(polygonWithInners);
    std::vector<Point2D> standard{{0, 2000}, {1000, 2000}, {4000, 2000}, {6000, 2000}, {9000, 2000}, {10000, 2000}};
    Geometry::Line line(Point2D{-2000, 2000}, Point2D{12000, 2000});

    auto result = region.intersectionsAll(line);

    for (const auto& position : result)
    {
        ASSERT_TRUE(find(standard.begin(), standard.end(), position.point) != standard.end());
    }
    EXPECT_EQ(result.size(), standard.size());
}

TEST_F(RegionTestFixture, intersectionAll_TouchInnersVertex)
{
    Region region;
    region.setPolygon(polygonWithInners);
    std::vector<Point2D> standard{{0, 3000}, {1000, 4000}, {3000, 6000}, {6000, 9000}, {7000, 10000}};
    Geometry::Line line(Point2D{-3000, 0}, Point2D{10000, 13000});

    auto result = region.intersectionsAll(line);

    for (const auto& position : result)
    {
        ASSERT_TRUE(find(standard.begin(), standard.end(), position.point) != standard.end());
    }
    EXPECT_EQ(result.size(), standard.size());
}

TEST_F(RegionTestFixture, intersectionAll_TouchInnersSameLinesIntersect)
{
    Region region;
    region.setPolygon(polygonWithInners);
    std::vector<Point2D> standard{{0, 1000}, {1000, 1000}, {4000, 1000}, {6000, 1000}, {9000, 1000}, {10000, 1000}};
    Geometry::Line line(Point2D{-2000, 1000}, Point2D{12000, 1000});

    auto result = region.intersectionsAll(line);

    for (const auto& position : result)
    {
        ASSERT_TRUE(find(standard.begin(), standard.end(), position.point) != standard.end());
    }
    EXPECT_EQ(result.size(), standard.size());
}

TEST_F(RegionTestFixture, intersectionsAll_TouchVertexTriangleAndSameLinesIntersect)
{
    Region region;
    Polygon2D polygon(polygonNonconvex);
    region.setPolygon(polygon);
    double polygonSize = region.size();
    Geometry::Line line(Point2D{4000, -1}, Point2D{4000, 5000});

    auto result = region.intersectionsAll(line, 0);

    EXPECT_EQ(result.size(), 3);
    EXPECT_NEAR(result[0].point.getX(), 4000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(result[0].point.getY(), 5000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_EQ(result[0].vertexIndex, 2);
    EXPECT_TRUE(result[0].isVertex);
    EXPECT_NEAR(result[1].point.getX(), 4000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(result[1].point.getY(), 4000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_EQ(result[1].vertexIndex, 4);
    EXPECT_TRUE(result[1].isVertex);
    EXPECT_NEAR(result[2].point.getX(), 4000, ACCURACY_DOUBLE * polygonSize);
    EXPECT_NEAR(result[2].point.getY(), 0, ACCURACY_DOUBLE * polygonSize);
    EXPECT_EQ(result[2].vertexIndex, 5);
    EXPECT_TRUE(result[2].isVertex);
}

} // namespace

TEST_F(RegionTestFixture, tackToExit)
{
    Region region;
    region.setPolygon(polygonNonconvex);
    region.setExit({2000, 4000});
    Point2D pos{2000, 2000};

    BorderedLine tackToExit = region.tackToExit(pos);
    ASSERT_EQ(tackToExit.getStart(), pos);
    ASSERT_EQ(tackToExit.getFinish(), region.getPolygon().outer()[region.getExit()]);
}

// penultimatePosition (по без прокладки пути [по теореме косинусов])
namespace {

TEST_F(RegionTestFixture, penultimatePositionCosine_OcuteAngle)
{
    Region region;
    region.setPolygon(polygonSquareMin);
    region.setExit({2000, 4000});
    Point2D pos{1000, 1000};
    Point2D penultPos{3000, 1000};
    ASSERT_TRUE(region.penultimatePosition(pos, penultPos, 4000));
    ASSERT_NEAR(penultPos.getX(), 2000, ACCURACY_DOUBLE * region.size());
    ASSERT_NEAR(penultPos.getY(), 1000, ACCURACY_DOUBLE * region.size());
}

TEST_F(RegionTestFixture, penultimatePositionCosine_ObtuseNegativeAngle)
{
    Region region;
    region.setPolygon(polygonSquareMin);
    region.setExit({2000, 4000});
    Point2D pos{2000, 2000};
    Point2D penultPos{500, 500};
    double length = 1000 * (sqrt(2) + sqrt(10));
    ASSERT_TRUE(region.penultimatePosition(pos, penultPos, length));
    ASSERT_NEAR(penultPos.getX(), 1000, ACCURACY_DOUBLE * region.size());
    ASSERT_NEAR(penultPos.getY(), 1000, ACCURACY_DOUBLE * region.size());
}

TEST_F(RegionTestFixture, penultimatePositionCosine_StraightAngle)
{
    Region region;
    region.setPolygon(polygonSquareMin);
    region.setExit({1000, 0});
    Point2D pos{1000, 2000};
    Point2D penultPos{1000, 3500};
    ASSERT_TRUE(region.penultimatePosition(pos, penultPos, 4000));
    ASSERT_NEAR(penultPos.getX(), 1000, ACCURACY_DOUBLE * region.size());
    ASSERT_NEAR(penultPos.getY(), 3000, ACCURACY_DOUBLE * region.size());
}

TEST_F(RegionTestFixture, penultimatePositionCosine_remainingPathMoreThanDist)
{
    Region region;
    region.setPolygon(polygonSquareMin);
    region.setExit({2000, 4000});
    Point2D point(2000, 1000);
    Point2D positionCurr(2000, 2000);
    ASSERT_TRUE(!region.penultimatePosition(point, positionCurr, 4000));
}

TEST_F(RegionTestFixture, penultimatePositionCosine_remainingPathEqualDist)
{
    Region region;
    region.setPolygon(polygonSquareMin);
    region.setExit({2000, 4000});
    Point2D point(2000, 1000);
    Point2D positionCurr(2000, 2000);
    ASSERT_TRUE(!region.penultimatePosition(point, positionCurr, 3000));
}

} // namespace

// penultimatePosition (с прокладкой пути)
namespace {

TEST_F(RegionTestFixture, penultimatePosition1)
{
    Region region;
    Polygon2D polygon;
    polygon.outer() = polygonSquareBig.outer();
    polygon.inners() = {{{2000, 3000}, {2000, 7000}, {7000, 7000}, {7000, 3000}}};
    region.setPolygon(polygon);
    region.setExit({1000, 0});
    std::vector<Polygon2D> poly;
    const Polygon2D& polygonRef = region.getPolygon();
    //    AbstractOperations::correct(tmpPolygon);
    poly.emplace_back(polygonRef);

    auto triangl = std::make_shared<TriangularGeometry::TriangularArea>(poly);
    Point2D routeBack(5000, 8000), positionCurr(3000, 8000);
    const double distAccuracy = 6;
    double remainingPath = Geometry::distance(routeBack, positionCurr) + Geometry::distance(positionCurr, polygonRef.outer()[region.getExit()]);
    TriangularGeometry::PathFinder path(triangl);
    Point2D mFinal = polygonRef.outer()[region.getExit()];

    Point2D point = region.penultimatePosition(routeBack, positionCurr, distAccuracy, remainingPath, path);

    double distPath = Geometry::distance(routeBack, point);
    for (size_t i = 0; i < path.getMPathCorner().size() - 1; i++)
    {
        distPath += Geometry::distance(*path.getMPathCorner()[i], *path.getMPathCorner()[i + 1]);
    }

    ASSERT_NEAR(remainingPath, distPath, distAccuracy);
}

TEST_F(RegionTestFixture, penultimatePosition2)
{
    Region region;
    Polygon2D polygon;
    polygon.outer() = polygonSquareBig.outer();
    polygon.inners() = {{{3000, 4000}, {3000, 6000}, {6000, 6000}, {6000, 3000}}};
    region.setPolygon(polygon);
    region.setEntrance({0, 8000});
    region.setExit({1000, 0});
    std::vector<Polygon2D> poly;
    const Polygon2D& polygonRef = region.getPolygon();
    poly.emplace_back(polygonRef);

    auto triangl = std::make_shared<TriangularGeometry::TriangularArea>(poly);
    Point2D routeBack(8000, 8000), positionCurr(8000, 6000);
    const double distAccuracy = 6;
    double remainingPath = Geometry::distance(routeBack, positionCurr) + Geometry::distance(positionCurr, polygonRef.outer()[region.getExit()]);
    TriangularGeometry::PathFinder path(triangl);
    Point2D mFinal = polygonRef.outer()[region.getExit()];

    Point2D point = region.penultimatePosition(routeBack, positionCurr, distAccuracy, remainingPath, path);

    double distPath = Geometry::distance(routeBack, point);
    for (size_t i = 0; i < path.getMPathCorner().size() - 1; i++)
    {
        distPath += Geometry::distance(*path.getMPathCorner()[i], *path.getMPathCorner()[i + 1]);
    }

    ASSERT_NEAR(remainingPath, distPath, distAccuracy);
}

TEST_F(RegionTestFixture, penultimatePosition3)
{
    Region region;
    Polygon2D polygon;
    polygon.outer() = polygonSquareBig.outer();
    polygon.inners() = {{{3000, 4000}, {3000, 6000}, {6000, 6000}, {6000, 3000}}};
    region.setPolygon(polygon);
    region.setExit({1000, 0});
    std::vector<Polygon2D> poly;
    const Polygon2D& polygonRef = region.getPolygon();
    poly.emplace_back(polygonRef);

    auto triangl = std::make_shared<TriangularGeometry::TriangularArea>(poly);
    Point2D routeBack(8000, 6000), positionCurr(8000, 8000);

    const double distAccuracy = 6;
    double remainingPath = Geometry::distance(routeBack, positionCurr) + Geometry::distance(positionCurr, polygonRef.outer()[region.getExit()]);
    TriangularGeometry::PathFinder path(triangl);
    Point2D mFinal = polygonRef.outer()[region.getExit()];

    Point2D point = region.penultimatePosition(routeBack, positionCurr, distAccuracy, remainingPath, path);

    double distPath = Geometry::distance(routeBack, point);
    for (size_t i = 0; i < path.getMPathCorner().size() - 1; i++)
    {
        distPath += Geometry::distance(*path.getMPathCorner()[i], *path.getMPathCorner()[i + 1]);
    }

    ASSERT_NEAR(remainingPath, distPath, distAccuracy);
}

TEST(region, penultimatePosition4)
{
    Region region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 9000}, {16000, 9000}, {16000, 0}};
    polygon.inners() = {{{4000, 1000}, {4000, 8000}, {5000, 8000}, {5000, 1000}}};
    region.setPolygon(polygon);
    region.setExit({0, 4000});
    Point2D prevPnt{7000, 4000};
    Point2D currPnt{11000, 4000};
    double remainingPath = 13000;
    double distAccuracy = 6;
    std::vector<Polygon2D> polygons;
    const Polygon2D& polygonRef = region.getPolygon();
    polygons.emplace_back(polygonRef);

    auto triangulation = std::make_shared<TriangularGeometry::TriangularArea>(polygons);
    TriangularGeometry::PathFinder path(triangulation);
    Point2D res = region.penultimatePosition(prevPnt, currPnt, distAccuracy, remainingPath, path);

    ASSERT_NEAR(res.getX(), 9000, distAccuracy);
    ASSERT_NEAR(res.getY(), 4000, distAccuracy);
}

TEST_F(RegionTestFixture, penultimatePosition5)
{
    Region region;
    Polygon2D polygon;
    polygon.outer() = polygonSquareBig.outer();
    polygon.inners() = {{{2000, 3000}, {2000, 7000}, {7000, 7000}, {7000, 3000}}};
    region.setPolygon(polygon);
    region.setExit({1000, 0});

    Point2D prevPnt{5000, 8000};
    Point2D currPnt{3000, 8000};
    std::vector<Polygon2D> polygons;
    auto polygonRef = region.getPolygon();
    polygons.emplace_back(polygonRef);

    double remainingPath = Geometry::distance(prevPnt, currPnt) + Geometry::distance(currPnt, polygonRef.outer()[region.getExit()]);
    double distAccuracy = 6.;

    auto triangulation = std::make_shared<TriangularGeometry::TriangularArea>(polygons);
    TriangularGeometry::PathFinder path(triangulation);

    Geometry::Line line(prevPnt, currPnt);
    Region regionDupl(region);
    regionDupl.setExit({2000, 7000});

    Point2D res = region.penultimatePosition(prevPnt, currPnt, distAccuracy, remainingPath, path);

    ASSERT_NEAR(res.getX(), 4750, distAccuracy);
    ASSERT_NEAR(res.getY(), 8000, distAccuracy);
}

TEST_F(RegionTestFixture, penultimatePosition6)
{
    Region region;
    Polygon2D polygon;
    polygon.outer() = polygonSquareBig.outer();
    polygon.inners() = {{{3000, 4000}, {3000, 6000}, {6000, 6000}, {6000, 3000}}};
    region.setPolygon(polygon);
    region.setExit({1000, 0});

    std::vector<Polygon2D> polygons;
    auto polygonRef = region.getPolygon();
    polygons.emplace_back(polygonRef);
    auto triangulation = std::make_shared<TriangularGeometry::TriangularArea>(polygons);
    Point2D routeBack(8000, 7000), positionCurr(8000, 5000);
    const double distAccuracy = 6;
    double remainingPath = Geometry::distance(routeBack, positionCurr) + Geometry::distance(positionCurr, polygonRef.outer()[region.getExit()]);
    TriangularGeometry::PathFinder path(triangulation);
    Point2D mFinal = polygonRef.outer()[region.getExit()];

    Point2D point = region.penultimatePosition(routeBack, positionCurr, distAccuracy, remainingPath, path);

    double distPath = Geometry::distance(routeBack, point);
    for (size_t i = 0; i < path.getMPathCorner().size() - 1; i++)
    {
        distPath += Geometry::distance(*path.getMPathCorner()[i], *path.getMPathCorner()[i + 1]);
    }

    ASSERT_NEAR(remainingPath, distPath, distAccuracy);
}

} // namespace

// isSeen
namespace {

TEST_F(RegionTestFixture, isSeenTrue)
{
    Region region;
    Polygon2D polygon;
    polygon.outer() = polygonSquareMid.outer();
    polygon.inners() = {{{3000, 3000}, {3000, 5000}, {5000, 5000}, {5000, 3000}}};
    region.setPolygon(polygon);
    region.setEntrance({1000, 6000});
    region.setExit({3000, 1000});
    Point2D positionCurr(2000, 3000);
    ASSERT_TRUE(region.isSeen(positionCurr));
}

TEST_F(RegionTestFixture, isSeenFalse)
{
    Region region;
    Polygon2D polygon;
    polygon.outer() = polygonSquareMid.outer();
    polygon.inners() = {{{3000, 3000}, {3000, 5000}, {5000, 5000}, {5000, 3000}}};
    region.setPolygon(polygon);
    region.setEntrance({1000, 6000});
    region.setExit({3000, 1000});
    Point2D positionCurr(5000, 6000);
    ASSERT_FALSE(region.isSeen(positionCurr));
}

TEST(region, isSeenTrueNonConvexPoly)
{
    Region region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 6000}, {6000, 6000}, {6000, 1000}, {8000, 8000}, {8000, 0}};
    polygon.inners() = {{{2000, 2000}, {2000, 4000}, {4000, 4000}, {4000, 2000}}};
    region.setPolygon(polygon);
    region.setEntrance({0, 6000});
    region.setExit({2000, 0});
    Point2D positionCurr(5000, 2000);
    ASSERT_TRUE(region.isSeen(positionCurr));
}

TEST(region, isSeenConcavityTrue)
{
    Region region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 5}, {2, 2}, {4, 5}, {4, 0}, {0, 0}};
    region.setPolygon(polygon);
    region.setEntrance({0, 2});
    region.setExit({4, 2});
    Point2D positionCurr(3, 2);
    ASSERT_TRUE(region.isSeen(positionCurr));
}

TEST(region, isSeenConcavityFalse)
{
    Region region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 5}, {2, 2}, {4, 5}, {4, 0}, {0, 0}};
    region.setPolygon(polygon);
    region.setEntrance({0, 2});
    region.setExit({4, 3});
    Point2D positionCurr(1, 2);
    ASSERT_FALSE(region.isSeen(positionCurr));
}

TEST(region, isSeenConcavityThouchVertex)
{
    Region region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 5}, {2, 2}, {4, 5}, {4, 0}, {0, 0}};
    region.setPolygon(polygon);
    region.setEntrance({0, 2});
    region.setExit({4, 2});
    Point2D positionCurr(1, 2);
    ASSERT_TRUE(region.isSeen(positionCurr));
}

TEST(region, isSeenPosCurrStart)
{
    Region region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 5}, {10, 5}, {10, 0}, {0, 0}};
    region.setPolygon(polygon);
    region.setEntrance({0, 2});
    region.setExit({10, 2});
    Point2D positionCurr(0, 2);
    ASSERT_TRUE(region.isSeen(positionCurr));
}

TEST(region, isSeenPosTouchVertexInners1)
{
    Region region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 5}, {10, 5}, {10, 0}, {0, 0}};
    polygon.inners() = {{{3, 2}, {3, 3}, {7, 3}, {7, 2}, {3, 2}}};
    region.setPolygon(polygon);
    region.setEntrance({0, 0});
    region.setExit({5, 5});
    Point2D positionCurr(0, 0);

    ASSERT_TRUE(region.isSeen(positionCurr));
}

TEST(region, isSeenPosTouchVertexInners2)
{
    Region region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 6}, {6, 6}, {6, 0}, {0, 0}};
    polygon.inners() = {{{2, 2}, {2, 4}, {4, 4}, {4, 2}, {2, 2}}};
    region.setPolygon(polygon);
    region.setEntrance({1, 0});
    region.setExit({0, 4});
    Point2D positionCurr(3, 1);

    ASSERT_TRUE(region.isSeen(positionCurr));
}

TEST(region, isSeenPosTouchVertexInners3)
{
    Region region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 6}, {6, 6}, {6, 0}, {0, 0}};
    polygon.inners() = {{{2, 2}, {2, 4}, {4, 4}, {4, 2}, {2, 2}}};
    region.setPolygon(polygon);
    region.setEntrance({1, 0});
    region.setExit({0, 4});
    Point2D positionCurr(5, 4);

    ASSERT_TRUE(region.isSeen(positionCurr));
}

TEST(region, isSeenPosTouchTwoVertexInners)
{
    Region region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 6}, {6, 6}, {6, 0}, {0, 0}};
    polygon.inners() = {{{2, 2}, {2, 4}, {4, 4}, {4, 2}, {2, 2}}};
    region.setPolygon(polygon);
    region.setEntrance({1, 0});
    region.setExit({0, 6});
    Point2D positionCurr(5, 1);

    ASSERT_FALSE(region.isSeen(positionCurr));
}

TEST(SearchRegionTests, bug1)
{
    InRegion::Region region;
    Polygon2D polygon;
    polygon.outer() = {{{-56005.3361985711, -55214.3937600496},
                        {-55160.5435120901, 55227.0013002307},
                        {54340.8061712153, 55221.6656113184},
                        {55172.9604263196, -55220.6846885126},
                        {-56005.3361985711, -55214.3937600496}}};

    polygon.inners() = {{{-28211.5081638632, -27791.11322013737},
                         {27377.18039422075, -27794.758885906},
                         {27170.3353688776, 27479.9499885496},
                         {-27998.60753279395, 27482.3564027351},
                         {-28211.5081638632, -27791.11322013737}}};
    region.setPolygon(polygon);
    region.setEntrance({-56005.3361985711, -55214.3937600496});
    region.setExit({54340.8061712153, 55221.6656113184});
    Point2D positionCurr(-48208.34703113768, -18390.43633429121);

    ASSERT_FALSE(region.isSeen(positionCurr));
}

TEST(SearchRegionTests, bug2)
{
    InRegion::Region region;
    Polygon2D polygon;
    polygon.outer() = {{{-56005.3361985711, -55214.3937600496},
                        {-55160.5435120901, 55227.0013002307},
                        {54340.8061712153, 55221.6656113184},
                        {55172.9604263196, -55220.6846885126},
                        {-56005.3361985711, -55214.3937600496}}};

    polygon.inners() = {{{-28211.5081638632, -27791.11322013737},
                         {27377.18039422075, -27794.758885906},
                         {27170.3353688776, 27479.9499885496},
                         {-27998.60753279395, 27482.3564027351},
                         {-28211.5081638632, -27791.11322013737}}};
    region.setPolygon(polygon);
    region.setEntrance({-56005.3361985711, -55214.3937600496});
    region.setExit({54340.8061712153, 55221.6656113184});
    Point2D positionCurr(-8749.883470415014, -46101.70638477665);

    ASSERT_FALSE(region.isSeen(positionCurr));
}

} // namespace

TEST(region, pathLength)
{
    std::vector<Point2D> path = {{0, 0}, {1000, 0}, {2000, 0}, {3000, 0}};
    EXPECT_EQ(Operations::pathLength(path), 3000);
}
*/
