#include <SamplesForTests/SamplesForTestsExceptions.hpp>
//==============================================================================
namespace SamplesForTests
{
//==============================================================================
/*
 *\brief Соотношение именованных кодов и текстов ошибок генерируемых исключений
 */
const std::map<unsigned int, std::string> sampForTestsFailuresText =
{
  INCLUDE_STANDART_FAILURE_IN_MAP
  {
    NegativeDistance,
    "<NegativeDistance>: Negative side's size"
  },
  {
    IncorrectGeometry,
    "<IncorrectGeometry>: Incorrect geometry"
  },
  {
    HoleNotInsidePolygon,
    "<HoleNotInsidePolygon>: Hole not inside polygon"
  },
  {
    UnknownInternalError,
    "<UnknownInternalError>: Unknown error"
  }
};
//==============================================================================
SamplesForTestsFailure::SamplesForTestsFailure(
                                            const unsigned int failureId,
                                            const std::string& postfix /*= ""*/,
                                            const std::string& prefix /*= ""*/ ) 
noexcept
: Failure (
    failureId, 
    sampForTestsLibTitle,
    sampForTestsFailuresText.at(failureId),
    postfix,
    prefix)
{
}
//==============================================================================
} //! SamplesForTests
//==============================================================================
