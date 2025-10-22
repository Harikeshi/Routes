#ifndef ABSTRACT_OPERATIONS_ALGORITHMS_BUFFER_HPP
#define ABSTRACT_OPERATIONS_ALGORITHMS_BUFFER_HPP
//==============================================================================
#include <AbstractOperations/Algorithms/Helper/BufferHelper.hpp>
//==============================================================================
namespace AbstractOperations
{
//==============================================================================
/*!
 * \brief Угловатый буфер для геометрии сущности
 * \details Реализация на основе geos::operation::buffer.
 *          Используются следующие параметры:
 *          - квадратные границы линии (CAP_SQUARE);
 *          - объединение линий острыми углами (JOIN_MITRE);
 *          - срез углов, выступающих от исходной геометрии более чем на
*             значение DEFAULT_MITRE_LIMIT = 5.
 *          Данный буфер с боксом не работает.
 * \param[in] inEntity      - геометрия (сущность), для которой строится буфер;
 * \param[out] outEntity    - мультиполигон корректных полигонов,
 *                            соответствующий исходному;
 * \param[in] distance      - размер буфера;
 * \param[in] autoCorrect   - флаг автоматической коррекции:
 *                            true - выполнять коррекцию, если она нужна,
 *                            false - не выполнять коррекцию в любом случае;
 * \param[in] autoNormalize - флаг автоматической нормализации:
 *                            true - выполнять нормализацию, если она нужна,
 *                            false - не выполнять нормализацию в любом случае
*/
template <typename InEntity, typename OutEntity>
void bufferSquare(const InEntity& inEntity, OutEntity& outEntity, 
                  const double distance,
                  bool autoCorrect = false, bool autoNormalize = true)
{
  //! Вычисляем буфер
  try
  {
    return Helper::buffer(
      inEntity, outEntity, distance,
      geos::operation::buffer::BufferParameters::DEFAULT_QUADRANT_SEGMENTS,
      geos::operation::buffer::BufferParameters::CAP_SQUARE,
      geos::operation::buffer::BufferParameters::JOIN_MITRE,
      5., // значение по-умолчанию, взятое из geos (DEFAULT_MITRE_LIMIT)
      autoCorrect, autoNormalize);
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>("bufferSquare");
  }
}
//==============================================================================
/*!
 * \brief Угловатый буфер "на месте" для площадных геометрий
 * \details Реализация на основе geos::operation::buffer.
 *          Используются следующие параметры:
 *          - квадратные границы линии (CAP_SQUARE);
 *          - объединение линий острыми углами (JOIN_MITRE);
 *          - срез углов, выступающих от исходной геометрии более чем на
*             значение DEFAULT_MITRE_LIMIT = 5.
 *          Функция применима только для площадных геометрий. В других случаях 
 *          генерируется исключение.
 * \param[in|out] geometry  - геометрия (сущность), для которой строится буфер;
 * \param[in] distance      - размер буфера;
 * \param[in] autoCorrect   - флаг автоматической коррекции:
 *                            true - выполнять коррекцию, если она нужна,
 *                            false - не выполнять коррекцию в любом случае;
 * \param[in] autoNormalize - флаг автоматической нормализации:
 *                            true - выполнять нормализацию, если она нужна,
 *                            false - не выполнять нормализацию в любом случае
 * \throws AbstractOperationsFailure : InvalidGeometryType || LossOfData
*/
template <typename Geometry>
void bufferSquare( Geometry& geometry,
  const double distance,
  bool autoCorrect = false, bool autoNormalize = true )
{
  throw AbstractOperationsFailure( InvalidGeometryType );
}
//==============================================================================
template <typename Point>
void bufferSquare( PrimaryEntities::Ring<Point>& geometry,
  const double distance,
  bool autoCorrect = false, bool autoNormalize = true )
{
  PrimaryEntities::MultiPolygon<Point> mult;

  // расширяем кольцо до полигона, чтобы буфер отработал корректно
  PrimaryEntities::Polygon<Point> polygon;
  polygon.outer() = geometry;
  bufferSquare( polygon, mult, distance, autoCorrect, autoNormalize );

  if( !mult.size( ) )
  {
    geometry = PrimaryEntities::Ring<Point>( );
    return;
  }

  if( ( mult.size( ) > 1 ) || ( mult[0].inners( ).size( ) ) )
    throw ( AbstractOperationsFailure( LossOfData ) );

  geometry = mult.at( 0 ).outer( );
}
//==============================================================================
template <typename Point>
void bufferSquare( PrimaryEntities::Polygon<Point>& geometry,
  const double distance,
  bool autoCorrect = false, bool autoNormalize = true )
{
  PrimaryEntities::MultiPolygon<Point> mult;
  bufferSquare( geometry, mult, distance, autoCorrect, autoNormalize );

  // кажется, полигон не может размножиться, но вдруг))
  if( mult.size( ) > 1 )
    throw ( AbstractOperationsFailure( LossOfData ) );

  if( mult.size( ) )
    geometry = mult.at( 0 );
  else
    geometry = PrimaryEntities::Polygon<Point>( );
}
//==============================================================================
template <typename Point>
void bufferSquare( PrimaryEntities::MultiPolygon<Point>& geometry,
  const double distance,
  bool autoCorrect = false, bool autoNormalize = true )
{
  PrimaryEntities::MultiPolygon<Point> mult;
  bufferSquare( geometry, mult, distance, autoCorrect, autoNormalize );
  geometry = mult;
}
//==============================================================================
/*!
 * \brief Округлый буфер для геометрии сущности
 * \details Реализация на основе geos::operation::buffer.
 *          Используются следующие параметры:
 *          - круглые границы линии (CAP_ROUND)
 *          - объединение линий через окружность (JOIN_ROUND)
 *          - срез углов, выступающих от исходной геометрии более чем на
 *            значение DEFAULT_MITRE_LIMIT = 5.
 *          Данный буфер с боксом не работает.
 * \param[in] inEntity            - геометрия (сущность), для которой строится
 *                                  буфер;
 * \param[out] outEntity          - мультиполигон корректных полигонов,
 *                                  соответствующий исходному;
 * \param[in] distance            - размер буфера;
 * \param[in] countPointsInCircle - количество вершин в окружности;
 * \param[in] autoCorrect         - флаг автоматической коррекции:
 *                                  true - выполнять коррекцию, если она нужна,
 *                                  false - не выполнять коррекцию в любом
 *                                  случае;
 * \param[in] autoNormalize       - флаг автоматической нормализации:
 *                                  true - выполнять нормализацию, если она
 *                                  нужна,
 *                                  false - не выполнять нормализацию в любом
 *                                  случае
*/
template <typename InEntity, typename OutEntity>
void bufferRound(const InEntity& inEntity, OutEntity& outEntity,
                 const double distance, 
                 const unsigned int countPointsInCircle = 8U,
                 bool autoCorrect = false, bool autoNormalize = true)
{
  //! Вычисляем буфер
  try 
  {
    return Helper::buffer(inEntity, outEntity, distance,
      countPointsInCircle / 4,
      geos::operation::buffer::BufferParameters::CAP_ROUND,
      geos::operation::buffer::BufferParameters::JOIN_ROUND,
      5., // значение по-умолчанию, взятое из geos (DEFAULT_MITRE_LIMIT)
      autoCorrect, autoNormalize);
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<AbstractOperationsFailure>("bufferRound");
  }
}
//==============================================================================
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_ALGORITHMS_BUFFER_HPP