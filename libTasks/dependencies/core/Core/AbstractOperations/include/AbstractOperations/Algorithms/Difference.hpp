#ifndef ABSTRACT_OPERATIONS_ALGORITHMS_DIFFERENCE_HPP
#define ABSTRACT_OPERATIONS_ALGORITHMS_DIFFERENCE_HPP
//==============================================================================
#include <AbstractOperations/AbstractOperationsException.hpp>
#include <AbstractOperations/Algorithms/Helper/CheckAndNormalize.hpp>
//! PrimaryEntities
#include <PrimaryEntities/AtEntitiesType.hpp>
#include <PrimaryEntities/GeometryConverter.hpp>
//==============================================================================
namespace AbstractOperations
{
//==============================================================================
/*!
 * \brief Исключение (Вычитание) одной геометрии (сущности) из другой на 
 *        плоскости
 * \details Реализация на основе метода difference геометрии geos
 *          Результирующая геометрия перед записью результата очищается.
 *          Работает только с площадными геометриями.
 *          ! Не работает для вычитания box из линейных геометрий (сущностей)
 * \param[in] baseEntity    - основная геометрия (сущность);
 * \param[in] difEntity     - вычитаемая геометрия (сущность);
 * \param[out] result       - результирующая геометрия (сущность);
 * \param[in] autoCorrect   - флаг автоматической коррекции:
 *                            true - выполнять коррекцию, если она нужна,
 *                            false - не выполнять коррекцию в любом случае;
 * \param[in] autoNormalize - флаг автоматической нормализации:
 *                            true - выполнять нормализацию, если она нужна,
 *                            false - не выполнять нормализацию в любом случае
 * \throws IncorrectGeometry || IrrelevantGeometryType
*/
template <typename BaseEntity, typename DifEntity, typename OutEntity>
void 
difference(const BaseEntity& baseEntity, const DifEntity& difEntity,
           OutEntity& result, bool autoCorrect = false,
           bool autoNormalize = true)
{
  using namespace PrimaryEntities;
  //! difference работает только с площадными геометриями
  if (!isEntityType1(baseEntity, ArealEntity)
    || !isEntityType1(difEntity, ArealEntity))
    throw AbstractOperationsFailure(IrrelevantGeometryType, "", "difference");

  BaseEntity entityLeft(baseEntity);
  DifEntity entityRight(difEntity);

  //! Проверка и нормализация: REGULAR -> NORMAL
  bool needNormalize = Helper::checkAndNormalize(
                                          entityLeft, entityRight,
                                          isEntityType1(result, GeodesicEntity),
                                          autoNormalize, autoCorrect,
                                          "difference");

  //! Инициализация результата
  OutEntity tmpResult;
  std::unique_ptr<geos::geom::Geometry> diffRes;

  //! Объединение средствами geos
  try
  {
    diffRes = PrimaryEntities::toGeosGeometry(entityLeft)->difference(
               PrimaryEntities::toGeosGeometry(entityRight).get());
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>("difference");
  }

  try
  {
    //! Если результатом пересечения является набор разных геометрий,
    //! учитываем только площадные
    if ((diffRes->getGeometryTypeId() == geos::geom::GEOS_GEOMETRYCOLLECTION)
      && ((typeid(OutEntity).hash_code()
                                    == typeid(MultiPolygon2D).hash_code())
        || (typeid(OutEntity).hash_code()
                                    == typeid(GeoMultiPolygon2D).hash_code())))
    {
      auto polygons = choosePolygonsFromCollection(
                  static_cast<geos::geom::GeometryCollection*>(diffRes.get()));
      tmpResult = fromGeosGeometry<OutEntity>(polygons.get());
    }
    else
      tmpResult = fromGeosGeometry<OutEntity>(diffRes.get());
  }
  catch (const PrimaryEntities::PrimaryEntitiesFailure& ex)
  {
    //! Если геометрия нужного типа не найдена, возвращаем пустую геометрию
    if (ex.getCode() != PrimaryEntities::IncorrectGeometryType)
      ExceptionTools::FailureHandler<AbstractOperationsFailure>("difference");
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>("difference");
  }

    //! Нормализация: NORMAL -> REGULAR
    if (needNormalize)
      normalize(tmpResult, REGULAR);

    result = std::move(tmpResult);
}
//==============================================================================
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_ALGORITHMS_DIFFERENCE_HPP