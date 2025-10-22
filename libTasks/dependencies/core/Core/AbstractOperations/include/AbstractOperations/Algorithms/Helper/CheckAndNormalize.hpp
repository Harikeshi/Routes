#ifndef ABSTRACT_OPERATIONS_ALGORITHMS_HELPER_CHECK_AND_NORMALIZE_HPP
#define ABSTRACT_OPERATIONS_ALGORITHMS_HELPER_CHECK_AND_NORMALIZE_HPP
//==============================================================================
#include <AbstractOperations/Algorithms/Correct.hpp>
#include <AbstractOperations/AbstractOperationsException.hpp>
#include <AbstractOperations/ExtractPointsFromGeoms.hpp>
#include <AbstractOperations/Normalize.hpp>
//! PrimaryEntities
#include <PrimaryEntities/AtEntitiesType.hpp>
//==============================================================================
namespace AbstractOperations
{
namespace Helper
{
//==============================================================================
/*
 * \brief Оценка корректности и нормализации (REGULAR -> NORMAL) 
 *        при необходимости
 * \param[in|out] leftEntity - левая геометрия в операции (нормализуется при 
 *                             необходимости)
 * \param[in|out] rightEntity - правая геометрия в операции (нормализуется при
 *                              необходимости)
 * \param[in] isGeodesicResult - признак, что результат должен быть в 
 *                               геокоординатах
 * \param[in] autoNormalize - флаг автоматической нормализации:
 *                            true - выполнять нормализацию, если она нужна,
 *                            false - не выполнять нормализацию в любом случае
 * \param[in] withCorrect - флаг необходимости выполнить корректировку 
 *                          геометрии, если это возможно:
 *                          true - выполнять коррекцию, если она нужна,
 *                          false - не выполнять коррекцию в любом случае
 * \return Признак необходимости нормализации: true - нормализация выполнена 
 * \throws IncorrectGeometry || IrrelevantGeometryType
*/
template <typename LeftEntity, typename RightEntity>
bool checkAndNormalize(LeftEntity& leftEntity, 
                       RightEntity& rightEntity,
                       bool isGeodesicResult,
                       bool autoNormalize,
                       bool withCorrect,
                       const std::string& functionName = "")
{
  //! признак, задана ли левая геометрия в геокоординатах
  using namespace PrimaryEntities;
  bool isLeftGeo = isEntityType1(leftEntity, GeodesicEntity);

  //! Проверка соответствия типов
  if (   (isLeftGeo != isEntityType1(rightEntity, GeodesicEntity))
      || (isLeftGeo != isGeodesicResult))
    throw AbstractOperationsFailure(IrrelevantGeometryType, "", functionName);

  //! Оценка необходимости нормализации
  bool needNormalize = isLeftGeo && autoNormalize;

  //! Нормализация: REGULAR -> NORMAL
  using AbstractOperations::getBorderPoints;
  if (needNormalize)
  {
    auto leftPoints = getBorderPoints(leftEntity);
    auto rightPoints = getBorderPoints(rightEntity);
    leftPoints.insert(leftPoints.end(), rightPoints.begin(), rightPoints.end());

    if (isNeedToBeNormalized(leftPoints))
    {
      normalize(leftEntity, NORMAL);
      normalize(rightEntity, NORMAL);
    }
    else
      needNormalize = false;
  }

  //! Проверка входных геометрий на корректность 
  std::string message, otherMessage;
  if (withCorrect)
  {
    if( ( ! correct( leftEntity, message ) )
     || ( ! correct( rightEntity, otherMessage ) ) )
    {
      if( message.size( ) && otherMessage.size( ) )
        // добавляем разделитель для нескольких сообщений
        message += "\n";
      throw AbstractOperationsFailure( IncorrectGeometry, 
        message + otherMessage, functionName );
    }
  }
  else
  {
    if( ( ! isCorrect( leftEntity, message ) ) 
     || ( ! isCorrect( rightEntity, otherMessage ) ) )
    {
      if( message.size( ) && otherMessage.size( ) )
        // добавляем разделитель для нескольких сообщений
        message += "\n";
      throw AbstractOperationsFailure( IncorrectGeometry, 
        message + otherMessage, functionName );
    }
  }
  //! Возвращаем флаг нормализации
  return needNormalize;
}
//==============================================================================
/*
 * \brief Оценка корректности и нормализации (REGULAR -> NORMAL) 
 *        при необходимости
 * \param[in|out] inEntity - геометрия (нормализуется при необходимости)
 * \param[in] isGeodesicResult - признак, что результат должен быть в 
 *                               геокоординатах
 * \param[in] autoNormalize - флаг автоматической нормализации:
 *                            true - выполнять нормализацию, если она нужна,
 *                            false - не выполнять нормализацию в любом случае
 * \param[in] withCorrect - флаг необходимости выполнить корректировку
 *                          геометрии, если это возможно:
 *                          true - выполнять коррекцию, если она нужна,
 *                          false - не выполнять коррекцию в любом случае
 * \return Признак необходимости нормализации: true - нормализация выполнена
 * \throws IncorrectGeometry || IrrelevantGeometryType
*/
template <typename InEntity>
bool checkAndNormalize(InEntity& inEntity,
                       bool isGeodesicResult,
                       bool autoNormalize,
                       bool withCorrect,
                       const std::string& functionName = "")
{
  //! признак, задана ли геометрия в геокоординатах
  using namespace PrimaryEntities;
  bool isInGeo = isEntityType1(inEntity, GeodesicEntity);

  //! Проверка соответствия типов
  if (isInGeo != isGeodesicResult)
    throw AbstractOperationsFailure(IrrelevantGeometryType, "", functionName);

  //! Оценка необходимости нормализации
  bool needNormalize = isInGeo && autoNormalize;

  //! Нормализация: REGULAR -> NORMAL
  if (needNormalize)
  {
    if (isNeedToBeNormalized(inEntity))
      normalize(inEntity, NORMAL);
    else
      needNormalize = false;
  }

  //! Проверка входной геометрии на корректность 
  std::string message;
  if (withCorrect)
  {
    if ( ! correct(inEntity, message) )
      throw AbstractOperationsFailure(IncorrectGeometry, message, functionName);
  }
  else
  {
    if ( ! isCorrect(inEntity, message) )
      throw AbstractOperationsFailure(IncorrectGeometry, message, functionName);
  }
  //! Возвращаем флаг нормализации
  return needNormalize;
}
//==============================================================================
} //! Helper 
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_ALGORITHMS_HELPER_CHECK_AND_NORMALIZE_HPP