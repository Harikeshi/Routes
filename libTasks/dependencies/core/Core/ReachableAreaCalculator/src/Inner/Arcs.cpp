#include <Inner/Arcs.hpp>

#include <ReachableArea/ReachableAreaException.hpp>
//==============================================================================
#ifndef Q_MOC_RUN
//! PrimaryEntities
#include <PrimaryEntities/Entities/Box.hpp>
//! AbstractOperations
#include <AbstractOperations/DeleteExcessVertices.hpp>
#endif
//==============================================================================
using namespace ReachableArea;
//==============================================================================
namespace ReachableAreaCalculator
{
//==============================================================================
Arc::Arc(
  const Point2D& center,
  const uinttype centerId,
  const double radius,
  const Angle& beginAngle,
  const Angle& endAngle,
  const bool isEmpty /*= true*/) noexcept :
  mCenter(center),
  mCenterId(centerId),
  mRadius(radius),
  mBeginAngle(beginAngle),
  mSize(mBeginAngle.deltaAngleCounterclockwise(endAngle)),
  mBox(center, center)
{
  if ((BasicMath::isEqual(beginAngle.value(), endAngle.value()))
    && (!isEmpty))
    mSize = 2. * M_PI;
  recalcBox();
}
//==============================================================================
void Arc::setBeginAngle(Angle angle, bool isEmpty /* = true*/) noexcept
{
  Angle end(mBeginAngle.value() + mSize);
  if ((BasicMath::isEqual(angle.value(), end.value())) && (!isEmpty))
    mSize = 2. * M_PI;
  else
    mSize = angle.deltaAngleCounterclockwise(end);
  mBeginAngle = angle;
  recalcBox();
}
//==============================================================================
void Arc::setEndAngle(Angle angle, bool isEmpty /* = true*/) noexcept
{
  if ((BasicMath::isEqual(mBeginAngle.value(), angle.value())) && (!isEmpty))
    mSize = 2. * M_PI;
  else
    mSize = mBeginAngle.deltaAngleCounterclockwise(angle);
  recalcBox();
}
//==============================================================================
void Arc::
setBeginEndAngles(
  Angle beginAngle,
  Angle endAngle,
  bool isEmpty /* = true*/) noexcept
{
  mBeginAngle = beginAngle;

  if ((BasicMath::isEqual(beginAngle.value(), endAngle.value())) && (!isEmpty))
    mSize = 2. * M_PI;
  else
    mSize = mBeginAngle.deltaAngleCounterclockwise(endAngle);
  recalcBox();
}
//==============================================================================
Point2D Arc::beginPoint() const noexcept
{
  return Point2D(mCenter.getX() + mRadius * std::cos(mBeginAngle.value()),
    mCenter.getY() + mRadius * std::sin(mBeginAngle.value()));
}
//==============================================================================
Point2D Arc::endPoint() const noexcept
{
  return Point2D(mCenter.getX() + mRadius * std::cos(endAngle().value()),
    mCenter.getY() + mRadius * std::sin(endAngle().value()));
}
//==============================================================================
bool Arc::isPointInSector(const Point2D& point) const noexcept
{
  if (Geometry::distance(mCenter, point) > mRadius)
    return false;

  Angle polarAngle = Geometry::lineAngularity(mCenter, point).get();

  if (mBeginAngle.deltaAngleCounterclockwise(polarAngle) > mSize)
    return false;

  return true;
}
//==============================================================================
void Arc::
line(std::vector<Point2D>& resultArc, uinttype segmentsNumber) const noexcept
{
  double beginningAngle = mBeginAngle.value();
  double delta = double(2 * M_PI / segmentsNumber);

  for (double shift = 0; shift < mSize; shift += delta)
  {
    resultArc.emplace_back(
      double(mCenter.getX() + mRadius * std::cos(beginningAngle + shift)),
      double(mCenter.getY() + mRadius * std::sin(beginningAngle + shift)));
  }

  resultArc.emplace_back(
    double(mCenter.getX() + mRadius * std::cos(beginningAngle + mSize)),
    double(mCenter.getY() + mRadius * std::sin(beginningAngle + mSize)));
}
//==============================================================================
void Arc::recalcBox() noexcept
{
  // Если все - нули, коробка могла создаться по умолчанию,
  // на всякий случай пересчитаем
  if (mBox.max_corner().getX() == 0 && mBox.max_corner().getY() == 0
    && mBox.min_corner().getX() == 0 && mBox.min_corner().getY() == 0)
  {
    mBox = Box2D(beginPoint(), beginPoint());
  }
  else
  {
    mBox.expandToPoint(beginPoint());
  }
  
  if (mBeginAngle.deltaAngleCounterclockwise(Angle(0)) <= mSize)
  {
    Point2D point(mCenter.getX() + mRadius, mCenter.getY());
    mBox.expandToPoint( point );
  }

  if (mBeginAngle.deltaAngleCounterclockwise( Angle(M_PI)) <= mSize)
  {
    Point2D point(mCenter.getX() - mRadius, mCenter.getY());
    mBox.expandToPoint( point );
  }

  if (mBeginAngle.deltaAngleCounterclockwise(Angle(M_PI / 2.0)) <= mSize)
  {
    Point2D point(mCenter.getX(), mCenter.getY() + mRadius);
    mBox.expandToPoint( point );
  }

  if (mBeginAngle.deltaAngleCounterclockwise(Angle(3.0 * M_PI / 2.0)) <= mSize)
  {
    Point2D point(mCenter.getX(), mCenter.getY() - mRadius);
    mBox.expandToPoint( point );
  }

  mBox.expandToPoint( endPoint( ) );
}
//==============================================================================
Polyarc::Polyarc(const Arc& arc) noexcept
  : mBox(arc.box())
{
  mArcs.push_back(arc);
}
//==============================================================================
Polyarc::Polyarc(const std::deque<Arc>& arcs) noexcept
  : mBox(Point2D(), Point2D())
{
  mArcs = arcs;
  mBox = mArcs[0].box();

  for (size_t i = 1; i < arcs.size(); ++i)
    mBox.expandToBox(arcs[i].box());
}
//==============================================================================
const Arc& Polyarc::beginArc() const
{
  if (mArcs.size() == 0)
    throw ReachableAreaFailure(ArcsArcReferenceFromEmptyPolyarc);
  return mArcs.front();
}
//==============================================================================
const Arc& Polyarc::endArc() const
{
  if (mArcs.size() == 0)
    throw ReachableAreaFailure(ArcsArcReferenceFromEmptyPolyarc);
  return mArcs.back();
}
//==============================================================================
bool Polyarc::addToBegin(const Arc& arc) noexcept
{
  // возможность добавить - соответствие типов обрыва, 
  // идентификаторов и родительских отношений заданий.
  // Добавлена может быть только дуга из следующего поколения,
  // оборванная зоной обзора расчёта если начальная дуга полидуги 
  // оборвана точкой преломления
  if (mArcs.empty())
    return false;
  bool canAdd = ((arc.parentTaskId() == beginArc().taskId()) &&
    (arc.endInfo() == ByViewZone) &&
    (beginArc().beginInfo() == ByRefPoint) &&
    (arc.centerId() == beginArc().arcBeginInfo()));

  if (!canAdd)
    return false;

  Arc newArc = arc;

  // Исправление перекрытия зон обзора.
  // При расчёте зона обзора потомка больше требуемой на mZonesCross,
  // но при склейке перекрытие убирается.
  if (arc.beginAngle().deltaAngleCounterclockwise(beginArc().beginAngle())
  > arc.size())
  {
    newArc.setBeginEndAngles(beginArc().beginAngle(),
      beginArc().beginAngle());
  }
  else
  {
    newArc.setEndAngle(beginArc().beginAngle());
  }

  mArcs.push_front(newArc);
  mBox.expandToBox(arc.box());

  return true;
}
//==============================================================================
bool Polyarc::addToEnd(const Arc& arc) noexcept
{
  // логика аналогична addToBegin
  if (mArcs.empty())
    return false;
  bool canAdd = ((arc.parentTaskId() == endArc().taskId()) &&
    (arc.beginInfo() == ByViewZone) &&
    (endArc().endInfo() == ByRefPoint) &&
    (arc.centerId() == endArc().arcEndInfo()));

  if (!canAdd)
    return false;

  Arc newArc = arc;

  if (endArc().endAngle().deltaAngleCounterclockwise(arc.endAngle())
    > arc.size())
  {
    newArc.setBeginEndAngles(endArc().endAngle(),
      endArc().endAngle());
  }
  else
  {
    newArc.setBeginAngle(endArc().endAngle());
  }

  mArcs.push_back(newArc);
  mBox.expandToBox(arc.box());

  return true;
}
//==============================================================================
void Polyarc::addToEndWithoutCheck(const Polyarc& polyarc) noexcept
{
  mArcs.insert(mArcs.end(), polyarc.mArcs.begin(), polyarc.mArcs.end());

  for (const Arc & arc : polyarc.arcs())
    mBox.expandToBox(arc.box());
}
//==============================================================================
void Polyarc::line(
  std::vector<Point2D>& resultPolyarc, uinttype segmentsNumber) const noexcept
{
  for (const Arc& arc : mArcs)
    arc.line(resultPolyarc, segmentsNumber);
}
//==============================================================================
bool Polyarc::getArc(
  uinttype arcId, Angle angle, uinttype& position, Arc& arc) const noexcept
{
  for (uinttype i = 0; i < mArcs.size(); i++)
  {
    if (mArcs[i].id() == arcId &&
      mArcs[i].beginAngle().deltaAngleCounterclockwise(angle)
      <= mArcs[i].size())
    {
      position = i;
      arc = mArcs[i];
      return true;
    }
  }

  return false;
}
//==============================================================================
bool Polyarc::isClosed() const noexcept
{
  if (mArcs.empty())
    return false;

  bool isZeroLength = true;

  for (const Arc & arc : mArcs)
  {
    if (arc.size() > 0)
    {
      isZeroLength = false;
      break;
    }
  }

  if (isZeroLength)
    return false;

  Point2D p1(beginArc().center().getX() +
    beginArc().radius() * std::cos(beginArc().beginAngle().value()),
    beginArc().center().getY() +
    beginArc().radius() * std::sin(beginArc().beginAngle().value()));

  Point2D p2(endArc().center().getX() +
    endArc().radius() * std::cos(endArc().endAngle().value()),
    endArc().center().getY() +
    endArc().radius() * std::sin(endArc().endAngle().value()));

  return (Geometry::distance(p1, p2)
          < (std::numeric_limits<double>::epsilon() * 10
          * std::max(Geometry::distance(p1, Point2D(0., 0.)),
                     Geometry::distance(p2, Point2D(0., 0.)))));
}
//==============================================================================
Polygon2D Polyarc::toPolygon(uinttype segmentsNumber /*= 360U*/) const noexcept
{
  std::vector<Point2D> vertices;
  line(vertices, segmentsNumber);

  /*!
   * Метод line создаёт кольцо с направлением обхода "против часовой стрелки",
   * поэтому массив требуется инвертировать.
   */
  AbstractOperations::deleteExcessVertices(vertices, true);

  Polygon2D result;
  result.outer().insert(
    result.outer().begin(),
    vertices.begin(),
    vertices.end());
  result.outer().push_back(result.outer().front());

  return result;
}
//==============================================================================
void SemiarcContour::push(const Polyarc& polyarc) noexcept
{
  mPolyarcs.push_back(polyarc);
  mElemTypes.push_back(false);
}
//==============================================================================
void SemiarcContour::push(const Point2D& point) noexcept
{
  mPoints.push_back(point);
  mElemTypes.push_back(true);
}
//==============================================================================
void SemiarcContour::push(const std::vector<Point2D>& polyline) noexcept
{
  for (const Point2D& point : polyline)
  {
    mPoints.push_back(point);
    mElemTypes.push_back(true);
  }
}
//==============================================================================
void SemiarcContour::line(
  std::vector<Point2D>& resultSemiarc, uinttype segmentsNumber) const noexcept
{
  // счетчик для полидуг
  uinttype polyarcCounter = 0; 
  // счетчик для точек
  uinttype pointCounter = 0; 

  for (bool isPoint : mElemTypes)
  {
    if (isPoint)
      resultSemiarc.push_back(mPoints[pointCounter++]);
    else
      mPolyarcs[polyarcCounter++].line(resultSemiarc, segmentsNumber);
  }
}
//==============================================================================
Polygon2D SemiarcContour::
toPolygon(uinttype segmentsNumber /*= 360U*/) const noexcept
{
  std::vector<Point2D> vertices;
  line(vertices, segmentsNumber);

  /*!
   * Метод line создаёт кольцо с направлением обхода "против часовой стрелки",
   * поэтому массив требуется инвертировать.
   */
  AbstractOperations::deleteExcessVertices(vertices, true);

  Polygon2D result;
  result.outer().insert(
    result.outer().begin(),
    vertices.begin(),
    vertices.end());
  result.outer().push_back(result.outer().front());

  return result;
}
//==============================================================================
ArcManager::ArcManager(const GeometryEnvironment& environment) noexcept
  : mEnvironment(environment)
{
  mNeedAbortMerge = false;
}
//==============================================================================
void ArcManager::push(const std::vector<Arc>& arcs) noexcept
{
  for (const Arc & curArc : arcs)
  {
    Arc arc = curArc;
    arc.setId(mArcIdCounter++);
    bool addedToPolyarc = false;

    // пытаемся подклеить к кому-то
    for (Polyarc & polyarc : mPolyarcs)
    {
      if (polyarc.addToBegin(arc) || polyarc.addToEnd(arc))
      {
        addedToPolyarc = true;
        break;
      }
    }

    // подклеить не удалось
    if (!addedToPolyarc)
      mPolyarcs.emplace_back(arc);
  }
}
//==============================================================================
void ArcManager::
merge(
  const std::set<uinttype>& touchedPolygons,
  ReachableAreaResult& result,
  uinttype segmentsNumber)
{
  result.clear();
  // mPolyarcs уже заполнен

  // процесс не прерван
  mNeedAbortMerge = false;

  // удалить дуги, полностью попадающие под другие
  deleteAbsorbedArcs();

  if (mNeedAbortMerge)
    return;

  // пересечь все полидуги
  intersectAllPolyarcs();

  // Имеем полидуги, не имеющие самопересечений

//        for (const Polyarc & pa : mPolyarcs)
//        {
//            Polygon pll;
//            pa.line(pll);
//            result.borders.push_back(pll);
//        }
//        return;

  if (mNeedAbortMerge)
    return;

  EdgesArcInfo edgesArcInfo;

  /*! 
   * Определение таблицы соответствий между
   * границами и лежащими на них терминальными точками полидуг.
   */
  calcEdgesArcInfo(edgesArcInfo);
  std::vector<bool> polyarcsActualities;

  // убрать из рассмотрения неактуальные полидуги
  calcPolyarcsActualities(edgesArcInfo, polyarcsActualities);

  if (mNeedAbortMerge)
    return;

  // сохранение меток актуальности всех граничных полидуг 
  // (для результата)
  const std::vector<bool> polyarcsActualitiesAll = polyarcsActualities;
  uinttype actualPolyarcsCount = 0;

  // посчитать, сколько актуальных полидуг на момент начала склейки
  for (bool b : polyarcsActualities)
  {
    if (b)
      actualPolyarcsCount++;
  }

  // счётчик обработанных полидуг
  uinttype proceededPolyarcsCount = 0;

  // полигоны, участки границ которых будут содержаться в 
  // результирующем контуре (т.е. такие, на которых лежат
  // терминальные точки полидуг)
  std::set<uinttype> touchedByPolyarcsPolygons;

  // АЛГОРИТМ СБОРКИ КОНТУРОВ:
  // 1. Если нет актуальных полидуг, завершить.
  // 2. Взять любую актуальную и назначить её текущей,
  //    а также запомнить её как первую для контура
  // 3. По текущей дойти до границы.
  // 4. Идти по границе по направлению обхода до терминальной точки
  //    следующей актуальной полидуги.
  // 5. Снять признак актуальности у текущей.
  // 6. Если следующая есть первая, завершить сборку контура, 
  //    положить его в накопитель и перейти на шаг 1.
  // 7. Назначить следующую текущей.
  // 8. Перейти на шаг 3.

  // собранные контура
  std::vector<SemiarcContour> contours;
  // позиция начальной полидуги текущего собираемого
  // контура в mPolyarcs
  uinttype startPolyarcId;

  do
  {
    // инициализация заведемо некорректным значением
    startPolyarcId = uinttype(mPolyarcs.size());

    // перебор для определения первого актуального - с него
    // начнётся сборка очередного контура
    for (uinttype i = 0; i < mPolyarcs.size(); i++)
    {
      if (polyarcsActualities[i])
      {
        startPolyarcId = i;
        break;
      }
    }

    // сборка нового контура
    if (startPolyarcId < mPolyarcs.size())
    {
      // собираемый контур
      SemiarcContour contour;
      // пока что не замкнут
      bool contourClosed = false;
      // текущая перебираемая полидуга
      uinttype currentPolyarcId = startPolyarcId;

      // пока контур не замкнут...
      while (!contourClosed)
      {
        // положить текущую полидугу в контур
        contour.push(mPolyarcs[currentPolyarcId]);
        //сбросить флаг актуальности (обработанная есть неактуальная)
        polyarcsActualities[currentPolyarcId] = false;
        // послать сигнал, что процесс идёт
        emit mergeInfo(BordersDetectionStage,
          ++proceededPolyarcsCount, actualPolyarcsCount);

        if (mNeedAbortMerge)
          return;

        // если текущая самозамкнутая
        if (mPolyarcs[currentPolyarcId].isClosed())
        {
          // завершить сборку контура
          contourClosed = true;
          break;
        }

        // точка, определяющая границу, в которую уперлась
        // завершающая точка полидуги
        uinttype edgeBeginPointId =
          mPolyarcs[currentPolyarcId].endArc().arcEndInfo();

        // отмечаем, что коснулись полигона
        touchedByPolyarcsPolygons.insert(
          mEnvironment.getPoint(edgeBeginPointId).polygonId());

        // Поиск информации о терминальных точках на данной границе
        auto it = edgesArcInfo.find(edgeBeginPointId);
        // предполагаем, что не нашли, и с этой границы деться некуда
        bool polyarcBeginFounded = false;

        // если всё же нашли
        if (it != edgesArcInfo.end())
        {
          // тащим набор терминальных точек для границы
          const std::vector<ArcTerminatePointOnEdge>&
            atpoes = it->second;
          bool needContinue = true;

          for (const ArcTerminatePointOnEdge& atpoe : atpoes)
          {
            // пропускаем все точки, следующие до
            // точки, в которую пришли
            if (needContinue)
            {
              if (atpoe.arcPos == currentPolyarcId &&
                !atpoe.beginPoint)
                needContinue = false;
              continue;
            }

            /*!
             * Здесь мы рано или поздно должны попасть
             * в начальную точку некоторой полидуги,
             * если это не так, значит, алгоритм следует доработать :)
             */
            if (atpoe.beginPoint)
            {
              //! Найден выход с границы многоугольника на очередную дугу.
              polyarcBeginFounded = true;
              //! Меняем номер текущей полидуги.
              currentPolyarcId = atpoe.arcPos;
              //! Если попали в начальную полидугу...
              if (currentPolyarcId == startPolyarcId)
                //! Получается, замкнули контур.
                contourClosed = true;
              /*!
               * Прекращаем перебор терминальных точек
               * на стороне многоугольника.
               */
              break;
            }
          }
        }

        if (!polyarcBeginFounded)
        {
          // если на следующую полидугу не перешли => надо идти по
          // границе полигона

          // полигон, по которому нужно идти
          const EnvironmentPolygon& plg = mEnvironment.getPolygon(
              mEnvironment.getPoint(edgeBeginPointId).polygonId());

          // Текущая точка.
          // Инициализируется следующей по контуру вершиной
          uinttype currentPointId = plg.nextPointId(edgeBeginPointId);
          uinttype firstBorderPointId = currentPointId;

          // Пока не найдётся полидуга, на которую можно перейти
          while (!polyarcBeginFounded) 
          {
            // положить текущую в контур
            contour.push(mEnvironment.getPoint(currentPointId));

            //! Попытаемся найти актуальную полидугу, чтобы уйти с полигона.

            //!< Текущая рассматриваемая граница.
            auto edgeIt = edgesArcInfo.find(currentPointId);

            if (edgeIt != edgesArcInfo.end())
            {
              /*!<
               * Ссылка на терминальные точки полидуг,
               * расположенные на этой границе.
               */
              const std::vector<ArcTerminatePointOnEdge>&
                atpoes = edgeIt->second;

              /*!
               * Просматриваем все терминальные точки
               * в порядке удаления от первой точки границы.
               */
              for (const ArcTerminatePointOnEdge& atpoe : atpoes)
              {
                /*!
                 * Если найденная точка принадлежит актуальной
                 * или начальной полидуге...
                 */
                if (polyarcsActualities[atpoe.arcPos] ||
                  atpoe.arcPos == startPolyarcId)
                {
                  /*!
                   * Если окружение и полидуги заданы корректно,
                   * такого произойти не должно!
                   */
                  if (!atpoe.beginPoint)
                    throw ReachableAreaFailure(
                                            ArcsNotBeginTerminalPointOnBorder);
                  polyarcBeginFounded = true;
                  currentPolyarcId = atpoe.arcPos;
                  break;
                }
              }
            }

            // меняем текущую точку на следующую
            currentPointId = plg.nextPointId(currentPointId);

            // проверяем, что не зациклились (при корректной работе такого не бывает!)
            if( ( currentPointId == firstBorderPointId ) && ( !polyarcBeginFounded ) )
              throw ReachableAreaFailure( SingleArcEndsAtBorder );

          }

          // пришли в начало
          if (currentPolyarcId == startPolyarcId)
          {
            // контур замкнулся
            contourClosed = true;
          }
        }
      }

      //замкнутый контур положить в накопитель
      contours.push_back(contour);
    }

  // пока значение корректно, то есть есть необработанные
  // (актуальные) полидуги
  } while (startPolyarcId < mPolyarcs.size()); 

  // Перебираем все изначально актуальные и кладём их в результат
  for (uinttype i = 0; i < mPolyarcs.size(); i++)
  {
    if (polyarcsActualitiesAll[i])
    {
      std::vector<Point2D> polyarcLine;
      mPolyarcs[i].line(polyarcLine, segmentsNumber);
      result.borders.push_back(polyarcLine);
    }
  }

  //! Очистка информации о дугах (они больше не нужны).
  clearArcs();

  // перебор всех найденных контуров и разбор их на 
  // внешний / внутренний
  for (const SemiarcContour& sac : contours)
  {
    std::vector<Point2D> plg;
    // контур переходит в полигон
    sac.line(plg, segmentsNumber); 

    /*!
     * Исправляем полигон.
     * Все контуры строились с обходом "против часовой стрелки".
     * Данная функция сменит направление обхода обратно (если сможет),
     * и мы получим корректный внешний контур.
     */
    if (AbstractOperations::deleteExcessVertices(plg, true))
      result.area.outer().insert(
        result.area.outer().end(),
        plg.begin(),
        plg.end());
  }

  // Добавление в качестве контуров полигонов среды без дуг
  // (полностью достижимые острова или, в случае озёр, внешняя граница)

  // счётчик обработанных
  uinttype cnt = 0; 

  // перебор всех полигонов, у которых достижима хотя бы одна точка
  for (uinttype plgId : touchedPolygons)
  {
    // высылаем сигнал прогресса
    emit mergeInfo(HolesDetectionStage,
      cnt++, uinttype(touchedPolygons.size()));

    if (mNeedAbortMerge)
      return;

    // если полигон полностью достижим
    // (то есть достижима хотя бы одна точка, но на нём нет 
    // терминальных точек полидуг)
    if (touchedByPolyarcsPolygons.find(plgId) ==
      touchedByPolyarcsPolygons.end())
    {
      uinttype ptId0 = mEnvironment.getPolygon(plgId).firstPointId();
      uinttype plgSize = mEnvironment.getPolygon(plgId).pointsCount();
      //! Рассматриваемый полигон.
      std::vector<Point2D> plg; 
      plg.reserve(plgSize);
      for (uinttype i = 0; i < plgSize; i++)
        plg.emplace_back(mEnvironment.getPoint(ptId0 + i));

      //! Исправляем полигон.
      if (AbstractOperations::deleteExcessVertices(plg, true)) 
      {
        //! Если внешняя граница пуста, внешняя граница есть полигон среды.
        if (result.area.outer().empty()) 
        {
          /*!
           * Определяем, текущий полигон внутренний или внешний,
           * по его направлению.
           */
          if (!isPolygonCW(plg))
          {
            Ring2D ring;
            ring.insert(ring.begin(), plg.begin(), plg.end());
            result.area.inners().emplace_back(ring);
          }
          else
            result.area.outer().insert(
              result.area.outer().begin(),
              plg.begin(),
              plg.end());
        }
        else 
        {
          //! Если внешняя граница уже есть, добавляются только внутренние.

          if (isPolygonCW(plg))
            throw ReachableAreaFailure(ArcsClockWiseInnerRing);

          Ring2D ring;
          ring.insert(ring.begin(), plg.begin(), plg.end());
          result.area.inners().emplace_back(ring);
        }
      }
    }
  }
}
//==============================================================================
void ArcManager::abortMerge() noexcept
{
  mNeedAbortMerge = true;
}
//==============================================================================
ArcManager::ArcIntersectInfo ArcManager::
arcIntersect(const Arc& arc1, const Arc& arc2) const noexcept
{
  // результат
  ArcIntersectInfo res;

  // проверка коробок
  if ( ! arc1.box().hasIntersectionWith(arc2.box())) 
  {
    res.intersectType = NoIntersect;
    return res;
  }

  // центры дуг
  const Point2D& c1 = arc1.center();   
  const Point2D& c2 = arc2.center();
  // радиусы дуг
  double r1 = arc1.radius();          
  double r2 = arc2.radius();
  // расстояние между центрами
  double l = Geometry::distance(c1, c2);  

  if (l > r1 + r2 || l == 0 || arc1.size() == 0 || arc2.size() == 0)
  {
    res.intersectType = NoIntersect;
    return res;
  }

  //!< Угол наклона прямой, соединяющей два центра, относительно Ox.
  Angle fi = Geometry::lineAngularity(c1, c2).get();

  // возможно касание в одной точке
  if (l == r1 + r2)           
  {
    // направление от первого на точку касания
    Angle az1 = fi;
    // ...от второго
    Angle az2 = Angle(M_PI + fi.value());

    // попадает ли точка касания в обе дуги
    bool valid 
      = (arc1.beginAngle().deltaAngleCounterclockwise(az1) <= arc1.size()
      && arc2.beginAngle().deltaAngleCounterclockwise(az2) <= arc2.size());

    if (valid)
    {
      //! Обратите внимание на следующую строку :)
      res.intersectType = DoubleIntersect;
      res.point1.angle1 = az1;
      res.point1.angle2 = az2;
      res.point1.point = Point2D(c1.getX() + r1 * std::cos(az1.value()),
        c1.getY() + r1 * std::sin(az1.value()));
      res.point2 = res.point1;
    }
    else
      res.intersectType = NoIntersect;

    return res;
  }

  // возможны две точки пересечения
  // ищем их пересечением двух окружностей с последующей проверкой

  /*!<
   * Расстояние от центра первой окружности
   * до проекции точек пересечения на линию,
   * соединяющую центры.
   */
  double l1 = 0.5 * (((r1*r1 - r2*r2) / l) + l);
  /*!<
   * Расстояние от центра второй окружности
   * до проекции точек пересечения на линию,
   * соединяющую центры.
   */
  double l2 = l - l1;
  double alpha1 = std::acos(l1 / r1);
  double alpha2 = std::acos(l2 / r2);
  // от ц1 на первую
  Angle az11 = Angle(fi.value() + alpha1);
  // от ц1 на вторую
  Angle az12 = Angle(fi.value() - alpha1);
  // от ц2 на первую
  Angle az21 = Angle(M_PI + fi.value() - alpha2);
  // от ц2 на вторую
  Angle az22 = Angle(M_PI + fi.value() + alpha2);

  // признак попадания в обе дуги для первой точки
  bool i1valid 
    = (arc1.beginAngle().deltaAngleCounterclockwise(az11) <= arc1.size()
      && arc2.beginAngle().deltaAngleCounterclockwise(az21) <= arc2.size());

  // признак попадания в обе дуги для второй точки
  bool i2valid 
    = (arc1.beginAngle().deltaAngleCounterclockwise(az12) <= arc1.size()
      && arc2.beginAngle().deltaAngleCounterclockwise(az22) <= arc2.size());

  if (i1valid)
  {
    res.point1.angle1 = az11;
    res.point1.angle2 = az21;
    res.point1.point = Point2D(c1.getX() + r1 * std::cos(az11.value()),
      c1.getY() + r1 * std::sin(az11.value()));

    if (i2valid)
    {
      res.intersectType = DoubleIntersect;
      res.point2.angle1 = az12;
      res.point2.angle2 = az22;
      res.point2.point = Point2D(c1.getX() + r1 * std::cos(az12.value()),
        c1.getY() + r1 * std::sin(az12.value()));
    }
    else
      res.intersectType = SingleIntersect;
  }
  else
  {
    if (i2valid)
    {
      res.intersectType = SingleIntersect;
      res.point1.angle1 = az12;
      res.point1.angle2 = az22;
      res.point1.point = Point2D(c1.getX() + r1 * std::cos(az12.value()),
        c1.getY() + r1 * std::sin(az12.value()));
    }
    else
      res.intersectType = NoIntersect;
  }

  return res;
}
//==============================================================================
void ArcManager::
calcAllIntersects(std::vector<ArcIntersect>& intersects) const noexcept
{
  intersects.clear();
  uinttype intersectIdCounter = 1;

  // перебор всех дуг всех полидуг со всеми дугами всех полидуг
  // и нахождение соответствующих пересечений
  for (std::size_t i = 0; i < mPolyarcs.size(); i++)
  {
    emit mergeInfo(
      IntersectsDetectionStage,
      uinttype(i),
      uinttype(mPolyarcs.size()));

    if (mNeedAbortMerge)
      return;

    const Polyarc& poly1 = mPolyarcs[i];

    for (std::size_t j = i; j < mPolyarcs.size(); j++)
    {
      const Polyarc& poly2 = mPolyarcs[j];

      if ( ! poly1.box().hasIntersectionWith(poly2.box()) )
        continue;

      for (std::size_t v = 0; v < poly1.arcs().size(); v++)
      {
        for (std::size_t w((i == j) ? v + 2 : 0); w < poly2.arcs().size(); w++)
        {
          if (i == j)
          {
            bool cont = true;

            for (std::size_t p = v + 1; p < w; p++)
            {
              if (poly1.arcs()[p].size() > 0)
              {
                cont = false;
                break;
              }
            }

            if (cont)
              continue;
          }

          ArcIntersectInfo info =
            arcIntersect(poly1.arcs()[v], poly2.arcs()[w]);

          if (info.intersectType != NoIntersect)
          {
            ArcIntersect intersect;
            intersect.active = true;
            intersect.arc1id = poly1.arcs()[v].id();
            intersect.arc2id = poly2.arcs()[w].id();
            intersect.id = intersectIdCounter++;
            intersect.intersectPoint = info.point1;
            intersects.push_back(intersect);

            if (info.intersectType == DoubleIntersect)
            {
              intersect.id = intersectIdCounter++;
              intersect.intersectPoint = info.point2;
              intersects.push_back(intersect);
            }
          }
        }
      }
    }
  }
}
//==============================================================================
// TODO: Можно убрать и вынести инверсию в метод структуры
ArcManager::ArcIntersect ArcManager::
reverseIntersect(const ArcManager::ArcIntersect &intersect) const noexcept
{
  ArcIntersect intersectReverse;
  intersectReverse.arc1id = intersect.arc2id;
  intersectReverse.arc2id = intersect.arc1id;
  intersectReverse.active = intersect.active;
  intersectReverse.id = intersect.id;
  intersectReverse.intersectPoint.angle1 = intersect.intersectPoint.angle2;
  intersectReverse.intersectPoint.angle2 = intersect.intersectPoint.angle1;
  intersectReverse.intersectPoint.point = intersect.intersectPoint.point;
  return intersectReverse;
}
//==============================================================================
bool ArcManager::getIntersectsForArc(
  const std::vector<ArcIntersect>& allIntersects,
  const Arc& arc,
  std::vector<ArcManager::ArcIntersect>& intersects) const noexcept
{
  intersects.clear();

  // перебор всех пересечений и нахождение подходящих
  for (const ArcIntersect& intersect : allIntersects)
  {

    if (!intersect.active)
      continue;

    if (intersect.arc1id == arc.id())
    {
      if (arc.beginAngle().deltaAngleCounterclockwise(
        intersect.intersectPoint.angle1) <= arc.size())
        intersects.push_back(intersect);
    }
    else if (intersect.arc2id == arc.id())
    {
      ArcIntersect intersectReverse = reverseIntersect(intersect);

      if (arc.beginAngle().deltaAngleCounterclockwise(
        intersectReverse.intersectPoint.angle1) <= arc.size())
        intersects.push_back(intersectReverse);
    }
  }

  // сортировка результата по возрастанию угла от центра 
  // на пересечение, считая начальный угол дуги началом отсчёта
  std::sort(intersects.begin(), intersects.end(),
    [&arc](const ArcIntersect& i1, const ArcIntersect& i2)
  {
    return
      arc.beginAngle().deltaAngleCounterclockwise(i1.intersectPoint.angle1)
      < arc.beginAngle().deltaAngleCounterclockwise(i2.intersectPoint.angle1);
  });

  return !intersects.empty();
}
//==============================================================================
void ArcManager::splitPolyarc(const Polyarc& src,
  const ArcManager::ArcIntersect& splitter,
  Polyarc& part1, Polyarc& part2) const
{
  uinttype splittingArcPos;
  Arc splittingArc;
  src.getArc(splitter.arc1id, splitter.intersectPoint.angle1,
    splittingArcPos, splittingArc);

  std::deque<Arc> arcs;
  for (uinttype i = 0; i < splittingArcPos; i++)
    arcs.push_back(src.arcs()[i]);

  Arc arc = splittingArc;
  arc.setEndAngle(splitter.intersectPoint.angle1);
  arc.setEndInfo(ByCut);
  arcs.push_back(arc);
  part1 = Polyarc(arcs);

  arcs.clear();
  arc = splittingArc;
  arc.setBeginAngle(splitter.intersectPoint.angle1);
  arc.setBeginInfo(ByCut);
  arcs.push_back(arc);

  for (uinttype i = splittingArcPos + 1; i < src.arcs().size(); i++)
    arcs.push_back(src.arcs()[i]);

  part2 = Polyarc(arcs);
}
//==============================================================================
void ArcManager::absorbPolyarc(const Polyarc& src, const Polyarc& absorber,
  std::vector<Polyarc>& res) const noexcept
{
  res.clear();
  std::deque<Arc> arcs;

  for (const Arc& arc : src.arcs())
  {
    // дуга поглощена полидугой, если обе ее терминальные точки
    // находятся под полидугой
    bool beginAbsorbed = false;
    bool endAbsorbed = false;
    Point2D beginPoint = arc.beginPoint();
    Point2D endPoint = arc.endPoint();

    for (const Arc& absorberArc : absorber.arcs())
    {
      if (!beginAbsorbed && absorberArc.isPointInSector(beginPoint))
        beginAbsorbed = true;

      if (!endAbsorbed && absorberArc.isPointInSector(endPoint))
        endAbsorbed = true;

      if (beginAbsorbed && endAbsorbed)
        break;
    }

    bool arcAbsorbed = beginAbsorbed && endAbsorbed;

    // дуга поглощена - собираемая полидуга обрывается
    if (arcAbsorbed) 
    {
      if (!arcs.empty())
      {
        res.emplace_back(arcs);
        arcs.clear();
      }
    }
    else 
    {
      // дуга не поглощена - продолжаем собирать полидугу
      arcs.push_back(arc);
    }
  }

  if (!arcs.empty())
    res.emplace_back(arcs);
}
//==============================================================================
void ArcManager::absorbPolyarcs(
  std::vector<Polyarc>& polyarcs,
  const Polyarc& absorber) const noexcept
{
  if (polyarcs.empty())
    return;

  std::vector<Polyarc> res;

  for (const Polyarc& polyarc : polyarcs)
  {
    std::vector<Polyarc> iterationRes;
    absorbPolyarc(polyarc, absorber, iterationRes);
    res.insert(res.end(), iterationRes.begin(), iterationRes.end());
  }

  polyarcs.swap(res);
}
//==============================================================================
void ArcManager::deleteAbsorbedArcs() noexcept
{
  std::vector<Polyarc> res;

  for (uinttype i = 0; i < mPolyarcs.size(); i++)
  {
    emit mergeInfo(PolyarcsClearingStage, i, uinttype(mPolyarcs.size()));

    if (mNeedAbortMerge)
      return;

    std::vector<Polyarc> iterationRes;
    iterationRes.push_back(mPolyarcs[i]);
    Box2D srcBox = mPolyarcs[i].box();

    for (uinttype j = 0; j < mPolyarcs.size(); j++)
    {
      if (j == i)
        continue;

      Box2D absorberBox = mPolyarcs[j].box();

      for (const Arc & arc : mPolyarcs[j].arcs())
        absorberBox.expandToPoint( arc.center() );

      if ( srcBox.hasIntersectionWith(absorberBox) )
        absorbPolyarcs(iterationRes, mPolyarcs[j]);
    }

    res.insert(res.end(), iterationRes.begin(), iterationRes.end());
  }

  mPolyarcs.swap(res);
}
//==============================================================================
void ArcManager::intersectAllPolyarcs() noexcept
{
  // получить все пересечения
  std::vector<ArcIntersect> allIntersects;
  calcAllIntersects(allIntersects);

  if (mNeedAbortMerge)
    return;

  // готовые
  std::vector<Polyarc> proceeded; 
  // счётчик для прогресса
  uinttype proceededCount = 0;

  // пока все не перекочуют в proceeded
  while (!mPolyarcs.empty()) 
  {
    emit mergeInfo(PolyarcsMergeStage,
      proceededCount++, uinttype(mPolyarcs.size()));

    if (mNeedAbortMerge)
      return;

    // берём последнюю
    Polyarc polyarc = mPolyarcs.back();
    mPolyarcs.pop_back();

    // с кем пересекается
    Polyarc polyarc2; 
    // как пересекается (текущее пересечение)
    ArcIntersect currentIntersect;
    // признак самопересечения
    bool selfIntersect = false;

    // перебор всех дуг полидуги
    for (const Arc & arc : polyarc.arcs()) 
    {
      // заглушки
      uinttype tempui;
      Arc temparc;

      // получить все пересечения для данной дуги
      std::vector<ArcIntersect> intersects;
      getIntersectsForArc(allIntersects, arc, intersects);

      // инициализация перебора всех пересечений для данной дуги
      currentIntersect.active = false; 

      // дуга с кем-то пересекается
      if (!intersects.empty()) 
      {
        const ArcIntersect& intersect = intersects.front();
        selfIntersect = polyarc.getArc(intersect.arc2id,
          intersect.intersectPoint.angle2,
          tempui, temparc);
        currentIntersect = intersect;
      }

      // нет активных пересечений => переход к следующей дуге
      if (!currentIntersect.active)
        continue;

      // несамопересекающаяся
      if (!selfIntersect) 
        // находим polyarc2 - с кем пересекается, и извлекаем её
        // из mPolyarcs
        for (auto it = mPolyarcs.begin(); it != mPolyarcs.end(); it++)
          if (it->getArc(currentIntersect.arc2id,
            currentIntersect.intersectPoint.angle2,
            tempui, temparc))
          {
            polyarc2 = *it;
            mPolyarcs.erase(it);
            break;
          }

      break;
    }

    // для полидуги нет пересечений
    if (!currentIntersect.active) 
    {
      // кинуть в результат и перейти к следующей полидуге
      proceeded.push_back(polyarc); 
      continue;                     
    }

    // пересечение дуг в одной полидуге
    if (selfIntersect) 
    {
      Polyarc temp1;
      Polyarc temp2;
      Polyarc temp3;
      Polyarc temp4;

      // участок итоговой границы ПОСЛЕ точки пересечения
      if (currentIntersect.intersectPoint.angle1.deltaAngle(
        currentIntersect.intersectPoint.angle2) > 0)
      {
        // режем первый раз
        splitPolyarc(polyarc, currentIntersect, temp1, temp2);

        // temp1 выкидывается, т.к. у него пересечений быть не может,
        // и он не может быть участком собранного контура
        // режем второй раз и кладём куски в mPolyarcs
        splitPolyarc(temp2, reverseIntersect(currentIntersect),
          temp3, temp1);
        mPolyarcs.push_back(temp1);
        mPolyarcs.push_back(temp3);
      }
      else 
      {
        // если участок итоговой границы ДО точки пересечения
        splitPolyarc(polyarc, currentIntersect, temp1, temp2);
        splitPolyarc(temp2, reverseIntersect(currentIntersect),
          temp3, temp4);
        // temp3 - замкнутый контур
        mPolyarcs.push_back(temp3);         
        temp1.addToEndWithoutCheck(temp4);
        // склеенные хвосты
        mPolyarcs.push_back(temp1);         
      }
    }
    else 
    {
      // если пересечение дуг в различных полидугах
      Polyarc part11;
      Polyarc part12;
      Polyarc part21;
      Polyarc part22;

      // разрезаем две дуги на четыре куска
      splitPolyarc(polyarc, currentIntersect, part11, part12);
      splitPolyarc(polyarc2, reverseIntersect(currentIntersect),
        part21, part22);

      bool polyarcClosed = polyarc.isClosed();
      bool polyarc2Closed = polyarc2.isClosed();

      // участок итоговой границы ПОСЛЕ точки пересечения
      if (currentIntersect.intersectPoint.angle1.deltaAngle(
        currentIntersect.intersectPoint.angle2) > 0)
      {
        if (polyarcClosed)
        {
          if (polyarc2Closed)
          {
            part22.addToEndWithoutCheck(part21);
            part22.addToEndWithoutCheck(part12);
            part22.addToEndWithoutCheck(part11);
            mPolyarcs.push_back(part22);
          }
          else
          {
            mPolyarcs.push_back(part22);
            part21.addToEndWithoutCheck(part12);
            part21.addToEndWithoutCheck(part11);
            mPolyarcs.push_back(part21);
          }
        }
        else
        {
          if (polyarc2Closed)
          {
            part22.addToEndWithoutCheck(part21);
            part22.addToEndWithoutCheck(part12);
            mPolyarcs.push_back(part22);
          }
          else
          {
            mPolyarcs.push_back(part22);
            part21.addToEndWithoutCheck(part12);
            mPolyarcs.push_back(part21);
          }
        }
      }
      else 
      {
        // если участок итоговой границы ДО точки пересечения
        if (polyarcClosed)
        {
          if (polyarc2Closed)
          {
            part12.addToEndWithoutCheck(part11);
            part12.addToEndWithoutCheck(part22);
            part12.addToEndWithoutCheck(part21);
            mPolyarcs.push_back(part12);
          }
          else
          {
            part12.addToEndWithoutCheck(part11);
            part12.addToEndWithoutCheck(part22);
            mPolyarcs.push_back(part12);
            mPolyarcs.push_back(part21);
          }
        }
        else
        {
          if (polyarc2Closed)
          {
            mPolyarcs.push_back(part12);
            part11.addToEndWithoutCheck(part22);
            part11.addToEndWithoutCheck(part21);
            mPolyarcs.push_back(part11);
          }
          else
          {
            mPolyarcs.push_back(part12);
            mPolyarcs.push_back(part21);
            part11.addToEndWithoutCheck(part22);
            mPolyarcs.push_back(part11);
          }
        }
      }
    }

    // обработали текущее пересечение - делаем его неактивным
    allIntersects[currentIntersect.id - 1].active = false;
  }

  // выбор полидуг, которые могут быть участками итоговых контуров
  // это либо замкнутые полидуги, либо полидуги, имеющие обе 
  // терминальные точки на границах полигонов
  for (const Polyarc& polyarc : proceeded)
    if (!polyarc.arcs().empty())
    {
      const Arc& beginArc = polyarc.beginArc();
      const Arc& endArc = polyarc.endArc();

      if ((beginArc.beginInfo() == ByEdge && endArc.endInfo() == ByEdge) ||
        polyarc.isClosed())
      {
        mPolyarcs.push_back(polyarc);
      }
    }
}
//==============================================================================
void ArcManager::calcEdgesArcInfo(EdgesArcInfo& edgesArcInfo) const
{
  edgesArcInfo.clear();

  // перебор всех полидуг
  for (uinttype i = 0; i < mPolyarcs.size(); i++) 
  {
    //! Замкнутые и пустые не рассматриваются.
    if ((mPolyarcs[i].isClosed()) || (mPolyarcs[i].arcs().empty())) 
      continue;

    if ((mPolyarcs[i].beginArc().beginInfo() != ByEdge)
        || (mPolyarcs[i].endArc().endInfo() != ByEdge))
      throw ReachableAreaFailure(ArcsPolyarcBreakNoByEdge);

    // идентификаторы терминальных точек полидуги
    uinttype edgeBeginPointIdBegin = mPolyarcs[i].beginArc().arcBeginInfo();
    uinttype edgeBeginPointIdEnd = mPolyarcs[i].endArc().arcEndInfo();

    if (!edgeBeginPointIdBegin)
      throw ReachableAreaFailure(ArcsPolyarcNullBeginInfo);
    if (!edgeBeginPointIdEnd)
      throw ReachableAreaFailure(ArcsPolyarcNullEndInfo);

    // начальные точки границ, на которых лежат терминальные точки 
    // полидуги
    const EnvironmentPoint& edgeBeginPointBegin =
      mEnvironment.getPoint(edgeBeginPointIdBegin);
    const EnvironmentPoint& edgeBeginPointEnd =
      mEnvironment.getPoint(edgeBeginPointIdEnd);

    // собственно, терминальные точки
    ArcTerminatePointOnEdge atpoeBegin{ i,true,Geometry::distance(
      edgeBeginPointBegin, mPolyarcs[i].beginArc().beginPoint()) };
    ArcTerminatePointOnEdge atpoeEnd{ i,false,Geometry::distance(
      edgeBeginPointEnd, mPolyarcs[i].endArc().endPoint()) };

    // две пары <граница по id начальной точки, терминальная точка>
    std::pair<uinttype, ArcTerminatePointOnEdge> atpoes[2]{
                                        {edgeBeginPointIdBegin,atpoeBegin},
                                        {edgeBeginPointIdEnd,atpoeEnd} };

    // перебор обеих
    for (const auto & atpoe : atpoes) 
    {
      // ищем, есть ли уже добавленные терминальные точки на границе
      auto it = edgesArcInfo.find(atpoe.first);

      // на этой границе что-то есть
      if (it != edgesArcInfo.end()) 
      {
        // впихиваем и сортируем по удалению от начала границы
        // если расстояния равны, то завершающие точки сначала,
        // ибо так нужно для алгоритма сборки контуров
        it->second.push_back(atpoe.second);
        std::sort(it->second.begin(), it->second.end(),
          [](const ArcTerminatePointOnEdge& atpoe1,
            const ArcTerminatePointOnEdge& atpoe2)
        {
          if (atpoe1.distToEdgeBegin != atpoe2.distToEdgeBegin)
            return atpoe1.distToEdgeBegin < atpoe2.distToEdgeBegin;
          else if (!atpoe1.beginPoint && atpoe2.beginPoint)
            return true;
          else if (atpoe1.beginPoint && !atpoe2.beginPoint)
            return false;
          else
            return false;
        });
      }
      else 
      {
        // если граница чистая, просто впихиваем
        edgesArcInfo.insert(std::make_pair(
          atpoe.first, std::vector<ArcTerminatePointOnEdge>{atpoe.second}));
      }
    }
  }
}
//==============================================================================
void ArcManager::calcPolyarcsActualities(
  const ArcManager::EdgesArcInfo& edgesArcInfo,
  std::vector<bool>& polyarcsActualities) const
{
  polyarcsActualities.clear();
  // сначала все нужные
  polyarcsActualities.assign(mPolyarcs.size(), true); 

  // перебор всех полидуг
  for (uinttype i = 0; i < mPolyarcs.size(); i++) 
  {
    emit mergeInfo(PolyarcsCheckStage, i, uinttype(mPolyarcs.size()));
    if ((mPolyarcs[i].isClosed()) || (mPolyarcs[i].arcs().empty()))
      // TODO: добавить дополнительную проверку полидуг, замкнутых
      // вывернуто, т.е. таких, у которых центры дуг лежат вне 
      // замкнутой области. Либо доказать, что после расчистки 
      // (deleteAbsorbedArcs()) таких полидуг с достижимой областью 
      // внутри быть не может.

      //            const Polyarc & polyarc = mPolyarcs[i];
      //            if (polyarc.arcs().size() < 3)
      //                continue;

      //            bool isNormalClosed = true;
      //            Polygon plg;
      //            polyarc.line(plg);
      //            for (const Arc & arc : polyarc.arcs())
      //            {
      //                if (!pointInPolygon(plg,arc.center()))
      //                {
      //                    isNormalClosed = false;
      //                    break;
      //                }
      //            }
      //       //     qDebug() << "closed found" << isNormalClosed; // debug
      //            if (!isNormalClosed)
      //            {
      //                // TODO проверка
      //              //  polyarcsActualities[i] = false;
      //            }
      continue;

    // идентификатор начальной точки границы, на которой лежит
    // начальная точка полидуги
    uinttype edgeBeginPointId = mPolyarcs[i].beginArc().arcBeginInfo();
    auto it = edgesArcInfo.find(edgeBeginPointId);

    if (it == edgesArcInfo.end())
      throw ReachableAreaFailure(ArcsPolyarcBeginPointNotFound);

    // информация о терминальных точках на этой границе
    const std::vector<ArcTerminatePointOnEdge>& atpoes = it->second;

    // дуга хорошая, пойдёт в склейку
    bool polyarcIsBorder = true;
    // флаг пропуска терминальных точек
    bool needContinue = true;

    // перебор всех терминальных точек на данной границе
    // точки отсортированы по удалению от начальной точки границы
    for (const ArcTerminatePointOnEdge& atpoe : atpoes)
    {
      // пропускать все терминальные точки до точки исследуемой дуги
      if (needContinue)
      {
        if (atpoe.arcPos == i && atpoe.beginPoint)
          needContinue = false;

        continue;
      }
      // если следующая за текущей - начальная
      if (atpoe.beginPoint)
      {
        // то текущая полидуга неактуальна
        polyarcIsBorder = false;
      }
      break;
    }

    // у неактуальной дуги сбрасываем флаг актуальности
    if (!polyarcIsBorder) 
      polyarcsActualities[i] = false;
  }
}
//==============================================================================
bool ArcManager::isPolygonCW(const std::vector<Point2D>& polygon) const noexcept
{
  if (polygon.empty())
    return false;

  double S = 0;

  for (uinttype i = 0; i < polygon.size() - 1; i++)
  {
    const Point2D & pt1 = polygon[i];
    const Point2D & pt2 = polygon[i + 1];
    S += (pt1.getX() * pt2.getY() - pt2.getX() * pt1.getY());
  }

  const Point2D & pt1 = polygon.back();
  const Point2D & pt2 = polygon[0];
  S += (pt1.getX() * pt2.getY() - pt2.getX() * pt1.getY());

  return (S < 0);
}
//==============================================================================
void ArcManager::clearArcs() noexcept
{
  mPolyarcs.clear();
  mArcIdCounter = 1U;
}
//==============================================================================
} //! ReachableAreaCalculator
