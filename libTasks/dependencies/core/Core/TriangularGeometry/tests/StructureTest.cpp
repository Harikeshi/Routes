#include <Triangulation/Objects/Structure/Linkage.hpp>
#include <Triangulation/Objects/Structure/Link.hpp>
#include <Triangulation/Objects/Structure/Head.hpp>
#include <Triangulation/Objects/Top.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace TriangularGeometry
{
namespace Tests
{
//==============================================================================
class StructureTest : public ::testing::Test
{
protected:
  //==============================================================================
  void SetUp( )
  {
    head = Head::create( );
    top0 = std::make_shared<Top>( 0.0, 0.0 );
    top1 = std::make_shared<Top>( 1.0, 1.0 );
    top2 = std::make_shared<Top>( 2.0, 2.0 );
    top3 = std::make_shared<Top>( 3.0, 3.0 );
  }
  //==============================================================================
  void TearDown( )
  {
    head->unlock( );
  }
  //==============================================================================
  std::shared_ptr<Head> head;
  std::shared_ptr<Top>  top0;
  std::shared_ptr<Top>  top1;
  std::shared_ptr<Top>  top2;
  std::shared_ptr<Top>  top3;
};
//==============================================================================
TEST_F( StructureTest, NextLinkage )
{
  top1->into( head );
  top2->into( head );
  ASSERT_TRUE( top1->next( ) == top2 );
  ASSERT_TRUE( top2->next( ) == nullptr );
}
//==============================================================================
TEST_F( StructureTest, PreviousLinkage )
{
  top1->into( head );
  top2->into( head );
  ASSERT_TRUE( top2->previous( ) == top1 );
  ASSERT_TRUE( top1->previous( ) == nullptr );
}
//==============================================================================
TEST_F( StructureTest, EmptyHead )
{
  ASSERT_TRUE( head->isEmpty( ) );
  top1->into( head );
  ASSERT_FALSE( head->isEmpty( ) );
}
//==============================================================================
TEST_F( StructureTest, FirstHead )
{
  top1->into( head );
  top2->into( head );
  ASSERT_TRUE( head->first( ) == top1 );
  ASSERT_FALSE( head->first( ) == top2 );
  ASSERT_FALSE( head->first( ) == nullptr );
  top1->out( );
  top2->out( );
  ASSERT_TRUE( head->first( ) == nullptr );
}
//==============================================================================
TEST_F( StructureTest, LastHead )
{
  top1->into( head );
  top2->into( head );
  ASSERT_TRUE( head->last( ) == top2 );
  ASSERT_FALSE( head->last( ) == top1 );
  ASSERT_FALSE( head->last( ) == nullptr );
  top1->out( );
  top2->out( );
  ASSERT_TRUE( head->last( ) == nullptr );
}
//==============================================================================
TEST_F( StructureTest, CardinalHead )
{
  ASSERT_EQ( 0, head->cardinal( ) );
  top1->into( head );
  ASSERT_EQ( 1, head->cardinal( ) );
  top2->into( head );
  ASSERT_EQ( 2, head->cardinal( ) );
}
//==============================================================================
TEST_F( StructureTest, ClearHead )
{
  top1->into( head );
  top2->into( head );
  head->clear( );
  ASSERT_EQ( 0, head->cardinal( ) );
  ASSERT_TRUE( head->first( ) == nullptr );
  ASSERT_TRUE( head->last( ) == nullptr );
}
//==============================================================================
TEST_F( StructureTest, OutLink )
{
  top1->into( head );
  top2->into( head );
  top1->out( );
  ASSERT_EQ( 1, head->cardinal( ) );
  ASSERT_TRUE( head->first( ) == top2 );
  ASSERT_TRUE( head->last( ) == top2 );
}
//==============================================================================
TEST_F( StructureTest, FollowLink )
{
  top1->into( head );
  top3->into( head );
  top2->follow( top1 );
  ASSERT_EQ( 3, head->cardinal( ) );
  ASSERT_TRUE( top1->next( ) == top2 );
  ASSERT_TRUE( top2->next( ) == top3 );
  ASSERT_TRUE( top3->previous( ) == top2 );
  ASSERT_TRUE( top2->previous( ) == top1 );
}
//==============================================================================
TEST_F( StructureTest, PrecedeLink )
{
  top1->into( head );
  top3->into( head );
  top2->precede( top3 );
  ASSERT_EQ( 3, head->cardinal( ) );
  ASSERT_TRUE( top1->next( ) == top2 );
  ASSERT_TRUE( top2->next( ) == top3 );
  ASSERT_TRUE( top3->previous( ) == top2 );
  ASSERT_TRUE( top2->previous( ) == top1 );
}
//==============================================================================
TEST_F( StructureTest, IntoLink )
{
  top1->into( head );
  ASSERT_EQ( 1, head->cardinal( ) );
  ASSERT_TRUE( head->first( ) == top1 );
  ASSERT_TRUE( head->last( ) == top1 );
  top2->into( head );
  ASSERT_EQ( 2, head->cardinal( ) );
  ASSERT_TRUE( head->first( ) == top1 );
  ASSERT_TRUE( head->last( ) == top2 );
}
//==============================================================================
} //! Tests
} //! TriangularGeometry
//==============================================================================
