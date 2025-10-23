#include <Task/Operations/JsonOperations.hpp>
#include <Task/SearchTask.hpp>

#include <gtest/gtest.h>

/// @todo пока не проходит тест
/*TEST(SearchTaskFromFileTests, DefaultFile)
{
    std::string binary = BINARY_DIR;
    std::string from = binary + "/bin/data/request_03_27.json";
    std::string to = binary + "/bin/data/report_03_27.json";

    // read from file to json
    auto request = Operations::fromfile(from);
    auto task = SearchTask{request};

    auto result = task.computeRoute(SearchScheme::Zigzag);

    Operations::toFile(to, result);

    ASSERT_FALSE(result["routes"].empty());
}*/
