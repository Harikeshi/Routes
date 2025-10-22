#ifndef PRIMARY_ENTITIES_EXCEPTIONS_HPP
#define PRIMARY_ENTITIES_EXCEPTIONS_HPP
//==============================================================================
#include <ExceptionTools/ExceptionTools.hpp>
//==============================================================================
namespace PrimaryEntities
{
//==============================================================================
/*
 *\brief Именованные коды генерируемых исключений
 */
  enum PrimaryEntitiesEnumFailure
  {
  //!< Некорректный Ring
  IncorrectRing = ExceptionTools::SpecialFailure,
  //!< Некорректный Linestring
  IncorrectLinestring,
  //!< Некорректный тип геометрии
  IncorrectGeometryType,
  //!< Отсутствует указатель на геометрию
  EmptyGeometryPtr
  };
//==============================================================================
  // !< название библиотеки для исключений
  const std::string primaryLibTitle = "Core::PrimaryEntities";
//==============================================================================
  /*!
   * \brief  Класс формирования исключений библиотеки GeometryTools
   */
  class PrimaryEntitiesFailure : public ExceptionTools::Failure
  {
//==============================================================================
  public:
    /*!
     * \brief Создание исключения.
     * \details Требуется только код, остальные параметры подставятся автоматически
     * \param[in] failureId - именованный код исключения
     * \param[in] postfix - текст уточняющий/дополняющий исключение
     *                      (может использоваться для передачи текста исходногоs
     *                       исключения)
     * \param[in] prefix - текст, локализующий место возникновени¤ исключения
     *                     (может использоваться для указания имени функции,
     *                      в которой локализовано исключение - трассировка
     *                      исключения)
     */
    explicit PrimaryEntitiesFailure(const unsigned int failureId,
                                    const std::string& postfix = "",
                                    const std::string& prefix = "") noexcept;
  };
//==============================================================================
} //! PrimaryEntities
//==============================================================================
#endif //! PRIMARY_ENTITIES_EXCEPTIONS_HPP
