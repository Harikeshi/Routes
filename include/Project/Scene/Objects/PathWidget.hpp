#pragma once

#include <Project/Scene/Entities/CoordinateSystem.hpp>
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QRandomGenerator>
#include <QTextStream>
#include <QToolTip>
#include <QWidget>

namespace Scene::Objects {

enum TargetPathDrawType
{
    DrawAsLinesWithPoints,
    DrawWithoutTurns
};

class TargetPathDrawState
{
    TargetPathDrawType state;

public:
    TargetPathDrawState(const TargetPathDrawType& state)
        : state{state}
    {
    }

    void drawLines(QPainter& painter, const QVector<QPointF>& path) const
    {
        QPen pen = QPen(Qt::darkYellow, 2);
        pen.setStyle(Qt::DotLine);
        pen.setCosmetic(true);

        painter.setPen(pen);

        for (int i = 0; i < path.size() - 1; ++i)
        {
            painter.drawLine(path[i], path[i + 1]);
        }
    }

    TargetPathDrawType type() const
    {
        return state;
    }

    virtual void draw(QPainter& painter, const QVector<QPointF>& path) = 0;
    virtual ~TargetPathDrawState() = default;
};

// Без отрисовки nullptr
class DrawAsLinesWithPointsState : public TargetPathDrawState
{
public:
    DrawAsLinesWithPointsState()
        : TargetPathDrawState(TargetPathDrawType::DrawAsLinesWithPoints)
    {
    }

    void draw(QPainter& painter, const QVector<QPointF>& path) override
    {
        painter.save();

        // Рисуем все точки
        painter.setPen(Qt::blue);
        painter.setBrush(Qt::blue);

        for (const QPointF& point : path)
        {
            // todo: Draw Point
            painter.drawEllipse(point, 3, 3);
        }

        // Отрисовка линий
        if (path.size() > 1)
        {
            drawLines(painter, path);
        }

        painter.restore();
    }
};

class DrawWithoutTurnsState : public TargetPathDrawState
{
public:
    DrawWithoutTurnsState()
        : TargetPathDrawState(TargetPathDrawType::DrawWithoutTurns)
    {
    }

    void draw(QPainter& painter, const QVector<QPointF>& path) override
    {
        painter.save();

        drawLines(painter, path);

        painter.restore();
    }
};

class PathDraw
{
    std::map<TargetPathDrawType, std::function<void(QPainter&)>> funcs;

public:
    PathDraw()
    {
        funcs[TargetPathDrawType::DrawAsLinesWithPoints] = [](QPainter& painter) {};
        funcs[TargetPathDrawType::DrawWithoutTurns] = [](QPainter& painter) {};
    }

    void draw()
    {
    }
};

class PointObject : public QWidget
{
    Q_OBJECT
public:
    PointObject(const QPointF& _point, double _radius, QWidget* parent = nullptr)
        : point(_point), radius(_radius), QWidget(parent)
    {
        // setMouseTracking(true);
    }
    void paintEvent(QPaintEvent* event) override
    {
        Q_UNUSED(event);
        QPainter painter(this);

        painter.setPen(Qt::NoPen);
        painter.setBrush(color);
        painter.drawEllipse(point, radius, radius);

        painter.restore();
    }

    void draw(QPainter& painter)
    {
        painter.save();

        QColor fillColor = isHovered ? Qt::green : Qt::black;

        painter.setPen(Qt::NoPen);
        painter.setBrush(fillColor);
        painter.drawEllipse(point, radius, radius);

        painter.restore();
    }

    void mousePressEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            QString tooltipText = QString(" X: %1, Y: %2 ").arg(point.x()).arg(point.y());
            QToolTip::showText(event->globalPos(), tooltipText, this);
        }

        update();
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        if (point == event->pos())
        {
            isHovered = true;
        }
        update();
    }

private:
    QColor color = Qt::black; // Цвет заливки по дефолту.

    bool isHovered = false;
    QPointF point;
    int hoveredPoint = -1;
    qreal radius;
};

enum DrawType
{
    OnlyLines,
    OnlyPoints,
    LinesAndPoints
};

enum AddType
{
    BySomeLines,
    Continuous
};

class PathWidget final : public QWidget
{
    Q_OBJECT

    int hoveredPoint = -1;
    double radius = 0; // Радиус точки.

    QVector<QPointF> path = QVector<QPointF>{};

    bool drawing = true;
    bool drawing_lines = true;
    bool drawing_points = true;

public:
    explicit PathWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        // setMouseTracking(true);              // Включаем отслеживание движения мыши
        setAttribute(Qt::WA_StaticContents);
    }

    bool hasDrawingPoints() const
    {
        return drawing_points;
    }

    bool hasDrawingLines() const
    {
        return drawing_lines;
    }

    void setRadius(double _radius)
    {
        radius = _radius;
    }

    QVector<QPointF> getPath() const
    {
        return path;
    }

    void addPoint(const QPointF& point)
    {
        path.append(point);
    }

    bool isEmpty() const
    {
        return path.isEmpty();
    }

    size_t size() const
    {
        return path.size();
    }

    void draw_lines(QPainter& painter)
    {
        painter.setRenderHint(QPainter::Antialiasing);

        QPen pen = QPen(Qt::darkYellow, 2);
        pen.setStyle(Qt::DotLine);
        pen.setCosmetic(true);

        painter.setPen(pen);

        for (int i = 0; i < path.size() - 1; ++i)
        {
            painter.drawLine(path[i], path[i + 1]);
        }
    }

    void reset()
    {
        hoveredPoint = -1;
        radius = 0; // Радиус точки.

        path.clear();

        drawing = true;
        drawing_lines = true;
        drawing_points = true;
    }

    void clear()
    {
        path.clear();
        //
        //        drawing = true;
        //        drawing_lines = true;
        //        drawing_points = true;
    }

    // TODO: Полная отрисовка, Отрисовка до currentIndex
    void draw_points(QPainter& painter)
    {
        for (int i = 0; i < path.size(); ++i)
        {
            const QPointF& point = path[i];

            QColor fillColor = (i == hoveredPoint) ? Qt::green : Qt::black;

            painter.setPen(Qt::NoPen);
            painter.setBrush(fillColor);
            painter.drawEllipse(point, radius, radius);
        }
    }

    // map отрисовка через line/point - line
    void draw(QPainter& painter)
    {
        painter.save();

        if (drawing_lines)
            draw_lines(painter);

        if (drawing_points)
            draw_points(painter);

        painter.restore();
    }

protected:
    // TODO: Управления тут не должно быть
    void mouseReleaseEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            //            if (drawing)
            //            {
            //                emit return target;targets->setRoute();

            update();

            // TODO:RESET
            //            targets->clear();
            //            targets->setModel(limits);
            //
            //            emit sendTargetPosition(targets->getCurrentPosition());
            //            emit sendTargetSpeed(targets->getSpeed());
        }

        update();
    }

public:
    void mousePress(QMouseEvent* event, const QPointF& point)
    {
        int pointIndex = findPointAt(point);
        if (pointIndex != -1)
        {
            // selectedPoint = pointIndex;
            update();

            QString tooltipText = QString("Point %1 :[ X: %2, Y: %3 ]").arg(pointIndex + 1).arg(path[pointIndex].x()).arg(path[pointIndex].y());
            QToolTip::showText(event->globalPos(), tooltipText, this);
        }
    }

public:
    // void mouseMove(const QPointF& point)
    // {
    //     int oldHovered = hoveredPoint;
    //     hoveredPoint = findPointAt(point);
    //
    //     if (hoveredPoint != oldHovered)
    //     {
    //         update();
    //     }
    // }

    // void keyPressEvent(QKeyEvent* event) override
    // {
    //     if (event->key() == Qt::Key_S)
    //     {
    //         //
    //         //setDrawing(!drawing);
    //     }
    //
    //     update();
    // }

private:
    int findPointAt(const QPointF& pos)
    {
        for (int i = 0; i < path.size(); ++i)
        {
            int dx = pos.x() - path[i].x();
            int dy = pos.y() - path[i].y();

            if (dx * dx + dy * dy <= (radius + radius * 0.05) * (radius + radius * 0.05))
            {
                return i;
            }
        }
        return -1;
    }

    void setTargetPath(const QVector<QPointF>& path)
    {
        this->path = path;
    }

public:
    void swapCoordinates()
    {
        for (size_t i = 0; i != path.size(); ++i)
        {
            auto x = path[i].x();
            path[i].setX(path[i].y());
            path[i].setY(x);
        }
    }
};
} // namespace Scene::Objects
