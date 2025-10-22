#include <TriangularGeometry/TriangularArea.hpp>

#include <TriangularGeometry/TriangularException.hpp>
#include "Triangulation/Area.hpp"
#include "Triangulation/Objects/Top.hpp"
//==============================================================================
namespace TriangularGeometry
{
//==============================================================================
TriangularArea::TriangularArea(
  const std::vector<Polygon2D>& polygons,
  int exponenta /*= 0*/)
: mArea(std::make_shared<TriangularGeometry::Area>(exponenta))
{
  //! Формирование области триангуляции
  std::vector<std::pair<uint32_t, uint32_t>> tgPolygons;
  uint32_t countPoints(0);
  uint32_t position(0);
  int countPolygons(static_cast<int>(polygons.size()));
  int countInners(0);
  std::vector<uint32_t> startHoles;
  for (int i = 0; i < countPolygons; i++)
  {
    // Добавление внешних границ
    countPoints = static_cast<uint32_t>(polygons[i].outer().size() - 1);
    for (uint32_t j = 0; j < countPoints; j++)
      mArea->insertTop(
        polygons[i].outer()[j].getX(), polygons[i].outer()[j].getY());
    tgPolygons.push_back(std::pair<uint32_t, uint32_t>(position, countPoints));
    position += static_cast<uint32_t>(countPoints);

    // Добавление внутренних границ
    countInners = static_cast<int>(polygons[i].inners().size());
    for (int j = 0; j < countInners; j++)
    {
      countPoints = static_cast<uint32_t>(polygons[i].inners()[j].size() - 1);
      for (uint32_t k = 0; k < countPoints; k++)
        mArea->insertTop(
          polygons[i].inners()[j][k].getX(), polygons[i].inners()[j][k].getY());
      tgPolygons.push_back(
                         std::pair<uint32_t, uint32_t>(position, countPoints));
      startHoles.push_back(position);
      position += static_cast<uint32_t>(countPoints);
    }
  }

  try
  {
    //! Триангулирование области
    mArea->makeTriangulation();

    //! Добавление фиксированных ребер
    countPolygons = static_cast<int>(tgPolygons.size());
    for (int i = 0; i < countPolygons; i++)
    {
      mArea->insertFixitPolygon(tgPolygons[i].first, 
                                tgPolygons[i].first + tgPolygons[i].second - 1);
    }
    mArea->setFixedEdgesToTops();

    //! Расчистка триангуляции
    mArea->clearingTriangulation(startHoles);
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<TriangularFailure>("<TriangularArea>");
  }
}
//==============================================================================
std::vector<Polygon2D> TriangularArea::getTriangles()
{
  std::vector<Polygon2D> result;

  //! Получение треугольников
  std::vector<uint32_t> triangles(mArea->trianglesVerties());
  std::size_t count(triangles.size());
  if (count == 0)
    return result;

  //! Формирование полигонов
  Polygon2D polygon;
  polygon.outer().reserve(4);

  for (std::size_t i = 0; i < count; i += 3)
  {
    for (int j = 0; j < 3; j++)
      polygon.outer().push_back(*mArea->tops[triangles[i + j]]);
    polygon.outer().push_back(polygon.outer().at(0));
    result.emplace_back(polygon);
    polygon.clear();
  }
  
  return result;
}
//==============================================================================
} // !TriangularGeometry
//==============================================================================