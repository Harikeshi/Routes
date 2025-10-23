#include <Task/Exceptions/AlgorithmException.hpp>
#include <Task/Exceptions/ValidationException.hpp>
//#include <Task/Operations/UnitConverter.hpp>
#include <Task/Schemes/Search/Config.hpp>
#include <Task/Schemes/Search/InRegion/Zigzag.hpp>

#include <gtest/gtest.h>

using Input = Schemes::Search::InRegion::Input;
using Zigzag = Schemes::Search::InRegion::Zigzag;
//using namespace Operations;

class ZigzagTest : public ::testing::Test
{
protected:
    Input input;

    void SetUp() override
    {
        Polygon2D polygon = {{{0, 0}, {0, 40000}, {40000, 40000}, {40000, 8000}, {50000, 40000}, {50000, 0}, {0, 0}}, {{14000, 16000}, {14000, 24000}, {26000, 24000}, {26000, 16000}, {14000, 16000}}};
        //        input.region.setPolygon(polygon);
        //        input.region.setEntrance({0, 12000});
        //        input.region.setExit({50000, 12000});
        input.region = Entities::SearchRegion(polygon, {0, 12000}, {50000, 12000});
        input.searchTime = 180000.;
        input.ship.detectionRange = 6000.;
        input.ship.searchVelocity = 9;
        input.ship.searchVelocity = 6.;
    }

    void TearDown() override
    {
    }
};

TEST_F(ZigzagTest, checkTime)
{
    //    metricToNauticalInput(input);
    Zigzag zigzag{input};
    Outputs::Route route;
    try
    {
        route = zigzag.calculate();
    }
    catch (ExceptionTools::Failure& f)
    {
        std::cout << f.getCode() << '\t' << f.what() << std::endl;
        FAIL();
    }
    if (!route.checkTime(input.searchTime, Schemes::Search::TIME_ACCURRACY * 2))
    {
        std::cout << "Время по теореме косинусов ";
        std::cout << "Время пути: " << route.duration() << " Время отведенное на поиск " << input.searchTime << std::endl;
        std::cout << "Дельта: " << std::abs(route.duration() - input.searchTime) << " ";
    }
    else
    {
        std::cout << "Время по бинарному поиску ";
        std::cout << "Время пути: " << route.duration() << " Время отведенное на поиск " << input.searchTime << std::endl;
        std::cout << "Дельта: " << std::abs(route.duration() - input.searchTime) << " ";
    }
    //    EXPECT_TRUE(route.checkTime(input.searchTime, Schemes::Search::TIME_ACCURRACY));
    EXPECT_NEAR(route.duration(), input.searchTime, Schemes::Search::TIME_ACCURRACY);
}

TEST_F(ZigzagTest, squarePolygon)
{
    //    Polygon2D polygon = {{{0, 0}, {0, 40000}, {40000, 40000}, {40000, 0}, {0, 0}}};
    input.region = Entities::SearchRegion({{{0, 0}, {0, 40000}, {40000, 40000}, {40000, 0}, {0, 0}}}, Point2D(0, 20000), Point2D(40000, 20000));
    //    input.region.setEntrance({0, 20000});
    //    input.region.setExit({40000, 20000});

    //    Operations::metricToNauticalInput(input);
    Zigzag zigzag{input};
    Outputs::Route route;
    try
    {
        route = zigzag.calculate();
    }
    catch (ExceptionTools::Failure& f)
    {
        std::cout << f.getCode() << '\t' << f.what() << std::endl;
        FAIL();
    }
    EXPECT_TRUE(route.checkTime(input.searchTime, ACCURACY_DOUBLE * input.searchTime));
}

TEST_F(ZigzagTest, calculate)
{
    //    Operations::metricToNauticalInput(input);
    Zigzag zigzag{input};

    Outputs::Route route;
    try
    {
        route = zigzag.calculate();
    }
    catch (ExceptionTools::Failure& f)
    {
        std::cout << f.getCode() << '\t' << f.what() << std::endl;
        FAIL();
    }
}

TEST_F(ZigzagTest, calculate_PolygonVariation_noException)
{
    Polygon2D polygon = {{{0, 0}, {50000, -100}, {50100, 39900}, {40100, 7900}, {40100, 40000}, {0, 40100}, {0, 0}}, {{14000, 16000}, {14000, 24000}, {26000, 24000}, {26000, 16000}, {14000, 16000}}};
    //    input.region.setPolygon(polygon);
    //    input.region.setEntrance({24000, -48});
    //    input.region.setExit({50029, 11900});
    input.region = Entities::SearchRegion(polygon, {24000, -48}, {50029, 11900});

    //    Operations::metricToNauticalInput(input);
    Zigzag zigzag{input};

    Outputs::Route route;
    try
    {
        route = zigzag.calculate();
    }
    catch (ExceptionTools::Failure& f)
    {
        std::cout << f.getCode() << '\t' << f.what() << std::endl;
        FAIL();
    }
}

TEST_F(ZigzagTest, calculate_DoorException)
{
    EXPECT_NO_THROW(input.region.setEntrance({1000, 1000}));

    //    Operations::metricToNauticalInput(input);
    Zigzag zigzag{input};

    try
    {
        zigzag.calculate();
        FAIL() << "calculate() must throw an exception";
    }
    catch (Exceptions::AlgorithmFailure& ae)
    {
        EXPECT_EQ(ae.getCode(), 200);
    }
    catch (...)
    {
        FAIL() << "exception type shall be AlgorithmFailure";
    }
}

TEST_F(ZigzagTest, calculate_ShortTimeException)
{
    input.searchTime = 100;

    //    Operations::metricToNauticalInput(input);
    Zigzag zigzag{input};

    try
    {
        zigzag.calculate();
        FAIL() << "calculate() must throw an exception";
    }
    catch (Exceptions::AlgorithmFailure& ae)
    {
        EXPECT_EQ(ae.getCode(), 210);
    }
    catch (...)
    {
        FAIL() << "exception type shall be AlgorithmFailure";
    }
}

/* не актуален для новой функции определения угла
TEST_F(ZigzagTest, calculate_EntranceCourseException)
{
    input.region.setEntrance({50000, 40000 - 90000 * Schemes::Search::PROJECTION_SHIFT * 2});

    //    Operations::metricToNauticalInput(input);
    Zigzag zigzag{input};

    try
    {
        zigzag.calculate();
        FAIL() << "calculate() must throw an exception";
    }
    catch (Exceptions::AlgorithmFailure& ae)
    {
        //        std::cout << ae.what() << std::endl;
        EXPECT_EQ(ae.getCode(), 211);
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        FAIL() << "exception type shall be AlgorithmFailure";
    }
    catch (...)
    {
        FAIL() << "exception type is unknown";
    }
}
*/

/// @todo раскомментировать после перехода к метрическим единицам
TEST_F(ZigzagTest, calculate_metricInputWithoutConversion)
{
    Input inputNoConversion;
    Polygon2D poly = {{{-56005.3361985711, -55214.3937600496}, {-55160.5435120901, 55227.0013002307}, {54340.8061712153, 55221.6656113184}, {55172.9604263196, -55220.6846885126}, {-56005.3361985711, -55214.3937600496}}, {{-28211.5081638632, -27791.11322013737}, {27377.18039422075, -27794.758885906}, {27170.3353688776, 27479.9499885496}, {-27998.60753279395, 27482.3564027351}, {-28211.5081638632, -27791.11322013737}}};
    //    inputNoConversion.region.setPolygon(poly);
    //    inputNoConversion.region.setEntrance({-56005.3361985711, -55214.3937600496});
    //    inputNoConversion.region.setExit({54340.8061712153, 55221.6656113184});
    inputNoConversion.region = Entities::SearchRegion(poly, {-56005.3361985711, -55214.3937600496}, {54340.8061712153, 55221.6656113184});

    inputNoConversion.ship.searchVelocity = 6;
    inputNoConversion.ship.maxVelocity = 9;
    inputNoConversion.ship.detectionRange = 6000;
    inputNoConversion.searchTime = 180000;

    Zigzag zigzag{inputNoConversion};
    Outputs::Route route;
    try
    {
        route = zigzag.calculate();
    }
    catch (ExceptionTools::Failure& f)
    {
        std::cout << f.getCode() << '\t' << f.what() << std::endl;
        FAIL();
    }
}
