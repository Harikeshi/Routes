#ifndef TRIANGULAR_GEOMETRY_PATHFINDER_HPP
#define TRIANGULAR_GEOMETRY_PATHFINDER_HPP
//==============================================================================
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//! TriangularGeometry
#include <TriangularGeometry/TriangularArea.hpp>
//! Triangulation
#include "Triangulation/Objects/Top.hpp"
//==============================================================================
namespace TriangularGeometry 
{
//==============================================================================
class PathHandler;
//==============================================================================
  /*
   * \brief Поиск пути между двумя точками по рёбрам триангуляции
   */
class PathFinder
{
//==============================================================================
public: 
//==============================================================================
  /*
   * \brief Конструктор
   *
   * \param[in] area  - область триангуляции
   */
  PathFinder(std::shared_ptr<TriangularArea> area);
//==============================================================================
  /*
   * \brief Поиск кратчайшего пути между двумя вершинами
   *
   * \param[in] startPoint - начальная вершина
   *
   * \param[in] finishPoint - конечная вершина
   *
   * \param[in] minLength - минимальная длина участка [м]
   *
   * \return Найденный путь
   */
  std::vector<Point2D>
    findPath(const Point2D& startPoint, const Point2D& finishPoint,
             const double minLength = 0.);
//==============================================================================
  /*
   * \brief Исправление готового маршрута
   *
   * \param[in] minLength - минимальная длина участка [м]
   *
   * \return Исправленный маршрут
   */
  std::vector<Point2D> correctPath(const double minLength);

  std::deque<std::shared_ptr<Top>>  getMPathCorner();
//==============================================================================
private:
//==============================================================================
  /*
   * \brief Локализация в триангуляции вершин,
   *        между которыми ищется путь.
   *
   * \details Осуществляется поиск граней, в которые попадают концевые
   *          вершины.
   *
   * \return true  - начальная и конечная вершины успешно локализованы;
   *         false - вершины локализовать не удалось.
   */
  bool checkPathTopsInTriangles();
//==============================================================================
  /*
   * \brief Проверка на возможность прокладки пути между двумя
   *        вершинами.
   *
   * \details Осуществляется проверка на достижимость концевой вершины
   *          из начальной для заданного маршрутного дерева.
   *
   * \throws TriangularFailure # OneOfPointsOutOfArea
   * \throws TriangularFailure # InconnectedPoints
   */
  void checkPointsConnection();
//==============================================================================
  /*
   * \brief Поиск кратчайшего пути.
   *
   * \details Производится поиск кратчайшего пути между концевыми
   *          вершинами на текущем маршрутном дереве.
   */
  void findBestPath();
//==============================================================================
  std::shared_ptr<Area> mArea;     //!< Область поиска

  std::shared_ptr<Top> mStartTop;  //!< Начальная вершина
  std::shared_ptr<Top> mFinishTop; //!< Конечная вершина

  std::shared_ptr<Triangle> mStartTriangle;  //!< Начальная грань
  std::shared_ptr<Triangle> mFinishTriangle; //!< Конечная грань

  std::deque<std::shared_ptr<Top>>  mPathCorner; //!< Проложенный маршрут

  std::shared_ptr<PathHandler> mPathHandler; //!< Обработчик маршрута
};
//==============================================================================
} // !TriangularGeometry
//==============================================================================
#endif //! TRIANGULAR_GEOMETRY_PATHFINDER_HPP
