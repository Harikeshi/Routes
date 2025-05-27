#pragma once

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QTimer>
#include <QToolTip>
#include <QWidget>

#include "../Data/Report.hpp"
#include "../Data/Request.hpp"

#include "../Scene/Entities/CoordinateSystem.hpp"
#include "../Scene/Entities/Limits.hpp"
#include "../Scene/Entities/Routes.hpp"
#include "../Scene/Entities/Targets.hpp"

#include "../Scene/Objects/GridObject.hpp"
#include "../Scene/Objects/RouteState.hpp"

#include "../Scene/Actors/InRegion.hpp"

namespace Widgets {
class SceneWidget : public QWidget
{
    using CoordinateSystem = Scene::Entities::CoordinateSystem;
    using Limits = Scene::Entities::Limits;
    using Routes = Scene::Entities::Routes;
    using Targets = Scene::Entities::Targets;

    using Grid = Scene::Objects::GridObject;
    using Actor = Scene::Actor;
    using Objects = Scene::Objects::Objects;

    using Request = Data::Request;
    using Report = Data::Report;

    using InRegionScene = Scene::Actors::InRegionScene;

    Q_OBJECT

protected:
    Grid* grid;          // Сетка
    CoordinateSystem cs; // Система координат
    Limits limits;       // Крайние значения по осям

    // Элементы отрисовки
    Routes* routes_;
    Targets* targets;
    Actor* actor;

    bool drawing; // Разрешить отрисовку цели

    double speedMultiplier; // Множитель скорости
    double margin;          // Отступы от каждой стороны в процентах

    QTimer* timer;

    // Расположение осей
    QPair<QString, QString> axies{"X", "Y"};

public:
    explicit SceneWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        routes_ = new Routes(this);
        targets = new Targets(this);

        grid = new Grid(this);
        timer = new QTimer(this);

        // TODO: переименовать
        actor = new InRegionScene(this);

        cs = CoordinateSystem();

        drawing = false;
        speedMultiplier = 1;
        margin = 5.;

        this->resize(800, 800);

        // Чтобы не было диких цифр при загрузке
        limits.set(static_cast<double>(rect().x()),
                   static_cast<double>(rect().y()),
                   static_cast<double>(rect().width()),
                   static_cast<double>(rect().height()));

        connect(timer, &QTimer::timeout, this, &SceneWidget::updateDrawing);

        connect(routes_, &Routes::stop, this, &SceneWidget::stop);
        connect(routes_, &Routes::sendCurrentTime, this, &SceneWidget::getDuringTime);
    }

    ~SceneWidget() = default;

    // Интерфейс Scene (API)
public:
    // Начало движения
    void start()
    {
        // Сброс буфера путей
        routes_->clear();
        targets->clear();

        // Установить состояние движения до Текущего
        routes_->setStateType(new Scene::Objects::CurrentDrawState());

        timer->start(16); // Обновление каждые 16 миллисекунд (60 кадров в секунду)
    }

    // Пауза
    void pause()
    {
        if (timer->isActive())
            timer->stop();
        else
        {
            timer->start(16);
        }
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
        targets->swapCoordinates();

        initCoordinateSystem(); // percent
    }

signals:
    void sendMessage(const QString&); // Оповещение о пересечении например
    void sendDrawing(const bool&);
    void sendDuringTime(const double&) const;

    void sendTargetPosition(const QPointF&) const;
    void sendTargetSpeed(const double&);

    // void sendRoutesPositions(const QVector<QPointF>&); //
    // void sendStartMove();

public slots:
    void changeDrawing()
    {
        setDrawing(!drawing);
    }

    void setDrawing(const bool check)
    {
        drawing = check;

        emit sendDrawing(drawing);
    }

    void stop()
    {
        qDebug() << "*stop*";
        timer->stop();
    }

    void setMultiplier(const double& multiplier)
    {
        this->speedMultiplier = multiplier;
    }

    void setLimits(const Limits& limits)
    {
        this->limits = limits;

        if (axies.first > axies.second)
            this->limits.swap();
    }

    void reloadRequest(const Request& request)
    {
        targets->reset();

        // Гарантированно получаем полностью инициализированный request, Проверяется в mainWindow
        targets->setParameters(request.getTarget()); // Инициализация данных цели
        targets->setModel(0.01 * limits.getMaxDifference());

        actor->reload(request);

        if (axies.first > axies.second)
        {
            actor->swapCoordinates();
        }

        routes_->setParameters(request.getShip());

        if (!routes_->isEmpty())
        {
            // Если Пути загружены, то пересчитываем модели
            routes_->setModel(Objects::Arrow, 0.01 * limits.getMaxDifference()); // 1%
        }

        initCoordinateSystem();
    }

    void reloadReport(const Report& report)
    {
        // В MainWindow гарантируем, что файл request уже был загружен
        targets->reset();

        // Инициализация routes
        routes_->setRoutes(report.routes());
        routes_->setModel(Objects::Arrow, 0.01 * limits.getMaxDifference()); // 1%

        // TODO: Возможно требуется пересчет модели цели
        if (axies.first > axies.second)
        {
            routes_->swapCoordinates();
        }

        initCoordinateSystem();
    }

    void getDuringTime(const double& time) const
    {
        emit sendDuringTime(time);
    }

    void resetTarget()
    {
        targets->reset();

        update();
    }

protected:
    // Events
    void paintEvent(QPaintEvent* event) override
    {
        // Отрисовка сетки, путей кораблей, пути лодки, периметер
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing); // Более плавная отрисовка, но наложение линий

        painter.drawImage(rect(), grid->getImage());

        painter.setTransform(cs.getTransform());

        actor->draw(painter);

        routes_->draw(painter);

        targets->draw(painter);
    }

    void resizeEvent(QResizeEvent* event) override
    {
        initCoordinateSystem();

        QWidget::resizeEvent(event);
    }

    // TODO: на маин здесь только возврат точки
    void mousePressEvent(QMouseEvent* event) override
    {
        if (drawing)
        {
            this->startDrawing(event);
        }
        else
        {
            this->showPosition(event);
        }
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        if (drawing)
        {
            // Набор пути цели
            if (rect().contains(event->pos()))
                targets->addToTargetPath(cs.toLogical(event->pos()));

            // Отрисовка
            update();
        }
    }

    // TODO: Управления тут не должно быть
    void mouseReleaseEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            if (drawing)
            {
                targets->setRoute();

                update();

                // TODO:RESET
                targets->clear();
                targets->setModel(limits);

                emit sendTargetPosition(targets->getCurrentPosition());
                emit sendTargetSpeed(targets->getSpeed());
            }
        }

        update();
    }

private:
    void updateDrawing()
    {
        routes_->move(speedMultiplier);
        targets->move(speedMultiplier);

        emit sendTargetPosition(targets->getCurrentPosition());
        emit sendTargetSpeed(targets->getSpeed());

        update();
    }

    void initCoordinateSystem()
    {
        auto hightLimits = limits.limitsWithMargins(this->margin);

        cs.setTransform(rect(), hightLimits);

        grid->draw(this->limits, rect(), axies.first, axies.second);

        update();
    }

    void startDrawing(QMouseEvent* event)
    {
        targets->reset();

        if (event->button() == Qt::LeftButton)
        {
            targets->addToTargetPath(cs.toLogical(event->pos()));
        }
    }

    void showPosition(QMouseEvent* event)
    {
        // Отрисовка таблички X, Y.
        if (rect().contains(event->pos()))
        {
            QPointF pos = cs.toLogical(event->pos());

            QString tooltipText = QString("X: %1, Y: %2").arg(pos.x()).arg(pos.y()); // : QString("X: %1, Y: %2").arg(pos.y()).arg(pos.x());

            QToolTip::showText(event->globalPos(), tooltipText, this);
        }
    }
};
} // namespace Widgets