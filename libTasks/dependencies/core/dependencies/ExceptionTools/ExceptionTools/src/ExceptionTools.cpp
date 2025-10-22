#include <ExceptionTools/ExceptionTools.hpp>
//==============================================================================
namespace ExceptionTools
{
//==============================================================================
Failure::Failure(
  unsigned int code, const std::string& libTitle, const std::string & text,
  const std::string& postfix /*= ""*/, const std::string& prefix /*= ""*/)
: std::runtime_error("[" + libTitle + "] " + prefix + text + postfix)
, mCode(code)
, mLibTitle(libTitle)
{
}
//==============================================================================
} //! ExceptionTools
//==============================================================================
