#include <TriangularGeometry/PathFinder.hpp>
//! TriangularGeometry
#include <TriangularGeometry/TriangularException.hpp>
//! SamplesForTests
#include <SamplesForTests/BaseFlatFigures.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace TriangularGeometry
{
namespace Tests
{
//==============================================================================
class PathFinderTest : public ::testing::Test
{
protected:
//==============================================================================
  void SetUp( )
  {
    // Первая триангулированная область

    // Создание полигона
    Polygon2D poly({
      {
        Point2D(0.0, 0.0), Point2D(0.0, 1.0), Point2D(1.0, 2.0),
        Point2D(0.0, 3.0), Point2D(0.0, 4.0), Point2D(5.0, 4.0),
        Point2D(5.0, 2.0), Point2D(5.0, 0.0), Point2D(0.0, 0.0)
      },
      {
        Point2D(2.0, 1.0), Point2D(4.0, 1.0), Point2D(4.0, 3.0),
        Point2D(2.0, 3.0), Point2D(2.0, 1.0)
      }
    });

    std::vector<Polygon2D> trArea;
    trArea.emplace_back( poly );
    // Формирование области триангуляции
    firstArea = std::make_shared<TriangularArea>( trArea );

    // Вторая триангулированная область

    // Создание полигона
    Polygon2D figure({
      {
        Point2D(0.5, 3.5), Point2D(1., 5.5), Point2D(2., 6.),
        Point2D(1., 6.5), Point2D(2.5, 8.), Point2D(3., 7.5),
      Point2D(3., 8.), Point2D(4.5, 8.5), Point2D(5., 7.),
      Point2D(6.5, 9.), Point2D(9., 7.), Point2D(8.5, 5.5),
      Point2D(7.5, 4.5), Point2D(8.5, 4.5), Point2D(8.5, 3.5),
      Point2D(7.5, 2.5), Point2D(6.5, 2.5), Point2D(6., 3.5),
      Point2D(4.5, 3.5), Point2D(4.5, 2.5), Point2D(2.5, 2.),
      Point2D(0.5, 3.5)
      },
      {
        Point2D(2.5, 4.5), Point2D(3., 4.), Point2D(3.5, 4.5),
        Point2D(3., 5.5), Point2D(2.5, 5.), Point2D(2.5, 4.5)
      },
      {
        Point2D(5.5, 6.), Point2D(6., 5.5), Point2D(6., 6.),
        Point2D(6.5, 5.), Point2D(6.5, 6.), Point2D(6., 6.5),
      Point2D(5.5, 6.)
      }
    });

    trArea.clear();
    trArea.emplace_back(figure);
    // Формирование области триангуляции
    secondArea = std::make_shared<TriangularArea>(trArea);

    // Третья триангулированная область

    // создание двух непересекающихся квадратов
    Polygon2D square1
      = SamplesForTests::buildRectangle(Point2D(0.0, 0.0), 1., 1.);

    Polygon2D square2
      = SamplesForTests::buildRectangle(Point2D(2.0, 2.0), 1., 1.);

    trArea.clear();
    trArea.emplace_back(square1);
    trArea.emplace_back(square2);
    // Формирование области триангуляции
    twoSquaresArea = std::make_shared<TriangularArea>(trArea);
  }
//==============================================================================
  void TearDown( )
  {
  }
//==============================================================================
  // Триангуляция простой области с одной дыркой
  std::shared_ptr<TriangularGeometry::TriangularArea> firstArea;

  // Триангуляция более сложной области с двумя дырками
  std::shared_ptr<TriangularGeometry::TriangularArea> secondArea;

  // Триангуляция двух непересекающихся квадратов
  std::shared_ptr<TriangularGeometry::TriangularArea> twoSquaresArea;
};
//==============================================================================
// Проверка построения пути между двумя точками
TEST_F(PathFinderTest, PathFinderFunction)
{
  // Первый пример (простая фигура)
  PathFinder pathFinder(firstArea);
  auto path = pathFinder.findPath(Point2D(1.5, 2.), Point2D(4.5, 2.));

  ASSERT_TRUE(path.size() == 4);
  ASSERT_EQ(path.at(0), Point2D(1.5, 2.));
  ASSERT_EQ(path.at(1), Point2D(2., 1.));
  ASSERT_EQ(path.at(2), Point2D(4., 1.));
  ASSERT_EQ(path.at(3), Point2D(4.5, 2.));

  // Второй пример (фигура посложнее)
  PathFinder figurePathFinder(secondArea);
  auto figurePath
    = figurePathFinder.findPath(Point2D(1., 3.5), Point2D(8., 7.5));

  ASSERT_TRUE(figurePath.size() == 4);
  ASSERT_EQ(figurePath.at(0), Point2D(1., 3.5));
  ASSERT_EQ(figurePath.at(1), Point2D(3., 4.));
  ASSERT_EQ(figurePath.at(2), Point2D(6., 6.5));
  ASSERT_EQ(figurePath.at(3), Point2D(8., 7.5));

  // Третий пример (та же фигура, смещён старт)
  figurePath
    = figurePathFinder.findPath(Point2D(5.5, 5.5), Point2D(8., 7.5));

  ASSERT_TRUE(figurePath.size() == 4);
  ASSERT_EQ(figurePath.at(0), Point2D(5.5, 5.5));
  ASSERT_EQ(figurePath.at(1), Point2D(5.5, 6.));
  ASSERT_EQ(figurePath.at(2), Point2D(6., 6.5));
  ASSERT_EQ(figurePath.at(3), Point2D(8., 7.5));

  // Пример с совпадением начальной и конечной вершин
  figurePath
    = figurePathFinder.findPath(Point2D(1., 3.5), Point2D(1., 3.5));

  ASSERT_TRUE(figurePath.size() == 2);
  ASSERT_EQ(figurePath.at(0), Point2D(1., 3.5));
  ASSERT_EQ(figurePath.at(1), Point2D(1., 3.5));

  // Пример с попаданием начальной и конечной вершин на одну грань
  figurePath
    = figurePathFinder.findPath(Point2D(1., 3.5), Point2D(2., 3.));

  ASSERT_TRUE(figurePath.size() == 2);
  ASSERT_EQ(figurePath.at(0), Point2D(1., 3.5));
  ASSERT_EQ(figurePath.at(1), Point2D(2., 3.));

  // Пример с попаданием начальной вершины на ребро грани
  path = pathFinder.findPath(Point2D(1., 3.), Point2D(4.5, 2.));

  ASSERT_TRUE(path.size() == 3);
  ASSERT_EQ(path.at(0), Point2D(1., 3.));
  ASSERT_EQ(path.at(1), Point2D(4., 3.));
  ASSERT_EQ(path.at(2), Point2D(4.5, 2.));

  // Пример с попаданием начальной вершины на граничное ребро
  // триангуляции
  path = pathFinder.findPath(Point2D(0., 3.5), Point2D(4.5, 2.));

  ASSERT_TRUE(path.size() == 2);
  ASSERT_EQ(path.at(0), Point2D(0., 3.5));
  ASSERT_EQ(path.at(1), Point2D(4.5, 2.));

  // Пример с попаданием конечной вершины на ребро грани
  path = pathFinder.findPath(Point2D(1., 3.), Point2D(4.5, 2.5));

  ASSERT_TRUE(path.size() == 3);
  ASSERT_EQ(path.at(0), Point2D(1., 3.));
  ASSERT_EQ(path.at(1), Point2D(4., 3.));
  ASSERT_EQ(path.at(2), Point2D(4.5, 2.5));

  // Пример с попаданием конечной вершины на граничное ребро
  // триангуляции
  path = pathFinder.findPath(Point2D(0., 0.), Point2D(5., 3.));

  ASSERT_TRUE(path.size() == 3);
  ASSERT_EQ(path.at(0), Point2D(0., 0.));
  ASSERT_EQ(path.at(1), Point2D(4., 1.));
  ASSERT_EQ(path.at(2), Point2D(5., 3.));

  // Пример с начальной вершиной, попавшей на вершину триангуляции
  path = pathFinder.findPath(Point2D(1., 2.), Point2D(4.5, 2.));

  ASSERT_TRUE(path.size() == 4);
  ASSERT_EQ(path.at(0), Point2D(1., 2.));
  ASSERT_EQ(path.at(1), Point2D(2., 1.));
  ASSERT_EQ(path.at(2), Point2D(4., 1.));
  ASSERT_EQ(path.at(3), Point2D(4.5, 2.));

  // Пример с конечной вершиной, попавшей на вершину триангуляции
  path = pathFinder.findPath(Point2D(4.5, 2.), Point2D(1., 2.));

  ASSERT_TRUE(path.size() == 4);
  ASSERT_EQ(path.at(0), Point2D(4.5, 2.));
  ASSERT_EQ(path.at(1), Point2D(4., 1.));
  ASSERT_EQ(path.at(2), Point2D(2., 1.));
  ASSERT_EQ(path.at(3), Point2D(1., 2.));

  // Пример с попаданием начальной вершины за пределы области
  // триангуляции
  ASSERT_THROW(
    figurePathFinder.findPath(Point2D(0., 3.5), Point2D(1., 3.5)),
    TriangularFailure);

  try
  {
    figurePath = figurePathFinder.findPath(Point2D(0., 3.5), Point2D(1., 3.5));
  }
  catch (TriangularFailure& ex)
  {
    ASSERT_EQ(ex.getCode(), TransitFailure);
    ASSERT_NE(std::string(ex.what()).find("OneOfPointsOutOfArea"),
      std::string::npos);
  }

  // Пример с несвязностью области (точки в разных полигонах)
  PathFinder twoSquaresPathFinder(twoSquaresArea);
  ASSERT_THROW(
    twoSquaresPathFinder.findPath(Point2D(0.5, 0.5), Point2D(2.5, 2.5)),
    TriangularFailure);

  try
  {
    auto twoSquaresPath
      = twoSquaresPathFinder.findPath(Point2D(0.5, 0.5), Point2D(2.5, 2.5));
  }
  catch (TriangularFailure& ex)
  {
    ASSERT_EQ(ex.getCode(), TransitFailure);
    ASSERT_NE(std::string(ex.what()).find("InconnectedPoints"),
      std::string::npos);
  }
}
//==============================================================================
} //! Tests
} //! TriangularGeometry
//==============================================================================