#include <Task/Entities/RegionClassifier.hpp>

#include <gtest/gtest.h>

// TODO: Относится к RegionClassify

namespace Entities {
using SearchRing = Entities::SearchRing;

TEST(SearchRingConcavityTest, square)
{
    SearchRing ring = SearchRing{{0, 0}, {0, 5}, {5, 5}, {5, 0}, {0, 0}};
    EXPECT_EQ(getMaxConcavity(ring), 0);
}

TEST(SearchRingConcavityTest, trapeze)
{
    SearchRing ring = SearchRing{{0, 0}, {2, 3}, {4, 3}, {5, 0}, {0, 0}};
    EXPECT_EQ(getMaxConcavity(ring), 0);
}

TEST(SearchRingConcavityTest, concavityTop)
{
    SearchRing ring = SearchRing{{1, 1}, {1, 5}, {4, 5}, {6, 3}, {8, 5}, {11, 5}, {11, 1}, {1, 1}};
    EXPECT_EQ(getMaxConcavity(ring), 2);
}

TEST(SearchRingConcavityTest, concavityRight)
{
    SearchRing ring = SearchRing{{1, 1}, {1, 5}, {11, 5}, {9, 3}, {11, 1}, {1, 1}};
    EXPECT_EQ(getMaxConcavity(ring), 2);
}

TEST(SearchRingConcavityTest, concavityTopAndRight)
{
    SearchRing ring = SearchRing{{1, 1}, {1, 5}, {4, 5}, {6, 2}, {8, 5}, {11, 5}, {9, 3}, {11, 1}, {1, 1}};
    EXPECT_EQ(getMaxConcavity(ring), 3);
}

TEST(SearchRingConcavityTest, concavityRightAndTop)
{
    SearchRing ring = SearchRing{{1, 1}, {1, 5}, {4, 5}, {6, 3}, {8, 5}, {11, 5}, {8, 3}, {11, 1}, {1, 1}};
    EXPECT_EQ(getMaxConcavity(ring), 3);
}

TEST(SearchRingConcavityTest, BigConcavityTop)
{
    SearchRing ring = SearchRing{{1, 1}, {1, 5}, {3, 5}, {4, 3}, {6, 2}, {8, 3}, {9, 5}, {11, 5}, {11, 1}, {1, 1}};
    EXPECT_EQ(getMaxConcavity(ring), 3);
}

TEST(SearchRingConcavityTest, BigConcavityRightAndTop)
{
    SearchRing ring = SearchRing{{1, 1}, {1, 5}, {3, 5}, {4, 3}, {6, 2}, {8, 3}, {9, 5}, {11, 5}, {9, 3}, {11, 1}, {1, 1}};
    EXPECT_EQ(getMaxConcavity(ring), 3);
}

TEST(SearchRingConcavityTest, BigConcavityTopAndRight)
{
    SearchRing ring = SearchRing{{1, 1}, {1, 5}, {2, 3}, {4, 2}, {6, 3}, {7, 5}, {11, 5}, {7, 3}, {11, 1}, {1, 1}};
    EXPECT_EQ(getMaxConcavity(ring), 4);
}

TEST(SearchRingConcavityTest, Star)
{
    SearchRing ring = SearchRing{{1, 1}, {2, 3}, {1, 5}, {3, 3}, {5, 5}, {4, 3}, {5, 1}, {3, 2}, {1, 1}};
    EXPECT_EQ(getMaxConcavity(ring), 2);
}

TEST(SearchRingConcavityTest, DoubleConcavity)
{
    SearchRing ring = SearchRing{{1, 1}, {2, 2}, {1, 5}, {4, 4}, {5, 2}, {8, 5}, {8, 1}, {1, 1}};
    EXPECT_EQ(getMaxConcavity(ring), 3);
}

TEST(SearchRingConcavityTest, Angle45Concavity)
{
    SearchRing ring = SearchRing{{1, 1}, {2, 2}, {1, 7}, {5, 7}, {6, 5}, {6, 3}, {9, 3}, {9, 1}, {1, 1}};
    EXPECT_NEAR(getMaxConcavity(ring), 2.12132, 1e-5);
}

TEST(SearchRingConcavityTest, Butterfly)
{
    SearchRing ring = SearchRing{{0, 0}, {0, 1}, {1, 0}, {2, 1}, {2, 0}, {1, 0}, {0, 0}};
    EXPECT_EQ(getMaxConcavity(ring), 1);
}
} // namespace Entities
