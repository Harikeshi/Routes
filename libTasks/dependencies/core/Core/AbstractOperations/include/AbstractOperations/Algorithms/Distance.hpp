#ifndef ABSTRACT_OPERATIONS_ALGORITHMS_DISTANCE_HPP
#define ABSTRACT_OPERATIONS_ALGORITHMS_DISTANCE_HPP
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
 * \brief Вычисление расстояния между геометриями
 * \details Реализация на основе метода distance геометрии geos
 * \param[in] entityFirst   - первая геометрия (сущность);
 * \param[in] entitySecond  - вторая геометрия (сущность);
 * \param[in] autoCorrect   - флаг автоматической коррекции:
 *                            true - выполнять коррекцию, если она нужна,
 *                            false - не выполнять коррекцию в любом случае;
 * \param[in] autoNormalize - флаг автоматической нормализации:
 *                            true - выполнять нормализацию, если она нужна,
 *                            false - не выполнять нормализацию в любом случае
 * \return Расстояние между двумя геометриями.
 * \throws IncorrectGeometry || IrrelevantGeometryType
 */
template <typename InEntityFirst, typename InEntitySecond>
double 
distanceBetweenGeometries(
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
                            autoNormalize, autoCorrect,
                            "distanceBetweenGeometries");

  //! Вычисление расстояния средствами geos
  try
  {
    return toGeosGeometry(entityLeft)->distance(
      toGeosGeometry(entityRight).get());
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>(
                                                  "distanceBetweenGeometries");
  }
}
//==============================================================================
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_ALGORITHMS_DISTANCE_HPP