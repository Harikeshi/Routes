#pragma once

/*! Core */
#include <ExceptionTools/ExceptionTools.hpp>

namespace Exceptions {

/**
 * @brief Именованные коды генерируемых исключений при выполнении алгоритмов(схем)
 *
 */
enum AlgorithmFailureEnum
{
    /**
     * @brief Коды ошибок исключений алгоритмов.
     *
     */
    DoorPointIncorrect = 200,                  //!< Точка входа и/или выхода не принадлежит границе района
    ShortSearchTime = DoorPointIncorrect + 10, //!< За установленное время поиска невозможно пройти по кратчайшему маршруту
    EntranceCourseNotFound,                    //!< Не определен курс при входе в район поиска
    IntersectionNotFound,                      //!< Не определено пересечение с границей района для нового курса
    HighRelativeBearing,                       //!< Курсовой угол больше критического
    TryDifferentScheme = 230                   //!< Попробуйте другую схему
};

/**
 * @brief Список кодов-сообщений исключений алгоритмов
 *
 */
const std::map<unsigned int, std::string> algorithmFailuresText = {
    {DoorPointIncorrect, "<DoorPointIncorrect>: Точка входа и/или выхода не принадлежит границе района."},
    {ShortSearchTime, "<ShortSearchTime>: За установленное время поиска невозможно пройти по кратчайшему маршруту."},
    {EntranceCourseNotFound, "<EntranceCourseNotFound>: Не удалось найти курс при входе в район поиска."},
    {IntersectionNotFound, "<IntersectionNotFound>: Не удалось найти пересечение с границей района для нового курса."},
    {HighRelativeBearing, "<HighRelativeBearing>: Относительный курсовой угол больше критического."},
    {TryDifferentScheme, "<TryDifferentScheme>: Схема не является оптимальной, рекомендуется выбрать другую."}};

const std::string algorithmErrorTitle = "Algorithm Error"; // Префикс исключений Algorithm Error.

/**
 * @brief Класс исключений генерируемых при выполнении алгоритмов(схем)
 *
 */
class AlgorithmFailure : public ExceptionTools::Failure
{
public:
    explicit AlgorithmFailure(const unsigned int failureId, const std::string& postfix = "", const std::string& prefix = "") noexcept
        : Failure(failureId, algorithmErrorTitle, algorithmFailuresText.at(failureId), postfix, prefix)
    {
    }
};

} // namespace Exceptions
