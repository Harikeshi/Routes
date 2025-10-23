#include <gtest/gtest.h>

#include <Task/Entities/RegionClassifier.hpp>

namespace Entities {

using SearchRing = Entities::SearchRing;

class ClassifierTest : public RegionClassifier
{
public:
    std::pair<Point2D, Point2D> longestMidline(const SearchRing::OBB& obb)
    {
        return RegionClassifier::longestMidline(obb);
    }
};

class ClassifyTest : public ::testing::Test
{
protected:
    ClassifierTest classifier{};
    SearchRegion region;

    ClassifyTest()
    {
    }

    void SetUp() override
    {
    }
};

void print(const PrimaryEntities::Polygon<Point2D>& polygon)
{
    for (auto const& point : polygon.outer())
    {
        std::cout << "(" << point.getX() << ", " << point.getY() << ")" << std::endl;
    }
}

void print(const PrimaryEntities::Ring<std::pair<Point2D, bool>>& ring)
{
    for (auto const& point : ring)
    {
        std::cout << "(" << point.first.getX() << ", " << point.first.getY() << ") - " << (point.second ? "HULL" : "miss") << std::endl;
    }
}

void print(const PrimaryEntities::Ring<Point2D>& ring)
{
    for (auto const& point : ring)
    {
        std::cout << "(" << point.getX() << ", " << point.getY() << ")" << std::endl;
    }
}

TEST_F(ClassifyTest, LongestMidlineTest)
{
    //!a--
    SearchRing ring{{0, 0}, {2, 2}, {1, -1}, {3, 1}};

    std::pair<Point2D, Point2D> midLine = classifier.longestMidline(ring.getOBB());

    ASSERT_EQ(midLine.first, (Point2D{.5, -.5}));
    ASSERT_EQ(midLine.second, (Point2D{2.5, 1.5}));

    //!b--
    ring = static_cast<SearchRing>(Ring2D{{0., 0.}, {1., 1.}, {3., -1.}, {2., -2.}});

    midLine = classifier.longestMidline(ring.getOBB());

    ASSERT_EQ(midLine.first, (Point2D{2.5, -1.5}));
    ASSERT_EQ(midLine.second, (Point2D{.5, .5}));

    //!c--
    ring = static_cast<SearchRing>(Ring2D{{0., 0.}, {0., 3.}, {2., 3.}, {2., 0.}});

    midLine = classifier.longestMidline(ring.getOBB());

    ASSERT_EQ(midLine.first, (Point2D{1., 0.}));
    ASSERT_EQ(midLine.second, (Point2D{1., 3.}));

    //!d--
    ring = static_cast<SearchRing>(Ring2D{{0, 0}, {0., 2.}, {3, 2}, {3, 0}});

    midLine = classifier.longestMidline(ring.getOBB());

    ASSERT_EQ(midLine.first, (Point2D{0., 1.}));
    ASSERT_EQ(midLine.second, (Point2D{3., 1.}));

    //!e--
    ring = static_cast<SearchRing>(Ring2D{{0, 0}, {0, 3}, {3, 3}, {3, 0}});

    midLine = classifier.longestMidline(ring.getOBB());

    ASSERT_EQ(midLine.first, (Point2D{1.5, 0}));
    ASSERT_EQ(midLine.second, (Point2D{1.5, 3}));

    //!f--
    ring = static_cast<SearchRing>(Ring2D{{0, 0}, {2, 2}, {4, 0}, {2, -2}});

    midLine = classifier.longestMidline(ring.getOBB());

    ASSERT_EQ(midLine.first, (Point2D{3., -1.}));
    ASSERT_EQ(midLine.second, (Point2D{1., 1.}));
}
} // namespace Entities
