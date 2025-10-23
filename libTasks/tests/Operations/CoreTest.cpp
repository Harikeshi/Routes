#include <Geometry/Objects/BorderedLine.hpp>
#include <Geometry/Objects/Line.hpp>
#include <MathTools/AngularUnits/RadiansOperations.hpp>
#include <MathTools/BasicMath/Arithmetic.hpp>
#include <Point/Point2D.hpp>
#include <PrimaryEntities/GeometryEntities.hpp>
#include <TriangularGeometry/PathFinder.hpp>
#include <gtest/gtest.h>

using namespace Geometry;

TEST(core, BorderedLine_horizontalVerticalIntersection)
{
    const double length = 10;
    BorderedLine horizontalSeg(Point2D(3.5, 1.5), Point2D(13.5, 1.5));
    BorderedLine horizontalSegEps = {Point2D(3.5, 1.5), Point2D(13.5, 1.50000000000007)};
    BorderedLine verticalSeg(Point2D(6, 1), Point2D(6, 6));
    BorderedLine verticalSegEps(Point2D(6, 1.000000000000005), Point2D(6, 6));

    Point2D pHorizTest = horizontalSeg.crossPoint(verticalSeg); // pHorizTest  X: 6, Y: 1.5
    EXPECT_TRUE(horizontalSeg.atBorderLine(pHorizTest));

    Point2D pHorizEpsTest = horizontalSegEps.crossPoint(verticalSeg); // pHorizTest  X: 6, Y: 1.5
    EXPECT_NEAR(pHorizEpsTest.getX(), 6, ACCURACY_DOUBLE * length);
    EXPECT_NEAR(pHorizEpsTest.getY(), 1.5, ACCURACY_DOUBLE * length);
    EXPECT_TRUE(horizontalSegEps.nearBorderLine(pHorizEpsTest, ACCURACY_DOUBLE * length));

    Point2D pVertEpsTest = horizontalSeg.crossPoint(verticalSegEps);
    EXPECT_TRUE(verticalSegEps.atBorderLine(pVertEpsTest));

    BorderedLine seg45(Point2D(-1, -1), Point2D(1, 1));
    Point2D point1(Point2D(-0.5, -0.5));
    EXPECT_TRUE(seg45.atBorderLine(point1));

    Point2D point2(Point2D(-2, -2));
    EXPECT_FALSE(seg45.atBorderLine(point2));
}

TEST(core, BorderedLine_crossPoint_test1)
{
    BorderedLine xPlus1000 = {{-1000, 0}, {1000, 0}};
    BorderedLine yPlus = {{0, -1}, {0, 1}};
    BorderedLine xMinus = {{1, 0}, {-1, 0}};
    BorderedLine yMinus1000 = {{0, 1000}, {0, -1000}};
    Point2D pnt;

    EXPECT_TRUE(xPlus1000.crossPoint(yPlus, pnt));
    EXPECT_TRUE(xPlus1000.nearBorderLine(pnt, ACCURACY_DOUBLE * 1000));
    EXPECT_TRUE(yPlus.nearBorderLine(pnt, ACCURACY_DOUBLE));
    EXPECT_TRUE(xMinus.crossPoint(yMinus1000, pnt));
    EXPECT_TRUE(xMinus.nearBorderLine(pnt, ACCURACY_DOUBLE));
    EXPECT_TRUE(yMinus1000.nearBorderLine(pnt, ACCURACY_DOUBLE * 1000));
    EXPECT_TRUE(xPlus1000.crossPoint(yMinus1000, pnt));
    EXPECT_TRUE(xPlus1000.nearBorderLine(pnt, ACCURACY_DOUBLE * 1000));
    EXPECT_TRUE(yMinus1000.nearBorderLine(pnt, ACCURACY_DOUBLE * 1000));
    EXPECT_TRUE(xMinus.crossPoint(yPlus, pnt));
    EXPECT_TRUE(xMinus.nearBorderLine(pnt, ACCURACY_DOUBLE));
    EXPECT_TRUE(yPlus.nearBorderLine(pnt, ACCURACY_DOUBLE));

    BorderedLine directProportionPlus = {{-1, -1}, {1, 1}};
    BorderedLine directProportionMinus = {{1, 1}, {-1, -1}};
    BorderedLine reversePropotion = {{-1, 1}, {1, -1}};
    EXPECT_TRUE(directProportionPlus.crossPoint(reversePropotion, pnt));
    EXPECT_TRUE(directProportionPlus.nearBorderLine(pnt, ACCURACY_DOUBLE));
    EXPECT_TRUE(reversePropotion.nearBorderLine(pnt, ACCURACY_DOUBLE));
    EXPECT_EQ(pnt.getX(), 0);
    EXPECT_EQ(pnt.getY(), 0);
    EXPECT_TRUE(directProportionMinus.crossPoint(reversePropotion, pnt));
    EXPECT_TRUE(directProportionMinus.nearBorderLine(pnt, ACCURACY_DOUBLE));
    EXPECT_TRUE(reversePropotion.nearBorderLine(pnt, ACCURACY_DOUBLE));
    EXPECT_EQ(pnt.getX(), 0);
    EXPECT_EQ(pnt.getY(), 0);
    EXPECT_TRUE(directProportionMinus.crossPoint(xPlus1000, pnt));
    EXPECT_TRUE(directProportionMinus.nearBorderLine(pnt, ACCURACY_DOUBLE * 1000));
    EXPECT_TRUE(xPlus1000.nearBorderLine(pnt, ACCURACY_DOUBLE * 1000));
    EXPECT_EQ(pnt.getX(), 0);
    EXPECT_EQ(pnt.getY(), 0);
}

TEST(core, BorderedLine_crossPoint_test2)
{
    BorderedLine seg1(Point2D(0, 0), Point2D(2, 0));
    BorderedLine seg2(Point2D(-1, -1), Point2D(1, 1));
    Point2D pnt;
    EXPECT_TRUE(seg1.crossPoint(BorderedLine({-1, -1}, {1, 1}), pnt));
    EXPECT_TRUE(seg1.nearBorderLine(pnt, ACCURACY_DOUBLE));
    EXPECT_TRUE(seg2.nearBorderLine(pnt, ACCURACY_DOUBLE));
    seg1 = {{2, 0}, {0, 0}};
    seg2 = {{1, 1}, {-1, -1}};
    EXPECT_TRUE(seg1.crossPoint(seg2, pnt));
    EXPECT_TRUE(seg1.nearBorderLine(pnt, ACCURACY_DOUBLE));
    EXPECT_TRUE(seg2.nearBorderLine(pnt, ACCURACY_DOUBLE));

    seg1 = {{-1, 0}, {1, 0}};
    EXPECT_TRUE(seg1.crossPoint(BorderedLine({100, 100}, {-100, -100}), pnt));
    EXPECT_TRUE(seg1.nearBorderLine(pnt, ACCURACY_DOUBLE));
    EXPECT_TRUE(seg2.nearBorderLine(pnt, ACCURACY_DOUBLE));
    EXPECT_TRUE(seg1.crossPoint(BorderedLine({1, 1}, {-1, -1}), pnt));
    EXPECT_TRUE(seg1.nearBorderLine(pnt, ACCURACY_DOUBLE));
    EXPECT_TRUE(seg2.nearBorderLine(pnt, ACCURACY_DOUBLE));
    EXPECT_TRUE(seg1.crossPoint(BorderedLine({-1, -1}, {1, 1}), pnt));
    EXPECT_TRUE(seg1.nearBorderLine(pnt, ACCURACY_DOUBLE));
    EXPECT_TRUE(seg2.nearBorderLine(pnt, ACCURACY_DOUBLE));
}

TEST(core, Line_crossPoint)
{
    Point2D position{16.208825109880799, 17.753730673423565};
    Radian course{1.0860003348270282};
    BorderedLine edge{{20, 20}, {0, 20}};
    Line line(position, course);
    Point2D crossP;
    ASSERT_TRUE(line.crossPoint(edge, crossP));
    ASSERT_TRUE(edge.nearBorderLine(crossP));
}

namespace { // ANS deploy test cases
TEST(core, Line_test1)
{
    Point2D position{-4.9286957271528431, -3.4327813220716878};
    Radian course = Radian{-1.4989877029523124 + M_PI - 1.0844201259890636};
    Line line(position, course);
    Point2D vert_0{-5.786187952246565, 5.522028313135551};
    Point2D vert_1{8.67746132910483, 1.9297585081993995};
    Point2D vert_2{7.120609698253412, -4.19461600814587};
    BorderedLine edge_1{vert_0, vert_1};
    BorderedLine edge_2{vert_1, vert_2};
    Point2D crossP_1, crossP_2;
    if (line.crossPoint(edge_1, crossP_1) || line.crossPoint(edge_2, crossP_2))
    {
        ASSERT_TRUE(edge_1.nearBorderLine(crossP_1) || edge_2.nearBorderLine(crossP_2));
    }
    else
    {
        FAIL();
    }
}

TEST(core, Line_test2)
{
    Point2D position{-4.9286957271528431, -3.4327813220716878};
    Radian course{1.6426049506374807};
    Line line(position, course);
    Point2D vert_0{-5.786187952246565, 5.522028313135551};
    Point2D vert_1{8.67746132910483, 1.9297585081993995};
    BorderedLine edge{vert_0, vert_1};
    Point2D crossPnt = line.crossPoint(edge);
    ASSERT_TRUE(edge.nearBorderLine(crossPnt /*, EPS*/));
}

TEST(core, Line_test3)
{
    Point2D position{-4.9286957271528431, -3.4327813220716878};
    Radian course{1.6426049506374807 + 0.1};
    Line line(position, course);
    Point2D vert_3{-7.44411057562514, -4.425133272618021};
    Point2D vert_4{-5.786187952246565, 5.522028313135551};
    BorderedLine edge{vert_3, vert_4};
    Point2D crossPnt = line.crossPoint(edge);
    ASSERT_TRUE(edge.nearBorderLine(crossPnt /*, EPS*/));
}

TEST(core, Line_test4)
{
    Point2D pos{-6.4524473953593464, -4.0093880253707797};
    Radian dirR{0.016657049152038805};
    Radian dirL{1.2190654197220319};
    Line lineR{pos, dirR};
    Line lineL{pos, dirL};

    Point2D vert_0{-5.786187952246565, 5.522028313135551};
    Point2D vert_1{8.67746132910483, 1.9297585081993995};
    Point2D vert_2{7.120609698253412, -4.19461600814587};
    BorderedLine edgeR{vert_1, vert_2};
    BorderedLine edgeL{vert_0, vert_1};

    Point2D cpR = lineR.crossPoint(edgeR);
    Point2D cpL = lineL.crossPoint(edgeL);
    ASSERT_TRUE(edgeR.nearBorderLine(cpR) && edgeL.nearBorderLine(cpL));
}

TEST(core, Line_test5)
{
    Point2D pos{2.192706772894051, 3.5403470624125712};
    Point2D vert_0{-5.786187952246565, 5.522028313135551};
    Point2D vert_1{8.67746132910483, 1.9297585081993995};
    BorderedLine edge{vert_0, vert_1};
    ASSERT_TRUE(edge.nearBorderLine(pos));
}

} // namespace

namespace { // metric_units_tests

// теперь проходит тест
TEST(core, BorderedLine_haveIntersection)
{
    BorderedLine longVerticalSeg(Point2D(50000, 0), Point2D(0, 0));
    auto size = longVerticalSeg.getLength();
    BorderedLine longSlopedLineSeg(Point2D(11673.037201230225, 17836.518600615116), Point2D(266.01416110042373, -71437.664819688129));
    Point2D inter;
    EXPECT_TRUE(longVerticalSeg.crossPoint(longSlopedLineSeg, inter));
    EXPECT_TRUE(longVerticalSeg.nearBorderLine(inter, ACCURACY_DOUBLE * size));
    /// @bug
    EXPECT_TRUE(longSlopedLineSeg.nearBorderLine(inter, ACCURACY_DOUBLE * size));

    BorderedLine longVerticalSegInt(Point2D(20000, 0), Point2D(0, 0));
    BorderedLine longSlopedLineSegInt(Point2D(11673.037201230225, 17836.518600615116), Point2D(266.014161100423, -71437.664819688129));
    Point2D interInt;
    EXPECT_TRUE(longVerticalSegInt.crossPoint(longSlopedLineSegInt, interInt));

    EXPECT_TRUE(longVerticalSegInt.nearBorderLine(interInt, ACCURACY_DOUBLE * size));
    /// @bug
    EXPECT_TRUE(longSlopedLineSegInt.nearBorderLine(interInt, ACCURACY_DOUBLE * size));
}

TEST(core, Line_haveIntersection)
{
    Point2D pos{-43987.299442149917, 28163.013184998166};
    ;
    Radian dir{1.6405754995460371};
    Line line{pos, dir};

    Point2D vert_1{-55160.5435120901, 55227.0013002307};
    Point2D vert_2{54340.8061712153, 55221.6656113184};
    BorderedLine edge{vert_1, vert_2};

    auto size = edge.getLength();

    Point2D cp = line.crossPoint(edge);
    ASSERT_TRUE(edge.nearBorderLine(cp, ACCURACY_DOUBLE * size));
}

} // namespace

TEST(core, nearBorderLine)
{
    BorderedLine edge({6122.547665537495, -7187.326997384482}, {-796.0298630247817, -7187.334259529256});
    Point2D pnt{-556.61437384710734, -7187.3343233468495};
    EXPECT_TRUE(edge.nearBorderLine(pnt, 14.));
}

TEST(core, hasPointNear)
{
    BorderedLine edge({6122.547665537495, -7187.326997384482}, {-796.0298630247817, -7187.334259529256});
    Point2D pnt{-556.61437384710734, -7187.3343233468495};
    EXPECT_TRUE(edge.hasPointNear(pnt, 14.));
}

TEST(TriangulationTest, Test)
{
    Polygon2D polygon;
    polygon.outer().emplace_back(Point2D(0.0, 0.0));
    polygon.outer().emplace_back(Point2D(0.0, 5.0));
    polygon.outer().emplace_back(Point2D(3.0, 5.0));
    polygon.outer().emplace_back(Point2D(3.0, 2.0));
    polygon.outer().emplace_back(Point2D(5.0, 5.0));
    polygon.outer().emplace_back(Point2D(5.0, 0.0));
    polygon.outer().emplace_back(Point2D(0.0, 0.0));

    polygon.inners().emplace_back(std::initializer_list<Point2D>());
    polygon.inners().back().emplace_back(Point2D(1.0, 2.0));
    polygon.inners().back().emplace_back(Point2D(2.0, 2.0));
    polygon.inners().back().emplace_back(Point2D(2.0, 3.0));
    polygon.inners().back().emplace_back(Point2D(1.0, 3.0));
    polygon.inners().back().emplace_back(Point2D(1.0, 2.0));

    std::vector<Polygon2D> poly;
    poly.emplace_back(polygon);
    std::shared_ptr<TriangularGeometry::TriangularArea> triangl = std::make_shared<TriangularGeometry::TriangularArea>(poly);

    TriangularGeometry::PathFinder path(triangl);
    EXPECT_NO_THROW(path.findPath(Point2D(0.5, 2.5), Point2D(5.0, 2.0)));
}

TEST(core, bisector)
{
    Radian alpha = Radian{-7 * M_PI / 8};
    Radian beta = -alpha;

    Radian bis = bisector(alpha, beta);
    ASSERT_NEAR(bis.get(), 0, ACCURACY_RADIAN);

    std::swap(alpha, beta);
    Radian bisInv = bisector(alpha, beta);
    ASSERT_NEAR((bisInv - Radian{M_PI}).normalize().get(), 0, ACCURACY_RADIAN);

    // pi, -7/8*pi, -15/16*pi
    alpha.set(M_PI);
    bis = bisector(alpha, beta);
    ASSERT_NEAR((bis - Radian{-15 * M_PI / 16}).normalize().get(), 0, ACCURACY_RADIAN);

    // pi, -pi / 2, 0
    beta.set(0);
    bis = bisector(alpha, beta);
    ASSERT_NEAR((bis - Radian{-M_PI / 2}).normalize().get(), 0, ACCURACY_RADIAN);

    // 0, pi/2, pi
    std::swap(alpha, beta);
    bis = bisector(alpha, beta);
    ASSERT_NEAR((bis - Radian{M_PI / 2}).normalize().get(), 0, ACCURACY_RADIAN);
}
