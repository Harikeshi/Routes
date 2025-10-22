#include <TriangularGeometry/PathFinder.hpp>
//! TriangularGeometry
#include <TriangularGeometry/TriangularException.hpp>
//! Triangulation
#include "Triangulation/Area.hpp"
#include "Triangulation/Delauney.hpp"
#include "Triangulation/Objects/Structure/Head.hpp"
//! PathFinder
#include "PathFinder/PathHandler.hpp"
#include "PathFinder/PathCorrection.hpp"
//==============================================================================
namespace TriangularGeometry
{
//==============================================================================
  PathFinder::PathFinder(
    std::shared_ptr<TriangularArea> area)
    : mArea(area->mArea)
  { }
//==============================================================================
  std::vector<Point2D>
    PathFinder::findPath(const Point2D& startPoint, const Point2D& finishPoint,
                         const double minLength/* = 0.*/)
  {
    std::vector<Point2D> result;

    // Подготавливаем конечные вершины
    std::shared_ptr<Top> startTop = std::make_shared<Top>(
      startPoint.getX(), startPoint.getY());
    std::shared_ptr<Top> finishTop = std::make_shared<Top>(
      finishPoint.getX(), finishPoint.getY());

    mStartTop = nullptr;
    mFinishTop = nullptr;

    // Проверка на совпадение концевых вершин
    // с вершинами триангуляции
    for (auto top : mArea->tops)
    {
      // Инициализация информации о вершине маршрута
      top->resetInfo();

      // Поиск совпадения концевых вершин с текущей вершиной
      if (top->isSame(startTop))
        mStartTop = top;
      if (top->isSame(finishTop))
        mFinishTop = top;
    }

    if (!mStartTop)
      mStartTop = startTop;
    if (!mFinishTop)
      mFinishTop = finishTop;

    // Заполнение информации о концевых вершинах маршрута
    if (mStartTop->info == nullptr)
      mStartTop->resetInfo();
    mStartTop->info->distance = 0.;

    if (mFinishTop->info == nullptr)
      mFinishTop->resetInfo();

    try
    {
      // Проверка на возможность прокладки маршрута
      checkPointsConnection();
      // Прокладка кратчайшего маршрута
      findBestPath();
      // Исправляем маршрут
      result = correctPath(minLength);
    }
    catch (...)
    {
      ExceptionTools::FailureHandler<TriangularFailure>("findPath");
    }

    return result;
  }
//==============================================================================
  std::vector<Point2D> PathFinder::correctPath(const double minLength)
  {
    std::deque<std::shared_ptr<Top>> newPath;

    if (minLength > 0.)
    {
      PathCorrector pathCorrector(mPathCorner, mStartTriangle);

      try
      {
        newPath = pathCorrector.correctSectionLengths(minLength);
      }
      catch (...)
      {
        ExceptionTools::FailureHandler<TriangularFailure>("correctPath");
      }
    }
    else
    {
      newPath = mPathCorner;
    }

    std::vector<Point2D> result;
    for (const auto& top : newPath)
      result.push_back(*top);

    return result;
  }
//==============================================================================
  bool PathFinder::checkPathTopsInTriangles()
  {
    mStartTriangle = nullptr;
    mFinishTriangle = nullptr;

    // Локализация
    std::shared_ptr<Triangle> curTriangle =
      std::dynamic_pointer_cast<Triangle>(mArea->delauney->listTrian->first());

    while (curTriangle != nullptr)
    {
      // Инициализация начальных параметров грани для поиска
      // маршрута
      curTriangle->met = false;
      curTriangle->neighTr = nullptr;

      // Проверка на попадание концевых вершин в текущую грань
      if (curTriangle->isTopInTriangle(mStartTop))
        mStartTriangle = curTriangle;
      if (curTriangle->isTopInTriangle(mFinishTop))
        mFinishTriangle = curTriangle;

      curTriangle = std::dynamic_pointer_cast<Triangle>(curTriangle->next());
    }

    return ((mStartTriangle != nullptr) && (mFinishTriangle != nullptr));
  }
//==============================================================================
  void PathFinder::checkPointsConnection()
  {
    // Локализация концевых вершин в гранях триангуляции
    // Если хотя бы одна вершина не локализована, сообщаем об этом
    if (!checkPathTopsInTriangles())
      throw TriangularFailure(TriangularEnumFailure::OneOfPointsOutOfArea);

    // Волна по триангуляции - поиск связей
    std::deque<std::shared_ptr<Triangle>> triangles;
    triangles.push_back(mStartTriangle);

    // Текущий треугольник
    std::shared_ptr<Triangle> curTriangle(nullptr);
    // Соседние треугольники
    std::shared_ptr<Triangle> firstNeighbour(nullptr);
    std::shared_ptr<Triangle> secondNeighbour(nullptr);
    std::shared_ptr<Triangle> thirdNeighbour(nullptr);

    int i(0);
    while (i < triangles.size())
    {
      curTriangle = triangles[i];

      // Если дошли до конечного треугольника, концевая вершина
      // достижима
      if (curTriangle == mFinishTriangle)
      {
        triangles.clear();
        break;
      }

      // Ищем соседей текущего треугольника
      curTriangle->nearTriangle(
        firstNeighbour, secondNeighbour, thirdNeighbour);

      // Добавляем в список проверяемых треугольников всех соседей,
      // у которых ещё не сохранена соседняя грань (иначе этот
      // треугольник уже был добавлен)
      if (firstNeighbour)
      {
        if (firstNeighbour->neighTr == nullptr)
        {
          firstNeighbour->neighTr = curTriangle;
          triangles.push_back(firstNeighbour);
        }
      }

      if (secondNeighbour)
      {
        if (secondNeighbour->neighTr == nullptr)
        {
          secondNeighbour->neighTr = curTriangle;
          triangles.push_back(secondNeighbour);
        }
      }

      if (thirdNeighbour)
      {
        if (thirdNeighbour->neighTr == nullptr)
        {
          thirdNeighbour->neighTr = curTriangle;
          triangles.push_back(thirdNeighbour);
        }
      }

      i++;
    }

    // Если не почистили количество треугольников, значит, не был
    // найден маршрут
    if (!triangles.empty())
    {
      triangles.clear();
      throw TriangularFailure(TriangularEnumFailure::InconnectedPoints);
    }

    // Вытягивание маршрута
    curTriangle = mFinishTriangle;
    mFinishTriangle->met = true;

    // Проходя от конечной грани к начальной, отметим грани, по
    // которым получилось соединить вершины
    while ((curTriangle) && (curTriangle != mStartTriangle))
    {
      curTriangle = curTriangle->neighTr;
      curTriangle->met = true;
    }
  }
//==============================================================================
  void PathFinder::findBestPath()
  {
    mPathCorner.clear();

    // Если грани начальной и конечной вершин совпадают, маршрут
    // найден
    if (mStartTriangle == mFinishTriangle)
    {
      mPathCorner.emplace_back(mStartTop);
      mPathCorner.emplace_back(mFinishTop);
      return;
    }

    if (mPathHandler)
      mPathHandler.reset();

    mPathHandler = std::make_shared<PathHandler>();

    // Ищем маршрут от начальной до конечной точки вдоль рёбер
    // триангуляции
    mPathHandler->scanPathGraph(
      mStartTop, mFinishTop, mStartTriangle, mFinishTriangle);

    // Записываем полученный маршрут, начиная с конца
    mPathCorner.emplace_back(mFinishTop);
    std::shared_ptr<Top> top = mFinishTop;

    while ((top) && (top != mStartTop))
    {
      // Берём предыдущую точку для последней записанной вершины
      top = top->info->prevTop;

      // Если точки нет, значит, осталась только начальная
      if (!top)
      {
        mPathCorner.emplace_front(mStartTop);
        break;
      }
      mPathCorner.emplace_front(top);
    }
  }

  std::deque<std::shared_ptr<Top>> PathFinder::getMPathCorner()
  {return mPathCorner;}
//==============================================================================
} // !TriangularGeometry
//==============================================================================
