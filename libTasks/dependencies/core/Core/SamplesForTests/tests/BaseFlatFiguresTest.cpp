#include <SamplesForTests/BaseFlatFigures.hpp>
#include <SamplesForTests/SamplesForTestsExceptions.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryConverter.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Correct.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace SamplesForTests
{
namespace Tests
{
//==============================================================================
//==============================================================================
TEST (BaseFlatFiguresTest, RectangleTest)
{
  Polygon2D rectangle;

  // обычный прямоугольник
  auto res ( buildRectangle(Point2D(1, 2), 3, 4) );
  ASSERT_EQ( res.outer().size(), 5);
  ASSERT_EQ( res.outer().at(0), Point2D(1, 2) );
  ASSERT_EQ( res.outer().at(1), Point2D(1, 6) );
  ASSERT_EQ( res.outer().at(2), Point2D(4, 6) );
  ASSERT_EQ( res.outer().at(3), Point2D(4, 2) );
  ASSERT_EQ( res.outer().at(4), res.outer().at(0) );

  // с направлением все хорошо
  std::string message;
  ASSERT_TRUE(AbstractOperations::isCorrect(res, message));
  ASSERT_TRUE(message.empty());

  // прямоугольник с нулевой стороной - пустая геометрия
  ASSERT_EQ(
    buildRectangle( Point2D( 1, 2 ), 0, 4 ).outer( ).size( ),
    0
    );

  // прямоугольник с отрицательной стороной генерирует исключение
  ASSERT_THROW( buildRectangle( Point2D( 1, 2 ), -3, 4 ),
                SamplesForTestsFailure);
  try
  {
    res = buildRectangle( Point2D( 1, 2 ), -3, 4 );
  }
  catch (const SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), NegativeDistance);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }
}
//==============================================================================
TEST( BaseFlatFiguresTest, BowTest)
{
  // обычный бантик (фигура с самопересечением)
  auto bow ( buildBow ( Point2D ( 1, 2 ), 3, 4 ) );

  ASSERT_EQ ( bow.outer().size(), 5 );
  ASSERT_EQ ( bow.outer().at(0), Point2D ( 1, 2 ) );
  ASSERT_EQ ( bow.outer().at(1), Point2D ( 1, 6 ) );
  ASSERT_EQ ( bow.outer().at(2), Point2D ( 4, 2 ) );
  ASSERT_EQ ( bow.outer().at(3), Point2D ( 4, 6 ) );
  ASSERT_EQ ( bow.outer().at(4), bow.outer().at(0) );

  std::string message;
  ASSERT_FALSE(AbstractOperations::isCorrect(bow, message));
  ASSERT_EQ(message, "Self-intersection");

  // противоположный контур тоже ошибочен
  bow.outer( ) = { 
    Point2D( 1, 2 ) , Point2D( 4, 6 ), Point2D( 4, 2 ), Point2D( 1, 6 ), 
    Point2D( 1, 2 ) };

  ASSERT_FALSE(AbstractOperations::isCorrect(bow, message));
  ASSERT_EQ(message, "Self-intersection");

  // фигура с нулевой стороной - пустая геометрия
  ASSERT_EQ(
    buildBow( Point2D( 1, 2 ), 0, 4 ).outer( ).size( ),
    0
    );

  // фигура с отрицательной стороной генерирует исключение
  ASSERT_THROW( buildBow( Point2D( 1, 2 ), -3, 4 ),
                SamplesForTestsFailure );
  try
  {
    bow = buildBow( Point2D( 1, 2 ), -3, 4 );
  }
  catch( const SamplesForTestsFailure& exception )
  {
    ASSERT_EQ( exception.getCode( ), NegativeDistance );
    ASSERT_EQ( sampForTestsLibTitle, exception.getLib( ) );
  }
}
//==============================================================================
TEST( BaseFlatFiguresTest, PostTest)
{
  // обычный конверт (фигура с самокасанием)
  auto post( buildPost( Point2D( 1, 2 ), 3, 4 ) );

  ASSERT_EQ( post.outer( ).size( ), 6 );
  ASSERT_EQ( post.outer( ).at( 0 ), Point2D( 1, 2 ) );
  ASSERT_EQ( post.outer( ).at( 1 ), Point2D( 1, 6 ) );
  ASSERT_EQ( post.outer( ).at( 2 ), Point2D( 2.5, 2 ) );
  ASSERT_EQ( post.outer( ).at( 3 ), Point2D( 4, 6 ) );
  ASSERT_EQ( post.outer( ).at( 4 ), Point2D( 4, 2 ) );
  ASSERT_EQ( post.outer( ).at( 5 ), post.outer( ).at( 0 ) );

  // самокасание - тоже ошибка
  std::string message;
  ASSERT_FALSE(AbstractOperations::isCorrect(post, message));
  ASSERT_EQ(message, "Ring Self-intersection");
  
  // фигура с нулевой стороной - пустая геометрия
  ASSERT_EQ(
    buildPost( Point2D( 1, 2 ), 0, 4 ).outer( ).size( ),
    0
    );

  // фигура с отрицательной стороной генерирует исключение
  ASSERT_THROW( buildPost( Point2D( 1, 2 ), -3, 4 ),
                SamplesForTestsFailure );
  try
  {
    post = buildPost( Point2D( 1, 2 ), -3, 4 );
  }
  catch( const SamplesForTestsFailure& exception )
  {
    ASSERT_EQ( exception.getCode( ), NegativeDistance );
    ASSERT_EQ( sampForTestsLibTitle, exception.getLib( ) );
  }
}
//==============================================================================
} //! Tests 
}//! SamplesForTests
//==============================================================================