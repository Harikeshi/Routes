#include <Task/Entities/SearchRing.hpp>

#include <chrono>
#include <random>
//! gtest
#include <gtest/gtest.h>
//==============================================================================

namespace Entities {
using SearchRing = Entities::SearchRing;

//==============================================================================
TEST(SearchRingCorrectTests, outerCW)
{
    SearchRing ring{{0, 0}, {0, 5}, {4, 5}, {2, 4}, {4, 4}, {4, 0}, {0, 0}};

    // false -> outer, true -> inner
    ring.correct(false);
    EXPECT_TRUE(ring.hasCorrectOrientation(false));
}

TEST(SearchRingCorrectTests, outerCCW)
{
    SearchRing ring{{0, 0}, {4, 0}, {4, 4}, {2, 4}, {4, 5}, {0, 5}, {0, 0}};

    ring.correct(false);
    EXPECT_TRUE(ring.hasCorrectOrientation(false));
}

TEST(SearchRingCorrectTests, innerCW)
{
    SearchRing ring{{0, 0}, {0, 5}, {4, 5}, {2, 4}, {4, 4}, {4, 0}, {0, 0}};

    ring.correct(true);

    EXPECT_TRUE(ring.hasCorrectOrientation(true));
}

TEST(SearchRingCorrectTests, innerCCW)
{
    SearchRing ring{{0, 0}, {4, 0}, {4, 4}, {2, 4}, {4, 5}, {0, 5}, {0, 0}};

    ring.correct(true);
    EXPECT_TRUE(ring.hasCorrectOrientation(true));
}
} // namespace Entities
