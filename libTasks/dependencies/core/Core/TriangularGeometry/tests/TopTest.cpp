#include <Triangulation/Objects/Top.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace TriangularGeometry
{
namespace Tests
{
//==============================================================================
/*!
* \brief Проверка на создание точки
*/
TEST( TopTest, CreateTop )
{
  double x( 2.0 );
  double y( 4.0 );
  std::shared_ptr<Top> testTop = std::make_shared<Top>( x, y );
  ASSERT_EQ( x, testTop->getX( ) );
  ASSERT_EQ( y, testTop->getY( ) );
}
//==============================================================================
/*!
* \brief Проверка на копирование точки
*/
TEST( TopTest, CopyTop )
{
  std::shared_ptr<Top> testTop = std::make_shared<Top>( 2.0, 4.0 );
  std::shared_ptr<Top> testTop2 = std::make_shared<Top>( testTop );
  ASSERT_EQ( testTop->getX( ), testTop2->getX( ) );
  ASSERT_EQ( testTop->getY( ), testTop2->getY( ) );
}
//==============================================================================
/*!
* \brief Проверка на сравнение
*/
TEST( TopTest, CompareTop )
{
  std::shared_ptr<Top> top22 = std::make_shared<Top>( 2.0, 2.0 );
  std::shared_ptr<Top> top21 = std::make_shared<Top>( 2.0, 1.0 );
  std::shared_ptr<Top> top23 = std::make_shared<Top>( 2.0, 3.0 );
  std::shared_ptr<Top> top11 = std::make_shared<Top>( 1.0, 1.0 );
  std::shared_ptr<Top> top33 = std::make_shared<Top>( 3.0, 3.0 );
  ASSERT_EQ( Larger, top22->compare( top11 ) );
  ASSERT_EQ( Larger, top22->compare( top21 ) );
  ASSERT_EQ( Less, top22->compare( top33 ) );
  ASSERT_EQ( Less, top22->compare( top23 ) );
  ASSERT_EQ( Equally, top22->compare( top22 ) );
}
//==============================================================================
/*!
* \brief Проверка на совпадение
*/
TEST(TopTest, IsSameTop)
{
  std::shared_ptr<Top> top1 = std::make_shared<Top>(2.0, 2.0);
  std::shared_ptr<Top> top2 = std::make_shared<Top>(2.0, 1.0);
  std::shared_ptr<Top> top3 = std::make_shared<Top>(3.0, 2.0);
  std::shared_ptr<Top> top4 = std::make_shared<Top>(1.0, 1.0);
  std::shared_ptr<Top> top5 = std::make_shared<Top>(2.0, 2.0);
  ASSERT_FALSE(top1->isSame(top2));
  ASSERT_FALSE(top1->isSame(top3));
  ASSERT_FALSE(top1->isSame(top4));
  ASSERT_TRUE(top1->isSame(top5));
}
//==============================================================================
/*!
* \brief Проверка на направление обхода для 3 точек
*/
TEST( TopTest, IsClockwiseTop )
{
  std::shared_ptr<Top> top00 = std::make_shared<Top>( 0.0, 0.0 );
  std::shared_ptr<Top> top01 = std::make_shared<Top>( 0.0, 1.0 );
  std::shared_ptr<Top> top10 = std::make_shared<Top>( 1.0, 0.0 );
  ASSERT_TRUE( Top::isClockwise( top00, top01, top10 ) );
  ASSERT_FALSE( Top::isClockwise( top00, top10, top01 ) );
}
//==============================================================================
/*!
* \brief Проверка на составление прямой из 3 точек
*/
TEST( TopTest, IsLineTop )
{
  std::shared_ptr<Top> top00 = std::make_shared<Top>( 0.0, 0.0 );
  std::shared_ptr<Top> top01 = std::make_shared<Top>( 0.0, 1.0 );
  std::shared_ptr<Top> top02 = std::make_shared<Top>( 0.0, 2.0 );
  std::shared_ptr<Top> top10 = std::make_shared<Top>( 1.0, 0.0 );
  ASSERT_TRUE( Top::isLine( top00, top01, top02 ) );
  ASSERT_FALSE( Top::isLine( top00, top10, top01 ) );
}
//==============================================================================
/*!
* \brief Проверка поиска фиксированного ребра
*/
TEST(TopTest, GetFixedEdge)
{
  // Вершина
  std::shared_ptr<Top> top = std::make_shared<Top>(0.0, 0.0);

  // Добавляем вершины рёбер триангуляции
  std::shared_ptr<Top> edgeLeftTop = std::make_shared<Top>(-1.0, 1.0);
  std::shared_ptr<Top> edgeFirstTop = std::make_shared<Top>(0.0, 1.0);
  std::shared_ptr<Top> edgeSecondTop = std::make_shared<Top>(1.0, 1.0);
  std::shared_ptr<Top> edgeThirdTop = std::make_shared<Top>(1.0, 0.0);
  std::shared_ptr<Top> edgeRightTop = std::make_shared<Top>(1.0, -1.0);

  // Формируем рёбра триангуляции
  std::shared_ptr<Edge> edgeLeft
    = std::make_shared<Edge>(top, edgeLeftTop);
  std::shared_ptr<Edge> edgeFirst
    = std::make_shared<Edge>(top, edgeFirstTop);
  std::shared_ptr<Edge> edgeSecond
    = std::make_shared<Edge>(top, edgeSecondTop);
  std::shared_ptr<Edge> edgeThird
    = std::make_shared<Edge>(top, edgeThirdTop);
  std::shared_ptr<Edge> edgeRight
    = std::make_shared<Edge>(top, edgeRightTop);

  edgeLeft->fixed = true;
  edgeRight->fixed = true;

  // Добавляем произвольное ребро вершине
  top->edge = edgeSecond;

  // Формируем рёбра граней напротив заданной вершины
  std::shared_ptr<Edge> edgeOpposite1
    = std::make_shared<Edge>(edgeLeftTop, edgeFirstTop);
  std::shared_ptr<Edge> edgeOpposite2
    = std::make_shared<Edge>(edgeFirstTop, edgeSecondTop);
  std::shared_ptr<Edge> edgeOpposite3
    = std::make_shared<Edge>(edgeSecondTop, edgeThirdTop);
  std::shared_ptr<Edge> edgeOpposite4
    = std::make_shared<Edge>(edgeThirdTop, edgeRightTop);

  // Все эти рёбра фиксированные
  edgeOpposite1->fixed = true;
  edgeOpposite2->fixed = true;
  edgeOpposite3->fixed = true;
  edgeOpposite4->fixed = true;

  // Формируем грани триангуляции
  std::shared_ptr<Triangle> triangle1
    = std::make_shared<Triangle>(edgeLeft, edgeFirst, edgeOpposite1);
  std::shared_ptr<Triangle> triangle2
    = std::make_shared<Triangle>(edgeFirst, edgeSecond, edgeOpposite2);
  std::shared_ptr<Triangle> triangle3
    = std::make_shared<Triangle>(edgeSecond, edgeThird, edgeOpposite3);
  std::shared_ptr<Triangle> triangle4
    = std::make_shared<Triangle>(edgeThird, edgeRight, edgeOpposite4);

  // Связываем грани и рёбра
  edgeLeft->right = triangle1;

  edgeFirst->left = triangle1;
  edgeFirst->right = triangle2;

  edgeSecond->left = triangle2;
  edgeSecond->right = triangle3;

  edgeThird->left = triangle3;
  edgeThird->right = triangle4;

  edgeRight->left = triangle4;

  edgeOpposite3->right = triangle3;

  // Проверяем поиск фиксированного ребра
  ASSERT_TRUE(edgeLeft->isSame(top->getFixedEdge(Geometry::Left)));
  ASSERT_TRUE(edgeRight->isSame(top->getFixedEdge(Geometry::Right)));

  // Поиск фиксированного ребра для вершины, которая в ребре вторая
  edgeSecondTop->edge = edgeOpposite3;
  ASSERT_TRUE(edgeOpposite3->isSame(
    edgeSecondTop->getFixedEdge(Geometry::Left)));
  ASSERT_TRUE(edgeOpposite2->isSame(
    edgeSecondTop->getFixedEdge(Geometry::Right)));
}
//==============================================================================
} //! Tests
} //! TriangularGeometry
//==============================================================================
