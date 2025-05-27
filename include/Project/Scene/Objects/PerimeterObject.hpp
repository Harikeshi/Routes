#pragma once

#include <QDebug>
#include <QPainter>
#include <QVector2D>
#include <QWidget>

#include "../Entities/Limits.hpp"
#include "./SegmentObject.hpp"
#include "./Models/Perimeter.hpp"

namespace Scene {
namespace Objects {
class PerimeterObject : public QObject
{
    using Perimeter = Models::Perimeter;

    Q_OBJECT

public:
    PerimeterObject(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

public:
    void show() const
    {
        perimeter.show();
    }

    QColor getColor() const
    {
        return color;
    }

    void setColor(const QColor& c)
    {
        color = c;
    }

    QVector<QPolygonF> getRings() const
    {
        return perimeter.getRings();
    }

    void setPerimeter(const Perimeter& perimeter)
    {
        this->perimeter = perimeter;
    }

    Perimeter getPerimeter() const
    {
        return perimeter;
    }

    void draw(QPainter& painter) const
    {
        QPen pen(color, 5); // Толщина
        pen.setCosmetic(true);

        painter.setPen(pen);

        for (const auto& ring : perimeter.getRings())
        {
            painter.drawPolygon(ring);
        }
    }

    void swapCoordinates()
    {
        this->perimeter.swapCoordinates();
    }

private:
    QColor color{Qt::blue};

    Perimeter perimeter;
};
} // namespace Objects
} // namespace Scene