#include <Task/Entities/RegionClassifier.hpp>
#include <Task/Entities/SearchRegion.hpp>

#include <gtest/gtest.h>

// TODO: Относится к RegionClassify

namespace Entities {
using SearchRing = Entities::SearchRing;

class RegionClassifierTest : public testing::Test
{
protected:
    SearchRing ring;
    SearchRing ring_for_polygon;
    Polygon2D polygon;
    Point2D start;
    Point2D end;
    void SetUp() override
    {
        ring = {{0, 0}, {0, 100}, {200, 200}, {0, 300}, {0, 400}, {400, 300}, {500, 400}, {500, 250}, {500, 0}, {250, 0}, {0, 0}};
        ring_for_polygon = SearchRing{{0, 0}, {0, 40100}, {40100, 40000}, {40100, 7900}, {50100, 39900}, {50029, 11900}, {50000, -100}, {24000, -48}, {0, 0}};
        polygon = {{{0, 0}, {50000, -100}, {50100, 39900}, {40100, 7900}, {40100, 40000}, {0, 40100}, {0, 0}}};
        start = {24000, -48};
        end = {50029, 11900};
    }
};

TEST_F(RegionClassifierTest, convex)
{
    auto convex = ring.getConvexHullGEOS();
    auto conc = ring_for_polygon.getConvexHullGEOS();
}

TEST_F(RegionClassifierTest, getConcavities_region)
{
    auto region = Entities::SearchRegion{polygon, start, end};

    auto convex = ring.getConvexHullGEOS();

    auto result = Entities::getConcavities(ring);
}

TEST_F(RegionClassifierTest, getConcavities)
{
    auto convex = ring.getConvexHullGEOS();

    auto result = Entities::getConcavities(ring);
}
} // namespace Entities