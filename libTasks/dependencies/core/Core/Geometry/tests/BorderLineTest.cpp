#include <Geometry/Objects/BorderedLine.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
/*!
* \brief Тестирование равенства отрезков
*/
TEST(BorderedLineTest, isEqualBorderLine)
{
  BorderedLine bLine1(Point2D(2., 1.), Point2D(2., 4.));
  BorderedLine bLine2(Point2D(2., 1.), Point2D(2., 4.));

  ASSERT_TRUE(bLine1.isEqual(bLine2));

  bLine2 = BorderedLine(Point2D(2., 4.), Point2D(2., 1.));
  ASSERT_FALSE(bLine1.isEqual(bLine2));
}
//==============================================================================
/*!
* \brief Тестирование нахождения точки на отрезке
*/
TEST(BorderedLineTest, atBorderLine)
{
  BorderedLine bLine(Point2D(2., 1.), Point2D(2., 4.));

  ASSERT_TRUE(bLine.atBorderLine(Point2D(2., 1.)));
  ASSERT_TRUE(bLine.atBorderLine(Point2D(2., 4.)));
  ASSERT_TRUE(bLine.atBorderLine(Point2D(2., 3.)));
  ASSERT_FALSE(bLine.atBorderLine(Point2D(2., 0.)));
  ASSERT_FALSE(bLine.atBorderLine(Point2D(0., 0.)));
}
//==============================================================================
/*!
* \brief Тестирование нахождения точки на отрезке с заданной
*        погрешностью
*/
TEST(BorderedLineTest, nearBorderLine)
{
  BorderedLine bLine(Point2D(0., 0.), Point2D(4., 4.));

  ASSERT_TRUE(bLine.nearBorderLine(Point2D(0., 0.)));
  ASSERT_TRUE(bLine.nearBorderLine(Point2D(1., 1.)));
  ASSERT_TRUE(bLine.nearBorderLine(Point2D(4., 4.)));
  ASSERT_FALSE(bLine.nearBorderLine(Point2D(-1., -1.)));
  ASSERT_FALSE(bLine.nearBorderLine(Point2D(5., 5.)));

  // отклонение от линии в сторону допускается
  ASSERT_TRUE(bLine.nearBorderLine(Point2D(3.01, 3.), 0.02));
  ASSERT_TRUE(bLine.nearBorderLine(Point2D(2.99, 3.), 0.02));
  // отклонение за граничные значения отрезка не допускаеся 
  ASSERT_FALSE(bLine.nearBorderLine(Point2D(0., -0.01), 0.02));
  ASSERT_FALSE(bLine.nearBorderLine(Point2D(4., 4.01), 0.02));
  ASSERT_FALSE(bLine.nearBorderLine(Point2D(-0.01, 0.), 0.02));
  ASSERT_FALSE(bLine.nearBorderLine(Point2D(4.01, 4.), 0.02));
  // выход за пределы заданной погрешности
  ASSERT_FALSE(bLine.nearBorderLine(Point2D(3.01, 3.), 0.002));
  ASSERT_FALSE(bLine.nearBorderLine(Point2D(2.99, 3.), 0.002));

  // Проверка работы на отрезке, параллельном оси x
  BorderedLine bLineParallelX(Point2D(0., 4.), Point2D(4., 4.));

  // отклонение от линии в сторону допускается
  ASSERT_TRUE(bLineParallelX.nearBorderLine(Point2D(2., 3.99), 0.02));
  ASSERT_TRUE(bLineParallelX.nearBorderLine(Point2D(4., 4.01), 0.02));
  // отклонение за граничные значения отрезка не допускаеся 
  ASSERT_FALSE(bLineParallelX.nearBorderLine(Point2D(4.01, 4.), 0.02));
  ASSERT_FALSE(bLineParallelX.nearBorderLine(Point2D(-0.01, 4.), 0.02));
  // выход за пределы заданной погрешности
  ASSERT_FALSE(bLineParallelX.nearBorderLine(Point2D(2., 3.99), 0.002));
  ASSERT_FALSE(bLineParallelX.nearBorderLine(Point2D(4., 4.01), 0.002));

  // Проверка работы на отрезке, параллельном оси y
  BorderedLine bLineParallelY(Point2D(4., 0.), Point2D(4., 4.));

  // отклонение от линии в сторону допускается
  ASSERT_TRUE(bLineParallelY.nearBorderLine(Point2D(4.01, 4.), 0.02));
  ASSERT_TRUE(bLineParallelY.nearBorderLine(Point2D(3.99, 4.), 0.02));
  // отклонение за граничные значения отрезка не допускаеся 
  ASSERT_FALSE(bLineParallelY.nearBorderLine(Point2D(4., 4.01), 0.02));
  ASSERT_FALSE(bLineParallelY.nearBorderLine(Point2D(4., -0.01), 0.02));
  // выход за пределы заданной погрешности
  ASSERT_FALSE(bLineParallelY.nearBorderLine(Point2D(4.01, 4.), 0.002));
  ASSERT_FALSE(bLineParallelY.nearBorderLine(Point2D(3.99, 4.), 0.002));
}
//==============================================================================
/*!
* \brief Тестирование нахождения пересечения отрезков
*/
TEST(BorderedLineTest, isIntersectTest)
{
  // Отрезки совпадают в начальной точке
  BorderedLine bLine1(Point2D(2., 1.), Point2D(2., 4.));
  BorderedLine bLine2(Point2D(2., 1.), Point2D(3., 3.));

  ASSERT_TRUE(bLine1.isIntersect(bLine2));

  // Отрезки совпадают в конечной точке
  bLine2 = BorderedLine(Point2D(5., 3.), Point2D(2., 4.));
  ASSERT_TRUE(bLine1.isIntersect(bLine2));

  // Начало одного отрезка совпадает с концом другого
  bLine2 = BorderedLine(Point2D(2., 4.), Point2D(5., 3.));
  ASSERT_TRUE(bLine1.isIntersect(bLine2));

  // Отрезки пересекаются
  bLine2 = BorderedLine(Point2D(3., 3.), Point2D(1., 1.));
  ASSERT_TRUE(bLine1.isIntersect(bLine2));

  // Отрезки совпадают
  bLine2 = bLine1;
  ASSERT_TRUE(bLine1.isIntersect(bLine2));

  // Отрезки совпадают с точностью до направления
  bLine2 = BorderedLine(Point2D(2., 4.), Point2D(2., 1.));
  ASSERT_TRUE(bLine1.isIntersect(bLine2));

  // Отрезки лежат на одной линии и пересекаются
  bLine2 = BorderedLine(Point2D(2., 0.), Point2D(2., 2.));
  ASSERT_TRUE(bLine1.isIntersect(bLine2));

  // Один отрезок внутри другого
  bLine2 = BorderedLine(Point2D(2., 2.), Point2D(2., 3.));
  ASSERT_TRUE(bLine1.isIntersect(bLine2));

  bLine2 = BorderedLine(Point2D(2., 1.), Point2D(2., 3.));
  ASSERT_TRUE(bLine1.isIntersect(bLine2));

  bLine2 = BorderedLine(Point2D(2., 2.), Point2D(2., 4.));
  ASSERT_TRUE(bLine1.isIntersect(bLine2));

  // Отрезки лежат на одной линии и пересекаются в одной точке
  bLine2 = BorderedLine(Point2D(2., 0.), Point2D(2., 1.));
  ASSERT_TRUE(bLine1.isIntersect(bLine2));

  bLine2 = BorderedLine(Point2D(2., 5.), Point2D(2., 4.));
  ASSERT_TRUE(bLine1.isIntersect(bLine2));

  // Отрезки не пересекаются
  bLine2 = BorderedLine(Point2D(0., 0.), Point2D(0., 4.));
  ASSERT_FALSE(bLine1.isIntersect(bLine2));

  bLine2 = BorderedLine(Point2D(0., 0.), Point2D(1., 1.));
  ASSERT_FALSE(bLine1.isIntersect(bLine2));

  // Отрезки лежат на одной линии и не пересекаются
  bLine2 = BorderedLine(Point2D(2., 6.), Point2D(2., 5.));
  ASSERT_FALSE(bLine1.isIntersect(bLine2));
}
//==============================================================================
/*!
* \brief Тестирование нахождения пересечения отрезков не в концевых вершинах
*/
TEST(BorderedLineTest, intersecWithoutBorderTest)
{
  BorderedLine bLine1(Point2D(2., 1.), Point2D(2., 4.));
  BorderedLine bLine2(Point2D(2., 1.), Point2D(3., 3.));

  ASSERT_FALSE(bLine1.isIntersectWithoutBorder(bLine2));

  bLine2 = BorderedLine(Point2D(2., 1.), Point2D(2., 4.));
  ASSERT_TRUE(bLine1.isIntersectWithoutBorder(bLine2));
 
  bLine2 = BorderedLine(Point2D(2., 4.), Point2D(2., 1.));
  ASSERT_TRUE(bLine1.isIntersectWithoutBorder(bLine2));

  bLine2 = BorderedLine(Point2D(0., 0.), Point2D(2., 4.));
  ASSERT_FALSE(bLine1.isIntersectWithoutBorder(bLine2));

  bLine2 = BorderedLine(Point2D(2., 1.), Point2D(2., 3.));
  ASSERT_TRUE(bLine1.isIntersectWithoutBorder(bLine2));

  bLine2 = BorderedLine(Point2D(0., 0.), Point2D(4., 4.));
  ASSERT_TRUE(bLine1.isIntersectWithoutBorder(bLine2));

  bLine2 = BorderedLine(Point2D(2., 0.), Point2D(2., 1.));
  ASSERT_FALSE(bLine1.isIntersectWithoutBorder(bLine2));

  bLine2 = BorderedLine(Point2D(2., 5.), Point2D(2., 4.));
  ASSERT_FALSE(bLine1.isIntersectWithoutBorder(bLine2));

  bLine2 = BorderedLine(Point2D(0., 0.), Point2D(0., 4.));
  ASSERT_FALSE(bLine1.isIntersectWithoutBorder(bLine2));

  bLine2 = BorderedLine(Point2D(0., 0.), Point2D(1., 1.));
  ASSERT_FALSE(bLine1.isIntersectWithoutBorder(bLine2));

  bLine1 = BorderedLine(Point2D(0., 0.), Point2D(2., 2.));
  bLine2 = BorderedLine(Point2D(0., 0.), Point2D(1., 1.));
  ASSERT_TRUE(bLine1.isIntersectWithoutBorder(bLine2));

  bLine2 = BorderedLine(Point2D(1.5, 1.5), Point2D(1., 1.));
  ASSERT_TRUE(bLine1.isIntersectWithoutBorder(bLine2));

  bLine2 = BorderedLine(Point2D(3., 3.), Point2D(4., 4.));
  ASSERT_FALSE(bLine1.isIntersectWithoutBorder(bLine2));

  bLine2 = BorderedLine(Point2D(2., 2.), Point2D(3., 3.));
  ASSERT_FALSE(bLine1.isIntersectWithoutBorder(bLine2));
}
//==============================================================================
/*!
* \brief Тестирование определения положения точки относительно отрезка
*/
TEST(BorderedLineTest, positionTest)
{
  BorderedLine bLine1(Point2D(2., 1.), Point2D(2., 4.));
  Point2D p(2., 1.);
  ASSERT_EQ(bLine1.position(p), PlanePosition::Origin);

  p = Point2D(0., 2.);
  ASSERT_EQ(bLine1.position(p), PlanePosition::Left);

  p = Point2D(3., 2.);
  ASSERT_EQ(bLine1.position(p), PlanePosition::Right);

  p = Point2D(2., 5.);
  ASSERT_EQ(bLine1.position(p), PlanePosition::Beyond);

  p = Point2D(2., 0.);
  ASSERT_EQ(bLine1.position(p), PlanePosition::Behind);

  p = Point2D(2., 3.);
  ASSERT_EQ(bLine1.position(p), PlanePosition::Between);

  p = Point2D(2., 4.);
  ASSERT_EQ(bLine1.position(p), PlanePosition::Destination);
}
//==============================================================================
/*!
* \brief Тестирование поиска точки на прямой
*/
TEST(BorderedLineTest, findPointAtLineTest)
{
  BorderedLine line1(Point2D(1., 1.), Point2D(3., 3.));
  Point2D point(2., 2.);
  double dist = sqrt(2);
  ASSERT_EQ(line1.findPointAtLine(dist, true), point);

  point = Point2D(4., 4.);
  ASSERT_EQ(line1.findPointAtLine(dist, false), point);

  // Прямая, параллельная x
  BorderedLine line2(Point2D(1., 1.), Point2D(3., 1.));
  point = Point2D(2., 1.);
  dist = 1.;
  ASSERT_EQ(line2.findPointAtLine(dist, true), point);

  point = Point2D(4., 1.);
  ASSERT_EQ(line2.findPointAtLine(dist, false), point);

  // Прямая, параллельная y
  BorderedLine line3(Point2D(1., 1.), Point2D(1., 3.));
  point = Point2D(1., 2.);
  ASSERT_EQ(line3.findPointAtLine(dist, true), point);

  point = Point2D(1., 4.);
  ASSERT_EQ(line3.findPointAtLine(dist, false), point);
}
//==============================================================================
/*!
* \brief Тестирование поиска точки на прямой, перпендикулярной заданной
*/
TEST(BorderedLineTest, findPointAtPerpendLineTest)
{
  BorderedLine line1(Point2D(1., 1.), Point2D(3., 3.));
  Point2D crossPoint1(2., 2.);
  Point2D leftPoint1(1., 3.);
  Point2D rigthPoint1(3., 1.);
  // Перпендикуляр в начале
  Point2D crossPoint2(1., 1.);
  Point2D leftPoint2(0., 2.);
  Point2D rigthPoint2(2., 0.);
  // Перпендикуляр в конце
  Point2D crossPoint3(3., 3.);
  Point2D leftPoint3(2., 4.);
  Point2D rigthPoint3(4., 2.);

  double dist = sqrt(2);

  ASSERT_EQ(line1.findPointAtPerpendLine(crossPoint1, dist, true), leftPoint1);
  ASSERT_EQ(line1.findPointAtPerpendLine(crossPoint1, dist, false), rigthPoint1);
  ASSERT_EQ(line1.findPointAtPerpendLine(crossPoint2, dist, true), leftPoint2);
  ASSERT_EQ(line1.findPointAtPerpendLine(crossPoint2, dist, false), rigthPoint2);
  ASSERT_EQ(line1.findPointAtPerpendLine(crossPoint3, dist, true), leftPoint3);
  ASSERT_EQ(line1.findPointAtPerpendLine(crossPoint3, dist, false), rigthPoint3);

  // Прямая, параллельная x
  BorderedLine line2(Point2D(1., 1.), Point2D(3., 1.));
  crossPoint1 = Point2D(2., 1.);
  leftPoint1 = Point2D(2., 2.);
  rigthPoint1 = Point2D(2., 0.);

  dist = 1.;
  ASSERT_EQ(line2.findPointAtPerpendLine(crossPoint1, dist, true), leftPoint1);
  ASSERT_EQ(line2.findPointAtPerpendLine(crossPoint1, dist, false), rigthPoint1);

  // Прямая, параллельная y
  BorderedLine line3(Point2D(1., 1.), Point2D(1., 3.));
  crossPoint1 = Point2D(1., 2.);
  leftPoint1 = Point2D(0., 2.);
  rigthPoint1 = Point2D(2., 2.);

  ASSERT_EQ(line3.findPointAtPerpendLine(crossPoint1, dist, true), leftPoint1);
  ASSERT_EQ(line3.findPointAtPerpendLine(crossPoint1, dist, false), rigthPoint1);
}
//==============================================================================
} // ! Tests
} // ! Geometry
