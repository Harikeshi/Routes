#include <MathTools/BasicMath/LengthOperations.hpp>
//! Constants
#include <MathTools/Constants.hpp>
//! BasicMath
#include <MathTools/BasicMath/Compare.hpp>
//==============================================================================
namespace BasicMath
{
//==============================================================================
bool avgSegmentLength(const double totalLength, const double maxSegmentLegth, 
                      double& avgSegmentLength, int& numSegments)
{
  //! Проверка на необходимость разбиения 
  if ((totalLength <= 0.) || (maxSegmentLegth <= 0.) 
                          || (totalLength <= maxSegmentLegth))
  {
    //! Задаём значения по умолчанию: разбивка на один сегмент
    numSegments = 1;                //!< 1 сегмент
    avgSegmentLength = totalLength; //!< длина сегмента = общей длине

    return false; //!< разбивать не надо
  }

  //! Поиск целого числа сегментов максимально возможной длины 
  numSegments = int(std::ceil(totalLength / maxSegmentLegth));

  //! Вычисляем усредненную длину равных сегментов
  avgSegmentLength = totalLength / numSegments;

  return true;  //!< нужно разбивать
}

//==============================================================================
}
//==============================================================================
