#pragma once

#include <QDebug>
#include <QJsonDocument>
#include <QKeyEvent>
#include <QPainter>
#include <QPointF>
#include <QTimer>
#include <QToolTip>
#include <QVBoxLayout>
#include <QVector2D>
#include <QVector>
#include <QWidget>

#include "JsonOperations.hpp"

#include "Visual/CoordinateSystem.hpp"
#include "Visual/GridWidget.hpp"
#include "Visual/ObjectWidget.hpp"
#include "Visual/PerimeterWidget.hpp"
#include "Visual/RandomTargetWidget.hpp"
#include "Visual/RouteWidget.hpp"
#include "Visual/SpeedWidget.hpp"
#include "Visual/TimeWidget.hpp"

#include "Visual/Models/Limits.hpp"
#include "Visual/Models/Object.hpp"
#include "Visual/Models/Perimeter.hpp"
#include "Visual/Models/Target.hpp"

// Responsibility Визуализировать объекты

// TODO: Без загрузки Perimeter и Route не должно отрисовываться
// TODO: Возможно привязать размеры ко всем сущностям в том числе и Routes
// TODO: Пересмотреть логику инициализации
// TODO: Размеры линий надо подбирать автоматически или отключать и включать транформацию

class VisWidget : public QWidget
{
    using RouteWidget = Visual::RouteWidget;
    using PerimeterWidget = Visual::PerimeterWidget;
    using CoordinateSystem = Visual::CoordinateSystem;
    using GridWidget = Visual::GridWidget;
    using TimeWidget = Visual::TimeWidget;

    using Route = Visual::Models::Route;
    using Segment = Visual::Models::Segment;
    using Object = Visual::Models::Object;
    using Limits = Visual::Models::Limits;
    using Perimeter = Visual::Models::Perimeter;

    Q_OBJECT

public:
    VisWidget(QWidget* parent = nullptr)
        : QWidget(parent), speedMultiplier(0.1)
    {
        targetTimer = new QTimer(this);
        timer = new QTimer(this);

        perimeter = new PerimeterWidget(this);

        target = new RouteWidget(this);
        target->setColor(Qt::black);

        speed = new SpeedWidget(this);
        speed->move(0, 0);
        speed->show();

        time = new TimeWidget(this);
        time->move(0, 20);
        time->show();

        connect(timer, &QTimer::timeout, this, &VisWidget::updateRoutes);
        connect(targetTimer, &QTimer::timeout, this, &VisWidget::updateTarget);

        connect(this, &VisWidget::sendMultiplier, time, &TimeWidget::updateTime);
    }

signals:
    void sendMultiplier(double);
    void sendChangedDrawing(bool);
    void sendTargetPosition(QPointF);
    void sendTargetSpeed(double);
    void sendIntersectionResult(QString);

public slots:
    void setDrawing(const bool value)
    {
        drawing = value;

        emit sendChangedDrawing(drawing);
    }

protected:
    void paintEvent(QPaintEvent* event) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing); // Более плавная отрисовка, но наложение линий

        painter.setTransform(cs.getTransform());

        // Отрисовка сетки
        auto lims = limits.limitsWithMargins(5);
        grid = new GridWidget(this);
        grid->draw(painter, lims);

        // Отрисовка Периметра
        perimeter->draw(painter);

        drawRoutes(painter);

        drawTarget(painter);
    }

    void drawRoutes(QPainter& painter)
    {
        for (const auto& route : routes)
        {
            route->draw(painter);

            if (!found && isPointInsideEllipse(route->getCurrentPosition(), route->getRadius()))
            {
                found = true;

                sendIntersectionResult(QString("Target Position: [" + QString("%1, %2").arg(target->getCurrentPosition().x()).arg(target->getCurrentPosition().y()) + ", Ship[" +
                                               QString("] position:") + QString("%1, %2").arg(route->getCurrentPosition().x()).arg(route->getCurrentPosition().y()) +
                                               QString(", Radius: ") + QString("%1").arg(route->getRadius())));

                pause();
            }
        }
    }

    void showFullTargetPath(QPainter& painter) const
    {
        QPen pen = QPen(Qt::darkYellow, 2);
        pen.setStyle(Qt::DotLine);
        pen.setCosmetic(true);

        painter.setPen(pen);

        if (!targetPath.isEmpty())
        {
            for (int i = 0; i < targetPath.size() - 1; ++i)
            {
                painter.drawLine(targetPath[i], targetPath[i + 1]);
            }
        }
    }

    void drawTarget(QPainter& painter)
    {
        showFullTargetPath(painter);

        target->draw(painter);
    }

public:
    void loadFirstScreen()
    {
        loaded = true;

        auto lims = limits.limitsWithMargins(5);

        initCoordinateSystem(lims);

        update();
    }

public:
    void start()
    {
        found = false;
        numberRoutes = routes.size(); // Количество путей

        // Сброс буфера путей
        resetRoutesToDefault();

        for (const auto route : routes)
        {
            route->setState(new Visual::CurrentDrawState());
        }

        // Сброс счетчика времени схемы
        time->reset();

        timer->start(16); // Обновление каждые 16 миллисекунд (60 кадров в секунду)

        paused = false;

        if (!target->getSegments().isEmpty())
        {
            target->clear();
            targetTimer->start(15);
        }
    }

    void pause()
    {
        if (!paused)
        {
            timer->stop();
            targetTimer->stop();
        }
        else
        {
            timer->start(16);
            found = false; // TODO: Спорный момент

            // TODO: Target Pause
            if (!target->getSegments().isEmpty())
            {
                targetTimer->start(16);
            }
        }

        paused = !paused;
    }

    void pauseTarget()
    {
        if (!pausedTarget)
        {
            targetTimer->stop();
        }
        else
        {
            targetTimer->start(16);
        }

        pausedTarget = !pausedTarget;
    }

    // TODO: реализовать и вывести для API
    double upSpeed(const int multiplier)
    {
        speedMultiplier += multiplier;
        speed->update(speedMultiplier);

        return speedMultiplier;
    }

    // TODO: При скорости от 0 до 1 уменьшать на 0.1
    double downSpeed(const int multiplier)
    {
        if (speedMultiplier - multiplier > 0)
            speedMultiplier -= multiplier;

        speed->update(speedMultiplier);

        return speedMultiplier;
    }

    // Отрисовка полных путей
    void setShowFull()
    {
        for (const auto route : routes)
        {
            if (route->getStateType() == Visual::State::Full)
            {
                route->setState(new Visual::CurrentDrawState());
            }
            else
            {
                route->setState(new Visual::FullDrawState());
            }
        }

        update();
    }

    // Показывать/не показывать пути
    void setShowLines()
    {
        for (const auto route : routes)
        {
            if (route->getStateType() == Visual::State::Clean)
            {
                route->setState(new Visual::CurrentDrawState());
            }
            else
            {
                route->setState(new Visual::WithOutDrawState());
            }
        }

        if (target->getStateType() == Visual::State::Clean)
        {
            target->setState(new Visual::CurrentDrawState());
        }
        else
        {
            target->setState(new Visual::WithOutDrawState());
        }

        update();
    }

    bool getDrawing() const
    {
        return drawing;
    }

    double getMultiplier() const
    {
        return speedMultiplier;
    }

    void setMultiplier(const double multi)
    {
        speed->update(multi);
        speedMultiplier = multi;
    }

    QVector<RouteWidget*> getRoutes() const
    {
        return routes;
    }

    PerimeterWidget* getPerimeter() const
    {
        return perimeter;
    }

    void targetClear()
    {
        target->reset();

        targetPath.clear();

        targetTimer->stop();

        update();
    }

    void setPerimeter(const Perimeter& p)
    {
        // TODO: Инициализируем
        perimeter = new PerimeterWidget(this);

        perimeter->setPerimeter(p);
    }

    void setRoutes(const QVector<Route>& routes_)
    {
        routes = QVector<RouteWidget*>();

        for (const auto& route_ : routes_)
        {
            auto route = new RouteWidget(this);

            route->initialize(route_.getSegments(), palette[routes.size() % palette.size()], shipParameters);

            this->routes.append(route);
        }

        resetRoutesToDefault(); // После загрузки обновляем
    }

    void setRoutesRadius()
    {
        for (const auto route : routes)
        {
            route->setRadius(shipParameters.getDetectionRange());
        }
    }

    Limits getLimits() const
    {
        return limits;
    }

    void initLimitsFromRoutes()
    {
        for (const auto& route : routes)
        {
            for (const auto& segment : route->getSegments())
                limits.initFromSegment(segment->getSegment());
        }

        limits.show();
    }

    void initLimitsFromPerimeter()
    {
        limits.initFromPerimeter(perimeter->getPerimeter());
        limits.show();
    }

    void setRoutesModels(const Visual::Objects model, const double size) const
    {
        //
        for (size_t i = 0; i != routes.size(); ++i)
        {
            routes[i]->setModel(model, size);
        }
    }

    void initCoordinateSystem(const Limits& lims)
    {
        cs = CoordinateSystem();

        cs.setTransform(rect(), lims);
    }

    void setTargetSpeed(const double value)
    {
        targetParameters.setCurrentVelocity(value);
    }

    void resetSpeed()
    {
        setMultiplier(1.);
    }

public:
    // Controls
    void resizeEvent(QResizeEvent* event) override
    {
        auto lims = limits.limitsWithMargins(5);
        cs.setTransform(rect(), lims);

        QWidget::resizeEvent(event);
    }

    void mousePressEvent(QMouseEvent* event) override
    {
        if (!loaded)
            return;

        if (drawing)
        {
            targetClear();

            if (event->button() == Qt::LeftButton)
            {
                targetPath.append(cs.toLogical(event->pos()));
            }
        }
        else
        {
            // Отрисовка таблички X, Y.
            if (rect().contains(event->pos()))
            {
                QPointF pos = cs.toLogical(event->pos());
                QString tooltipText = QString("X: %1, Y: %2").arg(pos.x()).arg(pos.y());
                QToolTip::showText(event->globalPos(), tooltipText, this);
            }
        }
    }

public:
    void setLoaded(const bool value)
    {
        loaded = value;
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        if (!loaded)
        {
            return;
        }

        if (drawing)
        {
            // Набор пути цели
            if (rect().contains(event->pos()))
                targetPath.append(cs.toLogical(event->pos()));

            // Отрисовка
            update();
        }
    }

    void mouseReleaseEvent(QMouseEvent* event) override
    {
        if (!loaded)
        {
            return;
        }

        if (event->button() == Qt::LeftButton && drawing)
        {
            addTargetSegments();

            update();

            // TODO:RESET
            target->clear();
            target->initHead(targetParameters);
            target->setModel(Visual::Objects::Arrow, 0.01 * limits.getMaxBorderLength()); // 1%

            sendTargetInformation();

            if (!paused)
            {
                // начинаем отрисовывать
                targetTimer->start(15);
            }
        }
    }

    void addTargetSegments()
    {
        // Если одна точка
        if (targetPath.size() == 1)
        {
            target->addSegment(targetPath[0], targetPath[0], targetParameters.getCurrentVelocity()); // Cкорость
            return;
        }

        for (int i = 0; i < targetPath.size() - 1; ++i)
        {
            if (targetPath[i] == targetPath[i + 1])
                continue;

            target->addSegment(targetPath[i], targetPath[i + 1], targetParameters.getCurrentVelocity()); // Cкорость
        }
    }

private slots:
    // TODO: Управление таймерами должно быть тоже тут
    void updateRoutes()
    {
        for (auto& route : routes)
        {
            // Когда путь заканчивается таймер останавливается
            if (!route->update(speedMultiplier))
            {
                --numberRoutes;
                // TODO: Требуется проверка что все доехали, а не один как сейчас
                // TODO: Добавить счетчик
                if (numberRoutes == 0)
                {
                    qDebug() << "Общая длина пути: " << route->getCurrentLength() << " метров.";
                    qDebug() << "Время в пути: " << route->getCurrentTime() << " минут.";
                    timer->stop();
                }
            }
        }
        // Обновление времени
        emit sendMultiplier(routes[0]->getCurrentTime());

        update();
    }

    void sendTargetInformation()
    {
        emit sendTargetPosition(target->getCurrentPosition());
        emit sendTargetSpeed(target->getSpeed());
    }

    void updateTarget()
    {
        if (target->update(speedMultiplier))
        {
            sendTargetInformation();
        }
        else
        {
            targetTimer->stop();
        }

        update();
    }

private:
    bool isPointInsideEllipse(const QPointF& center, const double& radius) const
    {
        return QLineF(target->getCurrentPosition(), center).length() <= radius;
    }

    // Расчет свойств для отрисовки путей
    // TODO: CLEAR
    void resetRoutesToDefault()
    {
        for (auto& route : routes)
        {
            if (!route->getSegments().isEmpty())
            {
                route->clear();
            }
        }
    }

public:
    void setShipParameters(const Object& obj)
    {
        shipParameters = obj;
    }

    void setTargetParameters(const Object& obj)
    {
        targetParameters = obj;
    }

    void resetLimits()
    {
        limits.reset();
    }

private:
    // Цвета для БЭНКов
    QVector<QColor> palette = {Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::cyan, Qt::magenta};

    // Начальные данные из входных данных
    Object shipParameters;
    Object targetParameters;

    // TODO: Добавить State и Momento
    // Время
    TimeWidget* time;
    SpeedWidget* speed;

    // Сетка  TODO: перевести в QImage
    GridWidget* grid;

    // БЭНКи
    QVector<RouteWidget*> routes;
    RouteWidget* target;

    QTimer* timer; // Таймер БЭНКов

    // Target
    QVector<QPointF> targetPath; // Экранный путь цели(координаты screen)
    QTimer* targetTimer;         // Таймер цели

    // Общие поля
    CoordinateSystem cs;
    PerimeterWidget* perimeter;

    double speedMultiplier; // Множитель скорости

    Limits limits;

    // Flags
    size_t numberRoutes;
    bool found = false; // Объект найден
    bool loaded = false;

    bool drawing = false; // Разрешить отрисовку цели

    bool paused = true; // Общая пауза
    bool pausedTarget = false;
};
