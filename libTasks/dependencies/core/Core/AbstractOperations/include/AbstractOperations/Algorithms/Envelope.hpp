#ifndef ABSTRACT_OPERATIONS_ALGORITHMS_ENVELOPE_HPP
#define ABSTRACT_OPERATIONS_ALGORITHMS_ENVELOPE_HPP
//==============================================================================
#include <AbstractOperations/AbstractOperationsException.hpp>
#include <AbstractOperations/Algorithms/Correct.hpp>
#include <AbstractOperations/Algorithms/Helper/CheckAndNormalize.hpp>
#include <AbstractOperations/Normalize.hpp>
//! PrimaryEntities
#include <PrimaryEntities/AtEntitiesType.hpp>
#include <PrimaryEntities/GeometryConverter.hpp>
//==============================================================================
namespace AbstractOperations
{
//==============================================================================
/*!
 * \brief Cоздание минимального прямоугольника, стороны которого параллельны 
 *        координатным осям системы координат, и содержащего определённую 
 *        геометрию.
 * \details Реализация на основе метода getEnvelope геометрии geos
 *          Результирующая геометрия перед записью результата очищается.
 *          Основная геометрия перед рассчётом нормализуется.
 * \param[in] baseEntity    - основная геометрия (сущность),
 *                            основанная на GeoPoint2D или GeoPoint3D.
 * \param[out] outEntity    - результирующий прямоугольник (или параллелипипед)
 *                            в географической системе координат, стороны
 *                            которого параллельны координатным осям системы
 *                            координат. 
 * \param[in] autoNormalize - флаг необходимости нормализовать данные внутри 
 *                            функции (применяется для географических
 *                            координат)
 * \throws IrrelevantGeometryType
 */
template <typename BaseEntity, typename OutEntity>
void envelope(const BaseEntity& baseEntity, OutEntity& outEntity, 
              bool autoNormalize = true)
{
  //! признак, задана ли геометрия в геокоординатах
  using namespace PrimaryEntities;
  bool isBaseGeo = isEntityType1(baseEntity, GeodesicEntity);

  //! Проверка соответствия типов
  if (isBaseGeo != isEntityType1(outEntity, GeodesicEntity))
    throw AbstractOperationsFailure(IrrelevantGeometryType, "", "envelope");

  //! поиск описывающего четырёхугольника средствами geos
  BaseEntity normalized(baseEntity);
  try
  {
    if (isBaseGeo && autoNormalize 
        && isNeedToBeNormalized(baseEntity))
    {
      normalize(normalized, NORMAL);
      std::unique_ptr<geos::geom::Geometry> geosEnvelope
        = toGeosGeometry(normalized)->getEnvelope();
      outEntity = fromGeosGeometry<OutEntity>(geosEnvelope.get());
      normalize(outEntity, REGULAR);
    }
    else
    {
      std::unique_ptr<geos::geom::Geometry> geosEnvelope
        = toGeosGeometry(normalized)->getEnvelope();
      outEntity = fromGeosGeometry<OutEntity>(geosEnvelope.get());
    }
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>("envelope");
  }
}
//==============================================================================
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_ALGORITHMS_ENVELOPE_HPP