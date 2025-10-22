#ifndef GEOMETRY_OPERATIONS_POLYGON_TO_CIRCLES_HPP
#define GEOMETRY_OPERATIONS_POLYGON_TO_CIRCLES_HPP
//==============================================================================
#include <Geometry/Objects/Circle.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//==============================================================================
namespace Geometry
{
/*!
 * \brief toPolygon -- функция, возвращающая для круга
 * его полигональное приближение в виде описанного около него
 * правильного многоугольника.
 *
 * \param[in] circle -- исходный круг.
 *
 * \param[in] countOfVertices -- количество вершин,
 * из которых должно состоять полигональное приближение круга
 * (без учёта последней, замыкающей).
 *
 * \param[in] flag --
 *
 * \return Искомый многоугольник.
 *
 * \details
 * Для круга с нулевым радиусом
 * возвращается многоугольник с единственной вершиной;
 * для круга с отрицательным радиусом возвращается пустой многоугольник.
 */
Polygon2D
  toPolygon(
    const Circle& circle,
    const unsigned int countOfVertices = 360U, bool flag = true) noexcept;
//==============================================================================
/*!
 * \brief minimalCoveringCircle -- функция,
 * возвращающая для множества точек на плоскости круг с минимальным радиусом,
 * покрывающий всё это множество.
 *
 * \param[in] vertices - исходное множество точек (должно быть непустым).
 *
 * return Искомый круг.
 *
 * \throws GeometryFailure # MinimalCircleCoveringEmptyPolygon
 */
Circle minimalCoveringCircle(const MultiPoint2D& vertices );
//==============================================================================
/*!
 * \brief minimalCoveringCircle -- функция,
 * возвращающая для многоугольника на плоскости круг с минимальным радиусом,
 * содержащий в себе этот многоугольник.
 *
 * \param[in] polygon - исходный многоугольник (должен быть непустым).
 *
 * return Искомый круг.
 *
 * \throws GeometryFailure # MinimalCircleCoveringEmptyPolygon
 */
Circle minimalCoveringCircle(const Polygon2D& polygon);
//==============================================================================
/*!
 * \brief polygonToCircles -- функция,
 * получающая на вход вещественное число от 0 до 1,
 * характеризующее степень точности покрытия многоугольника на плоскости
 * кругами,
 * построение которого является её основной задачей.
 *
 * \param[in] polygon -- многоугольник,
 * для которого требуется построить покрытие кругами.
 *
 * \param[in] accuracy -- вещественное число из полуинтервала [0, 1),
 * обозначающее желаемую точность.
 *
 * \return массив кругов, составляющих искомое покрытие.
 *
 * \throws GeometryFailure # PolygonToCirclesInvalidPolygon
 *         Failure         # UndefinedFailure
 *
 * \details Покрытие состоит из кругов с натуральным (в метрах) радиусом;
 * длина радиуса каждого круга кратна 1000 метров;
 * приоритет при построении -- минимизация количества кругов
 * при соблюдении фиксированной точности
 * (точностью называется отношение площади исходного многоугольника
 * к площади объединения всех кругов
 * из некоторого покрытия этого многоугольника).
 *
 * Если точность оказалась некорректно заданной,
 * ей присваивается ближайшее значение из [0, 1).
 *
 * Чем больше значение точности, тем дольше будет работать функция при вызове.
 *
 * Внутренние кольца многоугольника игнорируются.
 */
std::vector<Circle>
polygonToCircles(
  Polygon2D polygon,
  double accuracy = 0.7,
  const unsigned int minRadius = 1000U);
//==============================================================================
/*!
 *\brief Поиск максимального количества вершин описанного многоугольника при
 *       заданном ограничении на длины его сторон.
 *\param[in] radius     - радиус окружности;
 *\param[in] sideLength - минимальная длина стороны описанного многоульника;
 *\return - количество вершин многоугольника (не меньше 3).
 *\throws GeometryFailure # NegativeRadius ||
 *                          NegativePolygonSideLength ||
 *                          PointsCountForPolygonNotValid ||
 *                          NoMaximumPointsCountForPolygon
 */
int pointsCountForCircumscribedPolygon(double radius, double sideLength);
//==============================================================================
} //! Geometry
//==============================================================================
#endif //! GEOMETRY_OPERATIONS_POLYGON_TO_CIRCLES_HPP