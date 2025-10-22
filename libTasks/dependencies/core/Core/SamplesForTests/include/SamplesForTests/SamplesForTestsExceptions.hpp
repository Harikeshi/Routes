#ifndef SAMPLES_FOR_TESTS_EXCEPTIONS_HPP
#define SAMPLES_FOR_TESTS_EXCEPTIONS_HPP
//==============================================================================
#include <ExceptionTools/ExceptionTools.hpp>
//==============================================================================
namespace SamplesForTests
{
//==============================================================================
/*
 *\brief Именованные коды генерируемых исключений
 */
enum SamplesForTestsEnumFailure
{
  //!< Некорректная геометрия
  IncorrectGeometry = ExceptionTools::SpecialFailure,
  //!< Отрицательная дальность / расстояние
  NegativeDistance,
  //!< Дырка вне полигона
  HoleNotInsidePolygon,
  //!< Неизвестная причина некорректности
  UnknownInternalError
  };
//==============================================================================
// !< название библиотеки для исключений
const std::string sampForTestsLibTitle = "Core::SamplesForTests";
//==============================================================================
/*!
 * \brief  Класс формирования исключений библиотеки SamplesForTests
 */
class SamplesForTestsFailure : public ExceptionTools::Failure
{
//==============================================================================
public:
  /*!
   * \brief —Создание исключения.
   * \details Требуется только код, остальные параметры подставяются автоматически
   * \param[in] failureId - именованный код исключения
   * \param[in] postfix - текст уточняющий/дополняющий исключение
   *                      (может использоваться для передачи текста исходного
   *                       исключения)
   * \param[in] prefix - текст, локализующий место возникновения исключения
   *                     (может использоваться для указания имени функции,
   *                      в которой локализовано исключение - трассировка
   *                      исключения)
   */
   explicit SamplesForTestsFailure(const unsigned int failureId,
                                    const std::string& postfix = "",
                                    const std::string& prefix = "") noexcept;
};
//==============================================================================
} //! SamplesForTests
//==============================================================================
#endif //! SAMPLES_FOR_TESTS_EXCEPTIONS_HPP