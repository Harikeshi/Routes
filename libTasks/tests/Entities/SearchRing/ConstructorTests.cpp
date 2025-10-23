#include <Task/Entities/SearchRing.hpp>

#include <gtest/gtest.h>

namespace Entities {
using SearchRing = Entities::SearchRing;

TEST(SearchRingConstructorTests, initializer_listCW)
{
    SearchRing ring{{0, 0}, {0, 5}, {4, 5}, {2, 4}, {4, 4}, {4, 0}, {0, 0}};

    EXPECT_TRUE(ring.hasCorrectOrientation());
}

TEST(SearchRingConstructorTests, initializer_listCCW)
{
    SearchRing ring{{0, 0}, {4, 0}, {4, 4}, {2, 4}, {4, 5}, {0, 5}, {0, 0}};

    EXPECT_FALSE(ring.hasCorrectOrientation());
}

TEST(SearchRingConstructorTests, RingPointsCW)
{
    PrimaryEntities::Ring<Point2D> ring2d{{0, 0}, {0, 5}, {4, 5}, {2, 4}, {4, 4}, {4, 0}, {0, 0}};

    SearchRing ring(ring2d);

    EXPECT_TRUE(ring.hasCorrectOrientation());
}

TEST(SearchRingConstructorTests, RingPointsCCW)
{
    PrimaryEntities::Ring<Point2D> ring2d{{0, 0}, {4, 0}, {4, 4}, {2, 4}, {4, 5}, {0, 5}, {0, 0}};

    SearchRing ring(ring2d);

    EXPECT_FALSE(ring.hasCorrectOrientation());
}
} // namespace Entities
