#include <Geodesic/Operations/Conversion.hpp>
//! std
#include <iomanip>
//==============================================================================
namespace Geodesic 
{
//==============================================================================
std::string boxToPostgisString(const GeoBox2D& box, const int precision/*= -1*/)
{
  std::stringstream result;
  if (precision >= 0)
    result << std::fixed << std::setprecision(precision);
  result << "SRID=4326;POLYGON(("
         << box.min_corner().getLongitude().get() << ' '
         << box.min_corner().getLatitude().get()  << ','
         << box.max_corner().getLongitude().get() << ' '
         << box.min_corner().getLatitude().get()  << ','
         << box.max_corner().getLongitude().get() << ' '
         << box.max_corner().getLatitude().get()  << ','
         << box.min_corner().getLongitude().get() << ' '
         << box.max_corner().getLatitude().get()  << ','
         << box.min_corner().getLongitude().get() << ' '
         << box.min_corner().getLatitude().get()
         << "))";
  return result.str();
}
//==============================================================================
GeoBox2D boxFromStdString(const std::string& str)
{
  //! Удаление символа из строки
  auto eraser = [](std::string& val, const char& sym)
  {
    val.erase(std::remove(val.begin(), val.end(), sym), val.end());
  };

  //! Изменение символа локали
  auto local = [](std::string& val, const std::string& delimiter)
  {
    if (val.find(delimiter) == std::string::npos)
    {
      if (delimiter == ".")
        val.replace(val.find(","), 1, delimiter);
      else
        val.replace(val.find("."), 1, delimiter);
    }
  };

  //! Начинаем разбор
  std::string parseStr(str);

  //! Убираем скобочки
  eraser(parseStr, '(');
  eraser(parseStr, ')');

  //! Получаем десятичный разделитель
  std::string decDelimiter(localeconv()->decimal_point);

  //! Разбиваем через разделитель и приводим к double
  std::string delimiter(",");
  std::vector<double> vals;
  std::size_t pos;

  while ((pos = parseStr.find(delimiter)) != std::string::npos)
  {
    std::string tmp(parseStr.substr(0, pos));
    local(tmp, decDelimiter);
    vals.emplace_back(std::stod(tmp));
    parseStr.erase(0, pos + delimiter.length());
  }
  std::string tmp(parseStr.substr(0, pos));
  local(tmp, decDelimiter);
  vals.emplace_back(std::stod(tmp));

  //! Формируем бокс
  return GeoBox2D(GeoPoint2D(Degree(vals.at(3)), Degree(vals.at(2))),
                  GeoPoint2D(Degree(vals.at(1)), Degree(vals.at(0))));

}
//==============================================================================
} //! Geodesic
//==============================================================================
