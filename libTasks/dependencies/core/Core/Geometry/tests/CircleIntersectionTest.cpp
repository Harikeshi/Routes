#include <Geometry/Operations/CircleIntersection.hpp>
#include <Geometry/Objects/Circle.hpp>
#include <Geometry/Objects/BorderedLine.hpp>

#include <Geometry/GeometryException.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование поиска пересечения окружности и линии
 */
TEST(CircleIntersectionTest, findLineAndCircleIntersection)
{
  Circle circle(Point2D(4., -2.), 3.);
  // Линии за пределами окружности
  Line line(Point2D(-1., -2.), Point2D(0., 0.));
  Line lineParallelX = Line(Point2D(1., 2.), Point2D(0., 2.));
  Line lineParallelY = Line(Point2D(0., 0.), Point2D(0., 1.));

  std::pair<std::shared_ptr<Point2D>, std::shared_ptr<Point2D>>
    intersectionPoints = findLineAndCircleIntersection(circle, line);
  ASSERT_EQ(intersectionPoints.first, nullptr);
  ASSERT_EQ(intersectionPoints.second, nullptr);

  intersectionPoints = findLineAndCircleIntersection(circle, lineParallelX);
  ASSERT_EQ(intersectionPoints.first, nullptr);
  ASSERT_EQ(intersectionPoints.second, nullptr);

  intersectionPoints = findLineAndCircleIntersection(circle, lineParallelY);
  ASSERT_EQ(intersectionPoints.first, nullptr);
  ASSERT_EQ(intersectionPoints.second, nullptr);

  // Линии, проходящие по касательной к окружности
  lineParallelX = Line(Point2D(0., 1.), Point2D(1., 1.));
  lineParallelY = Line(Point2D(1., 0.), Point2D(1., -1.));

  intersectionPoints = findLineAndCircleIntersection(circle, lineParallelX);
  ASSERT_EQ(*intersectionPoints.first, Point2D(4., 1.));
  ASSERT_EQ(intersectionPoints.second, nullptr);

  intersectionPoints = findLineAndCircleIntersection(circle, lineParallelY);
  ASSERT_EQ(*intersectionPoints.first, Point2D(1., -2.));
  ASSERT_EQ(intersectionPoints.second, nullptr);

  // Линии, пересекающие окружность в двух точках
  line = Line(Point2D(1., -2.), Point2D(4., 1.));
  Line line2(Point2D(4., 1.), Point2D(7., -2.));

  intersectionPoints = findLineAndCircleIntersection(circle, line);
  ASSERT_EQ(*intersectionPoints.first, Point2D(1., -2.));
  ASSERT_EQ(*intersectionPoints.second, Point2D(4., 1.));

  intersectionPoints = findLineAndCircleIntersection(circle, line2);
  ASSERT_EQ(*intersectionPoints.first, Point2D(4., 1.));
  ASSERT_EQ(*intersectionPoints.second, Point2D(7., -2.));

  // Те же линии в обратном направлении
  line = Line(Point2D(4., 1.), Point2D(1., -2.));
  line2 = Line(Point2D(7., -2.), Point2D(4., 1.));

  intersectionPoints = findLineAndCircleIntersection(circle, line);
  ASSERT_EQ(*intersectionPoints.first, Point2D(4., 1.));
  ASSERT_EQ(*intersectionPoints.second, Point2D(1., -2.));

  intersectionPoints = findLineAndCircleIntersection(circle, line2);
  ASSERT_EQ(*intersectionPoints.first, Point2D(7., -2.));
  ASSERT_EQ(*intersectionPoints.second, Point2D(4., 1.));

  // Линии, проходящие через центр окружности
  lineParallelX = Line(Point2D(4., -2.), Point2D(5., -2.));
  lineParallelY = Line(Point2D(4., -2.), Point2D(4., -3.));

  intersectionPoints = findLineAndCircleIntersection(circle, lineParallelX);
  ASSERT_EQ(*intersectionPoints.first, Point2D(1., -2.));
  ASSERT_EQ(*intersectionPoints.second, Point2D(7., -2.));

  intersectionPoints = findLineAndCircleIntersection(circle, lineParallelY);
  ASSERT_EQ(*intersectionPoints.first, Point2D(4., 1.));
  ASSERT_EQ(*intersectionPoints.second, Point2D(4., -5.));
}
//==============================================================================
/*!
 * \brief Тестирование равенства отрезков
 */
TEST(CircleIntersectionTest, findBorderedLineAndCircleIntersection)
{
  // Окружность
  Circle circle(Point2D(4., 3.), 2.);

  // Отрезки, пересекающие окружность в одной точке
  BorderedLine firstLineWithOneIntersection(Point2D(7., 1.), Point2D(5., 3.));
  BorderedLine secondLineWithOneIntersection(Point2D(5., 3.), Point2D(2., 6.));

  // Отрезок, пересекающий окружность в двух точках
  BorderedLine lineWithTwoIntersections(Point2D(7., 1.), Point2D(2., 6.));

  // Отрезок, касающийся окружности
  BorderedLine tangentLine(Point2D(6., 0.), Point2D(6., 5.));

  // Отрезки, проходящий через центр окружности, параллельно осям
  BorderedLine lineParallelY(Point2D(4., 0.), Point2D(4., 5.));
  BorderedLine lineParallelX(Point2D(0., 3.), Point2D(7., 3.));

  // Отрезок, не пересекающий окружность
  // Сбоку от окружности
  BorderedLine lineAsideCircle(Point2D(8., 0.), Point2D(7., 1.));
  // Над окружностью
  BorderedLine lineOverCircle(Point2D(1., 10.), Point2D(7., 4.));
  // Внутри окружности
  BorderedLine lineInCircle(Point2D(4., 2.), Point2D(4., 4.));

  std::pair<std::shared_ptr<Point2D>, std::shared_ptr<Point2D>>
    intersectionPoints = findBorderedLineAndCircleIntersection(
    circle, firstLineWithOneIntersection);
  ASSERT_TRUE(circle.isOnCircle(*intersectionPoints.first, ACCURACY_DOUBLE));
  ASSERT_TRUE(firstLineWithOneIntersection.atBorderLine(
    *intersectionPoints.first));
  ASSERT_EQ(intersectionPoints.second, nullptr);

  intersectionPoints = findBorderedLineAndCircleIntersection(
    circle, secondLineWithOneIntersection);
  ASSERT_TRUE(circle.isOnCircle(*intersectionPoints.first, ACCURACY_DOUBLE));
  ASSERT_TRUE(secondLineWithOneIntersection.atBorderLine(
    *intersectionPoints.first));
  ASSERT_EQ(intersectionPoints.second, nullptr);

  // Найдём две точки пересечения
  intersectionPoints = findBorderedLineAndCircleIntersection(
    circle, lineWithTwoIntersections);
  // Первая точка принадлежит первому, но не второму отрезку
  ASSERT_TRUE(circle.isOnCircle(*intersectionPoints.first, ACCURACY_DOUBLE));
  ASSERT_TRUE(lineWithTwoIntersections.atBorderLine(
    *intersectionPoints.first));
  ASSERT_TRUE(firstLineWithOneIntersection.atBorderLine(
    *intersectionPoints.first));
  ASSERT_FALSE(secondLineWithOneIntersection.atBorderLine(
    *intersectionPoints.first));

  // Вторая точка принадлежит второму, но не первому отрезку
  ASSERT_TRUE(circle.isOnCircle(*intersectionPoints.second, ACCURACY_DOUBLE));
  ASSERT_TRUE(lineWithTwoIntersections.atBorderLine(
    *intersectionPoints.second));
  ASSERT_TRUE(secondLineWithOneIntersection.atBorderLine(
    *intersectionPoints.second));
  ASSERT_FALSE(firstLineWithOneIntersection.atBorderLine(
    *intersectionPoints.second));
  
  // Найдём точку касания
  intersectionPoints
    = findBorderedLineAndCircleIntersection(circle, tangentLine);
  ASSERT_TRUE(circle.isOnCircle(*intersectionPoints.first, ACCURACY_DOUBLE));
  ASSERT_TRUE(tangentLine.atBorderLine(*intersectionPoints.first));
  ASSERT_EQ(*intersectionPoints.first, Point2D(6., 3.));
  ASSERT_EQ(intersectionPoints.second, nullptr);

  // Пересечение окружности и отрезков, проходящих через её центр параллельно
  // осям
  intersectionPoints
    = findBorderedLineAndCircleIntersection(circle, lineParallelY);
  ASSERT_TRUE(circle.isOnCircle(*intersectionPoints.first, ACCURACY_DOUBLE));
  ASSERT_TRUE(lineParallelY.atBorderLine(*intersectionPoints.first));
  ASSERT_EQ(*intersectionPoints.first, Point2D(4., 1.));

  ASSERT_TRUE(circle.isOnCircle(*intersectionPoints.second, ACCURACY_DOUBLE));
  ASSERT_TRUE(lineParallelY.atBorderLine(*intersectionPoints.second));
  ASSERT_EQ(*intersectionPoints.second, Point2D(4., 5.));

  intersectionPoints
    = findBorderedLineAndCircleIntersection(circle, lineParallelX);
  ASSERT_TRUE(circle.isOnCircle(*intersectionPoints.first, ACCURACY_DOUBLE));
  ASSERT_TRUE(lineParallelX.atBorderLine(*intersectionPoints.first));
  ASSERT_EQ(*intersectionPoints.first, Point2D(2., 3.));

  ASSERT_TRUE(circle.isOnCircle(*intersectionPoints.second, ACCURACY_DOUBLE));
  ASSERT_TRUE(lineParallelX.atBorderLine(*intersectionPoints.second));
  ASSERT_EQ(*intersectionPoints.second, Point2D(6., 3.));

  // Если отрезок поменяет направление, то и найденные точки поменяют
  // направление
  lineParallelX = BorderedLine(Point2D(7., 3.), Point2D(0., 3.));

  intersectionPoints
    = findBorderedLineAndCircleIntersection(circle, lineParallelX);
  ASSERT_TRUE(circle.isOnCircle(*intersectionPoints.first, ACCURACY_DOUBLE));
  ASSERT_TRUE(lineParallelX.atBorderLine(*intersectionPoints.first));
  ASSERT_EQ(*intersectionPoints.first, Point2D(6., 3.));

  ASSERT_TRUE(circle.isOnCircle(*intersectionPoints.second, ACCURACY_DOUBLE));
  ASSERT_TRUE(lineParallelX.atBorderLine(*intersectionPoints.second));
  ASSERT_EQ(*intersectionPoints.second, Point2D(2., 3.));

  // Если нет касания, обе точки будут пусты

  intersectionPoints
    = findBorderedLineAndCircleIntersection(circle, lineAsideCircle);
  ASSERT_EQ(intersectionPoints.first, nullptr);
  ASSERT_EQ(intersectionPoints.second, nullptr);

  intersectionPoints
    = findBorderedLineAndCircleIntersection(circle, lineOverCircle);
  ASSERT_EQ(intersectionPoints.first, nullptr);
  ASSERT_EQ(intersectionPoints.second, nullptr);

  intersectionPoints
    = findBorderedLineAndCircleIntersection(circle, lineInCircle);
  ASSERT_EQ(intersectionPoints.first, nullptr);
  ASSERT_EQ(intersectionPoints.second, nullptr);
}
//==============================================================================
} // ! Tests
} // ! Geometry
