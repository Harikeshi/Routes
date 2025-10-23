#include <AbstractOperations/DeleteExcessVertices.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace AbstractOperations
{
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование функции,
 * удаляющей лишние вершины в кольце многоугольника.
 */
TEST(DeleteExcessVerticesTest, DeleteExcessVertices)
{
  //! Пустое кольцо.

  std::vector<Point2D> ring;

  ASSERT_FALSE(deleteExcessVertices(ring, true));
  ASSERT_TRUE(ring.empty());

  //! Кольцо-треугольник.

  ring = {
    Point2D(0., 0.),
    Point2D(-1., 1.),
    Point2D(1., 1.)
  };

  ASSERT_TRUE(deleteExcessVertices(ring, true));
  ASSERT_EQ(4, ring.size());
  ASSERT_EQ(Point2D(1., 1.), ring[0]);
  ASSERT_EQ(Point2D(-1., 1.), ring[1]);
  ASSERT_EQ(Point2D(0., 0.), ring[2]);
  ASSERT_EQ(ring[0], ring[3]);

  //! Кольцо, все вершины которого расположены на одной прямой.

  ring = {
    Point2D(0., 0.),
    Point2D(1., 0.),
    Point2D(2., 0.)
  };

  ASSERT_FALSE(deleteExcessVertices(ring, true));
  ASSERT_TRUE(ring.empty());

  //! Кольцо с замыкающей вершиной.

  ring = {
    Point2D(0., 0.),
    Point2D(0., 1.),
    Point2D(1., 1.),
    Point2D(1., 0.),
    Point2D(0., 0.)
  };
  std::vector<Point2D> ringCopy(ring);

  ASSERT_TRUE(deleteExcessVertices(ring, false));
  ASSERT_EQ(ringCopy.size(), ring.size());
  ASSERT_EQ(ringCopy, ring);

  //! Кольцо, три точки которого лежат на одной линии
  ring = {
    Point2D(0., 0.),
    Point2D(0., 1.),
    Point2D(0.5, 1.),
    Point2D(1., 1.),
    Point2D(1., 0.),
    Point2D(0., 0.)
  };

  ASSERT_TRUE(deleteExcessVertices(ring, false));
  ASSERT_EQ(5, ring.size());
  ASSERT_EQ(Point2D(0., 0.), ring[0]);
  ASSERT_EQ(Point2D(0., 1.), ring[1]);
  ASSERT_EQ(Point2D(1., 1.), ring[2]);
  ASSERT_EQ(Point2D(1., 0.), ring[3]);
  ASSERT_EQ(ring[0], ring[4]);

  //! Кольцо, четыре точки которого лежат на одной линии, две из которых
  //! совпадают
  ring = {
    Point2D(0., 0.),
    Point2D(0., 1.),
    Point2D(0.5, 1.),
    Point2D(0.5, 1.),
    Point2D(1., 1.),
    Point2D(1., 0.),
    Point2D(0., 0.)
  };

  ASSERT_TRUE(deleteExcessVertices(ring, false));
  ASSERT_EQ(5, ring.size());
  ASSERT_EQ(Point2D(0., 0.), ring[0]);
  ASSERT_EQ(Point2D(0., 1.), ring[1]);
  ASSERT_EQ(Point2D(1., 1.), ring[2]);
  ASSERT_EQ(Point2D(1., 0.), ring[3]);
  ASSERT_EQ(ring[0], ring[4]);

  //! Кольцо, одна вершина которого задублирована
  ring = {
    Point2D(0., 0.),
    Point2D(0., 1.),
    Point2D(0., 1.),
    Point2D(1., 1.),
    Point2D(1., 0.),
    Point2D(0., 0.)
  };

  ASSERT_TRUE(deleteExcessVertices(ring, false));
  ASSERT_EQ(5, ring.size());
  ASSERT_EQ(Point2D(0., 0.), ring[0]);
  ASSERT_EQ(Point2D(0., 1.), ring[1]);
  ASSERT_EQ(Point2D(1., 1.), ring[2]);
  ASSERT_EQ(Point2D(1., 0.), ring[3]);
  ASSERT_EQ(ring[0], ring[4]);

  //! Кольцо, у которого необходимо расчистить первую точку
  ring = {
    Point2D(0., 0.),
    Point2D(0., 0.),
    Point2D(0., 1.),
    Point2D(1., 1.),
    Point2D(1., 0.),
    Point2D(0., 0.)
  };

  ASSERT_TRUE(deleteExcessVertices(ring, false));
  ASSERT_EQ(5, ring.size());
  ASSERT_EQ(Point2D(0., 0.), ring[0]);
  ASSERT_EQ(Point2D(0., 1.), ring[1]);
  ASSERT_EQ(Point2D(1., 1.), ring[2]);
  ASSERT_EQ(Point2D(1., 0.), ring[3]);
  ASSERT_EQ(ring[0], ring[4]);

  //! Кольцо, у которого необходимо расчистить последнюю точку
  ring = {
    Point2D(0., 0.),
    Point2D(0., 1.),
    Point2D(1., 1.),
    Point2D(1., 0.),
    Point2D(0.5, 0.),
    Point2D(0., 0.)
  };

  ASSERT_TRUE(deleteExcessVertices(ring, false));
  ASSERT_EQ(5, ring.size());
  ASSERT_EQ(Point2D(0., 0.), ring[0]);
  ASSERT_EQ(Point2D(0., 1.), ring[1]);
  ASSERT_EQ(Point2D(1., 1.), ring[2]);
  ASSERT_EQ(Point2D(1., 0.), ring[3]);
  ASSERT_EQ(ring[0], ring[4]);

  //! Кольцо, у которого первая и последняя точки совпадают (кроме замыкания)
  ring = {
    Point2D(0., 0.),
    Point2D(0., 1.),
    Point2D(1., 1.),
    Point2D(1., 0.),
    Point2D(0., 0.),
    Point2D(0., 0.)
  };

  ASSERT_TRUE(deleteExcessVertices(ring, false));
  ASSERT_EQ(5, ring.size());
  ASSERT_EQ(Point2D(0., 1.), ring[0]);
  ASSERT_EQ(Point2D(1., 1.), ring[1]);
  ASSERT_EQ(Point2D(1., 0.), ring[2]);
  ASSERT_EQ(Point2D(0., 0.), ring[3]);
  ASSERT_EQ(ring[0], ring[4]);

  //! Некорректное кольцо-конвертик
  ring = { Point2D(1, 1), Point2D(-1, 1), Point2D(-1, -1), Point2D(1, -1),
           Point2D(1, 1), Point2D( 0, 0), Point2D(-1,  1), Point2D(1,  1) };

  deleteExcessVertices(ring);

  ASSERT_EQ(8, ring.size());
  ASSERT_EQ(Point2D( 1,  1), ring[0]);
  ASSERT_EQ(Point2D(-1,  1), ring[1]);
  ASSERT_EQ(Point2D(-1, -1), ring[2]);
  ASSERT_EQ(Point2D( 1, -1), ring[3]);
  ASSERT_EQ(Point2D( 1,  1), ring[4]);
  ASSERT_EQ(Point2D( 0,  0), ring[5]);
  ASSERT_EQ(Point2D(-1,  1), ring[6]);
  ASSERT_EQ(ring[0], ring[7]);

  //! Пустой многоугольник.

  Polygon2D polygon;

  ASSERT_FALSE(deleteExcessVertices(polygon));
  ASSERT_TRUE(polygon.outer().empty());
  ASSERT_TRUE(polygon.inners().empty());

  //! Многоугольник с пустым внутренним кольцом.

  polygon.outer() = {
    Point2D(0., 0.),
    Point2D(0., 1.),
    Point2D(1., 1.),
    Point2D(1., 0.),
    Point2D(0., 0.)
  };
  polygon.inners().emplace_back();

  ASSERT_FALSE(deleteExcessVertices(polygon));
  ASSERT_EQ(5, polygon.outer().size());
  ASSERT_EQ(Point2D(0., 0.), polygon.outer()[0]);
  ASSERT_EQ(Point2D(0., 1.), polygon.outer()[1]);
  ASSERT_EQ(Point2D(1., 1.), polygon.outer()[2]);
  ASSERT_EQ(Point2D(1., 0.), polygon.outer()[3]);
  ASSERT_EQ(Point2D(0., 0.), polygon.outer()[4]);
  ASSERT_TRUE(polygon.inners().empty());

  //! Множество многоугольников, состоящее из пустого многоугольника.

  MultiPolygon2D multiPolygon;
  multiPolygon.emplace_back();

  ASSERT_FALSE(deleteExcessVertices(multiPolygon));
  ASSERT_EQ(0, multiPolygon.size());
}  
//==============================================================================
} // ! Tests
} // ! Operations
//==============================================================================