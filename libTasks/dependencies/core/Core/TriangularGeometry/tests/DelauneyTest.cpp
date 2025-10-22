#include <Triangulation/Delauney.hpp>
//! Structure
#include <Triangulation/Objects/Structure/Head.hpp>
//! Objects
#include <Triangulation/Objects/Edge.hpp>
#include <Triangulation/Objects/Top.hpp>
#include <Triangulation/Objects/Triangle.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace TriangularGeometry
{
namespace Tests
{
//==============================================================================
class DelauneyPrivate : public Delauney
{
  friend class DelauneyTest;
  FRIEND_TEST( DelauneyTest, TriangulateAreaDelauney );
  FRIEND_TEST( DelauneyTest, InsertDelauney );
  FRIEND_TEST( DelauneyTest, NestDelauney );
  FRIEND_TEST( DelauneyTest, DistroyNetDelauney );
  FRIEND_TEST( DelauneyTest, RecoveryNetDelauney );
  FRIEND_TEST( DelauneyTest, FinitTriangleDelauney );
  FRIEND_TEST( DelauneyTest, ClearTriangulationDelauney );
  FRIEND_TEST( DelauneyTest, TestOrientationDelauney );
  FRIEND_TEST( DelauneyTest, ClearingTriangulateAreaDelauney );
  FRIEND_TEST( DelauneyTest, QuickSortDelauney );
  FRIEND_TEST( DelauneyTest, SortByIdDelauney );
  FRIEND_TEST( DelauneyTest, ClearingIntoPolygonDelauney );
  FRIEND_TEST( DelauneyTest, FindEdgeDelauney );
  FRIEND_TEST( DelauneyTest, ClearingSurplusEdgesDelauney );
  FRIEND_TEST( DelauneyTest, ClearingNearTrianglesDelauney );
};
//==============================================================================
class DelauneyTest : public ::testing::Test
{
protected:
  //==============================================================================
  void SetUp( )
  {
    delauney = std::make_shared<Delauney>( );
    delauneyPrivate = std::make_shared<DelauneyPrivate>( );
    tops.push_back( std::make_shared<Top>( 0.0, 0.0 ) );
    tops.push_back( std::make_shared<Top>( 0.0, 4.0 ) );
    tops.push_back( std::make_shared<Top>( 4.0, 4.0 ) );
    tops.push_back( std::make_shared<Top>( 4.0, 0.0 ) );
    uint32_t countTops = static_cast< uint32_t >( tops.size( ) );
    for( uint32_t i = 0; i < countTops; i++ )
      tops[i]->id = i;
    polygons.push_back( std::make_pair( tops[0], tops[1] ) );
  }
  //============================================================================
  void TearDown( )
  {}
  //============================================================================
  std::shared_ptr<Delauney>                                     delauney;
  std::shared_ptr<DelauneyPrivate>                              delauneyPrivate;
  std::deque<std::shared_ptr<Top>>                                    tops;
  std::vector<std::pair<std::shared_ptr<Top>, std::shared_ptr<Top>>>  polygons;
};
//==============================================================================
TEST_F( DelauneyTest, CreateDelauney )
{
  ASSERT_TRUE( delauney->listEdge->isEmpty( ) );
  ASSERT_TRUE( delauney->listTrian->isEmpty( ) );
}
//==============================================================================
TEST_F( DelauneyTest, TriangulateAreaDelauney )
{
  delauneyPrivate->triangulateArea( tops );
  double minX( delauneyPrivate->mArea[0]->getX( ) );
  double minY( delauneyPrivate->mArea[0]->getY( ) );
  double maxX( minX );
  double maxY( minY );
  for( std::size_t i = 1; i < delauneyPrivate->mArea.size( ); i++ )
  {
    if( delauneyPrivate->mArea[i]->getX( ) < minX )
      minX = delauneyPrivate->mArea[i]->getX( );
    else if( delauneyPrivate->mArea[i]->getX( ) > maxX )
      maxX = delauneyPrivate->mArea[i]->getX( );
    if( delauneyPrivate->mArea[i]->getY( ) < minY )
      minY = delauneyPrivate->mArea[i]->getY( );
    else if( delauneyPrivate->mArea[i]->getY( ) > maxY )
      maxY = delauneyPrivate->mArea[i]->getY( );
  }
  ASSERT_EQ( 4, delauneyPrivate->mArea.size( ) );
  ASSERT_GT( tops[0]->getX( ), minX );
  ASSERT_GT( tops[0]->getY( ), minY );
  ASSERT_LT( tops[2]->getX( ), maxX );
  ASSERT_LT( tops[2]->getY( ), maxY );
  ASSERT_EQ( 5, delauneyPrivate->listEdge->cardinal( ) );
  ASSERT_EQ( 2, delauneyPrivate->listTrian->cardinal( ) );
}
//==============================================================================
TEST_F( DelauneyTest, InsertDelauney )
{
  delauneyPrivate->triangulateArea( tops );
  std::shared_ptr<Top> top11 = std::make_shared<Top>( 100.0, 100.0 );
  delauneyPrivate->include( tops[0] );
  ASSERT_EQ( 8, delauneyPrivate->listEdge->cardinal( ) );
  ASSERT_EQ( 4, delauneyPrivate->listTrian->cardinal( ) );
}
//==============================================================================
TEST_F( DelauneyTest, NestDelauney )
{
  delauneyPrivate->triangulateArea( tops );
  std::shared_ptr<Top> top11 = std::make_shared<Top>( 100.0, 100.0 );
  std::shared_ptr<Triangle> trT = delauneyPrivate->nest( tops[0] );
  std::shared_ptr<Triangle> trF = delauneyPrivate->nest( top11 );
  ASSERT_TRUE( trT != nullptr );
  ASSERT_TRUE( trF == nullptr );
}
//==============================================================================
TEST_F( DelauneyTest, DistroyNetDelauney )
{
  delauneyPrivate->triangulateArea( tops );
  std::shared_ptr<Triangle> tr = delauneyPrivate->nest( tops[0] );
  delauneyPrivate->distroyNet( tr, tops[0] );
  tr = std::dynamic_pointer_cast< Triangle >( delauneyPrivate->listTrian->first( ) );
  bool inCircle( false );
  while( tr )
  {
    if( tr->isTopInCircle( tops[0] ) )
    {
      inCircle = true;
      break;
    }
    tr = std::dynamic_pointer_cast< Triangle >( tr->next( ) );
  }
  ASSERT_FALSE( inCircle );
  ASSERT_GT( 2, delauneyPrivate->listEdge->cardinal( ) );
  ASSERT_GT( 5, delauneyPrivate->listTrian->cardinal( ) );
}
//==============================================================================
TEST_F( DelauneyTest, RecoveryNetDelauney )
{
  delauneyPrivate->triangulateArea( tops );
  std::shared_ptr<Triangle> tr = delauneyPrivate->nest( tops[0] );
  delauneyPrivate->distroyNet( tr, tops[0] );
  delauneyPrivate->recoveryNet( tops[0] );
  bool isDelauney( true );
  tr = std::dynamic_pointer_cast< Triangle >( delauneyPrivate->listTrian->first( ) );
  while( tr )
  {
    if( ( !tr->hasTopTriangle( tops[0] ) ) && ( tr->isTopInCircle( tops[0] ) ) )
    {
      isDelauney = false;
      break;
    }
    tr = std::dynamic_pointer_cast< Triangle >( tr->next( ) );
  }
  ASSERT_TRUE( isDelauney );
  ASSERT_EQ( 8, delauneyPrivate->listEdge->cardinal( ) );
  ASSERT_EQ( 4, delauneyPrivate->listTrian->cardinal( ) );
}
//==============================================================================
TEST_F( DelauneyTest, FinitTriangleDelauney )
{
  std::shared_ptr<Top> top00 = std::make_shared<Top>( 0.0, 0.0 );
  std::shared_ptr<Top> top24 = std::make_shared<Top>( 2.0, 4.0 );
  std::shared_ptr<Top> top40 = std::make_shared<Top>( 4.0, 0.0 );
  std::shared_ptr<Edge> p = std::make_shared<Edge>( top00, top40 );
  std::shared_ptr<Edge> q = std::make_shared<Edge>( top40, top24 );
  std::shared_ptr<Edge> r = std::make_shared<Edge>( top00, top24 );
  std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>( p, q, r );
  p->left = triangle;
  q->left = triangle;
  r->right = triangle;
  delauneyPrivate->finitTriangle( triangle );
  ASSERT_TRUE( triangle->finit );
  top00->id = std::numeric_limits<uint32_t>::max( );
  delauneyPrivate->finitTriangle( triangle );
  ASSERT_FALSE( triangle->finit );
  triangle->disTry( );
}
//==============================================================================
TEST_F( DelauneyTest, TriangulateDelauney )
{
  delauney->triangulate( tops );
  bool isDelauney( true );
  std::size_t countTops( tops.size( ) );
  std::shared_ptr<Triangle> tr( nullptr );
  for( std::size_t i = 0; i < countTops; i++ )
  {
    tr =
      std::dynamic_pointer_cast< Triangle >( delauneyPrivate->listTrian->first( ) );
    while( tr )
    {
      if( ( !tr->hasTopTriangle( tops[i] ) ) && ( tr->isTopInCircle( tops[i] ) ) )
      {
        isDelauney = false;
        break;
      }
      tr = std::dynamic_pointer_cast< Triangle >( tr->next( ) );
    }
    if( !isDelauney )
      break;
  }
  bool isSingular( false );
  tr = std::dynamic_pointer_cast< Triangle >( delauneyPrivate->listTrian->first( ) );
  std::shared_ptr<Top> i( nullptr );
  std::shared_ptr<Top> j( nullptr );
  std::shared_ptr<Top> k( nullptr );
  while( tr )
  {
    tr->allTops( i, j, k );
    if( Top::isLine( i, j, k ) )
    {
      isSingular = true;
      break;
    }
    tr = std::dynamic_pointer_cast< Triangle >( tr->next( ) );
  }
  ASSERT_TRUE( isDelauney );
  ASSERT_FALSE( isSingular );
  ASSERT_EQ( 17, delauney->listEdge->cardinal( ) );
  ASSERT_EQ( 10, delauney->listTrian->cardinal( ) );
}
//==============================================================================
TEST_F( DelauneyTest, ClearTriangulationDelauney )
{
  delauneyPrivate->triangulate( tops );
  delauneyPrivate->clearTriangulation( );
  ASSERT_TRUE( delauneyPrivate->listEdge->isEmpty( ) );
  ASSERT_TRUE( delauneyPrivate->listTrian->isEmpty( ) );
  ASSERT_EQ( 0, delauneyPrivate->mArea.size( ) );
}
//==============================================================================
TEST_F( DelauneyTest, ClearingBadEdgesDelauney )
{
  delauney->triangulate( tops );
  for( std::size_t i = 0; i < tops.size( ) - 1; i++ )
    delauney->addFixedEdge( tops[i], tops[i + 1] );
  delauney->addFixedEdge( tops[tops.size( ) - 1], tops[0] );
  delauney->clearingBadEdges( polygons );
  ASSERT_EQ( 5, delauney->listEdge->cardinal( ) );
  ASSERT_EQ( 2, delauney->listTrian->cardinal( ) );
}
//==============================================================================
TEST_F( DelauneyTest, TestOrientationDelauney )
{
  delauneyPrivate->triangulate( tops );
  for( std::size_t i = 0; i < tops.size( ) - 1; i++ )
    delauneyPrivate->addFixedEdge( tops[i], tops[i + 1] );
  delauneyPrivate->addFixedEdge( tops[tops.size( ) - 1], tops[0] );
  delauneyPrivate->clearingTriangulateArea( );
  delauneyPrivate->testOrientation( );
  std::shared_ptr<Edge> ed =
    std::dynamic_pointer_cast< Edge >( delauneyPrivate->listEdge->first( ) );
  std::shared_ptr<Triangle> tr( nullptr );
  std::shared_ptr<Top> top( nullptr );
  bool isFail( false );
  while( ed )
  {
    tr = ed->left;
    if( tr )
    {
      top = tr->topOppositeEdge( ed );
      if( ed->position( *top ) != Geometry::PlanePosition::Left )
      {
        isFail = true;
        break;
      }
    }
    else
    {
      tr = ed->right;
      if( tr )
      {
        top = tr->topOppositeEdge( ed );
        if( ed->position( *top ) != Geometry::PlanePosition::Right )
        {
          isFail = true;
          break;
        }
      }
    }
    ed = std::dynamic_pointer_cast< Edge >( ed->next( ) );
  }
  ASSERT_FALSE( isFail );
}
//==============================================================================
TEST_F( DelauneyTest, ClearingTriangulateAreaDelauney )
{
  delauneyPrivate->triangulate( tops );
  for( std::size_t i = 0; i < tops.size( ) - 1; i++ )
    delauneyPrivate->addFixedEdge( tops[i], tops[i + 1] );
  delauneyPrivate->addFixedEdge( tops[tops.size( ) - 1], tops[0] );
  delauneyPrivate->clearingTriangulateArea( );
  ASSERT_EQ( 5, delauneyPrivate->listEdge->cardinal( ) );
  ASSERT_EQ( 2, delauneyPrivate->listTrian->cardinal( ) );
}
//==============================================================================
TEST_F( DelauneyTest, AddFixedEdgeDelauney )
{
  delauney->triangulate( tops );
  for( std::size_t i = 0; i < tops.size( ) - 1; i++ )
    delauney->addFixedEdge( tops[i], tops[i + 1] );
  delauney->addFixedEdge( tops[tops.size( ) - 1], tops[0] );
  std::shared_ptr<Edge> ed =
    std::dynamic_pointer_cast< Edge >( delauney->listEdge->first( ) );
  int countFixEdge( 0 );
  bool isFail( false );
  while( ed )
  {
    if( ed->fixed )
    {
      countFixEdge++;
      bool inRange( false );
      for( std::size_t i = 0; i < tops.size( ) - 1; i++ )
      {
        if( ( ( ed->getI( ) == tops[i] ) && ( ed->getJ( ) == tops[i + 1] ) )
          || ( ( ed->getJ( ) == tops[i] ) && ( ed->getI( ) == tops[i + 1] ) ) )
        {
          inRange = true;
          break;
        }
      }
      if( !inRange )
      {
        if( ( ( ed->getI( ) == tops[0] ) && ( ed->getJ( ) == tops[tops.size( ) - 1] ) )
          || ( ( ed->getJ( ) == tops[0] ) && ( ed->getI( ) == tops[tops.size( ) - 1] ) ) )
          inRange = true;
      }
      if( !inRange )
        isFail = true;
    }
    ed = std::dynamic_pointer_cast< Edge >( ed->next( ) );
  }
  ASSERT_FALSE( isFail );
  ASSERT_EQ( 4, countFixEdge );
}
//==============================================================================
TEST_F( DelauneyTest, QuickSortDelauney )
{
  delauneyPrivate->quickSort( tops, 0, static_cast< int >( tops.size( ) ) - 1 );
  bool isAsc( true );
  for( std::size_t i = 1; i < tops.size( ); i++ )
  {
    if( tops[i - 1]->compare( tops[i] ) != Less )
    {
      isAsc = false;
      break;
    }
  }
  ASSERT_TRUE( isAsc );
}
//==============================================================================
TEST_F( DelauneyTest, SortByIdDelauney )
{
  std::vector<uint32_t> ids;
  for( std::size_t i = 0; i < tops.size( ); i++ )
    ids.push_back( tops[i]->id );
  delauneyPrivate->quickSort( tops, 0, static_cast< int >( tops.size( ) ) - 1 );
  delauneyPrivate->sortById( tops );
  bool isBase( true );
  for( std::size_t i = 0; i < tops.size( ); i++ )
  {
    if( tops[i]->id != ids[i] )
    {
      isBase = false;
      break;
    }
  }
  ASSERT_TRUE( isBase );
}
//==============================================================================
TEST_F( DelauneyTest, ClearingIntoPolygonDelauney )
{
  tops.push_back( std::make_shared<Top>( 1.0, 1.0 ) );
  tops.push_back( std::make_shared<Top>( 3.0, 1.0 ) );
  tops.push_back( std::make_shared<Top>( 3.0, 3.0 ) );
  tops.push_back( std::make_shared<Top>( 1.0, 3.0 ) );
  uint32_t countTops = static_cast< uint32_t >( tops.size( ) );
  for( uint32_t i = 0; i < countTops; i++ )
    tops[i]->id = i;
  polygons.push_back( std::make_pair( tops[4], tops[5] ) );
  delauneyPrivate->triangulate( tops );
  for( size_t i = 0; i < 2; i++ )
  {
    for( size_t j = 0; j < 3; j++ )
      delauneyPrivate->addFixedEdge( tops[4 * i + j], tops[4 * i + j + 1] );
    delauneyPrivate->addFixedEdge( tops[3 + 4 * i], tops[4 * i] );
  }
  delauneyPrivate->clearingTriangulateArea( );
  delauneyPrivate->testOrientation( );
  for( std::size_t i = 0; i < polygons.size( ); i++ )
    delauneyPrivate->clearingIntoPolygon( polygons[i].first, polygons[i].second );
  bool isClear( true );
  std::shared_ptr<Edge> ed =
    std::dynamic_pointer_cast< Edge >( delauneyPrivate->listEdge->first( ) );
  while( ed )
  {
    if( ( ( ed->getI( ) == tops[4] ) && ( ed->getJ( ) == tops[6] ) )
      || ( ( ed->getJ( ) == tops[4] ) && ( ed->getI( ) == tops[6] ) )
      || ( ( ed->getI( ) == tops[5] ) && ( ed->getJ( ) == tops[7] ) )
      || ( ( ed->getJ( ) == tops[5] ) && ( ed->getI( ) == tops[7] ) ) )
    {
      if( ( ed->left ) || ( ed->right ) )
        isClear = false;
      break;
    }
    ed = std::dynamic_pointer_cast< Edge >( ed->next( ) );
  }
  ASSERT_TRUE( isClear );
  ASSERT_EQ( 16, delauneyPrivate->listEdge->cardinal( ) );
  ASSERT_EQ( 8, delauneyPrivate->listTrian->cardinal( ) );
}
//==============================================================================
TEST_F( DelauneyTest, FindEdgeDelauney )
{
  delauneyPrivate->triangulate( tops );
  std::shared_ptr<Edge> ed1 = delauneyPrivate->findEdge( tops[0], tops[1] );
  std::shared_ptr<Top> top55 = std::make_shared<Top>( 5.0, 5.0 );
  ASSERT_TRUE( delauneyPrivate->findEdge( tops[0], tops[1] ) != nullptr );
  ASSERT_TRUE( delauneyPrivate->findEdge( tops[0], tops[1] ) == ed1 );
  ASSERT_TRUE( delauneyPrivate->findEdge( tops[0], top55 ) == nullptr );
}
//==============================================================================
TEST_F( DelauneyTest, ClearingSurplusEdgesDelauney )
{
  tops.push_back( std::make_shared<Top>( 1.0, 1.0 ) );
  tops.push_back( std::make_shared<Top>( 3.0, 1.0 ) );
  tops.push_back( std::make_shared<Top>( 3.0, 3.0 ) );
  tops.push_back( std::make_shared<Top>( 1.0, 3.0 ) );
  uint32_t countTops = static_cast< uint32_t >( tops.size( ) );
  for( uint32_t i = 0; i < countTops; i++ )
    tops[i]->id = i;
  polygons.push_back( std::make_pair( tops[4], tops[5] ) );
  delauneyPrivate->triangulate( tops );
  for( std::size_t i = 0; i < 2; i++ )
  {
    for( std::size_t j = 0; j < 3; j++ )
      delauneyPrivate->addFixedEdge( tops[4 * i + j], tops[4 * i + j + 1] );
    delauneyPrivate->addFixedEdge( tops[3 + 4 * i], tops[4 * i] );
  }
  delauneyPrivate->clearingTriangulateArea( );
  delauneyPrivate->testOrientation( );
  for( std::size_t i = 0; i < polygons.size( ); i++ )
    delauneyPrivate->clearingIntoPolygon( polygons[i].first, polygons[i].second );
  delauneyPrivate->clearingSurplusEdges( );
  bool isClear( true );
  std::shared_ptr<Edge> ed =
    std::dynamic_pointer_cast< Edge >( delauneyPrivate->listEdge->first( ) );
  while( ed )
  {
    if( ( ( ed->getI( ) == tops[4] ) && ( ed->getJ( ) == tops[6] ) )
      || ( ( ed->getJ( ) == tops[4] ) && ( ed->getI( ) == tops[6] ) )
      || ( ( ed->getI( ) == tops[5] ) && ( ed->getJ( ) == tops[7] ) )
      || ( ( ed->getJ( ) == tops[5] ) && ( ed->getI( ) == tops[7] ) ) )
    {
      isClear = false;
      break;
    }
    ed = std::dynamic_pointer_cast< Edge >( ed->next( ) );
  }
  ASSERT_TRUE( isClear );
  ASSERT_EQ( 16, delauneyPrivate->listEdge->cardinal( ) );
  ASSERT_EQ( 8, delauneyPrivate->listTrian->cardinal( ) );
  for( std::size_t i = 0; i < 4; i++ )
    tops.pop_back( );
}
//==============================================================================
TEST_F( DelauneyTest, ClearingNearTrianglesDelauney )
{
  tops.push_back( std::make_shared<Top>( 1.0, 1.0 ) );
  tops.push_back( std::make_shared<Top>( 3.0, 1.0 ) );
  tops.push_back( std::make_shared<Top>( 3.0, 3.0 ) );
  tops.push_back( std::make_shared<Top>( 1.0, 3.0 ) );
  uint32_t countTops = static_cast< uint32_t >( tops.size( ) );
  for( uint32_t i = 0; i < countTops; i++ )
    tops[i]->id = i;
  polygons.push_back( std::make_pair( tops[4], tops[5] ) );
  delauneyPrivate->triangulate( tops );
  for( std::size_t i = 0; i < 2; i++ )
  {
    for( std::size_t j = 0; j < 3; j++ )
      delauneyPrivate->addFixedEdge( tops[4 * i + j], tops[4 * i + j + 1] );
    delauneyPrivate->addFixedEdge( tops[3 + 4 * i], tops[4 * i] );
  }
  delauneyPrivate->clearingTriangulateArea( );
  delauneyPrivate->testOrientation( );
  std::shared_ptr<Edge> ed = delauneyPrivate->findEdge( tops[4], tops[5] );
  std::shared_ptr<Head> triangles = Head::create( );
  if( ed->getI( ) == tops[4] )
    ed->left->into( triangles );
  else
    ed->right->into( triangles );
  delauneyPrivate->clearingNearTriangles( triangles );

  // разлочил список
  triangles->unlock( );

  bool isClear( true );
  ed = std::dynamic_pointer_cast< Edge >( delauneyPrivate->listEdge->first( ) );
  while( ed )
  {
    if( ( ( ed->getI( ) == tops[4] ) && ( ed->getJ( ) == tops[6] ) )
      || ( ( ed->getJ( ) == tops[4] ) && ( ed->getI( ) == tops[6] ) )
      || ( ( ed->getI( ) == tops[5] ) && ( ed->getJ( ) == tops[7] ) )
      || ( ( ed->getJ( ) == tops[5] ) && ( ed->getI( ) == tops[7] ) ) )
    {
      if( ( ed->left ) || ( ed->right ) )
        isClear = false;
      break;
    }
    ed = std::dynamic_pointer_cast< Edge >( ed->next( ) );
  }
  ASSERT_TRUE( isClear );
  ASSERT_EQ( 8, delauneyPrivate->listTrian->cardinal( ) );
  for( std::size_t i = 0; i < 4; i++ )
    tops.pop_back( );
}
//==============================================================================
} //! Tests
} //! TriangularGeometry
//==============================================================================
