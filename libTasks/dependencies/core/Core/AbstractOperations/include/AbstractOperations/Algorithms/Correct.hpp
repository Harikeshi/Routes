#ifndef ABSTRACT_OPERATIONS_ALGORITHMS_CORRECT_HPP
#define ABSTRACT_OPERATIONS_ALGORITHMS_CORRECT_HPP
//==============================================================================
#include <AbstractOperations/Algorithms/Helper/CorrectHelper.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryConverter.hpp>
//! Geos
#include <geos/operation/valid/IsValidOp.h>
#include <geos/operation/valid/MakeValid.h>
#include <geos/operation/valid/RepeatedPointRemover.h>
//==============================================================================
namespace AbstractOperations
{
//==============================================================================
/*!
 * \brief Исправление геометрий (сущностей) на плоскости
 * \details Реализация на основе geos:
 *          - исправление самопересечений (geos::operation::valid::MakeValid);
 *          - исключение дублирующих точек, идущих подряд
 *            (geos::operation::valid::RepeatedPointRemover);
 *          - исправление направления обхода (метод orient для кольца,
 *            orientRings для полигонов и мультиполигонов);
 *          - замыкание (для колец, полигонов и мультиполигонов, не geos).
 *          [!] Неочевидные результаты исправления геометрий:
 *              - если исправляется мультиполигон, состоящий из пересекающихся
 *                между собой полигонов, останутся только непересекающиеся
 *                части полигонов (области пересечений будут удалены);
 *              - если исправляется полигон, внутреннее кольцо которого
 *                выступает за пределы внешнего, то часть внутреннего кольца,
 *                оставшаяся внутри внешнего, будет удалена, а часть
 *                внутреннего кольца, оказавшаяся за пределами внешнего кольца,
 *                станет отдельным полигоном.
 * \param[in|out] inEntity - геометрия (сущность) для исправления;
 * \param[out] messages    - сообщение о найденных проблемах контура.
 * \return true  - геометрия (сущность) корректна;
 *         false - геометрия (сущность) не корректна и исправление не помогло.
 *         Пустую геометрию считаем корректной.
 */
template <typename InEntity>
bool correct(InEntity& inEntity, std::string& message)
{
  message.clear();

  // Замыкание колец
  Helper::closeRings(inEntity);

  std::unique_ptr<geos::geom::Geometry> geometry;
  std::unique_ptr<geos::geom::Geometry> correctedGeometry;
  try
  {
    geometry = PrimaryEntities::toGeosGeometry(inEntity);

    //! Проверка на пустоту
    if (geometry->isEmpty())
      return true; //!< Пустая геометрия - корректная геометрия

    // Исправление геометрии
    geos::operation::valid::MakeValid makeValid;
    std::unique_ptr<geos::geom::Geometry> validGeometry
                                             = makeValid.build(geometry.get());

    // Удаление дублируемых точек
    correctedGeometry
      = geos::operation::valid::RepeatedPointRemover::removeRepeatedPoints(
                                                          validGeometry.get());

    // Исправление направления обхода
    Helper::correctOrientation(correctedGeometry);
  }
  catch (const ExceptionTools::Failure& failure)
  {
    message = failure.what();
    return false;
  }
  catch (...)
  {
    // Если исправление геометрии не справилось, напишем об ошибке далее
    correctedGeometry = std::move(geometry);
  }

  //! Финальная проверка на корректность
  geos::operation::valid::IsValidOp isValid(correctedGeometry.get());
  bool retVal = isValid.isValid();

  if (!retVal)
  {
    // Если геометрия некорректна, запишем тип некорректности
    message = isValid.getValidationError()->getMessage();
  }
  else if (correctedGeometry->getGeometryTypeId()
                                              != geometry->getGeometryTypeId())
  {
    // Тип возвращаемого значения должен совпадать с типом исходной геометрии
    message
      = "Corrected geometry has type different from source geometry type";
    return false;
  }

  inEntity = PrimaryEntities::fromGeosGeometry<InEntity>(
                                                      correctedGeometry.get());

  return retVal;
}
//==============================================================================
/*!
 * \brief Исправление box на плоскости
 * \details Исправление координат минимальной и максимальной вершин.
 * \param[in|out] inEntity - коробка;
 * \param[out] messages    - сообщение о найденных проблемах коробки.
 * \return true  - геометрия (сущность) корректна;
 *         false - геометрия (сущность) не корректна и исправление не помогло
 *                 (такого быть не должно).
 */
template <typename PointType>
bool correct(PrimaryEntities::Box<PointType>& inEntity, std::string& message)
{
  message.clear();

  // geos всегда правильно задаёт минимальную и максимальную точку,
  // поэтому его средствами проверять box нет смысла
  std::unique_ptr<geos::geom::CoordinateSequence> minPoint
    = PrimaryEntities::toGeosGeometry(inEntity.min_corner())->getCoordinates();
  std::unique_ptr<geos::geom::CoordinateSequence> maxPoint
    = PrimaryEntities::toGeosGeometry(inEntity.max_corner())->getCoordinates();

  bool isCorrect = true;

  for (size_t i = 0; i < minPoint->getDimension(); ++i)
  {
    if (minPoint->getOrdinate(0, i) > maxPoint->getOrdinate(0, i))
    {
      isCorrect = false;

      auto tmpCoord = minPoint->getOrdinate(0, i);
      minPoint->setOrdinate(0, i, maxPoint->getOrdinate(0, i));
      maxPoint->setOrdinate(0, i, tmpCoord);
    }
  }

  // Если были найдены некорректные координаты, запишем исправленные
  if (!isCorrect)
  {
    geos::geom::GeometryFactory::Ptr geometryFactory
      = geos::geom::GeometryFactory::create();

    inEntity = PrimaryEntities::Box<PointType>(
      PrimaryEntities::fromGeosGeometry<PointType>(
        geometryFactory->createPoint(std::move(minPoint)).get()),
      PrimaryEntities::fromGeosGeometry<PointType>(
        geometryFactory->createPoint(std::move(maxPoint)).get()));
  }
  
  return true;
}
//==============================================================================
/*!
 * \brief Исправление геометрий (сущностей) на плоскости
 * \details Реализация на основе geos:
 *          - исправление самопересечений (geos::operation::valid::MakeValid);
 *          - исключение дублирующих точек, идущих подряд
 *            (geos::operation::valid::RepeatedPointRemover);
 *          - исправление направления обхода (метод orient для кольца,
 *            orientRings для полигонов и мультиполигонов);
 *          - замыкание (для колец, полигонов и мультиполигонов, не geos).
 * \param[in|out] inEntity - геометрия (сущность) для исправления;
 * \return true  - геометрия (сущность) корректна;
 *         false - геометрия (сущность) не корректна и исправление не помогло.
 *         Пустую геометрию считаем корректной.
 */
template <typename InEntity>
bool correct(InEntity& inEntity)
{
  std::string message;
  return correct( inEntity, message );
}
//==============================================================================
/*!
 * \brief Проверка корректности геометрии
 * \details Реализация на основе geos::operation::valid::IsValidOp.
 *          Дополнительно реализована проверка корретности направления
 *          обхода колец в геометриях.
 *          ! Пустая геометрия - корректная геометрия
 * \param[in] inEntity  - геометрия (сущность), проверяемая на корректность;
 * \param[out] messages - сообщение о найденных проблемах контура.
 * \return true  - геометрия (сущность) корректна;
 *         false - геометрия (сущность) не корректна.
*/
template <typename InEntity>
bool isCorrect(InEntity& inEntity, std::string& message)
{
  message.clear();

  std::unique_ptr<geos::geom::Geometry> geometry;

  try
  {
    // Частично проверки геометрий выполняются при конвертации
    // (замкнутость колец, ограничение на количество точек в линиях)
    geometry = PrimaryEntities::toGeosGeometry(inEntity);
  }
  catch (const ExceptionTools::Failure& failure)
  {
    message = failure.what();
    return false;
  }

  if(geometry->isEmpty())
    return true;  //!< пустая геометрия - корректная геометрия

  // Проверка корректности средствами geos
  geos::operation::valid::IsValidOp isValid(geometry.get());

  if(!isValid.isValid())
  {
    // Если геометрия некорректна, запишем тип некорректности
    message = isValid.getValidationError()->getMessage();
    return false;
  }

  // Проверка правильного направления обхода колец (для geos любое направление
  // считается корректным)
  if (!Helper::hasCorrectOrientation(geometry.get()))
  {
    message = "Wrong orientation";
    return false;
  }

  return true;
}
//==============================================================================
/*!
 * \brief Проверка корректности box
 * \details Проверка корректности координат минимальной и максимальной вершин.
 * \param[in] inEntity  - коробка;
 * \param[out] messages - сообщение о найденных проблемах коробки.
 * \return true  - геометрия (сущность) корректна;
 *         false - геометрия (сущность) не корректна.
*/
template <typename PointType>
bool isCorrect(PrimaryEntities::Box<PointType>& inEntity, std::string& message)
{
  message.clear();

  // geos всегда правильно задаёт минимальную и максимальную точку,
  // поэтому его средствами проверять box нет смысла
  std::unique_ptr<geos::geom::CoordinateSequence> minPoint
    = PrimaryEntities::toGeosGeometry(inEntity.min_corner())->getCoordinates();
  std::unique_ptr<geos::geom::CoordinateSequence> maxPoint
    = PrimaryEntities::toGeosGeometry(inEntity.max_corner())->getCoordinates();

  for (size_t i = 0; i < minPoint->getDimension(); ++i)
  {
    if (minPoint->getOrdinate(0, i) > maxPoint->getOrdinate(0, i))
    {
      message = "Wrong order";
      return false;
    }
  }

  return true;
}
//==============================================================================
/*!
 * \brief Проверка корректности геометрии
 * \details Реализация на основе geos::operation::valid::IsValidOp.
 *          Дополнительно реализована проверка корретности направления
 *          обхода колец в геометриях.
 *          ! Пустая геометрия - корректная геометрия
 * \param[in] inEntity - геометрия (сущность), проверяемая на корректность.
 * \return true  - геометрия (сущность) корректна;
 *         false - геометрия (сущность) не корректна.
*/
template <typename InEntity>
bool isCorrect(InEntity& inEntity)
{
  std::string message;
  return isCorrect( inEntity, message );
}
//==============================================================================
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_ALGORITHMS_CORRECT_HPP