#include <OSTools/OSException.hpp>
//==============================================================================
/*!
 *\brief Соотношение именованных кодов и текстов ошибок генерируемых исключений
 */
const std::map<unsigned int, std::string> OS_FAILURES_TEXT =
{
  INCLUDE_STANDART_FAILURE_IN_MAP
  {
    CouldNotOpenDirectory,
    "<OS::CouldNotOpenDirectory>: "
    "Не удалось открыть директорию"
  },
  {
    UnrecognizedFileSystemError,
    "<OS::UnrecognizedFileSystemError>: "
    "Нераспознанная ошибка при работе с файловой системой"
  }
};
//==============================================================================
  OSFailure::OSFailure(const unsigned int failureId,
    const std::string& postfix /*= ""*/,
    const std::string& prefix /*= ""*/) noexcept
    : Failure(
      failureId,
      OS_LIB_TITLE,
      OS_FAILURES_TEXT.at(failureId),
      postfix,
      prefix)
{
}
//==============================================================================
