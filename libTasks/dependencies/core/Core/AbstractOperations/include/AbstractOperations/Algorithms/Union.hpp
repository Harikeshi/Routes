#ifndef ABSTRACT_OPERATIONS_ALGORITHMS_UNION_HPP
#define ABSTRACT_OPERATIONS_ALGORITHMS_UNION_HPP
//==============================================================================
#include <AbstractOperations/AbstractOperationsException.hpp>
#include <AbstractOperations/Algorithms/Helper/CheckAndNormalize.hpp>
//!  PrimaryEntities
#include <PrimaryEntities/AtEntitiesType.hpp>
#include <PrimaryEntities/GeometryConverter.hpp>
//! Geos
#include <geos/simplify/DouglasPeuckerSimplifier.h>
//==============================================================================
namespace AbstractOperations
{
//==============================================================================
/*!
 * \brief Объединение геометрий (сущностей) на плоскости
 * \details Реализация на основе метода Union геометрии geos
 *          Результирующая геометрия перед записью результата очищается.
 *          Работает только с площадными геометриями.
 * \param[in] entityFirst   - первая геометрия (сущность) для объединения;
 * \param[in] entitySecond  - вторая геометрия (сущность) для объединения;
 * \param[out] result       - результирующая геометрия (сущность);
 * \param[in] autoCorrect   - флаг автоматической коррекции:
 *                            true - выполнять коррекцию, если она нужна,
 *                            false - не выполнять коррекцию в любом случае;
 * \param[in] autoNormalize - флаг автоматической нормализации:
 *                            true - выполнять нормализацию, если она нужна,
 *                            false - не выполнять нормализацию в любом случае
 * \throws IncorrectGeometry || IrrelevantGeometryType
 */
template <typename InEntityFirst, typename InEntitySecond, typename OutEntity>
void 
unionOf(const InEntityFirst& entityFirst, const InEntitySecond& entitySecond,
        OutEntity& result, bool autoCorrect = false,
        bool autoNormalize = true)
{
  using namespace PrimaryEntities;
  //! unionOf работает только с площадными геометриями
  if (!isEntityType1(entityFirst, ArealEntity)
    || !isEntityType1(entitySecond, ArealEntity))
    throw AbstractOperationsFailure(IrrelevantGeometryType, "", "unionOf");

  InEntityFirst entityLeft(entityFirst);
  InEntitySecond entityRight(entitySecond);

  //! Проверка и нормализация: REGULAR -> NORMAL
  bool needNormalize = Helper::checkAndNormalize(
                                          entityLeft, entityRight,
                                          isEntityType1(result, GeodesicEntity),
                                          autoNormalize, autoCorrect,
                                          "unionOf");

  //! Инициализация результата
  OutEntity tmpResult;
  std::unique_ptr<geos::geom::Geometry> unionRes;

  //! Объединение средствами geos
  try
  {
    auto geosLeft(toGeosGeometry(entityLeft));
    auto geosRight(toGeosGeometry(entityRight));

    //! Если одна из геометрий пуста, пропускаем её, иначе при
    //! объединении она может быть добавлена к объединённой геометрии
    if (geosLeft->isEmpty())
      unionRes = std::move(geosRight);
    else if (geosRight->isEmpty())
      unionRes = std::move(geosLeft);
    else
      unionRes = geosLeft->Union(geosRight.get());
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>("unionOf");
  }

  try
  {
    tmpResult = fromGeosGeometry<OutEntity>(unionRes.get());
  }
  catch (const PrimaryEntities::PrimaryEntitiesFailure& ex)
  {
    if (ex.getCode() != PrimaryEntities::IncorrectGeometryType)
      ExceptionTools::FailureHandler<AbstractOperationsFailure>("unionOf");
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>("unionOf");
  }

  //! Нормализация: NORMAL -> REGULAR
  if (needNormalize)
    normalize(tmpResult, REGULAR);
  result = std::move(tmpResult);
}
//==============================================================================
/*!
 * \brief Объединение геометрий (сущностей) на плоскости внутри коллекции
 * \details Реализация на основе метода Union геометрии geos
 *          Может использоваться только для площадных коллекций на плоскости
 * \param[in|out] inEntity  - коллекция геометрий (сущностей) для объединения;
 * \param[in] autoCorrect   - флаг автоматической коррекции:
 *                            true - выполнять коррекцию, если она нужна,
 *                            false - не выполнять коррекцию в любом случае;
 * \param[in] autoNormalize - флаг автоматической нормализации:
 *                            true - выполнять нормализацию, если она нужна,
 *                            false - не выполнять нормализацию в любом случае
 * \throws InvalidGeometryType
*/
template <typename InEntity>
void unionInternal(InEntity& inEntity, bool autoCorrect = false,
                   bool autoNormalize = true)
{
  //! Проверка на допустимость типа геометрии в операции
  using namespace PrimaryEntities;
  if (!isEntityType1(inEntity, ArealEntity | CollectionEntity | Dim2DEntity))
    throw AbstractOperationsFailure(InvalidGeometryType, "", "unionInternal");

  InEntity entity(inEntity);

  //! Оценка необходимости нормализации
  bool needNormalize = isEntityType1(entity, GeodesicEntity) && autoNormalize;

  //! Нормализация: REGULAR -> NORMAL
  if (needNormalize)
  {
    if (isNeedToBeNormalized(entity))
      normalize(entity, NORMAL);
    else
      needNormalize = false;
  }

  //! Инициализация переменной для расчёта результата
  InEntity result;

  //! Объединение 
  try
  {
    for (auto& polygon : entity)
      unionOf(result, polygon, result, autoCorrect, autoNormalize);

    //! Нормализация: NORMAL -> REGULAR
    if (needNormalize)
      normalize(result, REGULAR);

    inEntity = std::move(result);
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>("unionInternal");
  }
}
//==============================================================================
/*!
 * \brief Объединение полигонов в мультиполигоне на месте
 * \details Реализация на основе метода Union геометрии geos
 *          Внутри только одиночная проверка корректности каждого полигона,
 *          без контроля корректности каждого последовательного объединения
 *          (вместо unionOf используется чистый Union).
 * \param[in|out] mpolygon  - мультиполигон;
 * \param[in] autoCorrect   - флаг автоматической коррекции:
 *                            true - выполнять коррекцию, если она нужна,
 *                            false - не выполнять коррекцию в любом случае;
 * \param[in] autoNormalize - флаг автоматической нормализации:
 *                            true - выполнять нормализацию, если она нужна,
 *                            false - не выполнять нормализацию в любом случае
 * \throws IncorrectGeometry
*/
template <typename PointType>
void unionAll(PrimaryEntities::MultiPolygon<PointType>& mpolygon,
              bool autoCorrect = false, bool autoNormalize = true)
{
  //! Если мультиполигон пуст, то он пуст
  if (mpolygon.empty() || mpolygon.at(0).outer().empty())
    return;

  //! Основной функционал
  try
  {
    //! Нормализация: REGULAR -> NORMAL
    bool isGeo = isEntityType1(mpolygon, PrimaryEntities::GeodesicEntity);

    bool needNormalize = isGeo && autoNormalize;
    if (needNormalize)
    {
      if (isNeedToBeNormalized(mpolygon))
        normalize(mpolygon, NORMAL);
      else
        needNormalize = false;
    }

    //! Инициализация результата
    PrimaryEntities::MultiPolygon<PointType> result;
    std::unique_ptr<geos::geom::Geometry> unionRes(
      PrimaryEntities::toGeosGeometry(result));

    //! Проверка входных геометрий на корректность и объединение
    for (auto& polygon : mpolygon)
    {
      auto geosPoly(PrimaryEntities::toGeosGeometry(polygon));

      //! Пустые полигоны пропускаем
      if (geosPoly->isEmpty())
        continue;

      //! Корректность 
      std::string message;
      if (autoCorrect)
      {
        if (!correct(polygon, message))
          throw AbstractOperationsFailure(IncorrectGeometry, message,
                                          "unionAll");
      }
      else
      {
        if (!isCorrect(polygon, message))
          throw AbstractOperationsFailure(IncorrectGeometry, message,
                                          "unionAll");
      }

      //! Объединение
      unionRes = geosPoly->Union(unionRes.get());
    }

    result = PrimaryEntities::fromGeosGeometry<
      PrimaryEntities::MultiPolygon<PointType>>(unionRes.get());

    //! Нормализация: NORMAL -> REGULAR
    if (needNormalize)
      normalize(result, REGULAR);

    //! Перезаписываем исходный полигон
    mpolygon = std::move(result);
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>("unionAll");
  }
}
//==============================================================================
/*!
 * \brief Объединение коллекции полигонов
 * \details Реализация на основе unionAll
 * \param[in] polygons - коллекция полигонов;
 * \param[in] autoCorrect - флаг автоматической коррекции:
 *                          true - выполнять коррекцию, если она нужна,
 *                          false - не выполнять коррекцию в любом случае;
 * \param[in] autoNormalize - флаг автоматической нормализации:
 *                            true - выполнять нормализацию, если она нужна,
 *                            false - не выполнять нормализацию в любом случае
 * \return Результирующий мальтиполигон
*/
template <typename PointType>
PrimaryEntities::MultiPolygon<PointType>
unionPolygons(
  const std::vector<PrimaryEntities::Polygon<PointType>>& polygons,
  bool autoCorrect = false, bool autoNormalize = true)
{
  PrimaryEntities::MultiPolygon<PointType> result;

  //! Если набор пуст, то и результат пуст
  if (polygons.empty())
    return result;

  //! Создание копии
  for (const auto& polygon : polygons)
    result.push_back(polygon);

  //! Вызываем объединение для мультиполигона
  try
  {
    unionAll(result, autoCorrect, autoNormalize);
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>("unionPolygons");
  }

  return result;
}
//==============================================================================
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_ALGORITHMS_UNION_HPP
