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
    // TODO: + const Perimeter или убрать Routes
    // TODO: Инициализация CoordinateSystem во время загрузки request.json, а установка trans каждый раз когда надо
    VisWidget(QWidget* parent = nullptr)
        : QWidget(parent), speedMultiplier(1.0)
    {
        // TODO: Для прикола
        random = new Visual::RandomTargetWidget(this);

        rTimer = new QTimer(this);
        connect(rTimer, &QTimer::timeout, this, &VisWidget::movePoint);

        // TODO: Должен запускаться пустой(чистый)
        // TODO: Добавить Лодку. Для лодки отдельный таймер
        targetTimer = new QTimer(this);
        timer = new QTimer(this);

        perimeter = new PerimeterWidget(this);

        target = new RouteWidget(this);
        target->setColor(Qt::black);

        grid = new GridWidget(this);

        time = new TimeWidget(this);
        time->move(0, 20);
        time->show();

        connect(timer, &QTimer::timeout, this, &VisWidget::updateRoutes);
        connect(targetTimer, &QTimer::timeout, this, &VisWidget::updateTarget);

        connect(this, &VisWidget::sendMultiplier, time, &TimeWidget::updateTime);
    }

    bool rand = false;
signals:
    void sendMultiplier(double);
    void sendChangedDrawing(bool);
    void sendTargetPosition(QPointF);
    void sendTargetSpeed(double);
    void sendIntersectionResult(QString);

public slots:

    void movePoint()
    {
        random->movePoint(*perimeter, limits, targetParameters.getCurrentVelocity() * speedMultiplier);

        emit sendTargetPosition(random->getCurrentPosition());
        emit sendTargetSpeed(targetParameters.getCurrentVelocity());

        update();
    }

    void ResetPoint()
    {
        random->reset();
        rTimer->stop();
    }

    void startRandomTarget()
    {
        if (perimeterLoaded && routesLoaded)
        {
            random->setStartPoint(*perimeter, limits);
            rTimer->start(15);
        }
    }

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
        auto lims = limitsWithMargins(5);
        grid->draw(painter, lims);

        // Отрисовка Периметра
        perimeter->draw(painter);

        // TODO: fun
        // if (rand)
        random->draw(painter);

        drawRoutes(painter);

        drawTarget(painter);

        drawMultiplier(painter);
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
            auto lims = limitsWithMargins(5);

            initCoordinateSystem(lims);

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

        // TODO: Все состояния на отрисовку текущей точки
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

    void showCompletedRoutes()
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
        //TODO: fun
        ResetPoint();

        target->reset();

        targetPath.clear();

        targetTimer->stop();

        update();
    }

    void setRoutes(const QVector<Route>& routes_)
    {
        routes = QVector<RouteWidget*>();

        for (const auto& route_ : routes_)
        {
            auto route = new RouteWidget(this); // TODO

            route->initialize(route_.getSegments(), palette[routes.size() % palette.size()], shipParameters);

            this->routes.append(route);
        }

        routesLoaded = true;

        resetRoutesToDefault();

        initLimitsFromRoutes();

        // Инициализация движущихся Объектов.
        setRoutesModels(Visual::Objects::Arrow, 0.01 * limits.getMaxBorderLength()); // 1%

        loadFirstScreen();
    }

    void setRoutesModels(const Visual::Objects model, const double size) const
    {
        //
        for (size_t i = 0; i != routes.size(); ++i)
        {
            routes[i]->setModel(model, size);
        }

        // ПЛ
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

    void initCoordinateSystem(const Limits& lims)
    {
        cs = CoordinateSystem(/*limits*/);

        qDebug() << "X: " << lims.minX << "-" << lims.maxX << ", Y: " << lims.minY << "-" << lims.maxY;
        qDebug() << "X: " << limits.minX << "-" << limits.maxX << ", Y: " << limits.minY << "-" << limits.maxY;

        cs.setTransform(rect(), lims);
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
        for (const auto& route : routes)
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
        targetParameters.setCurrentVelocity(value);
    }

    void resetSpeed()
    {
        speedMultiplier = 1.0;
    }

public:
    // Controls
    void resizeEvent(QResizeEvent* event) override
    {
        auto lims = limitsWithMargins(5);
        // qDebug() << "X: " << lims.minX << "-" << lims.maxX << ", Y: " << lims.minY << "-" << lims.maxY;
        // qDebug() << "X: " << limits.minX << "-" << limits.maxX << ", Y: " << limits.minY << "-" << limits.maxY;
        cs.setTransform(rect(), lims);

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

            // Отрисовка
            update();
        }
    }

    void mouseReleaseEvent(QMouseEvent* event) override
    {
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
                // TODO: Требуется проверка что все доехали, а не один как сейчас
                timer->stop();
            }
        }

        // TODO: Время только первого маршрута
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

        shipParameters = JsonHelpers::parseShip(obj);

        targetParameters = JsonHelpers::parseTarget(obj);

        setPerimeter(perimeter);
    }

private:
    // TODO: Для теста

    Visual::RandomTargetWidget* random;
    QTimer* rTimer;
    // TODO: при добавлении setColor()
    // Цвета для БЭНКов
    QVector<QColor> palette = {Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::cyan, Qt::magenta};

    // Начальные данные из входных данных
    Object shipParameters;
    Object targetParameters;

    // TODO: Добавить State и Momento
    // Время
    TimeWidget* time;

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

    int speedMultiplier; // Множитель скорости

    Limits limits;

    // Flags
    bool found = false; // Объект найден

    bool perimeterLoaded = false; // Периметер загружен
    bool routesLoaded = false;    // Пути загружены

    bool drawing = false; // Разрешить отрисовку цели

    bool paused = true; // Общая пауза
    bool pausedTarget = false;
};
