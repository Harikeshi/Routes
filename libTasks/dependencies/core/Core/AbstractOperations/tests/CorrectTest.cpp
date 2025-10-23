#include <AbstractOperations/Algorithms/Correct.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
//! Тесты для проверки правильности работы функции correct
//==============================================================================
namespace AbstractOperations
{
namespace Tests
{
//==============================================================================
TEST(CorrectTest, Correct)
{
  Point2D point;
  std::string mes ("smth");
  ASSERT_TRUE(correct(point, mes));
  ASSERT_EQ( mes, "" );
  point.set(1., NAN);
  ASSERT_FALSE(correct(point, mes ));
  ASSERT_EQ( mes, "Invalid Coordinate");

  MultiPoint2D points = { { 1., 0. },{ 1., 0. },{ 2., 0. } };
  ASSERT_TRUE(correct(points, mes ));
  ASSERT_EQ( mes, "" );
  ASSERT_EQ(3, points.size()); //!< дублирующие точки из коллекции не исключаются

  Linestring2D line = { { 1., 0. } };
  ASSERT_FALSE(isCorrect(line, mes ));
  ASSERT_FALSE( mes.find( "Too few points" ) == std::string::npos );
  ASSERT_FALSE(correct(line, mes ));
  ASSERT_FALSE( mes.find( "Too few points" ) == std::string::npos );
  line = { { 1., 0. },{ 2., 0. } };
  ASSERT_TRUE(isCorrect(line, mes ));
  ASSERT_EQ( mes, "" );
  ASSERT_TRUE(correct(line, mes ));
  ASSERT_EQ( mes, "" );
  // Дублирующие точки не делают геометрию некорректной, но исключаются
  // при исправлении геометрии
  line = { { 1., 0. },{ 1., 0. },{ 2., 0. } };
  ASSERT_TRUE(isCorrect(line, mes ));
  ASSERT_EQ( mes, "" );
  ASSERT_TRUE(correct(line, mes ));
  ASSERT_EQ( mes, "" );
  ASSERT_EQ(2, line.size());

  Box2D box(Point2D(2., 2.), Point2D(0., 0.));
  ASSERT_FALSE(isCorrect(box, mes));
  ASSERT_FALSE(mes.find("Wrong order") == std::string::npos);
  ASSERT_TRUE(correct(box, mes));
  ASSERT_EQ(mes, "");
  ASSERT_EQ(Point2D(0., 0.), box.min_corner());
  ASSERT_EQ(Point2D(2., 2.), box.max_corner());

  Ring2D ring = { { 1., 1. },{ 3., 1. },{ 1., 1. } };
  ASSERT_FALSE(isCorrect(ring, mes ));
  ASSERT_FALSE( mes.find( "Too few points" ) == std::string::npos );
  ASSERT_FALSE(correct(ring, mes ));
  ASSERT_FALSE( mes.find( "Too few points" ) == std::string::npos );
  ring = { { 1., 1. },{ 3., 1. },{ 3., 1. },{ 3., 3. } };
  ASSERT_FALSE(isCorrect(ring, mes));
  ASSERT_FALSE( mes.find("Ring must be closed") == std::string::npos );
  ASSERT_TRUE(correct(ring, mes ));
  ASSERT_EQ(mes, "");
  ASSERT_EQ(4, ring.size());
  ASSERT_EQ(Point2D(1., 1.), ring.at(0));
  ASSERT_EQ(Point2D(3., 3.), ring.at(1));
  ASSERT_EQ(Point2D(3., 1.), ring.at(2));
  ASSERT_EQ(Point2D(1., 1.), ring.at(3));

  Polygon2D polygon;
  ASSERT_TRUE(correct(polygon, mes ));
  ASSERT_EQ( mes, "" );
  polygon = { { { 1., 1. },{ 3., 1. },{ 3., 1. },{ 3., 3. } } };
  ASSERT_FALSE(isCorrect(polygon, mes ));
  ASSERT_FALSE(mes.find("Ring must be closed") == std::string::npos);
  ASSERT_TRUE(correct(polygon, mes ));
  ASSERT_EQ(mes, "");
  ASSERT_EQ(4, polygon.outer().size());
  ASSERT_EQ(Point2D(1., 1.), polygon.outer().at(0));
  ASSERT_EQ(Point2D(3., 3.), polygon.outer().at(1));
  ASSERT_EQ(Point2D(3., 1.), polygon.outer().at(2));
  ASSERT_EQ(Point2D(1., 1.), polygon.outer().at(3));

  MultiPolygon2D 
  mpolygon = { { { { 0., 0. },{ 1., 2. },{ 2., 0. },{ 0., 0. } } },
               { { { 0., 3. },{ 2., 3. },{ 1., 1. },{ 0., 3. } } } };
  ASSERT_FALSE(isCorrect(mpolygon, mes ));
  ASSERT_FALSE( mes.find( "Self-intersection" ) == std::string::npos );

  //! Исправление пересечения полигонов
  //! (общая часть будет удалена у обеих геометрий)
  ASSERT_TRUE(correct(mpolygon, mes ));
  ASSERT_EQ( mes, "");
  ASSERT_EQ(Point2D(2., 3.), mpolygon.at(0).outer().at(0));
  ASSERT_EQ(Point2D(1.25, 1.5), mpolygon.at(0).outer().at(1));
  ASSERT_EQ(Point2D(1., 2.), mpolygon.at(0).outer().at(2));
  ASSERT_EQ(Point2D(0.75, 1.5), mpolygon.at(0).outer().at(3));
  ASSERT_EQ(Point2D(0., 3.), mpolygon.at(0).outer().at(4));
  ASSERT_EQ(Point2D(2., 3.), mpolygon.at(0).outer().at(5));

  ASSERT_EQ(Point2D(2., 0.), mpolygon.at(1).outer().at(0));
  ASSERT_EQ(Point2D(0., 0.), mpolygon.at(1).outer().at(1));
  ASSERT_EQ(Point2D(0.75, 1.5), mpolygon.at(1).outer().at(2));
  ASSERT_EQ(Point2D(1., 1.), mpolygon.at(1).outer().at(3));
  ASSERT_EQ(Point2D(1.25, 1.5), mpolygon.at(1).outer().at(4));
  ASSERT_EQ(Point2D(2., 0.), mpolygon.at(1).outer().at(5));

  mpolygon = { { { { 0., 0. },{ 1., 2. },{ 2., 0. },{ 0., 0. } },
                 { { 0., 3. },{ 2., 3. },{ 1., 1. },{ 0., 3. } } } };
  ASSERT_FALSE(isCorrect(mpolygon, mes ));
  ASSERT_FALSE( mes.find( "Self-intersection" ) == std::string::npos );

  //! Исправление внутреннего кольца за пределами внешнего
  //! (общая часть будет удалена, остальная часть внутреннего кольца станет
  //! отдельным полигоном)
  ASSERT_TRUE(correct(mpolygon, mes ));
  ASSERT_EQ(mes, "");
  ASSERT_EQ(Point2D(2., 3.), mpolygon.at(0).outer().at(0));
  ASSERT_EQ(Point2D(1.25, 1.5), mpolygon.at(0).outer().at(1));
  ASSERT_EQ(Point2D(1., 2.), mpolygon.at(0).outer().at(2));
  ASSERT_EQ(Point2D(0.75, 1.5), mpolygon.at(0).outer().at(3));
  ASSERT_EQ(Point2D(0., 3.), mpolygon.at(0).outer().at(4));
  ASSERT_EQ(Point2D(2., 3.), mpolygon.at(0).outer().at(5));

  ASSERT_EQ(Point2D(2., 0.), mpolygon.at(1).outer().at(0));
  ASSERT_EQ(Point2D(0., 0.), mpolygon.at(1).outer().at(1));
  ASSERT_EQ(Point2D(0.75, 1.5), mpolygon.at(1).outer().at(2));
  ASSERT_EQ(Point2D(1., 1.), mpolygon.at(1).outer().at(3));
  ASSERT_EQ(Point2D(1.25, 1.5), mpolygon.at(1).outer().at(4));
  ASSERT_EQ(Point2D(2., 0.), mpolygon.at(1).outer().at(5));

  //! Исправление самопересечения геометрии (бантик)
  //! Полигон исправить не получится, так как в результате исправления
  //! получится мультиполигон
  polygon = { { { 0., 0. },{ 1., 1. },{ 1., -1. },{ 0., 0. },
                { -1., 1. },{ -1., -1. },{ 0., 0. } } };
  ASSERT_FALSE(isCorrect(polygon, mes));
  ASSERT_FALSE(mes.find("Self-intersection") == std::string::npos);
  ASSERT_FALSE(correct(polygon, mes));
  ASSERT_EQ(mes,
            "Corrected geometry has type different from source geometry type");

  //! Для мультиполигона исправление самопересечения возможно
  mpolygon = { polygon };
  ASSERT_FALSE(isCorrect(mpolygon, mes));
  ASSERT_FALSE(mes.find("Self-intersection") == std::string::npos);
  ASSERT_TRUE(correct(mpolygon, mes));
  ASSERT_EQ(mes, "");

  ASSERT_EQ(Point2D(-1., 1.), mpolygon.at(0).outer().at(0));
  ASSERT_EQ(Point2D(0., 0.), mpolygon.at(0).outer().at(1));
  ASSERT_EQ(Point2D(-1., -1.), mpolygon.at(0).outer().at(2));
  ASSERT_EQ(Point2D(-1., 1.), mpolygon.at(0).outer().at(3));

  ASSERT_EQ(Point2D(1., -1.), mpolygon.at(1).outer().at(0));
  ASSERT_EQ(Point2D(0., 0.), mpolygon.at(1).outer().at(1));
  ASSERT_EQ(Point2D(1., 1.), mpolygon.at(1).outer().at(2));
  ASSERT_EQ(Point2D(1., -1.), mpolygon.at(1).outer().at(3));
}
//==============================================================================
} //! Tests
} //! AbstractOperations
