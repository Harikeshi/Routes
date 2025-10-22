#ifndef ABSTRACT_OPERATIONS_ALGORITHMS_HELPER_CORRECT_HELPER_HPP
#define ABSTRACT_OPERATIONS_ALGORITHMS_HELPER_CORRECT_HELPER_HPP
//==============================================================================
//! PrimaryEntities
#include <PrimaryEntities/Entities/Ring.hpp>
#include <PrimaryEntities/Entities/Polygon.hpp>
#include <PrimaryEntities/Entities/MultiPolygon.hpp>
//==============================================================================
namespace AbstractOperations
{
namespace Helper
{
//==============================================================================
/*!
 * \brief Функция замыкания колец
 * \details Фиктивная функция для геометрий, не содержащих колец.
 * \param[in|out] inEntity - геометрия, в которой требуется замкнуть кольца
 */
template <typename InEntity>
void closeRings(InEntity& inEntity) {}
//==============================================================================
/*!
 * \brief Функция замыкания колец
 * \details Реализация для кольца
 * \param[in|out] inEntity - кольцо
 */
template <typename PointType>
void closeRings(PrimaryEntities::Ring<PointType>& inEntity)
{
  if (!inEntity.empty()
    && !(inEntity.at(0) == inEntity.at(inEntity.size() - 1)))
    inEntity.push_back(inEntity.at(0));
}
//==============================================================================
/*!
 * \brief Функция замыкания колец
 * \details Реализация для полигона
 * \param[in|out] inEntity - полигон
 */
template <typename PointType>
void closeRings(PrimaryEntities::Polygon<PointType>& inEntity)
{
  closeRings(inEntity.outer());

  for (size_t i = 0; i < inEntity.inners().size(); ++i)
    closeRings(inEntity.inners().at(i));
}
//==============================================================================
/*!
 * \brief Функция замыкания колец
 * \details Реализация для мультиполигона
 * \param[in|out] inEntity - мультиполигон
 */
template <typename PointType>
void closeRings(PrimaryEntities::MultiPolygon<PointType>& inEntity)
{
  for (size_t i = 0; i < inEntity.size(); ++i)
    closeRings(inEntity.at(i));
}
//==============================================================================
/*!
 * \brief Исправление направления обхода замкнутых геометрий
 * \details Внешние кольца исправленной геометрии направлены по часовой
 *          стрелке, внутренние - против часовой стрелки.
 *          Незамкнутые геометрии исправлены не будут.
 * \param[in|out] geometry - геометрия geos
 */
void correctOrientation(std::unique_ptr<geos::geom::Geometry>& geometry);
//==============================================================================
/*!
 * \brief Проверка корректности направления обхода замкнутых геометрий
 * \details Незамкнутые геометрии всегда направлены корректно
 * \param[in|out] geometry - геометрия geos
 * \return true - внешние кольца геометрии направлены по часовой стрелке,
 *                а внутренние кольца - против часовой стрелки, либо
 *                геометрия незамкнута;
 *         false - направление колец замкнутой геометрии некорректно.
 */
bool hasCorrectOrientation(const geos::geom::Geometry* geometry);
//==============================================================================
} //! Helper 
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_ALGORITHMS_HELPER_CORRECT_HELPER_HPP