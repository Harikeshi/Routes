#ifndef ABSTRACT_OPERATIONS_ALGORITHMS_AREA_HPP
#define ABSTRACT_OPERATIONS_ALGORITHMS_AREA_HPP
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
 * \brief Вычисление площади геометрии
 * \details Реализация на основе метода getArea геометрии geos.
 *          Вычисляет площадь в единицах измерения входной геометрии.
 * \param[in] entityFirst   - геометрия (сущность);
 * \param[in] autoCorrect   - флаг автоматической коррекции:
 *                            true - выполнять коррекцию, если она нужна,
 *                            false - не выполнять коррекцию в любом случае;
 * \param[in] autoNormalize - флаг автоматической нормализации:
 *                            true - выполнять нормализацию, если она нужна,
 *                            false - не выполнять нормализацию в любом случае
 * \return площадь геометрии.
 * \throws IncorrectGeometry || IrrelevantGeometryType
 */
template <typename InEntity>
double 
area(
  const InEntity& inEntity,
  bool autoCorrect = false,
  bool autoNormalize = true)
{
  InEntity entity(inEntity);

  //! Проверка и нормализация: REGULAR -> NORMAL
  using namespace PrimaryEntities;
  Helper::checkAndNormalize(entity,
                            isEntityType1(entity, GeodesicEntity),
                            autoNormalize, autoCorrect, "area");

  //! Вычисление площади средствами geos
  double resArea;
  try
  {
    resArea = toGeosGeometry(entity)->getArea();
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>("area");
  }

  return resArea;
}
//==============================================================================
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_ALGORITHMS_AREA_HPP