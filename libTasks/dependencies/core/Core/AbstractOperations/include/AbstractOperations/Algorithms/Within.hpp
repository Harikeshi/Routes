#ifndef ABSTRACT_OPERATIONS_ALGORITHMS_WITHIN_HPP
#define ABSTRACT_OPERATIONS_ALGORITHMS_WITHIN_HPP
//==============================================================================
#include <AbstractOperations/AbstractOperationsException.hpp>
#include <AbstractOperations/Algorithms/Helper/CheckAndNormalize.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryConverter.hpp>
//==============================================================================
namespace AbstractOperations
{
//==============================================================================
/*!
 * \brief Проверка вхождения одной геометрии (сущности) в другую полностью на 
 *        плоскости
 * \details Реализация на основе метода within геометрии geos
 * \param[in] entityFirst   - проверяемая геометрия (сущность);
 * \param[in] entitySecond  - геометрия (сущность), на вхождение в которую 
 *                            осуществляется проверка;
 * \param[in] autoCorrect   - флаг автоматической коррекции:
 *                            true - выполнять коррекцию, если она нужна,
 *                            false - не выполнять коррекцию в любом случае;
 * \param[in] autoNormalize - флаг автоматической нормализации:
 *                            true - выполнять нормализацию, если она нужна,
 *                            false - не выполнять нормализацию в любом случае
 * \return true  - первая геометрия (сущности) полностью входит во вторую;
 *         false - первая геометрия (сущности) не входит полностью во вторую
 * \throws IncorrectGeometry || IrrelevantGeometryType
 */
template <typename InEntityFirst, typename InEntitySecond>
bool 
within(
  const InEntityFirst& entityFirst,
  const InEntitySecond& entitySecond,
  bool autoCorrect = false,
  bool autoNormalize = true)
{
  InEntityFirst entityLeft(entityFirst);
  InEntitySecond entityRight(entitySecond);

  //! Проверка и нормализация: REGULAR -> NORMAL
  using namespace PrimaryEntities;
  Helper::checkAndNormalize(entityLeft, entityRight,
                            isEntityType1(entityLeft, GeodesicEntity),
                            autoNormalize, autoCorrect, "within");

  //! Проверка вхождения средствами geos
  try
  {
    return toGeosGeometry(entityLeft)->within(
      toGeosGeometry(entityRight).get());
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>("within");
  }

  return false;
}
//==============================================================================
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_ALGORITHMS_WITHIN_HPP