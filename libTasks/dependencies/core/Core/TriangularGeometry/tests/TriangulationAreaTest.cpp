#include <TriangularGeometry/TriangularArea.hpp>
//! IOTools
#include <IOTools/GeoJsonIO/GeoJsonIO.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace TriangularGeometry
{
namespace Tests
{
//==============================================================================
class TriangulationTest : public ::testing::Test
{
protected:
//==============================================================================
  void SetUp( )
  {
    // Создание полигона
    Polygon2D poly;
    poly.outer( ).emplace_back( Point2D( 0.0, 0.0 ) );
    poly.outer( ).emplace_back( Point2D( 0.0, 1.0 ) );
    poly.outer( ).emplace_back( Point2D( 1.0, 2.0 ) );
    poly.outer( ).emplace_back( Point2D( 0.0, 3.0 ) );
    poly.outer( ).emplace_back( Point2D( 0.0, 4.0 ) );
    poly.outer( ).emplace_back( Point2D( 5.0, 4.0 ) );
    poly.outer( ).emplace_back( Point2D( 5.0, 2.0 ) );
    poly.outer( ).emplace_back( Point2D( 5.0, 0.0 ) );
    poly.outer( ).emplace_back( Point2D( 0.0, 0.0 ) );

    poly.inners( ).emplace_back( std::initializer_list<Point2D>( ) );
    poly.inners( ).back( ).emplace_back( Point2D( 2.0, 1.0 ) );
    poly.inners( ).back( ).emplace_back( Point2D( 4.0, 1.0 ) );
    poly.inners( ).back( ).emplace_back( Point2D( 4.0, 3.0 ) );
    poly.inners( ).back( ).emplace_back( Point2D( 2.0, 3.0 ) );
    poly.inners( ).back( ).emplace_back( Point2D( 2.0, 1.0 ) );

    std::vector<Polygon2D> trArea;
    trArea.emplace_back( poly );
    // Формирование области триангуляции
    area = std::make_shared<TriangularArea>( trArea );
    // точность вычислений
    eps = 0.0000001;
    // Выполнение триангуляции
    triangles = area->getTriangles( );
  }
//==============================================================================
  void TearDown( )
  {
  }
//==============================================================================
  std::shared_ptr<TriangularGeometry::TriangularArea> area;
  std::vector<Polygon2D>                         triangles;
  double                                               eps;
};
//==============================================================================
// Проверка на выполнение триангуляции
TEST_F( TriangulationTest, TriangulationComplete )
{
  ASSERT_FALSE( triangles.size( ) == 0. );
}
//==============================================================================
// Проверка на правильное число треугольников
TEST_F( TriangulationTest, CountTringles )
{
  ASSERT_EQ( 12, triangles.size( ) );
}
//==============================================================================
// Проверка на вырожденные треугольники
TEST_F( TriangulationTest, SingularTriangles )
{
  bool isSingular( false );
  std::size_t countTriangles( triangles.size( ) );
  double det( 0.0 );
  double x1, x2, x3, y1, y2, y3;
  for( std::size_t i = 0; i < countTriangles; i++ )
  {
    x1 = triangles[i].outer( )[0].getX( );
    x2 = triangles[i].outer( )[1].getX( );
    x3 = triangles[i].outer( )[2].getX( );
    y1 = triangles[i].outer( )[0].getY( );
    y2 = triangles[i].outer( )[1].getY( );
    y3 = triangles[i].outer( )[2].getY( );
    det = ( x2 - x1 ) * ( y3 - y1 ) - ( x3 - x1 ) * ( y2 - y1 );
    if( BasicMath::isEqual( det, eps ) )
    {
      isSingular = true;
      break;
    }
  }
  ASSERT_FALSE( isSingular );
}
//==============================================================================
// Проверка триангуляции района, вызывавшего падение
TEST_F(TriangulationTest, ComplexAreaTriangulation)
{
  MultiPolygon2D complexArea = importMultiPolygonFromGeoJson(
    "./TestArea/area.geojson");

  // Выполнение триангуляции
  std::shared_ptr<TriangularGeometry::TriangularArea> complexTrArea
    = std::make_shared<TriangularArea>(complexArea);

  std::vector<Polygon2D> complexTriangles = complexTrArea->getTriangles();

  ASSERT_FALSE(complexTriangles.empty());
  ASSERT_EQ(392, complexTriangles.size());
}
//==============================================================================
} //! Tests
} //! TriangularGeometry
//==============================================================================