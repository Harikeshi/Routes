#include <Task/Operations/Random.hpp>
#include <gtest/gtest.h>

namespace Operations {

TEST(Random, notEqual)
{
    for (int i = 0; i < 10; ++i)
    {
        double currVal = Random::getRandom(1, 2);
        double nextVal = Random::getRandom(1, 2);
        EXPECT_NE(currVal, nextVal);
    }
}

} // namespace Operations
