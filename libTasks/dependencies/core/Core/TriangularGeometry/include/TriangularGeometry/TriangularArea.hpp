#ifndef TRIANGULAR_GEOMETRY_TRIANGULATIONAREA_HPP
#define TRIANGULAR_GEOMETRY_TRIANGULATIONAREA_HPP
//==============================================================================
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//! std
#include <functional>
//==============================================================================
namespace TriangularGeometry 
{
class Area;
//==============================================================================
  /*
  * \brief Область триангуляции
  */
class TriangularArea
{
public: 
//==============================================================================
  /*
  * \brief Конструктор
  * \param[in] polygons  - полигон, описывающий область триангуляции
  * \param[in] exponenta - количество значащих знаков после запятой
  * \throws TriangularFailure # DelauneyNoEdge
  * \throws TriangularFailure # DelauneyNoTriangle
  * \throws TriangularFailure # DelauneyClearSoMuch
  */
  TriangularArea(
    const std::vector<Polygon2D>& polygons,
    int exponenta = 0);
//==============================================================================
  /*
  * \brief Получение треугольников в виде полигонов
  * \return Полигоны, полученные из треугольников
  */
  std::vector<Polygon2D> getTriangles();
//==============================================================================
private:
  std::shared_ptr<TriangularGeometry::Area> 
  mArea;            //!< реализация функционала работы с областью триангуляции
//==============================================================================
  friend class PathFinder;
//==============================================================================
};
//==============================================================================
} // !TriangularGeometry
//==============================================================================
#endif //! TRIANGULAR_GEOMETRY_TRIANGULATIONAREA_HPP