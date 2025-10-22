#ifndef ABSTRACT_OPERATIONS_ALGORITHMS_SIMPLIFY_HPP
#define ABSTRACT_OPERATIONS_ALGORITHMS_SIMPLIFY_HPP
//==============================================================================
#include <AbstractOperations/AbstractOperationsException.hpp>
#include <AbstractOperations/Algorithms/Helper/CheckAndNormalize.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryConverter.hpp>
//! Geos
#include <geos/simplify/DouglasPeuckerSimplifier.h>
//==============================================================================
namespace AbstractOperations
{
//==============================================================================
/*!
 * \brief Упрощение геометрии
 * \details Реализация на основе geos::simplify::DouglasPeuckerSimplifier
 * \param[in] inEntity      - упрощаемая геометрия;
 * \param[out] outEntity    - результат упрощения;
 * \param[in] maxDistance   - максимальное отклонение точки от исследуемого
 *                            отрезка, при котором ее разрешено удалить.
 *                            По умолчанию разрешено удалять только точки,
 *                            лежащие непосредственно на отрезке;
 * \param[in] autoCorrect   - флаг автоматической коррекции:
 *                            true - выполнять коррекцию, если она нужна,
 *                            false - не выполнять коррекцию в любом случае;
 * \param[in] autoNormalize - флаг автоматической нормализации:
 *                            true - выполнять нормализацию, если она нужна,
 *                            false - не выполнять нормализацию в любом случае
 * \throws IncorrectGeometry || IrrelevantGeometryType
 *                           || OperationNotPerformedCorrect
 */
template <typename Geometry>
void simplify( const Geometry& inEntity, Geometry& outEntity,
               const double maxDistance = 0,
               bool autoCorrect = false, bool autoNormalize = true )
{
  //! Очищаем переменную-результат.
  outEntity.clear();

  Geometry entity(inEntity);
  std::unique_ptr<geos::geom::Geometry> simplifyRes;

  //! Проверка и нормализация: REGULAR -> NORMAL
  using namespace PrimaryEntities;
  bool needNormalize = Helper::checkAndNormalize(
                                       entity,
                                       isEntityType1(outEntity, GeodesicEntity),
                                       autoNormalize, autoCorrect,
                                       "simplify");

  try
  {
    simplifyRes = geos::simplify::DouglasPeuckerSimplifier::simplify(
      toGeosGeometry(entity).get(), maxDistance);
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>("simplify");
  }

  try
  {
    outEntity = fromGeosGeometry<Geometry>(simplifyRes.get());
  }
  catch (const PrimaryEntities::PrimaryEntitiesFailure& ex)
  {
    // Если получили геометрию, не соответствующую ожидаемой,
    // возвращаем пустую геометрию
    if (ex.getCode() != PrimaryEntities::IncorrectGeometryType)
      ExceptionTools::FailureHandler<AbstractOperationsFailure>("simplify");
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>("simplify");
  }

  //! Проверка корректности результата
  std::string message;
  if ( ! isCorrect(outEntity, message) )
    throw AbstractOperationsFailure(
      OperationNotPerformedCorrect, message, "simplify:");

  //! Нормализация: NORMAL -> REGULAR
  if (needNormalize)
    normalize(outEntity, REGULAR);
};
//==============================================================================
/*!
 * \brief Упрощение геометрии
 * \details Реализация на основе geos::simplify::DouglasPeuckerSimplifier
 * \param[in|out] entity    - упрощаемая геометрия;
 * \param[in] maxDistance   - максимальное отклонение точки от исследуемого
 *                            отрезка, при котором ее разрешено удалить.
 *                            По умолчанию разрешено удалять только точки,
 *                            лежащие непосредственно на отрезке;
 * \param[in] autoCorrect   - флаг автоматической коррекции:
 *                            true - выполнять коррекцию, если она нужна,
 *                            false - не выполнять коррекцию в любом случае;
 * \param[in] autoNormalize - флаг автоматической нормализации:
 *                            true - выполнять нормализацию, если она нужна,
 *                            false - не выполнять нормализацию в любом случае
 */
template <typename Geometry>
void simplify( Geometry& entity, 
               const double maxDistance = 0,
               bool autoCorrect = false, bool autoNormalize = true )
{
  Geometry tmp;
  simplify( entity, tmp, maxDistance, autoCorrect, autoNormalize );
  entity = tmp;
};
//==============================================================================
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_ALGORITHMS_SIMPLIFY_HPP