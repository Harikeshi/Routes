#include "Triangulation/Area.hpp"

#include "Triangulation/Delauney.hpp"
#include <TriangularGeometry/TriangularException.hpp>
//! Structure
#include "Triangulation/Objects/Structure/Head.hpp"
//! Objects
#include "Triangulation/Objects/Edge.hpp"
#include "Triangulation/Objects/Top.hpp"
#include "Triangulation/Objects/Triangle.hpp"
//! std
#include <limits>
//==============================================================================
namespace TriangularGeometry
{
//==============================================================================
Area::Area(int exponenta)
  : mExponenta(int(pow(10, exponenta)))
  , tops(std::deque<std::shared_ptr<Top>>())
  , delauney(new Delauney)
  , mLastPointId(std::numeric_limits<uint32_t>::max())
  , mLastIndex(1)
{}
//==============================================================================
Area::~Area()
{
  clearTops();
}
//==============================================================================
void Area::clearTops()
{
  tops.clear();
}
//==============================================================================
void Area::insertFixitPolygon(uint32_t first, uint32_t last)
{
  try
  {
    for (uint32_t i = first; i < last; i++)
    {
      delauney->addFixedEdge(tops[i], tops[i + 1]);
    }
    delauney->addFixedEdge(tops[last], tops[first]);
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<TriangularFailure>("<insertFixitPolygon>");
  }
}
//==============================================================================
void Area::makeTriangulation()
{
  try
  {
    delauney->triangulate(tops);
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<TriangularFailure>("<makeTriangulation>");
  }
}
//==============================================================================
void Area::clearingTriangulation(const std::vector<uint32_t>& startHoles)
{
  std::vector<std::pair<std::shared_ptr<Top>, std::shared_ptr<Top>>> polygons;
  std::size_t count = startHoles.size();
  for (std::size_t i = 0; i < count; i++)
    polygons.push_back(std::make_pair(tops[startHoles[i]],
                                      tops[startHoles[i]+1]));

  try
  {
    // Расчистка триангуляции
    delauney->clearingBadEdges(polygons);
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<TriangularFailure>("<clearingTriangulation>");
  }
}
//==============================================================================
void Area::insertTop(double x, double y)
{
  tops.push_back(std::make_shared<Top>(x * mExponenta, y * mExponenta));
  uint32_t count = static_cast<uint32_t>(tops.size()) - 1;
  tops[count]->id = count;
}
//==============================================================================
std::vector<uint32_t> Area::trianglesVerties()
{
  std::vector<uint32_t> triangles;
  triangles.clear();
  
  //! Собираем трегольники, заменяя вершины на их индексы в исходной
  //! последовательности
  std::shared_ptr<Top> top0(nullptr);
  std::shared_ptr<Top> top1(nullptr);
  std::shared_ptr<Top> top2(nullptr);
  std::shared_ptr<Triangle> tr =
            std::dynamic_pointer_cast<Triangle>(delauney->listTrian->first());
  while (tr)
  {
    tr->allTops(top0, top1, top2);
    triangles.push_back(top0->id);
    if (Top::isClockwise(top0, top1, top2))
    {
      triangles.push_back(top1->id);
      triangles.push_back(top2->id);
    }
    else
    {
      triangles.push_back(top2->id);
      triangles.push_back(top1->id);
    }
    tr = std::dynamic_pointer_cast<Triangle>(tr->next());
  }

  return triangles;
}
//==============================================================================
void Area::setFixedEdgesToTops()
{
  //! Берём первое ребро из списка всех рёбер
  std::shared_ptr<Edge> curEdge =
    std::dynamic_pointer_cast<Edge>(delauney->listEdge->first());

  while (curEdge)
  {
    //! Задаем вершинам фиксированное ребро
    if (curEdge->isBound())
    {
      curEdge->getI()->edge = curEdge;
      curEdge->getJ()->edge = curEdge;
    }
    curEdge = std::dynamic_pointer_cast<Edge>(curEdge->next());
  }
}
//==============================================================================
uint32_t 
Area::addNeighboursInList(std::shared_ptr<Triangle> tr,
                          std::deque<std::shared_ptr<Triangle>>& listTrs)
{
  uint32_t count(0);
  std::shared_ptr<Triangle> tr1(nullptr);
  std::shared_ptr<Triangle> tr2(nullptr);
  std::shared_ptr<Triangle> tr3(nullptr);
  tr->nearTriangle(tr1, tr2, tr3);
  if (tr1 != nullptr)
  {
    if (!tr1->met)
    {
      listTrs.insert(listTrs.begin(), tr1);
      count++;
    }
  }
  if (tr2 != nullptr)
  {
    if (!tr2->met)
    {
      listTrs.insert(listTrs.begin(), tr2);
      count++;
    }
  }
  if (tr3 != nullptr)
  {
    if (!tr3->met)
    {
      listTrs.insert(listTrs.begin(), tr3);
      count++;
    }
  }
  return count;
}
//==============================================================================
void
Area::insertTopInSet(
      uint32_t top, std::deque<uint32_t>& contour, uint32_t top1, uint32_t top2)
{
  uint32_t index(0);
  // Точка вставляется рядом с предыдущей вставленной
  if (top1 == mLastPointId)
  {
    if (top2 != contour[mLastIndex - 1])
    {
      // Точка вставляется после последней добавленной
      mLastIndex++;
      contour.insert(contour.begin() + mLastIndex, top);
      mLastPointId = top;
      return;
    }
    else
    {
      // Точка вставляется перед последней добавленной
      contour.insert(contour.begin() + mLastIndex, top);
      mLastPointId = top;
      return;
    }
  }
  else if (top2 == mLastPointId)
  {
    if (top1 != contour[mLastIndex - 1])
    {
      // Точка вставляется после последней добавленной
      mLastIndex++;
      contour.insert(contour.begin() + mLastIndex, top);
      mLastPointId = top;
      return;
    }
    else
    {
      // Точка вставляется перед последней добавленной
      contour.insert(contour.begin() + mLastIndex, top);
      mLastPointId = top;
      return;
    }
  }
  else
  {
    // Проверка что вставка осуществляется в конец
    uint32_t lastTop = static_cast<uint32_t>(contour.size()) - 1;

    if (((contour[0] == top1) && (contour[lastTop] == top2))
     || ((contour[0] == top2) && (contour[lastTop] == top1)))
    {
      contour.push_back(top);
      mLastIndex = lastTop + 1;
      mLastPointId = top;
      return;
    }
    
    // Ищем позицию добавления вершины
    for (uint32_t i = lastTop; i > 0; i--)
    {
      if (contour[i] == top2)
      {
        index = i;
        break;
      }
    }
    if (index == 0)
    {
      if (contour[1] != top1)
      {
        // Вставка осуществляется в конец
        contour.push_back(top);
        mLastIndex = lastTop + 1;
        mLastPointId = top;
        return;
      }
      else
      {
        // Вставка на позицию 1
        mLastIndex = 1;
        contour.insert(contour.begin() + mLastIndex, top);
        mLastPointId = top;
        return;
      }
    }
    else if (contour[index - 1] == top1)
    {
      // Вставка на позицию top2
      mLastIndex = index;
      contour.insert(contour.begin() + mLastIndex, top);
      mLastPointId = top;
      return;
    }
    else if (index == lastTop)
    {
      // Вставка осуществляется в конец
      contour.push_back(top);
      mLastIndex = lastTop + 1;
      mLastPointId = top;
      return;
    }
    else
    {
      // Вставка на позицию после top2
      mLastIndex = index + 1;
      contour.insert(contour.begin() + mLastIndex, top);
      mLastPointId = top;
      return;
    }
  }
}
//==============================================================================
} // !TriangularGeometry
//==============================================================================
