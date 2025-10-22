#include "Triangulation/Delauney.hpp"
//! Structure
#include "Triangulation/Objects/Structure/Head.hpp"
//! Objects
#include "Triangulation/Objects/Edge.hpp"
#include "Triangulation/Objects/Top.hpp"
#include "Triangulation/Objects/Triangle.hpp"
//! TriangularGeometry
#include <TriangularGeometry/TriangularException.hpp>
//! Geometry
#include <Geometry/Objects/BorderedLine.hpp>
#include <Geometry/Operations/AtPoints2D.hpp>
//! std
#include <cstddef>
#include <memory>
#include <limits>
#include <stdint.h>
//==============================================================================
namespace TriangularGeometry
{
//==============================================================================
Delauney::Delauney()
  : listEdge(nullptr)
  , listTrian(nullptr)
  , mHole(nullptr)
  , mArea(std::deque<std::shared_ptr<Top>>())
  , mEmptyTr(new Triangle)
{
  listEdge = Head::create();
  listTrian = Head::create();
  mHole = Head::create();
  mEmptyTr->finit = false;
}
//==============================================================================
void Delauney::triangulateArea(const std::deque<std::shared_ptr<Top>>& tops)
{
  std::size_t countTops(tops.size() - 1);
  //! Задание начальных экстремльных значений
  double minX(tops[0]->getX()); 
  double minY(tops[0]->getY());
  double maxX(tops[0]->getX());
  double maxY(tops[0]->getY());
  //! Поиск экстремальных значений
  for (std::size_t i = 1; i <= countTops; i++)
  {
    if (tops[i]->getX() < minX)
      minX = tops[i]->getX();
    else if (tops[i]->getX() > maxX)
      maxX = tops[i]->getX();
    if (tops[i]->getY() < minY)
      minY = tops[i]->getY();
    else if (tops[i]->getY() > maxY)
      maxY = tops[i]->getY();
  }
  //! Построение области триангуляции
  minX -= 1;
  minY -= 1;
  maxX += 1;
  maxY += 1;
  mArea.push_back(std::make_shared<Top>(minX, minY));
  mArea.push_back(std::make_shared<Top>(minX, maxY));
  mArea.push_back(std::make_shared<Top>(maxX, maxY));
  mArea.push_back(std::make_shared<Top>(maxX, minY));
  for (std::size_t i = 0; i < 4; i++)
    mArea[i]->id = std::numeric_limits<uint32_t>::max();
  //! Задание начальной триангуляции
  std::shared_ptr<Edge> p = std::make_shared<Edge>(mArea[2], mArea[0]);
  std::shared_ptr<Edge> q = std::make_shared<Edge>(mArea[3], mArea[0]);
  std::shared_ptr<Edge> r = std::make_shared<Edge>(mArea[2], mArea[3]);
  std::shared_ptr<Edge> ed1 = std::make_shared<Edge>(mArea[2], mArea[1]);
  std::shared_ptr<Edge> ed2 = std::make_shared<Edge>(mArea[0], mArea[1]);
  std::shared_ptr<Triangle> trA = std::make_shared<Triangle>(p, q, r);
  std::shared_ptr<Triangle> trB = std::make_shared<Triangle>(ed1, ed2, p);
  p->left = trA;
  p->right = trB;
  q->left = mEmptyTr;
  q->right = trA;
  r->left = mEmptyTr;
  r->right = trA;
  ed1->left = trB;
  ed1->right = mEmptyTr;
  ed2->left = mEmptyTr;
  ed2->right = trB;
  trA->finit = false;
  trB->finit = false;
  p->into(listEdge);
  q->into(listEdge);
  r->into(listEdge);
  ed1->into(listEdge);
  ed2->into(listEdge);
  trB->into(listTrian);
  trA->into(listTrian);
}
//==============================================================================
void Delauney::include(std::shared_ptr<Top> top)
{
  //! Локализация вершины в триангуляции
  std::shared_ptr<Triangle> tr = nest(top);

  if (!(tr))
    throw TriangularFailure(TriangularEnumFailure::DelauneyNoTriangle);

  //! Расчистка триангуляции от граней, не соответствующих условию Делоне
  distroyNet(tr, top);
  //! Штопка. Построение новых граней
  recoveryNet(top);
}
//==============================================================================
std::shared_ptr<Triangle> Delauney::nest(std::shared_ptr<Top> top)
{
  std::shared_ptr<Triangle> tr = 
                        std::dynamic_pointer_cast<Triangle>(listTrian->last());
  while (tr)
  {
    if (tr->isTopInTriangle(top))
      break;
    tr = std::dynamic_pointer_cast<Triangle>(tr->previous());
  }
  return tr;
}
//==============================================================================
void Delauney::distroyNet(std::shared_ptr<Triangle> tr,
                          std::shared_ptr<Top> top)
{
  std::shared_ptr<Triangle> trB(nullptr);
  std::shared_ptr<Edge> p(nullptr);
  std::shared_ptr<Edge> q(nullptr);
  std::shared_ptr<Top> i(nullptr);
  //! Формирование дыры
  p = tr->p; 
  p->into(mHole);
  p = tr->q; 
  p->into(mHole);
  p = tr->r; 
  p->into(mHole);
  tr->disTry();
  tr->out();
  tr.reset();
  p = std::dynamic_pointer_cast<Edge>(mHole->first());
  while (p)
  {
    if ((p->right == mEmptyTr) || (p->left == mEmptyTr))
      p = std::dynamic_pointer_cast<Edge>(p->next());
    else
    {
      trB = p->left;
      if (!(trB))
        trB = p->right;
      if (!(trB))
      {
        q = std::dynamic_pointer_cast<Edge>(p->next());
        p->out();
        p.reset();
      }
      else if (trB->isTopInCircle(top))
      {
        trB->disTry();
        trB->out();
        if (trB->p == p)
        {
          trB->q->follow(p);
          trB->r->follow(trB->q);
        }
        else if (trB->q == p)
        {
          trB->r->follow(p);
          trB->p->follow(trB->r);
        }
        else
        {
          trB->p->follow(p);
          trB->q->follow(trB->p);
        }
        q = std::dynamic_pointer_cast<Edge>(p->next());
        p->out();
        p.reset();
        trB->out();
        trB.reset();
      }
      else
        q = std::dynamic_pointer_cast<Edge>(p->next());
      p = q;
    }
  }
  //! Ожерелье
  p = std::dynamic_pointer_cast<Edge>(mHole->first());
  while (p)
  {
    if (!(p->left))
      i = p->getI();
    else
      i = p->getJ();
    i->precede(p);
    p = std::dynamic_pointer_cast<Edge>(p->next());
  }
}
//==============================================================================
void Delauney::recoveryNet(std::shared_ptr<Top> point)
{
  std::shared_ptr<Top> i(nullptr);
  std::shared_ptr<Top> top1(nullptr);
  std::shared_ptr<Top> top2(nullptr);
  std::shared_ptr<Top> top3(nullptr);
  std::shared_ptr<Edge> p(nullptr);
  std::shared_ptr<Edge> q(nullptr);
  std::shared_ptr<Edge> r(nullptr);
  std::shared_ptr<Triangle> tr(nullptr);
  i = std::dynamic_pointer_cast<Top>(mHole->first());
  while (i)
  {
    p = std::dynamic_pointer_cast<Edge>(i->next());
    q = std::make_shared<Edge>(point, i);
    q->precede(p);
    i->out();
    i = std::dynamic_pointer_cast<Top>(p->next());
  }
  q = std::dynamic_pointer_cast<Edge>(mHole->last());
  p = std::dynamic_pointer_cast<Edge>(q->previous());
  r = std::dynamic_pointer_cast<Edge>(mHole->first());
  while (r)
  {
    tr = std::make_shared<Triangle>(p, q, r);
    tr->allTops(top1, top2, top3);
    if (Top::isLine(top1, top2, top3))
    {
      p = r;
      q = std::dynamic_pointer_cast<Edge>(r->next());
      r = std::dynamic_pointer_cast<Edge>(q->next());
      tr->out();
      tr.reset();
    }
    else
    {
      tr->circle();
      finitTriangle(tr);
      p->left = tr;
      if (!(q->left))
        q->left = tr;
      else
        q->right = tr;
      r->right = tr;
      p = r;
      q = std::dynamic_pointer_cast<Edge>(r->next());
      r = std::dynamic_pointer_cast<Edge>(q->next());
      if (tr->finit)
        tr->precede(listTrian->first());
      else
        tr->into(listTrian);
    }
  }
  p = std::dynamic_pointer_cast<Edge>(mHole->first());
  while (p)
  {
    p->into(listEdge);
    p = std::dynamic_pointer_cast<Edge>(mHole->first());
  }
}
//==============================================================================
void Delauney::finitTriangle(std::shared_ptr<Triangle>& tr)
{
  if (tr->p->getI()->id == std::numeric_limits<uint32_t>::max())
    tr->finit = false;
  else if (tr->p->getJ()->id == std::numeric_limits<uint32_t>::max())
    tr->finit = false;
  else if (tr->q->getI()->id == std::numeric_limits<uint32_t>::max())
    tr->finit = false;
  else if (tr->q->getJ()->id == std::numeric_limits<uint32_t>::max())
    tr->finit = false;
}
//==============================================================================
void Delauney::triangulate(std::deque<std::shared_ptr<Top>>& tops)
{
  //! Очистка триангуляции
  clearTriangulation();
  //! Сортировка верщин
  quickSort(tops, 0, static_cast<int>(tops.size()) - 1);
  //! Определение границ триангуляции. Описывающий все вершины прямоугольник
  triangulateArea(tops);
  // Поочередное добавление точек в триангуляцию
  std::size_t countTops(tops.size());
  for (std::size_t i = 0; i < countTops; i++)
    include(tops[i]);
  //! Формирование ссылок вершин на рёбра
  std::shared_ptr<Edge> ed = std::dynamic_pointer_cast<Edge>(listEdge->first());
  while (ed)
  {
    ed->getI()->edge = ed;
    ed->getJ()->edge = ed;
    ed = std::dynamic_pointer_cast<Edge>(ed->next());
  }
  //! Возвращение изначальной сортировки вершин
  sortById(tops);
}
//==============================================================================
void Delauney::clearTriangulation()
{
  std::shared_ptr<Triangle> tr(nullptr);
  while (!listTrian->isEmpty())
  {
    tr = std::dynamic_pointer_cast<Triangle>(listTrian->first());
    tr->out();
    tr->disTry();
    tr.reset();
  }
  std::shared_ptr<Edge> edge(nullptr);
  while (!listEdge->isEmpty())
  {
    edge = std::dynamic_pointer_cast<Edge>(listEdge->first());
    edge->out();
    edge.reset();
  }
  mArea.clear();
}
//==============================================================================
Delauney::~Delauney()
{
  //! Очистка триангуляции
  clearTriangulation();
  //! Очистка списков и их размыкание
  listEdge->unlock();
  listTrian->unlock();
  mHole->unlock();
}
//==============================================================================
void Delauney::testOrientation()
{
  std::shared_ptr<Top> top(nullptr);
  std::shared_ptr<Triangle> tr(nullptr);
  bool revers;
  std::shared_ptr<Edge> ed = std::dynamic_pointer_cast<Edge>(listEdge->first());
  while (ed)
  {
    revers = false;
    tr = ed->left;
    if ((tr) && (tr != mEmptyTr))
    {
      top = tr->topOppositeEdge(ed);
      revers = (ed->position(*top) != Geometry::PlanePosition::Left);
    }
    else
    {
      tr = ed->right;
      if ((tr) && (tr != mEmptyTr))
      {
        top = tr->topOppositeEdge(ed);
        revers = (ed->position(*top) != Geometry::PlanePosition::Right);
      }
    }
    if (revers)
    {
      tr = ed->left;
      ed->left = ed->right;
      ed->right = tr;
    }
    ed = std::dynamic_pointer_cast<Edge>(ed->next());
  }
}
//==============================================================================
void
Delauney::clearingBadEdges(const std::vector<std::pair<std::shared_ptr<Top>,
                                             std::shared_ptr<Top>>>&
                           polygons)
{
  //! Правильное ориентирование ребер триангуляции
  testOrientation();

  //! Расчистка граней, прилегающих к границам области триангуляции
  clearingTriangulateArea();

  //! Расчистка граней, внутри дыр (заданных полигонов)
  std::size_t count = polygons.size();
  for (std::size_t i = 0; i < count; i++)
    clearingIntoPolygon(polygons[i].first, polygons[i].second);
   
  //! Удаление плохих ребер
  clearingSurplusEdges();

  //! Дополнительная проверка, что расчистилось не всё
  if (listTrian->isEmpty() && (polygons.size() != 0))
    throw TriangularFailure(TriangularEnumFailure::DelauneyClearSoMuch);
}
//============================================================================== 
void Delauney::clearingTriangulateArea()
{
  //! Фиксация ребер области триангуляции
  std::vector<std::shared_ptr<Edge>> areaEdges;
  std::size_t countTops(mArea.size() - 1);
  for (std::size_t i = 0; i < countTops; i++)
  {
    areaEdges.emplace_back(findEdge(mArea[i], mArea[i + 1]));
    if (!areaEdges[i])
      throw TriangularFailure(TriangularEnumFailure::DelauneyNoEdge);
    areaEdges[i]->fixed = true;
  }
  areaEdges.emplace_back(findEdge(mArea[countTops], mArea[0]));
  if (!areaEdges[countTops])
    throw TriangularFailure(TriangularEnumFailure::DelauneyNoEdge);
  areaEdges[countTops]->fixed = true;

  //! Инициализация списка расчищаемых граней, 
  //! прилегающих к границам триангуляции
  std::shared_ptr<Head> badTriangles = Head::create();
  if (  ((areaEdges[0]->left == mEmptyTr) && (areaEdges[0]->right == mEmptyTr))
    ||  ((areaEdges[0]->left != mEmptyTr) && (areaEdges[0]->right != mEmptyTr))
    ||  ((!(areaEdges[0]->left)) && (!(areaEdges[0]->right))))
    throw TriangularFailure(TriangularEnumFailure::DelauneyNoTriangle);

  if (areaEdges[0]->right == mEmptyTr)
    if (areaEdges[0]->left)
      areaEdges[0]->left->into(badTriangles);
    else
      throw TriangularFailure(TriangularEnumFailure::DelauneyNoTriangle);
  else 
    if (areaEdges[0]->right)
      areaEdges[0]->right->into(badTriangles);
    else
      throw TriangularFailure(TriangularEnumFailure::DelauneyNoTriangle);

  //! Удаление всех смежных граней через нефиксированные ребра
  clearingNearTriangles(badTriangles);

  //! Удаление рёбер
  for (auto edge : areaEdges)
  {
    edge->out();
    edge.reset();
  }

  //! Отключение цикличных ссылок списка, для корректной отработки деструктора
  badTriangles->unlock();
}
//==============================================================================
void Delauney::addFixedEdge(std::shared_ptr<Top> point1,
                            std::shared_ptr<Top> point2)
{
  std::shared_ptr<Top> top1(nullptr);
  std::shared_ptr<Top> top2(nullptr);
  std::shared_ptr<Edge> ed(nullptr);
  std::shared_ptr<Edge> ed1(nullptr);
  std::shared_ptr<Edge> ed2(nullptr);
  std::shared_ptr<Edge> rb1(nullptr);
  std::shared_ptr<Edge> rb2(nullptr);
  std::shared_ptr<Triangle> tr(nullptr);
  std::shared_ptr<Triangle> tr1(nullptr);
  std::shared_ptr<Triangle> tr2(nullptr);
  std::shared_ptr<Triangle> tr3(nullptr);
  double co, cosMin;
  // Образовать новое ребро
  ed = std::make_shared<Edge>(point1, point2);
  ed->fixed = true;

  // Найти все его пересечения с триангуляцией
  std::shared_ptr<Head> tempHeadL = Head::create();
  // Найти все треугольники инцидентные point1
  rb1 = point1->edge;
  if ((rb1->left) && (rb1->left != mEmptyTr))
    rb1->left->into(tempHeadL);
  if ((rb1->right) && (rb1->right != mEmptyTr))
    rb1->right->into(tempHeadL);
  tr = std::dynamic_pointer_cast<Triangle>(tempHeadL->first());
  while (tr)
  {
    if (!tr->met)
    {
      tr->met = true;
      tr->nearTriangle(tr1, tr2, tr3);
      if ((tr1) && (tr1 != mEmptyTr))
      {
        if ((!tr1->met) && (tr1->hasTopTriangle(point1)))
          tr1->into(tempHeadL);
      }
      if ((tr2) && (tr2 != mEmptyTr))
      {
        if ((!tr2->met) && (tr2->hasTopTriangle(point1)))
          tr2->into(tempHeadL);
      }
      if ((tr3) && (tr3 != mEmptyTr))
      {
        if ((!tr3->met) && tr3->hasTopTriangle(point1))
          tr3->into(tempHeadL);
      }
    }
    tr = std::dynamic_pointer_cast<Triangle>(tr->next());
  }

  // Найти первый треугольник маршрута
  bool isStructRib(false);
  tr = std::dynamic_pointer_cast<Triangle>(tempHeadL->first());
  while (tr)
  {
    tr1 = std::dynamic_pointer_cast<Triangle>(tr->next());
    // Проверка на совпадение структурного ребра
    if (ed->isSame(tr->p))
    {
      tr->p->fixed = true;
      tr->p->getI()->edge = tr->p;
      tr->p->getJ()->edge = tr->p;
      isStructRib = true;
    }
    else if (ed->isSame(tr->q))
    {
      tr->q->fixed = true;
      tr->q->getI()->edge = tr->q;
      tr->q->getJ()->edge = tr->q;
      isStructRib = true;
    }
    else if (ed->isSame(tr->r))
    {
      tr->r->fixed = true;
      tr->r->getI()->edge = tr->r;
      tr->r->getJ()->edge = tr->r;
      isStructRib = true;
    }
    if (isStructRib)
      break; //!< Есть структурное ребро
    else
    {
      rb1 = nullptr;

      if (ed->isIntersectEdge(tr->p))
        rb1 = tr->p;
      else if (ed->isIntersectEdge(tr->q))
        rb1 = tr->q;
      else if (ed->isIntersectEdge(tr->r))
        rb1 = tr->r;

      if (rb1 == nullptr)
      {
        tr->met = false;
        tr->into(listTrian);
      }
      else
        rb1->into(mHole);
    }
    tr = tr1;
  }

  //! Случай совпадения со структурным ребром
  if (isStructRib)
  {
    while (!mHole->isEmpty())
    {
      ed1 = std::dynamic_pointer_cast<Edge>(mHole->first());
      ed1->into(listEdge);
    }
    ed->out();
    ed.reset();
  }
  else
  {
    if (!mHole->isEmpty())
    {
      //! Вытягивание всего маршрута
      rb1 = std::dynamic_pointer_cast<Edge>(mHole->first());
      tr = std::dynamic_pointer_cast<Triangle>(tempHeadL->first());

      while (true)
      {
        // <-- Если что-то пошло не так, то фиксированное ребро не добавлять
        if (rb1 == nullptr)
          throw TriangularFailure(TriangularEnumFailure::DelauneyNoEdge);
        //! Такого быть конечно не должно, но что-то может пойти не так -->

        // Если одна из вершин удаляемого ребра ссылается на это же ребро,
        // изменим его на другое
        std::shared_ptr<Edge>& edgeI = rb1->getI()->edge;
        std::shared_ptr<Edge>& edgeJ = rb1->getJ()->edge;
        if (rb1->isSame(edgeI))
        {
          // Все рёбра, пересёкшиеся с добавляемым, должны быть удалены,
          // так что пропускаем их
          while (ed->isIntersectEdge(edgeI))
          {
            edgeI = edgeI->getNeighbour(rb1->getI());
          }
        }
        if (rb1->isSame(edgeJ))
        {
          // Все рёбра, пересёкшиеся с добавляемым, должны быть удалены,
          // так что пропускаем их
          while (ed->isIntersectEdge(edgeJ))
          {
            edgeJ = edgeJ->getNeighbour(rb1->getJ());
          }
        }

        rb1->into(mHole);
        rb2 = nullptr;
        if (rb1->left == tr)
          tr1 = rb1->right;
        else
          tr1 = rb1->left;
        if (tr1 == nullptr)
          throw TriangularFailure(TriangularEnumFailure::DelauneyNoTriangle);
        if (tr1->hasTopTriangle(point2))
          break;
        if ((tr1->p != rb1) && ed->isIntersectEdge(tr1->p))
          rb2 = tr1->p;
        else if ((tr1->q != rb1) && ed->isIntersectEdge(tr1->q))
          rb2 = tr1->q;
        else if ((tr1->r != rb1) && ed->isIntersectEdge(tr1->r))
          rb2 = tr1->r;

        rb1 = rb2;
        tr = tr1;
      }
    }
  }
  while (!tempHeadL->isEmpty())
  {
    tr = std::dynamic_pointer_cast<Triangle>(tempHeadL->first());
    tr->met = false;
    tr->into(listTrian);
  }
  // разлочил список
  tempHeadL->unlock();

  if (isStructRib)
    return;
  // Разрушение пересеченных ребер
  std::shared_ptr<Head> tempHead              //!< Список плохих треугольников 
                                 = Head::create(); 
  while (!mHole->isEmpty())
  {
    ed1 = std::dynamic_pointer_cast<Edge>(mHole->first());
    ed1->left->into(tempHead);
    ed1->right->into(tempHead);
    ed1->out();
    ed1.reset();
  }
  // Разрушение плохих треугольников
  while (!tempHead->isEmpty())
  {
    tr1 = std::dynamic_pointer_cast<Triangle>(tempHead->first());
    if (tr1->p)
      tr1->p->into(mHole);
    if (tr1->q)
      tr1->q->into(mHole);
    if (tr1->r)
      tr1->r->into(mHole);
    tr1->disTry();
    tr1->out();
    tr1.reset();
  }
  // Зашивание дыры
  for (int j = 1; j <= 2; j++)
  {
    ed->into(tempHead);
    while (!tempHead->isEmpty())
    {
      // ed1 - Очередное открытое ребро
      ed1 = std::dynamic_pointer_cast<Edge>(tempHead->first());
      top2 = nullptr;
      // Поиск третьей вершины для открытого ребра
      cosMin = 2.0;
      ed2 = std::dynamic_pointer_cast<Edge>(mHole->first());
      while (ed2)
      {
        top1 = ed2->getI();
        for (int i = 1; i <= 2; i++)
        {
          if (ed1->position(*top1) == Geometry::PlanePosition::Left)
          {
            co = Geometry::cosAngle(*ed1->getI(), *top1, *ed1->getJ());
            if (co < cosMin)
            {
              cosMin = co;
              top2 = top1;
            }
          }
          top1 = ed2->getJ();
        }
        ed2 = std::dynamic_pointer_cast<Edge>(ed2->next());
      }
      //! Образуем новый треугольник 
      if (top2)
      {
        rb1 = std::make_shared<Edge>(top2, ed1->getJ());
        rb2 = std::make_shared<Edge>(ed1->getI(), top2);
        tr1 = std::make_shared<Triangle>(ed1, rb1, rb2);
        ed1->left = tr1;
        rb1->right = tr1;
        rb2->right = tr1;
        tr1->into(listTrian); 
        finitTriangle(tr1);
        adOrPaste(rb1, tempHead);
        adOrPaste(rb2, tempHead);
      } 
      ed1->into(listEdge);
      auto topI = ed1->getI();
      auto topJ = ed1->getJ();
      topI->edge = ed1;
      topJ->edge = ed1;
      ed1->setI(topI);
      ed1->setJ(topJ);
    }
    if (j == 1)
    {
      top1 = ed->getI();
      top2 = ed->getJ();
      ed->setI(top2);
      ed->setJ(top1);
      ed->right = ed->left;
      ed->left = nullptr;
    }
  }
  // разлочил список
  tempHead->unlock();
  
  mHole->clear();
}
//==============================================================================
void Delauney::adOrPaste(std::shared_ptr<Edge>& rb, std::shared_ptr<Head>& head)
{
  std::shared_ptr<Edge> ed = std::dynamic_pointer_cast<Edge>(mHole->first());
  while (ed)
  {
    if (((ed->getI() == rb->getI()) && (ed->getJ() == rb->getJ()))
     || ((ed->getI() == rb->getJ()) && (ed->getJ() == rb->getI())))
      break;
    ed = std::dynamic_pointer_cast<Edge>(ed->next());
  }
  if (ed)
  {
    if (!(ed->left))
      ed->left = rb->right;
    else if (!(ed->right))
      ed->right = rb->right;
    if (rb->right->p == rb)
      rb->right->p = ed;
    else if (rb->right->q == rb)
      rb->right->q = ed;
    else if (rb->right->r == rb)
      rb->right->r = ed;
    rb->out();
    rb.reset();
    ed->into(listEdge);
  }
  else
    rb->into(head);
}
//==============================================================================
void 
Delauney::quickSort(std::deque<std::shared_ptr<Top>>& tops, int first, int last)
{
  int i(first);
  int j(last);
  std::shared_ptr<Top> p = tops[(last + first) / 2];
  while (i <= j)
  {
    while (tops[i]->compare(p) == Less)
      i++;
    while (tops[j]->compare(p) == Larger)
      j--;
    if (i <= j)
    {
      std::swap(tops[i], tops[j]);
      i++;
      j--;
    }
  }
  if (j > first)
    quickSort(tops, first, j);
  if (last > i)
    quickSort(tops, i, last);
}
//==============================================================================
void Delauney::sortById(std::deque<std::shared_ptr<Top>>& tops)
{
  uint32_t countTops = static_cast<uint32_t>(tops.size());
  for (uint32_t i = 0; i < countTops; i++)
  {
    while (tops[i]->id != i)
    {
      std::swap(tops[tops[i]->id], tops[i]);
    };
  }
}
//==============================================================================
void Delauney::clearingIntoPolygon(std::shared_ptr<Top> first,
                                   std::shared_ptr<Top> second)
{
  // Поиск ребра в триангуляции
  std::shared_ptr<Edge> edge = findEdge(first, second);
  if (!(edge))
    throw TriangularFailure(TriangularEnumFailure::DelauneyNoEdge);

  if ((!(edge->left)) && (!(edge->right)))
    throw TriangularFailure(TriangularEnumFailure::DelauneyNoTriangle);

  //! Поиск грани, лежащей внутри дыры (полигона)
  //! Третья вершина грани, должна лежать слева от ребра, заданого вершинами,
  //! при этом направленность ребра должна быть от первой вершины ко второй
  
  //! Инициализация списка граней расчистки
  std::shared_ptr<Head> badTriangles = Head::create();

  if (edge->getI() == first)
  {
    if ((edge->left) && (edge->left != mEmptyTr))
      edge->left->into(badTriangles);
  }
  else
  {
    if ((edge->right) && (edge->right != mEmptyTr))
      edge->right->into(badTriangles);
  }

  //! Очистка всех связных граней внутри границы (фиксированных рёбер)
  clearingNearTriangles(badTriangles);
  
  //! Разрыв циклического списка
  badTriangles->unlock();
}
//==============================================================================
std::shared_ptr<Edge> Delauney::findEdge(std::shared_ptr<Top> first,
                                         std::shared_ptr<Top> second)
{
  std::shared_ptr<Edge> edge = first->edge;
  // Вершина уже имеет ссылку на это ребро
  if ( (edge) 
    && (((edge->getI() == first) && (edge->getJ() == second))
   || ((edge->getJ() == first) && (edge->getI() == second))))
    return edge;
  edge = second->edge;
  if ((edge)
    && (((edge->getI() == first) && (edge->getJ() == second))
    || ((edge->getJ() == first) && (edge->getI() == second))))
    return edge;

  edge = std::dynamic_pointer_cast<Edge>(listEdge->first());
  while (edge)
  {
    if (((edge->getI() == first) && (edge->getJ() == second))
      || ((edge->getJ() == first) && (edge->getI() == second)))
      return edge;
    edge = std::dynamic_pointer_cast<Edge>(edge->next());
  }
  // TODO: сейчас долгая проверка по всем ребрам.
  // После исправления ошибок в расчистке итд оптимизировать и это место

  return nullptr;
}
//==============================================================================
void Delauney::clearingSurplusEdges()
{
  std::shared_ptr<Edge> ed = std::dynamic_pointer_cast<Edge>(listEdge->first());
  std::shared_ptr<Edge> ed1(nullptr);
  while (ed)
  {
    ed1 = std::dynamic_pointer_cast<Edge>(ed->next());
    //! Для удаления внешних ребер триангуляции убираем пустые треугольники
    if (ed->left == mEmptyTr)
      ed->left = nullptr;
    else if (ed->right == mEmptyTr)
      ed->right = nullptr;
    //! Если нет ни одного треугольника - удаляем ребро
    if ((!(ed->left)) && (!(ed->right)))
    {
      ed->out();
      ed.reset();
    }
    ed = ed1;
  }
}
//==============================================================================
void Delauney::clearingNearTriangles(std::shared_ptr<Head>& listTriangles)
{
  std::shared_ptr<Head> badEdges;

  std::shared_ptr<Triangle> lastTr(nullptr);
  while (!listTriangles->isEmpty())
  {
    lastTr = std::dynamic_pointer_cast<Triangle>(listTriangles->first());
    if (!lastTr->p->fixed)
    {
      if ((lastTr->p->left == lastTr) && (lastTr->p->right))
        lastTr->p->right->into(listTriangles);
      else if ((lastTr->p->right == lastTr) && (lastTr->p->left))
        lastTr->p->left->into(listTriangles);
      lastTr->p->into(badEdges);
    }
    if (!lastTr->q->fixed)
    {
      if ((lastTr->q->left == lastTr) && (lastTr->q->right))
        lastTr->q->right->into(listTriangles);
      else if ((lastTr->q->right == lastTr) && (lastTr->q->left))
        lastTr->q->left->into(listTriangles);
      lastTr->q->into(badEdges);
    }
    if (!lastTr->r->fixed)
    {
      if ((lastTr->r->left == lastTr) && (lastTr->r->right))
        lastTr->r->right->into(listTriangles);
      else if ((lastTr->r->right == lastTr) && (lastTr->r->left))
        lastTr->r->left->into(listTriangles);
      lastTr->r->into(badEdges);
    }
    lastTr->out();
    lastTr->disTry();
    lastTr.reset();
  }
}
//==============================================================================
} // !TriangularGeometry
//==============================================================================
