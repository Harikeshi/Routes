#ifndef REACHABLE_AREA_CALCULATOR_REACHABLE_AREA_HPP
#define REACHABLE_AREA_CALCULATOR_REACHABLE_AREA_HPP
//==============================================================================
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
//==============================================================================
namespace ReachableArea
{
//==============================================================================
/*
 * /brief Построение ГЗДЦ с учётом препятсвий для заданной точки с указанной
 *        дальностью на плоскости
 * /param[in] point - точка, относительно которой строить ГЗДЦ
 * /param[in] range - дальность, на которую строить ГЗДЦ [м]
 * /param[in] restrictions - мультиполигон, задающий препятсвия
 * /param[in] area - область, по которой обрезать ГЗДЦ
 *                   Если задана пустая, то обрезать не надо.
 * /param[in] numVerticies - количество вершин в представлении окружности
 *                           (без учёта замыкания)
 * /return Полигон ГЗДЦ
 * /throw ReachableAreaFailure # IncorrectGeometry || NotPositiveRange
 *                                                 || PointInRestriction
*/
Polygon2D calcReachableArea(const Point2D& point, const double range,
                            const MultiPolygon2D& restrictions,
                            const Polygon2D& area = Polygon2D(),
                            const unsigned int numVerticies = 360);
//==============================================================================
/*
 * /brief Построение ГЗДЦ с учётом препятсвий для заданной точки с указанной
 *        дальностью в геопространственных координатах
 * /param[in] point - точка, относительно которой строить ГЗДЦ
 * /param[in] range - дальность, на которую строить ГЗДЦ [м]
 * /param[in] restrictions - мультиполигон, задающий препятсвия
 * /param[in] area - область, по которой обрезать ГЗДЦ
 *                   Если задана пустая, то обрезать не надо.
 * /param[in] cutByOrthodromy - тип геодезической линии для обрезки:
 *                              - true - по ортодромии;
 *                              - false - по локсодромии. 
 * /param[in] numVerticies - количество вершин в представлении окружности 
 *                           (без учёта замыкания)
 * /return Полигон ГЗДЦ
 * /throw ReachableAreaFailure # IncorrectGeometry || NotPositiveRange
 *                                                 || PointInRestriction
*/
GeoPolygon2D calcReachableArea(const GeoPoint2D& point, const double range,
                               const GeoMultiPolygon2D& restrictions,
                               const GeoPolygon2D& area = GeoPolygon2D(),
                               const bool cutByOrthodromy = false,
                               const unsigned int numVerticies = 360);
//==============================================================================
} //! ReachableArea
//==============================================================================
#endif //! REACHABLE_AREA_CALCULATOR_REACHABLE_AREA_HPP