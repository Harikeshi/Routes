#include <Triangulation/Objects/Triangle.hpp>

#include <Triangulation/Objects/Edge.hpp>
#include <Triangulation/Objects/Top.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace TriangularGeometry
{
namespace Tests
{
//==============================================================================
class TriangleTest : public ::testing::Test
{
protected:
  //==============================================================================
  void SetUp( )
  {
    topI = std::make_shared<Top>( 0.0, 0.0 );
    topJ = std::make_shared<Top>( 2.0, 4.0 );
    topK = std::make_shared<Top>( 4.0, 0.0 );
    edgeP = std::make_shared<Edge>( topI, topJ );
    edgeQ = std::make_shared<Edge>( topJ, topK );
    edgeR = std::make_shared<Edge>( topK, topI );
    tr = std::make_shared<Triangle>( edgeP, edgeQ, edgeR );
    std::shared_ptr<Top> topK1 = std::make_shared<Top>( 0.0, 2.0 );
    std::shared_ptr<Edge> edgeQ1 = std::make_shared<Edge>( topJ, topK1 );
    std::shared_ptr<Edge> edgeR1 = std::make_shared<Edge>( topK1, topI );
    tr1 = std::make_shared<Triangle>( edgeP, edgeQ1, edgeR1 );
    std::shared_ptr<Top> topI2 = std::make_shared<Top>( 4.0, 2.0 );
    std::shared_ptr<Edge> edgeP2 = std::make_shared<Edge>( topI2, topJ );
    std::shared_ptr<Edge> edgeR2 = std::make_shared<Edge>( topK, topI2 );
    tr2 = std::make_shared<Triangle>( edgeP2, edgeQ, edgeR2 );
    std::shared_ptr<Top> topJ3 = std::make_shared<Top>( 0.0, -1.0 );
    std::shared_ptr<Edge> edgeP3 = std::make_shared<Edge>( topI, topJ3 );
    std::shared_ptr<Edge> edgeQ3 = std::make_shared<Edge>( topJ3, topK );
    tr3 = std::make_shared<Triangle>( edgeP3, edgeQ3, edgeR );
    tr->p->right = tr;
    tr->p->left = tr1;
    tr->q->right = tr;
    tr->q->left = tr2;
    tr->r->right = tr;
    tr->r->left = tr3;
  }
  //==============================================================================
  void TearDown( )
  {
    tr->disTry( );
  }
  //==============================================================================
  std::shared_ptr<Triangle> tr;
  std::shared_ptr<Triangle> tr1;
  std::shared_ptr<Triangle> tr2;
  std::shared_ptr<Triangle> tr3;
  std::shared_ptr<Top>      topI;
  std::shared_ptr<Top>      topJ;
  std::shared_ptr<Top>      topK;
  std::shared_ptr<Edge>     edgeP;
  std::shared_ptr<Edge>     edgeQ;
  std::shared_ptr<Edge>     edgeR;
};
//==============================================================================
/*!
* \brief Проверка на создание треугольника и поиск центра и радиуса описывающей
* \      окружности при создании
*/
TEST_F( TriangleTest, CreateTriangle )
{
  std::shared_ptr<Triangle> triangle
    = std::make_shared<Triangle>( edgeP, edgeQ, edgeR );
  ASSERT_TRUE( triangle->p == edgeP );
  ASSERT_TRUE( triangle->q == edgeQ );
  ASSERT_TRUE( triangle->r == edgeR );
  ASSERT_TRUE( triangle->pCenter == Point2D( 2., 1.5 ) );
  ASSERT_TRUE( BasicMath::isEqual( triangle->radius, 2.5 ) );
  ASSERT_TRUE( triangle->finit );
  ASSERT_FALSE( triangle->met );
  ASSERT_EQ( std::numeric_limits<uint32_t>::max( ), triangle->id );
}
//==============================================================================
/*!
* \brief Проверка копирующего конструктора
*/
TEST_F( TriangleTest, CopyTriangle )
{
  std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>( tr );
  ASSERT_TRUE( triangle->p == tr->p );
  ASSERT_TRUE( triangle->q == tr->q );
  ASSERT_TRUE( triangle->r == tr->r );
  ASSERT_TRUE( triangle->pCenter == tr->pCenter );
  ASSERT_TRUE( BasicMath::isEqual( triangle->radius, tr->radius ) );
  ASSERT_TRUE( triangle->finit == tr->finit );
  ASSERT_TRUE( triangle->met == tr->met );
  ASSERT_TRUE( triangle->id == tr->id );
}
//==============================================================================
/*!
* \brief Проверка на отцепление ребер от треугольника
*/
TEST_F( TriangleTest, DisTryTriangle )
{
  std::shared_ptr<Edge> p = tr->p;
  std::shared_ptr<Edge> q = tr->q;
  std::shared_ptr<Edge> r = tr->r;
  ASSERT_FALSE( ( p->left != tr ) && ( p->right != tr ) );
  ASSERT_FALSE( ( q->left != tr ) && ( q->right != tr ) );
  ASSERT_FALSE( ( r->left != tr ) && ( r->right != tr ) );
  tr->disTry( );
  ASSERT_TRUE( ( p->left != tr ) && ( p->right != tr ) );
  ASSERT_TRUE( ( q->left != tr ) && ( q->right != tr ) );
  ASSERT_TRUE( ( r->left != tr ) && ( r->right != tr ) );
}
//==============================================================================
/*!
* \brief Проверка на попадание точки в описывающую окружность треугольника и
* \      на определение правильного положения точки относительно ребра
*/
TEST_F( TriangleTest, TopInCircleTriangle )
{
  std::shared_ptr<Top> top11 = std::make_shared<Top>( 1.0, 1.0 );
  std::shared_ptr<Top> top2m1 = std::make_shared<Top>( 2.0, -1.0 );
  std::shared_ptr<Top> top91 = std::make_shared<Top>( 9.0, 1.0 );
  ASSERT_TRUE( tr->isTopInCircle( top11 ) );
  ASSERT_TRUE( tr->isTopInCircle( top2m1 ) );
  ASSERT_FALSE( tr->isTopInCircle( top91 ) );
}
//==============================================================================
/*!
* \brief Проверка на попадание точки в треугольник
*/
TEST_F( TriangleTest, TopInTriangle )
{
  std::shared_ptr<Top> top11 = std::make_shared<Top>( 1.0, 1.0 );
  std::shared_ptr<Top> top2m1 = std::make_shared<Top>( 2.0, -1.0 );
  std::shared_ptr<Top> top91 = std::make_shared<Top>( 9.0, 1.0 );
  ASSERT_TRUE( tr->isTopInTriangle( top11 ) );
  ASSERT_FALSE( tr->isTopInTriangle( top2m1 ) );
  ASSERT_FALSE( tr->isTopInTriangle( top91 ) );
}
//==============================================================================
TEST_F( TriangleTest, NearTriangle )
{
  std::shared_ptr<Triangle> nearTr1( tr1 );
  std::shared_ptr<Triangle> nearTr2( tr2 );
  std::shared_ptr<Triangle> nearTr3( tr3 );
  tr->nearTriangle( nearTr1, nearTr2, nearTr3 );
  bool isNear( false );
  if( nearTr1 == tr1 )
  {
    if( nearTr2 == tr2 )
    {
      if( nearTr3 == tr3 )
        isNear = true;
    }
    else if( nearTr2 == tr3 )
    {
      if( nearTr3 == tr2 )
        isNear = true;
    }
  }
  else if( nearTr1 == tr2 )
  {
    if( nearTr2 == tr1 )
    {
      if( nearTr3 == tr3 )
        isNear = true;
    }
    else if( nearTr2 == tr3 )
    {
      if( nearTr3 == tr1 )
        isNear = true;
    }
  }
  else if( nearTr1 == tr3 )
  {
    if( nearTr2 == tr1 )
    {
      if( nearTr3 == tr2 )
        isNear = true;
    }
    else if( nearTr2 == tr2 )
    {
      if( nearTr3 == tr1 )
        isNear = true;
    }
  }
  ASSERT_TRUE( isNear );
}
//==============================================================================
TEST_F( TriangleTest, HasTopTriangle )
{
  std::shared_ptr<Top> top11 = std::make_shared<Top>( 1.0, 1.0 );
  std::shared_ptr<Top> top00 = std::make_shared<Top>( 0.0, 0.0 );
  ASSERT_FALSE( tr->hasTopTriangle( top11 ) );
  ASSERT_FALSE( tr->hasTopTriangle( top00 ) );
  ASSERT_TRUE( tr->hasTopTriangle( topI ) );
}
//==============================================================================
TEST_F( TriangleTest, TopOppositeEdge )
{
  ASSERT_TRUE( tr->topOppositeEdge( tr->p ) == topK );
  ASSERT_TRUE( tr->topOppositeEdge( tr->q ) == topI );
  ASSERT_TRUE( tr->topOppositeEdge( tr->r ) == topJ );
}
//==============================================================================
TEST_F( TriangleTest, AllTops )
{
  std::shared_ptr<Top> i( nullptr );
  std::shared_ptr<Top> j( nullptr );
  std::shared_ptr<Top> k( nullptr );
  tr->allTops( i, j, k );
  bool isVerties( false );
  if( i == topI )
  {
    if( j == topJ )
    {
      if( k == topK )
        isVerties = true;
    }
    else if( j == topK )
    {
      if( k == topJ )
        isVerties = true;
    }
  }
  else if( i == topJ )
  {
    if( j == topI )
    {
      if( k == topK )
        isVerties = true;
    }
    else if( j == topK )
    {
      if( k == topI )
        isVerties = true;
    }
  }
  else if( i == topK )
  {
    if( j == topI )
    {
      if( k == topJ )
        isVerties = true;
    }
    else if( j == topJ )
    {
      if( k == topI )
        isVerties = true;
    }
  }
  ASSERT_TRUE( isVerties );
}
//==============================================================================
TEST_F( TriangleTest, EdgeOppositeTop )
{
  ASSERT_TRUE( tr->edgeOppositeTop( topI ) == tr->q );
  ASSERT_TRUE( tr->edgeOppositeTop( topJ ) == tr->r );
  ASSERT_TRUE( tr->edgeOppositeTop( topK ) == tr->p );
}
//==============================================================================
} //! Tests
} //! TriangularGeometry
//==============================================================================
