#include <Task/Operations/MatrixSerializer.hpp>
#include <gtest/gtest.h>

// TODO: Именовать файлы с тестами целесообразно xxxTests.cpp, так как в файле обычно несколько тестов.
using namespace Operations;

TEST(matrix, oneTwoTree)
{
    std::vector<std::vector<int>> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    saveMatrix(matrix, "test.txt");
    auto result = loadMatrix("test.txt");

    EXPECT_EQ(matrix, result);
}
