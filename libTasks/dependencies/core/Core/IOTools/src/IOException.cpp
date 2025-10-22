#include <IOTools/IOException.hpp>
//==============================================================================
/*!
 *\brief Соотношение именованных кодов и текстов ошибок генерируемых исключений
 */
const std::map<unsigned int, std::string> IO_FAILURES_TEXT =
{
  INCLUDE_STANDART_FAILURE_IN_MAP
};
//==============================================================================
  IOFailure::IOFailure(const unsigned int failureId,
    const std::string& postfix /*= ""*/,
    const std::string& prefix /*= ""*/) noexcept
    : Failure(
      failureId,
      IO_LIB_TITLE,
      IO_FAILURES_TEXT.at(failureId),
      postfix,
      prefix)
{
}
//==============================================================================