#pragma once

#include "Project/Models/Segment.hpp"
#include <QPainter>
#include <QPainterPath>
#include <QPointF>
#include <QWidget>

namespace Scene::Objects {
/*!
     * Класс отображения отрезка пути.
     */
class SegmentObject final : public QObject
{
    using Segment = Models::Segment;

public:
    explicit SegmentObject(QObject* parent = nullptr, const Segment& segment = {})
        : QObject(parent),
          segment{segment},
          current{segment.getStart()}
    {
        this->segment = segment;
    }

    explicit SegmentObject(const QPointF& start, const QPointF& end, double speed, QObject* parent = nullptr)
        : segment{0, start, end, speed},
          current{start}, QObject(parent)
    {
    }

    void clear()
    {
        current = segment.getStart();
    }

    void initialize(const Segment& segment)
    {
        this->segment = segment;
        current = segment.getStart();
    }

    void drawCurrent(QPainter& painter, const QColor& color) const
    {
        setPen(painter, color);
        painter.drawLine(segment.start, current);
    }

    void drawFull(QPainter& painter, const QColor& color) const
    {
        setPen(painter, color);
        painter.drawLine(segment.start, segment.end);
    }

    double getSpeed() const
    {
        return segment.getSpeed();
    }

    QPointF getEnd() const
    {
        return segment.getEnd();
    }

    QPointF getStart() const
    {
        return segment.getStart();
    }

    void show() const
    {
        segment.show();
    }

    double length() const
    {
        return segment.length();
    }

    double currentLength() const
    {
        return std::hypot(current.x() - segment.getStart().x(), current.y() - segment.getStart().y());
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
        return currentLength() / segment.getSpeed();
    }

    double getFullTime() const
    {
        return segment.time();
    }

    Segment getSegment() const
    {
        return segment;
    }

    void setCurrent(const QPoint& curr)
    {
        current = curr;
    }

    void setSegment(const Segment& seg)
    {
        segment = seg;
    }

private:
    void setPen(QPainter& painter, const QColor& color) const
    {
        QPen pen = QPen(color, 2);
        pen.setCosmetic(true);

        painter.setPen(pen);
    }

public:
    void swapCoordinates()
    {
        segment.swapCoordinates();
        auto x = current.x();

        current.setX(current.y());
        current.setY(x);
    }

private:
    Segment segment;

    QPointF current;
};
} // namespace Scene::Objects
