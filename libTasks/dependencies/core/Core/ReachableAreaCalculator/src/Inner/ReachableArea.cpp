#include <Inner/ReachableArea.hpp>

#include <Inner/Arcs.hpp>
#include <Inner/Visibility.hpp>
//! ReachableAreaCalculator
#include <ReachableArea/ReachableAreaException.hpp>
//! Qt
#include <QThreadPool>
#include <QElapsedTimer>
//==============================================================================
using namespace ReachableArea;
//==============================================================================
namespace ReachableAreaCalculator
{
//==============================================================================
  CalculationTask::
  CalculationTask(
    uinttype taskId,
    uinttype parentId,
    const EnvironmentPoint* point,
    std::shared_ptr<const GeometrySubEnvironment> subEnvironment,
    double currentPath,
    double maxPath,
    Angle viewZoneBegin,
    Angle viewZoneEnd) noexcept
    : mTaskId(taskId)
    , mParentTaskId(parentId)
    , mPoint(point)
    , mSubEnvironment(subEnvironment)
    , mMaxPath(maxPath)
    , mCurrentPath(currentPath)
    , mViewZoneBegin(viewZoneBegin)
    , mViewZoneEnd(viewZoneEnd)
    , mValid(true)
  { }
//==============================================================================
  InfoAndQueueManager::
  InfoAndQueueManager(
    const GeometryEnvironment* environment,
#ifdef WA_REACHABLE_AREA_ALL_POLYGONS
    std::vector<Polygon>& area,
#endif
    ArcManager& arcManager, 
    const Point2D& center, 
    double maxPath) 
    : mEnvironment(environment) 
#ifdef WA_REACHABLE_AREA_ALL_POLYGONS
    , mArea(area)
#endif
    , mArcManager(arcManager)
    , mMaxPath(maxPath)
    , mReadyCount(0U)
    , mCenterEnvironment(mEnvironment, 0U, 0U, center)
    , mTaskIdCount(1U)
  {
    if (!environment)
      throw ReachableAreaFailure(InfoAndQueueManagerEnvironmentNullPointer);

    // инициализация информации о точках
    mPointsReachableInfo.assign(mEnvironment->pointsCount() + 1, false);
    mInfo.assign(mEnvironment->pointsCount() + 1,
      CalculationPointInfo{ std::numeric_limits<double>::infinity(), 0 });

    // в info[0] лежит информация о центральной точке,
    // для остальных точек индекс равен идентификатору (строго говоря, 
    // для центральной точки это также выполняется, т.к. её id == 0)
    mInfo[0].minPath = 0; 

    // обрезалка
    CircleSubEnvironmentCutter cutter; 
    cutter.set(center, maxPath);

    // самое первое задание
    CalculationTask task(
      mTaskIdCount++,
      0, // parentId
      &mCenterEnvironment,
      std::shared_ptr<const GeometrySubEnvironment>(
        cutter.generateCuttedSubEnvironment(mEnvironment)),
      0,//текущий путь от центральной точки нулевой
      mMaxPath,
      Angle(0), Angle(0) // без ограничения зоны видимости
      );

    // инициализация очереди заданий
    mQueue.push_back(task);
    mInfo[0].taskId = task.id();
  }
//==============================================================================
  CalculationTask InfoAndQueueManager::popTask()
  {
    CalculationTask result;

    mMutex.lock();

    if (mQueue.empty())
    {
      mMutex.unlock();
      return result;
    }

    while (!mQueue.empty())
    {
      CalculationTask& task = mQueue.front();

      // задания, которые были поставлены в очередь,
      //  но впоследствии были заменены иными,
      // либо определены как не требующиеся к обработке
      if (mInfo[task.point()->id()].taskId != task.id()) 
      {
        task.close();
        mQueue.pop_front();
      }
      else
      {
        result = task;
        mInfo[task.point()->id()].taskId = 0;
        mQueue.pop_front();
        break;
      }
    }

    mMutex.unlock();

    return result;
  }
//==============================================================================
  void InfoAndQueueManager::applyResult(
    const Visibility::ProcessResult& result,
    uinttype taskId,
    uinttype parentTaskId,
    double currentPath,
    uinttype centerId,
    std::shared_ptr<const GeometrySubEnvironment> processSub)
  {
    if (!processSub.get())
      throw ReachableAreaFailure(
                          InfoAndQueueManagerApplyResultForEmptySubEnvironment);

    // если для данной точки за время проведения расчёта был получен 
    // лучший результат, то расчёт проводился зря и обработка его 
    // результатов не требуется.
    if (mInfo[centerId].taskId ||
      (mInfo[centerId].minPath 
        < (currentPath - std::numeric_limits<double>::epsilon())))
    {
      mMutex.lock();
      ++mReadyCount;
      mMutex.unlock();
      return;
    }

    // каких полигонов коснулся данный результат расчёта
    std::set<uinttype> resTouchedPolygons; 

    const Point2D& center 
      = centerId ? mEnvironment->getPoint(centerId) : mCenterEnvironment;
    mMutex.lock();

    if (centerId)
      resTouchedPolygons.insert(mEnvironment->getPoint(centerId).polygonId());

    // перебор видимых точек
    for (uinttype pointId : result.absorptionPoints) 
    {
      const EnvironmentPoint point = mEnvironment->getPoint(pointId);
      double minPathNew = currentPath + Geometry::distance(center, point);

      // при улучшении необходимо снять точку из очереди заданий. 
      // Для этого обнуляется её taskId
      if (minPathNew < mInfo[pointId].minPath)
      {
        mInfo[pointId].minPath = minPathNew;
        mInfo[pointId].taskId = 0;
        mPointsReachableInfo[pointId] = true;
        resTouchedPolygons.insert(point.polygonId());
      }
    }

    // Кладём часный резульат в общий
    mTouchedPolygons.insert(
      resTouchedPolygons.begin(), resTouchedPolygons.end());

    for (
      const std::pair<uinttype, double>& point : result.unvisibleReachablePoints)
    {
      double minPathNew = currentPath + point.second;

      // при улучшении необходимо снять точку из очереди заданий. 
      // Для этого обнуляется её taskId
      if (minPathNew < mInfo[point.first].minPath)
      {
        mInfo[point.first].minPath = minPathNew;
        mInfo[point.first].taskId = 0;
        mPointsReachableInfo[point.first] = true;
      }
    }

    // список новых заданий
    std::list<CalculationTask> tasks; 

    // перебор точек преломления
    for (uinttype pointId : result.refractionPoints)
    {
      double minPathNew  = currentPath 
        + Geometry::distance(center, mEnvironment->getPoint(pointId));

      if (minPathNew < mInfo[pointId].minPath)
      {
        if (mMaxPath > minPathNew)
        {
          mInfo[pointId].minPath = minPathNew;
          mPointsReachableInfo[pointId] = true;

          const EnvironmentPoint& refractionPoint 
            = mEnvironment->getPoint(pointId);
          const EnvironmentPolygon& polygon =
            mEnvironment->getPolygon(refractionPoint.polygonId());

          // Определить предыдущую и следующую точки
          uinttype prevId = polygon.prevPointId(pointId);
          uinttype nextId = polygon.nextPointId(pointId);
          const EnvironmentPoint& prevPoint = mEnvironment->getPoint(prevId);
          const EnvironmentPoint& nextPoint = mEnvironment->getPoint(nextId);

          // Определить углы на предыдущую и следующую точки
          Angle fromRefToPrev
            = Geometry::lineAngularity(refractionPoint, prevPoint).get();
          Angle fromRefToNext
            = Geometry::lineAngularity(refractionPoint, nextPoint).get();

          // Определить угол биссектрисы вершины точки преломления
          Angle prevToNextBisector(fromRefToPrev.value() +
            fromRefToPrev.deltaAngle(fromRefToNext) / 2.0);

          // Определить угол от точки расчёта на точку преломления
          Angle fromCenterToRef
            = Geometry::lineAngularity(center, refractionPoint).get();

          // Определить ограничения зоны видимости следующего задания
          Angle viewZoneBegin;
          Angle viewZoneEnd;
          bool bisectorIsViewZoneBegin =
            prevToNextBisector.deltaAngle(fromCenterToRef) > 0;

          if (bisectorIsViewZoneBegin)
          {
            viewZoneBegin =
              Angle(prevToNextBisector.value()
                + prevToNextBisector.deltaAngleCounterclockwise(fromRefToNext)
                / 2.0);

            viewZoneEnd =
              Angle(fromCenterToRef.value() + mZonesCross);
          }
          else
          {
            viewZoneBegin =
              Angle(fromCenterToRef.value() - mZonesCross);

            viewZoneEnd =
              Angle(prevToNextBisector.value()
                - prevToNextBisector.deltaAngleCounterclockwise(fromRefToNext)
                / 2.0);
          }

          // Сформировать новое задание и поставить его в очередь
          CalculationTask task(mTaskIdCount++,
            taskId, // parentId 
            &(mEnvironment->getPoint(pointId)),
            processSub, minPathNew, mMaxPath - minPathNew,
            viewZoneBegin, viewZoneEnd);

          // предыдущие задания для данной
          // точки аннулируются
          mInfo[pointId].taskId = task.id();
          tasks.push_back(task);
        }
      }
    }

    // Функция сортировки заданий в очереди
    auto sortLambda =
      [](const CalculationTask& task1, const CalculationTask& task2)
    {return task1.maxPath() > task2.maxPath(); };

    // сортировка новых заданий
    tasks.sort(sortLambda);

    // слияние новых заданий с очередью
    // Можно сразу класть задание в очередь с последующей сортировкой,
    // но нетрудно показать, что сортировка новых заданий с последующим
    // слиянием с существующей очередью выполняется быстрее
    mQueue.merge(tasks, sortLambda);

#ifdef WA_REACHABLE_AREA_ALL_POLYGONS
    // сохранение полигона видимости
    if (!result.area.empty())
      mArea.push_back(result.area); 
#endif

    // запихнуть полученные дуги в менеджер дуг
    if (!result.arcs.empty())
    {
      std::vector<Arc> arcs = result.arcs;

      for (Arc& arc : arcs)
        arc.setTaskInfo(taskId, parentTaskId);

      mArcManager.push(arcs);
    }

    // счётчик обработанных точек
    ++mReadyCount; 
    mMutex.unlock();
  }
//==============================================================================
  void IterationRunnable::run()
  {
    if (!mTask.subEnvironment().get())
      throw ReachableAreaFailure(IterationRunnableRunWithEmptySubEnvironment);
    if (!mTask.isValid())
      throw ReachableAreaFailure(IterationRunnableRunInvalidTask);
    if (mTask.maxPath() <= 0.)
      throw ReachableAreaFailure(
                                IterationRunnableRunTaskWithNonPositiveMaxPath);
    if (!mTask.point())
      throw ReachableAreaFailure(
                      IterationRunnableRunTaskWithCalculationPointNullPointer);

    CircleSubEnvironmentCutter cutter;
    cutter.set(*mTask.point(), mTask.maxPath());

    // выполнение обрезки суб-среды до новой суб-среды
    std::shared_ptr<const GeometrySubEnvironment> subEnvironment
      (cutter.generateCuttedSubEnvironment(mTask.subEnvironment().get()));
    Visibility::ProcessResult result;

    // обработка новой суб-среды
    Visibility::process(subEnvironment.get(), mTask.point(), mTask.maxPath(),
      mTask.viewZoneBegin().value(), mTask.viewZoneEnd().value(),
      Visibility::AddUnvisible | Visibility::ResRefractions |
      Visibility::ResAbsorptions | Visibility::ResUnvisibles |
      Visibility::ResArcs
#ifdef WA_REACHABLE_AREA_ALL_POLYGONS
      | Visibility::ResArea
#endif
      , result, mSegmentsNumber);

    // обработка результата
    mManager.applyResult(
      result,
      mTask.id(),
      mTask.parentId(),
      mTask.currentPath(),
      mTask.point()->id(),
      subEnvironment);

    mTask.close(); // завершение задания
    mSemaphore.release(); // ресурс (поток) освободился
  }
//==============================================================================
  bool ReachableAreaInner::calculate(
    const Point2D& center, double maxPath,
    ReachableAreaResult& result, unsigned int segmentsNumber, int threadsCount)
  {
    // попытка нового расчёта при незавершенном старом
    // или расчёт с некорректным ограничением
    if (mState != Waiting || maxPath <= 0)
      return false;

    mState = Calculating;

    emit processInfo(StartCalculationStage, 0, 0);

    // менеджер дуг
    ArcManager arcManager(*mEnvironment);

#ifdef WA_REACHABLE_AREA_ALL_POLYGONS
    // очистка предыдущих полигонов видимости
    mArea.clear();
#endif

    InfoAndQueueManager manager(mEnvironment,
#ifdef WA_REACHABLE_AREA_ALL_POLYGONS
      mArea,
#endif 
      arcManager, center, maxPath);

    // количество используемых потоков
    unsigned int maxRunnablesCount =
      (threadsCount < 1 ||
        threadsCount > QThreadPool::globalInstance()->maxThreadCount()) ?
      QThreadPool::globalInstance()->maxThreadCount() : threadsCount;

    // семафор, определяющий возможность запуска задания на расчёт
    // в отдельном потоке
    QSemaphore semaphore(maxRunnablesCount);

    emit processInfo(CalculationStage, manager.readyCount(), manager.queueSize());

    mTimer.start(); // сброс таймера

    do
    {
      // нужно прервать выполнение
      if (mState == AbortRequest)
        break;

      // очередь заданий не пуста
      if (!manager.isQueueEmty())
      {
        // пора высылать сигнал
        if (mTimer.elapsed() >= mSignalsInterval)
        {
          emit processInfo(CalculationStage,
            manager.readyCount(), manager.queueSize());

          // сброс таймера
          mTimer.start();
        }

        // ждать свободный поток
        semaphore.acquire();
        // получить задание
        CalculationTask task = manager.popTask();

        if (task.isValid())
        {
          IterationRunnable* runnable =
            new IterationRunnable(task, manager, semaphore, segmentsNumber);
          runnable->setAutoDelete(true);

          // запустить задание на обработку в отдельном потоке
          QThreadPool::globalInstance()->start(runnable);
        }
        else
          semaphore.release();
      }

      // признак завершения расчёта - очередь заданий пуста
      // и нет выполняющихся потоков
    } while (static_cast<unsigned int>(semaphore.available())
               != maxRunnablesCount
             || !manager.isQueueEmty());

    // если расчёт идёт нормально (не был прерван)
    if (mState == Calculating)
    {
      emit processInfo(CalculationStage,
        manager.readyCount(), manager.queueSize());

      mPrevEmittedStage = CalculationStage;
    }

    // если расчёт был прерван
    if (mState == AbortRequest)
    {
      // ждать, когда все потоки завершатся
      while (static_cast<unsigned int>(semaphore.available())
             != maxRunnablesCount);
      emit processInfo(AbortedStage, 0, 0);
      mState = Waiting;
      return false;
    }

    QObject::connect(
      &arcManager,
      SIGNAL(mergeInfo(ArcManager::EMergeStage, uinttype, uinttype)),
      this,
      SLOT(mergeInfo(ArcManager::EMergeStage, uinttype, uinttype)));

    // сохраняем указатель для обработки abort()
    mArcManager = &arcManager;
    // склеиваем дуги, выделяя контур зоны достижимости
    arcManager.merge(manager.touchedPolygons(), result, segmentsNumber);
    // чистим сохранённый указатель
    mArcManager = nullptr;

    // если было прервано
    if (mState == AbortRequest)
    {
      emit processInfo(AbortedStage, 0, 0);
      mState = Waiting;
      return false;
    }

    emit processInfo(ReadyStage, 0, 0);
    mState = Waiting;

    return true;
  }
//==============================================================================
  void ReachableAreaInner::abort()
  {
    if (mState == Calculating)
    {
      mState = AbortRequest;

      if (mArcManager)
        mArcManager->abortMerge();
    }
  }
//==============================================================================
  void ReachableAreaInner::mergeInfo(ArcManager::EMergeStage stage,
    uinttype p1, uinttype p2)
  {
    // пришло время отправлять сигнал или изменилась стадия расчёта
    if (mTimer.elapsed() >= mSignalsInterval ||
      mPrevEmittedStage != EReachableAreaCalculationStage(stage))
    {
      // Замечание: ArcManager::EMergeStage и EReachableAreaCalculationStage
      // на соответствующих стадиях имеют одинаковые значения
      emit processInfo(EReachableAreaCalculationStage(stage), p1, p2);
      mPrevEmittedStage = EReachableAreaCalculationStage(stage);
      mTimer.start();
    }
  }
//==============================================================================

//==============================================================================
} // !ReachableAreaCalculator
//==============================================================================