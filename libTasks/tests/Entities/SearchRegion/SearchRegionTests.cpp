#include <Task/Entities/SearchRegion.hpp>
#include <Task/Operations/RouteOperations.hpp>
#include <Task/Schemes/Search/Config.hpp>

#include <TriangularGeometry/PathFinder.hpp>
#include <TriangularGeometry/TriangularArea.hpp>

#include <gtest/gtest.h>

namespace Entities {
using namespace Schemes::Search;

TEST(SearchRegionTests, constructors)
{
    SearchRegion region;

    Polygon2D polygon = {{{1, 2}, {5, 6}, {9, 2}, {5, -2}}};

    region.setFromPolygon(polygon);
    region.setEntrance({1, 2});
    region.setExit({3, 0});

    SearchRegion region2(region);
    SearchRegion region3 = region;

    EXPECT_EQ(region2.getEntrance(), region3.getEntrance());
    EXPECT_EQ(region2.getExit(), region3.getExit());

    EXPECT_EQ((region2.outer()), (region3.outer()));
    EXPECT_EQ(region2.inners(), region3.inners());
}

TEST(SearchRegionTests, halfPerimeter)
{
    SearchRegion region;
    Polygon2D poly{{{-10., -10.}, {-10., 10}, {10., 10.}, {10., -10.}, {-10., -10.}}};
    region.setFromPolygon(poly);
    EXPECT_EQ(region.halfPerimeter(), 40);
}

TEST(SearchRegionTests, maxTackLength)
{
    PrimaryEntities::Polygon<Point2D> polygon;
    SearchRegion region;

    polygon = {PrimaryEntities::Ring<Point2D>({Point2D{0, 0}, Point2D{0, 9000}, Point2D{7000, 9000}, Point2D{2000, 7000}, Point2D{7000, 7000}, Point2D{7000, 0}, Point2D{0, 0}})};
    region.setFromPolygon(polygon);

    const double halfPerimeter = region.halfPerimeter();

    RingIntersection inter = RingIntersection{Intersection{{3000, 0}, 5}, false};

    EXPECT_NEAR(region.maxTackLength(Point2D(2000, 1000), inter), sqrt(2) * (1000 - TRAVERSA_MIN), ACCURACY_DOUBLE * halfPerimeter);

    inter = RingIntersection{Intersection{{0, 3000}, 0}, false};
    //{Point2D(0, 3), 0, 0};
    EXPECT_NEAR(region.maxTackLength(Point2D(2000, 1000), inter), sqrt(2) * (2000 - TRAVERSA_MIN), ACCURACY_DOUBLE * halfPerimeter);

    inter = RingIntersection{Intersection{{0, 5000}, 0}, false};
    //{Point2D(0, 5), 0, 0};
    EXPECT_NEAR(region.maxTackLength(Point2D(2000, 1000), inter), sqrt(5) * (2000 - TRAVERSA_MIN), ACCURACY_DOUBLE * halfPerimeter);

    inter = RingIntersection{Intersection{{4000, 0}, 5}, false};
    //{Point2D(4, 0), 0, 5};
    EXPECT_NEAR(region.maxTackLength(Point2D(4000, 2000), inter), (2000 - TRAVERSA_MIN), ACCURACY_DOUBLE * halfPerimeter);

    inter = RingIntersection{Intersection{{6000, 0}, 5}, false};
    //{Point2D(6, 0), 0, 5};
    EXPECT_NEAR(region.maxTackLength(Point2D(2000, 2000), inter), sqrt(5) * (2000 - TRAVERSA_MIN), ACCURACY_DOUBLE * halfPerimeter);

    inter = RingIntersection{Intersection{{0, 0}, 0}, true};
    //{Point2D(0, 0), 5, 0};
    EXPECT_THROW(region.maxTackLength(Point2D(1000, 1000), inter), std::runtime_error);
}

TEST(SearchRegionTests, nextCourse)
{
    PrimaryEntities::Polygon<Point2D> polygon = {PrimaryEntities::Ring<Point2D>({Point2D{1000, 1000}, Point2D{6000, 1000}, Point2D{6000, 6000}, Point2D{1000, 6000}, Point2D{1000, 1000}})};
    SearchRegion region(polygon, Point2D(2000, 1000), Point2D(2000, 6000));

    //    std::optional<Radian> resultOpt = {};
    //    resultOpt = region.nextCourse(Point2D(3, 2), Radian(M_PI/4), Radian(M_PI/4));
    EXPECT_NEAR(region.nextCourse(Point2D(3000, 2000), Radian(M_PI / 4), Radian(M_PI / 4), true, IntersectionMethod::Core)->first.get(), M_PI / 2, ACCURACY_RADIAN);
    EXPECT_NEAR(region.nextCourse(Point2D(2000, 3000), Radian(0), Radian(M_PI / 4), true, IntersectionMethod::Core)->first.get(), M_PI / 4, ACCURACY_RADIAN);
    EXPECT_NEAR(region.nextCourse(Point2D(3000, 5000), Radian(-M_PI / 2), Radian(M_PI / 4), true, IntersectionMethod::Core)->first.get(), -M_PI / 4, ACCURACY_RADIAN);
}

TEST(SearchRegionTests, longerTack)
{
    /// @todo
}

namespace { // effectiveWidth

TEST(SearchRegionTests, effectiveWidth1)
{
    SearchRegion region{
        {SearchRing{{0, 0}, {0, 9}, {9, 9}, {9, 3}, {13, 9}, {13, 0}, {0, 0}},
         SearchRing{{3, 3}, {3, 6}, {6, 6}, {6, 3}, {3, 3}}},
        {0, 1},
        {1, 0}};
    double detectionRange = 3;
    const Point2D& pnt{2, 8};
    double res = region.effectiveWidth(pnt, Radian{-M_PI / 2}, detectionRange);
    EXPECT_NEAR(res, 4, ACCURACY_DOUBLE);
}

TEST(SearchRegionTests, effectiveWidth2)
{
    SearchRegion region{
        {SearchRing{{0, 0}, {0, 9}, {9, 9}, {9, 3}, {13, 9}, {13, 0}, {0, 0}},
         SearchRing{{3, 3}, {3, 6}, {6, 6}, {6, 3}, {3, 3}}},
        {0, 1},
        {1, 0}};
    double detectionRange = 3;
    const Point2D& pnt{5.5, 7};
    double res = region.effectiveWidth(pnt, Radian{M_PI / 4}, detectionRange);
    EXPECT_NEAR(res, std::sqrt(2) * 3, ACCURACY_DOUBLE);
}

TEST(SearchRegionTests, effectiveWidth3)
{
    SearchRegion region{
        {SearchRing{{0, 0}, {0, 9}, {9, 9}, {9, 3}, {13, 9}, {13, 0}, {0, 0}},
         SearchRing{{3, 3}, {3, 6}, {6, 6}, {6, 3}, {3, 3}}},
        {0, 1},
        {1, 0}};
    double detectionRange = 3;
    const Point2D& pnt{11.5, 4};
    const Radian& dir{std::atan2(-1, 3)};
    double res = region.effectiveWidth(pnt, dir, detectionRange);
    EXPECT_NEAR(res, 3.1622776601683795, ACCURACY_DOUBLE);
}

TEST(SearchRegionTests, effectiveWidth4)
{
    SearchRegion region{
        {SearchRing{{0, 0}, {0, 9}, {9, 9}, {9, 3}, {13, 9}, {13, 0}, {0, 0}},
         SearchRing{{3, 3}, {3, 6}, {6, 6}, {6, 3}, {3, 3}}},
        {0, 1},
        {1, 0}};
    double detectionRange = 3;
    const Point2D& pnt{7, 1};
    double res = region.effectiveWidth(pnt, Radian{M_PI}, detectionRange);
    EXPECT_EQ(res, 6);
}
} // namespace

TEST(SearchRegionTests, tackToExit)
{
    SearchRegion region;
    Polygon2D polygon({PrimaryEntities::Ring<Point2D>{{0, 0}, {4, 0}, {4, 4}, {2, 4}, {4, 5}, {0, 5}, {0, 0}}});
    region.setFromPolygon(polygon);
    region.setExit({2, 4});
    Point2D pos{2, 2};

    Geometry::BorderedLine tackToExit = region.tackToExit(pos);

    auto a = tackToExit.getFinish();
    auto x = region.getExitPoint();

    ASSERT_EQ(tackToExit.getStart(), pos);
    ASSERT_EQ(tackToExit.getFinish(), region.getExitPoint());
}

// penultimatePosition (по без прокладки пути [по теореме косинусов])
namespace {

TEST(SearchRegionTests, penultimatePositionCosine_OcuteAngle)
{
    SearchRegion region;
    Polygon2D polygon({PrimaryEntities::Ring<Point2D>{{0, 0}, {4, 0}, {4, 4}, {0, 4}, {0, 0}}});
    region.setFromPolygon(polygon);
    region.setExit({2, 4});
    Point2D pos{1, 1};
    Point2D penultPos{3, 1};

    ASSERT_TRUE(region.penultimatePosition(pos, penultPos, 4));
    ASSERT_NEAR(penultPos.getX(), 2, ACCURACY_DOUBLE);
    ASSERT_NEAR(penultPos.getY(), 1, ACCURACY_DOUBLE);
}

TEST(SearchRegionTests, penultimatePositionCosine_ObtuseNegativeAngle)
{
    SearchRegion region;
    Polygon2D polygon({PrimaryEntities::Ring<Point2D>{{0, 0}, {4, 0}, {4, 4}, {0, 4}, {0, 0}}});
    region.setFromPolygon(polygon);
    region.setExit({2, 4});
    Point2D pos{2, 2};
    Point2D penultPos{0.5, 0.5};
    double length = sqrt(2) + sqrt(10);

    ASSERT_TRUE(region.penultimatePosition(pos, penultPos, length));
    ASSERT_EQ(penultPos, Point2D(1, 1));
}

TEST(SearchRegionTests, penultimatePositionCosine_StraightAngle)
{
    SearchRegion region;
    Polygon2D polygon({PrimaryEntities::Ring<Point2D>{{0, 0}, {4, 0}, {4, 4}, {0, 4}, {0, 0}}});
    region.setFromPolygon(polygon);
    region.setExit({1, 0});
    Point2D pos{1, 2};
    Point2D penultPos{1, 3.5};

    ASSERT_TRUE(region.penultimatePosition(pos, penultPos, 4));
    ASSERT_NEAR(penultPos.getX(), 1, ACCURACY_DOUBLE);
    ASSERT_NEAR(penultPos.getY(), 3, ACCURACY_DOUBLE);
}

TEST(SearchRegionTests, penultimatePositionCosine_remainingPathMoreThanDist)
{
    SearchRegion region;
    Polygon2D polygon({PrimaryEntities::Ring<Point2D>{{0, 0}, {4, 0}, {4, 4}, {0, 4}, {0, 0}}});
    region.setFromPolygon(polygon);
    region.setExit({2, 4});
    Point2D point(2, 1);
    Point2D positionCurr(2, 2);

    ASSERT_TRUE(!region.penultimatePosition(point, positionCurr, 4));
}

TEST(SearchRegionTests, penultimatePositionCosine_remainingPathEqualDist)
{
    SearchRegion region;
    Polygon2D polygon({PrimaryEntities::Ring<Point2D>{{0, 0}, {4, 0}, {4, 4}, {0, 4}, {0, 0}}});
    region.setFromPolygon(polygon);
    region.setExit({2, 4});
    Point2D point(2, 1);
    Point2D positionCurr(2, 2);

    ASSERT_TRUE(!region.penultimatePosition(point, positionCurr, 3));
}

} // namespace

// penultimatePosition (с прокладкой пути)
namespace {

TEST(SearchRegionTests, pathToExit1)
{
    SearchRegion region{
        {SearchRing{{0, 0}, {9, 0}, {9, 9}, {0, 9}},
         SearchRing{{2, 3}, {2, 7}, {7, 7}, {7, 3}}},
        {0, 1},
        {1, 0}};

    auto poly = std::vector<Polygon2D>{region.getPolygon()};

    TriangularGeometry::TriangularArea area(poly);

    TriangularGeometry::PathFinder path(std::make_shared<TriangularGeometry::TriangularArea>(area));

    auto triangles = area.getTriangles();

    Point2D routeBack(5, 8), positionCurr(3, 8);
    const double lastOptimalPath = 0.006;

    double remainingPath = Geometry::distance(routeBack, positionCurr) + Geometry::distance(positionCurr, region.getExitPoint());

    Point2D mFinal = region.getExitPoint();

    std::vector<Point2D> pathToExit = region.pathToExitBisection(routeBack, positionCurr, lastOptimalPath, remainingPath, path);

    const double pathLen = Geometry::distance(routeBack, pathToExit[0]) + Operations::pathLength(pathToExit);

    ASSERT_NEAR(remainingPath, pathLen, lastOptimalPath);
}

TEST(SearchRegionTests, pathToExit2)
{
    SearchRegion region{
        {SearchRing{{0, 0}, {0, 9}, {9, 9}, {9, 0}},
         SearchRing{{3, 4}, {3, 6}, {6, 6}, {6, 3}}},
        {0, 8},
        {1, 0}};

    std::vector<Polygon2D> poly{region.getPolygon()};

    auto triangl = std::make_shared<TriangularGeometry::TriangularArea>(poly);

    Point2D routeBack(8, 8), positionCurr(8, 6);
    const double lastOptimalPath = 0.006;

    double remainingPath = Geometry::distance(routeBack, positionCurr) + Geometry::distance(positionCurr, region.getExitPoint());
    TriangularGeometry::PathFinder path(triangl);
    Point2D mFinal = region.getExitPoint();

    std::vector<Point2D> pathToExit = region.pathToExitBisection(routeBack, positionCurr, lastOptimalPath, remainingPath, path);
    const double pathLen = Geometry::distance(routeBack, pathToExit[0]) + Operations::pathLength(pathToExit);

    ASSERT_NEAR(remainingPath, pathLen, lastOptimalPath);
}

TEST(SearchRegionTests, pathToExit3)
{
    SearchRegion region{
        {SearchRing{{0, 0}, {0, 9}, {9, 9}, {9, 0}},
         SearchRing{{3, 4}, {3, 6}, {6, 6}, {6, 3}}},
        {0, 0},
        {1, 0}};

    std::vector<Polygon2D> poly{region.getPolygon()};

    auto triangl = std::make_shared<TriangularGeometry::TriangularArea>(poly);
    Point2D routeBack(8, 6), positionCurr(8, 8);

    const double lastOptimalPath = 0.006;
    double remainingPath = Geometry::distance(routeBack, positionCurr) + Geometry::distance(positionCurr, region.getExitPoint());
    TriangularGeometry::PathFinder path(triangl);

    Point2D mFinal = region.getExit();

    std::vector<Point2D> pathToExit = region.pathToExitBisection(routeBack, positionCurr, lastOptimalPath, remainingPath, path);
    const double pathLen = Geometry::distance(routeBack, pathToExit[0]) + Operations::pathLength(pathToExit);

    ASSERT_NEAR(remainingPath, pathLen, lastOptimalPath);
}

TEST(SearchRegionTests, pathToExit4)
{
    SearchRegion region{
        {SearchRing{{0, 0}, {0, 9}, {16, 9}, {16, 0}},
         SearchRing{{4, 1}, {4, 8}, {5, 8}, {5, 1}}},
        {0, 0},
        {0, 4}};

    Point2D prevPnt{7, 4};
    Point2D currPnt{11, 4};
    double remainingPath = 13;
    std::vector<Polygon2D> polygons{region.getPolygon()};

    auto triangulation = std::make_shared<TriangularGeometry::TriangularArea>(polygons);
    TriangularGeometry::PathFinder path(triangulation);
    std::vector<Point2D> pathToExit = region.pathToExitBisection(prevPnt, currPnt, 0.006, remainingPath, path);

    ASSERT_NEAR(pathToExit[0].getX(), 9., 0.006);
    ASSERT_NEAR(pathToExit[0].getY(), 4., 0.006);
}

TEST(SearchRegionTests, pathToExit5)
{
    SearchRegion region{
        {SearchRing{{0, 0}, {0, 9}, {9, 9}, {9, 0}},
         SearchRing{{2, 3}, {2, 7}, {7, 7}, {7, 3}}},
        {0, 0},
        {1, 0}};

    Point2D prevPnt{5, 8};
    Point2D currPnt{3, 8};
    std::vector<Polygon2D> polygons{region.getPolygon()};

    double remainingPath = Geometry::distance(prevPnt, currPnt) + Geometry::distance(currPnt, region.getExitPoint());

    auto triangulation = std::make_shared<TriangularGeometry::TriangularArea>(polygons);
    TriangularGeometry::PathFinder path(triangulation);

    Geometry::Line line(prevPnt, currPnt);
    SearchRegion regionDupl(region);
    regionDupl.setExit({2, 7});

    std::vector<Point2D> pathToExit = region.pathToExitBisection(prevPnt, currPnt, 0.006, remainingPath, path);

    ASSERT_NEAR(pathToExit[0].getX(), 4.75, 0.006);
    ASSERT_NEAR(pathToExit[0].getY(), 8., 0.006);
}

TEST(SearchRegionTests, pathToExit6)
{
    SearchRegion region{
        {SearchRing{{0, 0}, {0, 9}, {9, 9}, {9, 0}},
         SearchRing{{3, 4}, {3, 6}, {6, 6}, {6, 3}}},
        {0, 0},
        {1, 0}};

    std::vector<Polygon2D> polygons{region.getPolygon()};

    auto triangulation = std::make_shared<TriangularGeometry::TriangularArea>(polygons);

    Point2D routeBack(8, 7), positionCurr(8, 5);
    const double lastOptimalPath = 0.006;
    double remainingPath = Geometry::distance(routeBack, positionCurr) + Geometry::distance(positionCurr, region.getExitPoint());
    TriangularGeometry::PathFinder path(triangulation);
    Point2D mFinal = region.getExit();

    std::vector<Point2D> pathToExit = region.pathToExitBisection(routeBack, positionCurr, lastOptimalPath, remainingPath, path);
    const double pathLen = Geometry::distance(routeBack, pathToExit[0]) + Operations::pathLength(pathToExit);

    ASSERT_NEAR(remainingPath, pathLen, lastOptimalPath);
}
} // namespace

// isSeen
namespace {

TEST(SearchRegionTests, isSeenTrue)
{
    SearchRegion region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 6000}, {6000, 6000}, {6000, 1000}, {8000, 8000}, {8000, 0}};
    polygon.inners() = {{{2000, 2000}, {2000, 4000}, {4000, 4000}, {4000, 2000}}};
    region.setFromPolygon(polygon);
    region.setEntrance({0, 6000});
    region.setExit({2000, 0});
    Point2D positionCurr(5000, 2000);
    ASSERT_TRUE(region.isSeen(positionCurr));
}

TEST(SearchRegionTests, isSeenFalse)
{
    SearchRegion region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 6000}, {6000, 6000}, {6000, 1000}, {8000, 8000}, {8000, 0}};
    polygon.inners() = {{{3000, 3000}, {3000, 5000}, {5000, 5000}, {5000, 3000}}};
    region.setFromPolygon(polygon);
    region.setEntrance({1000, 6000});
    region.setExit({3000, 0});
    Point2D positionCurr(5000, 6000);
    ASSERT_FALSE(region.isSeen(positionCurr));
}

TEST(SearchRegionTests, isSeenFalse_outer)
{
    SearchRegion region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 35000}, {15000, 35000}, {25000, 25000}, {32000, 5000}, {37000, 25000}, {42000, 35000}, {60000, 35000}, {60000, 0}, {20000, -5000}, {0, 0}};
    region.setFromPolygon(polygon);
    region.setEntrance({0, 12000});
    region.setExit({60000, 12000});
    Point2D positionCurr(8000, 15000);
    ASSERT_FALSE(region.isSeen(positionCurr));
}

TEST(SearchRegionTests, isSeenTrueNonConvexPoly)
{
    SearchRegion region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 6000}, {6000, 6000}, {6000, 1000}, {8000, 8000}, {8000, 0}};
    polygon.inners() = {{{2000, 2000}, {2000, 4000}, {4000, 4000}, {4000, 2000}}};
    region.setFromPolygon(polygon);
    region.setEntrance({0, 6000});
    region.setExit({2000, 0});
    Point2D positionCurr(5000, 2000);
    ASSERT_TRUE(region.isSeen(positionCurr));
}

TEST(SearchRegionTests, isSeenConcavityTrue)
{
    SearchRegion region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 5}, {2, 2}, {4, 5}, {4, 0}, {0, 0}};
    region.setFromPolygon(polygon);
    region.setEntrance({0, 2});
    region.setExit({4, 2});
    Point2D positionCurr(3, 2);
    ASSERT_TRUE(region.isSeen(positionCurr));
}

TEST(SearchRegionTests, isSeenConcavityFalse)
{
    SearchRegion region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 5}, {2, 2}, {4, 5}, {4, 0}, {0, 0}};
    region.setFromPolygon(polygon);
    region.setEntrance({0, 2});
    region.setExit({4, 3});
    Point2D positionCurr(1, 2);
    ASSERT_FALSE(region.isSeen(positionCurr));
}

TEST(SearchRegionTests, isSeenConcavityThouchVertex)
{
    SearchRegion region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 5}, {2, 2}, {4, 5}, {4, 0}, {0, 0}};
    region.setFromPolygon(polygon);
    region.setEntrance({0, 2});
    region.setExit({4, 2});
    Point2D positionCurr(1, 2);
    ASSERT_TRUE(region.isSeen(positionCurr));
}

TEST(SearchRegionTests, isSeenPosCurrStart)
{
    SearchRegion region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 5}, {10, 5}, {10, 0}, {0, 0}};
    region.setFromPolygon(polygon);
    region.setEntrance({0, 2});
    region.setExit({10, 2});
    Point2D positionCurr(0, 2);
    ASSERT_TRUE(region.isSeen(positionCurr));
}

TEST(SearchRegionTests, isSeenPosTouchVertexInners1)
{
    SearchRegion region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 5}, {10, 5}, {10, 0}, {0, 0}};
    polygon.inners() = {{{3, 2}, {3, 3}, {7, 3}, {7, 2}, {3, 2}}};
    region.setFromPolygon(polygon);
    region.setEntrance({0, 0});
    region.setExit({5, 5});
    Point2D positionCurr(0, 0);

    ASSERT_TRUE(region.isSeen(positionCurr));
}

TEST(SearchRegionTests, isSeenPosTouchVertexInners2)
{
    SearchRegion region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 6}, {6, 6}, {6, 0}, {0, 0}};
    polygon.inners() = {{{2, 2}, {2, 4}, {4, 4}, {4, 2}, {2, 2}}};
    region.setFromPolygon(polygon);
    region.setEntrance({1, 0});
    region.setExit({0, 4});
    Point2D positionCurr(3, 1);

    ASSERT_TRUE(region.isSeen(positionCurr));
}

TEST(SearchRegionTests, isSeenPosTouchVertexInners3)
{
    SearchRegion region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 6}, {6, 6}, {6, 0}, {0, 0}};
    polygon.inners() = {{{2, 2}, {2, 4}, {4, 4}, {4, 2}, {2, 2}}};
    region.setFromPolygon(polygon);
    region.setEntrance({1, 0});
    region.setExit({0, 4});
    Point2D positionCurr(5, 4);

    ASSERT_TRUE(region.isSeen(positionCurr));
}

TEST(SearchRegionTests, isSeenPosTouchTwoVertexInners)
{
    SearchRegion region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 6}, {6, 6}, {6, 0}, {0, 0}};
    polygon.inners() = {{{2, 2}, {2, 4}, {4, 4}, {4, 2}, {2, 2}}};
    region.setFromPolygon(polygon);
    region.setEntrance({1, 0});
    region.setExit({0, 6});
    Point2D positionCurr(5, 1);

    ASSERT_FALSE(region.isSeen(positionCurr));
}

TEST(SearchRegionTests, bug1)
{
    SearchRegion region;
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
    region.setFromPolygon(polygon);
    region.setEntrance({-56005.3361985711, -55214.3937600496});
    region.setExit({54340.8061712153, 55221.6656113184});
    Point2D positionCurr(-48208.34703113768, -18390.43633429121);

    ASSERT_FALSE(region.isSeen(positionCurr));
}

TEST(SearchRegionTests, bug2)
{
    SearchRegion region;
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
    region.setFromPolygon(polygon);
    region.setEntrance({-56005.3361985711, -55214.3937600496});
    region.setExit({54340.8061712153, 55221.6656113184});
    Point2D positionCurr(-8749.883470415014, -46101.70638477665);

    ASSERT_FALSE(region.isSeen(positionCurr));
}

} // namespace

TEST(SearchRegionTests, pathLength)
{
    std::vector<Point2D> path = {{0, 0}, {1, 0}, {2, 0}, {3, 0}};

    EXPECT_EQ(Operations::pathLength(path), 3);
}

// patrollingLength
namespace {

TEST(SearchRegionTests, patrollingLengthEven)
{
    SearchRegion region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 5}, {5, 5}, {5, 0}};
    region.setFromPolygon(polygon);
    region.setEntrance({1, 0});
    region.setExit({0, 0});
    int countOfRepeats = 2;
    Point2D pointFirst(0, 5), pointSecond(5, 5);

    auto length = region.patrollingLength(countOfRepeats, pointFirst, pointSecond);

    EXPECT_EQ(length, 15);
}

TEST(SearchRegionTests, patrollingLengthOdd)
{
    SearchRegion region;
    Polygon2D polygon;
    polygon.outer() = {{0, 0}, {0, 5}, {5, 5}, {5, 0}};
    region.setFromPolygon(polygon);
    region.setEntrance({1, 0});
    region.setExit({0, 0});
    int countOfRepeats = 3;
    Point2D pointFirst(5, 5), pointSecond(0, 5);

    auto length = region.patrollingLength(countOfRepeats, pointFirst, pointSecond);

    EXPECT_EQ(length, 20);
}
} // namespace

TEST(SearchRegionTests, classify)
{
}

} // namespace Entities
