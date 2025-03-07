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
#include "Visual/PerimeterWidget.hpp"
#include "Visual/TargetWidget.hpp"
#include "Visual/TimeWidget.hpp"

#include "Visual/Models/Perimeter.hpp"
#include "Visual/Models/Ship.hpp"
#include "Visual/Models/Target.hpp"

#include "Visual/Models/Limits.hpp"

// Responsibility Визуализировать объекты

// TODO: Без загрузки Perimeter и Route не должно отрисовываться
// TODO: Возможно привязать размеры ко всем сущностям в том числе и Routes
// TODO: Пересмотреть логику инициализации
// TODO: Размеры линий надо подбирать автоматически или отключать и включать транформацию

class VisWidget : public QWidget
{
    Q_OBJECT

public:
    // TODO: + const Perimeter или убрать Routes
    // TODO: Инициализация CoordinateSystem во время загрузки request.json, а установка trans каждый раз когда надо
    VisWidget(QWidget* parent = nullptr)
        : QWidget(parent), speedMultiplier(1.0)
    {
        // TODO: Должен запускаться пустой(чистый)
        // TODO: Добавить Лодку. Для лодки отдельный таймер
        targetTimer = new QTimer(this);
        timer = new QTimer(this);

        perimeter = new PerimeterWidget(this);

        target = new TargetWidget(this);

        grid = new GridWidget(this);

        time = new TimeWidget(this);
        time->move(0, 20);
        time->show();

        connect(timer, &QTimer::timeout, this, &VisWidget::updateRoutes);
        connect(targetTimer, &QTimer::timeout, this, &VisWidget::updateTarget);

        connect(this, &VisWidget::sendMultiplier, time, &TimeWidget::updateTime);
    }

    std::unique_ptr<RouteState> state;

    std::unique_ptr<RouteState> lastState;

    bool inState = false;

    void setState(std::unique_ptr<RouteState> newState)
    {
        // TODO: При повторном нажатии на то же состояние откат к предыдущему
        // TODO: При нажатии на новое смена на новое

        // Для использования одной кнопкой например
        if (inState)
        {
            state = std::move(lastState);

            inState = false;
        }
        else
        {
            lastState = std::move(state);

            state = std::move(newState);

            inState = true;
        }
    }

signals:
    void sendMultiplier(double);
    void sendChangedDrawing(bool);
    void sendTargetPointSpeed(std::pair<QPointF, double>);
    void sendIntersectionResult(QString);

public:
    void setDrawing(const bool value)
    {
        drawing = value;

        emit sendChangedDrawing(drawing);
    }

protected:
    // TODO: Отрисовка Widget начинается после инициализации всех Loaded
    // TODO: Скорости свести к множителю по времени, Выводить время поверх
    void paintEvent(QPaintEvent* event) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing); // Более плавная отрисовка, но наложение линий

        painter.setTransform(cs.getTransform());

        // Отрисовка сетки
        grid->draw(painter, limits);

        // Отрисовка Периметра
        perimeter->draw(painter);

        drawRoutes(painter);

        drawTarget(painter);

        drawMultiplier(painter);
    }

    void drawRoutes(QPainter& painter)
    {
        for (const auto& route : _routes)
        {
            // route->drawCurrent(painter);
            route->draw(painter, showLines, showFull);

            if (!found && isPointInsideEllipse(route->getPosition(), route->getRadius()))
            {
                found = true;

                sendIntersectionResult(QString("Target Position: [" + QString("%1, %2").arg(target->getPosition().x()).arg(target->getPosition().y()) + ", Ship[" +
                                               QString("] position:") + QString("%1, %2").arg(route->getPosition().x()).arg(route->getPosition().y()) +
                                               QString(", Radius: ") + QString("%1").arg(route->getRadius())));

                pause();
            }
        }
    }

    void drawTarget(QPainter& painter)
    {
        QPen pen = QPen(Qt::darkYellow, 2);
        pen.setStyle(Qt::DotLine);
        pen.setCosmetic(true);

        painter.setPen(pen);

        if (showLines)
            if (!targetPath.isEmpty())
            {
                for (int i = 0; i < targetPath.size() - 1; ++i)
                {
                    painter.drawLine(targetPath[i], targetPath[i + 1]);
                }
            }

        target->draw(painter, showLines);
    }

    void drawMultiplier(QPainter& painter)
    {
        painter.resetTransform();
        painter.setPen(Qt::black);
        painter.drawText(10, 20, QString("Speed Multiplier: %1x").arg(speedMultiplier));
    }

private:
    void loadFirstScreen()
    {
        if (perimeterLoaded && routesLoaded)
        {
            auto limits = limitsWithMargins(5);

            initCoordinateSystem(limits);

            //grid->initialize(limits);

            update();
        }
    }

public:
    // TODO: прорисовка не должна начинаться без инициализации Perimeter и Routes
    void start()
    {
        found = false;

        // Сброс путей и начинаем отрисовку сначала
        if (!perimeterLoaded)
        {
            throw std::runtime_error("Периметер не загружен.");
        }
        if (!routesLoaded)
        {
            throw std::runtime_error("Пути не загружены.");
        }

        // Сброс буфера пути
        resetRoutesToDefault();

        // Сброс счетчика времени схемы
        time->reset();

        timer->start(16); // Обновление каждые 16 миллисекунд (~60 кадров в секунду)

        paused = false;

        if (!target->getSegments().isEmpty())
        {
            target->reset();
            targetTimer->start(15);
        }
    }

    void full()
    {
        showFull = !showFull;

        if (showFull)
            update();
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

            found = false;

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

        return speedMultiplier;
    }

    double downSpeed(const int multiplier)
    {
        if (speedMultiplier - multiplier > 0)
            speedMultiplier -= multiplier;

        return speedMultiplier;
    }

    void changeShowLines()
    {
        showLines = !showLines;

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
        speedMultiplier = multi;
    }

    QVector<RouteWidget*> getRoutes() const
    {
        return _routes;
    }

    PerimeterWidget* getPerimeter() const
    {
        return perimeter;
    }

    void targetClear()
    {
        target->clear();

        targetPath.clear();

        targetTimer->stop();

        update();
    }

    void setRoutes(const QVector<Route>& routes)
    {
        _routes = QVector<RouteWidget*>();

        for (const auto& route : routes)
        {
            auto _route = new RouteWidget(this); // TODO

            _route->initialize(route.getSegments(), palette[_routes.size() % palette.size()], shipParameters.getDetectionRange());

            _routes.append(_route);
        }

        routesLoaded = true;

        resetRoutesToDefault();

        initLimitsFromRoutes();

        loadFirstScreen();
    }

    void setRoutes(const QVector<QVector<Segment>>& segmentsVector)
    {
        _routes = QVector<RouteWidget*>();

        for (const auto& segments : segmentsVector)
        {
            auto _route = new RouteWidget();

            _route->initialize(segments, palette[_routes.size() % palette.size()], 1);

            _routes.append(_route);
        }

        routesLoaded = true;

        resetRoutesToDefault();

        initLimitsFromRoutes();

        loadFirstScreen();
    }

    void setPerimeter(const Perimeter& p)
    {
        // TODO: Инициализируем
        perimeter = new PerimeterWidget(this);

        for (auto inner : p.getInners())
        {
            this->perimeter->addInner(inner);
        }

        perimeterLoaded = true;

        initLimitsFromPerimeter();

        loadFirstScreen();
    }

    Limits limitsWithMargins(const double percent)
    {
        // Отступы по сторонам
        // TODO: Брать процентное отношение от ширины и длины
        double height = std::fabs(limits.maxY - limits.minY) * percent / 100;
        double width = std::fabs(limits.maxX - limits.minX) * percent / 100;

        auto minX = limits.minX - width;  // minX
        auto minY = limits.minY - height; // minY
        auto maxX = limits.maxX + width;  // maxX
        auto maxY = limits.maxY + height; // maxY

        return {minX, minY, maxX, maxY};
    }

    void initCoordinateSystem(const Limits& limits)
    {
        cs = CoordinateSystem(/*limits*/);

        cs.setTransform(rect(), limits);
    }

    void initLimitsFromPerimeter()
    {
        limits.minX = std::min(limits.minX, perimeter->getMinX());

        limits.minY = std::min(limits.minY, perimeter->getMinY());

        limits.maxX = std::max(limits.maxX, perimeter->getMaxX());

        limits.maxY = std::max(limits.maxY, perimeter->getMaxY());
    }

    void initLimitsFromRoutes()
    {
        // максимумы из Routes
        for (const auto& route : _routes)
        {
            for (const auto& segment : route->getSegments())
            {
                limits.minX = std::min(limits.minX, segment->getStart().x());
                limits.minX = std::min(limits.minX, segment->getEnd().x());

                limits.minY = std::min(limits.minY, segment->getStart().y());
                limits.minY = std::min(limits.minY, segment->getEnd().y());

                limits.maxX = std::max(limits.maxX, segment->getStart().x());
                limits.maxX = std::max(limits.maxX, segment->getEnd().x());

                limits.maxY = std::max(limits.maxY, segment->getStart().y());
                limits.maxY = std::max(limits.maxY, segment->getEnd().y());
            }
        }
    }

    void setTargetSpeed(const double value)
    {
        targetSpeed = value;
    }

    void resetSpeed()
    {
        speedMultiplier = 1.0;
    }

public:
    // Controls
    void resizeEvent(QResizeEvent* event) override
    {
        cs.setTransform(rect(), limits);

        QWidget::resizeEvent(event);
    }

    void mousePressEvent(QMouseEvent* event) override
    {
        if (!perimeterLoaded || !routesLoaded)
            return;

        if (drawing)
        {
            targetClear();

            if (event->button() == Qt::LeftButton)
            {
                // cs.setTransform(rect());

                targetPath.append(cs.toLogical(event->pos()));

                target->setPosition(cs.toLogical(event->pos()));
            }
        }
        else
        {
            if (rect().contains(event->pos()))
            {
                QPointF pos = cs.toLogical(event->pos());
                QString tooltipText = QString("X: %1, Y: %2").arg(pos.x()).arg(pos.y());
                QToolTip::showText(event->globalPos(), tooltipText, this);
            }
        }
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        if (!perimeterLoaded || !routesLoaded)
        {
            return;
        }

        if (drawing)
        {
            // Набор пути цели

            if (rect().contains(event->pos()))
                targetPath.append(cs.toLogical(event->pos()));

            // Отрисовка пути цели
            update();
        }

        // Если Периметр загружен и отключена прокладка пути цели
        //     // cs.setTransform(rect());

        //     // Только внутри Widget
        //     if (rect().contains(event->pos()))
        //     {
        //         QPointF pos = cs.toLogical(event->pos());
        //         QString tooltipText = QString("X: %1, Y: %2").arg(pos.x()).arg(pos.y());
        //         QToolTip::showText(event->globalPos(), tooltipText, this);
        //     }
    }

    void mouseReleaseEvent(QMouseEvent* event) override
    {
        // cs.setTransform(rect());

        if (event->button() == Qt::LeftButton && drawing)
        {
            // cs.setTransform(rect());

            addTargetSegments();

            update();

            target->reset();

            if (!paused)
            {
                // начинаем отрисовывать
                targetTimer->start(15);
            }
        }

        qDebug() << "------------------------------------";
        for (auto segment : target->getSegments())
            segment->show();
    }

    void addTargetSegments()
    {
        for (int i = 0; i < targetPath.size() - 1; ++i)
        {
            if (targetPath[i] == targetPath[i + 1])
                continue;

            target->addSegment(targetPath[i], targetPath[i + 1], targetSpeed); // Cкорость
        }
    }

protected:
    // TODO: Оставить для тестов
    void keyPressEvent(QKeyEvent* event) override
    {
        // TODO: Сделать switch
        // Увеличить скорость
        if (event->key() == Qt::Key_Equal)
        {
            upSpeed(2);
        }
        // Сброс скрости к базовой
        else if (event->key() == Qt::Key_Plus)
        {
            resetSpeed();
        }
        else if (event->key() == Qt::Key_Minus)
        // Уменьшить скорость
        {
            downSpeed(2);
        }
        // Старт движения
        else if (event->key() == Qt::Key_S)
        {
            start();
        }
        // Показывать пути БЭНКов
        else if (event->key() == Qt::Key_L)
        {
            changeShowLines();
        }
        // Пауза
        else if (event->key() == Qt::Key_P)
        {
            pause();
        }
        // Загрузка Периметра
        else if (event->key() == Qt::Key_F)
        {
            setupP();
        }
        // Загрузка путей БЭНКов
        else if (event->key() == Qt::Key_R)
        {
            setupR();
        }
        // Разрешить отрисовку цели
        else if (event->key() == Qt::Key_X)
        {
            setDrawing(true);
        }
        // Очистка путей и точки цели
        else if (event->key() == Qt::Key_C)
        {
            targetClear(); // Tagret clear()
        }

        update();
    }

private slots:
    void sendPairPositionSpeed(const QPointF& position, const double& speed)
    {
        // TODO: пока считаем что скорость одна
        std::pair<QPointF, double> pointSpeed{position, speed};

        emit sendTargetPointSpeed(pointSpeed);
    }

    // TODO: Управление таймерами должно быть тоже тут
    void updateRoutes()
    {
        for (auto& route : _routes)
        {
            // Когда путь заканчивается таймер останавливается
            if (!route->update(speedMultiplier))
            {
                timer->stop();
            }
        }

        // TODO: Время только первого маршрута
        // Обновление времени
        emit sendMultiplier(_routes[0]->getCurrentTime());

        update();
    }

    void updateTarget()
    {
        if (target->update(speedMultiplier))
        {
            sendPairPositionSpeed(target->getPosition(), target->getSpeed());
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
        return QLineF(target->getPosition(), center).length() <= radius;
    }

    // Расчет свойств для отрисовки путей
    void resetRoutesToDefault()
    {
        for (auto& route : _routes)
        {
            if (!route->getSegments().isEmpty())
            {
                route->reset();
            }
        }
    }

public:
    void setupS()
    {
    }

    void setupR(const QString path = "/home/harikeshi/work/routes/tresult.json")
    {
        if (!perimeterLoaded)
            throw std::runtime_error("Периметер не загружен!");

        QByteArray barray;

        if (!path.isEmpty())
        {
            QFile file(path);

            // Проверка
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                throw std::runtime_error("Не удалось открыть файл");
            }

            barray = file.readAll();

            file.close();
        }

        QJsonDocument doc = QJsonDocument::fromJson(barray);

        auto obj = doc.object();

        // Routes<Segment>
        auto routes = JsonHelpers::parseToRoutes(obj);

        setRoutes(routes);
    }

    void setupP(const QString& path = "/home/harikeshi/work/routes/trequest.json")
    {
        QByteArray barray;
        if (!path.isEmpty())
        {
            QFile file(path);

            // Проверка
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                throw std::runtime_error("Не удалось открыть файл");
            }

            barray = file.readAll();

            file.close();
        }

        QJsonDocument doc = QJsonDocument::fromJson(barray);

        QJsonObject obj = doc.object();

        auto perimeter = JsonHelpers::parseToPolygon(obj);

        shipParameters = JsonHelpers::parseToShip(obj);

        targetParameters = JsonHelpers::parseToTarget(obj);

        setPerimeter(perimeter);
    }

private:
    // TODO: при добавлении setColor()
    // Цвета для БЭНКов
    QVector<QColor> palette = {Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::cyan, Qt::magenta};

    // Начальные данные из входных данных
    Ship shipParameters;
    Target targetParameters;

    // TODO: Добавить State и Momento

    // Установка: Ship, Target, Route, Segment => Модели
    //  Одноименные виджеты для использования в программе

    // Установка: Первым проводится инициализация входных данных

    // Время
    TimeWidget* time;

    // Сетка  TODO: перевести в QImage
    GridWidget* grid;

    // БЭНКи
    QVector<RouteWidget*> _routes;
    // QVector<Ship*> ships;

    QTimer* timer; // Таймер БЭНКов

    // Target
    TargetWidget* target;
    QVector<QPointF> targetPath; // Экранный путь цели(координаты screen)
    QTimer* targetTimer;         // Таймер цели
    double targetSpeed = 5;      // TODO: Брать из targetParameters

    // Общие поля
    CoordinateSystem cs;
    PerimeterWidget* perimeter;
    int speedMultiplier; // Множитель скорости

    Limits limits;
    // std::pair<double, double> minimums;
    // std::pair<double, double> maximums;

    // Flags
    bool found = false; // Объект найден

    bool perimeterLoaded = false; // Периметер загружен
    bool routesLoaded = false;    // Пути загружены

    bool drawing = false;  // Разрешить отрисовку цели
    bool showLines = true; // Показывать линии путей

    bool paused = true; // Общая пауза
    bool pausedTarget = false;

    bool showFull = false;
};
