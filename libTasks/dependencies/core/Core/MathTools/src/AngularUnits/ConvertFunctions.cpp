#include "ConvertFunctions.hpp"
//! std
#include <locale>
//==============================================================================
std::string intToString(const int number, const unsigned int digitDepth)
{
  //! Перевод числа в строку
  std::string outputString(std::to_string(number));
  //! Сравнение требуемого числа символов с получившимся
  int count(digitDepth - static_cast<int>(outputString.size()));
  if (number >= 0)
  {
    if (count > 0)
      outputString.insert(0, count, '0'); //!< добавление нулей до необходимого
  }
  else
  {
    count++;
    if (count > 0)
      outputString.insert(1, count, '0'); //!< добавление нулей до необходимого
                                          //!  первым (0) идет знак "-"
  }
  return outputString;
}
//==============================================================================
std::string doubleToString(const double number, 
                           const unsigned int intDigitDepth, 
                           const unsigned int fractDigitDepth,
                           const std::string& decimalDelimiter /*= "."*/)
{
  //! Разделение вещественного числа на целую и дробную часть
  double intPart;
  double fractPart(modf(fabs(number), &intPart));
  
  //! Собираем строку
  if (number < 0)
    intPart *= -1.;
  if (fractDigitDepth > 0)
  {
    //! Если требуется дробная часть

    //! Обрабатываем дробную часть
    std::string fractString(
                  intToString(static_cast<int>(std::round(
                                fractPart * std::pow(10., fractDigitDepth))),
                              fractDigitDepth));
    if (fractString.size() > fractDigitDepth)
    {
      //! После округления требуется увеличить целую часть на 1
      intPart ++;
      //! Дробная часть становится 0
      fractString = intToString(0, fractDigitDepth);
    }
    return intToString(static_cast<int>(intPart), intDigitDepth)
           +  decimalDelimiter
           + fractString;
  }
  //! Если дробная часть не требуется 
  return intToString(static_cast<int>(std::round(number)), intDigitDepth);
}
//==============================================================================
std::string 
fromDegreesToDegreeString(const double value, 
                          const GeoCoordType geoCordType, 
                          const unsigned int digitDepth /*= 2*/, 
                          const std::string& decimalDelimiter /*= "."*/)
{
  //! Вычисляем значения градусов, минут и секунд
  double degrees;
  double minutes(modf(fabs(value), &degrees));
  minutes *= DEGREE_RATIO;
  double seconds(modf(minutes, &minutes));
  seconds *= DEGREE_RATIO;
  //! Собираем строку
  std::string result;
  //! Добавляем значение градусов
  if (geoCordType == LATITUDE)
    result += intToString(static_cast<int>(degrees), 2);
  else
    result += intToString(static_cast<int>(degrees), 3);
  result += "\xB0"                                    //!< символ °
         + intToString(static_cast<int>(minutes), 2)  //!< значение минут
         + "\x27"                                     //!< символ '
                                                      //! значение секунд:
         + doubleToString(fabs(seconds), 2, digitDepth, decimalDelimiter)
         + "\x22";                                    //!< символ "
  if (geoCordType == LATITUDE)
  {
    if (value < 0.)
      result += "\xDE.\xD8."; //!< Ю.Ш. 
    else
      result += "\xD1.\xD8."; //!< С.Ш.
  }
  else if (geoCordType == LONGITUDE)
  {
    if (value < 0.)
      result += "\xC7.\xC4."; //!< З.Д.
    else
      result += "\xC2.\xC4."; //!< В.Д.
  }
  return result;
}
//==============================================================================
std::string 
fromRadiansToDegreeString(const double value, 
                          const GeoCoordType geoCordType, 
                          const unsigned int digitDepth /*= 2*/, 
                          const std::string& decimalDelimiter /*= "."*/)
{
  return fromDegreesToDegreeString(fromRadiansToDegrees(value), geoCordType,
                                   digitDepth, decimalDelimiter);
}
//==============================================================================
bool fromDegreesStringToDegrees(const std::string& degreesString,
                                double& degrees,
                                bool hasNeedCheck /*= false*/)
{
  //! В зависимости от необходимости проверять исходную строку на корректность,
  //! запускается одна из двух веток.
  //! Да, есть дублирование кода, его можно убрать, но тогда возрастает 
  //! количество проверок флага, поэтому выбран более оптимизированный вариант.
  if (hasNeedCheck)
  {
    //! C проверкой

    //! Поиск позиции символа °
    size_t degreePosition(degreesString.find("\xB0"));
    if (degreePosition == std::string::npos)
      return false; //!< строка не формализована

                    //! Поиск позиции символа '
    size_t minutePosition(degreesString.find("\x27"));
    if (   (minutePosition == std::string::npos)
        || (minutePosition < degreePosition))
      return false; //!< строка не формализована 

                    //! Поиск позиции символа "
    size_t secondPosition(degreesString.find("\x22"));
    if (   (secondPosition == std::string::npos)
        || (secondPosition < minutePosition))
      return false; //!< строка не формализована 

                    //! Получение строки со значением полушария
    std::string stringHemisphere(degreesString, secondPosition + 1);
    if (stringHemisphere.size() != 4)
      return false; //!< строка не формализована

    double signDegree(1.);
    if (   (stringHemisphere == "\xC7.\xC4.") 
        || (stringHemisphere == "\xDE.\xD8."))
      signDegree = -1.;
    else if (   (stringHemisphere != "\xC2.\xC4.")
             && (stringHemisphere != "\xD1.\xD8."))
      return false; //!< строка не формализована

                    //! Получение строки значения градусов
    const std::string stringDegree(degreesString, 0, degreePosition);

    //! Получаем значение градусов
    int valueDegree;
    try
    {
      valueDegree = std::stoi(stringDegree);
    }
    catch (...)
    {
      return false; //!< строка не формализована
    }

    //! Проверяем, что градусы определились корректно
    if (   stringDegree
        != intToString(valueDegree,
                       static_cast<unsigned int>(stringDegree.size())))
      return false; //!< строка не формализована

                    //! Получение строки значения минут
    std::string stringMinute(degreesString, degreePosition + 1,
                             minutePosition - degreePosition - 1);

    //! Получаем значение минут
    int valueMinute;
    try
    {
      valueMinute = std::stoi(stringMinute);
    }
    catch (...)
    {
      return false; //!< строка не формализована
    }

    //! Проверяем, что минуты определились корректно
    if (   stringMinute
        != intToString(valueMinute,
                       static_cast<unsigned int>(stringMinute.size())))
      return false; //!< строка не формализована

    //! Получение строки значения секунд
    std::string stringSecond(degreesString, minutePosition + 1,
                             secondPosition - minutePosition - 1);

    //! Определяем символ разделитель
    std::string decimalDelimiter(localeconv()->decimal_point);
    size_t delimiterPosition(stringSecond.find("."));
    if (delimiterPosition == std::string::npos)
    {
      //! Если не точка, то может запятая
      delimiterPosition = stringSecond.find(",");
    }
    unsigned int digitDepth(0);
    if (delimiterPosition != std::string::npos)
    {
      //! Разделитель найден
      if (decimalDelimiter != stringSecond.substr(delimiterPosition, 1))
      {
        //! Надо менять разделитель под локаль
        stringSecond.replace(delimiterPosition, 1, decimalDelimiter);
      }
      //! Определяем разрядность дробной части
      digitDepth = static_cast<unsigned int>(stringSecond.size() - 1
                                             - delimiterPosition);
    }
    else
      delimiterPosition = stringSecond.size();

    //! Получаем значение секунд
    double valueSecond;
    try
    {
      valueSecond = std::stod(stringSecond);
    }
    catch (...)
    {
      return false; //!< строка не формализована
    }

    //! Проверяем, что секунды определились корректно
    if (   stringSecond
        != doubleToString(valueSecond,
                          static_cast<unsigned int>(delimiterPosition),
                          digitDepth, decimalDelimiter))
      return false; //!< строка не формализована

                    //! Формируем результат
    degrees = signDegree
              * (std::stod(stringDegree)
                 + std::stod(stringMinute) / DEGREE_RATIO
                 + std::stod(stringSecond) / (DEGREE_RATIO * DEGREE_RATIO));

    return true;
  }

  //! Без проверки
  try
  {
    size_t degreePosition(degreesString.find("\xB0"));  //!< позиция символа °
    size_t minutePosition(degreesString.find("\x27"));  //!< позиция символа '
    size_t secondPosition(degreesString.find("\x22"));  //!< позиция символа "

    //! Получение строки значения секунд
    std::string stringSecond(degreesString, minutePosition + 1,
                             secondPosition - minutePosition - 1);

    //! Определяем символ разделитель
    std::string decimalDelimiter(localeconv()->decimal_point);
    size_t delimiterPosition(stringSecond.find("."));
    if (delimiterPosition == std::string::npos)
    {
      //! Если не точка, то может запятая
      delimiterPosition = stringSecond.find(",");
    }
    if (delimiterPosition != std::string::npos)
    {
      //! Разделитель найден
      if (decimalDelimiter != stringSecond.substr(delimiterPosition, 1))
      {
        //! Надо менять разделитель под локаль
        stringSecond.replace(delimiterPosition, 1, decimalDelimiter);
      }
    }

    //! Получаем величину
    degrees = std::stod(degreesString.substr(0, degreePosition))
              + std::stod(degreesString.substr(degreePosition + 1,
                                           minutePosition - degreePosition - 1))
                / DEGREE_RATIO
              + std::stod(stringSecond) / (DEGREE_RATIO * DEGREE_RATIO);

    //! Определяем знак
    std::string hemisphere(degreesString, secondPosition + 1); //!< полушарие
    if ((hemisphere == "\xC7.\xC4.") || (hemisphere == "\xDE.\xD8."))
      degrees *= -1.;
  }
  catch (...)
  {
    return false;
  }
  return true;
}
//==============================================================================
bool fromDegreesStringToRadians(const std::string& degreesString,
                                double& radians,
                                bool hasNeedCheck /*= false*/)
{
  //! Проводим преобразование в градусы
  double degrees(0.);
  bool isSuccessful(fromDegreesStringToDegrees(degreesString, degrees, 
                                               hasNeedCheck));
  
  //! Если перевод успешен, то переводим результат в радианы
  if (isSuccessful)
    radians = fromDegreesToRadians(degrees);

  //! Возвращаем флаг успешности операции
  return isSuccessful;
}
//==============================================================================
