#include <Task/Schemes/Restore/PetljaK.hpp>
#include <Task/Schemes/Restore/Input.hpp>

#include <gtest/gtest.h>

using PetljaInput = Schemes::Restore::Input;
using PetljaK = Schemes::Restore::PetljaK;
using VecPoints = std::vector<Point2D>;

//Против часовой
class PetljaTestFixture : public testing::Test
{
protected:
    PetljaInput input;
    VecPoints vecTrue;

    void SetUp() override
    {
        input.Tpk       = Point2D{ 0.0, 0.0 }; // Tpk
        input.Tbek_0    = Point2D{ 2, 1 }; // Tbek_0
        input.Tbek_exit = Point2D{ 5., 5. }; // Tbek_exit
        input.Dpgas     = 4.0; // Dpgas
        input.Dbgas     = 4; //  Dbgas
        input.Kt_deg    = 180;//90 - -90, /
        input.Kbek_deg  = -60; //90 - 150,
        input.Ut        = 8; //   Ut
        input.Ukp       = 12; //   Ukp
        input.Ukph      = 16; //        Ukp
        input.Ukp_r     = 12;//         Ukp

        vecTrue = {Point2D {2.15678,   0.64609},
                   Point2D {1.20000,   0.00000},
                   Point2D {1.20000,   -1.20000},
                   Point2D {0.00000,   -1.20000},
                   Point2D {-1.20000,  -1.20000},
                   Point2D {-1.20000,  0.00000},
                   Point2D {-1.20000,  1.20000},
                   Point2D {0.00000,   1.20000},
                   Point2D {0.46877,   1.20000},
                   Point2D {-2.60000,  2.60000},
                   Point2D {-10.61377, 2.60000},
                   Point2D {-13.03977, 2.60000},
                   Point2D {50.00000,  20.000000},
                   };
    }
};

TEST_F(PetljaTestFixture, Test1)
{
    vecTrue = {Point2D {2.00000,	 1.00000},
               Point2D {1.20000,	 0.00000},
               Point2D {1.20000,	 -1.20000},
               Point2D {0.00000,	 -1.20000},
               Point2D {-1.20000,	 -1.20000},
               Point2D {-1.20000,	 0.00000},
               Point2D {-1.20000,	 1.20000},
               Point2D {-0.36047,	 1.20000},
               Point2D {-2.60000,	 2.60000},
               Point2D {-10.44112,   2.60000},
               Point2D {-13.10778,   2.60000},
               Point2D {5.0,	     5.0}};

    PetljaK petlja{input};
    Outputs::Route testRoute = petlja.calculate();

    ASSERT_TRUE(testRoute.points.size() > 0);
    ASSERT_EQ(vecTrue.size(), testRoute.points.size());
    for (size_t i = 0; i < vecTrue.size(); ++i) {
        EXPECT_NEAR(vecTrue[i].getX(), testRoute.points[i].getX(), 0.001);
        EXPECT_NEAR(vecTrue[i].getY(), testRoute.points[i].getY(), 0.001);
    }
}

TEST_F(PetljaTestFixture, Test2)
{
    input.Tbek_0 = {4, 1};
    vecTrue =     {Point2D {4.00000,	   1.00000},
                   Point2D {1.20000,	   0.00000},
                   Point2D {1.20000,	  -1.20000},
                   Point2D {0.00000,	  -1.20000},
                   Point2D {-1.20000,	  -1.20000},
                   Point2D {-1.20000,	   0.00000},
                   Point2D {-1.20000,	   0.77009},
                   Point2D {-2.60000,	   2.60000},
                   Point2D {-10.104,	   2.60000},
                   Point2D {-12.771,	   2.60000},
                   Point2D {5.0,	       5.0}};

    PetljaK petlja{input};
    Outputs::Route testRoute = petlja.calculate();

    ASSERT_TRUE(testRoute.points.size() > 0);
    ASSERT_EQ(vecTrue.size(), testRoute.points.size());
    for (size_t i = 0; i < vecTrue.size(); ++i) {
        EXPECT_NEAR(vecTrue[i].getX(), testRoute.points[i].getX(), 0.001);
        EXPECT_NEAR(vecTrue[i].getY(), testRoute.points[i].getY(), 0.001);
    }
}

TEST_F(PetljaTestFixture, Test3)
{
    input.Tbek_0 = {2, 1};
    input.Ut = 7;
    vecTrue =     {Point2D{2.00000,	   1.00000},
                   Point2D{1.20000,	   0.00000},
                   Point2D{1.20000,	   -1.20000},
                   Point2D{0.00000,	   -1.20000},
                   Point2D{-1.20000,   -1.20000},
                   Point2D{-1.20000,   0.00000},
                   Point2D{-1.20000,   1.20000},
                   Point2D{0.00000,	   1.20000},
                   Point2D{0.75382,	   1.20000},
                   Point2D{-2.6,       2.60000},
                   Point2D{-10.04889,  2.60000},
                   Point2D{-12.3822,   2.60000},
                   Point2D{5.0,	       5.0}};

    PetljaK petlja{input};
    Outputs::Route testRoute = petlja.calculate();

    ASSERT_TRUE(testRoute.points.size() > 0);
    ASSERT_EQ(vecTrue.size(), testRoute.points.size());
    for (size_t i = 0; i < vecTrue.size(); ++i) {
        EXPECT_NEAR(vecTrue[i].getX(), testRoute.points[i].getX(), 0.001);
        EXPECT_NEAR(vecTrue[i].getY(), testRoute.points[i].getY(), 0.001);
    }
}

TEST_F(PetljaTestFixture, Test4)
{
    input.Ut = 10;
    vecTrue =     {Point2D{2.00000,	   1.00000},
                   Point2D{1.20000,	   0.00000},
                   Point2D{1.20000,	   -1.20000},
                   Point2D{0.00000,	   -1.20000},
                   Point2D{-1.20000,   -1.20000},
                   Point2D{-1.20000,   0.00000},
                   Point2D{-1.20000,   0.47953},
                   Point2D{-2.60000,   2.60000},
                   Point2D{-13.7682,   2.60000},
                   Point2D{-17.10157,  2.60000},
                   Point2D{5.0,        5.0}};

    PetljaK petlja{input};
    Outputs::Route testRoute = petlja.calculate();

    ASSERT_TRUE(testRoute.points.size() > 0);
    ASSERT_EQ(vecTrue.size(), testRoute.points.size());
    for (size_t i = 0; i < vecTrue.size(); ++i) {
        EXPECT_NEAR(vecTrue[i].getX(), testRoute.points[i].getX(), 0.001);
        EXPECT_NEAR(vecTrue[i].getY(), testRoute.points[i].getY(), 0.001);
    }
}

TEST_F(PetljaTestFixture, Test5)
{
    input.Tbek_0 = {4, 1};
    input.Ut = 10;
    vecTrue =     {Point2D{4.00000,	  1.00000},
                   Point2D{1.20000,	  0.00000},
                   Point2D{1.20000,	  -1.20000},
                   Point2D{0.00000,	  -1.20000},
                   Point2D{-1.20000,  -1.20000},
                   Point2D{-1.20000,  -0.78991},
                   Point2D{-2.60000,  -2.60000},
                   Point2D{-13.3472,  -2.60000},
                   Point2D{-16.6805,  -2.60000},
                   Point2D{5.0,       5.0}};

    PetljaK petlja{input};
    Outputs::Route testRoute = petlja.calculate();

    ASSERT_TRUE(testRoute.points.size() > 0);
    ASSERT_EQ(vecTrue.size(), testRoute.points.size());
    for (size_t i = 0; i < vecTrue.size(); ++i) {
        EXPECT_NEAR(vecTrue[i].getX(), testRoute.points[i].getX(), 0.001);
        EXPECT_NEAR(vecTrue[i].getY(), testRoute.points[i].getY(), 0.001);
    }
}

TEST_F(PetljaTestFixture, Test6)
{
    input.Tbek_0 = {15, 1};
    input.Ut = 10;
    vecTrue = {};

    PetljaK petlja{input};

    ASSERT_THROW(petlja.calculate(), std::runtime_error);
}

//По часовой
class PetljaTestFixture2 : public testing::Test
{
protected:
    PetljaInput input;
    VecPoints vecTrue;

    void SetUp() override
    {
        input.Tpk       = Point2D{ 0.0, 0.0 }; // Tpk
        input.Tbek_0    = Point2D{2.45023, -0.0112904}; // Tbek_0
        input.Tbek_exit = Point2D{ 50., 20. }; // Tbek_exit
        input.Dpgas     = 4.0; // Dpgas
        input.Dbgas     = 4.0; //  Dbgas
        input.Kt_deg    = 180;//90 - -90, /
        input.Kbek_deg  = 90 - 120.021; //90 - 150,
        input.Ut        = 7.57653; //   Ut
        input.Ukp       = 13.6959; //   Ukp
        input.Ukph      = 16; //        Ukp
        input.Ukp_r     = 12;//         Ukp

        vecTrue = { Point2D {2.45023,	  -0.01129},
                    Point2D {1.20000,	   0.00000},
                    Point2D {1.20000,	  -1.20000},
                    Point2D {0.00000,	  -1.20000},
                    Point2D {-1.20000,	  -1.20000},
                    Point2D {-1.20000,	   0.00000},
                    Point2D {-1.20000,	   1.20000},
                    Point2D {0.00000,      1.20000},
                    Point2D {1.12967,      1.20000},
                    Point2D {-2.60000,     2.60000},
                    Point2D {-11.12181,	   2.60000},
                    Point2D {-13.33460,	   2.60000},
                    Point2D { 50.00000,	   20.0000000},
                    };
    }
};
//По часовой
TEST_F(PetljaTestFixture2, Test1) {

    PetljaK petlja{input};
    Outputs::Route testRoute = petlja.calculate();

    ASSERT_TRUE(testRoute.points.size() > 0);
    ASSERT_EQ(vecTrue.size(), testRoute.points.size());
    for (size_t i = 0; i < vecTrue.size(); ++i) {
        EXPECT_NEAR(vecTrue[i].getX(), testRoute.points[i].getX(), 0.001);
        EXPECT_NEAR(vecTrue[i].getY(), testRoute.points[i].getY(), 0.001);
    }
}

TEST_F(PetljaTestFixture2, Test2) {

    input.Tbek_0 = {1.45, -1.2};
    vecTrue =     {Point2D{1.45,	   -1.2},
                   Point2D{1.20000,	   7.347880794884119e-17},
                   Point2D{1.20000,	   1.20000},
                   Point2D{0.00000,	   1.20000},
                   Point2D{-1.20000,   1.20000},
                   Point2D{-1.20000,   7.347880794884119e-17},
                   Point2D{-1.20000,   -1.20000},
                   Point2D{0,          -1.20000},
                   Point2D{1.15066,    -1.20000},
                   Point2D{-2.6,       -2.6},
                   Point2D{-11.1395,   -2.6},
                   Point2D{-13.3523,   -2.6},
                   Point2D{50.0,       20.0}};

    PetljaK petlja{input};
    Outputs::Route testRoute = petlja.calculate();

    ASSERT_TRUE(testRoute.points.size() > 0);
    ASSERT_EQ(vecTrue.size(), testRoute.points.size());
    for (size_t i = 0; i < vecTrue.size(); ++i) {
        EXPECT_NEAR(vecTrue[i].getX(), testRoute.points[i].getX(), 0.001);
        EXPECT_NEAR(vecTrue[i].getY(), testRoute.points[i].getY(), 0.001);
    }
}

TEST_F(PetljaTestFixture2, Test3) {

    input.Tbek_0 = {5.4, -1.2};
    vecTrue =     {Point2D{5.4,	      -1.2},
                   Point2D{1.20000,	  0.0},
                   Point2D{1.20000,	  -1.20000},
                   Point2D{0.00000,	  -1.20000},
                   Point2D{-1.20000,  -1.20000},
                   Point2D{-1.20000,  7.347880794884119e-17},
                   Point2D{-1.20000,  0.8608702067246699},
                   Point2D{-2.6,      2.6},
                   Point2D{-9.5467,	  2.6},
                   Point2D{-11.7595,  2.6},
                   Point2D{50.0,      20.0}};

    PetljaK petlja{input};
    Outputs::Route testRoute = petlja.calculate();

    ASSERT_TRUE(testRoute.points.size() > 0);
    ASSERT_EQ(vecTrue.size(), testRoute.points.size());
    for (size_t i = 0; i < vecTrue.size(); ++i) {
        EXPECT_NEAR(vecTrue[i].getX(), testRoute.points[i].getX(), 0.001);
        EXPECT_NEAR(vecTrue[i].getY(), testRoute.points[i].getY(), 0.001);
    }
}

TEST_F(PetljaTestFixture2, Test4) {

    input.Tbek_0 = {2, -2};
    vecTrue =     {Point2D{2.0,	      -2.0},
                   Point2D{1.20000,	  7.347880794884119e-17},
                   Point2D{1.20000,	  1.20000},
                   Point2D{0.00000,	  1.20000},
                   Point2D{-1.20000,  1.20000},
                   Point2D{-1.20000,  7.347880794884119e-17},
                   Point2D{-1.2,	  -1.2},
                   Point2D{0,         -1.2},
                   Point2D{0.35604,   -1.2},
                   Point2D{-2.6,      -2.6},
                   Point2D{-10.4805,  -2.6},
                   Point2D{-12.6933,  -2.6},
                   Point2D{50.0,      20.0}};

    PetljaK petlja{input};
    Outputs::Route testRoute = petlja.calculate();

    ASSERT_TRUE(testRoute.points.size() > 0);
    ASSERT_EQ(vecTrue.size(), testRoute.points.size());
    for (size_t i = 0; i < vecTrue.size(); ++i) {
        EXPECT_NEAR(vecTrue[i].getX(), testRoute.points[i].getX(), 0.001);
        EXPECT_NEAR(vecTrue[i].getY(), testRoute.points[i].getY(), 0.001);
    }
}
