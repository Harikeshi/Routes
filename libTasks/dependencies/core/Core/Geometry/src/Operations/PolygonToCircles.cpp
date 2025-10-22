#include <Geometry/Operations/PolygonToCircles.hpp>

#include <Geometry/Operations/AtPoints2D.hpp>
#include <Geometry/GeometryException.hpp>
#include "Inner/IntCircles.hpp"
//! AbstractOperations
#include <AbstractOperations/Algorithms/Area.hpp>
#include <AbstractOperations/Algorithms/Buffer.hpp>
#include <AbstractOperations/Algorithms/Correct.hpp>
#include <AbstractOperations/Algorithms/CoveredBy.hpp>
#include <AbstractOperations/Algorithms/Difference.hpp>
#include <AbstractOperations/Algorithms/Distance.hpp>
#include <AbstractOperations/Algorithms/Intersection.hpp>
//! std
#include <list>
//==============================================================================
namespace Geometry
{
//==============================================================================
//==============================================================================
/*!
 * \brief roundUpToMultipleOfA1000 -- функция,
 * округляющая некоторое вещественное число вверх до ближайшего числа,
 * кратного 1000
 *
 * \param[in] x -- исходное вещественное число.
 *
 * \return Искомое число, кратное 1000
 */
inline int roundUpToMultipleOfA1000(double x) noexcept
{
  return static_cast<int>(std::ceil(x / 1000.)) * 1000;
}
//==============================================================================
Polygon2D
toPolygon(
  const Circle& circle,
  const unsigned int countOfVertices, /*= 360U*/
  bool flag) noexcept
{

  //!< Многоугольник, в который будут далее добавляться вершины.
  Polygon2D result;
  
  //! Если радиус круга неположителен, это особенный случай.
  if (circle.radius() < ACCURACY_DOUBLE)
  {
    /*!
     * Если радиус круга равен нулю, возвращаем многоугольник-точку,
     * иначе возвращаем пустой многоугольник.
     */
    if (BasicMath::isNull(circle.radius()))
    {
      result.outer().resize(2);
      result.outer()[0] = circle.center();
      result.outer()[1] = circle.center();
    }
    return result;
  }
  /*!
   * Если радиус положителен,
   * резервируем во внешнем кольце многоугольника место
   * под известное количество вершин.
   */
  result.outer().reserve(countOfVertices + 1U);

  //! Запишем в локальные переменные основные параметры круга.

  //!< Шаг угла.
  const Radian deltaAngle(2. * M_PI / countOfVertices);
  //!< Центр круга.
  const Point2D circleCenter(circle.center());
  //!< Радиус круга (в метрах).
  const double  circleRadius(circle.radius());
  //!< Расстояние от центра круга до вершины описанного многоугольника.
  const double  distanceFromCenterToVertex(
    circleRadius / std::sqrt((std::cos(deltaAngle.get()) + 1.) / 2.));
  //!< 2*pi.
  const Radian  doublePi(2. * M_PI);

  /*!
   * Последовательно строим многоугольник,
   * для которого окружность является вписанной,
   * добавляя новые вершины.
   */

  Radian angle(0.);
  for (unsigned int vertexIndex(0U);
       vertexIndex < countOfVertices;
       angle += deltaAngle, ++vertexIndex)
       if (flag)
       {
           result.outer().emplace_back(
               circleCenter.getX() + distanceFromCenterToVertex * std::cos(angle.get()),
               circleCenter.getY() + distanceFromCenterToVertex * std::sin(angle.get()));
       }
       else
       {
           result.outer().emplace_back(
               circleCenter.getX() + distanceFromCenterToVertex * std::sin(angle.get()),
               circleCenter.getY() + distanceFromCenterToVertex * std::cos(angle.get()));
       }

  //! Замыкаем многоугольник.
  result.outer().push_back(result.outer()[0]);

  return result;
}
//==============================================================================
  /*! 
   * \brief circleWith2PointsOnEdge -- функция,
   * возвращающая круг с минимальным радиусом,
   * содержащий на своей границе две определённые точки,
   * а так же содержащий в себе определённое множество точек.
   * 
   * \param[in] pointsAlreadyCovered -- точки, содержащиеся в круге.
   * 
   * \param[in] pointToBeOnEdge1 -- первая точка, находящаяся в границе круга.
   *
   * \param[in] pointToBeOnEdge2 -- вторая точка, находящаяся в границе круга.
   *
   * \return Искомый круг.
   *
   * \details Функция предназначена только для использования в рамках алгоритма
   * поиска минимального круга, содержащего многоугольник.
   */ 
Circle circleWith2PointsOnEdge(
  const std::vector<Point2D>& pointsAlreadyCovered,
  const Point2D& pointToBeOnEdge1,
  const Point2D& pointToBeOnEdge2) noexcept
{
  //!< Искомый круг.
  Circle circleWithPoints(
    middle(pointToBeOnEdge1, pointToBeOnEdge2),
    distance(pointToBeOnEdge1, pointToBeOnEdge2) / 2.);

  std::size_t count(pointsAlreadyCovered.size());
  for (std::size_t vertexIndex(0); vertexIndex < count; ++vertexIndex)
    if (distance(circleWithPoints.center(), pointsAlreadyCovered[vertexIndex])
        > circleWithPoints.radius())
      /*!
       * Хотя вызываемая ниже функция и может сгенерировать исключение,
       * можно строго доказать, что
       * алгоритм, в котором она вызывается, устроен таким образом,
       * что вызываемая функция никогда не выбросит исключение
       * при использовании в нём.
       */
        circleWithPoints.buildNearTriangle(
            pointsAlreadyCovered[vertexIndex],
            pointToBeOnEdge1,
            pointToBeOnEdge2);

  return circleWithPoints;
}
//==============================================================================
/*! 
 * \brief circleWithPointOnEdge -- функция,
 * возвращающая круг с минимальным радиусом,
 * содержащий на своей границе одну определённую точку,
 * а также содержащий в себе определённое множество точек.
 * 
 * \param[in] pointsAlreadyCovered -- точки, которые содержатся в круге.
 *
 * \param[in] pointToBeOnEdge -- точка, лежащая на границе круга.
 *
 * \return Искомый круг.
 *
 * \details Функция предназначена только для использования в рамках алгоритма
 * поиска минимального круга, содержащего многоугольник.
 */ 
Circle
circleWithPointOnEdge(
  const std::vector<Point2D>& pointsAlreadyCovered,
  const Point2D& pointToBeOnEdge) noexcept
{
  Circle circleWithPointN(
    middle(pointsAlreadyCovered[0], pointToBeOnEdge),
    distance(pointsAlreadyCovered[0], pointToBeOnEdge) / 2.0);

  std::size_t count(pointsAlreadyCovered.size());
  for (std::size_t vertexIndex(1); vertexIndex < count; ++vertexIndex)
    if (distance(circleWithPointN.center(), pointsAlreadyCovered[vertexIndex])
        > circleWithPointN.radius())
    {
      //!< Точки, уже содержащиеся в круге.
      const std::vector<Point2D> pointsInside(
        pointsAlreadyCovered.begin(),
        pointsAlreadyCovered.begin() + vertexIndex);
      circleWithPointN = circleWith2PointsOnEdge(pointsInside, pointToBeOnEdge,
        pointsAlreadyCovered[vertexIndex]);
    };

  return circleWithPointN;
}
//==============================================================================
Circle minimalCoveringCircle( const MultiPoint2D& vertices )
{
  if( vertices.size( ) < 2 )
  {
    //! Если многоугольник пуст, круг не определён.
    if( vertices.size( ) == 0 )
      throw GeometryFailure(
        GeometryEnumFailure::MinimalCircleCoveringEmptyPolygon );
    /*!
    * Если во внешнем кольце многоугольника есть только одна вершина,
    * возвращаем вырожденный круг с нулевым радиусом.
    */
    if( vertices.size( ) == 1 )
      return Circle( vertices[0], 0. );
  }

  //!< Искомый круг.
  Circle coveringCircle(
    middle( vertices[0], vertices[1] ),
    distance( vertices[0], vertices[1] ) / 2. );

  std::size_t countOfVertices( vertices.size( ) );
  if( countOfVertices == 2 )
    return coveringCircle;

  /*!
  * Меняем круг таким образом,
  * чтобы он содержал ещё одну точку из массива,
  * пока не сделаем так, чтобы он содержал все точки.
  */
  for( std::size_t vertexNumber( 2 );
  vertexNumber < countOfVertices;
    ++vertexNumber )

    if( distance( coveringCircle.center( ), vertices[vertexNumber] )
  > coveringCircle.radius( ) - ACCURACY_DOUBLE )
    {
      //!< Точки, уже располагающиеся внутри круга.
      const std::vector<Point2D> pointsInside(
        vertices.begin( ),
        vertices.begin( ) + vertexNumber );
      coveringCircle =
        circleWithPointOnEdge( pointsInside, vertices[vertexNumber] );
    };

  return coveringCircle;
}
//==============================================================================
Circle minimalCoveringCircle(const Polygon2D& polygon)
{
  MultiPoint2D points ( polygon.outer().begin(), polygon.outer().end());

  return minimalCoveringCircle( points );
}
//==============================================================================
/*!
 * \brief findClosestVertexIndex -- функция, возвращающая
 * для многоугольника и точки
 * индекс ближайшей к этой точке вершины многоугольника
 *
 * \param[in] polygon -- исходный многоугольник.
 *
 * \param[in] point -- исходная точка.
 *
 * \return Индекс ближайшей к исходной точке
 * вершины внешнего кольца многоугольника
 */
std::size_t
findClosestVertexIndex(const Polygon2D& polygon, const Point2D& point) noexcept
{
  std::size_t result(0);
  double      minDistance(distance(point, polygon.outer()[0]));
  std::size_t polygonSize(polygon.outer().size());

  for (std::size_t i(1); i < polygonSize; ++i)
  {
    if (distance(point, polygon.outer()[i]) < minDistance)
    {
      minDistance = distance(point, polygon.outer()[i]);
      result = i;
    }
  }

  return result;
}
//==============================================================================
/*!
 * \brief separatePolygon -- функция, возвращающая для многоугольника
 * его разбиение на две части.
 *
 * \param parentPolygon -- многоугольник, который нужно разделить.
 *
 * \return Пара из двух многоугольников, что в объединении дают исходный.
 *
 * \throws GeometryFailure # SeparateInvalidPolygon
 *         GeometryFailure # SeparatePolygonUnknownFailure
 *
 * \details Данная функция должна стараться минимизировать
 * отношение площадей минимальных кругов, содержащих полученные многоугольники,
 * к площадям этих многоугольников.
 */
std::pair<Polygon2D, Polygon2D>
separatePolygon(const Polygon2D& parentPolygon)
{
  using namespace PrimaryEntities;

  //!< Какую часть от исходного радиуса составляет радиус "сжатого" круга.
  constexpr double CIRCLE_COMPRESSION_RATIO(0.9995);
  //!< Погрешность при сравнении с радиусом круга.
  constexpr double CIRCLE_RADIUS_FAULT(1.);

  //!< Многоугольник, который требуется разбить на две части.
  Polygon2D polygonToBeSeparated(parentPolygon);

  //! Если к нам попал некорректный многоугольник, не обрабатываем его.
  if (!AbstractOperations::isCorrect(polygonToBeSeparated))
    throw GeometryFailure(GeometryEnumFailure::SeparateInvalidPolygon);

  /*!<
   * Минимальный круг, содержащий в своей внутренности исходный многоугольник.
   */
  Circle realCoveringCircle(
    std::move(minimalCoveringCircle(polygonToBeSeparated)));

  /*!
   * Здесь происходит следующий трюк.
   * Мы уменьшаем радиус круга,
   * чтобы в дальнейшем его полигональное представление
   * покрывало многоугольник не полностью, а разность круга и многоугольника
   * была несвязным множеством.
   */
  realCoveringCircle.setRadius(
    realCoveringCircle.radius() * CIRCLE_COMPRESSION_RATIO);

  //!< Разность круга и многоугольника, для которого этот круг был построен.
  MultiPolygon2D uselessCovering;
  AbstractOperations::difference(toPolygon(realCoveringCircle),
                                 polygonToBeSeparated, uselessCovering);
  /*!<
   * Количество взимно непересекающихся частей разности круга и многоугольника.
   */
  const std::size_t uselessCoveringSize(uselessCovering.size());

  //! Если разность связна, мы имеем какие-то проблемы.
  if (uselessCoveringSize < 2)
    throw GeometryFailure(GeometryEnumFailure::SeparatePolygonUnknownFailure);

  //! Поиск двух связных подмножеств разности с наибольшей площадью.

  std::vector<double> uselessCoveringAreas(uselessCoveringSize);
  double              maxUselessArea(0.);
  std::size_t         maxUselessAreaIndex1(0);
  for (std::size_t i(0); i < uselessCoveringSize; ++i)
  {
    const double currentArea( AbstractOperations::area(uselessCovering[i]));
    uselessCoveringAreas[i] = currentArea;
    if (maxUselessArea < currentArea)
    {
      maxUselessArea = currentArea;
      maxUselessAreaIndex1 = i;
    }
  }
  maxUselessArea = 0.;
  std::size_t maxUselessAreaIndex2(0);
  for (std::size_t i(0); i < uselessCoveringSize; ++i)
  {
    const double currentArea(uselessCoveringAreas[i]);
    if ((maxUselessArea < currentArea)
      && (i != maxUselessAreaIndex1))
    {
      maxUselessArea = currentArea;
      maxUselessAreaIndex2 = i;
    }
  }

  /*!
   * Для каждого из двух подмножеств ищем вершину многоугольника,
   * касающуюся её и наиболее близкую к центру круга.
   */

  std::size_t border1ClosestPointIndex(0);
  std::size_t border2ClosestPointIndex(0);
  double      minDistanceToCenter1(realCoveringCircle.radius());
  double      minDistanceToCenter2(realCoveringCircle.radius());

  if (AbstractOperations::coveredBy(
        realCoveringCircle.center(),
        uselessCovering[maxUselessAreaIndex1]))
  {
    std::swap(maxUselessAreaIndex1, maxUselessAreaIndex2);
  }

  border1ClosestPointIndex = findClosestVertexIndex(
    uselessCovering[maxUselessAreaIndex1],
    realCoveringCircle.center());

  minDistanceToCenter1 = distance(
    uselessCovering[maxUselessAreaIndex1].outer()[border1ClosestPointIndex],
    realCoveringCircle.center());

  //! Теперь находим индекс этих вершин относительно исходного многоугольника.

  double      tempMinDistance(realCoveringCircle.radius());
  std::size_t position(0);

  /*!
   * Но не всё так гладко.
   * Чуть ранее поиск как минимум одной из этих вершин мог закончиться ничем
   * (ближайшие вершины лежат на описанной около многоугольника окружности).
   * Если это так, с разностью соприкасается лишь одно ребро многоугольника.
   * Найдём его и построим новую точку на его середине.
   * Она и заменит нам не найденную изначально вершину многоугольника.
   * Напомню, что радиус исходного круга был намеренно уменьшен.
   */

  std::size_t polygonToBeSeparatedSize(polygonToBeSeparated.outer().size());
  //! Если соприкасающаяся вершина не была найдена...
  if (minDistanceToCenter1 >= realCoveringCircle.radius() - CIRCLE_RADIUS_FAULT)
  {
    /*!
     * Ищем вершину многугольника,
     * наиболее близкую к связному подмножеству разности.
     * Затем выбираем из двух соседних с ней вершин также наиближайшую.
     * Найденные две вершины принадлежат искомому ребру многоугольника.
     */
    for (std::size_t i(0); i < polygonToBeSeparatedSize - 1; ++i)
    {
      const double currentDistance(
        AbstractOperations::distanceBetweenGeometries(
          polygonToBeSeparated.outer()[i],
          uselessCovering[maxUselessAreaIndex1]));

      if (currentDistance < tempMinDistance)
      {
        tempMinDistance = currentDistance;
        position = i;
      };
    }
    if ((position == 0)
        &&
        (AbstractOperations::distanceBetweenGeometries(
          polygonToBeSeparated.outer()[polygonToBeSeparatedSize - 2],
          uselessCovering[maxUselessAreaIndex1])
         < AbstractOperations::distanceBetweenGeometries(
             polygonToBeSeparated.outer()[position + 1],
             uselessCovering[maxUselessAreaIndex1])))
    {
      position = polygonToBeSeparatedSize - 2;
    }
    else if (position > 0)
    {
      if (AbstractOperations::distanceBetweenGeometries(
            polygonToBeSeparated.outer()[position - 1],
            uselessCovering[maxUselessAreaIndex1])
        < AbstractOperations::distanceBetweenGeometries(
            polygonToBeSeparated.outer()[position + 1],
            uselessCovering[maxUselessAreaIndex1]))
      {
        --position;
      }
    }
    /*!
     * Строим и добавляем в многоугольник середину искомого ребра
     * и запоминаем её индекс.
     */
    polygonToBeSeparated.outer().insert(
      polygonToBeSeparated.outer().begin() + position + 1,
      std::move(
        middle(
          polygonToBeSeparated.outer()[position],
          polygonToBeSeparated.outer()[position + 1])));
    ++polygonToBeSeparatedSize;
    border1ClosestPointIndex = position + 1;
  }
  /*!
   * Если же мы нашли вершину ранее,
   * просто запомним её индекс относительно многоугольника.
   */
  else
  {
    for (; position < polygonToBeSeparatedSize - 1; ++position)
    {
      if (BasicMath::isNull(
            distance(
              uselessCovering[maxUselessAreaIndex1].outer()[
                border1ClosestPointIndex],
              polygonToBeSeparated.outer()[position])))
      {
        break;
      }
    }
    border1ClosestPointIndex = position;
  }

  border2ClosestPointIndex = findClosestVertexIndex(
    uselessCovering[maxUselessAreaIndex2],
    realCoveringCircle.center());

  minDistanceToCenter2 = distance(
    uselessCovering[maxUselessAreaIndex2].outer()[border2ClosestPointIndex],
    realCoveringCircle.center());

  /*!
   * Повторяем то же самое для второй по размеру связной части разности.
   * Только теперь нужно учитывать,
   * что мы уже добавили в многоугольник дополнительную вершину,
   * которую можно случайно выбрать или задеть (но делать этого нельзя).
   */
  tempMinDistance = realCoveringCircle.radius();
  position = 0;

  if (minDistanceToCenter2 >= realCoveringCircle.radius() - CIRCLE_RADIUS_FAULT)
  {
    for (std::size_t i(0); i < polygonToBeSeparatedSize - 1; ++i)
    {
      const double currentDistance(
        AbstractOperations::distanceBetweenGeometries(
          polygonToBeSeparated.outer()[i],
          uselessCovering[maxUselessAreaIndex2]));

      if ((currentDistance < tempMinDistance)
          &&
          (i != border1ClosestPointIndex)
          &&
          (i != ((border1ClosestPointIndex > 0) ?
           (border1ClosestPointIndex - 1)
           : (polygonToBeSeparatedSize - 2))))
      {
        tempMinDistance = currentDistance;
        position = i;
      }
    }
    if ((position == 0)
        &&
        (AbstractOperations::distanceBetweenGeometries(
          polygonToBeSeparated.outer()[polygonToBeSeparatedSize - 2],
          uselessCovering[maxUselessAreaIndex2])
         < AbstractOperations::distanceBetweenGeometries(
            polygonToBeSeparated.outer()[position + 1],
            uselessCovering[maxUselessAreaIndex2]))
        &&
        (border1ClosestPointIndex != polygonToBeSeparatedSize - 2))
    {
      position = polygonToBeSeparatedSize - 2;
    }
    else if (position > 0)
    {
      if ((AbstractOperations::distanceBetweenGeometries(
             polygonToBeSeparated.outer()[position - 1],
             uselessCovering[maxUselessAreaIndex2])
           < AbstractOperations::distanceBetweenGeometries(
             polygonToBeSeparated.outer()[position + 1],
               uselessCovering[maxUselessAreaIndex2]))
          &&
          (position - 1 != border1ClosestPointIndex))
      {
        --position;
      }
    }
    polygonToBeSeparated.outer().insert(
      polygonToBeSeparated.outer().begin() + position + 1,
      middle(
        polygonToBeSeparated.outer()[position],
        polygonToBeSeparated.outer()[position + 1])
      );
    ++polygonToBeSeparatedSize;
    border2ClosestPointIndex = position + 1;

    if (border1ClosestPointIndex > border2ClosestPointIndex)
      ++border1ClosestPointIndex;

  }
  else
  {
    for (; position < polygonToBeSeparatedSize - 1; ++position)
      if ((BasicMath::isNull(
             distance(
               uselessCovering[maxUselessAreaIndex2].outer()[
                 border2ClosestPointIndex],
               polygonToBeSeparated.outer()[position])))
          &&
          (position != border1ClosestPointIndex)
          &&
          (position != ((border1ClosestPointIndex > 0) ?
                        (border1ClosestPointIndex - 1)
                        : (polygonToBeSeparatedSize - 2))))
      {
        break;
      }
    border2ClosestPointIndex = position;
  }
  /*!
   * Если построенный секущий отрезок не принадлежит исходному многоугольнику,
   * во избежание самопересечений будем строить вершину,
   * соответствующую второй выбранной части разности круга и многоугольника,
   * по-особенному. Замечу, что центр круга
   * может содержаться не более чем в одной части
   * разности круга и многоугольника.
   */

  /*!<
   * Отрезок, рассекающий многоугольник
   * (соединяет две выбранные ранее вершины многоугольника).
   */
  const Linestring2D separatingSegment{
    polygonToBeSeparated.outer()[border1ClosestPointIndex],
    polygonToBeSeparated.outer()[border2ClosestPointIndex]
  };
  if (!AbstractOperations::coveredBy(separatingSegment, polygonToBeSeparated))
  {
    //!< Пересечение исходного мноугольника и separatingSegment.
    MultiLinestring2D separatingSegmentIntersection;
    AbstractOperations::intersection(separatingSegment,
                                     polygonToBeSeparated,
                                     separatingSegmentIntersection);

    //!< Отрезок, рассекающий многоугольник и полностью содержащийся в нём.
    Linestring2D internalSeparatingSegment;
    //! Именно он (его концы) нам в конечном счёте и нужен.

    /*!
     * По построению, так как другая часть uselessCovering
     * не содержит центр realCoveringCircle, некоторая часть отрезка
     * separatingSegment (ненулевой длины),
     * причём содержащая выбранную точку из той части uselessCovering,
     * будет содержаться в исходном многоугольнике.
     * Именно эту часть мы и выберем
     * в качестве корректного рассекающего отрезка.
     */

    for (const Linestring2D& segment : separatingSegmentIntersection)
    {
      if ((BasicMath::isNull(
             distance(
               segment[0],
               polygonToBeSeparated.outer()[border1ClosestPointIndex])))
        ||
        (BasicMath::isNull(distance(
           segment[1],
           polygonToBeSeparated.outer()[border1ClosestPointIndex]))))
      {
        internalSeparatingSegment = segment;
        break;
      }
      if ((BasicMath::isNull(
             distance(
               segment[0],
               polygonToBeSeparated.outer()[border2ClosestPointIndex])))
        ||
        (BasicMath::isNull(
           distance(
             segment[1],
             polygonToBeSeparated.outer()[border2ClosestPointIndex]))))
      {
        /*!
         * Если окрестность точек из внутреннего
         * относительно многоугольника отрезка,
         * содержащаяся в многоугольнике,
         * прилегает к противоположному концу исходного рассекающего отрезка,
         * просто поменяем индексы местами, чтобы не писать дублирующийся код.
         */
        std::swap(border1ClosestPointIndex, border2ClosestPointIndex);
        internalSeparatingSegment = segment;
        break;
      }
    }

    /*!
     * Один из краёв этой части уже существует
     * как вершина в исходном многоугольнике.
     * А вот другой край нужно ещё туда записать.
     */

    //!< Противоположный край корректного рассекающего отрезка.
    Point2D newVertex;
    
    if (BasicMath::isNull(
          distance(
            internalSeparatingSegment[0],
            polygonToBeSeparated.outer()[border1ClosestPointIndex])))
    {
      newVertex = internalSeparatingSegment[1];
    }
    else
      newVertex = internalSeparatingSegment[0];

    //! Находим сторону исходного многоугольника, на которой лежит newVertex.

    //!< Сторона исходного многоугольника.
    Linestring2D polygonSide;
    polygonSide.resize(2);
    
    for (std::size_t i(0); i < polygonToBeSeparatedSize - 1; ++i)
    {
      polygonSide[0] = polygonToBeSeparated.outer()[i];
      polygonSide[1] = polygonToBeSeparated.outer()[i + 1];
      if (BasicMath::isNull(
        AbstractOperations::distanceBetweenGeometries(newVertex,
                                                      polygonSide)))
      {
        /*!
         * Если вдруг звёзды сошлись
         * и наша newVertex совпадает
         * с уже существующей вершиной исходного многоугольника,
         * учитываем это и ничего в многоугольник не добавляем.
         */
        if (BasicMath::isNull(
              distance(newVertex, polygonToBeSeparated.outer()[i])))
          border2ClosestPointIndex = i;
        else if (BasicMath::isNull(
                   distance(newVertex, polygonToBeSeparated.outer()[i + 1])))
          border2ClosestPointIndex = i + 1;
        /*!
         * А если нет,
         * добавляем новую вершину и корректируем переменную,
         * содержащую информацию об индексе другой вершины,
         * если после добавления он фактически изменился.
         */
        else
        {
          polygonToBeSeparated.outer().insert(
            polygonToBeSeparated.outer().begin() + i + 1,
            newVertex);
          ++polygonToBeSeparatedSize;
          if (border1ClosestPointIndex > i)
            ++border1ClosestPointIndex;
          border2ClosestPointIndex = i + 1;
        }
        break;
      }
    }
  }

  //! Создаём новые дочерние многоугольники.

  Polygon2D newPolygon1;
  Polygon2D newPolygon2;

  const std::size_t start(
    std::min(border1ClosestPointIndex, border2ClosestPointIndex));
  const std::size_t finish(
    std::max(border1ClosestPointIndex, border2ClosestPointIndex));

  /*!
   * Инициализируем многоугольники следующим образом.
   * Через те две вершины, что мы нашли (построили) можно провести отрезок,
   * который будет рассекать многоугольник на две части.
   * Эти две части и станут новыми многоугольниками.
   */
  newPolygon1.outer().insert(
    newPolygon1.outer().end(),
    polygonToBeSeparated.outer().begin(),
    polygonToBeSeparated.outer().begin() + start + 1);
  newPolygon1.outer().insert(
    newPolygon1.outer().end(),
    polygonToBeSeparated.outer().begin() + finish,
    polygonToBeSeparated.outer().end());
  
  newPolygon2.outer().insert(
    newPolygon2.outer().end(),
    polygonToBeSeparated.outer().begin() + start,
    polygonToBeSeparated.outer().begin() + finish + 1);
  newPolygon2.outer().push_back(newPolygon2.outer()[0]);

  if ((newPolygon1.outer().size() < 4)
      ||
      (newPolygon2.outer().size() < 4)
      ||
      (!AbstractOperations::isCorrect(newPolygon1))
      ||
      (!AbstractOperations::isCorrect(newPolygon2)))
    throw GeometryFailure(GeometryEnumFailure::SeparatePolygonUnknownFailure);

  return std::pair<Polygon2D, Polygon2D>(
    std::move(newPolygon1),
    std::move(newPolygon2));
}
//==============================================================================
//!< Круг с целым неотрицательным радиусом и его полигональное приближение.
typedef std::pair<IntCircle, Polygon2D> CircleWithItsPolygon;
//==============================================================================
/*!
 * \brief Функция-"компаратор" для сортировки кругов по возрастанию радиуса.
 *
 * \param a, b -- круги со своими полигональными приближениями.
 *
 * \return true, если радиус второго круга больше радиуса первого;
 *         иначе - false.
 */
inline bool 
sortByRadiusAscending(
  const CircleWithItsPolygon& a,
  const CircleWithItsPolygon& b) noexcept
{
  return a.first.intRadius() < b.first.intRadius();
}
//==============================================================================
/*!
 * \brief circleFilter -- функция, получающая на вход список с кругами
 * и удаляющая в нём избыточные круги
 * (круги, при исключении которых многоугольник-параметр всё ещё остаётся,
 * покрыт объединением оставшихся).
 * Причём каждый элемент списка помимо непосредственно круга
 * содержит также его полигональное приближение.
 *
 * \param[in|out] circles -- исходный список, содержащий информацию
 * о кругах и их полигональных приближениях.
 *
 * \param polygonToBeOvercasted -- многоугольник, который должен покрываться
 * объединением возвращаемого множества.
 *
 * \details Исходный многоугольник должен содержаться
 * в объединении кругов из исходного списка.
 * Функция может экстренно прервать фильтрацию из-за внутренних ошибок.
 */
void
circleFilter(
  std::list<CircleWithItsPolygon>& circles,
  const Polygon2D& polygonToBeOvercasted)
{
  using namespace PrimaryEntities;
  using namespace geos::operation::buffer;

  //!< Объект для передачи данных через geos.
  MultiPolygon2D intermediateBuffer;

  /*!
   * Сортируем круги по возрастанию радиуса,
   * чтобы сначала удалялись более маленькие.
   */
  circles.sort(sortByRadiusAscending);

  auto endOfCircles(circles.end());

  //! Быстрая фильтрация избыточных кругов с помощью geos.

  for (auto currentCircleToExamine(circles.begin());
  currentCircleToExamine != circles.end();)
  {
    //!< Объединение всех кругов, имеющих непустое пересечение с текущим.
    MultiPolygon2D intersectedCirclesUnion;
    
    /*!
     * Собираем в переменной intersectedCirclesUnion данные
     * об объединении пересечений currentCircleToExamine
     * со всеми остальными кругами.
     */

    for (auto it(circles.begin()); it != endOfCircles; ++it)
      if (it != currentCircleToExamine)
        if (AbstractOperations::hasIntersection(
          currentCircleToExamine->second, it->second))
        {
          AbstractOperations::unionOf(intersectedCirclesUnion, it->second,
                                      intermediateBuffer);
          intersectedCirclesUnion = std::move(intermediateBuffer);
        }

    //!< Часть круга, которая не пересекается ни с одним другим.
    MultiPolygon2D nonIntersectedPart;
    AbstractOperations::difference(
      currentCircleToExamine->second,
      intersectedCirclesUnion,
      nonIntersectedPart);

    /*!
     * Если эта часть не покрывает исходный многоугольник
     * (а то и вовсе равна пустому множеству),
     * исключаем текущий круг из контейнера.
     */
    if (!AbstractOperations::hasIntersection(nonIntersectedPart,
                                             polygonToBeOvercasted))
      currentCircleToExamine = circles.erase(currentCircleToExamine);
    else
      ++currentCircleToExamine;
  }

  //! Мощная фильтрация.

  //! Последовательно рассматриваем каждый круг.
  for (auto it1(--circles.end());;)
  {
    //! Для каждого круга рассматриваем каждый другой круг.
    for (auto it2(circles.begin()); it2 != circles.end();)
    {
      /*!
       * Если круги пересекаются,
       * можно попробовать увеличить радиус одного круга
       * и тем самым полностью покрыть другой круг.
       */
      if ((it1 != it2) && (AbstractOperations::hasIntersection(it1->second,
                                                               it2->second)))
      {

        /*!<
         *  Объединение всех кругов, имеющих непустое пересечение с кругом,
         *  который мы хотим ликвидировать.
         */
        MultiPolygon2D intersectedCirclesUnion;

        //! Заполняем intersectedCirclesUnion.
        endOfCircles = circles.end();
        for (auto c(circles.begin()); c != endOfCircles; ++c)
        {
          if ((c != it2) && (AbstractOperations::hasIntersection(c->second,
                                                                 it2->second)))
          {
            AbstractOperations::unionOf(intersectedCirclesUnion, c->second,
                                        intermediateBuffer);
            intersectedCirclesUnion = std::move(intermediateBuffer);
          }
        }

        //!< Часть круга, которая не пересекается ни с одним другим кругом.
        MultiPolygon2D nonIntersectedPart;
        AbstractOperations::difference(it2->second, intersectedCirclesUnion,
                                       nonIntersectedPart);
        /*!<
         * Часть круга, которая не пересекается ни с одним другим кругом,
         * но пересекается с многоугольником
         * (так называемое "полезное покрытие").
         */
        MultiPolygon2D usefulCovering;
        AbstractOperations::intersection(nonIntersectedPart,
                                         polygonToBeOvercasted,
                                         usefulCovering);
        /*!
         * Если эта часть не пуста, пробуем её покрыть,
         * увеличивая первый круг.
         */
        if (!usefulCovering.empty())
        {
          //!< Объединение двух рассматриваемых кругов.
          MultiPolygon2D twoCirclesUnion;
          AbstractOperations::unionOf(it1->second, it2->second,
                                      twoCirclesUnion);
          /*!<
           * Объединение всех кругов, имеющих непустое пересечение с кругами,
           * которые мы рассматриваем.
           */
          MultiPolygon2D intersectedCirclesUnionForTwoCircles;

          //! Заполняем intersectedCirclesUnionForTwoCircles.
          for (auto c(circles.begin()); c != endOfCircles; ++c)
            if ((c != it2)
                &&
                (c != it1)
                &&
                (AbstractOperations::hasIntersection(c->second,
                                                     twoCirclesUnion)))
            {
              AbstractOperations::unionOf(
                intersectedCirclesUnionForTwoCircles,
                c->second,
                intermediateBuffer);
              intersectedCirclesUnionForTwoCircles = std::move(
                intermediateBuffer);
            }

          /*!<
           * Часть объединения двух кругов,
           * которая не пересекается ни с исходным многоугольником,
           * ни с каким-либо третьим кругом.
           */
          MultiPolygon2D initialUselessPart;
          AbstractOperations::difference(
            twoCirclesUnion,
            intersectedCirclesUnionForTwoCircles,
            intermediateBuffer);
          AbstractOperations::difference(
            intermediateBuffer,
            polygonToBeOvercasted,
            initialUselessPart);

          /*!<
           * Площадь "лишнего" покрытия,
           * создаваемая двумя рассматриваемыми кругами.
           */
          const double initialUselessArea(
            AbstractOperations::area(initialUselessPart));
          //<! Тестовый круг, которым мы будем пытаться покрывать второй.
          CircleWithItsPolygon firstCircleTest(*it1);
          /*<!
           * Пересечение тестового круга с исходным многоугольником.
           * Инициализация пересечения.
           */
          MultiPolygon2D       intersectionWithPolygon;
          AbstractOperations::intersection(
            firstCircleTest.second,
            polygonToBeOvercasted,
            intersectionWithPolygon);

          /*!
           * В процессе увеличения радиуса первого круга
           * (и смещения его центра) мы неизбежно
           * либо покроем "полезное покрытие" второго,
           * либо уменьшим точность покрытия
           * (может, и то, и другое сразу).
           */
          do
          {
            /*!<
             * Точка "полезного покрытия" второго круга,
             * наиболее удалённая от центра первого круга.
             */
            Point2D edgeOfUsefulCovering(usefulCovering[0].outer()[0]);
            /*!<
             * Расстояние от центра первого круга
             * до наиболее удалённой от него точки
             * "полезного покрытия" второго круга.
             */
            double  distanceToEdgeOfUsefulCovering(
              distance(it1->first.center(), usefulCovering[0].outer()[0]));

            //! Инициализируем эти значения.
            for (const Polygon2D& part : usefulCovering)
              for (const Point2D& vertex : part.outer())
              {
                const double currentDistance(
                  distance(it1->first.center(), vertex));
                if (currentDistance > distanceToEdgeOfUsefulCovering)
                {
                  edgeOfUsefulCovering = vertex;
                  distanceToEdgeOfUsefulCovering = currentDistance;
                }
              }
            /*!<
             * Вектор, соединяющий центр тестового круга
             * и самую удалённую от него точку "полезного покрытия"
             * круга-кандидата на удаление.
             */
            Point2D connectingVector(
              (edgeOfUsefulCovering.getX()
                - firstCircleTest.first.center().getX()),
              (edgeOfUsefulCovering.getY()
                - firstCircleTest.first.center().getY()));

            //! Нормирование.
            double length(distance(Point2D(.0, .0), connectingVector));
            connectingVector.set(connectingVector.getX() / length,
                                 connectingVector.getY() / length);

            //! Достижение нужной длины.
            double newLength((distanceToEdgeOfUsefulCovering
                            - firstCircleTest.first.radius()) / 2.);
            connectingVector.set(connectingVector.getX() * newLength,
                                 connectingVector.getY() * newLength);

            /*!
             * Теперь будем одновременно сдвигать центр и увеличивать радиус
             * на одну и ту же величину.
             */

            //! Увеличение радиуса и сдвиг центра на connectingVector.
            
            Point2D newCenter(firstCircleTest.first.center().getX()
                              + connectingVector.getX(),
                              firstCircleTest.first.center().getY()
                              + connectingVector.getY());
            firstCircleTest.first.setCenter(newCenter);
            firstCircleTest.first.setRadius(
              firstCircleTest.first.intRadius()
              + roundUpToMultipleOfA1000(
                  distance(Point2D(.0, .0), connectingVector)));
            firstCircleTest.second = std::move(
              toPolygon(firstCircleTest.first));
            AbstractOperations::intersection(
              firstCircleTest.second,
              polygonToBeOvercasted,
              intersectionWithPolygon);

          } while (!AbstractOperations::coveredBy(usefulCovering,
                                                  firstCircleTest.second));

          /*!<
           * Новая площадь,
           * покрытая нашим тестовым кругом вне исходного многоугольника.
           */
          MultiPolygon2D newUselessPart;
          intermediateBuffer.clear();
          AbstractOperations::difference(
            firstCircleTest.second,
            intersectedCirclesUnionForTwoCircles,
            newUselessPart);
          /*!
           * "Надувание" делается,
           * чтобы избежать появления самопересечений в дальнейшем.
           */
          AbstractOperations::Helper::buffer(
            newUselessPart,
            intermediateBuffer,
            .5,
            BufferParameters::DEFAULT_QUADRANT_SEGMENTS,
            BufferParameters::CAP_SQUARE,
            BufferParameters::JOIN_MITRE,
            1.);
          newUselessPart.clear();
          try
          {
            AbstractOperations::difference(
              intermediateBuffer,
              polygonToBeOvercasted,
              newUselessPart);
            intermediateBuffer.clear();
            double newUselessArea(AbstractOperations::area(newUselessPart));
            /*!
             * Если своими действиями мы улучшили ситуацию,
             * изменяем список кругов.
             x*/
            if (newUselessArea < initialUselessArea * (1. + ACCURACY_DOUBLE))
            {
              //! Заменяем первый круг на увеличенный.
              *it1 = firstCircleTest;
              //! Удаляем второй круг.
              it2 = circles.erase(it2);
              //! Обходим список заново.
              it1 = --circles.end();
              break;
            }
            /*!
             * Иначе забываем про наш тестовый первый круг
             * и идём на следующую итерацию.
             */
            ++it2;
          }
          //! То же самое делаем при возникновении исключения.
          catch (...)
          {
            ++it2;
          }
        }
        /*!
         * Если "полезное покрытие" второго круга
         * было и вовсе пустым, удаляем его без всякого сожаления.
         */
        else
        {
          it2 = circles.erase(it2);
        }
      }
      //! Иначе идём на следующую итерацию.
      else
        ++it2;
    }
    if (it1 == circles.begin())
      break;
    else
      --it1;
  }
}
//==============================================================================
std::vector<Circle>
polygonToCircles(
  Polygon2D polygon,
  double accuracy /*= 0.7*/,
  const unsigned int minRadius /*= 1000U*/)
{
  using namespace PrimaryEntities;

  //! Проверка входного значения на корректность.
  if (std::isnan(accuracy))
    return std::vector<Circle>();
  if (accuracy < .0)
    accuracy = .0;
  if (accuracy > 1.0 - ACCURACY_DOUBLE)
    accuracy = 1.0 - ACCURACY_DOUBLE;

  //! Если многоугольник некорректный, нам не о чем говорить...
  if (!AbstractOperations::isCorrect(polygon))
    throw(GeometryFailure(
      GeometryEnumFailure::PolygonToCirclesInvalidPolygon));

  //! Игнорируем внутренние кольца многоугольника.
  polygon.inners().clear();

  //! Объявление и инициализация основных переменных.

  /*!<
   * Структура,
   * содержащая данные о многоугольнике и минимальном круге, содержащем его.
   */
  struct PolygonAndItsCoveringCircle {
    /*!
     * \brief Конструктор.
     */
    PolygonAndItsCoveringCircle(
      const Polygon2D pPolygon,
      const IntCircle& pCircle,
      const Polygon2D& pCircleAsPolygon,
      double pUselessArea) :
      polygon(pPolygon),
      circle(pCircle),
      circleAsPolygon(pCircleAsPolygon),
      uselessArea(pUselessArea) {}
    //!< Многоугольник.
    Polygon2D polygon;
    //!< Круг минимального радиуса, содержащий многоугольник.
    IntCircle circle;
    //!< Полигональное приближение круга.
    Polygon2D circleAsPolygon;
    //!< Площадь разности круга и многоугольника.
    double    uselessArea;
  };

  //!< Многоугольник, который берётся из начала списка в начале цикла.
  Polygon2D       polygonToBeSeparated(polygon);
  //!< Круг минимального радиуса, содержащий многоугольник.
  const Circle    realCircle(
    std::move(minimalCoveringCircle(polygonToBeSeparated)));
  //!< Тот же круг, но с округлённым до километров радиусом.
  IntCircle       parentCircle(
    realCircle.center(),
    roundUpToMultipleOfA1000(realCircle.radius()));
  //!< Разность круга и многоугольника, для которого этот круг был построен.
  MultiPolygon2D  uselessCovering;
  AbstractOperations::difference(toPolygon(parentCircle),
                                 polygonToBeSeparated, uselessCovering);

  /*!<
   * Информация об исходном многоугольнике
   * и минимальном круге, содержащем его.
   */
  PolygonAndItsCoveringCircle            initialPolygonWithItsCircle{
    polygonToBeSeparated,
    parentCircle,
    std::move(toPolygon(parentCircle)),
    AbstractOperations::area(uselessCovering)};
  //!< Текущее разбиение исходного многоугольника.
  std::list<PolygonAndItsCoveringCircle> currentCirclesList{
    std::move(initialPolygonWithItsCircle)
  };

  //!< Значение уже достигнутой точности. Вычисляется в конце каждой итерации.
  double areaProportion( AbstractOperations::area(polygonToBeSeparated)
                               / (M_PI * std::pow(parentCircle.radius(), 2U)));
  //!< Объединение всех кругов, содержащихся в текущем разбиении.
  MultiPolygon2D coveringCirclesUnion{
    initialPolygonWithItsCircle.circleAsPolygon
  };
  
  /*!
   * Повторяем пока либо не достигнем нужной точности,
   * либо не закончатся круги, которые есть смысл обрабатывать.
   */
  while ((areaProportion < accuracy)
         && (currentCirclesList.begin()->uselessArea > ACCURACY_DOUBLE))
  {
    //! Делим текущий многоугольник на две части.

    std::pair<Polygon2D, Polygon2D> polygonsChildren;
    try
    {
      polygonsChildren = std::move(
        separatePolygon(currentCirclesList.begin()->polygon));
    }
    /*!
     * Некорректность возвращаемых дочерних многоугольников
     * -- признак проблем в обработке текущего.
     */
    catch (...)
    {
      currentCirclesList.emplace_back(*(currentCirclesList.begin()));
      //! Этим флагом помечаются все "неприятные" круги.
      --currentCirclesList.end()->uselessArea = -1.;
      currentCirclesList.erase(currentCirclesList.begin());
      //! Переходим к следующей итерации.
      continue;
    }

    //! Создаём и инициализируем для новых многоугольников круги.

    const Circle    tempCircle1(
      std::move(minimalCoveringCircle(polygonsChildren.first)));
    const Circle    tempCircle2(
      std::move(minimalCoveringCircle(polygonsChildren.second)));
    const IntCircle newCircle1(
      tempCircle1.center(),
      roundUpToMultipleOfA1000(tempCircle1.radius()));
    const IntCircle newCircle2(
      tempCircle2.center(),
      roundUpToMultipleOfA1000(tempCircle2.radius()));
    const Polygon2D newCircleAsPolygon1(std::move(toPolygon(newCircle1)));
    const Polygon2D newCircleAsPolygon2(std::move(toPolygon(newCircle2)));

    /*!
     * Перед стиранием создаём увеличенную копию круга,
     * содержащего исходный многоугольник итерации.
     * Она пригодится нам в самом конце цикла при подсчёте точности.
     */

    parentCircle = currentCirclesList.begin()->circle;
    parentCircle.setRadius(static_cast<double>(parentCircle.intRadius() + 1U));
    MultiPolygon2D parentCircleUselessArea{
      std::move(toPolygon(parentCircle))
    };

    /*!
     * Вычтем из неё все остальные круги текущего покрытия
     * и получим, так называемое, бесполезное покрытие
     * от круга, содержащего исходный многоугольник итерации
     */

    MultiPolygon2D intermediateBuffer;
    auto currentCirclesListEnd(currentCirclesList.end());
    for (auto it(++currentCirclesList.begin());
         it != currentCirclesListEnd;
         ++it)
      if (AbstractOperations::hasIntersection(parentCircleUselessArea,
                                              it->circleAsPolygon))
      {
        AbstractOperations::difference(
          parentCircleUselessArea,
          it->circleAsPolygon,
          intermediateBuffer);
        parentCircleUselessArea = std::move(intermediateBuffer);
      }

    //! Наконец, стираем первый элемент из списка кругов текущего покрытия.
    currentCirclesList.erase(currentCirclesList.begin());

    //! Подготовка к записи в списки информации о новых многоугольниках.
    uselessCovering.clear();
    AbstractOperations::difference(newCircleAsPolygon1, polygon,
                                   uselessCovering);
    //!< Площадь разности круга и многоугольника.
    double currentUselessArea(AbstractOperations::area(uselessCovering));

    /*!
     * Если радиус построенного круга не слишком маленький,
     * записываем круг в список в соответствии с его currentUselessArea
     * (чем меньше это число, тем глубже в список отправится многоугольник
     * и сопутствующие ему объекты).
     */
    if (newCircle1.radius() > minRadius)
    {
      //!< Итератор списка.
      auto it(currentCirclesList.begin());
      currentCirclesListEnd = currentCirclesList.end();
      for (; it != currentCirclesListEnd; ++it)
        if (currentUselessArea > it->uselessArea)
          break;

      currentCirclesList.insert(
        it,
        std::move(
          PolygonAndItsCoveringCircle{
            polygonsChildren.first,
            newCircle1,
            newCircleAsPolygon1,
            currentUselessArea
          }));
    }
    /*!
     * Если же радиус круга слишком маленький,
     * пометим его как "неприятный" и отправим в конец списка.
     */
    else
      currentCirclesList.emplace_back(
        polygonsChildren.first,
        newCircle1,
        newCircleAsPolygon1,
        -1.);

    //! Всё то же самое для второго построенного многоугольника.
    uselessCovering.clear();
    AbstractOperations::difference(newCircleAsPolygon2,
                                   polygon, uselessCovering);
    currentUselessArea = AbstractOperations::area(uselessCovering);
    if (newCircle2.radius() > minRadius)
    {
      auto it(currentCirclesList.begin());
      currentCirclesListEnd = currentCirclesList.end();
      for (; it != currentCirclesListEnd; ++it)
        if (currentUselessArea > it->uselessArea)
          break;

      currentCirclesList.insert(
        it,
        std::move(
          PolygonAndItsCoveringCircle{
            polygonsChildren.second,
            newCircle2,
            newCircleAsPolygon2,
            currentUselessArea
        }));
    }
    else
    {
      currentCirclesList.emplace_back(
        polygonsChildren.second,
        newCircle2,
        newCircleAsPolygon2,
        -1.);
    }

    //! Теперь рассчитываем достигнутую точность.

    /*!
     * Мы увеличивали круг parentCircleAsPolygon при инициализации,
     * чтобы попробовать избежать появления некорректных многоугольников
     * при выполнении следующих строчек
     * (это самый опасный код во всей функции!).
     */
    try
    {
      AbstractOperations::difference(
        coveringCirclesUnion,
        parentCircleUselessArea,
        intermediateBuffer);
      coveringCirclesUnion = std::move(intermediateBuffer);
      AbstractOperations::unionOf(coveringCirclesUnion, newCircleAsPolygon1,
                                  intermediateBuffer);
      coveringCirclesUnion = std::move(intermediateBuffer);
      AbstractOperations::unionOf(coveringCirclesUnion, newCircleAsPolygon2,
                                  intermediateBuffer);
      coveringCirclesUnion = std::move(intermediateBuffer);
    }
    //! Попытка спасти ситуацию перестроением coveringCirclesUnion с нуля.
    catch (...)
    {
      intermediateBuffer.clear();
      coveringCirclesUnion.clear();
      try
      {
        for (const PolygonAndItsCoveringCircle& obj : currentCirclesList)
        {
          AbstractOperations::unionOf(coveringCirclesUnion,
                                      obj.circleAsPolygon,
                                      intermediateBuffer);
          coveringCirclesUnion = std::move(intermediateBuffer);
        }
      }
      catch (...)
      {
        throw GeometryFailure(StandartFailureEnum::UndefinedFailure);
      }
    }
    areaProportion = AbstractOperations::area(polygon)
                     / AbstractOperations::area(coveringCirclesUnion);

  }
  //! Пробуем отфильтровать круги (достаточно долгая операция!).
  std::list<CircleWithItsPolygon> circlesWithItsPolygons;
  for (const PolygonAndItsCoveringCircle& obj : currentCirclesList)
    circlesWithItsPolygons.emplace_back(
      std::move(obj.circle),
      std::move(obj.circleAsPolygon));
  try
  {
    circleFilter(circlesWithItsPolygons, polygon);
  }
  catch (...) {}

  //! Наконец, переписываем и возвращаем данные.

  std::vector<Circle> result;
  result.reserve(circlesWithItsPolygons.size());
  auto circlesEnd(circlesWithItsPolygons.end());
  for (auto it(circlesWithItsPolygons.begin()); it != circlesEnd ; ++it)
    result.emplace_back(it->first);
  return result;
}
//==============================================================================
int pointsCountForCircumscribedPolygon(double radius, double sideLength)
{
  // Радиус не может быть отрицательным
  if (radius < 0.)
    throw GeometryFailure(GeometryEnumFailure::NegativeRadius);

  // Длина стороны не может быть отрицательной
  if (sideLength < 0.)
    throw GeometryFailure(GeometryEnumFailure::NegativePolygonSideLength);

  // Для окружности с нулевым радиусом нет подходящего многоугольника
  if (radius < ACCURACY_DOUBLE)
    throw GeometryFailure(GeometryEnumFailure::PointsCountForPolygonNotValid);

  // Не задано ограничение на длину стороны многоугольника - не можем найти
  // максимальное количество вершин
  if (sideLength < ACCURACY_DOUBLE)
    throw GeometryFailure(GeometryEnumFailure::NoMaximumPointsCountForPolygon);

  // Половина центрального угла правильного многоугольника,
  // описанного около окружности
  Radian halfCentralAngle(std::atan(sideLength / (2 * radius)));
  // Количество вершин многоугольника
  int pointsCount(std::floor(M_PI / halfCentralAngle.get()));

  // Меньше трёх вершин быть не может, если такое получилось, найти
  // многоугольник не удалось
  if (pointsCount < 3)
    throw GeometryFailure(GeometryEnumFailure::PointsCountForPolygonNotValid);

  return pointsCount;
}
//==============================================================================
} //! Geometry
//==============================================================================