#include <Task/Exceptions/AlgorithmException.hpp>
#include <Task/Exceptions/ValidationException.hpp>
#include <Task/Schemes/Search/Config.hpp>
#include <Task/Schemes/Search/InRegion/_Zigzag.hpp>

#include <gtest/gtest.h>

using Input = Schemes::Search::InRegion::Input;
using Zigzag = Schemes::Search::InRegion::_Zigzag;
/*
class _ZigzagTests : public ::testing::Test
{
protected:
    Input input;

    void SetUp() override
    {
        input.region.outer() = {{0, 0}, {0, 40000}, {40000, 40000}, {40000, 8000}, {50000, 40000}, {50000, 0}, {0, 0}};
        input.region.inners() = {{{14000, 16000}, {14000, 24000}, {26000, 24000}, {26000, 16000}, {14000, 16000}}};
        input.region.setEntrance({0, 12000});
        input.region.setExit({50000, 12000});
        input.searchTime = 180000.;
        input.ship.detectionRange = 6000.;
        input.ship.maxVelocity = 9;
        input.ship.searchVelocity = 6.;
    }

    void TearDown() override
    {
        // Код для очистки после каждого теста
    }
};

TEST_F(_ZigzagTests, checkTime)
{
    Zigzag zigzag{input};
    Outputs::Route route;
    try
    {
        route = zigzag.calculate();
    }
    catch (std::runtime_error& ex)
    {
        std::cout << 1;
    }

    EXPECT_NO_THROW(route = zigzag.calculate());
    EXPECT_TRUE(route.checkTime(input.searchTime, Schemes::Search::TIME_ACCURRACY));
}

TEST_F(_ZigzagTests, squarePolygon)
{
    Polygon2D polygon = {{{0, 0}, {0, 40000}, {40000, 40000}, {40000, 0}, {0, 0}}};

    input.region.setFromPolygon(polygon);
    input.region.setEntrance({0, 20000});
    input.region.setExit({40000, 20000});

    Zigzag zigzag{input};
    Outputs::Route route;
    EXPECT_NO_THROW(route = zigzag.calculate());

    EXPECT_TRUE(route.checkTime(input.searchTime, ACCURACY_DOUBLE * input.searchTime));
}

TEST_F(_ZigzagTests, calculate)
{
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

TEST_F(_ZigzagTests, calculate_PolygonVariation_noException)
{
    Polygon2D polygon = {{{0, 0}, {50000, -100}, {50100, 39900}, {40100, 7900}, {40100, 40000}, {0, 40100}, {0, 0}}, {{14000, 16000}, {14000, 24000}, {26000, 24000}, {26000, 16000}, {14000, 16000}}};
    input.region.setFromPolygon(polygon);
    input.region.setEntrance({24000, -48});
    input.region.setExit({50029, 11900});

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

TEST_F(_ZigzagTests, calculate_DoorException)
{
    // TODO: Точка не лежит на периметре?
    EXPECT_NO_THROW(input.region.setEntrance({1000, 1000}));

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

TEST_F(_ZigzagTests, calculate_ShortTimeException)
{
    input.searchTime = 100;

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


/// @todo раскомментировать после перехода к метрическим единицам
TEST_F(_ZigzagTests, calculate_metricInputWithoutConversion)
{
    Input inputNoConversion;
    Polygon2D poly = {{{-56005.3361985711, -55214.3937600496}, {-55160.5435120901, 55227.0013002307}, {54340.8061712153, 55221.6656113184}, {55172.9604263196, -55220.6846885126}, {-56005.3361985711, -55214.3937600496}}, {{-28211.5081638632, -27791.11322013737}, {27377.18039422075, -27794.758885906}, {27170.3353688776, 27479.9499885496}, {-27998.60753279395, 27482.3564027351}, {-28211.5081638632, -27791.11322013737}}};
    inputNoConversion.region.setFromPolygon(poly);
    inputNoConversion.region.setEntrance({-56005.3361985711, -55214.3937600496});
    inputNoConversion.region.setExit({54340.8061712153, 55221.6656113184});

    inputNoConversion.ship.searchVelocity = 6;
    inputNoConversion.ship.maxVelocity = 9;
    inputNoConversion.ship.detectionRange = 6000;
    inputNoConversion.searchTime = 180000;

    Zigzag zigzag{inputNoConversion};

    zigzag.calculate();
}
*/
