#ifndef ABSTRACT_OPERATIONS_ALGORITHMS_COVERED_BY_HPP
#define ABSTRACT_OPERATIONS_ALGORITHMS_COVERED_BY_HPP
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
 * \brief Проверка вхождения одной геометрии (сущности) в другую с возможным 
 *        захватом границы
 * \details Реализация на основе метода coveredBy геометрии geos
 * \param[in] entityFirst   - проверяемая геометрия;
 * \param[in] entitySecond  - геометрия, на вхождение в которую 
 *                            осуществляется проверка;
 * \param[in] autoCorrect   - флаг автоматической коррекции:
 *                            true - выполнять коррекцию, если она нужна,
 *                            false - не выполнять коррекцию в любом случае;
 * \param[in] autoNormalize - флаг автоматической нормализации:
 *                            true - выполнять нормализацию, если она нужна,
 *                            false - не выполнять нормализацию в любом случае.
 * \return true  - первая геометрия входит во внутреннюю часть второй
 *                 или лежит на ее границе;
 *         false - первая геометрия хотя бы частично не входит во вторую
 * \throws IncorrectGeometry || IrrelevantGeometryType
 */
template <typename InEntityFirst, typename InEntitySecond>
bool 
coveredBy(
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
                            autoNormalize, autoCorrect, "covered by");

  //! Проверка вхождения средствами geos
  try
  {
    return toGeosGeometry(entityLeft)->coveredBy(
      toGeosGeometry(entityRight).get());
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>("covered by");
  }

  return false;
}
//==============================================================================
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_ALGORITHMS_COVERED_BY_HPP