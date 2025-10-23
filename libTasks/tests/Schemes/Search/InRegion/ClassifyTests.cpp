#include <Task/Entities/RegionClassifier.hpp>

#include <gtest/gtest.h>

class ClassifierTest : public Entities::RegionClassifier
{
public:
    std::pair<Point2D, Point2D> longestMidline(const Entities::SearchRing::OBB& obb)
    {
        return RegionClassifier::longestMidline(obb);
    }

    bool canPassThroughCenter(const std::vector<Entities::SearchRing>& inners, const Entities::SearchRing::OBB& obb) const
    {
        return Entities::RegionClassifier::canPassThroughCenter(inners, obb);
    }

    bool isType1(const Entities::SearchRegion& region, double radiusHAS) const
    {
        return Entities::RegionClassifier::isType1(region, radiusHAS);
    }

    bool isType2(const Entities::SearchRegion& region) const
    {
        return Entities::RegionClassifier::isType2(region);
    }
};

class ClassifyTests : public ::testing::Test
{
protected:
    Entities::SearchRegion slim_polygon;
    Entities::SearchRegion without_holes_polygon;
    Entities::SearchRegion circle_with_hole_polygon;
    Entities::SearchRegion square;

    ClassifierTest classifier;

    void SetUp() override
    {
        slim_polygon = Entities::SearchRegion{{{{0, 0}, {4000, 6000}, {50000, 6000}, {50000, 0}, {0, 0}}}, {0, 0}, {50000, 0}};
        without_holes_polygon = Entities::SearchRegion{{{{0, 0}, {0, 50000}, {70000, 50000}, {90000, 25000}, {70000, 5000}, {20000, 10000}, {0, 0}}}, {0, 0}, {20000, 10000}};
        circle_with_hole_polygon = Entities::SearchRegion{{{{0, 0}, {15000, 45000}, {60000, 45000}, {75000, 0}, {55000, -30000}, {15000, -40000}, {0, 0}},
                                                           {{20000, -1000}, {22000, 22000}, {40000, 25000}, {20000, -1000}}},
                                                          {0, 0},
                                                          {22000, 22000}};
        square = Entities::SearchRegion{{{{0, 0}, {0, 6000}, {6000, 6000}, {6000, 0}, {0, 0}}}, {0, 0}, {6000, 6000}};
    }

    void TearDown() override
    {
    }
};

TEST_F(ClassifyTests, longestMidline_slim)
{
    // Slim
    auto obb = slim_polygon.outer().getOBB();
    auto [start, end] = classifier.longestMidline(obb);

    ASSERT_EQ(start, Point2D(0, 3000));
    ASSERT_EQ(end, Point2D(50000, 3000));
}

TEST_F(ClassifyTests, longestMidline_long)
{
    auto obb = without_holes_polygon.outer().getOBB();
    auto [start, end] = classifier.longestMidline(obb);

    ASSERT_EQ(start, Point2D(89543.147207929695, 31395.939086079259));
    ASSERT_EQ(end, Point2D(-1776.6497461425097, 24873.09644677371));
}

TEST_F(ClassifyTests, longestMidline_about_circle)
{
    auto obb = circle_with_hole_polygon.outer().getOBB();
    auto [start, end] = classifier.longestMidline(obb);

    ASSERT_EQ(start, Point2D(23250.000000179658, -42750.000000070802));
    ASSERT_EQ(end, Point2D(53249.999999840162, 47249.999999810658));
}

TEST_F(ClassifyTests, canPassThroughCenter)
{
    // Slim
    auto obb = slim_polygon.outer().getOBB();
    ASSERT_TRUE(classifier.canPassThroughCenter(slim_polygon.inners(), obb));

    // long
    obb = without_holes_polygon.outer().getOBB();
    ASSERT_TRUE(classifier.canPassThroughCenter(without_holes_polygon.inners(), obb));

    // about circle
    obb = circle_with_hole_polygon.outer().getOBB();
    ASSERT_FALSE(classifier.canPassThroughCenter(circle_with_hole_polygon.inners(), obb));
}

TEST_F(ClassifyTests, isType1)
{
    // slim
    ASSERT_FALSE(classifier.isType1(slim_polygon, 5999));
    ASSERT_TRUE(classifier.isType1(slim_polygon, 6000));

    // long
    ASSERT_FALSE(classifier.isType1(without_holes_polygon, 6000)); // по ширине

    // about circle
    ASSERT_FALSE(classifier.isType1(circle_with_hole_polygon, 6000)); // по ширине
}

TEST_F(ClassifyTests, isType2)
{
    // slim
    ASSERT_FALSE(classifier.isType2(slim_polygon)); // по ширине

    // long
    ASSERT_FALSE(classifier.isType2(without_holes_polygon)); // по ширине

    // about circle
    ASSERT_FALSE(classifier.isType2(circle_with_hole_polygon)); // много внутренних полигонов > 10%
}

TEST_F(ClassifyTests, classify)
{
    // slim
    auto type = classifier.classify(slim_polygon, 6001);
    ASSERT_EQ(type, 0);

    // long
    type = classifier.classify(without_holes_polygon, 6001);
    ASSERT_EQ(type, 2);

    // about circle
    type = classifier.classify(circle_with_hole_polygon, 6001);
    ASSERT_EQ(type, 2);

    // square
    type = classifier.classify(square, 6001);
    ASSERT_EQ(type, 1);
}
