#pragma once

#include <QComboBox>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPushButton>
#include <QStackedLayout>
#include <QToolTip>
#include <QWidget>

#include "Project/Models/Report.hpp"
#include "Project/Models/Request.hpp"
#include "Project/Scene/ActorType.hpp"
#include "TimeWidget.hpp"

#include "../Scene/Entities/CoordinateSystem.hpp"
#include "../Scene/Entities/Limits.hpp"
#include "../Scene/Entities/Routes.hpp"

#include "../Scene/Objects/GridObject.hpp"
#include "../Scene/Objects/PathWidget.hpp"
#include "../Scene/Objects/RouteState.hpp"
#include "../Scene/Objects/TargetObject.hpp"

#include "../Scene/Actors/InRegion.hpp"
#include "../Scene/Actors/Spiral.hpp"

namespace Widgets {
/*!
     * Класс отображения сцены.
     */
class SceneWidget final : public QWidget
{
    using CoordinateSystem = Scene::Entities::CoordinateSystem;
    using Limits = Scene::Entities::Limits;
    using Routes = Scene::Entities::Routes;
    using Targets = Scene::Objects::TargetObject;
    using PathWidget = Scene::Objects::PathWidget;

    using TimeWidget = Widgets::TimeWidget; // Перевести в QObject

    using Grid = Scene::Objects::GridObject;
    using Actor = Scene::Actor;
    using Objects = Scene::Objects::Objects;

    using Request = Models::Request;
    using Report = Models::Report;

    using InRegionScene = Scene::Actors::InRegionScene;

    Q_OBJECT

    enum ActorType
    {
        Zigzag,
        Shift,

        Straight,
        Spiral
    };

    const std::map<ActorType, QString> ActorTypeName{
        {ActorType::Zigzag, "Зигзаг"},
        {ActorType::Shift, "Сдвиг"},
        {ActorType::Straight, "Гребенка"},
        {ActorType::Spiral, "Спираль"}};

    std::map<ActorType, std::function<Scene::Actor*()>> actors{
        {ActorType::Zigzag, [this]() { target->setState(new Scene::Objects::WithOutDrawState());return new Scene::Actors::InRegionScene(); }},
        {ActorType::Shift, []() { return new Scene::Actors::InRegionScene(); }},
        {ActorType::Straight, []() { return new Scene::Actors::InRegionScene(); }},
        {ActorType::Spiral, [this]() { target->setState(new Scene::Objects::CurrentDrawState());return new Scene::Actors::Spiral(); }}};

protected:
    // TODO: Добавляем    comboBox = new QComboBox(this);
    //         comboBox->addItem("Scheme 1");
    //         comboBox->addItem("Scheme 2");
    //        comboBox->setFixedWidth(200);
    // и Кнопку вниз
    // Widgets
    // TODO: Вверху Справа
    QComboBox* actorChoose;

    // TODO: Внизу справа
    QPushButton* metricChoose;
    TimeWidget* timeWidget;

    Grid* grid; // Сетка

    CoordinateSystem cs; // Система координат
    Limits limits;       // Крайние значения по осям

    // Элементы отрисовки цели.
    Routes* routes_;
    Targets* target;

    PathWidget* targetPath;
    Actor* actor;

    bool drawing; // Разрешить отрисовку цели

    double speedMultiplier{1}; // Множитель скорости

    // time block
    double fullTime{0}; //! Общее время схемы. Принимается максимальное время из маршрутов.
    double currentTime{0};
    double targetStartTime{0};

    QTimer* timer;

    const double margin = 5.; // Отступы от каждой стороны в процентах
    const double pointPercent = 0.005;

    // Расположение осей
    // TODO: Оси будут меняться (X-Y), (Ш-Д)
    QPair<QString, QString> axies{"X", "Y"};

public slots:

public:
    explicit SceneWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        targetPath = new PathWidget(this);

        QVBoxLayout* mainLayout = new QVBoxLayout(this);

        mainLayout->setContentsMargins(0, 0, 0, 0); // Убираем отступы по краям
        mainLayout->setSpacing(0);                  // Убираем промежутки

        // Верхняя часть
        QWidget* topWidget = new QWidget(this);
        QHBoxLayout* topLayout = new QHBoxLayout(topWidget);
        topLayout->setContentsMargins(0, 10, 10, 0); // Отступы: слева, сверху, справа, снизу

        // Добавляем растягивающееся пространство слева
        topLayout->addStretch();

        // TODO: Все же отдельный виджет
        actorChoose = new QComboBox(topWidget);

        //! Инициализация Выбора Actor.
        for (const auto& act : ActorTypeName)
            actorChoose->addItem(act.second, static_cast<int>(act.first));

        connect(actorChoose, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int type) {
            qDebug() << "Выбран: " << type;
            changeActor(static_cast<ActorType>(type));
        });

        actorChoose->setFixedSize(80, 30);

        // actorChoose->setEnabled(false);

        topLayout->addWidget(actorChoose);

        mainLayout->addWidget(topWidget);

        // Центральная часть
        QWidget* centerWidget = new QWidget(this);
        centerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        mainLayout->addWidget(centerWidget, 1);

        // Нижняя часть
        QWidget* bottomWidget = new QWidget(this);
        QHBoxLayout* bottomLayout = new QHBoxLayout(bottomWidget);
        bottomLayout->setContentsMargins(0, 0, 10, 10);

        // Добавляем растягивающееся пространство слева
        bottomLayout->addStretch();

        metricChoose = new QPushButton("metric", bottomWidget);
        metricChoose->setFixedSize(80, 30);
        metricChoose->setFlat(true); // Убираем стандартное оформление

        // Виджет в правом нижнем углу
        bottomLayout->addWidget(metricChoose);
        mainLayout->addWidget(bottomWidget);

        routes_ = new Routes(this);
        target = new Targets(this);
        target->setState(new Scene::Objects::WithOutDrawState());

        grid = new Grid(this);
        timer = new QTimer(this);

        // TODO: переименовать
        actor = new InRegionScene(this);

        cs = CoordinateSystem();

        drawing = false;
        speedMultiplier = 1;

        this->resize(800, 800);

        // Чтобы не было диких цифр при загрузке
        limitesToRect();

        // connect(timer, &QTimer::timeout, this, &SceneWidget::updateDrawing);
        connect(timer, &QTimer::timeout, this, &SceneWidget::moveFromTimer);

        connect(routes_, &Routes::complete, this, &SceneWidget::stop);
        connect(routes_, &Routes::sendIndexCurrentPositionSpeed, this, &SceneWidget::sendObjectInformation);

        // TimeWidget
        timeWidget = new TimeWidget(this);
        connect(this, &SceneWidget::sendCurrentTime, timeWidget, &TimeWidget::updateTime);

        // Размещение Времени и множителя на сцене.

        QWidget* upperWidget = new QWidget(this);
        upperWidget->setGeometry(10, 10, 200, 70);                   // Отступы, ширина и высота
        upperWidget->setAttribute(Qt::WA_TransparentForMouseEvents); // Игнорировать события мыши
        upperWidget->show();
        upperWidget->setStyleSheet("background: transparent;"); // Прозрачный фон

        QVBoxLayout* sceneLayout = new QVBoxLayout(upperWidget);

        sceneLayout->addWidget(timeWidget);

        QStackedLayout* stackedLayout = new QStackedLayout(this);

        stackedLayout->addWidget(upperWidget);
        stackedLayout->setCurrentIndex(1); // overlayWidget поверх
    }

    ~SceneWidget() = default;

signals:

    void sendMessage(const QString&); // Оповещение о пересечении например
    void sendDrawing(const bool&);

    void sendIndexCurrentPositionSpeed(size_t, const QPointF&, double);

    void sendTargetSpeed(double); // При изменения значения скорости.

    void sendCurrentTime(double time);

    void sendFullTime(double time);

    void sceneStarted();
    void sceneStopped();
    void scenePaused();
    void sceneReseted();

public:
    int getActorType() const
    {
        return actorChoose->currentIndex();
    }

    void changeActor(ActorType type)
    {
        // 1. Сброс сцены.
        // 2. Назначение актера.
        this->reset();

        actor = actors[type]();
    }

public:
    /*!
     * Метод сброса сцены к стартовому состоянию.
     */
    void reset()
    {
        //!
        //! Обнулить все динамические объекты.
        timeWidget->reset();

        //! Прогрузятся после загрузки Report.
        //! Сетка Остается.
        //! Система координат Остается.
        //! Крайние значения по осям Остается.
        //! Расположение Осей Остается.

        routes_->reset();
        target->reset();
        target->setState(new Scene::Objects::WithOutDrawState());

        targetPath->reset();
        actor->reset();

        setDrawing(false); // Запретить

        speedMultiplier = 1; // Множитель скорости

        // time block
        fullTime = 0; //! Общее время схемы. Принимается максимальное время из маршрутов.
        currentTime = 0;
        targetStartTime = 0;

        timer->stop();

        limitesToRect();

        initCoordinateSystem();

        update();

        emit sceneReseted();
    };

    void limitesToRect()
    {
        // Чтобы не было диких цифр при загрузке
        limits.set(static_cast<double>(rect().x()),
                   static_cast<double>(rect().y()),
                   static_cast<double>(rect().width()),
                   static_cast<double>(rect().height()));
    }

    void sendObjectInformation(size_t index, const QPointF& position, double speed)
    {
        emit sendIndexCurrentPositionSpeed(index, position, speed);
    }

    /*!
     * Метод перемещения из Таймера.
     */
    void moveFromTimer()
    {
        setCurrentTime(currentTime += speedMultiplier);

        if (currentTime >= fullTime)
        {
            timer->stop();
            emit sceneStopped();
        }

        this->move(currentTime);
    }

    void moveFromProgress(double time)
    {
        routes_->clear();

        timer->stop();

        emit sceneStopped();

        setCurrentTime(time);

        this->move(time);
    }

    // Интерфейс Scene (API)
public:
    void setCurrentTime(double time)
    {
        currentTime = time;

        actor->setCurrentTime(time);

        emit sendCurrentTime(time);
    }

    void upSpeed(double up)
    {
        this->speedMultiplier = timeWidget->speedUp(up);
    }

    void downSpeed(int down)
    {
        this->speedMultiplier = timeWidget->speedDown(down);
    }

    void resetSpeed()
    {
        this->speedMultiplier = timeWidget->setSpeed(1);
    }

    /*!
     * Максимальное время БЭНК и ПЛ.
     */
    void setFullTime()
    {
        fullTime = std::max(routes_->getMaximumTime(), targetStartTime + target->getFullTime()); // TODO: Время цели можно не учитывать

        setCurrentTime(0);

        emit sendFullTime(fullTime);
    }

    // Начало движения
    void start()
    {
        setCurrentTime(0);

        // Сброс буфера путей
        routes_->clear();
        target->clear();

        // Установить состояние движения до Текущего
        routes_->setStateType(new Scene::Objects::CurrentDrawState());

        timer->start(16); // Обновление каждые 16 миллисекунд (60 кадров в секунду)
        emit sceneStarted();
    }

    // Сброс к началу
    void clear()
    {
    }

    // Пауза
    bool pause()
    {
        if (timer->isActive())
        {
            timer->stop();
            emit scenePaused();

            return true;
        }
        else
        {
            timer->start(16);
            emit sceneStarted();
        }

        return false;
    }

    // Полный показ/Текущий показ
    void full()
    {
        if (routes_->getStateType() == Scene::Objects::StateType::Full)
        {
            routes_->setStateType(new Scene::Objects::CurrentDrawState());
        }
        else
        {
            routes_->setStateType(new Scene::Objects::FullDrawState());
        }

        update();
    }

    // Убрать/показать линии
    void lines()
    {
        if (routes_->getStateType() == Scene::Objects::StateType::Clean)
        {
            routes_->setStateType(new Scene::Objects::CurrentDrawState());
        }
        else
        {
            routes_->setStateType(new Scene::Objects::WithOutDrawState());
        }

        update();
    }

    // Изменить оси
    void change()
    {
        std::swap(axies.first, axies.second); // swapAxies

        limits.swap();

        // TODO: Где-то уменьшается Или не меняется
        actor->swapCoordinates();
        routes_->swapCoordinates();
        target->swapCoordinates();
        targetPath->swapCoordinates();

        initCoordinateSystem(); // percent

        update();
    }

public slots:
    void changeShowRoutesPoints()
    {
        routes_->changeShowPoints();

        update();
    }

    void changeDrawing()
    {
        setDrawing(!drawing);
    }

    // TODO: Не похоже что используется.
    void setDrawing(const bool check)
    {
        drawing = check;

        emit sendDrawing(drawing);
    }
    /*!
     * Установка скорости цели.
     * @param speed
     */
    void setTargetSpeed(double speed)
    {
        target->setSpeed(speed);
    }

    /*!
     * Действия при остановке.
     */
    void stop()
    {
        qDebug() << "*stop*";
        timer->stop();
        currentTime = 0;
        emit sceneStopped();

        routes_->clear();
        target->clear();

        update();
    }

    /*!
     * Метод определяет какие настройки устанавливаются при изменении Limits.
     * Пересчет только делаем при перезагрузке request или report.
     * @param limits
     */
    void setLimits(bool isReport)
    {
        limits.reset();

        //! Собрать из Путей
        if (isReport)
            limits.initFromRoutes(this->routes_->getRoutes());

        //! Инициализация из Actor.
        //! Данные Actor не забываем сбросить.
        this->limits.compareLimits(actor->getLimits());

        //! ПЛ не учитываем, всегда сбрасываем путь лодки.
        initTargetPath(limits.diagonal() * pointPercent);

        //! Обновляем модели.
        setModels(Objects::Arrow, 0.01 * limits.diagonal());

        if (axies.first > axies.second)
            this->limits.swap();

        //! Обновляем систему координат.
        initCoordinateSystem();

        update();
    }

    /*!
     * Установка моделей для ПЛ и БЭНК.
     * @param type
     * @param size
     */
    void setModels(Objects type, double size)
    {
        routes_->setModel(type, size); //getMaxDifference()); // 1%
        target->setModel(type, size);  // getMaxDifference());
    }

    /*!
     * Действия при обновлении Request.
     * @param request
     */
    void reloadRequest(const Request& request)
    {
        target->reset();

        // Гарантированно получаем полностью инициализированный request. Проверяется в mainWindow
        // targets->setParameters(request.getTarget()); // Инициализация данных цели
        target->initialize(request.getTarget(), 10);
        targetPath->clear();

        emit sendTargetSpeed(target->getSpeed());

        actor->reload(request);

        if (axies.first > axies.second)
        {
            actor->swapCoordinates();
        }

        routes_->reset();
        routes_->setParameters(request.getShip());

        this->setLimits(false);
    }

    /*!
     * Действия при обновлении Report.
     * @param report
     */
    void reloadReport(const Report& report)
    {
        // В MainWindow гарантируем, что файл request уже был загружен
        // targets->reset();

        // Инициализация routes
        routes_->setRoutes(report.routes(), pointPercent * limits.diagonal()); // Радиус точки 1% диагонали

        setFullTime();

        // TODO: Возможно требуется пересчет модели цели
        if (axies.first > axies.second)
        {
            routes_->swapCoordinates();
        }

        this->setLimits(true);
    }

    /*!
     * Инициализация пути цели.
     * @param radius
     */
    void initTargetPath(double radius)
    {
        targetPath->clear();
        targetStartTime = 0;

        targetPath->setRadius(radius);
    }

    /*!
     * Полный сброс установок ПЛ.
     */
    void resetTarget()
    {
        target->reset();
        targetPath->clear();
        targetStartTime = 0;

        update();
    }

protected:
    /*!
     * Метод определяет, что надо отображать.
     * @param event
     */
    void paintEvent(QPaintEvent* event) override
    {
        //! Определение общего QPainter.
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing); // Более плавная отрисовка, но наложение линий
        painter.drawImage(rect(), grid->getImage());
        painter.setTransform(cs.getTransform());

        //! Отрисовка Объектов
        actor->draw(painter);

        routes_->draw(painter);

        target->draw(painter);

        targetPath->draw(painter);
    }

    /*!
     * Действия при изменении размера виджета.
     * @param event
     */
    void resizeEvent(QResizeEvent* event) override
    {
        initCoordinateSystem();

        update();

        QWidget::resizeEvent(event);
    }

    void addPointToTargetPathInitTargets(const QPoint& screenPosition)
    {
        if (targetPath->isEmpty())
            targetStartTime = currentTime;

        targetPath->addPoint(cs.toLogical(screenPosition));

        target->setRoute(targetPath->getPath());
        target->setModel(Objects::Arrow, 0.01 * limits.diagonal());
    }

    /*!
     *
     * @param event
     */
    void mousePressEvent(QMouseEvent* event) override
    {
        if (drawing)
        {
            // TODO: Включение отображения ПЛ.
            if (targetPath->isEmpty())
                target->setState(new Scene::Objects::CurrentDrawState());

            addPointToTargetPathInitTargets(event->pos());
        }
        else
        {
            this->showPosition(event);
        }

        //! Обработка события pressMouse для targetPath.
        if (targetPath->hasDrawingPoints())
            targetPath->mousePress(event, cs.toLogical(event->pos()));

        //! Обработка события pressMouse для Routes.
        routes_->mousePressEvent(event, cs.toLogical(event->pos()));

        update();
    }

public:
    /*!
     *
     * @param event
     */
    void mouseMoveEvent(QMouseEvent* event) override
    {
        if (drawing)
        {
            //! При рисовании добавляем точки движения.
            if (rect().contains(event->pos()))
            {
                addPointToTargetPathInitTargets(event->pos());
            }

            // Отрисовка
        }
        //! Обработка событий для путей и точек соответственно.
        // if (targetPath->hasDrawingPoints())
        //     targetPath->mouseMove(cs.toLogical(event->pos()));

        //! Routes
        // routes_->mouseMoveEvent(event);

        update();
    }

public:
    /*!
     * Метод расчета новых позиций перемещения для всех объектов.
     * @param time Текущее время.
     */
    void move(double time)
    {
        routes_->move(time);

        //! Расчет позиции ПЛ.
        if (!target->isEmpty())
        {
            if (targetStartTime <= currentTime)
                target->move(time - targetStartTime);

            //! Текущие данные о цели.
            emit sendIndexCurrentPositionSpeed(0, target->getCurrentPosition(), target->getSpeed());
        }

        update();
    }

    /*!
     * Метод пересчета системы координат.
     */
    void initCoordinateSystem()
    {
        //! Пределы с учетом отступов.
        auto hightLimits = limits.limitsWithMargins(this->margin);

        cs.setTransform(rect(), hightLimits);

        //! Перерисовать изображение сетки Image.
        grid->draw(this->limits, rect(), axies.first, axies.second);
    }

    /*!
     * Метод показывает координаты точки под курсором при нажатии.
     * @param event
     */
    void showPosition(QMouseEvent* event)
    {
        // Отрисовка таблички X, Y.
        if (rect().contains(event->pos()))
        {
            QPointF pos = cs.toLogical(event->pos());

            QString tooltipText = QString("X: %1, Y: %2").arg(pos.x()).arg(pos.y());
            // : QString("X: %1, Y: %2").arg(pos.y()).arg(pos.x());

            QToolTip::showText(event->globalPos(), tooltipText, this);
        }
    }
};
} // namespace Widgets
