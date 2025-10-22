#pragma once

/*! Core */
#include <ExceptionTools/ExceptionTools.hpp>

namespace Exceptions {

/**
 * @brief Именованные коды генерируемых исключений во время валидации входных данных.
 * 
 */
enum ValidationEnumFailure
{
    /**
     * @brief Коды ошибок исключений валидации.
     * 
     */
    MissingField = 100,           //!< не содержит поле.
    NotArray = MissingField + 10, //!< не является массивом.
    NotDigit,                     //!< не является числом.
    NotString,                    //!< не является строкой.
    InvalidValue,                 //!< не в интервале.
    EmptyField,                   //!< пустое поле == "null", ""
    EmptyArray,                   //!< пустой массив.
    BadLength,                    //!< длина не соответствует.
    IsNull
};

/**
 * @brief Перечисление код-сообщение исключений валидации.
 * 
 */
const std::map<unsigned int, std::string> validationFailuresText = {
    {MissingField, "<MissingField>: "},
    {NotArray, "<NotArray>: "},
    {NotDigit, "<NotDigit>: "},
    {NotString, "<NotString>: "},
    {InvalidValue, "<InvalidValue>: "},
    {EmptyField, "<EmptyField>: "},
    {EmptyArray, "<EmptyArray>: "},
    {BadLength, "<BadLength>: "},
    {IsNull, "<IsNull>"}};

const std::string validationTitle = "Validation Error"; // Префикс исключений Validation Error.

/**
 * @brief Класс исключений генерируемых при валидации структур входных данных с использованием библиотеки Core.
 * 
 */
class ValidationFailure : public ExceptionTools::Failure
{
    //==============================================================================
public:
    /**
   * @param[in] failureId -- именованный код исключения.
   * @param[in] postfix - текст уточняющий/дополняющий исключение
   *                      (может использоваться для передачи текста исходного
   *                       исключения)
   * @param[in] prefix - текст, локализующий место возникновения исключения
   *                     (может использоваться для указания имени функции,
   *                      в которой локализовано исключение - трассировка
   *                      исключения)
   */
    explicit ValidationFailure(const unsigned int failureId, const std::string& postfix = "", const std::string& prefix = "") noexcept
        : Failure(
              failureId,
              validationTitle,
              validationFailuresText.at(failureId),
              postfix,
              prefix),
          message(prefix + postfix)
    {
    }

    std::string getMessage() const
    {
        return message;
    }

private:
    std::string message;
};
} // namespace Exceptions
