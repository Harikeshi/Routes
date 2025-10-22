#include "PathFinder/PathCorrection.hpp"
//! TriangularGeometry
#include <TriangularGeometry/TriangularException.hpp>
//==============================================================================
namespace TriangularGeometry
{
//==============================================================================
  PathCorrector::PathCorrector(
    const std::deque<std::shared_ptr<Top>>& path,
    const std::shared_ptr<Triangle>& startTriangle)
    : mPath(path)
    , mStartTriangle(startTriangle)
    , mCurTriangle(startTriangle)
  { }
//==============================================================================
  std::deque<std::shared_ptr<Top>> PathCorrector::correctSectionLengths(
    const double minLength)
  {
    // Обновим положение текущей грани
    mCurTriangle = mStartTriangle;

    // Если участок маршрута всего один, то это особый случай 
    // (ну или Python что-то сделал не так)
    if (mPath.size() == 2)
    {
      //! Если отрезок не проходит ограничение по длине или 
      //! если ситуация с пересечением отрезком хотя бы одного препятствия - 
      //! то маршрут исправить нельзя 
      if (   (!isCorrectLength(mPath.at(0), mPath.at(1), minLength))
          || (!moveCurTriangle(1)))
        mPath.clear();
      return mPath;
    }

    try
    {
      // Сначала пытаемся исправить все участки, вершины которых расположены на
      // препятствиях по одну сторону от маршрута. Поскольку анализируем
      // только вершины на препятствиях, первый и последний участки пропускаем.
      for (size_t i = 1; i < mPath.size() - 2; ++i)
      {
        // Если в результате упрощения предыдущего участка вышли за границы
        // триангуляции - корректный маршрут не найден
        if (!mCurTriangle)
        {
          mPath.clear();
          return mPath;
        }

        // Вершины текущего участка
        std::shared_ptr<Top> firstPoint(mPath.at(i));
        std::shared_ptr<Top> secondPoint(mPath.at(i + 1));

        // Участок корректен - пропускаем его
        if (isCorrectLength(firstPoint, secondPoint, minLength))
        {
          // Переместим значение текущей грани к следующей вершине (первой
          // на проверяемом участке, поскольку вторая ещё может измениться в
          // результате следующих исправлений)
          if (!moveCurTriangle(i))
          {
            mPath.clear();
            return mPath;
          }

          continue;
        }

        // Участок некорректнен, а его вершины лежат по одну сторону
        // от препятствия
        if ((firstPoint->edge) && (getRestrictionPosition(i + 1, false)
                                == getRestrictionPosition(i, true)))
        {
          // Проверим, есть ли ещё вершины по ту же сторону от препятствия,
          // которые нужно исправлять. Найдём их количество.
          size_t pointsCount(findNumberOfDeletedPoints(i, minLength));

          // Исправляем некорректные участки
          size_t newPointsNumber(simplifyPathForOneSidePoints(i, pointsCount,
                                                              minLength));

          // Если добавлена всего одна вершина - текущий участок нужно
          // проверить ещё раз
          if (newPointsNumber == 1)
          {
            --i;
            continue;
          }
        }

        // Если добавилось две вершины, переместим значение текущей грани к
        // первой из них
        if (!moveCurTriangle(i))
        {
          mPath.clear();
          return mPath;
        }
      }

      // проверим последние вершины
      if (!moveCurTriangle(mPath.size() - 2)
        || !moveCurTriangle(mPath.size() - 1))
      {
        mPath.clear();
        return mPath;
      }

      // Обновим положение текущей грани
      mCurTriangle = mStartTriangle;

      // Если остались некорректные участки (здесь проверяется первый участок
      // и некорректные участки, вершины которых не расположены на одном
      // препятствии), просто продлим их
      for (size_t i = 0; i < mPath.size() - 2; ++i)
      {
        std::shared_ptr<Top> firstPoint(mPath.at(i));
        std::shared_ptr<Top> secondPoint(mPath.at(i + 1));

        // Участок некорректен - исправляем
        if (!isCorrectLength(firstPoint, secondPoint, minLength))
        {
          // Немного сместим вершину от препятствия, чтобы продлённый
          // участок его гарантированно не задел
          std::shared_ptr<Top> movedPoint;

          try
          {
            movedPoint = movePoint(i + 1);
          }
          catch (const TriangularFailure& exception)
          {
            // Если не смогли найти фиксированное ребро для вершины, значит,
            // она уже была отодвинута в результате предыдущих исправлений
            if (exception.getCode() == NoFixedEdge)
              movedPoint = mPath.at(i + 1);
            else
              throw;
          }

          Geometry::BorderedLine curPathPart(*firstPoint, *movedPoint);
          Point2D newPoint(curPathPart.findPoint(*firstPoint, minLength));
          mPath[i + 1] = std::make_shared<Top>(newPoint.getX(),
                                               newPoint.getY());
        }

        // Проверяем корректность участка (до последней его вершины, она больше
        // меняться не будет)
        if (!moveCurTriangle(i + 1))
        {
          mPath.clear();
          return mPath;
        }
      }

      // Проверяем последний участок: если он некорректен, исправить не можем
      if (!isCorrectLength(mPath.at(mPath.size() - 2),
                           mPath.at(mPath.size() - 1), minLength)
        || (!moveCurTriangle(mPath.size() - 1)))
      {
        mPath.clear();
      }
    }
    catch (...)
    {
      mPath.clear();
    }

    return mPath;
  }
//==============================================================================
  bool PathCorrector::isCorrectLength(
    const std::shared_ptr<Top>& firstPoint,
    const std::shared_ptr<Top>& secondPoint,
    const double minLength)
  {
    return ((minLength - Geometry::distance(*firstPoint, *secondPoint))
                                                           <= ACCURACY_DOUBLE);
  }
//==============================================================================
  Geometry::PlanePosition PathCorrector::getRestrictionPosition(
    const size_t index,
    bool useNextPathPart/* = true*/)
  {
    std::shared_ptr<Top> top = mPath.at(index);
    Geometry::BorderedLine curPathPart = useNextPathPart
      ? Geometry::BorderedLine(*top, *mPath.at(index + 1))
      : Geometry::BorderedLine(*mPath.at(index - 1), *top);
    
    // Находим одно фиксированное ребро
    std::shared_ptr<Edge> firstFixedEdge(top->getFixedEdge(Geometry::Left));
    if (!firstFixedEdge)
      throw TriangularFailure(TriangularEnumFailure::NoFixedEdge);

    // Берём его вершину, отличающуюся от входной
    std::shared_ptr<Top> otherTopInFirstEdge
      = firstFixedEdge->getOtherTop(top);

    // Определяем положение этой вершины относительно заданного участка
    // маршрута
    Geometry::PlanePosition restrictionPosition(
      curPathPart.position(*otherTopInFirstEdge));

    // Если получилось определить положение препятствия, возвращаем его
    if ((restrictionPosition == Geometry::Left)
      || (restrictionPosition == Geometry::Right))
      return restrictionPosition;

    // Если ребро совпало с маршрутом, будем определять по второму ребру

    // Находим второе фиксированное ребро
    std::shared_ptr<Edge> secondFixedEdge(top->getFixedEdge(Geometry::Right));
    if (!secondFixedEdge)
      throw TriangularFailure(TriangularEnumFailure::NoFixedEdge);

    // Берём его вершину, отличающуюся от входной
    std::shared_ptr<Top> otherTopInSecondEdge
      = secondFixedEdge->getOtherTop(top);

    // Определяем положение этой вершины относительно заданного участка
    // маршрута
    restrictionPosition = curPathPart.position(*otherTopInSecondEdge);

    if ((restrictionPosition == Geometry::Left)
      || (restrictionPosition == Geometry::Right))
      return restrictionPosition;

    // Если и второе ребро совпало по направлению с маршрутом, можем определить
    // положение препятствия по расположению третьей вершины грани, которой
    // принадлежит фиксированное ребро
    std::shared_ptr<Triangle> triangle(firstFixedEdge->left);
    if (!triangle)
      firstFixedEdge->right;

    std::shared_ptr<Top> topInsideTriangulation(
      triangle->topOppositeEdge(firstFixedEdge));

    Geometry::PlanePosition areaPosition(
      curPathPart.position(*topInsideTriangulation));

    // Если снова не получилось, что-то сломалось. Такого быть не должно.
    if ((areaPosition != Geometry::Left) && (areaPosition != Geometry::Right))
      throw TriangularFailure(TriangularEnumFailure::IncorrectDirection);

    // Препятсвие расположено по другую сторону от вершины триангуляции
    if (areaPosition == Geometry::Left)
      return Geometry::Right;

    return Geometry::Left;
  }
//==============================================================================
  size_t PathCorrector::findNumberOfDeletedPoints(const size_t firstIndex,
                                                  const double minLength)
  {
    // Количество упрощаемых точек
    size_t pointsCount(2);

    // Считаем количество вершин, которые будем обрабатывать вместе

    // Вершины текущего проверяемого участка маршрута
    std::shared_ptr<Top> firstCurPoint(mPath.at(firstIndex + pointsCount - 1));
    std::shared_ptr<Top> secondCurPoint(mPath.at(firstIndex + pointsCount));

    Geometry::PlanePosition firstRestrictionPosition(
      getRestrictionPosition(firstIndex, true));

    // Добавляем вершины к исправляемым, пока они не удовлетворяют ограничению
    // по минимальной длине участка
    while (((firstIndex + pointsCount) < mPath.size() - 1)
      && !isCorrectLength(firstCurPoint, secondCurPoint, minLength))
    {
      // Если на текущем участке препятствие оказалось с другой стороны
      // от маршрута, чем было раньше, этот участок будет упрощаться отдельно,
      // как и все следующие
      Geometry::PlanePosition curRestrictionPosition(
        getRestrictionPosition(firstIndex + pointsCount));

      if (firstRestrictionPosition != curRestrictionPosition)
        break;

      ++pointsCount;

      // Берём следующий участок для проверки
      firstCurPoint = mPath.at(firstIndex + pointsCount - 1);
      secondCurPoint = mPath.at(firstIndex + pointsCount);
    }

    return pointsCount;
  }
//==============================================================================
  std::shared_ptr<Top> PathCorrector::movePoint(
    const size_t index,
    const double distance/* = ACCURACY_DOUBLE*/)
  {
    std::shared_ptr<Top> top(mPath.at(index));

    std::shared_ptr<Edge> leftFixedEdge(top->getFixedEdge(Geometry::Left));
    std::shared_ptr<Edge> rightFixedEdge(top->getFixedEdge(Geometry::Right));
    if (!leftFixedEdge || !rightFixedEdge)
      throw TriangularFailure(TriangularEnumFailure::NoFixedEdge);

    // Вершины фиксированных рёбер, не совпадающие с текущей вершиной маршрута
    std::shared_ptr<Top> secondLeftTop = leftFixedEdge->getOtherTop(top);
    std::shared_ptr<Top> secondRightTop = rightFixedEdge->getOtherTop(top);

    // Формируем линии фиксированных рёбер так, чтобы их направление было
    // от текущей вершины
    Geometry::BorderedLine leftLine(*top, *secondLeftTop);
    Geometry::BorderedLine rightLine(*top, *secondRightTop);

    // Угол между отрезками со стороны области триангуляции
    Radian angle(leftLine.getAngleTo(rightLine).normalizeWithShift());

    // Линия смещения вершины
    Geometry::Line movedLine(*top,
                             (rightLine.getSlope() + angle / 2.).normalize());

    // Смещённая вершина
    Point2D movedPoint(movedLine.findPoint(*top, distance, true));

    return std::make_shared<Top>(movedPoint.getX(), movedPoint.getY());
  }
//==============================================================================
  size_t PathCorrector::simplifyPathForOneSidePoints(
    const size_t firstIndex,
    const size_t pointsCount,
    const double minLength)
  {
    // Первый участок (линия которого останется неизменна)
    Geometry::BorderedLine firstLine(*mPath.at(firstIndex - 1), 
                                     *movePoint(firstIndex));
    // Последний участок (линия которого останется неизменна)
    Geometry::BorderedLine lastLine(*movePoint(firstIndex + pointsCount - 1),
                                    *mPath.at(firstIndex + pointsCount));

    // Точка пересечения линий
    Point2D crossPoint(firstLine.crossPoint(lastLine));

    // Если упрощаемых вершин всего две, заменяем их на вершину пересечения
    if (pointsCount == 2)
    {
      mPath.erase(mPath.begin() + firstIndex, mPath.begin() + firstIndex + 2);
      mPath.insert(mPath.begin() + firstIndex,
                  std::make_shared<Top>(crossPoint.getX(), crossPoint.getY()));
      return 1;
    }

    // Положение последнего участка относительно первого
    Geometry::PlanePosition positionfromFirstToLast(
      firstLine.position(*mPath.at(firstIndex + pointsCount - 1)));

    // Угол в точке пересечения первого и последнего участков
    Radian crossPointAngle(Geometry::findAngle(
      *mPath.at(firstIndex), crossPoint,
      *mPath.at(firstIndex + pointsCount - 1),
      (positionfromFirstToLast == Geometry::Left)));

    // Угол поворота до линии нового участка маршрута
    Radian angleToNewLine((M_PI - crossPointAngle.get()) / 2.);
    // Угол наклона первого участка
    Radian slopeOfFirstLine(firstLine.getSlope());

    // Угол наклона нового участка маршрута
    Radian slopeOfNewLine = (positionfromFirstToLast == Geometry::Right)
                          ? (slopeOfFirstLine - angleToNewLine)
                          : (slopeOfFirstLine + angleToNewLine);
    slopeOfNewLine.normalize();

    // Линия, параллельная новому участку маршрута, проходящая через точку
    // пересечения первого и последнего участков
    Geometry::Line lineParallelToNew(crossPoint, slopeOfNewLine);

    // Найдём среди расчищаемых точек маршрута ближайшую к заданной линии
    double minDistanceToLine(std::numeric_limits<double>::max());
    size_t nearestPointIndex;
    for (size_t i = firstIndex; i < firstIndex + pointsCount; ++i)
    {
      double curDistance(lineParallelToNew.distanceFromPoint(*mPath.at(i)));
      if (curDistance < minDistanceToLine)
      {
        minDistanceToLine = curDistance;
        nearestPointIndex = i;
      }
    }

    // Через эту точку (немного смещённую) и будет проходить новый участок
    std::shared_ptr<Top> movedPoint(movePoint(nearestPointIndex));

    Geometry::Line newLine(*movedPoint, slopeOfNewLine);

    // Точки пересечения этой линии с первым и последним участками и будут
    // новыми точками маршрута
    Point2D firstNewPoint(newLine.crossPoint(firstLine));
    Point2D secondNewPoint(newLine.crossPoint(lastLine));

    // Удаляем исправляемые точки
    mPath.erase(mPath.begin() + firstIndex,
                mPath.begin() + firstIndex + pointsCount);

    // Если расстояние между двумя новыми точками удовлетворяет ограничению,
    // добавляем их
    std::shared_ptr<Top> firstNewTop
      = std::make_shared<Top>(firstNewPoint.getX(),
                              firstNewPoint.getY());
    std::shared_ptr<Top> secondNewTop
      = std::make_shared<Top>(secondNewPoint.getX(),
                              secondNewPoint.getY());

    if (isCorrectLength(firstNewTop, secondNewTop, minLength))
    {
      mPath.insert(mPath.begin() + firstIndex, firstNewTop);
      mPath.insert(mPath.begin() + firstIndex + 1, secondNewTop);

      return 2;
    }

    // Иначе добавляем только одну точку -  точку пересечения первого и
    // последнего участков
    mPath.insert(mPath.begin() + firstIndex,
                 std::make_shared<Top>(crossPoint.getX(), crossPoint.getY()));

    return 1;
  }
//==============================================================================
  bool PathCorrector::moveThroughEdge(
    const std::shared_ptr<Edge>& pathEdge,
    std::shared_ptr<Edge>& lastIntersectedEdge)
  {
    std::shared_ptr<Edge> intersectedEdge;

    // Функция поиска пересечения ребра маршрута с заданным ребром (попадание
    // начала участка маршрута на ребро также считается пересечением)
    auto hasIntersection
      = [pathEdge, lastIntersectedEdge]
        (const std::shared_ptr<Edge>& edge)
        {
          return ((pathEdge->isIntersectEdge(edge)
              && (!lastIntersectedEdge || !lastIntersectedEdge->isSame(edge)))
              || (edge->atEdge(pathEdge->getI())));
        };

    // Найдём ребро грани, пересекающее текущий участок
    if (hasIntersection(mCurTriangle->p))
      intersectedEdge = mCurTriangle->p;
    else if (hasIntersection(mCurTriangle->q))
      intersectedEdge = mCurTriangle->q;
    else if (hasIntersection(mCurTriangle->r))
      intersectedEdge = mCurTriangle->r;

    // Если нашли такое ребро, проходим через него
    if (intersectedEdge)
    {
      if (mCurTriangle == intersectedEdge->left)
        mCurTriangle = intersectedEdge->right;
      else
        mCurTriangle = intersectedEdge->left;

      lastIntersectedEdge = intersectedEdge;

      return true;
    }

    return false;
  }
//==============================================================================
  std::shared_ptr<Top> PathCorrector::getIntersectedTop(
    const std::shared_ptr<Edge>& pathEdge)
  {
    std::shared_ptr<Top> i, j, k;
    mCurTriangle->allTops(i, j, k);

    if (pathEdge->atBorderLine(*i))
      return i;
    if (pathEdge->atBorderLine(*j))
      return j;
    if (pathEdge->atBorderLine(*k))
      return k;

    return nullptr;
  }
//==============================================================================
  bool PathCorrector::moveThroughEdgeAfterTopForOneSide(
    const std::shared_ptr<Edge>& pathEdge,
    const std::shared_ptr<Top>& intersectedTop,
    const std::shared_ptr<Top>& prevTop,
    std::shared_ptr<Edge>& lastEdge,
    bool isLeftSide)
  {
    Geometry::PlanePosition position = isLeftSide ? Geometry::Left
                                                  : Geometry::Right;

    // Возьмём произвольное ребро вершины, через которую проходит маршрут
    std::shared_ptr<Edge> startEdge(intersectedTop->edge);

    std::shared_ptr<Edge> curEdge(startEdge);
    std::shared_ptr<Edge> nextEdge(nullptr);
    std::shared_ptr<Edge> intersectedEdge(nullptr);
    std::shared_ptr<Triangle> nextTriangle(nullptr);

    Edge::scanBunch(curEdge, intersectedTop, position,
                    nextTriangle, nextEdge);

    while (nextTriangle && (startEdge != nextEdge))
    {
      // Если последняя вершина участка оказалась внутри грани, нашли
      // последнюю грань участка
      if (nextTriangle->isTopInTriangle(pathEdge->getJ()))
      {
        mCurTriangle = nextTriangle;
        return true;
      }

      // Найдём ребро грани, пересекающее текущий участок
      if (pathEdge->isIntersectEdge(nextTriangle->p))
        intersectedEdge = nextTriangle->p;
      else if (pathEdge->isIntersectEdge(nextTriangle->q))
        intersectedEdge = nextTriangle->q;
      else if (pathEdge->isIntersectEdge(nextTriangle->r))
        intersectedEdge = nextTriangle->r;

      // Если нашли, обновляем грань
      if (intersectedEdge)
      {
        if (intersectedEdge->left->hasTopTriangle(intersectedTop))
          mCurTriangle = intersectedEdge->right;
        else
          mCurTriangle = intersectedEdge->left;

        lastEdge = intersectedEdge;

        return true;
      }

      // Если не нашли, возможно, идём вдоль ребра - тогда ищем следующую
      // пересечённую вершину
      std::shared_ptr<Edge> oppositeEdge(
        nextTriangle->edgeOppositeTop(intersectedTop));

      if (pathEdge->atBorderLine(*oppositeEdge->getI())
        && (!prevTop || !prevTop->isSame(oppositeEdge->getI())))
      {
        mCurTriangle = nextTriangle;

        if (moveThroughEdgeAfterTop(pathEdge, oppositeEdge->getI(),
                                    intersectedTop, lastEdge))
          return true;
      }
      else if (pathEdge->atBorderLine(*oppositeEdge->getJ())
        && (!prevTop || !prevTop->isSame(oppositeEdge->getJ())))
      {
        mCurTriangle = nextTriangle;

        if (moveThroughEdgeAfterTop(pathEdge, oppositeEdge->getJ(),
                                    intersectedTop, lastEdge))
          return true;
      }

      // Не нашли пересечений, смещаемся к следующей грани
      curEdge = nextEdge;

      Edge::scanBunch(curEdge, intersectedTop, position,
                      nextTriangle, nextEdge);
    }

    return false;
  }
//==============================================================================
  bool PathCorrector::moveThroughEdgeAfterTop(
    const std::shared_ptr<Edge>& pathEdge,
    const std::shared_ptr<Top>& intersectedTop,
    const std::shared_ptr<Top>& prevTop,
    std::shared_ptr<Edge>& lastEdge)
  {
    // Если у вершины нет ребра, не сможем найти нужное ребро
    if (!intersectedTop->edge)
      return false;

    // Сначала ищем ребро слева от ребра вершины
    if (moveThroughEdgeAfterTopForOneSide(pathEdge, intersectedTop, prevTop,
                                          lastEdge, true))
      return true;

    // Если не нашли - ищем справа
    if (moveThroughEdgeAfterTopForOneSide(pathEdge, intersectedTop, prevTop,
                                          lastEdge, false))
      return true;

    return false;
  }
//==============================================================================
  bool PathCorrector::moveCurTriangle(const size_t index)
  {
    std::shared_ptr<Top> curTop(mPath.at(index));

    // Текущий участок маршрута
    std::shared_ptr<Edge> pathEdge
      = std::make_shared<Edge>(mPath.at(index - 1), curTop);

    // Последнее ребро триангуляции, через которое прошли (необходимо,
    // чтобы следовать в нужном направлении)
    std::shared_ptr<Edge> intersectedEdge(nullptr);

    // Продолжаем смещать грань, пока не найдём ту, которая содержит
    // заданную вершину
    while (mCurTriangle && !mCurTriangle->isTopInTriangle(curTop))
    {
      if (moveThroughEdge(pathEdge, intersectedEdge))
        continue;

      // Если не нашли ребро, значит, пересекли вершину
      std::shared_ptr<Top> intersectedTop(getIntersectedTop(pathEdge));

      // Не нашли вершину - что-то пошло не так, не можем найти следующую грань
      if (!intersectedTop)
        return false;

      if (!moveThroughEdgeAfterTop(pathEdge, intersectedTop,
                                   nullptr, intersectedEdge))
        return false;
    }

    if (!mCurTriangle)
      return false;

    return true;
  }
//==============================================================================
} // !TriangularGeometry
//==============================================================================
