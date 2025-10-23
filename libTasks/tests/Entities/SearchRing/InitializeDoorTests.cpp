#include <Task/Entities/SearchRing.hpp>

#include <chrono>
#include <iostream>
#include <random>
//! gtest
#include <gtest/gtest.h>

namespace Entities {
using SearchRing = Entities::SearchRing;

class SearchRingInitializeDoorTests : public ::testing::Test
{
protected:
    SearchRing ring1;
    SearchRing ring2;

    const double PROJECTION_SHIFT = 0.001;

    void SetUp() override
    {
        ring1 = SearchRing{{-10., -10.}, {-10., 10}, {10., 10.}, {10., -10.}, {-10., -10.}};

        ring2 = SearchRing{{0, 0}, {4, 0}, {4, 4}, {2, 4}, {4, 5}, {0, 5}, {0, 0}};
    }

    void
    TearDown() override
    {
        // Код для очистки после каждого теста
    }
};

TEST_F(SearchRingInitializeDoorTests, initializeDoorPoint1)
{
    // {-10, 0}, {0, -10}
    size_t idx1 = 1, idx2 = 5;
    auto point1 = Point2D{-10, 0}, point2 = Point2D{0, -10};
    ring1.initializeDoorPoint(point1, idx1);
    EXPECT_EQ(ring1[idx1], point1);

    ring1.initializeDoorPoint(point2, idx2);
    EXPECT_EQ(ring1[idx2], point2);
}

TEST_F(SearchRingInitializeDoorTests, initializeDoorPoint_InverseOrder)
{
    // {0, -10}, {-10, 0}
    size_t idx1 = 4, idx2 = 1;
    auto point1 = Point2D{0, -10}, point2 = Point2D{-10, 0};
    ring1.initializeDoorPoint(point1, idx1);
    EXPECT_EQ(ring1[idx1], point1);

    ring1.initializeDoorPoint(point2, idx2);
    EXPECT_EQ(ring1[idx2], point2);
}

TEST_F(SearchRingInitializeDoorTests, initializeDoorPoint_SameEdge)
{
    // {-10, -1}, {-10, 1}
    size_t idx1 = 1, idx2 = 2;
    auto point1 = Point2D{-10, -1}, point2 = Point2D{-10, 1};
    ring1.initializeDoorPoint(point1, idx1);
    EXPECT_EQ(ring1[idx1], point1);

    ring1.initializeDoorPoint(point2, idx2);
    EXPECT_EQ(ring1[idx2], point2);
}

TEST_F(SearchRingInitializeDoorTests, initializeDoorPoint_SameEdgeInverseOrder)
{
    //{-10, 1}, {-10, -1}
    size_t idx1 = 1, idx2 = 1;
    auto point1 = Point2D{-10, 1}, point2 = Point2D{-10, -1};
    ring1.initializeDoorPoint(point1, idx1);
    EXPECT_EQ(ring1[idx1], point1);

    ring1.initializeDoorPoint(point2, idx2);
    EXPECT_EQ(ring1[idx2], point2);
}

TEST_F(SearchRingInitializeDoorTests, initializeDoorPoint_SameEdgeValidity)
{
    // {4, 0}, {4, 4}, {0, 0}, {0, 5}
    size_t idx1 = 5, idx2 = 4, idx3 = 0, idx4 = 1;
    auto point1 = Point2D{4, 0}, point2 = Point2D{4, 4}, point3 = Point2D{0, 0}, point4 = Point2D{0, 5};
    ring2.correct(false); // считаем внешнее кольцо

    ring2.initializeDoorPoint(point1, idx1);
    EXPECT_EQ(ring2[idx1], point1);

    ring2.initializeDoorPoint(point2, idx2);
    EXPECT_EQ(ring2[idx2], point2);

    ring2.initializeDoorPoint(point3, idx3);
    EXPECT_EQ(ring2[idx3], point3);

    ring2.initializeDoorPoint(point4, idx4);
    EXPECT_EQ(ring2[idx4], point4);
}

TEST_F(SearchRingInitializeDoorTests, initializeDoorPoint_EdgesValidity)
{
    //{1, 0}, {1, 5}
    ring2.correct(false); // считаем внешнее кольцо

    size_t idx1 = 6, idx2 = 2;
    auto point1 = Point2D{1, 0}, point2 = Point2D{1, 5};
    ring2.initializeDoorPoint(point1, idx1);
    EXPECT_EQ(ring2[idx1], point1);

    ring2.initializeDoorPoint(point2, idx2);
    EXPECT_EQ(ring2[idx2], point2);
}

TEST_F(SearchRingInitializeDoorTests, initializeDoorPoint_VertexAndEdgeValidity)
{
    //{2, 0}, {2, 4}
    ring2.correct(false); // считаем внешнее кольцо

    size_t idx1 = 6, idx2 = 3;
    auto point1 = Point2D{2, 0}, point2 = Point2D{2, 4};
    ring2.initializeDoorPoint(point1, idx1);
    EXPECT_EQ(ring2[idx1], point1);

    ring2.initializeDoorPoint(point2, idx2);
    EXPECT_EQ(ring2[idx2], point2);
}

TEST_F(SearchRingInitializeDoorTests, initializeDoorPoint_VerticesValidity2)
{
    // {4, 0}, {4, 5}
    ring2.correct(false); // считаем внешнее кольцо

    size_t idx1 = 5, idx2 = 3;
    auto point1 = Point2D{4, 0}, point2 = Point2D{4, 5};
    ring2.initializeDoorPoint(point1, idx1);
    EXPECT_EQ(ring2[idx1], point1);

    ring2.initializeDoorPoint(point2, idx2);
    EXPECT_EQ(ring2[idx2], point2);
}


/*TEST_F(SearchRingInitializeDoorTests, initializeDoorPoint_invalidEntrance)
{
    // {2, 0}, {2, 2}
    ring2.correct(false); // считаем внешнее кольцо

    auto index = ring2.initializeDoorPoint({2, 0}, ring2.halfPerimeter() * PROJECTION_SHIFT);
    EXPECT_EQ(index, 6);

    index = ring2.initializeDoorPoint({2, 2}, ring2.halfPerimeter() * PROJECTION_SHIFT);
    EXPECT_EQ(index, 8);

    EXPECT_EQ(ring2.indexOf({2, 0}), 6);
    EXPECT_THROW(ring2.indexOf({2, 2}), std::runtime_error);
}

TEST_F(SearchRingInitializeDoorTests, initializeDoorPoint_invalidExit)
{
    // {3, 2}, {3, 4}
    ring2.correct(false); // считаем внешнее кольцо

    auto index = ring2.initializeDoorPoint({3, 2}, ring2.halfPerimeter() * PROJECTION_SHIFT);
    EXPECT_EQ(index, 7);

    index = ring2.initializeDoorPoint({3, 4}, ring2.halfPerimeter() * PROJECTION_SHIFT);
    EXPECT_EQ(index, 4);

    EXPECT_THROW(ring2.indexOf({3, 2}), std::runtime_error);
    EXPECT_EQ(ring2.indexOf({3, 4}), 4);
}*/

TEST_F(SearchRingInitializeDoorTests, initializeDoorPoint_VerticesValidity1)
{
    // {0, 0}, {4, 4}
    ring2.correct(false); // считаем внешнее кольцо

    size_t idx1 = 0, idx2 = 4, idx3 = 5, idx4 = 2;
    auto point1 = Point2D{0, 0}, point2 = Point2D{4, 4}, point3 = Point2D{4, 0}, point4 = Point2D{4, 5};

    ring2.initializeDoorPoint(point1, idx1);
    EXPECT_EQ(ring2[idx1], point1);

    ring2.initializeDoorPoint(point2, idx2);
    EXPECT_EQ(ring2[idx2], point2);

    ring2.initializeDoorPoint(point3, idx3);
    EXPECT_EQ(ring2[idx3], point3);

    ring2.initializeDoorPoint(point4, idx4);
    EXPECT_EQ(ring2[idx4], point4);
}

TEST_F(SearchRingInitializeDoorTests, lastPointFirstPoint)
{
    // {-10, 0}, {0, -10}
    size_t idx1 = 1, idx2 = 5;
    auto point1 = Point2D{0, -10}, point2 = Point2D{-10, 10};
    ring1.initializeDoorPoint(point1, idx1);
    ring1.initializeDoorPoint(point2, idx2);
    EXPECT_EQ(ring1[idx1], point1);
    EXPECT_EQ(ring1[idx2], point2);
}

} // namespace Entities
