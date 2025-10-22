#include <AbstractOperations/AbstractOperationsException.hpp>
//==============================================================================
namespace AbstractOperations
{
//==============================================================================
/*
 *\brief Соотношение именованных кодов и текстов ошибок генерируемых исключений
 */
const std::map<unsigned int, std::string> AbstractOpFailuresText =
{ 
  INCLUDE_STANDART_FAILURE_IN_MAP

  {
    NullGeometryIncorrectOperation,
    "<NullGeometryIncorrectOperation>: Попытка выполнить для нулевой геометрии операцию, не определённую для неё"
  },
  {
    IrrelevantGeometryType,
    "<IrrelevantGeometryType>: Типы геометрий (сущностей) не соответствуют"
  "друг другу"
  },
  {
    IncorrectGeometry,
    "<IncorrectGeometry>: Некорректная геометрия (сущность)"
  },
  {
    InvalidGeometryType,
    "<InvalidGeometryType>: Недопустимый тип геометрии (сущности) для операции"
  },
  {
    OperationNotPerformedCorrect,
    "<OperationNotPerformedCorrect>: Невозможно корректно выполнить операцию "
  "над заданной геометрией (сущностью)."
  },
  { FakeIntersection,
  "<FakeIntersection>: Поиск простого пересечения там, где его нет"
  },
  {
    PointNotInInterval,
    "<PointNotInInterval>: Поиск на отрезке точки, не принадлежащей ему"
  },
  {
    LineThroughOnePoint,
    "<LineThroughOnePoint>: Построение прямой через две совпадающие точки"
  },
  {
    LossOfData, 
    "<LossOfData>: Потеря данных при сужении типа геометрии"
  }

};
//==============================================================================
AbstractOperationsFailure::AbstractOperationsFailure(const unsigned int failureId,
                                     const std::string& postfix /*= ""*/,
                                     const std::string& prefix /*= ""*/ ) noexcept
: Failure ( 
           failureId, 
           abstrLibTitle, 
           AbstractOpFailuresText.at(failureId),
           postfix,
           prefix)
{
}
//==============================================================================
} //! AbstractOperations
//==============================================================================
