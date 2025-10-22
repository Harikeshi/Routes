#ifndef MATH_TOOLS_EXCEPTIONS_HPP
#define MATH_TOOLS_EXCEPTIONS_HPP
//==============================================================================
#include <ExceptionTools/ExceptionTools.hpp>
//==============================================================================
using namespace ExceptionTools;
//==============================================================================
/*!
 * \brief Именованные коды генерируемых исключений библиотеки MathTools.
 */
enum MathEnumFailure
{
  /*!<
   * Размеры матриц "потребителей", "поставщиков" и "цен"
   * в реализации приближённого решения обобщённой задачи о назначениях
   * сочетаются неправильно.
   */
  GAPApproximationBadParametersSizes = SpecialFailure,          //!< 100
  //!< В матрице "потребителей" содержится ненатуральное число.
  GAPApproximationNonNaturalUserValue,                          //!< 101
  //!< В векторе "поставщиков" содержится отрицательное число.
  GAPApproximationNegativeProviderValue,                        //!< 102
  //!< В матрице "цен" содержится отрицательное число.
  GAPApproximationNegativeRateValue,                            //!< 103

  /*!< 110
   * Размерности векторов "цен" и "весов"
   * в реализации решения задачи о рюкзаке 0-1
   * не равны друг другу.
   */
  KnapsackProblemDifferentDimCounts = GAPApproximationBadParametersSizes + 10, 
  //!< Отрицательная "вместимость". 
  KnapsackProblemNegativeCapacity,                              //!< 111
  //!< В векторе "весов" содержится отрицательное число.
  KnapsackProblemNegativeWeightValue,                           //!< 112
  
  //!< Деление радиан на нуль.
  RadianDivideByZero = KnapsackProblemDifferentDimCounts + 10,  //!< 120 
  //!< Деление градусов на нуль.
  DegreeDivideByZero,                                           //!< 121
};
//==============================================================================
//!< Префикс исключений MathTools.
const std::string mathLibTitle = "Core::MathTools"; 
//==============================================================================
/*!
  * \brief Класс формирования исключений библиотеки MathTools
  */
class MathFailure : public ExceptionTools::Failure
{
//==============================================================================
public:
  /*!
   * \brief Конструктор.
   *
   * \details Требуется только код,
   * остальные параметры подставятся автоматически.
   *
   * \param[in] failureId -- именованный код исключения.
   * \param[in] postfix - текст уточняющий/дополняющий исключение
   *                      (может использоваться для передачи текста исходного
   *                       исключения)
   * \param[in] prefix - текст, локализующий место возникновения исключения
   *                     (может использоваться для указания имени функции,
   *                      в которой локализовано исключение - трассировка
   *                      исключения)
   */
  explicit MathFailure(const unsigned int failureId,
                       const std::string& postfix = "",
                       const std::string& prefix = "" ) noexcept;
};
//==============================================================================
#endif //! MATH_TOOLS_EXCEPTIONS_HPP