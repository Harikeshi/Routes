#include <Task/Schemes/Search/ByCall/SectorSpiral.hpp>
#include <gtest/gtest.h>

using namespace Schemes::Search::ByCall;

TEST(SectorSpiral, base)
{
    Input input;
    input.initialPosition = Point2D(-75, 150);
    input.ship.detectionRange = 5;
    input.ship.maxVelocity = 18;
    input.ship.searchVelocity = 12;

    input.target.contactPoint = Point2D(10, 10);
    input.target.contactRms = 2;
    input.target.contactVelocity = 13;
    input.target.midVelocity = 15;
    input.target.minVelocity = 10;
    input.target.maxVelocity = 30;

    input.target.timeSinceContact = 0.5;
    input.target.courseSector.first = Radian{M_PI / 2};
    input.target.courseSector.second = Radian{Degree{65}};

    SectorSpiral sector{input};
    std::vector<Point2D> points;
    EXPECT_NO_THROW(points = sector.calculate().points);
    EXPECT_EQ(points.size(), 4);
    EXPECT_DOUBLE_EQ(points[1].getX(), 10.00000000000001);
    EXPECT_DOUBLE_EQ(points[1].getY(), 174.37778948709882);
    EXPECT_DOUBLE_EQ(points[2].getX(), 84.66449569994484);
    EXPECT_DOUBLE_EQ(points[2].getY(), 170.11852774488784);
    EXPECT_DOUBLE_EQ(points[3].getX(), 43.30637611169174);
    EXPECT_DOUBLE_EQ(points[3].getY(), 160.23535395317472);
}


TEST(SectorSpiral, CollinearPositions)
{
    Input input;
    input.initialPosition = Point2D(20, -20);
    input.ship.detectionRange = 100;
    input.ship.maxVelocity = 20;
    input.ship.searchVelocity = 20;

    input.target.contactPoint = Point2D(0, 0);
    input.target.contactRms = 0;
    input.target.contactVelocity = 21;
    input.target.midVelocity = 10;
    input.target.minVelocity = 4 * sqrt(3);
    input.target.maxVelocity = 20;

    input.target.timeSinceContact = 0.;
    input.target.courseSector.first = Radian{0};
    input.target.courseSector.second= Radian{Degree{60}};

    Point2D benchmark1(20, 0);
    Point2D benchmark2(10, 20 * std::sin(M_PI / 3));
    Point2D benchmark3(15, 5 * sqrt(3));

    SectorSpiral sector{input};
    Outputs::Route result;

    EXPECT_NO_THROW(result = sector.calculate());
    std::vector<Point2D> positions = result.points;
    EXPECT_EQ(positions.size(), 4);
    EXPECT_NEAR(positions[1].getX(), benchmark1.getX(), ACCURACY_DOUBLE);
    EXPECT_NEAR(positions[1].getY(), benchmark1.getY(), ACCURACY_DOUBLE);
    EXPECT_NEAR(positions[2].getX(), benchmark2.getX(), ACCURACY_DOUBLE);
    EXPECT_NEAR(positions[2].getY(), benchmark2.getY(), ACCURACY_DOUBLE);
    EXPECT_NEAR(positions[3].getX(), benchmark3.getX(), ACCURACY_DOUBLE);
    EXPECT_NEAR(positions[3].getY(), benchmark3.getY(), ACCURACY_DOUBLE);
}

TEST(SectorSpiral, standardScheme)
{
    Input input;
    input.initialPosition = Point2D(-75, 150);
    input.ship.detectionRange = 5;
    input.ship.maxVelocity = 18;
    input.ship.searchVelocity = 13;

    input.target.contactPoint = Point2D(10, 10);
    input.target.contactRms = 2;
    input.target.contactVelocity = 12;
    input.target.midVelocity = 15;
    input.target.minVelocity = 10;
    input.target.maxVelocity = 30;

    input.target.timeSinceContact = 0.5;
    input.target.courseSector.first = Radian{M_PI / 2};
    input.target.courseSector.second = Radian{Degree{65}};

    SectorSpiral sector{input};
    std::vector<Point2D> points;
    EXPECT_NO_THROW(points = sector.calculate().points);
    EXPECT_EQ(points.size(), 4);
    EXPECT_DOUBLE_EQ(points[1].getX(), 10.000000000000004);
    EXPECT_DOUBLE_EQ(points[1].getY(), 88.10019920307433);
    EXPECT_DOUBLE_EQ(points[2].getX(), 99.46043769816995);
    EXPECT_DOUBLE_EQ(points[2].getY(), 201.8485277555397);
    EXPECT_DOUBLE_EQ(points[3].getX(), 86.81728388816146);
    EXPECT_DOUBLE_EQ(points[3].getY(), 356.5003756625522);
}

TEST(SectorSpiral, standardScheme_integerPoints)
{

    Input input;
    input.initialPosition = Point2D(4, 18);
//    input.ship.detectionRange = 5;
    input.ship.maxVelocity = 3;
    input.ship.searchVelocity = input.ship.maxVelocity;

    input.target.contactPoint = Point2D(0, 0);
    input.target.contactRms = 0;
    input.target.contactVelocity = 1;
//    input.target.midVelocity = 15;
//    input.target.minVelocity = 10;
//    input.target.maxVelocity = 30;

    input.target.timeSinceContact = 0.;
    input.target.courseSector.second = Radian{0};
    input.target.courseSector.first = Radian{atan2(3, 4)};
    SectorSpiral sector{input};
    std::vector<Point2D> points;
    EXPECT_NO_THROW(points = sector.calculate().points);

    EXPECT_DOUBLE_EQ(points[1].getX(), 4);
    EXPECT_DOUBLE_EQ(points[1].getY(), 3);
    EXPECT_DOUBLE_EQ(points[2].getX(), 6.25);
    EXPECT_DOUBLE_EQ(points[2].getY(), 0.);
}
