#pragma once

#include <QColor>
#include <QDebug>
#include <QPainter>
#include <QPointF>
#include <QWidget>

#include <cmath>

#include "Models/Segment.hpp"

namespace Visual {
class SegmentWidget : public QWidget
{
    using Segment = Visual::Models::Segment;

    QPointF start;
    QPointF end;

    QColor color;
    QPointF current;
    double baseSpeed;

    // TODO: без булевой как будто не избежать отрисовки последнего сегмента пути

public:
    SegmentWidget(QWidget* parent = nullptr, const Segment& segment = {})
        : QWidget(parent), start{segment.getStart()}, end{segment.getEnd()}, current{segment.getStart()}, baseSpeed{segment.getSpeed()}
    {
    }

    void initialize(const Segment& segment)
    {
        start = segment.getStart();
        end = segment.getEnd();

        current = segment.getStart();
        baseSpeed = segment.getSpeed();
    }

    void draw(QPainter& painter, const QColor& color, const double size = 10) const
    {
        setPen(painter, color);
        // TODO: если полностью пройден то current == end
        painter.drawLine(start, current);

        if (end == current)
            drawArrow(painter, color, size);
    }

    void drawArrow(QPainter& painter, const QColor& color, const double size) const
    {
        // Отрисовка стрелки
        double angle = std::atan2(end.y() - start.y(), end.x() - start.x());

        QPolygonF arrow;

        arrow << QPointF(0, 0)
              << QPointF(-size, size / 2)
              << QPointF(-size, -size / 2);

        QTransform transform; // перенос в точку
        transform.translate(end.x(), end.y());
        transform.rotateRadians(angle); // повернуть на угол

        arrow = transform.map(arrow);

        // Заполнить полигон
        painter.setBrush(color);
        painter.drawPolygon(arrow);
    }

    QColor getColor() const
    {
        return color;
    }

    void setColor(const QColor& color)
    {
        this->color = color;
    }

    double getSpeed() const
    {
        return baseSpeed;
    }

    QPointF getEnd() const
    {
        return end;
    }

    QPointF getStart() const
    {
        return start;
    }

    void show() const
    {
        qDebug() << "start: [" << start.x() << ", " << start.y() << "], end: [" << end.x() << ", " << end.y() << "], speed: " << baseSpeed;
    }

    double length() const
    {
        return std::hypot(end.x() - start.x(), end.y() - start.y());
    }

    double currentLength() const
    {
        return std::hypot(current.x() - start.x(), current.y() - start.y());
    }

    void setCurrentPoint(const QPointF& point)
    {
        current = point;
    }

    QPointF getCurrentPoint() const
    {
        return current;
    }

    double getCurrentTime() const
    {
        return currentLength() / baseSpeed;
    }

private:
    void setPen(QPainter& painter, const QColor& color) const
    {
        QPen pen = QPen(color, 2);
        pen.setCosmetic(true);

        painter.setPen(pen);
    }
};
} // namespace Visual
