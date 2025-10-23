#include <Task/Entities/SearchRing.hpp>

//! gtest
#include <gtest/gtest.h>

namespace Entities {
using SearchRing = Entities::SearchRing;

class SearchRingTests : public ::testing::Test
{
protected:
    SearchRing ring;

    void SetUp() override
    {
        ring = SearchRing{{0, 0.}, {0., 100}, {100., 100.}, {100., 0}, {0., 0.}};
    }
};

TEST_F(SearchRingTests, HasIntersection)
{
    ASSERT_TRUE(ring.hasIntersection(Point2D{-1, 1}, Point2D{1, -1}));
}
} // namespace Entities
