#include <MathTools/MathException.hpp>
//==============================================================================
/*!<
 * Соотношение именованных кодов и текстов ошибок генерируемых исключений.
 */
const std::map<unsigned int, std::string> mathFailuresText =
{
  INCLUDE_STANDART_FAILURE_IN_MAP
  {
    GAPApproximationBadParametersSizes,
    "<GAPApproximationBadParametersSizes>: "
    "Размеры матриц \"потребителей\", \"поставщиков\" и \"цен\" "
    "в реализации приближённого решения обобщённой задачи о назначениях "
    "сочетаются неправильно. "
    "Для корректной работы необходимо, чтобы размеры матриц \"потребителей\" "
    "и \"цен\" были одинаковыми, "
    "а размер вектора \"поставщиков\" был равен количеству столбцов "
    "в матрице \"цен\"."
  },
  {
    GAPApproximationNonNaturalUserValue,
    "<GAPApproximationNonNaturalUserValue>: "
    "В матрице \"потребителей\" содержится ненатуральное число."
  },
  {
    GAPApproximationNegativeProviderValue,
    "<GAPApproximationNegativeProviderValue>: "
    "В векторе \"поставщиков\" содержится отрицательное число."
  },
  {
    GAPApproximationNegativeRateValue,
    "<GAPApproximationNegativeRateValue>: "
    "В матрице \"цен\" содержится отрицательное число."
  },
  {
    KnapsackProblemDifferentDimCounts,
    "<KnapsackProblemDifferentDimCounts>: "
    "Размерности векторов \"цен\" и \"весов\" "
    "в реализации решения задачи о рюкзаке 0 - 1"
    "не равны друг другу."
  },
  {
    KnapsackProblemNegativeCapacity,
    "<KnapsackProblemNegativeCapacity>: Отрицательная \"вместимость\"."
  },
  {
    KnapsackProblemNegativeWeightValue,
    "<KnapsackProblemNegativeWeightValue>: В векторе \"весов\" содержится "
    " отрицательное число."
  },
  {
    RadianDivideByZero,
    "<RadianDivideByZero>: Деление числа в радианах на нуль."
  },
  {
    DegreeDivideByZero,
    "<DegreeDivideByZero>: Деление числа в градусах на нуль."
  }
};
//==============================================================================
MathFailure::MathFailure(const unsigned int failureId,
                         const std::string& postfix /*= ""*/,
                         const std::string& prefix /*= ""*/) noexcept
: Failure(
  failureId,
  mathLibTitle,
  mathFailuresText.at(failureId),
  postfix,
  prefix)
{}
//==============================================================================
