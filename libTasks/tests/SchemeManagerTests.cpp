#include <Task/SchemeManager.hpp>

#include <gtest/gtest.h>

class SchemeManagerTest : public ::testing::Test
{
protected:
    SchemeManager manager{};
};

/**
 * @brief Тест создания объекта входных данных схемы.
 *
 */
TEST_F(SchemeManagerTest, CreateInput)
{
    auto input = manager.createInput(SearchScheme::Zigzag);

    EXPECT_NE(input, nullptr);
}

TEST_F(SchemeManagerTest, CreateBadInput)
{
    auto input = manager.createInput(static_cast<SearchScheme>(12));

    EXPECT_EQ(input, nullptr);
}

TEST_F(SchemeManagerTest, CreateScheme)
{
    Schemes::Search::InRegion::Input input;

    auto scheme = manager.createScheme(SearchScheme::Zigzag, input);

    EXPECT_NE(scheme, nullptr);
}

TEST_F(SchemeManagerTest, GetSchemes)
{
    auto schemes = manager.getSchemes(SearchType::InRegion);

    EXPECT_FALSE(schemes.empty());
    EXPECT_EQ(schemes[0], 0);
}
