#ifndef ABSTRACT_OPERATIONS_EXCEPTION_HPP
#define ABSTRACT_OPERATIONS_EXCEPTION_HPP
//==============================================================================
#include <ExceptionTools/ExceptionTools.hpp>
//==============================================================================
namespace AbstractOperations
{
//==============================================================================
  using namespace ExceptionTools;
//==============================================================================
/*
 *\brief Именованные коды генерируемых исключений
 */
enum AbstractOperationsEnumFailure
{
  /*!<
  * попытка выполнить над нулевой геометрией операцию, не определённую для неё
  */
  NullGeometryIncorrectOperation = SpecialFailure,                     //!< 100
  //!< Несоответсвующие типы геометрий (сущностей)
  IrrelevantGeometryType,                                   
  //!< Некорректная геометрия (сущность)
  IncorrectGeometry,
  //!< Недопустимый тип геометрий (сущности)
  InvalidGeometryType,
  //! Невозможно корректно выполнить операцию над геометрией (сущностью)
  OperationNotPerformedCorrect,
  //!< Попытка найти пересечение там, где его нет
  FakeIntersection,
  //!< поиск на отрезке точки, не принадлежащей ему
  PointNotInInterval, 
  //!< прямая через две совпадающие точки
  LineThroughOnePoint, 
  //!< потеря данных при сужении типа геометрии
  LossOfData
};
//==============================================================================
// !< название библиотеки для исключений
const std::string abstrLibTitle = "Core::AbstractOperations"; 
//==============================================================================
/*!
  * \brief Класс формирования исключений библиотеки AbstractOperations
  */
class AbstractOperationsFailure : public ExceptionTools::Failure
{
//==============================================================================
public:
  /*!
  * \brief Создание исключения.
  * \details Требуется только код, остальные параметры подставятся автоматически 
  * \param[in] failureId - именованный код исключения
  * \param[in] postfix - текст уточняющий/дополняющий исключение
  *                      (может использоваться для передачи текста исходного
  *                       исключения)
  * \param[in] prefix - текст, локализующий место возникновения исключения
  *                     (может использоваться для указания имени функции,
  *                      в которой локализовано исключение - трассировка
  *                      исключения)
  */
  explicit AbstractOperationsFailure(const unsigned int failureId,
                                     const std::string& postfix = "",
                                     const std::string& prefix = "" ) noexcept;
};
//==============================================================================
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_EXCEPTION_HPP