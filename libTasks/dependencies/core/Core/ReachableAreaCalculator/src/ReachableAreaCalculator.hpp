#ifndef REACHABLEAREACALCULATOR_REACHABLE_AREA_CALCULATOR_HPP
#define REACHABLEAREACALCULATOR_REACHABLE_AREA_CALCULATOR_HPP
//==============================================================================
#ifndef Q_MOC_RUN
//! ReachableAreaCalculator
#include <Environment.hpp>
//! Point
#include <Point/Point2D.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
#endif

//! Qt
#include <QObject>
//==============================================================================
namespace ReachableAreaCalculator
{
class ReachableAreaInner;
//==============================================================================
/*
* \brief Результат расчёта
*/
struct ReachableAreaResult
{
  Polygon2D area;                             //! область достижимости;

  std::vector<std::vector<Point2D> > borders; //! линии, представляющие 
                                              //! участки границы области 
                                              //! достижимости, не 
                                              //! являющиеся сторонами 
                                              //! полигонов среды(дуги, 
                                              //! представленные отрезками).
  /*!
   * \brief clear() -- функция, очищающая объект.
   */
  void clear() noexcept;
};
//==============================================================================
/*
* \brief Стадии процесса расчёта области достижимости
*/
enum EReachableAreaCalculationStage 
{
  StartCalculationStage,     //! начало расчёта;
  CalculationStage,          //! расчёт;
  PolyarcsClearingStage = 2, //! расчистка области склейки от заведомо
                             //! ненужных дуг(поглощенных другими);
  IntersectsDetectionStage,  //! определение всех пересечений дуг;
  PolyarcsMergeStage,        //! объединение дуг по пересечениям;
  PolyarcsCheckStage,        //! определение игнорируемых дуг;
  BordersDetectionStage,     //! сборка граничных контуров;
  HolesDetectionStage,       //! добавление дырок;
  ReadyStage,                //! успешное завершение расчёта;
  AbortedStage               //! принудительное завершение расчёта.
};
//==============================================================================
/*
* \brief Рассчёт области достижимости
*/
class ReachableArea : public QObject
{
//==============================================================================
  Q_OBJECT
public: 
//==============================================================================
  /*
  * \brief Конструктор
  */
  ReachableArea(QObject * parent = 0) 
    : QObject(parent) 
  { }
//==============================================================================
  /*
  * \brief Расчёт области достижимости
  * \param[in] environment    - среда, в которой производится расчёт;
  * \param[in] center         - точка, относительно которой производится 
  * \                           расчёт;
  * \param[in] maxPath        - ограничение области достижимости по 
  * \                           дальности;
  * \param[out] result        - результат расчёта;
  * \param[in] segmentsNumber - количество сегментов, для представления 
  * \                           окружности;
  * \param[in] threadsCount   - количество потоков;
  * \return true  - удалось построить область достижимости;
  *         false - не удалось.
  */
  bool calculate(
    const Environment& environment,
    const Point2D& center,
    double maxPath,
    ReachableAreaResult& result,
    unsigned int segmentsNumber,
    int threadsCount = -1);
//==============================================================================
  /*
  * \brief Прерывание процесса вычисления
  * \details Потокобезопасно
  */
  void abort();
//==============================================================================
signals:
//==============================================================================
  /*
  * \brief Отправление сигнала
  * \details
  * \stage == 0 (StartCalculationStage): начало расчёта.
  * \stage == 1 (CalculationStage): расчёт.
  * \                  param1 - кол-во обработанных точек;
  * \                  param2 - размер очереди.
  * \stage == 2 (PolyarcsClearingStage): расчистка области склейки от 
  * \заведомо ненужных дуг (поглощенных другими).
  * \                  param1 - кол-во обработанных дуг;
  * \                  param2 - всего дуг.
  * \stage == 3 (IntersectsDetectionStage): определение всех пересечений дуг.
  * \                  param1 - кол-во обработанных дуг;
  * \                  param2 - всего дуг.
  * \stage == 4 (PolyarcsMergeStage): объединение дуг по пересечениям.
  * \                  param1 - кол-во обработанных;
  * \                  param2 - размер очереди.
  * \stage == 5 (PolyarcsCheckStage): определение игнорируемых дуг.
  * \                  param1 - кол-во обработанных;
  * \                  param2 - всего.
  * \stage == 6 (BordersDetectionStage): сборка граничных контуров.
  * \                  param1 - кол-во обработанных дуг;
  * \                  param2 - всего.
  * \stage == 7 (HolesDetectionStage): добавление дырок.
  * \                  param1 - кол-во обработанных;
  * \                  param2 - всего.
  * \stage == 8 (ReadyStage): успешное завершение расчёта.
  * \stage == 9 (AbortedStage): принудительное завершение расчёта.
  */
  void processInfo(quint8 stage, quint64 param1, quint64 param2);
//==============================================================================
private:
//==============================================================================
  ReachableAreaInner* mInnerCalculator = nullptr;
};
//==============================================================================
} //! ReachableAreaCalculator
//==============================================================================
#endif //! REACHABLEAREACALCULATOR_REACHABLE_AREA_CALCULATOR_HPP