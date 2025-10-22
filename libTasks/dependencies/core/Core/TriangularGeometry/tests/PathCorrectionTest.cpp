//! TriangularGeometry
#include <TriangularGeometry/PathFinder.hpp>
//! Geometry
#include <Geometry/Objects/Circle.hpp>
#include <Geometry/Operations/PolygonToCircles.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Difference.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace TriangularGeometry
{
namespace Tests
{
//==============================================================================
class PathCorrectionTest : public ::testing::Test
{
protected:
//==============================================================================
  void SetUp( )
  {
    // Создание полигона
    Polygon2D poly({
      {
        Point2D(0.0, 0.0),
        Point2D(0.0, 10000.0),
        Point2D(10000.0, 10000.0),
        Point2D(10000.0, 0.0),
        Point2D(0.0, 0.0)
      },
      {
        Point2D(1000.0, 9000.0),
        Point2D(2000.0, 5000.0),
        Point2D(3000.0, 5000.0),
        Point2D(4000.0, 9000.0),
        Point2D(1000.0, 9000.0)
      },
      {
        Point2D(2500.0, 1000.0),
        Point2D(4500.0, 1000.0),
        Point2D(3500.0, 5500.0),
        Point2D(2500.0, 1000.0)
      },
      {
        Point2D(6000.0, 7000.0),
        Point2D(4000.0, 6000.0),
        Point2D(6200.0, 4600.0),
        Point2D(6000.0, 7000.0)
      },
      {
        Point2D(6200.0, 2400.0),
        Point2D(6200.0, 1400.0),
        Point2D(7000.0, 1800.0),
        Point2D(7800.0, 1400.0),
        Point2D(7800.0, 2400.0),
        Point2D(7000.0, 2000.0),
        Point2D(6200.0, 2400.0)
      }
    });

    // Добавим к препятствиям окружность
    Geometry::Circle circle(Point2D(7000.0, 6000.0), 1500.0);

    MultiPolygon2D resPoly;
    AbstractOperations::difference(poly, Geometry::toPolygon(circle), resPoly);

    std::vector<Polygon2D> area;
    area.emplace_back(resPoly.at(0));
    // Формирование области триангуляции
    std::shared_ptr<TriangularGeometry::TriangularArea> triangularArea(
      std::make_shared<TriangularArea>(area));

    // Добавление области триангуляции в прокладчик
    pathFinder = std::make_shared<PathFinder>(triangularArea);
  }
//==============================================================================
  void TearDown( )
  {
  }
//==============================================================================
  /*
   * \brief Отодвижение вершины от препятствия.
   *
   * \details Вершина отодвигается в направлении середины угла между
   *          рёбрами препятствия, в сторону от препятствия.
   *          Рёбра препятствия задаются таким образом, чтобы первой вершиной
   *          у них была совпадающая с отодвигаемой точкой маршрута.
   *
   * \param[in] firstBorderSide  - первое фиксированное ребро (самое левое в
   *                               пучке рёбер отодвигаемой вершины);
   *
   * \param[in] secondBorderSide - второе фиксированное ребро (самой правое в
   *                               пучке рёбер отодвигаемой вершины);
   *
   * \param[in] distance         - расстояние отодвижения.
   *
   * \return новая вершина.
   */
  Point2D movePoint(
    Geometry::BorderedLine firstBorderSide,
    Geometry::BorderedLine secondBorderSide,
    const double distance)
  {
    // Угол между отрезками со стороны области триангуляции
    Radian angle(
      firstBorderSide.getAngleTo(secondBorderSide).normalizeWithShift());

    // Линия смещения вершины
    Geometry::Line movedLine(firstBorderSide.getStart(),
      (secondBorderSide.getSlope() + angle / 2.).normalize());

    // Смещённая вершина
    return movedLine.findPoint(firstBorderSide.getStart(), distance, true);
  }
//==============================================================================
  std::shared_ptr<TriangularGeometry::PathFinder> pathFinder; //!< Прокладчик
};
//==============================================================================
// Тесты для исправления некорректных по длине участков маршрута
TEST_F(PathCorrectionTest, CorrectPath)
{
  /* Маршрут через одно препятствие (трапеция) */

  // Прокладка маршрута без исправления вершин
  std::vector<Point2D> pathThroughOneRestriction
    = pathFinder->findPath(Point2D(3600.0, 6300.0), Point2D(1300.0, 6300.0));

  ASSERT_EQ(pathThroughOneRestriction.size(), 4);

  // Исправление участка маршрута, расположенного на трапеции
  std::vector<Point2D> correctedPath = pathFinder->correctPath(1100.0);
  ASSERT_EQ(correctedPath.size(), 3);

  // Построение сразу корректного маршрута
  std::vector<Point2D> correctPathThroughOneRestriction
    = pathFinder->findPath(Point2D(3600.0, 6300.0),
                           Point2D(1300.0, 6300.0), 1100.0);

  ASSERT_EQ(correctPathThroughOneRestriction.size(), 3);

  // Результаты совпадают
  for (size_t i = 0; i < correctedPath.size(); ++i)
    ASSERT_EQ(correctPathThroughOneRestriction.at(i), correctedPath.at(i));

  // Маршрут был исправлен пересечением первого и последнего участков
  Point2D newFirstPoint(movePoint(
    Geometry::BorderedLine(pathThroughOneRestriction.at(1),
                           Point2D(4000.0, 9000.0)),
    Geometry::BorderedLine(pathThroughOneRestriction.at(1),
                           Point2D(2000.0, 5000.0)),
    5.));
  Point2D newSecondPoint(movePoint(
    Geometry::BorderedLine(pathThroughOneRestriction.at(2),
                           Point2D(3000.0, 5000.0)),
    Geometry::BorderedLine(pathThroughOneRestriction.at(2),
                           Point2D(1000.0, 9000.0)),
    5.));

  Geometry::BorderedLine firstLine(pathThroughOneRestriction.at(0),
                                   newFirstPoint);
  Geometry::BorderedLine lastLine(newSecondPoint,
                                  pathThroughOneRestriction.at(3));
  Point2D crossPoint(firstLine.crossPoint(lastLine));

  ASSERT_TRUE(Geometry::distance(crossPoint, correctedPath.at(1))
                                                       < ACCURACY_DOUBLE * 10);

  // Исправление корректного маршрута не требуется
  correctPathThroughOneRestriction
    = pathFinder->findPath(Point2D(3600.0, 6300.0),
                           Point2D(1300.0, 6300.0), 500);

  ASSERT_EQ(correctPathThroughOneRestriction.size(), 4);

  // Совпадает с исходным маршрутом
    for (size_t i = 0; i < correctPathThroughOneRestriction.size(); ++i)
      ASSERT_EQ(correctPathThroughOneRestriction.at(i),
                pathThroughOneRestriction.at(i));

  /* Маршрут между двух препятствий (между трапецией и треугольником) */

  // Прокладка маршрута без исправления вершин
  std::vector<Point2D> pathThroughNarrow
    = pathFinder->findPath(Point2D(1700.0, 4900.0), Point2D(4000.0, 4600.0));

  ASSERT_EQ(pathThroughNarrow.size(), 4);

  // Исправление участка, расположенного между препятствиями
  correctedPath = pathFinder->correctPath(1100.0);
  ASSERT_EQ(correctedPath.size(), 4);

  // Построение сразу корректного маршрута
  std::vector<Point2D> correctPathThroughNarrow
    = pathFinder->findPath(Point2D(1700.0, 4900.0),
                           Point2D(4000.0, 4600.0), 1100.0);

  ASSERT_EQ(correctPathThroughNarrow.size(), 4);

  // Результаты совпадают
  for (size_t i = 0; i < correctedPath.size(); ++i)
    ASSERT_EQ(correctPathThroughNarrow.at(i), correctedPath.at(i));

  // Маршрут был исправлен продлением второго участка
  Point2D movedPoint(movePoint(
    Geometry::BorderedLine(pathThroughNarrow.at(2),
                           Point2D(2500.0, 1000.0)),
    Geometry::BorderedLine(pathThroughNarrow.at(2),
                           Point2D(4500.0, 1000.0)),
    5.));

  Geometry::BorderedLine correctedLine(pathThroughNarrow.at(1), movedPoint);

  Point2D newPoint(correctedLine.findPoint(pathThroughNarrow.at(1), 1100.0));

  ASSERT_TRUE(Geometry::distance(newPoint, correctedPath.at(2))
                                                       < ACCURACY_DOUBLE * 10);

  // Маршрут, для которого не удалось исправить последний участок
  std::vector<Point2D> incorrectPathThroughNarrow
    = pathFinder->findPath(Point2D(2300.0, 4900.0),
                           Point2D(3900.0, 5100.0), 1100.0);

  ASSERT_EQ(incorrectPathThroughNarrow.size(), 0);

  /* Маршрут, проходящий через препятствие-окружность */

  // Прокладка маршрута без исправления вершин
  std::vector<Point2D> pathThroughCircle
    = pathFinder->findPath(Point2D(8300.0, 7000.0), Point2D(7000.0, 4300.0));

  ASSERT_EQ(pathThroughCircle.size(), 79);

  // Исправление некорректных участков с помощью двух вершин
  correctedPath = pathFinder->correctPath(200);
  ASSERT_EQ(correctedPath.size(), 4);

  // Построение сразу корректного маршрута
  std::vector<Point2D> correctPathThroughCircle
    = pathFinder->findPath(Point2D(8300.0, 7000.0),
                           Point2D(7000.0, 4300.0), 200);

  ASSERT_EQ(correctPathThroughCircle.size(), 4);

  // Результаты совпадают
  for (size_t i = 0; i < correctedPath.size(); ++i)
    ASSERT_EQ(correctPathThroughCircle.at(i), correctedPath.at(i));

  // Пересечение первого и последнего участков
  newFirstPoint = movePoint(
    Geometry::BorderedLine(pathThroughCircle.at(1),
                           Point2D(8448.9439107779126, 6388.243350770932)),
    Geometry::BorderedLine(pathThroughCircle.at(1),
                           Point2D(8461.6107508366185, 6337.4394301753246)),
    5.);
  newSecondPoint = movePoint(
    Geometry::BorderedLine(pathThroughCircle.at(77),
                           Point2D(7727.2421215211052, 4688.0204831310375)),
    Geometry::BorderedLine(pathThroughCircle.at(77),
                           Point2D(7681.0116804473828, 4663.4393215822693)),
    5.);

  firstLine = Geometry::BorderedLine(pathThroughCircle.at(0),
                                     newFirstPoint);
  lastLine = Geometry::BorderedLine(newSecondPoint,
                                    pathThroughCircle.at(78));
  crossPoint = firstLine.crossPoint(lastLine);

  // Добавленные вершины принадлежат отрезкам, соединяющим первую и последнюю
  // исправленные вершины маршрута с найденной точкой пересечения участков
  Geometry::BorderedLine firstCrossLine(newFirstPoint, crossPoint);
  Geometry::BorderedLine secondCrossLine(newSecondPoint, crossPoint);

  ASSERT_TRUE(firstCrossLine.nearBorderLine(correctedPath.at(1), 1.));
  ASSERT_TRUE(secondCrossLine.nearBorderLine(correctedPath.at(2), 1.));

  // Исправление некорректных участков с помощью одной вершины
  correctPathThroughCircle
    = pathFinder->findPath(Point2D(8300.0, 7000.0),
                           Point2D(7000.0, 4300.0), 1100.0);

  ASSERT_EQ(correctPathThroughCircle.size(), 3);

  // Маршрут был исправлен пересечением первого и последнего участков
  ASSERT_TRUE(
    Geometry::distance(crossPoint, correctPathThroughCircle.at(1)) < 1.);

  /* Маршрут, проходящий по окружности и треугольнику, расположенным с одной
     стороны от маршрута */

  // Прокладка маршрута без исправления вершин
  std::vector<Point2D> pathThroughCircleAndTriangle
    = pathFinder->findPath(Point2D(3800.0, 5900.0), Point2D(8300.0, 4800.0));

  ASSERT_EQ(pathThroughCircleAndTriangle.size(), 27);

  // Исправление некорректных участков с помощью двух вершин
  correctedPath = pathFinder->correctPath(700);
  ASSERT_EQ(correctedPath.size(), 4);

  // Построение сразу корректного маршрута
  std::vector<Point2D> correctPathThroughCircleAndTriangle
    = pathFinder->findPath(Point2D(3800.0, 5900.0),
                           Point2D(8300.0, 4800.0), 700.0);

  ASSERT_EQ(correctPathThroughCircleAndTriangle.size(), 4);

  // Результаты совпадают
  for (size_t i = 0; i < correctedPath.size(); ++i)
    ASSERT_EQ(correctPathThroughCircleAndTriangle.at(i), correctedPath.at(i));

  // Пересечение первого и последнего участков
  newFirstPoint = movePoint(
    Geometry::BorderedLine(pathThroughCircleAndTriangle.at(1),
                           Point2D(6000.0, 7000.0)),
    Geometry::BorderedLine(pathThroughCircleAndTriangle.at(1),
                           Point2D(4000.0, 6000.0)),
    5.);
  newSecondPoint = movePoint(
    Geometry::BorderedLine(pathThroughCircleAndTriangle.at(25),
      Point2D(7413.4717774638475, 4558.0525511520464)),
    Geometry::BorderedLine(pathThroughCircleAndTriangle.at(25),
      Point2D(7362.8966613924013, 4544.5009896435922)),
    5.);

  firstLine = Geometry::BorderedLine(pathThroughCircleAndTriangle.at(0),
                                     newFirstPoint);
  lastLine = Geometry::BorderedLine(newSecondPoint,
                                    pathThroughCircleAndTriangle.at(26));
  crossPoint = firstLine.crossPoint(lastLine);

  // Добавленные вершины принадлежат отрезкам, соединяющим первую и последнюю
  // исправленные вершины маршрута с найденной точкой пересечения участков
  firstCrossLine = Geometry::BorderedLine(newFirstPoint, crossPoint);
  secondCrossLine = Geometry::BorderedLine(newSecondPoint, crossPoint);

  ASSERT_TRUE(firstCrossLine.nearBorderLine(correctedPath.at(1), 1.));
  ASSERT_TRUE(secondCrossLine.nearBorderLine(correctedPath.at(2), 1.));

  // Исправление некорректных участков с помощью одной вершины
  correctPathThroughCircleAndTriangle
    = pathFinder->findPath(Point2D(3800.0, 5900.0),
                           Point2D(8300.0, 4800.0), 1100.0);

  ASSERT_EQ(correctPathThroughCircleAndTriangle.size(), 3);

  // Маршрут был исправлен пересечением первого и последнего участков
  ASSERT_TRUE(
    Geometry::distance(crossPoint, correctPathThroughCircleAndTriangle.at(1))
                                                                         < 1.);

  /* Маршрут, сначала обходящий трапецию, а затем проходящий между трапецией
     и треугольником. */

   // Прокладка маршрута без исправления вершин
  std::vector<Point2D> pathThroughTwoRestrictions
    = pathFinder->findPath(Point2D(1700.0, 5100.0), Point2D(4200.0, 4800.0));

  ASSERT_EQ(pathThroughTwoRestrictions.size(), 5);

  // В результате исправления получается слишком короткий последний участок
  std::vector<Point2D> incorrectPathThroughTwoRestrictions
    = pathFinder->findPath(Point2D(1700.0, 5100.0),
                           Point2D(4200.0, 4800.0), 1100.0);

  ASSERT_EQ(incorrectPathThroughTwoRestrictions.size(), 0);

  // Некорректен только первый участок
  std::vector<Point2D> correctPathThroughTwoRestrictions
    = pathFinder->findPath(Point2D(1700.0, 5100.0),
                           Point2D(4200.0, 4800.0), 400.0);

  ASSERT_EQ(correctPathThroughTwoRestrictions.size(), 5);

  // Все вершины совпадают с исходными, кроме смещённой второй для исправления
  // первого участка

  ASSERT_EQ(correctPathThroughTwoRestrictions.at(0),
            pathThroughTwoRestrictions.at(0));

  // Маршрут был исправлен продлением первого участка
  movedPoint = movePoint(
    Geometry::BorderedLine(pathThroughTwoRestrictions.at(1),
                           Point2D(3000.0, 5000.0)),
    Geometry::BorderedLine(pathThroughTwoRestrictions.at(1),
                           Point2D(1000.0, 9000.0)),
    5.);

  correctedLine = Geometry::BorderedLine(pathThroughTwoRestrictions.at(0),
                                         movedPoint);

  newPoint = correctedLine.findPoint(pathThroughTwoRestrictions.at(0), 400.0);

  ASSERT_TRUE(
    Geometry::distance(newPoint, correctPathThroughTwoRestrictions.at(1))
                                                      < ACCURACY_DOUBLE * 10.);

  for (size_t i = 2; i < correctPathThroughTwoRestrictions.size(); ++i)
    ASSERT_EQ(correctPathThroughTwoRestrictions.at(0),
              pathThroughTwoRestrictions.at(0));

  /* Маршрут, проходящий через все препятствия
     (трапеция, треугольник, треугольник с окружностью) */

  // Прокладка маршрута без исправления вершин
  std::vector<Point2D> pathThroughAllRestrictions
    = pathFinder->findPath(Point2D(8700.0, 5500.0), Point2D(1300.0, 5300.0));

  ASSERT_EQ(pathThroughAllRestrictions.size(), 56);

  // Исправление некорректных участков
  pathThroughAllRestrictions
    = pathFinder->findPath(Point2D(8700.0, 5500.0),
                           Point2D(1300.0, 5300.0), 1100.0);

  ASSERT_EQ(pathThroughAllRestrictions.size(), 5);

  /* Простейшие маршруты */

  // Маршрут, состоящий из двух вершин (не задевает препятствия)
  std::vector<Point2D> pathWithTwoTops
    = pathFinder->findPath(Point2D(1000.0, 2000.0), Point2D(1000.0, 3000.0));

  ASSERT_EQ(pathWithTwoTops.size(), 2);

  // Если такой маршрут не удовлетворяет ограничению на минимальную длину
  // участка, ничего сделать не можем
  pathWithTwoTops
    = pathFinder->findPath(Point2D(1000.0, 2000.0),
                           Point2D(1000.0, 3000.0), 1100.0);

  ASSERT_EQ(pathWithTwoTops.size(), 0);

  // Маршрут, сосотящий из трёх вершин (проходящий через треугольник)
  std::vector<Point2D> pathThroughTriangle
    = pathFinder->findPath(Point2D(3200.0, 5000.0), Point2D(3800.0, 5000.0));

  ASSERT_EQ(pathThroughTriangle.size(), 3);

  // Если такой маршрут не удовлетворяет ограничению на минимальную длину
  // участка, ничего сделать не можем
  pathThroughTriangle
    = pathFinder->findPath(Point2D(3200.0, 5000.0),
                           Point2D(3800.0, 5000.0), 1100.0);

  ASSERT_EQ(pathThroughTriangle.size(), 0);

  // Маршрут, пересечение участков которого даст вершину внутри препятствия
  std::vector<Point2D> pathThroughBow
    = pathFinder->findPath(Point2D(6900.0, 2100.0), Point2D(6900.0, 1700.0));

  ASSERT_EQ(pathThroughBow.size(), 4);

  // Пока в таких случаях считаем, что не можем упростить маршрут
  // TODO: в дальнейшем можно попробовать в таких случаях удлинять некорректный
  // участок
  pathThroughBow
    = pathFinder->findPath(Point2D(6900.0, 2100.0),
                           Point2D(6900.0, 1700.0), 1300.0);

  ASSERT_EQ(pathThroughBow.size(), 0);
}
//==============================================================================
} //! Tests
} //! TriangularGeometry
//==============================================================================