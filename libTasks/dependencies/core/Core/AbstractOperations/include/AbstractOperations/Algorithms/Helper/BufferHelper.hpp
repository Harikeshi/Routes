#ifndef ABSTRACT_OPERATIONS_ALGORITHMS_HELPER_BUFFER_HELPER_HPP
#define ABSTRACT_OPERATIONS_ALGORITHMS_HELPER_BUFFER_HELPER_HPP
//==============================================================================
#include <AbstractOperations/AbstractOperationsException.hpp>
#include <AbstractOperations/Algorithms/Helper/CheckAndNormalize.hpp>
#include <AbstractOperations/Algorithms/Union.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! PrimaryEntities
#include <PrimaryEntities/AtEntitiesType.hpp>
#include <PrimaryEntities/GeometryConverter.hpp>
//! Geos
#include <geos/operation/buffer/BufferOp.h>
#include <geos/simplify/DouglasPeuckerSimplifier.h>
//==============================================================================
namespace AbstractOperations
{
namespace Helper
{
//==============================================================================
  /*!
 * \brief Буфер для геометрии сущности
 * \details Реализация на основе geos::operation::buffer.
 *          Данный буфер с боксом не работает.
 * \param[in] inEntity         - геометрия (сущность), для которой строится
 *                               буфер;
 * \param[out] outEntity       - мультиполигон корректных полигонов,
 *                               соответствующий исходному;
 * \param[in] distance         - размер буфера;
 * \param[in] quadrantSegments - количество сегментов для разбиения четверти
 *                               квадрата (для скругления буфера);
 * \param[in] endCapStyle      - тип формирования точек буфера на границах
 *                               линий;
 * \param[in] joinStyle        - тип формирования промежуточных точек линий;
 * \param[in] mitreLimit       - максимальное расстояние от промежуточных
 *                               вершин линии до границы буфера
 *                              (используется, чтобы не допустить вытягивания
 *                               острых углов при надувании геометрии, слишком
 *                               вытянутые углы будут срезаны);
 * \param[in] autoCorrect      - флаг автоматической коррекции:
 *                               true  - выполнять коррекцию, если она нужна,
 *                               false - не выполнять коррекцию в любом случае
 * \param[in] autoNormalize    - флаг автоматической нормализации:
 *                               true  - выполнять нормализацию, если она
 *                                       нужна,
 *                               false - не выполнять нормализацию в любом
 *                                       случае
 * \throws IncorrectGeometry || IrrelevantGeometryType
                             || OperationNotPerformedCorrect
 */
  template <typename InEntity, typename OutEntity>
  void buffer(const InEntity& inEntity, OutEntity& outEntity,
    double distance, int quadrantSegments,
    geos::operation::buffer::BufferParameters::EndCapStyle endCapStyle,
    geos::operation::buffer::BufferParameters::JoinStyle joinStyle,
    double mitreLimit,
    bool autoCorrect = false, bool autoNormalize = true)
  {
    //! Очищаем переменную-результат.
    outEntity.clear();

    //!< Код типа исходной геометрии.
    size_t inEntityType = typeid(inEntity).hash_code();

    //! buffer не работает c box
    if ((inEntityType == typeid(Box2D).hash_code())
      || (inEntityType == typeid(Box3D).hash_code())
      || (inEntityType == typeid(GeoBox2D).hash_code()))
      throw AbstractOperationsFailure(IrrelevantGeometryType, "", "buffer:");

    InEntity entity(inEntity);

    //! Проверка и нормализация: REGULAR -> NORMAL
    using namespace PrimaryEntities;
    bool needNormalize = Helper::checkAndNormalize(entity,
      isEntityType1(outEntity,
        GeodesicEntity),
      autoNormalize, autoCorrect,
      "buffer");
    //TODO: возможно, следует отдельно реализовывать отрицательный буфер через 
    // надувание противоположной фигуры как более стабильную операцию

    try
    {
      // Задаём параметры буфера
      geos::operation::buffer::BufferParameters bufferParams(
        quadrantSegments, endCapStyle, joinStyle, mitreLimit);

      // Буфер
      std::unique_ptr<geos::geom::Geometry> bufferRes
        = geos::operation::buffer::BufferOp::bufferOp(
          PrimaryEntities::toGeosGeometry(entity).get(), distance, bufferParams);

      //! Удаление лишних вершин, образовавшихся после вычисления.
      std::unique_ptr<geos::geom::Geometry> simplifiedRes
        = geos::simplify::DouglasPeuckerSimplifier::simplify(
          bufferRes.get(), ACCURACY_DOUBLE);

      // Конвертация геометрии из geos
      outEntity = fromGeosGeometry<OutEntity>(simplifiedRes.get());

      //! объединение полученных геометрий
      unionInternal(outEntity, true, autoNormalize);
    }
    catch (...)
    {
      ExceptionTools::FailureHandler<AbstractOperationsFailure>("buffer");
    }

    // TODO Возможно, следует дополнительно проверять минимальную вменяемость результата:
    // положительный буфер не может уменьшить фигуру, а отрицательный - увеличить ее

    //! Проверка корректности результата
    std::string message;
    if (!isCorrect(outEntity, message))
      throw AbstractOperationsFailure(
        OperationNotPerformedCorrect, message, "buffer:");

    //! Нормализация: NORMAL -> REGULAR
    if (needNormalize)
      AbstractOperations::normalize(outEntity, REGULAR);
  };
//==============================================================================
} //! Helper 
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_ALGORITHMS_HELPER_BUFFER_HELPER_HPP