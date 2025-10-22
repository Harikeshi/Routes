#ifndef REACHABLEAREACALCULATOR_REACHABLE_AREA_HPP
#define REACHABLEAREACALCULATOR_REACHABLE_AREA_HPP
//==============================================================================
#include <ReachableAreaCalculator.hpp>

//! Inner
#include <Inner/Arcs.hpp>
#include <Inner/SubEnvironment.hpp>
#include <Inner/Visibility.hpp>
#include <Inner/GeometryEnvironment.hpp>
#include <Inner/Angle.hpp>
//! ReachableAreaCalculator
#include <Environment.hpp>
//! Point
#ifndef Q_MOC_RUN
#include <Point/Point2D.hpp>
#include <PrimaryEntities/GeometryEntities.hpp>
#endif
//! std
#include <memory>
#include <vector>
#include <set>
#include <list>
#include <limits>
#include <atomic>
//! Qt
#include <QObject>
#include <QMutex>
#include <QSemaphore>
#include <QRunnable>
#include <QElapsedTimer>
//==============================================================================
namespace ReachableAreaCalculator
{
//==============================================================================
struct CalculationPointInfo
{
  double   minPath;  //! минимальный путь от наблюдателя на текущий момент;
  uinttype taskId;   //! id актуального задания с центром в данной точке.
                     //! 0, если нет.
};
//==============================================================================
/*
* \brief Задание для расчёта
*/
class CalculationTask
{
//==============================================================================
public:
//==============================================================================
  /*
  * \brief Конструктор
  */
  CalculationTask()
  { }
//==============================================================================
  /*!
   * \brief Конструктор
   * \param[in] taskId         - id задания;
   * \param[in] parentId       - id задания, породившего данное;
   * \param[in] point          - точка расчёта;
   * \param[in] subEnvironment - суб-среда задания;
   * \param[in] currentPath    - путь до точки расчёта от центральной;
   * \param[in] maxPath        - максимальный путь от центральной точки;
   * \param[in] viewZoneBegin  - начало зоны видимости;
   * \param[in] viewZoneEnd    - конец зоны видимости.
   */
  CalculationTask(
    uinttype taskId,
    uinttype parentId,
    const EnvironmentPoint* point,
    std::shared_ptr<const GeometrySubEnvironment> subEnvironment,
    double currentPath,
    double maxPath,
    Angle viewZoneBegin,
    Angle viewZoneEnd) noexcept;
//==============================================================================
  /*
  * \brief Проверка корректности задания
  * \details Введено для случая получения задания из пустой очереди 
  * \        заданий.
  * \return true  - задание корректно;
  *         false - не корректно.
  */
  inline bool isValid() const 
  { 
    return mValid; 
  }
//==============================================================================
  /*
  * \brief Получение идентификатора задания
  * \return идентификатор задания
  */
  inline uinttype id() const 
  {
    return mTaskId; 
  }
//==============================================================================
  /*
  * \brief Получение id задания, породившего данное
  * \return id задания, породившего данное
  */
  inline uinttype parentId() const 
  { 
    return mParentTaskId; 
  }
//==============================================================================
  /*
  * \brief Получение точки расчёта
  * \return точка расчёта
  */
  inline const EnvironmentPoint* point() const 
  { 
    return mPoint; 
  }
//==============================================================================
  /*
  * \brief Получение максимальной дальности от центра
  * \return максимальная дальность от центра
  */
  inline double maxPath() const 
  {
    return mMaxPath; 
  }
//==============================================================================
  /*
  * \brief Получение пути до точки расчёта от центральной точки
  * \return путь до точки расчёта от центральной
  */
  inline double currentPath() const 
  { 
    return mCurrentPath; 
  }
//==============================================================================
  /*
  * \brief Получение начала зоны видимости при расчёте
  * \return начало зоны видимости
  */
  inline Angle viewZoneBegin() const
  { 
    return mViewZoneBegin; 
  }
//==============================================================================
  /*
  * \brief Получение конца зоны видимости при расчёте
  * \return конец зоны видимости
  */
  inline Angle viewZoneEnd() const
  { 
    return mViewZoneEnd; 
  }
//==============================================================================
  /*
  * \brief Получение суб-среды задания
  * \return суб-среда задания
  */
  inline std::shared_ptr<const GeometrySubEnvironment> subEnvironment() const
  {
    return mSubEnvironment;
  }
//==============================================================================
  /*
  * \brief Закрытие задания
  * \details С удалением суб-среды
  */
  inline void close() 
  { 
    mSubEnvironment.reset(); 
  }
//==============================================================================
private:
//==============================================================================
  uinttype                mTaskId = 0;         //! идентификатор задания;
  uinttype                mParentTaskId = 0;   //! id задания, породившего 
                                               //! данное;
  const EnvironmentPoint* mPoint = nullptr;    //! центральная точка;
  std::shared_ptr<const GeometrySubEnvironment> 
                          mSubEnvironment;     //! суб-среда;
  double                  mMaxPath = -1;       //! максимальная дальность 
                                               //! от центра;
  double                  mCurrentPath         //! путь до точки расчёта
    = std::numeric_limits<double>::infinity(); //! от центральной;                              
  Angle                   mViewZoneBegin;      //! начало зоны видимости;
  Angle                   mViewZoneEnd;        //! конец зоны видимости;
  bool                    mValid = false;      //! корректность задания.
  };
//==============================================================================
//==============================================================================
/*
* \brief Класс управления очередью заданий и данными, 
* \      используемыми при расчёте
*/
class InfoAndQueueManager
{
//==============================================================================
public:
//==============================================================================
  /*!<
   * \brief Конструктор
   * \param[in] environment - среда;
   * \param[in] arcManager  - объект управления дугами;
   * \param[in] center      - основная точка, относительно которой 
   *                          происходит построение области достижимости;
   * \param[in] maxPath     - максимальный путь, определяющий 
   *                          область достижимости.
   * \throws ReachableAreaFailure # InfoAndQueueManagerEnvironmentNullPointer
   */
  InfoAndQueueManager(
    const GeometryEnvironment* environment,
#ifdef WA_REACHABLE_AREA_ALL_POLYGONS
    std::vector<Polygon2D>& area,
#endif
    ArcManager& arcManager,
    const Point2D& center,
    double maxPath);
//==============================================================================
  /*
  * \brief Возвращение задания из очереди
  * \return задание из очереди
  */
  CalculationTask popTask();
//==============================================================================
  /*!<
   * \brief  applyResult - обработка результата расчёта
   * \param[in] result       - результат;
   * \param[in] taskId       - id задания, для которого получен результат;
   * \param[in] parentTaskId - id задания, породившего задание taskId;
   * \param[in] currentPath  - расстояние от центральной точки до точки расчёта;
   * \param[in] canterId     - точка расчёта;
   * \param[in] processSub   - суб-среда, в которой проводился расчёт.
   * \throws ReachableAreaFailure #
   *           InfoAndQueueManagerApplyResultForEmptySubEnvironment
   */
  void applyResult(
    const Visibility::ProcessResult& result,
    uinttype taskId,
    uinttype parentTaskId,
    double currentPath,
    uinttype centerId,
    std::shared_ptr<const GeometrySubEnvironment> processSub);
//==============================================================================
  /*
  * \brief Получение количества обработанных точек
  * \return количество обработанных точек
  */
  inline uinttype readyCount() const 
  { 
    return mReadyCount; 
  }
//==============================================================================
  /*
  * \brief Получение размера очереди заданий
  * \return размер очереди заданий
  */
  inline uinttype queueSize() const 
  { 
    return uinttype(mQueue.size()); 
  }
//==============================================================================
  /*
  * \brief Проверка пустоты очереди заданий
  * \return true  - очередь заданий пуста;
  *         false - не пуста.
  */
  inline bool isQueueEmty() const 
  { 
    return mQueue.empty(); 
  }
//==============================================================================
  /*
  * \brief Получение информации о достижимости точек по id
  * \details Элемент [0] игнорируется
  * \return информация о достижимости точек по id
  */
  inline const std::vector<bool>& pointsReachableInfo() const
  {
    return mPointsReachableInfo;
  }
//==============================================================================
  /*!
   * \brief touchedPolygons -- метод,
   * возвращающий множество id достижимых полигонов.
   * \return искомое множество id достижимых полигонов
   */
  inline const std::set<uinttype>& touchedPolygons() const noexcept
  {
    return mTouchedPolygons;
  }
//==============================================================================
private:
//==============================================================================
  const GeometryEnvironment* const  
                     mEnvironment;         //! глобальная среда;

#ifdef WA_REACHABLE_AREA_ALL_POLYGONS
  std::vector<Polygon>& 
                     mArea;                //! приемник полигонов видимости;
#endif

  ArcManager&        mArcManager;          //! объект управления дугами;

  std::vector<CalculationPointInfo> 
                     mInfo;                //! информация о точках среды;

  std::vector<bool>  mPointsReachableInfo; //! информация о достижимости 
                                           //! точек;

  std::set<uinttype> mTouchedPolygons;     //! достижимые полигоны;

  std::list<CalculationTask> 
                     mQueue;               //! очередь заданий;

  QMutex             mMutex;               //! мьютекс доступа к 
                                           //! разделяемым ресурсам;

  double             mMaxPath;             //! максимальная дальность;
  uinttype           mReadyCount;          //! количество обработанных 
                                           //! точек;
  EnvironmentPoint   mCenterEnvironment;   //! центральная точка;
  uinttype           mTaskIdCount;         //! счётчик идентификаторов 
                                           //! заданий;
  const double       mZonesCross = 1E-6;   //! перекрытие зон видимости.
};
//==============================================================================
//==============================================================================
/*
* \brief Класс, обрабатывающий задание в отдельном потоке
*/
class IterationRunnable : public QRunnable
{
//==============================================================================
public:
//==============================================================================
  /*
  * \brief  Конструктор
  * \param[in] task           - задание на обработку;
  * \param[in] manager        - менеджер;
  * \param[in] semaphore      - семафор, отпускаемый при завершении расчёта;
  * \param[in] segmentsNumber - количество сегментов, для представления 
  * \                           окружности.
  */
  IterationRunnable(
    CalculationTask& task, 
    InfoAndQueueManager& manager,
    QSemaphore& semaphore, 
    uinttype segmentsNumber) 
    : QRunnable()
    , mManager(manager)
    , mSemaphore(semaphore)
    , mTask(task)
    , mSegmentsNumber(segmentsNumber) 
  { }
//==============================================================================
  /*!<
   * \brief run -- основной метод, производящий обработку задания
   * \throws ReachableAreaFailure # IterationRunnableRunWithEmptySubEnvironment
   *         ReachableAreaFailure # IterationRunnableRunInvalidTask
   *         ReachableAreaFailure #
   *           IterationRunnableRunTaskWithNonPositiveMaxPath
   *         ReachableAreaFailure #
   *           IterationRunnableRunTaskWithCalculationPointNullPointer
   */
  void run();
//==============================================================================
private:
//==============================================================================
  InfoAndQueueManager& mManager;        //! менеджер;
  QSemaphore&          mSemaphore;      //! семафор, отпускаемый при 
                                        //! завершении расчёта;
  CalculationTask      mTask;           //! задание на обработку;
  uinttype             mSegmentsNumber; //! количество сегментов, для
                                        //! представления окружности.
 };
//==============================================================================
//==============================================================================
/*
* \brief Рассчёт области достижимости
*/
class ReachableAreaInner : public QObject
{
  Q_OBJECT
 //==============================================================================
public: 
//==============================================================================
  /*
  * \brief Конструктор
  * \param[in] environment - среда, в которой производится расчёт
  */
  ReachableAreaInner(
    const GeometryEnvironment* environment, 
    QObject* parent = 0)
    : QObject(parent)
    , mEnvironment(environment)
    , mState(Waiting)
  { }
//==============================================================================
  /*
  * \brief Запуск процесса построения области достижимости
  * \param[in]  center         - точка, относительно которой производится 
  * \                           расчёт;
  * \param[in]  maxPath        - ограничение области достижимости по 
  * \                           дальности;
  * \param[out] result         - результат расчёта;
  * \param[in]  segmentsNumber - количество точек, для представления 
  * \                           окружности;
  * \param[in] t hreadsCount   - количество потоков;
  * \return true  - удалось построить область достижимости;
  * \       false - не удалось.
  */
  bool calculate(
    const Point2D& center,
    double maxPath,
    ReachableAreaResult& result,
    unsigned int segmentsNumber, 
    int threadsCount = -1);
//==============================================================================
  /*
  * \brief Прерывание процесса вычисления
  */
  void abort();
//==============================================================================
#ifdef WA_REACHABLE_AREA_ALL_POLYGONS
  /*
  * \brief Получение области достижимости в виде набора полигонов
  * \details При вызове до вызова proceed возвращает пустой вектор
  * \return область достижимости в виде набора полигонов
  */
  inline const std::vector<Polygon2D>& allPolygons() const 
  { 
    return mArea; 
  }
#endif
//==============================================================================
signals:
//==============================================================================
  /*
  * \brief Отправление сигнала
  * \details
  * \ Высылается во время работы каждые mSignalsInterval миллисекунд.
  * \stage == 0 (StartCalculationStage): начало расчёта.
  * \stage == 1 (CalculationStage): расчёт.
  * \                  param1 - кол-во обработанных точек
  * \                  param2 - размер очереди
  * \stage == 2 (PolyarcsClearingStage): расчистка области склейки от
  * \заведомо ненужных дуг (поглощенных другими).
  * \                  param1 - кол-во обработанных дуг
  * \                  param2 - всего дуг
  * \stage == 3 (IntersectsDetectionStage): определение всех пересечений дуг.
  * \                  param1 - кол-во обработанных дуг
  * \                  param2 - всего дуг
  * \stage == 4 (PolyarcsMergeStage): объединение дуг по пересечениям.
  * \                  param1 - кол-во обработанных
  * \                  param2 - размер очереди
  * \stage == 5 (PolyarcsCheckStage): определение игнорируемых дуг.
  * \                  param1 - кол-во обработанных
  * \                  param2 - всего
  * \stage == 6 (BordersDetectionStage): сборка граничных контуров.
  * \                  param1 - кол-во обработанных дуг
  * \                  param2 - всего
  * \stage == 7 (HolesDetectionStage): добавление дырок.
  * \                  param1 - кол-во обработанных
  * \                  param2 - всего
  * \stage == 8 (ReadyStage): успешное завершение расчёта.
  * \stage == 9 (AbortedStage): принудительное завершение расчёта.
  */
  void processInfo(quint8 stage, quint64 param1, quint64 param2);
//==============================================================================
private:
//==============================================================================
  /*
  * \brief Текущее состояние
  */
  enum EState 
  { 
    Waiting,     //! готов к началу расчёта;
    Calculating, //! идёт расчёт;
    AbortRequest //! идёт расчёт, но он должен быть прерван.
  };
  const GeometryEnvironment* 
                         mEnvironment;      //! глобальная среда;
#ifdef WA_REACHABLE_AREA_ALL_POLYGONS
  std::vector<Polygon2D> mArea;             //! приемник полигонов видимости;
#endif
  EReachableAreaCalculationStage 
                         mPrevEmittedStage; //! значение стадии, переданное
                                            //!  при последней высылке 
                                            //! сигнала processInfo;
  QElapsedTimer          mTimer;            //! таймер для определения 
                                            //! интревалов высылки сигналов;
  std::atomic<EState>    mState;            //! текущее стостояние;
  ArcManager*            mArcManager        //! на время склейки здесь лежит
                          = nullptr;        //! указатель на локальный 
                                            //! (в calculate) менеджер дуг 
                                            //! для обеспечения возможности 
                                            //! прервать его работу;
  const unsigned int     mSignalsInterval   //! Время между высылаемыми при
                         = 100;             //! расчёте сигналами processInfo.
//==============================================================================
private slots:
//==============================================================================
  void mergeInfo(ArcManager::EMergeStage stage, uinttype p1, uinttype p2);
};
//==============================================================================
} //! ReachableAreaCalculator
//==============================================================================
#endif //! REACHABLEAREACALCULATOR_REACHABLE_AREA_HPP