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

public:
    SegmentWidget(QWidget* parent = nullptr, const Segment& segment = {})
        : QWidget(parent),
          segment{segment},
          current{segment.getStart()}
    {
        this->segment = segment;
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

        painter.drawLine(segment.getStart(), current);
    }

    void drawFull(QPainter& painter, const QColor& color) const
    {
        setPen(painter, color);

        painter.drawLine(segment.getStart(), segment.getEnd());
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

    Segment getSegment() const
    {
        return segment;
    }

private:
    void setPen(QPainter& painter, const QColor& color) const
    {
        QPen pen = QPen(color, 2);
        pen.setCosmetic(true);

        painter.setPen(pen);
    }

private:
    Segment segment;

    QPointF current;
};
} // namespace Visual
