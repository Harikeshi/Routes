#include <OSTools/CreateDirectory.hpp>
//==============================================================================
//! std
#ifdef __linux__
#include <sys/stat.h>
#include <sys/types.h>
#else
#include <Windows.h>
#endif
//==============================================================================
namespace OS
{
//==============================================================================
bool
createDirectory(
  const std::string& sourceDirectory,
  const std::string& directoryToCreateName) noexcept
{
  std::string newDirectoryPath(sourceDirectory + "/" + directoryToCreateName);
#ifdef __linux__
  return mkdir(newDirectoryPath.c_str(), 0777) == 0;
#else
  return CreateDirectory(newDirectoryPath.c_str(), NULL) != 0;
#endif
}
//==============================================================================
} //! OS
//==============================================================================