#include <Geometry/ComplicatedOperations/CakeProblem.hpp>

#include <Geometry/Objects/BorderedLine.hpp>
#include <Geometry/Objects/Circle.hpp>
#include <Geometry/Objects/SectorsUnity.hpp>
#include <Geometry/Operations/PolygonToCircles.hpp>
#include <Geometry/GeometryException.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Difference.hpp>
#include <AbstractOperations/Algorithms/Intersection.hpp>
#include <AbstractOperations/Algorithms/Union.hpp>
#include <AbstractOperations/Algorithms/Correct.hpp>
#include <AbstractOperations/Algorithms/Within.hpp>
#include <AbstractOperations/DeleteExcessVertices.hpp>
//==============================================================================
namespace Geometry
{
using namespace AbstractOperations;
//==============================================================================
  Polygon2D getPolyCircle(
    const Point2D& center,
    const double radius)
  {
    if (radius < -ACCURACY_DOUBLE)
      throw GeometryFailure(NegativeRadius);

    // Результирующая окружность
    Polygon2D circle;
    
    // Если радиус нулевой, возвращаем точку центра
    if (BasicMath::isNull(radius))
    {
      circle.outer().emplace_back(center);
      return circle;
    }

    // Задаём окружность
    Geometry::Circle circleArea(center, radius);

    // Преобразовываем её в полигон
    circle = Geometry::toPolygon(circleArea);

    std::string message;
    if ( ! correct(circle, message) )
      throw GeometryFailure(OperationNotPerformedCorrect,
        message, "getPolyCircle:");

    return circle;
  }
//==============================================================================
  void findPolygonLeftAndRightPoints(
    const Polygon2D& polygon,
    const Point2D& center,
    std::vector<size_t>& iLeftPoints, 
    std::vector<size_t>& iRightPoints
    )
  {
    if (polygon.outer().size() < 1)
      throw GeometryFailure(EmptyPolygon);

    // Очистим векторы на случай, если они пришли не пустые
    iLeftPoints.clear();
    iRightPoints.clear();

    // Проходим по всем точкам полигона
    for (std::size_t i = 0; i < polygon.outer().size() - 1; ++i)
    {
      Geometry::BorderedLine lineToCurPoint(center, polygon.outer()[i]);

      int prevInd = (i - 1 + (polygon.outer().size() - 1))
        % (polygon.outer().size() - 1);
      int nextInd = (i + 1) % (polygon.outer().size() - 1);

      if (lineToCurPoint.position(polygon.outer()[prevInd]) == Geometry::Between
        || lineToCurPoint.position(polygon.outer()[prevInd]) == Geometry::Beyond)
      {
        // Если предыдущая или следующая точка лежит на одной 
        // прямой с текущей, определяем, правая точка или левая, 
        // по второй.
        if (lineToCurPoint.position(
          polygon.outer()[nextInd])
          == Geometry::Right)
        {
          iLeftPoints.emplace_back(i);
        }
        else
        {
          iRightPoints.emplace_back(i);
        }
      }
      else if (lineToCurPoint.position(polygon.outer()[nextInd]) == Geometry::Between
        || lineToCurPoint.position(polygon.outer()[nextInd]) == Geometry::Beyond)
      {
        // Если предыдущая или следующая точка лежит на одной 
        // прямой с текущей, определяем, правая точка или левая, 
        // по второй.
        if (lineToCurPoint.position(
          polygon.outer()[prevInd]) == Geometry::Right)
        {
          iLeftPoints.emplace_back(i);
        }
        else
        {
          iRightPoints.emplace_back(i);
        }
      }
      else if (lineToCurPoint.position(polygon.outer()[prevInd]) ==
        lineToCurPoint.position(polygon.outer()[nextInd]))
      {
        // Если предыдущая и следующая точки оказались с одной стороны
        // от текущей, то текущая или крайняя левая, или крайняя
        // правая. Если предыдущая и следующая точки оказались справа
        // от текущей, точка крайняя левая, иначе правая.
        if (lineToCurPoint.position(polygon.outer()[nextInd])
          == Geometry::Right)
        {
          iLeftPoints.emplace_back(i);
        }
        else if (lineToCurPoint.position(polygon.outer()[nextInd])
          == Geometry::Left)
        {
          iRightPoints.emplace_back(i);
        }
      }
    }
  }
//==============================================================================
  size_t findPolygonBorderedPoint(
    const Polygon2D& polygon,
    const Point2D& center,
    const std::vector<size_t>& iBorderedPoints,
    bool isRight
    )
  {
    if (polygon.outer().size() < 1)
      throw GeometryFailure(EmptyPolygon);

    if (iBorderedPoints.size() < 1)
      throw GeometryFailure(NoBorderedPointsInPolygon);

    if (iBorderedPoints.size() == 1)
      return iBorderedPoints[0];

    int iBordered = iBorderedPoints[0];

    Geometry::PlanePosition position 
      = isRight ? Geometry::Right : Geometry::Left;

    for (std::size_t i = 1; i < iBorderedPoints.size(); ++i)
    {
      bool correctPoint(true);

      if (iBorderedPoints[i] < 0 || iBorderedPoints[i] > polygon.outer().size())
        throw GeometryFailure(PolygonIndexIsOutOfRange);

      // Если угол между двумя точками больше 90, правая точка может
      // искаться некорректно, выбираем точку, которая не 
      // пересекается с рёбрами полигона
      double angleBetweenPoints = Geometry::findAngle(
        polygon.outer()[iBordered], center, 
        polygon.outer()[iBorderedPoints[i]]).get();

      if ((angleBetweenPoints > M_PI / 2)
        && (angleBetweenPoints < 3 * M_PI / 2))
      {
        // Ищем пересечение отрезков с рёбрами
        Geometry::BorderedLine lineToBorderedPoint(center,
          polygon.outer()[iBorderedPoints[i]]);
        for (size_t j = 0; j < polygon.outer().size() - 1; ++j)
        {
          correctPoint = !lineToBorderedPoint.isIntersectWithoutBorder(
            Geometry::BorderedLine(polygon.outer()[j], polygon.outer()[j + 1]));

          if (!correctPoint)
            break;
        }
      }

      Geometry::BorderedLine lineToCurBorderedPoint(
        center, polygon.outer()[iBordered]);

      if (lineToCurBorderedPoint.position(
        polygon.outer()[iBorderedPoints[i]]) == position  && correctPoint)
        iBordered = iBorderedPoints[i];

      lineToCurBorderedPoint = Geometry::BorderedLine(
        center, polygon.outer()[iBordered]);

      // Если точки на одной линии, берём ближнюю к центру
      if (lineToCurBorderedPoint.position(polygon.outer()[iBorderedPoints[i]])
        == Geometry::Between
        || lineToCurBorderedPoint.position(polygon.outer()[iBorderedPoints[i]])
        == Geometry::Beyond)
      {
        if (Geometry::distance(center, polygon.outer()[iBorderedPoints[i]])
          < Geometry::distance(center, polygon.outer()[iBordered]))
          iBordered = iBorderedPoints[i];
      }
    }

    return iBordered;
  }
//==============================================================================
  void getSectorPolygon(
    const Polygon2D& polygon,
    const Point2D& center,
    const double radius,
    size_t iLeft,
    size_t iRight,
    SectorPoly& sector
    )
  {
    if (polygon.outer().size() < 1)
      throw GeometryFailure(EmptyPolygon);

    if (radius < -ACCURACY_DOUBLE)
      throw GeometryFailure(NegativeRadius);

    sector.poly.clear();

    double triangleSide = radius
      / sqrt((1. + Geometry::cosAngle(
        sector.leftPoint, center, sector.rightPoint)) / 2.);

    // Если сектор больше прямого угла, откладываем на расстояние
    // радиуса
    if ((Geometry::findAngle(
      sector.rightPoint, center, sector.leftPoint)).get()
      > (M_PI / 2 + ACCURACY_DOUBLE))
    {
      triangleSide = radius;
    }
      
    sector.externalLeftPoint = Geometry::findPointAtLine(
      sector.leftPoint, center, triangleSide, true);
    sector.poly.outer().emplace_back(sector.externalLeftPoint);

    // Идём в сторону меньшего угла, чтобы не менять 
    // внутреннюю часть полигона
    if (Geometry::cosAngle(center, polygon.outer()[iLeft],
      polygon.outer()[(iLeft + 1) % (polygon.outer().size() - 1)]) <
      Geometry::cosAngle(center, polygon.outer()[iLeft],
        polygon.outer()[(iLeft - 1 + polygon.outer().size() - 1)
        % (polygon.outer().size() - 1)]))
    {
      int i(iLeft + 1);

      do
      {
        --i;
        sector.poly.outer().emplace_back(polygon.outer()[i]);

        if ((i == 0) && (i != iRight))
          i = polygon.outer().size() - 1;

      } while (i != iRight);
    }
    else
    {
      int i(iLeft - 1);

      do
      {
        ++i;
        sector.poly.outer().emplace_back(polygon.outer()[i]);
        if ((i == polygon.outer().size() - 1) && (i != iRight))
          i = 0;

      } while (i != iRight);
    }

    sector.externalRightPoint = Geometry::findPointAtLine(
      sector.rightPoint, center, triangleSide, true);
    sector.poly.outer().emplace_back(sector.externalRightPoint);

    // Если угол между левой и правой границами сектора больше 90
    // градусов, но меньше или равен 270 - строим по трём касательным
    if (((Geometry::findAngle(
      sector.rightPoint, center, sector.leftPoint)).get()
      > (M_PI / 2 + ACCURACY_DOUBLE))
      && ((Geometry::findAngle(
        sector.rightPoint, center, sector.leftPoint)).get()
      <= (3 * M_PI / 2 + ACCURACY_DOUBLE)))
    {
      // Угол, обратный углу сектора
      double alpha = Geometry::findAngle(
        sector.leftPoint, center, sector.rightPoint).get();

      double lastSide = radius * (1 + cos(alpha / 2)) / sin(alpha / 2);

      Geometry::BorderedLine lineToLastPoint(center,
        sector.poly.outer()[sector.poly.outer().size() - 1]);
      Geometry::BorderedLine lineToFirstPoint(center,
        sector.poly.outer()[0]);

      sector.poly.outer().emplace_back(
        lineToLastPoint.findPointAtPerpendLine(
        sector.poly.outer()[sector.poly.outer().size() - 1],
        lastSide, true));

      sector.poly.outer().emplace_back(
        lineToFirstPoint.findPointAtPerpendLine(
        sector.poly.outer()[0], lastSide, false));
    }

    // Если угол больше 270 градусов, строим по пяти касательным
    if ((Geometry::findAngle(
      sector.rightPoint, center, sector.leftPoint)).get()
      > (3 * M_PI / 2 + ACCURACY_DOUBLE))
    {
      // отрезок вдоль правой границы сектора
      Geometry::BorderedLine lineNearRight(center,
        sector.poly.outer()[sector.poly.outer().size() - 1]);
      // отрезок вдоль левой границы сектора
      Geometry::BorderedLine lineNearLeft(center,
        sector.poly.outer()[0]);

      sector.poly.outer().emplace_back(
        lineNearRight.findPointAtPerpendLine(
        sector.poly.outer()[sector.poly.outer().size() - 1],
        radius, true));

      // запоминаем последнюю точку, 
      // чтобы вычислить от неё предпоследнюю
      Point2D lastPoint = lineNearLeft.findPointAtPerpendLine(
        sector.poly.outer()[0], radius, false);

      // угол сектора
      double sectorAngle = Geometry::findAngle(
        sector.rightPoint, center, sector.leftPoint).get();

      double lastSide = radius * (1 + tan((sectorAngle - M_PI) / 4));

      // отрезок, перпендикулярный правой границе сектора
      Geometry::BorderedLine lineNearRight2(
        sector.poly.outer()[sector.poly.outer().size() - 2],
        sector.poly.outer()[sector.poly.outer().size() - 1]);
      // отрезок, перпендикулярный левой границе сектора
      Geometry::BorderedLine lineNearLeft2(sector.poly.outer()[0], lastPoint);

      sector.poly.outer().emplace_back(
        lineNearRight2.findPointAtPerpendLine(
        sector.poly.outer()[sector.poly.outer().size() - 1], lastSide, true));

      sector.poly.outer().emplace_back(
        lineNearLeft2.findPointAtPerpendLine(lastPoint, lastSide, false));

      sector.poly.outer().emplace_back(lastPoint);
    }

    try
    {
      std::string message;
      if ( ! correct(sector.poly, message) )
        throw GeometryFailure(OperationNotPerformedCorrect,
          message, "getSectorPolygon:");
    }
    catch (...)
    {
      ExceptionTools::FailureHandler<GeometryFailure>("getSectorPolygon");
    }
  }
//==============================================================================
  Radian findExpansionAngle(
    const Point2D& center,
    const Point2D& polyPoint,
    const Point2D& curExpansionPoint,
    double widthExpansion,
    bool isClockwise)
  {
    // Формируем окружность возле точки полигона, от которой
    // ищем новое расширение
    Geometry::Circle circleNearNewPoint(polyPoint, widthExpansion);

    // точка нового расширения
    Point2D newExpansionPoint 
      = circleNearNewPoint.tangentPoint(center, !isClockwise);

    Radian angle = Geometry::findAngle(
      curExpansionPoint, center, newExpansionPoint, isClockwise);

    // Приводим угол к диапазону (-pi, pi]
    if (angle <= M_PI)
      return angle;

    return angle - Radian(2 * M_PI);
  }
//==============================================================================
  void expandOneSectorsBorder(
    SectorPoly& sector, const Point2D& center, double radius, double widthExpansion,
    const MultiPolygon2D& problemZone, const Point2D& pointNearBorder,
    bool isRightExpansion)
  {
    if (sector.poly.outer().size() < 1)
      throw GeometryFailure(EmptySectorPolygon);

    if (radius < -ACCURACY_DOUBLE)
      throw GeometryFailure(NegativeRadius);

    if (widthExpansion <= ACCURACY_DOUBLE)
      throw GeometryFailure(NegativeOrZeroWidthExpansion);

    Polygon2D additionalSector;

    // Если есть пересечение с текущим расширением, нужно найти 
    // новое
    if (!problemZone.empty())
    {
      // Найдём точку пересечения, от которой придётся отодвигать 
      // на наибольший угол от текущего маршрута
      Point2D nearPoint = problemZone[0].outer()[0];

      for (const auto& polygon : problemZone)
      {
        for (const auto& point : polygon.outer())
        {
          if (findExpansionAngle(
            center, point, pointNearBorder, widthExpansion, isRightExpansion)
            > findExpansionAngle(
            center, nearPoint, pointNearBorder, widthExpansion, isRightExpansion))
          {
            nearPoint = point;
          }
        }
      }

      // Найдём точку на расстоянии ширины трубки от новой точки
      Geometry::Circle circleFromNearPoint(nearPoint, widthExpansion);

      Point2D pointNewBorder
        = circleFromNearPoint.tangentPoint(center, !isRightExpansion);

      // Формируем полигон расширения сектора
      // Если угол расширения не больше угла исходного сектора,
      // выбираем для точек расширения такое расстояние от центра,
      // чтобы точка на общей линии с сектором совпала с 
      // соответствующей точкой сектора. Это нужно для того,
      // чтобы было меньше шансов столкнуться с проблемами при 
      // объединении полигонов сектора и расширения
      double triangleSide = radius
        / sqrt((1. + Geometry::cosAngle(
          sector.leftPoint, center, sector.rightPoint)) / 2.);

      if ((Geometry::findAngle(
        sector.rightPoint, center, sector.leftPoint)).get()
        > (M_PI / 2 + ACCURACY_DOUBLE))
      {
        triangleSide = radius;
      }
      else if ((radius / sqrt((1. + Geometry::cosAngle(
        nearPoint, center, pointNewBorder)) / 2.)) > triangleSide)
      {
        if (BasicMath::isNull(sqrt((1. + Geometry::cosAngle(
          nearPoint, center, pointNewBorder)) / 2.)))
          throw GeometryFailure(DivisionByZero);

        // если угол расширения оказался всё же больше угла сектора
        // и расстояния не хватает, чтобы покрыть полигоном расширения
        // всю нужную часть окружности, тогда пересчитываем расстояние
        // по углу расширения
        triangleSide = radius / sqrt((1. + Geometry::cosAngle(
          nearPoint, center, pointNewBorder)) / 2.);
      }

      additionalSector = { {
        Geometry::findPointAtLine(nearPoint, center, triangleSide, true),
        Geometry::findPointAtLine(pointNewBorder, center, triangleSide, true),
        pointNewBorder, nearPoint
        } };
    }
    else
    {
      // Формируем полигон расширения сектора
      // Если угол расширения не больше угла исходного сектора,
      // выбираем для точек расширения такое расстояние от центра,
      // чтобы точка на общей линии с сектором совпала с 
      // соответствующей точкой сектора. Это нужно для того,
      // чтобы было меньше шансов столкнуться с проблемами при 
      // объединении полигонов сектора и расширения
      double triangleSide = radius
        / sqrt((1. + Geometry::cosAngle(
          sector.leftPoint, center, sector.rightPoint)) / 2.);

      // Граничная точка сектора
      Point2D curBorderPoint;
      // Точка, полученная в результате продления отрезка, соединяющего центр
      // окружности и граничную точку, за пределы окружности
      Point2D externalBorderPoint;
      if (isRightExpansion)
      {
        curBorderPoint = sector.rightPoint;
        externalBorderPoint = sector.externalRightPoint;
      }
      else
      {
        curBorderPoint = sector.leftPoint;
        externalBorderPoint = sector.externalLeftPoint;
      }

      if ((Geometry::findAngle(
        sector.rightPoint, center, sector.leftPoint)).get()
        > (M_PI / 2 + ACCURACY_DOUBLE))
      {
        triangleSide = radius;
      }
      else if ((radius / sqrt((1. + Geometry::cosAngle(
        curBorderPoint, center, pointNearBorder)) / 2.)) > triangleSide)
      {
        if (BasicMath::isNull(sqrt((1. + Geometry::cosAngle(
          curBorderPoint, center, pointNearBorder)) / 2.)))
          throw GeometryFailure(DivisionByZero);

        // если угол расширения оказался всё же больше угла сектора
        // и расстояния не хватает, чтобы покрыть полигоном расширения
        // всю нужную часть окружности, тогда пересчитываем расстояние
        // по углу расширения
        triangleSide = radius / sqrt((1. + Geometry::cosAngle(
          curBorderPoint, center, pointNearBorder)) / 2.);
        externalBorderPoint = Geometry::findPointAtLine(curBorderPoint, center,
                                                        triangleSide, true);
      }

      // Отодвигаем от крайней точки на расстояние ширины трубки
      additionalSector = { {
        externalBorderPoint,
        Geometry::findPointAtLine(pointNearBorder, center, triangleSide, true),
        pointNearBorder, curBorderPoint
          } };
    }
    // Берём объединение нового сектора с исходным
    MultiPolygon2D newSector;

    try
    {
      std::string message;
      if (!correct(additionalSector, message))
        throw GeometryFailure(OperationNotPerformedCorrect,
          message, "expandOneSectorsBorder:");

      unionOf(sector.poly, additionalSector, newSector);
    }
    catch (...)
    {
      ExceptionTools::FailureHandler<GeometryFailure>("expandOneSectorsBorder");
    }

    sector.poly = newSector[0];
  }
//==============================================================================
  void expandSectorsBorders(
    SectorPoly& sector, const Point2D& center, double radius, double widthExpansion)
  {
    if (sector.poly.outer().size() < 1)
      throw GeometryFailure(EmptySectorPolygon);

    if (radius < -ACCURACY_DOUBLE)
      throw GeometryFailure(NegativeRadius);

    if (widthExpansion > ACCURACY_DOUBLE)
    {
      // Строим прямоугольники, в противоположных углах которых 
      // крайняя точка сектора и финишная точка КУ
      // Найдём недостающие точки
      Geometry::Circle circleFromSectorRight(sector.rightPoint, widthExpansion);
      Geometry::Circle circleFromSectorLeft(sector.leftPoint, widthExpansion);
      Geometry::Circle circleFromCenter(center, widthExpansion);

      try
      {
        Point2D rightPointNearBorder
          = circleFromSectorRight.tangentPoint(center, false);
        Point2D rightPointNearFinish
          = circleFromCenter.tangentPoint(sector.rightPoint, false);

        Point2D leftPointNearBorder
          = circleFromSectorLeft.tangentPoint(center, true);
        Point2D leftPointNearFinish
          = circleFromCenter.tangentPoint(sector.leftPoint, true);

        // Формируем полигоны прямоугольников
        Polygon2D rightRectangle{ { 
          center, rightPointNearBorder, sector.rightPoint, rightPointNearFinish
        } };
        Polygon2D leftRectangle{ {
          center, leftPointNearBorder, sector.leftPoint, leftPointNearFinish
        } };

        correct(rightRectangle);
        correct(leftRectangle);

        // Ищем пересечение прямоугольников с сектором
        MultiPolygon2D rightProblemZone;
        MultiPolygon2D leftProblemZone;

        intersection(sector.poly, rightRectangle, rightProblemZone);
        intersection(sector.poly, leftRectangle, leftProblemZone);

        // Расширяем сектор вправо
        expandOneSectorsBorder(
          sector, center, radius, widthExpansion,
          rightProblemZone, rightPointNearBorder, true);

        // Расширяем сектор влево
        expandOneSectorsBorder(
          sector, center, radius, widthExpansion,
          leftProblemZone, leftPointNearBorder, false);
      }
      catch (...)
      {
        ExceptionTools::FailureHandler<GeometryFailure>("expandSectorsBorders");
      }
    }
  }
//==============================================================================
  std::vector<SectorPoly> getSectorsInPoints(
    const MultiPolygon2D& polygons,
    const Point2D& center,
    bool findPolygons,
    double radius /*= 0*/)
  {
    // Зададим сектора, ограничивающие полигоны по их крайним точкам
    std::vector<SectorPoly> sectors;
    sectors.reserve(polygons.size());

    // Строим сектора
    for (const auto& polygon : polygons)
    {
      std::vector<size_t> iLeftPoints, iRightPoints;

      // Находим все левые и правые точки
      findPolygonLeftAndRightPoints(
        polygon, center, iLeftPoints, iRightPoints);

      // Из всех левых точек выбираем самую левую
      size_t iLeft = findPolygonBorderedPoint(
        polygon, center, iLeftPoints, false);

      // Из всех правых точек выбираем самую правую
      size_t iRight = findPolygonBorderedPoint(
        polygon, center, iRightPoints, true);

      // Проверяем сектор на полный угол
      SectorPoly curSector(polygon.outer()[iRight], polygon.outer()[iLeft]);
      Geometry::BorderedLine sectorsLeftBorder(center, polygon.outer()[iLeft]);
      Geometry::BorderedLine sectorsRightBorder(center, polygon.outer()[iRight]);
      for (size_t i = 0; i < polygon.outer().size() - 1; ++i)
      {
        // Ищем пересечение границ сектора с рёбрами полигона
        if (sectorsLeftBorder.isIntersectWithoutBorder(Geometry::BorderedLine(
          polygon.outer()[i], polygon.outer()[i + 1]))
          || sectorsRightBorder.isIntersectWithoutBorder(Geometry::BorderedLine(
            polygon.outer()[i], polygon.outer()[i + 1])))
        {
          curSector.isFullSector = true;
          break;
        }
      }

      // Формируем полигон сектора
      if (findPolygons && !curSector.isFullSector)
        getSectorPolygon(polygon, center, radius, iLeft, iRight, curSector);

      sectors.emplace_back(curSector);
    }

    return sectors;
  }
//==============================================================================
  MultiPolygon2D calculateCake(
    const MultiPolygon2D& area,
    const Point2D& center,
    const double radius,
    const double widthExpansion
    )
  {
    if(area.size() < 1)
      throw GeometryFailure(EmptyPolygon);

    if (radius < -ACCURACY_DOUBLE)
      throw GeometryFailure(NegativeRadius);

    // Если нет ограничивающей окружности, препятствий нет,
    // возвращаем пустой мультиполигон
    if (BasicMath::isNull(radius))
      return MultiPolygon2D();

    // Задаём окружность, ограничивающую область
    Polygon2D finishCircle = getPolyCircle(center, radius);

    // Получим полигоны, попавшие в окружность
    MultiPolygon2D polygons;

    try
    {
      difference(finishCircle, area, polygons);
    }
    catch (...)
    {
      ExceptionTools::FailureHandler<GeometryFailure>("FlightCircleCalculator");
    }

    // Если в окружности нет полигонов, препятствий нет,
    // возвращаем пустой мультиполигон
    if (polygons.size() == 0)
      return polygons;

    MultiPolygon2D polyCake;

    // Заранее проверим, нет ли у полигонов более двух точек на одном
    // ребре. Если есть - очищаем их
    for (auto& polygon : polygons)
    {
      // Если после удаления лишних точек полигон выродился, он некорректен
      if (!AbstractOperations::deleteExcessVertices(polygon.outer()))
        throw GeometryFailure(IncorrectGeometry);
    }

    // Если центр попал препятствие, вся область - запретная
    if (within(center, polygons))
    {
      SectorPoly curSector(0., 0.);
      curSector.poly.outer().insert(curSector.poly.outer().end(),
        finishCircle.outer().begin(), finishCircle.outer().end());

      polyCake.emplace_back(curSector.poly);
      return polyCake;

    }

    // Зададим сектора, ограничивающие полигоны по их крайним точкам
    std::vector<SectorPoly> sectors 
      = getSectorsInPoints(polygons, center, true, radius);

    // Строим сектора
    for (auto& curSector : sectors)
    {
      // Если один из секторов формирует полный сектор, сразу его 
      // возвращаем
      if (curSector.isFullSector)
      {
        curSector.poly.outer().insert(curSector.poly.outer().end(),
          finishCircle.outer().begin(), finishCircle.outer().end());

        polyCake.emplace_back(curSector.poly);
        return polyCake;
      }
    }

    // Расширяем границы
    for (auto& sector : sectors)
    {
      if (!sector.isFullSector)
        expandSectorsBorders(sector, center, radius, widthExpansion);
    }
      
    // Объединяем сектора в мультиполигон
    polygons.clear();
    polygons.emplace_back(sectors[0].poly);

    try
    {
      for (size_t i(1); i < sectors.size(); ++i)
      {
        MultiPolygon2D tempPoly;
        unionOf(polygons, sectors[i].poly, tempPoly);
      polygons = std::move(tempPoly);
      }
      correct(polygons);

      // Обрежем сектора по окружности конечного участка
      intersection(polygons, finishCircle, polyCake);

      // Если есть какие-то внутренности, удаляем их
      for (auto& sector : polyCake)
        sector.inners().clear();

    }
    catch (...)
    {
      ExceptionTools::FailureHandler<GeometryFailure>("FlightCircleCalculator");
    }

    return polyCake;
  }
//==============================================================================
} //! Geometry
//==============================================================================
