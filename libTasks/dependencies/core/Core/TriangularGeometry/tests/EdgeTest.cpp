#include <Triangulation/Objects/Edge.hpp>

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
  * \brief Проверка на создание ребра
  */
TEST( EdgeTest, CreateEdge )
{
  std::shared_ptr<Top> topI = std::make_shared<Top>( 0.0, 0.0 );
  std::shared_ptr<Top> topJ = std::make_shared<Top>( 2.0, 2.0 );
  std::shared_ptr<Edge> edge = std::make_shared<Edge>( topI, topJ );
  ASSERT_TRUE( edge->getI( ) == topI );
  ASSERT_TRUE( edge->getJ( ) == topJ );
  ASSERT_TRUE( edge->left == nullptr );
  ASSERT_TRUE( edge->right == nullptr );
  ASSERT_FALSE( edge->fixed );
}
//==============================================================================
/*!
* \brief Проверка на копирование ребра
*/
TEST( EdgeTest, CopyEdge )
{
  std::shared_ptr<Top> topI = std::make_shared<Top>( 0.0, 0.0 );
  std::shared_ptr<Top> topJ = std::make_shared<Top>( 2.0, 2.0 );
  std::shared_ptr<Edge> edge = std::make_shared<Edge>( topI, topJ );
  edge->fixed = true;
  std::shared_ptr<Edge> edgeCopy = std::make_shared<Edge>( edge );
  ASSERT_TRUE( edgeCopy->getI( ) == edge->getI( ) );
  ASSERT_TRUE( edgeCopy->getJ( ) == edge->getJ( ) );
  ASSERT_TRUE( edgeCopy->left == edge->left );
  ASSERT_TRUE( edgeCopy->right == edge->right );
  ASSERT_TRUE( edgeCopy->fixed == edge->fixed );
}
//==============================================================================
/*!
* \brief Проверка на совпадение рёбер
*/
TEST(EdgeTest, IsSameEdge)
{
  std::shared_ptr<Top> topI = std::make_shared<Top>(0.0, 0.0);
  std::shared_ptr<Top> topJ = std::make_shared<Top>(2.0, 2.0);
  std::shared_ptr<Top> topOtherJ = std::make_shared<Top>(3.0, 2.0);
  std::shared_ptr<Edge> edge = std::make_shared<Edge>(topI, topJ);
  std::shared_ptr<Edge> equalEdge = std::make_shared<Edge>(topI, topJ);
  std::shared_ptr<Edge> reversedEdge = std::make_shared<Edge>(topJ, topI);
  std::shared_ptr<Edge> otherEdge = std::make_shared<Edge>(topI, topOtherJ);

  // Совпадающие рёбра
  ASSERT_TRUE(edge->isSame(equalEdge));
  // Рёбра, отличающиеся только направлением, также считаются совпавшими
  ASSERT_TRUE(edge->isSame(reversedEdge));
  // Различные рёбра
  ASSERT_FALSE(edge->isSame(otherEdge));
}
//==============================================================================
/*!
* \brief Проверка на пересечение рёбер не в концевых вершинах
*/
TEST(EdgeTest, IsIntersectEdge)
{
  std::shared_ptr<Top> topI = std::make_shared<Top>(0.0, 0.0);
  std::shared_ptr<Top> topJ = std::make_shared<Top>(2.0, 2.0);
  std::shared_ptr<Top> topIntersectedI = std::make_shared<Top>(0.0, 2.0);
  std::shared_ptr<Top> topIntersectedJ = std::make_shared<Top>(2.0, 0.0);
  std::shared_ptr<Top> topOtherI = std::make_shared<Top>(0.0, 1.0);
  std::shared_ptr<Top> topOtherJ = std::make_shared<Top>(0.0, 3.0);

  std::shared_ptr<Edge> edge = std::make_shared<Edge>(topI, topJ);
  std::shared_ptr<Edge> equalEdge = std::make_shared<Edge>(topI, topJ);
  std::shared_ptr<Edge> reversedEdge = std::make_shared<Edge>(topJ, topI);
  std::shared_ptr<Edge> intersectedEdge
    = std::make_shared<Edge>(topIntersectedI, topIntersectedJ);
  std::shared_ptr<Edge> otherEdge = std::make_shared<Edge>(topOtherI, topOtherJ);
  std::shared_ptr<Edge> intersectedInTopEdge
    = std::make_shared<Edge>(topI, topOtherJ);

  // Пересекающиеся рёбра
  ASSERT_TRUE(edge->isIntersectEdge(intersectedEdge));

  // Различные рёбра
  ASSERT_FALSE(edge->isIntersectEdge(otherEdge));
  // Рёбра, совпавшие только в одной концевой вершине
  ASSERT_FALSE(edge->isIntersectEdge(intersectedInTopEdge));
  // Совпадающие рёбра совпадают в том числе и в концевых вершинах, не подходят
  ASSERT_FALSE(edge->isIntersectEdge(equalEdge));
  ASSERT_FALSE(edge->isIntersectEdge(reversedEdge));
}
//==============================================================================
/*!
* \brief Проверка на попадание вершины на ребро
*/
TEST(EdgeTest, AtEdge)
{
  std::shared_ptr<Top> topI = std::make_shared<Top>(0.0, 0.0);
  std::shared_ptr<Top> topJ = std::make_shared<Top>(2.0, 0.0);
  std::shared_ptr<Edge> edge = std::make_shared<Edge>(topI, topJ);

  std::shared_ptr<Top> topAtEdge = std::make_shared<Top>(1.0, 0.0);
  std::shared_ptr<Top> topNotAtEdge = std::make_shared<Top>(3.0, 0.0);
  
  // Вершина, попавшая на ребро
  ASSERT_TRUE(edge->atEdge(topAtEdge));

  // Вершина, попавшая на вершину ребра
  ASSERT_FALSE(edge->atEdge(topI));
  ASSERT_FALSE(edge->atEdge(topJ));

  // Вершина, не попавшая на ребро
  ASSERT_FALSE(edge->atEdge(topNotAtEdge));
}
//==============================================================================
/*!
* \brief Проверка определения ориентации ребра в грани
*/
TEST(EdgeTest, IsCounterclockwise)
{
  std::shared_ptr<Top> topI = std::make_shared<Top>(0.0, 0.0);
  std::shared_ptr<Top> topJ = std::make_shared<Top>(2.0, 0.0);
  std::shared_ptr<Top> topK = std::make_shared<Top>(2.0, 2.0);

  std::shared_ptr<Edge> edgeFirst = std::make_shared<Edge>(topI, topJ);
  std::shared_ptr<Edge> edgeSecond = std::make_shared<Edge>(topJ, topK);
  std::shared_ptr<Edge> edgeThird = std::make_shared<Edge>(topK, topI);

  std::shared_ptr<Edge> edgeFirstOtherDirection
    = std::make_shared<Edge>(topJ, topI);
  std::shared_ptr<Edge> edgeSecondOtherDirection
    = std::make_shared<Edge>(topK, topJ);
  std::shared_ptr<Edge> edgeThirdOtherDirection
    = std::make_shared<Edge>(topI, topK);

  std::shared_ptr<Triangle> triangleCounterclockwise
    = std::make_shared<Triangle>(edgeFirst, edgeSecond, edgeThird);

  std::shared_ptr<Triangle> triangleClockwise = std::make_shared<Triangle>(
    edgeFirstOtherDirection,
    edgeSecondOtherDirection,
    edgeThirdOtherDirection);

  // Все рёбра направлены против часовой стрелки
  ASSERT_TRUE(Edge::isCounterclockwise(
    triangleCounterclockwise->p, triangleCounterclockwise));
  ASSERT_TRUE(Edge::isCounterclockwise(
    triangleCounterclockwise->q, triangleCounterclockwise));
  ASSERT_TRUE(Edge::isCounterclockwise(
    triangleCounterclockwise->r, triangleCounterclockwise));

  // Все рёбра направлены по часовой стрелке
  ASSERT_FALSE(Edge::isCounterclockwise(
    triangleClockwise->p, triangleClockwise));
  ASSERT_FALSE(Edge::isCounterclockwise(
    triangleClockwise->q, triangleClockwise));
  ASSERT_FALSE(Edge::isCounterclockwise(
    triangleClockwise->r, triangleClockwise));
}
//==============================================================================
/*!
* \brief Проверка поиска смежной грани и следующего ребра в пучке
*/
TEST(EdgeTest, ScanBunch)
{
  std::shared_ptr<Top> topI = std::make_shared<Top>(0.0, 0.0);
  std::shared_ptr<Top> topJ = std::make_shared<Top>(2.0, 0.0);
  std::shared_ptr<Top> topK = std::make_shared<Top>(2.0, 2.0);
  std::shared_ptr<Top> topL = std::make_shared<Top>(0.0, 2.0);

  std::shared_ptr<Edge> edgeIJ = std::make_shared<Edge>(topI, topJ);
  std::shared_ptr<Edge> edgeJK = std::make_shared<Edge>(topJ, topK);
  std::shared_ptr<Edge> edgeKI = std::make_shared<Edge>(topK, topI);
  std::shared_ptr<Edge> edgeKL = std::make_shared<Edge>(topK, topL);
  std::shared_ptr<Edge> edgeLI = std::make_shared<Edge>(topL, topI);

  // Строим две грани с общим ребром KI
  std::shared_ptr<Triangle> triangleFirst
    = std::make_shared<Triangle>(edgeIJ, edgeJK, edgeKI);
  std::shared_ptr<Triangle> triangleSecond
    = std::make_shared<Triangle>(edgeKI, edgeKL, edgeLI);

  // Заполняем для рёбер информацию о смежных гранях
  edgeIJ->left = triangleFirst;
  edgeJK->left = triangleFirst;
  edgeKI->left = triangleFirst;
  edgeKI->right = triangleSecond;
  edgeKL->left = triangleSecond;
  edgeLI->left = triangleSecond;

  std::shared_ptr<Triangle> adjacentTriangle;
  std::shared_ptr<Edge> nextEdge;

  // Ищем левую грань относительно общего ребра и I-й вершины
  Edge::scanBunch(edgeKI, topI, Geometry::Left, adjacentTriangle, nextEdge);

  ASSERT_TRUE(adjacentTriangle == triangleSecond);
  ASSERT_TRUE(nextEdge == edgeLI);

  // Ищем правую грань относительно общего ребра и I-й вершины
  Edge::scanBunch(edgeKI, topI, Geometry::Right, adjacentTriangle, nextEdge);

  ASSERT_TRUE(adjacentTriangle == triangleFirst);
  ASSERT_TRUE(nextEdge == edgeIJ);

  // Ищем левую грань относительно общего ребра и K-й вершины
  Edge::scanBunch(edgeKI, topK, Geometry::Left, adjacentTriangle, nextEdge);

  ASSERT_TRUE(adjacentTriangle == triangleFirst);
  ASSERT_TRUE(nextEdge == edgeJK);

  // Ищем правую грань относительно общего ребра и K-й вершины
  Edge::scanBunch(edgeKI, topK, Geometry::Right, adjacentTriangle, nextEdge);

  ASSERT_TRUE(adjacentTriangle == triangleSecond);
  ASSERT_TRUE(nextEdge == edgeKL);

  // Ищем левую (существующую) грань относительно граничного ребра
  Edge::scanBunch(edgeJK, topJ, Geometry::Left, adjacentTriangle, nextEdge);

  ASSERT_TRUE(adjacentTriangle == triangleFirst);
  ASSERT_TRUE(nextEdge == edgeIJ);

  // Ищем правую (несуществующую) грань относительно граничного ребра
  Edge::scanBunch(edgeJK, topJ, Geometry::Right, adjacentTriangle, nextEdge);

  ASSERT_TRUE(adjacentTriangle == nullptr);
  ASSERT_TRUE(nextEdge == nullptr);
}
//==============================================================================
/*!
* \brief Проверка получения вершины ребра, отличающейся от заданной
*/
TEST(EdgeTest, GetOtherTop)
{
  std::shared_ptr<Top> topI = std::make_shared<Top>(0.0, 0.0);
  std::shared_ptr<Top> topJ = std::make_shared<Top>(2.0, 0.0);

  std::shared_ptr<Edge> edge = std::make_shared<Edge>(topI, topJ);

  std::shared_ptr<Top> top(edge->getOtherTop(topI));
  ASSERT_TRUE(top->isSame(topJ));

  top = edge->getOtherTop(topJ);
  ASSERT_TRUE(top->isSame(topI));
}
//==============================================================================
} //! Tests
} //! TriangularGeometry
//==============================================================================
