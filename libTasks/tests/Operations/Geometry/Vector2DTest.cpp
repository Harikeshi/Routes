#include <Task/Operations/Geometry/Vector2D.hpp>
#include <MathTools/AngularUnits/Radian.hpp>
#include <gtest/gtest.h>
#include <iostream>

using namespace Operations;

using point = Point2D;
using Vec = Vector2D<point>;

// Arrange
const Vector2D quadrant1(Point2D{0, 0}, Point2D{1, 1});
const Vector2D quadrant2(Point2D{0, 0}, Point2D{-1, 1});
const Vector2D quadrant3(Point2D{0, 0}, Point2D{-1, -1});
const Vector2D quadrant4(Point2D{0, 0}, Point2D{1, -1});
const Vector2D oxPlus(Point2D{0, 0}, Point2D{1, 0});
const Vector2D oxMinus(Point2D{0, 0}, Point2D{-1, 0});
const Vector2D oyPlus(Point2D{0, 0}, Point2D{0, 1});
const Vector2D oyMinus(Point2D{0, 0}, Point2D{0, -1});


TEST (Vector2D, constructorRadian)
{
    Radian phi(M_PI/4);
    double R = 1./sin(phi.get());
    point b {0,0};
    Vec V{b, R, phi};

    EXPECT_TRUE((V.b==b));
    EXPECT_NEAR(V.X, double{1.}, ACCURACY_DOUBLE);
    EXPECT_NEAR(V.Y, double{1.}, ACCURACY_DOUBLE);

}

TEST(Vector2D, getCourse)
{
    // Arrange & Act & Assert
/*
    EXPECT_NEAR(oxPlus.getCourse(), M_PI / 2, EPS);
    EXPECT_NEAR(oyPlus.getCourse(), 0, EPS);
    EXPECT_NEAR(oxMinus.getCourse(), - M_PI / 2, EPS);
    EXPECT_NEAR(oyMinus.getCourse(), M_PI, EPS);

    EXPECT_NEAR(quadrant1.getCourse(), M_PI / 4, EPS);
    EXPECT_NEAR(quadrant2.getCourse(), - M_PI / 4, EPS);
    EXPECT_NEAR(quadrant3.getCourse(), - 3 * M_PI / 4, EPS);
    EXPECT_NEAR(quadrant4.getCourse(), 3 * M_PI / 4, EPS);
    */

    EXPECT_NEAR(oxPlus.getCourse(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(oyPlus.getCourse(), M_PI / 2, ACCURACY_DOUBLE);
    EXPECT_NEAR(oxMinus.getCourse(), M_PI, ACCURACY_DOUBLE);
    EXPECT_NEAR(oyMinus.getCourse(), - M_PI / 2, ACCURACY_DOUBLE);

    EXPECT_NEAR(quadrant1.getCourse(), M_PI / 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(quadrant2.getCourse(), 3 * M_PI / 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(quadrant3.getCourse(), - 3 * M_PI / 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(quadrant4.getCourse(), - M_PI / 4, ACCURACY_DOUBLE);
}


TEST(Vector2D, getOrientedAngleTo)
{
    // Arrange & Act & Assert
    /*EXPECT_NEAR(oxPlus.getAngleTo(quadrant1, true).get(), M_PI / 4, EPS);
    EXPECT_NEAR(quadrant1.getAngleTo(oxPlus, true).get(), - M_PI / 4, EPS);
    EXPECT_NEAR(quadrant4.getAngleTo(quadrant1, true).get(), M_PI / 2, EPS);
    EXPECT_NEAR(quadrant2.getAngleTo(quadrant1, true).get(), - M_PI / 2, EPS);
    EXPECT_NEAR(oxPlus.getAngleTo(quadrant2, true).get(), 3 * M_PI / 4, EPS);
    EXPECT_NEAR(oxPlus.getAngleTo(quadrant3, true).get(), - 3 * M_PI / 4, EPS);
    EXPECT_NEAR(oxPlus.getAngleTo(quadrant4, true).get(), - M_PI / 4, EPS);*/

    EXPECT_NEAR(oxPlus.getAngleTo(quadrant1, true).get(), - M_PI / 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(quadrant1.getAngleTo(oxPlus, true).get(), M_PI / 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(quadrant4.getAngleTo(quadrant1, true).get(), - M_PI / 2, ACCURACY_DOUBLE);
    EXPECT_NEAR(quadrant2.getAngleTo(quadrant1, true).get(), M_PI / 2, ACCURACY_DOUBLE);
    EXPECT_NEAR(oxPlus.getAngleTo(quadrant2, true).get(), - 3 * M_PI / 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(oxPlus.getAngleTo(quadrant3, true).get(), 3 * M_PI / 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(oxPlus.getAngleTo(quadrant4, true).get(), M_PI / 4, ACCURACY_DOUBLE);
}

TEST(Vector2D, getAngleTo)
{
    //Arrange & Act & Assert
    EXPECT_NEAR(oxPlus.getAngleTo(quadrant1).get(), M_PI / 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(quadrant1.getAngleTo(oxPlus).get(), M_PI / 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(quadrant4.getAngleTo(quadrant1).get(), M_PI / 2, ACCURACY_DOUBLE);
    EXPECT_NEAR(quadrant2.getAngleTo(quadrant1).get(), M_PI / 2, ACCURACY_DOUBLE);
    EXPECT_NEAR(oxPlus.getAngleTo(quadrant2).get(), 3 * M_PI / 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(oxPlus.getAngleTo(quadrant3).get(), 3 * M_PI / 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(oxPlus.getAngleTo(quadrant4).get(), M_PI / 4, ACCURACY_DOUBLE);
    EXPECT_NEAR(oxPlus.getAngleTo(oxPlus).get(), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(oxMinus.getAngleTo(oxPlus).get(), M_PI, ACCURACY_DOUBLE);
}


TEST(Vector2D, operatorEqual)
{
    point b1{0,0};
    point e1{1,1};
    point b2{0,0};
    point e2{1,1};

    EXPECT_TRUE((Vec{b1,e1}==Vec{b2,e2}));
    EXPECT_TRUE((Vec{e1,b1}==Vec{e2,b2}));
    EXPECT_TRUE((Vec{e1,b1}!=Vec{b2,e2}));
}


TEST(Vector2D, operatorPlus)
{
    point P00{0,0};
    point P11{1,1};
    point P01{0,1};

    Vec a{P00,P11};
    Vec b{P11,P01};
    Vec c_{P00,P01};
    auto sum=a+b;
    // std::cout<< "c_.Abs() = " << c_.Abs() << "\n";
    // std::cout<< "sum.Abs() = " << sum.Abs() << "\n";
    // std::cout<< "sum.b.getX() =" <<  sum.b.getX() << "\n";
    // std::cout<< "sum.b.getY() =" <<  sum.b.getY() << "\n";
    // std::cout<< "sum.e.getX() =" <<  sum.e.getX() << "\n";
    // std::cout<< "sum.e.getY() =" <<  sum.e.getY() << "\n";

    // std::cout<< "c_.b.getX() =" <<  c_.b.getX() << "\n";
    // std::cout<< "c_.b.getX() =" <<  c_.b.getX() << "\n";
    // std::cout<< "c_.e.getX() =" <<  c_.e.getX() << "\n";
    // std::cout<< "c_.e.getY() =" <<  c_.e.getY() << "\n";


    EXPECT_TRUE((c_==sum));
}


TEST(Vector2D, MirrorTest)
{
    point b1{-1,0};
    point e1{-2,0};
    point b2{1,0};
    point e2{2,0};
    point P{0,0};

    Vec v{b1,e1};
    Vec vR = normalReflect(v,P);
    Vec vTst { b2,e2};

    // std::cout<< vTst.Abs() << " =? " << vR.Abs() << "\n";
    // std::cout<< vTst.e.getX() << " =? " << vR.e.getX() << "\n";
    // std::cout<< vTst.e.getY() << " =? " << vR.e.getY() << "\n";

    EXPECT_NEAR(vTst.Abs(), vR.Abs(), ACCURACY_DOUBLE);
    EXPECT_TRUE(vTst.e == vR.e);


}
