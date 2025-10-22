#ifndef ABSTRACT_OPERATIONS_ALGORITHMS_INTERSECTION_HPP
#define ABSTRACT_OPERATIONS_ALGORITHMS_INTERSECTION_HPP
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
 * \brief Пересечение двух геометрий (сущностей) на плоскости
 * \details Реализация на основе метода intersection геометрии geos
 *          Результирующая геометрия перед записью результата очищается.
 *          Работает только с площадными геометриями.
 * \param[in] leftEntity    - геометрия (сущность) с которой ищется
 *                            пересечение;
 * \param[in] rightEntity   - геометрия (сущность) которая пересекает;
 * \param[out] result       - результирующая геометрия (сущность);
 * \param[in] autoCorrect   - флаг автоматической коррекции:
 *                            true  - выполнять коррекцию, если она нужна,
 *                            false - не выполнять коррекцию в любом случае;
 * \param[in] autoNormalize - флаг автоматической нормализации:
 *                            true  - выполнять нормализацию, если она нужна,
 *                            false - не выполнять нормализацию в любом случае
 * \throws IncorrectGeometry || IrrelevantGeometryType
*/
template <typename LeftEntity, typename RightEntity, typename OutEntity>
void 
intersection(const LeftEntity& leftEntity, const RightEntity& rightEntity,
             OutEntity& result, bool autoCorrect = false, 
             bool autoNormalize = true)
{
  using namespace PrimaryEntities;
  //! intersection работает только с площадными геометриями
  if (!isEntityType1(leftEntity, ArealEntity)
    || !isEntityType1(rightEntity, ArealEntity))
    throw AbstractOperationsFailure(IrrelevantGeometryType,
                                    "", "intersection");

  LeftEntity entityLeft(leftEntity);
  RightEntity entityRight(rightEntity);

  //! Проверка и нормализация: REGULAR -> NORMAL
  bool needNormalize = Helper::checkAndNormalize(
                                          entityLeft, entityRight,
                                          isEntityType1(result, GeodesicEntity),
                                          autoNormalize, autoCorrect,
                                          "intersection");
   
  //! Инициализация результата
  OutEntity tmpResult;
  std::unique_ptr<geos::geom::Geometry> intersectionRes;

  //! Пересечение средствами geos
  try
  {
    intersectionRes = toGeosGeometry(entityLeft)->intersection(
                      toGeosGeometry(entityRight).get());
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>("intersection");
  }
    
  try
  {
    //! Если результатом пересечения является набор разных геометрий,
    //! учитываем только площадные
    if ((intersectionRes->getGeometryTypeId()
                                        == geos::geom::GEOS_GEOMETRYCOLLECTION)
      && ((typeid(OutEntity).hash_code() == typeid(MultiPolygon2D).hash_code())
        || (typeid(OutEntity).hash_code()
                                    == typeid(GeoMultiPolygon2D).hash_code())))
    {
      auto polygons = choosePolygonsFromCollection(
          static_cast<geos::geom::GeometryCollection*>(intersectionRes.get()));
      tmpResult = fromGeosGeometry<OutEntity>(polygons.get());
    }
    else
      tmpResult = fromGeosGeometry<OutEntity>(intersectionRes.get());
  }
  catch (const PrimaryEntities::PrimaryEntitiesFailure& ex)
  {
    //! Если геометрия нужного типа не найдена, возвращаем пустую геометрию
    if (ex.getCode() != PrimaryEntities::IncorrectGeometryType)
      ExceptionTools::FailureHandler<AbstractOperationsFailure>(
                                                              "intersection");
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>("intersection");
  }

  //! Нормализация: NORMAL -> REGULAR
  if (needNormalize)
    normalize(tmpResult, REGULAR);

  result = std::move(tmpResult);
}
//==============================================================================
/*!
 * \brief Анализ пересечения двух геометрий (сущностей) на плоскости
 * \details Реализация на основе метода intersects геометрии geos
 * \param[in] leftEntity    - геометрия (сущность);
 * \param[in] rightEntity   - геометрия (сущность);
 * \param[in] autoCorrect   - флаг автоматической коррекции:
 *                            true - выполнять коррекцию, если она нужна,
 *                            false - не выполнять коррекцию в любом случае;
 * \param[in] autoNormalize - флаг автоматической нормализации:
 *                            true - выполнять нормализацию, если она нужна,
 *                            false - не выполнять нормализацию в любом случае
 * \return true - пересечение есть;
 *         false - пересечения нет
 * \throws IncorrectGeometry || IrrelevantGeometryType
*/
template <typename LeftEntity, typename RightEntity>
bool hasIntersection(const LeftEntity& leftEntity,
                      const RightEntity& rightEntity, 
                      bool autoCorrect = false,
                      bool autoNormalize = true)
{
  LeftEntity entityLeft(leftEntity);
  RightEntity entityRight(rightEntity);

  //! Проверка и нормализация: REGULAR -> NORMAL
  using namespace PrimaryEntities;
  bool needNormalize = Helper::checkAndNormalize(
                                      entityLeft, entityRight,
                                      isEntityType1(entityLeft, GeodesicEntity),
                                      autoNormalize, autoCorrect,
                                      "hasIntersection");

  //! Анализ наличия пересечения
  try
  {
    return toGeosGeometry(entityLeft)->intersects(
                                            toGeosGeometry(entityRight).get());
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>(
                                                            "hasIntersection");
  }

  return false;
}
//==============================================================================
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_ALGORITHMS_INTERSECTION_HPP