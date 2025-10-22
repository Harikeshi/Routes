#include <PrimaryEntities/PrimaryEntitiesExceptions.hpp>
//==============================================================================
namespace PrimaryEntities
{
//==============================================================================
  /*
   *\brief Соотношение именованных кодов и текстов ошибок генерируемых исключений
   */
  const std::map<unsigned int, std::string> primaryFailuresText =
  {
    INCLUDE_STANDART_FAILURE_IN_MAP
    {
      IncorrectRing,
      "<IncorrectRing>: Incorrect ring"
    },
    {
      IncorrectLinestring,
      "<IncorrectLinestring>: Incorrect linestring"
    },
    {
      IncorrectGeometryType,
      "<IncorrectGeometryType>: Incorrect geometry type"
    },
    {
      EmptyGeometryPtr,
      "<EmptyGeometryPtr>: Empty geometry pointer"
    }
  };
//==============================================================================
  PrimaryEntitiesFailure::PrimaryEntitiesFailure(
    const unsigned int failureId,
    const std::string& postfix /*= ""*/,
    const std::string& prefix /*= ""*/)
    noexcept
    : Failure(
      failureId,
      primaryLibTitle,
      primaryFailuresText.at(failureId),
      postfix,
      prefix)
  {
  }
//==============================================================================
} //! PrimaryEntities
//==============================================================================
