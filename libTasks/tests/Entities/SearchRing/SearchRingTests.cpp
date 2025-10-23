#include <Task/Entities/SearchRing.hpp>

#include <chrono>
#include <random>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Entities {
using SearchRing = Entities::SearchRing;

static SearchRing generateRandomRing(size_t n, double min = 0, double max = 100)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);

    SearchRing ring;
    for (size_t i = 0; i < n; ++i)
    {
        ring.emplace_back(dis(gen), dis(gen));
    }
    return ring;
}
//==============================================================================
TEST(SearchRingTest, getOBBGEOSTest)
{
    SearchRing ring{{0., 0.}, {0., 2.}, {2., 0.}};
    ring.getOBB();
}
//==============================================================================
void print(const SearchRing& ring)
{
    for (const auto& point : ring)
    {
        std::cout << "(" << point.getCoordinates() << "):";
    }
    std::cout << "\n";
}
//==============================================================================
TEST(SearchRingTest, getRingWithConvexHullGEOSTest)
{
    double value = 11829812912891280;
    //------------------------------------------------------------------------------------------------------------------
    // 1. Крайние случаи (минимум точек, коллиниарность)
    //------------------------------------------------------------------------------------------------------------------
    //SearchRing ring;
    //{{0., 0.}, {0., 0.}, {0., 0.}};

    //auto hull = ring.getRingWithConvexHullGEOS();
    //Ring<std::pair<Point2D, bool>> result = {{{0., 0.}, true}};

    //ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    //ring = SearchRing{{0., 0.}, {1. * value, 1. * value}, {0.,0.}};

    //auto hull = ring.getRingWithConvexHullGEOS();

    //Ring<std::pair<Point2D, bool>> result = {{{0., 0.}, true}, {{1. * value, 1. * value}, true}};
    //ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    /* SearchRing ring = SearchRing{{0., 0.}, {1. * value, 1. * value}, {2. * value, 2. * value}, {3. * value, 3. * value}, {0., 0.}};

    auto hull = ring.getRingWithConvexHullGEOS();

    Ring<std::pair<Point2D, bool>>  result = {{{0., 0.}, true}, {{1. * value, 1. * value}, false}, {{2. * value, 2. * value}, false}, {{3. * value, 3. * value}, true}};
    ASSERT_EQ(hull, result);*/
    //------------------------------------------------------------------------------------------------------------------
    /*   SearchRing ring = SearchRing{{0., 0.}, {1. * value, 0.}, {2. * value, 0.}, {3. * value, 0.}};

    auto hull = ring.getRingWithConvexHullGEOS();

    Ring<std::pair<Point2D, bool>>  result = {{{0., 0.}, true}, {{1. * value, 0.}, false}, {{2. * value, 0.}, false}, {{3. * value, 0.}, true}};
    ASSERT_EQ(hull, result);*/
    //------------------------------------------------------------------------------------------------------------------
    // 2. Общие случаи
    //------------------------------------------------------------------------------------------------------------------
    SearchRing ring = SearchRing{{0., 0.}, {0, 1. * value}, {1. * value, 1. * value}, {1. * value, 0.}}; // a

    auto hull = ring.getRingWithConvexHullGEOS();

    PrimaryEntities::Ring<std::pair<Point2D, bool>> result = {{{0., 0.}, true}, {{0., 1. * value}, true}, {{1. * value, 1. * value}, true}, {{1. * value, 0.}, true}, {{0., 0.}, true}};
    ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {0, 1. * value}, {1. * value, 0.}}; // b

    hull = ring.getRingWithConvexHullGEOS();

    result = {{{0., 0.}, true}, {{0., 1. * value}, true}, {{1. * value, 0.}, true}, {{0., 0.}, true}};
    ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {0, 2. * value}, {1. * value, 1. * value}, {2. * value, 2. * value}, {2. * value, 0.}}; // c

    hull = ring.getRingWithConvexHullGEOS();

    result = {{{0., 0.}, true}, {{0., 2. * value}, true}, {{1. * value, 1. * value}, false}, {{2. * value, 2. * value}, true}, {{2. * value, 0.}, true}, {{0., 0.}, true}};
    ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {0, 2. * value}, {1. * value, 1. * value}, {1.5 * value, 1.2 * value}, {2. * value, 2. * value}, {2. * value, 0.}}; // d

    hull = ring.getRingWithConvexHullGEOS();

    result = {{{0., 0.}, true}, {{0., 2. * value}, true}, {{1. * value, 1. * value}, false}, {{1.5 * value, 1.2 * value}, false}, {{2. * value, 2. * value}, true}, {{2. * value, 0.}, true}, {{0., 0.}, true}};
    ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {1.5 * value, 2.2 * value}, {0., 4. * value}, {2. * value, 4. * value}, {3. * value, 6. * value}, {4. * value, 4. * value}, {6. * value, 4. * value}, {4.5 * value, 2.2 * value}, {6. * value, 0.}, {3. * value, 1.5 * value}}; // f (+/- звезда)

    hull = ring.getRingWithConvexHullGEOS();
    result = {{{0., 0.}, true}, {{1.5 * value, 2.2 * value}, false}, {{0., 4. * value}, true}, {{2. * value, 4. * value}, false}, {{3. * value, 6. * value}, true}, {{4. * value, 4. * value}, false}, {{6. * value, 4. * value}, true}, {{4.5 * value, 2.2 * value}, false}, {{6. * value, 0.}, true}, {{3. * value, 1.5 * value}, false}, {{0., 0.}, true}};
    ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {0., 2. * value}, {1. * value, 1. * value}, {2. * value, 1.5 * value}, {3. * value, 1. * value}, {4. * value, 1.5 * value}, {5. * value, 1. * value}, {6. * value, 2. * value}, {6. * value, 0.}}; // g

    hull = ring.getRingWithConvexHullGEOS();

    result = {{{0., 0.}, true}, {{0., 2. * value}, true}, {{1. * value, 1. * value}, false}, {{2. * value, 1.5 * value}, false}, {{3. * value, 1. * value}, false}, {{4. * value, 1.5 * value}, false}, {{5. * value, 1. * value}, false}, {{6. * value, 2. * value}, true}, {{6. * value, 0.}, true}, {{0., 0.}, true}}; // g
    ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    //ring = SearchRing{{0., 0.}, {1. * value, 1. * value}, {1. * value, -1. * value}, {2. * value, 2. * value}, {2. * value, -2. * value}}; // h

    //hull = ring.getRingWithConvexHullGEOS();

    //result = {{{0., 0.}, true}, {{1. * value, 1. * value}, false}, {{1. * value, -1. * value}, false}, {{2. * value, 2. * value}, true}, {{2. * value, -2. * value}, true}, {{0., 0.}, true}}; // h
    //ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    // 3. Специальные случаи (дубликаты точек, точки на прямой)
    //------------------------------------------------------------------------------------------------------------------
    // TODO: Убрать повторяющиеся точки
    ring = SearchRing{{0., 0.}, {0., 0.}, {0., 1. * value}, {1. * value, 1. * value}, {1. * value, 0.}};

    hull = ring.getRingWithConvexHullGEOS();

    result = {{{0., 0.}, true}, {{0., 1. * value}, true}, {{1. * value, 1. * value}, true}, {{1. * value, 0.}, true}, {{0., 0.}, true}};
    ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {1. * value, 1. * value}, {2. * value, 2. * value}, {2. * value, 0.}};

    hull = ring.getRingWithConvexHullGEOS();

    result = {{{0., 0.}, true}, {{1. * value, 1. * value}, false}, {{2. * value, 2. * value}, true}, {{2. * value, 0.}, true}, {{0., 0.}, true}};
    ASSERT_EQ(hull, result);
}
//==============================================================================
TEST(SearchRingTest, getConvexHullGEOSTest)
{
    double value = 11829812912891280;

    // TODO: Закомментированы случае не поддерживаемые ring
    //------------------------------------------------------------------------------------------------------------------
    // 1. Крайние случаи (минимум точек, коллиниарность)
    //------------------------------------------------------------------------------------------------------------------
    //SearchRing ring{{0., 0.}};

    //auto hull = ring.getConvexHullGEOS();

    //SearchRing result = SearchRing{{0., 0.}};
    //ASSERT_EQ(hull, result);
    ////------------------------------------------------------------------------------------------------------------------
    //ring = SearchRing{{0., 0.}, {1. * value, 1. * value}};

    //hull = ring.getConvexHullGEOS();

    //result = SearchRing{{0., 0.}, {1. * value, 1. * value}};
    //ASSERT_EQ(hull, result);
    ////------------------------------------------------------------------------------------------------------------------
    //ring = SearchRing{{0., 0.}, {1. * value, 1. * value}, {2. * value, 2. * value}, {3. * value, 3. * value}};

    //hull = ring.getConvexHullGEOS();

    //result = SearchRing{{0., 0.}, {3. * value, 3. * value}};
    //ASSERT_EQ(hull, result);
    ////------------------------------------------------------------------------------------------------------------------
    //ring = SearchRing{{0., 0.}, {1. * value, 0.}, {2. * value, 0.}, {3. * value, 0.}};

    //hull = ring.getConvexHullGEOS();

    //result = SearchRing{{0., 0.}, {3. * value, 0.}};
    //ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    // 2. Общие случаи
    //------------------------------------------------------------------------------------------------------------------
    SearchRing ring = SearchRing{{0., 0.}, {0, 1. * value}, {1. * value, 1. * value}, {1. * value, 0.}}; // a

    SearchRing hull = ring.getConvexHullGEOS();

    SearchRing result = SearchRing{{0., 0.}, {0., 1. * value}, {1. * value, 1. * value}, {1. * value, 0.}, {0., 0.}};
    ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {0, 1. * value}, {1. * value, 0.}}; // b

    hull = ring.getConvexHullGEOS();

    result = SearchRing{{0., 0.}, {0., 1. * value}, {1. * value, 0.}, {0., 0.}};
    ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {0, 2. * value}, {1. * value, 1. * value}, {2. * value, 2. * value}, {2. * value, 0.}}; // c

    hull = ring.getConvexHullGEOS();

    result = SearchRing{{0., 0.}, {0., 2. * value}, {2. * value, 2. * value}, {2. * value, 0.}, {0., 0.}};
    ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {0, 2. * value}, {1. * value, 1. * value}, {1.5 * value, 1.2 * value}, {2. * value, 2. * value}, {2. * value, 0.}}; // d

    hull = ring.getConvexHullGEOS();

    result = SearchRing{{0., 0.}, {0., 2. * value}, {2. * value, 2. * value}, {2. * value, 0.}, {0., 0.}};
    ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    // Звезда
    ring = SearchRing{{0., 0.}, {1.5 * value, 2.2 * value}, {0., 4. * value}, {2. * value, 4. * value}, {3. * value, 6. * value}, {4. * value, 4. * value}, {6. * value, 4. * value}, {4.5 * value, 2.2 * value}, {6. * value, 0.}, {3. * value, 1.5 * value}}; // f (+/- звезда)

    hull = ring.getConvexHullGEOS();

    result = SearchRing{{0., 0.}, {0., 4. * value}, {3. * value, 6. * value}, {6. * value, 4. * value}, {6. * value, 0.}, {0., 0.}};
    ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {0, 2. * value}, {1. * value, 1. * value}, {2. * value, 1.5 * value}, {3. * value, 1. * value}, {4. * value, 1.5 * value}, {5. * value, 1. * value}, {6. * value, 2. * value}, {6. * value, 0.}}; // g
    hull = ring.getConvexHullGEOS();

    result = SearchRing{{0., 0.}, {0., 2. * value}, {6. * value, 2. * value}, {6. * value, 0.}, {0., 0.}};
    ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    //ring = SearchRing{{0., 0.}, {1. * value, 1. * value}, {1. * value, -1. * value}, {2. * value, 2. * value}, {2. * value, -2. * value}}; // h

    //hull = ring.getConvexHullGEOS();

    //result = SearchRing{{2. * value, -2. * value}, {0., 0.}, {2. * value, 2. * value}};
    //ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    // 3. Специальные случаи (дубликаты точек, точки на прямой)
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {0., 0.}, {0., 1. * value}, {1. * value, 1. * value}, {1. * value, 0.}};

    hull = ring.getConvexHullGEOS();

    result = SearchRing{{0., 0.}, {0., 1. * value}, {1. * value, 1. * value}, {1. * value, 0.}, {0., 0.}};
    ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {1. * value, 1. * value}, {2. * value, 2. * value}, {2. * value, 0.}};

    hull = ring.getConvexHullGEOS();

    result = SearchRing{{0., 0.}, {2. * value, 2. * value}, {2. * value, 0.}, {0., 0.}};
    ASSERT_EQ(hull, result);
    //------------------------------------------------------------------------------------------------------------------
    /* ring = SearchRing{{0., 0.}, {0., 0.}, {0., 0.}};

    hull = ring.getConvexHullGEOS();

    result = SearchRing{{0., 0.}};
    ASSERT_EQ(hull, result);*/
    //------------------------------------------------------------------------------------------------------------------
    //// 1000000 точек
    //ring = generateRandomRing(1000000, -1000, 1000);
    //// TODO: разница выигрыша собственной реализацией x2.5 от geos
    //auto start = std::chrono::high_resolution_clock::now();
    //hull = ring.getConvexHullGEOS();
    //auto end = std::chrono::high_resolution_clock::now();

    //auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //EXPECT_LT(duration.count(), 5000);
}

TEST(SearchRingTest, getOrientedBoundingBoxGEOSTest)
{
    //------------------------------------------------------------------------------------------------------------------
    // 1. Крайние случаи (минимум точек, коллиниарность)
    //------------------------------------------------------------------------------------------------------------------
    //SearchRing ring{{0., 0.}};

    //auto box = ring.getOrientedBoundingBoxGEOS();
    //SearchRing result = SearchRing{{0., 0.}};

    //ASSERT_EQ(box, result);
    ////------------------------------------------------------------------------------------------------------------------
    //ring = SearchRing{{0., 0.}, {1., 1.}};

    //box = ring.getOrientedBoundingBoxGEOS();

    //result = SearchRing{{0., 0.}, {1., 1.}};
    //ASSERT_EQ(box, result);
    ////------------------------------------------------------------------------------------------------------------------
    //ring = SearchRing{{0., 0.}, {1., 1.}, {2., 2.}, {3., 3.}};

    //box = ring.getOrientedBoundingBoxGEOS();

    //result = SearchRing{{0., 0.}, {3., 3.}};
    //ASSERT_EQ(box, result);
    ////------------------------------------------------------------------------------------------------------------------
    //ring = SearchRing{{0., 0.}, {1., 0.}, {2., 0.}, {3., 0.}};

    //box = ring.getOrientedBoundingBoxGEOS();

    //result = SearchRing{{0., 0.}, {3., 0.}};
    //ASSERT_EQ(box, result);
    //------------------------------------------------------------------------------------------------------------------
    // 2. Общие случаи
    //------------------------------------------------------------------------------------------------------------------
    SearchRing ring = SearchRing{{0., 0.}, {0, 1.}, {1., 1.}, {1., 0.}}; // a

    auto box = ring.getOrientedBoundingBoxGEOS();

    SearchRing result = SearchRing{{0., 0.}, {0, 1.}, {1., 1.}, {1., 0.}, {0., 0.}};
    ASSERT_EQ(box, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {0, 1.}, {1., 0.}}; // b

    box = ring.getOrientedBoundingBoxGEOS();

    result = SearchRing{{0., 1}, {1., 0.}, {.5, -.5}, {-.5, .5}, {0., 1.}};
    ASSERT_EQ(box, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {0, 2.}, {1., 1.}, {2., 2.}, {2., 0.}}; // c

    box = ring.getOrientedBoundingBoxGEOS();

    result = SearchRing{{0., 0.}, {0., 2.}, {2., 2.}, {2., 0.}, {0., 0.}};
    ASSERT_EQ(box, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {0, 2.}, {1., 1.}, {1.5, 1.2}, {2., 2.}, {2., 0.}}; // d

    box = ring.getOrientedBoundingBoxGEOS();

    result = SearchRing{{0., 0.}, {0., 2.}, {2., 2.}, {2., 0.}, {0., 0.}};
    ASSERT_EQ(box, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {1.5, 2.2}, {0., 4.}, {2., 4.}, {3., 6.}, {4., 4.}, {6., 4.}, {4.5, 2.2}, {6., 0.}, {3., 1.5}}; // f (+/- звезда)

    box = ring.getOrientedBoundingBoxGEOS();

    result = SearchRing{{0., 0.}, {0., 6.}, {6., 6.}, {6., 0.}, {0., 0.}};
    ASSERT_EQ(box, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {0, 2.}, {1., 1.}, {2., 1.5}, {3., 1.}, {4., 1.5}, {5., 1.}, {6., 2.}, {6., 0.}}; // g

    box = ring.getOrientedBoundingBoxGEOS();

    result = SearchRing{{0., 2.}, {6., 2.}, {6., 0.}, {0., 0.}, {0., 2.}};
    ASSERT_EQ(box, result);
    //------------------------------------------------------------------------------------------------------------------
    //ring = SearchRing{{0., 0.}, {1., 1.}, {1., -1.}, {2., 2.}, {2., -2.}}; // h

    //box = ring.getOrientedBoundingBoxGEOS();

    //result = SearchRing{{2., 2.}, {0., 2.}, {0., -2.}, {2., -2.}, {2., 2.}};
    //ASSERT_EQ(box, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {2., 2.}, {3., 1.}, {1., -1.}}; // +

    box = ring.getOrientedBoundingBoxGEOS();

    result = SearchRing{{0., 0.}, {2., 2.}, {3., 1.}, {1., -1.}, {0., 0.}};
    ASSERT_EQ(box, result);
    //------------------------------------------------------------------------------------------------------------------
    // 3. Специальные случаи (дубликаты точек, точки на прямой)
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {0., 0.}, {0., 1.}, {1., 1.}, {1., 0.}};

    box = ring.getOrientedBoundingBoxGEOS();

    result = SearchRing{{0., 0.}, {0., 1.}, {1., 1.}, {1., 0.}, {0., 0.}};
    ASSERT_EQ(box, result);
    //------------------------------------------------------------------------------------------------------------------
    ring = SearchRing{{0., 0.}, {1., 1.}, {2., 2.}, {2., 0.}};

    box = ring.getOrientedBoundingBoxGEOS();

    result = SearchRing{{0., 0.}, {2., 2.}, {3., 1.}, {1., -1.}, {0., 0.}};
    ASSERT_EQ(box, result);
    //------------------------------------------------------------------------------------------------------------------
    /* ring = SearchRing{{0., 0.}, {0., 0.}, {0., 0.}};

    box = ring.getOrientedBoundingBoxGEOS();

    result = SearchRing{{0., 0.}};
    ASSERT_EQ(box, result);*/
    //------------------------------------------------------------------------------------------------------------------
    //// 1000000 точек
    //ring = generateRandomRing(1000000, -1000, 1000);
    //// TODO: разница выигрыша собственной реализацией x2.5 от geos
    //auto start = std::chrono::high_resolution_clock::now();
    //hull = ring.getConvexHullGEOS();
    //auto end = std::chrono::high_resolution_clock::now();

    //auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //EXPECT_LT(duration.count(), 5000);
}

//==============================================================================
} // namespace Entities
//==============================================================================
