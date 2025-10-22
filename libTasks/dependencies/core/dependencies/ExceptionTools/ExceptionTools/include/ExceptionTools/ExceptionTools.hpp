#ifndef EXCEPTION_TOOLS_FAILURE_HPP
#define EXCEPTION_TOOLS_FAILURE_HPP
//==============================================================================
// std
#include <stdexcept>
#include <string>
#include <map>
//==============================================================================
namespace ExceptionTools
{
//==============================================================================
  /*!
   * \brief Класс формирования исключений
   */
  class Failure : public std::runtime_error
  {
//==============================================================================
  public:
    /*!
     * \brief Получение кода исключения.
     * \return Код исключения
    */
    inline unsigned int getCode() const
    {
      return mCode;
    }
//==============================================================================
    /*!
     * \brief Получение названия библиотеки, сгенерировавшей исключение.
     * \return Краткое название библиотеки
    */
    inline std::string getLib() const
    {
      return mLibTitle;
    }
//==============================================================================
  protected:
    /*!
     * \brief Создание исключения.
     * \param[in] code - код исключения
     * \param[in] libTitle - краткое название сгенерировавшей библиотеки
     * \param[in] text - текст исключения
     * \param[in] postfix - текст уточняющий/дополняющий исключение
     *                      (может использоваться для передачи текста исходного
     *                       исключения)
     * \param[in] prefix - текст, локализующий место возникновения исключения
     *                     (может использоваться для указания имени функции,
     *                      в которой локализовано исключение - трассировка
     *                      исключения)
    */
    Failure(
      unsigned int code, const std::string& libTitle, const std::string& text,
      const std::string& postfix = "", const std::string& prefix = "");

    virtual ~Failure( )
    {}
//==============================================================================
  private:
//==============================================================================
    unsigned int mCode;     //!< код исключения
    std::string  mLibTitle; //!< краткое название сгенерировавшей библиотеки
//==============================================================================
  };
//==============================================================================

//==============================================================================
  /*!
   * \brief Перечисление стандартных исключений
  */
  enum StandartFailureEnum
  {
    UndefinedFailure  = 0,  //!< Неопределённое исключение
    UnknownFailure    = 1,  //!< Неизвестное, но стандартное исключение
    TransitFailure    = 2,  //!< Определённое исключение, передаваемое выше
    SpecialFailure    = 100 //!< Специализированное исключение (значение),
                            //!  с которого стартуют специализированные
                            //!  перечисления исключений
  };
//==============================================================================
  /*!
   * \brief Стандартные сообщения исключений
  */
  const std::map<unsigned int, std::string> standartFailuresText =
        { { UndefinedFailure, " <Undefined exception>" },
          { UnknownFailure,   " <Third-party exception> Original message: " },
          { TransitFailure,   " <Transit failure>\n" } };
//==============================================================================
  /*!
  * \brief Макрос для встраивания стандартных сообщений в перечень
  *        специализированных, используемых в конкретной библиотеке
  */
  #define INCLUDE_STANDART_FAILURE_IN_MAP                                     \
  {                                                                           \
    ExceptionTools::UndefinedFailure,                                         \
    ExceptionTools::standartFailuresText.at(ExceptionTools::UndefinedFailure) \
  },                                                                          \
  {                                                                           \
    ExceptionTools::UnknownFailure,                                           \
    ExceptionTools::standartFailuresText.at(ExceptionTools::UnknownFailure)   \
  },                                                                          \
  {                                                                           \
    ExceptionTools::TransitFailure,                                           \
    ExceptionTools::standartFailuresText.at(ExceptionTools::TransitFailure)   \
  },                                                                          \
//==============================================================================
  /*!
    * \brief Функция обработки исключений.
    *
    * \details Только для исключений, которые отвечают следующим требованиям:
    *          - исключение унаследовано об базового класса Failure
    *          - интерфейс должен содержать три следующих параметра:
    *            1. код исключения (unsigned int);
    *            2. постфикс (std::string), по умолчению может быть пустым;
    *            3. префикс (std::string), по умолчению может быть пустым.
    *          - в перечислении исключений должны присутствовать следующие типы:
    *            0 - неопределённое исключение;
    *            1 - неизвестное исключение из состава стандартных;
    *            2 - транзитное исключение.
    *          - map с текстами исключения строится на кодах (unsigned int)
    *
    * Пример класса исключения:
    *
    * enum ExampleFailureEnum
    * {
    *   Failure100 = ExceptionTools::SpecialFailure, //!< 100
    *   Failure101,                                  //!< 101
    *   Failure110 = Failure100 + 10,                //!< 110
    *   ...
    * };
    *
    * const std::map<unsigned int, std::string>
    * exampleMap = {
    *                INCLUDE_STANDART_FAILURE_IN_MAP
    *                { Failure100, " Failure 100 "},
    *                { Failure101, " Failure 101 "  },
    *                { Failure110, " Failure 110 "  },
    *                ...
    *              };
    *
    * class ExampleFailure : public Failure
    * {
    * public:
    *   ExampleFailure (unsigned int code,
    *                   const std::string& postfix = "",
    *                   const std::string& prefix = "" )
    *   : Failure (code, std::string("Example"),
    *              exampleMap.at(code),
    *              postfix, prefix)
    *   {}
    * };
    *
    * \param[in] functionName - описание блока, для которого вызывается
    *                           обработчик исключений.
  */
  template <class FailureClass>
  void FailureHandler(const std::string& functionName = "")
  {
    try
    {
      throw;
    }
    catch (const Failure& exception)
    {
      //! Проброс нашего исключения дальше наверх
        throw FailureClass(TransitFailure,
                         std::string(exception.what()),
                         functionName);
    }
    catch (const std::exception & exception)
    {
      //! Проброс стандартного исключения дальше наверх
      throw FailureClass(UnknownFailure,
                         std::string(exception.what()),
                         functionName);
    }
    catch (...)
    {
      //! Проброс неопознанного исключения дальше наверх
      throw FailureClass(UndefinedFailure,
                         std::string(""),
                         functionName);
    }
  }
//==============================================================================
} //! ExceptionTools
//==============================================================================
#endif //! EXCEPTION_TOOLS_FAILURE_HPP