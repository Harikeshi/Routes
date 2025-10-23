#include <gtest/gtest.h>

#ifdef DEBUG

extern "C" int __lsan_is_turned_off()
{
    return 1;
}
#endif

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Russian");

    ::testing::InitGoogleTest(&argc, argv);
    //::testing::GTEST_FLAG(filter) = "*AbstrEfficiencyTest*";
    //::testing::GTEST_FLAG(filter) = "*computeRoute_rectangle_inners*";
    //::testing::GTEST_FLAG(filter) = "*detTackTest*";
    //::testing::GTEST_FLAG(filter) = "*SearchTaskInRegionTest*"; //SearchTaskInRegionTest EfficiencyTest

    return RUN_ALL_TESTS();
}
