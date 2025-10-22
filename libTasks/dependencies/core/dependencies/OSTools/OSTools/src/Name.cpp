#include <OSTools/Name.hpp>
//==============================================================================
namespace OS
{
//==============================================================================
std::string getEntityName(const std::string& path) noexcept
{
  /*!<
   * Индекс последнего символа, не являющегося разделителем имён ('/' или '\\').
   */
  const auto lastNonDelimiterSymbolIndex(path.find_last_not_of("/\\"));
  //! Если нет символов, не являющихся разделителями, возвращаем пустую строку.
  if (lastNonDelimiterSymbolIndex == std::string::npos)
    return std::string("");
  /*!<
   * Индекс последнего разделителя,
   * после которого следует символ, не являющийся разделителем.
   */
  const auto lastImportantDelimiterIndex(
    path.find_last_of("/\\", lastNonDelimiterSymbolIndex));
  //! Если перед именем нет разделителей, всё равно возвращаем имя.
  if (lastImportantDelimiterIndex == std::string::npos)
    return path.substr(0, lastNonDelimiterSymbolIndex + 1);
  
  return path.substr(
    lastImportantDelimiterIndex + 1,
    lastNonDelimiterSymbolIndex - lastImportantDelimiterIndex);
}
//==============================================================================
} //! OS
//==============================================================================