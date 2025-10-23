#include <Task/Schemes/Restore/ProryvK.hpp>
#include <Task/Schemes/Restore/Input.hpp>

#include <gtest/gtest.h>

using RestoreInput = Schemes::Restore::Input;
using ProryvK = Schemes::Restore::ProryvK;
using VecPoints = std::vector<Point2D>;

//Против часовой
class ProryvTestFixture : public testing::Test
{
protected:
    RestoreInput input;
    VecPoints vecTrue;

    void SetUp() override
    {
        input.Tpk       = Point2D{ 0.0, 0.0 }; // Tpk
        input.Tbek_0    = Point2D{ 2.1, 1 }; // Tbek_0
        input.Tbek_exit = Point2D{ 5., 5. }; // Tbek_exit
        input.Dpgas     = 4.0; // Dpgas
        input.Dbgas     = 4; //  Dbgas
        input.Kt_deg    = 180;//90 - -90, // Kt_deg
        input.Kbek_deg  = -147; // (90 - (237 - 360)) - 360 //  Kbek_deg
        input.Ut        = 8; //   Ut
        input.Ukp       = 12; //   Ukp
        input.Ukph      = 16; //        Ukp
        input.Ukp_r     = 12;//         Ukp
    }
};

TEST_F(ProryvTestFixture, Test1)
{
    vecTrue =     {Point2D{2.1,      1.00000},
                   Point2D{0,	     1.2},
                   Point2D{-1.20000, 1.20000},
                   Point2D{-1.2,	 0},
                   Point2D{-1.20000, -1.2},
                   Point2D{0,        -1.20000},
                   Point2D{1.2,	     -1.2},
                   Point2D{1.2,	     -0.9821},
                   Point2D{-2.6,	 -2.6},
                   Point2D{-11.9301, -2.6},
                   Point2D{-14.5967, -2.6},
                   Point2D{5.0,	     5.0}};

    ProryvK proryv{input};
    Outputs::Route testRoute = proryv.calculate();

    ASSERT_TRUE(testRoute.points.size() > 0);
    for (size_t i = 0; i < vecTrue.size(); ++i) {
        EXPECT_NEAR(vecTrue[i].getX(), testRoute.points[i].getX(), 0.001);
        EXPECT_NEAR(vecTrue[i].getY(), testRoute.points[i].getY(), 0.001);
    }
}

TEST_F(ProryvTestFixture, Test2)
{
    input.Tbek_0 = {3.5, 2};
    vecTrue =     {Point2D{3.5,       2},
                   Point2D{0,         1.2},
                   Point2D{-1.20000,  1.2},
                   Point2D{-1.2,      0},
                   Point2D{-1.20000,  -1.2},
                   Point2D{0,         -1.2},
                   Point2D{0.3073,	  -1.2},
                   Point2D{-2.6,      -2.6},
                   Point2D{-11.0268,  -2.6},
                   Point2D{-13.6935,  -2.6},
                   Point2D{5.0,       5.0}};

    ProryvK proryv{input};
    Outputs::Route testRoute = proryv.calculate();

    ASSERT_TRUE(testRoute.points.size() > 0);
    ASSERT_EQ(vecTrue.size(), testRoute.points.size());
    for (size_t i = 0; i < vecTrue.size(); ++i) {
        EXPECT_NEAR(vecTrue[i].getX(), testRoute.points[i].getX(), 0.001);
        EXPECT_NEAR(vecTrue[i].getY(), testRoute.points[i].getY(), 0.001);
    }
}

TEST_F(ProryvTestFixture, Test3)
{
    input.Tbek_0 = {1.5, 1};
    vecTrue =     {Point2D{1.5,       1},
                   Point2D{0,	      1.2},
                   Point2D{-1.20000,  1.20000},
                   Point2D{-1.2,	  0},
                   Point2D{-1.20000,  -1.2},
                   Point2D{0,	      -1.20000},
                   Point2D{1.2,	      -1.2},
                   Point2D{1.2,	      -0.535},
                   Point2D{-2.6,	  -2.6},
                   Point2D{-12.1249,  -2.6},
                   Point2D{-14.7915,  -2.6},
                   Point2D{5.0,       5.0}};

    ProryvK proryv{input};
    Outputs::Route testRoute = proryv.calculate();

    ASSERT_TRUE(testRoute.points.size() > 0);
    ASSERT_EQ(vecTrue.size(), testRoute.points.size());
    for (size_t i = 0; i < vecTrue.size(); ++i) {
        EXPECT_NEAR(vecTrue[i].getX(), testRoute.points[i].getX(), 0.001);
        EXPECT_NEAR(vecTrue[i].getY(), testRoute.points[i].getY(), 0.001);
    }
}

TEST_F(ProryvTestFixture, Test4)
{
    input.Tbek_0 = {1.5, -0.5};
    vecTrue =     {Point2D{1.5,        -0.5},
                   Point2D{0,          -1.2},
                   Point2D{-1.20000,   -1.20000},
                   Point2D{-1.2,	   0},
                   Point2D{-1.20000,   1.2},
                   Point2D{0,	       1.20000},
                   Point2D{1.2,	       1.2},
                   Point2D{1.2,	       0.6414},
                   Point2D{-2.6,	   2.6},
                   Point2D{-12.075,	   2.6},
                   Point2D{-14.7417,   2.6},
                   Point2D{5.0,        5.0}};

    ProryvK proryv{input};
    Outputs::Route testRoute = proryv.calculate();

    ASSERT_TRUE(testRoute.points.size() > 0);
    ASSERT_EQ(vecTrue.size(), testRoute.points.size());
    for (size_t i = 0; i < vecTrue.size(); ++i) {
        EXPECT_NEAR(vecTrue[i].getX(), testRoute.points[i].getX(), 0.001);
        EXPECT_NEAR(vecTrue[i].getY(), testRoute.points[i].getY(), 0.001);
    }
}

TEST_F(ProryvTestFixture, Test5)
{
    input.Tbek_0 = {5, 1};
    vecTrue =     {Point2D{5,          1},
                   Point2D{0,	       1.2},
                   Point2D{-1.20000,   1.20000},
                   Point2D{-1.2,	   0},
                   Point2D{-1.20000,   -1.2},
                   Point2D{-0.753,	   -1.20000},
                   Point2D{-2.6,	   -2.6},
                   Point2D{-10.1176,   -2.6},
                   Point2D{-12.7843,   -2.6},
                   Point2D{5,	       5}};

    ProryvK proryv{input};
    Outputs::Route testRoute = proryv.calculate();

    ASSERT_TRUE(testRoute.points.size() > 0);
    ASSERT_EQ(vecTrue.size(), testRoute.points.size());
    for (size_t i = 0; i < vecTrue.size(); ++i) {
        EXPECT_NEAR(vecTrue[i].getX(), testRoute.points[i].getX(), 0.001);
        EXPECT_NEAR(vecTrue[i].getY(), testRoute.points[i].getY(), 0.001);
    }
}
