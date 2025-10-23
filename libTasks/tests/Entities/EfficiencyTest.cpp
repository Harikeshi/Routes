#include <Task/Entities/Efficiency.hpp>
#include <gtest/gtest.h>

namespace Entities {

class EfficiencyTest : public EfficiencyIndicators
{
public:
    EfficiencyTest(const int& cl)
        : EfficiencyIndicators(cl){};
    // initializeMask
    std::pair<int, int> countMaskTest(const PrimaryEntities::Polygon<Point2D>& polygon, const double& minX, const double& minY, const int& sizeX, const int& sizeY)
    {
        int countTrue = 0, countFalse = 0;
        EfficiencyIndicators::calculateMask(polygon, minX, minY, sizeX, sizeY);
        auto mask = EfficiencyIndicators::getMask();
        for (size_t i = 0; i < mask.size(); ++i)
        {
            for (size_t j = 0; j < mask[i].size(); ++j)
            {
                if (mask[i][j] == true)
                {
                    ++countTrue;
                }
                else
                {
                    ++countFalse;
                }
            }
        }
        return std::pair(countTrue, countFalse);
    }

    // addDetectionTack
    std::vector<std::vector<double>> observationTackTest(const Point2D& polygonMinPoint, const Point2D& firstPoint, const Point2D& secondPoint, const double& searchVelocity, const double& detRange, const int& sizeX, const int& sizeY)
    {
        EfficiencyIndicators::observationDensity.resize(sizeY);
        for (int i = 0; i < sizeY; ++i)
        {
            observationDensity[i].resize(sizeX);
        }
        EfficiencyIndicators::addObservationTack(polygonMinPoint, firstPoint, secondPoint, searchVelocity, detRange);
        return EfficiencyIndicators::getObservationDensity();
    }

    void setObservationDensity(std::vector<std::vector<double>> values)
    {
        if (values.empty() || values.at(0).empty())
        {
            throw std::runtime_error("Пустая матрица значений!");
        }
        EfficiencyIndicators::initializeObservationDensity(values[0].size(), values.size());
        for (size_t i = 0; i < values.size(); ++i)
        {
            for (size_t j = 0; j < values[0].size(); ++j)
            {
                observationDensity[i][j] = values[i][j];
            }
        }
    }
};

// initializeMaskTest
namespace {
TEST(EfficiencyTest, squareMaskTest)
{
    Polygon2D polygon = {{{0, 0}, {0, 50}, {50, 50}, {50, 0}, {0, 0}}};
    double minX = 0, minY = 0;
    int sizeX = 6, sizeY = 6, cellLength = 10;

    EfficiencyTest e(cellLength);
    auto countTrueAndFalse = e.countMaskTest(polygon, minX, minY, sizeX, sizeY);

    EXPECT_EQ(countTrueAndFalse.first, 36);
    EXPECT_EQ(countTrueAndFalse.second, 0);
}

TEST(EfficiencyTest, squareWhithInnerMaskTest)
{
    Polygon2D polygon = {{{0, 0}, {0, 50}, {50, 50}, {50, 0}, {0, 0}}, {{10, 10}, {40, 10}, {40, 40}, {10, 40}, {10, 10}}};
    double minX = 0, minY = 0;
    int sizeX = 6, sizeY = 6, cellLength = 10;

    EfficiencyTest e(cellLength);
    auto countTrueAndFalse = e.countMaskTest(polygon, minX, minY, sizeX, sizeY);

    EXPECT_EQ(countTrueAndFalse.first, 32);
    EXPECT_EQ(countTrueAndFalse.second, 4);
}

TEST(EfficiencyTest, squareInSquareMaskTest)
{
    Polygon2D polygon = {{{0, 0}, {0, 50}, {50, 50}, {50, 0}, {0, 0}}};
    double minX = -10, minY = -10;
    int sizeX = 8, sizeY = 8, cellLength = 10;

    EfficiencyTest e(cellLength);
    auto countTrueAndFalse = e.countMaskTest(polygon, minX, minY, sizeX, sizeY);

    EXPECT_EQ(countTrueAndFalse.first, 36);
    EXPECT_EQ(countTrueAndFalse.second, 28);
}

TEST(EfficiencyTest, triangleMaskTest)
{
    Polygon2D polygon = {{{0, 0}, {50, 50}, {50, 0}, {0, 0}}};
    double minX = 0, minY = 0;
    int sizeX = 6, sizeY = 6, cellLength = 10;

    EfficiencyTest e(cellLength);
    auto countTrueAndFalse = e.countMaskTest(polygon, minX, minY, sizeX, sizeY);

    EXPECT_EQ(countTrueAndFalse.first, 21);
    EXPECT_EQ(countTrueAndFalse.second, 15);
}

TEST(EfficiencyTest, triangleInSquareMaskTest)
{
    Polygon2D polygon = {{{0, 0}, {50, 50}, {50, 0}, {0, 0}}};
    double minX = -10, minY = -10;
    int sizeX = 8, sizeY = 8, cellLength = 10;

    EfficiencyTest e(cellLength);
    auto countTrueAndFalse = e.countMaskTest(polygon, minX, minY, sizeX, sizeY);

    EXPECT_EQ(countTrueAndFalse.first, 21);
    EXPECT_EQ(countTrueAndFalse.second, 43);
}
} // namespace

/// @todo рефакторить тесты
/*
// addDetectionTack TEST
namespace {
TEST(EfficiencyTest, detTackTestHorizont)
{
    Outputs::Route route(std::vector<Point2D>{{3, 3}, {10, 3}}, std::vector<double>{6});
    double detRange = 3;
    int cellLength = 1, sizeX = 14, sizeY = 7;

    EfficiencyTest e(cellLength);
    auto result = e.detectionTackTest({0, 0}, route.points[0], route.points[1], route.velocities[0], detRange, sizeX, sizeY);

    std::vector<double> row0 = {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0};
    std::vector<double> row1 = {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0};
    std::vector<double> row2 = {0, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 0};
    std::vector<double> row3 = {1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1};
    std::vector<double> row4 = {0, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 0};
    std::vector<double> row5 = {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0};
    std::vector<double> row6 = {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0};

    EXPECT_EQ(row0, result[0]);
    EXPECT_EQ(row1, result[1]);
    EXPECT_EQ(row2, result[2]);
    EXPECT_EQ(row3, result[3]);
    EXPECT_EQ(row4, result[4]);
    EXPECT_EQ(row5, result[5]);
    EXPECT_EQ(row6, result[6]);
}

TEST(EfficiencyTest, detTackTestVertic)
{
    Outputs::Route route(std::vector<Point2D>{{3, 3}, {3, 10}}, std::vector<double>{6});
    double detRange = 3;
    int cellLength = 1, sizeX = 7, sizeY = 14;

    EfficiencyTest e(cellLength);
    auto result = e.detectionTackTest({0, 0}, route.points[0], route.points[1], route.velocities[0], detRange, sizeX, sizeY);

    std::vector<double> row0 = {0, 0, 0, 1, 0, 0, 0};
    std::vector<double> row1 = {0, 2, 2, 2, 2, 2, 0};
    std::vector<double> row2 = {0, 2, 3, 3, 3, 2, 0};
    std::vector<double> row3 = {1, 2, 3, 4, 3, 2, 1};
    std::vector<double> row4 = {1, 2, 3, 4, 3, 2, 1};
    std::vector<double> row5 = {1, 2, 3, 4, 3, 2, 1};
    std::vector<double> row6 = {1, 2, 3, 4, 3, 2, 1};
    std::vector<double> row7 = {1, 2, 3, 4, 3, 2, 1};
    std::vector<double> row8 = {1, 2, 3, 4, 3, 2, 1};
    std::vector<double> row9 = {1, 2, 3, 4, 3, 2, 1};
    std::vector<double> row10 = {1, 2, 3, 4, 3, 2, 1};
    std::vector<double> row11 = {0, 2, 3, 3, 3, 2, 0};
    std::vector<double> row12 = {0, 2, 2, 2, 2, 2, 0};
    std::vector<double> row13 = {0, 0, 0, 1, 0, 0, 0};

    EXPECT_EQ(row0, result[0]);
    EXPECT_EQ(row1, result[1]);
    EXPECT_EQ(row2, result[2]);
    EXPECT_EQ(row3, result[3]);
    EXPECT_EQ(row4, result[4]);
    EXPECT_EQ(row5, result[5]);
    EXPECT_EQ(row6, result[6]);
    EXPECT_EQ(row7, result[7]);
    EXPECT_EQ(row8, result[8]);
    EXPECT_EQ(row9, result[9]);
    EXPECT_EQ(row10, result[10]);
    EXPECT_EQ(row11, result[11]);
    EXPECT_EQ(row12, result[12]);
    EXPECT_EQ(row13, result[13]);
}

TEST(EfficiencyTest, detTackTestHorizontWithPoly)
{
    Outputs::Route route(std::vector<Point2D>{{3, 5}, {7, 5}}, std::vector<double>{6});
    double detRange = 1;
    int cellLength = 1, sizeX = 12, sizeY = 12;

    EfficiencyTest e(cellLength);
    auto result = e.detectionTackTest({0, 0}, route.points[0], route.points[1], route.velocities[0], detRange, sizeX, sizeY);

    std::vector<double> v1 = {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0};
    std::vector<double> v2 = {0, 0, 1, 2, 2, 2, 2, 2, 1, 0, 0, 0};

    EXPECT_EQ(result[4], v1);
    EXPECT_EQ(result[5], v2);
    EXPECT_EQ(result[6], v1);
}

} // namespace
*/

TEST(EfficiencyTest, timeStatistics)
{
    std::vector<std::vector<double>> values{
        {1, 1, 1, 1},
        {1, 1, 1, 1},
        {1, 1, 1, 1}};
    PrimaryEntities::Polygon<Point2D> poly = {{{0, 0}, {0, 4}, {3, 4}, {3, 0}, {0, 0}}};
    EfficiencyTest effTest(1);
    effTest.setObservationDensity(values);
    std::pair<double, double> timeStat = effTest.timeStatistics(poly, Outputs::Route{}, 0);
    EXPECT_EQ(timeStat.first, 1);
    EXPECT_EQ(timeStat.second, 0);
}

}; // namespace Entities
