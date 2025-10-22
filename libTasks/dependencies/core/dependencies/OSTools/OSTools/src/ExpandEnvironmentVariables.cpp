#include <OSTools/ExpandEvironmentVariables.hpp>
//==============================================================================
#if defined(_WIN64) || defined(_WIN32)
std::string OS::expandEnvironmentVariables(const std::string& rawPath) noexcept
{
  DWORD count(ExpandEnvironmentStrings(rawPath.c_str(), NULL, 0));
  LPSTR buf((LPSTR)(new CHAR[count]));
  ExpandEnvironmentStrings(rawPath.c_str(), buf, count);
  std::string result(buf);
  delete[] buf;
  return result;
}
//==============================================================================
#endif
//==============================================================================